/**
 *  @file oglplus/client/object_binding.hpp
 *  @brief Client current object stack
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2015 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#pragma once
#ifndef OGLPLUS_CLIENT_OBJECT_BINDING_1412071213_HPP
#define OGLPLUS_CLIENT_OBJECT_BINDING_1412071213_HPP

#include <oglplus/glfunc.hpp>
#include <oglplus/client/setting.hpp>
#include <oglplus/utils/nothing.hpp>
#include <oglplus/bound/buffer.hpp>
#include <oglplus/bound/framebuffer.hpp>
#include <oglplus/bound/renderbuffer.hpp>
#include <oglplus/bound/texture.hpp>
#include <oglplus/texture_unit.hpp>

namespace oglplus {
namespace client {
namespace aux {

// CurrentObject
template <typename ObjTag>
struct CurrentObject
{
	template <typename ObjBindingOps<ObjTag>::Target ObjTgt>
	class WithTarget
	 : public SettingStack<GLuint, Nothing>
	 , public BoundObjOps<ObjTag>
	{
	private:
		static
		GLuint _do_get(Nothing)
		{
			return GetGLName(ObjBindingOps<ObjTag>::Binding(ObjTgt));
		}

		static
		void _do_bind(GLuint obj, Nothing)
		{
			ObjBindingOps<ObjTag>::Bind(
				ObjTgt,
				ObjectName<ObjTag>(obj)
			);
		}
	public:
		WithTarget(void)
		 : SettingStack<GLuint, Nothing>(&_do_get, &_do_bind)
		 , BoundObjOps<ObjTag>(ObjTgt)
		{ }

		ObjectName<ObjTag> Get(void) const
		OGLPLUS_NOEXCEPT(true)
		{
			return ObjectName<ObjTag>(this->_top());
		}

		operator ObjectName<ObjTag> (void) const
		OGLPLUS_NOEXCEPT(true)
		{
			return Get();
		}

		typedef SettingHolder<GLuint, Nothing> Holder;

		Holder Push(ObjectName<ObjTag> obj)
		{
			return this->_push(GetName(obj));
		}

		void Bind(ObjectName<ObjTag> obj)
		{
			return this->_set(GetName(obj));
		}
	};
};

// CurrentObjectsWithTarget
template <typename ObjTag>
class CurrentObjectsWithTarget
 : public oglplus::enums::EnumToClass<
	Nothing,
	typename ObjBindingOps<ObjTag>::Target,
	CurrentObject<ObjTag>::template WithTarget
>
{
public:
	CurrentObjectsWithTarget(void) { }
};

// CurrentObjectWithoutTarget
template <typename ObjTag>
class CurrentObjectWithoutTarget
 : public SettingStack<GLuint, Nothing>
{
private:
	static
	GLuint _do_get(Nothing)
	{
		return GetGLName(ObjBindingOps<ObjTag>::Binding());
	}

	static
	void _do_bind(GLuint obj, Nothing)
	{
		ObjBindingOps<ObjTag>::Bind(
			ObjectName<ObjTag>(obj)
		);
	}
public:
	CurrentObjectWithoutTarget(void)
	 : SettingStack<GLuint, Nothing>(&_do_get, &_do_bind)
	{ }

	ObjectName<ObjTag> Get(void) const
	OGLPLUS_NOEXCEPT(true)
	{
		return ObjectName<ObjTag>(this->_top());
	}

	operator ObjectName<ObjTag> (void) const
	OGLPLUS_NOEXCEPT(true)
	{
		return Get();
	}

	typedef SettingHolder<GLuint, Nothing> Holder;

	Holder Push(ObjectName<ObjTag> obj)
	{
		return this->_push(GetName(obj));
	}

	void Bind(ObjectName<ObjTag> obj)
	{
		this->_set(GetName(obj));
	}
};

// CurrentUnitTexture
template <TextureTarget ObjTgt>
class CurrentUnitTexture
 : public SettingStack<GLuint, GLuint>
{
private:
	typedef tag::Texture ObjTag;

	static
	void _active_tex(GLuint tex_unit)
	{
		OGLPLUS_GLFUNC(ActiveTexture)(
			GLenum(GL_TEXTURE0 + tex_unit)
		);
		OGLPLUS_VERIFY(
			ActiveTexture,
			Error,
			Index(tex_unit)
		);
	}

	static
	GLuint _do_get(GLuint tex_unit)
	{
		_active_tex(tex_unit);
		return GetGLName(ObjBindingOps<ObjTag>::Binding(ObjTgt));
	}


	static
	void _do_bind(GLuint obj, GLuint tex_unit)
	{
		_active_tex(tex_unit);
		ObjBindingOps<ObjTag>::Bind(
			ObjTgt,
			ObjectName<ObjTag>(obj)
		);
	}
public:
	CurrentUnitTexture(TextureUnitSelector tex_unit)
	 : SettingStack<GLuint, GLuint>(&_do_get, &_do_bind, GLuint(tex_unit))
	{ }

	ObjectName<ObjTag> Get(void) const
	OGLPLUS_NOEXCEPT(true)
	{
		return ObjectName<ObjTag>(this->_top());
	}

	operator ObjectName<ObjTag> (void) const
	OGLPLUS_NOEXCEPT(true)
	{
		return Get();
	}

	typedef SettingHolder<GLuint, GLuint> Holder;

	Holder Push(ObjectName<ObjTag> obj)
	{
		return this->_push(GetName(obj));
	}

	void Bind(ObjectName<ObjTag> obj)
	{
		return this->_set(GetName(obj));
	}
};

// CurrentTextures
class CurrentTextures
{
private:
	typedef oglplus::enums::EnumToClass<
		TextureUnitSelector,
		TextureTarget,
		CurrentUnitTexture
	> CurrentUnitTextures;

	std::vector<CurrentUnitTextures> _units;
public:
	CurrentTextures(void) { }

	CurrentUnitTextures& Unit(std::size_t index)
	{
		for(std::size_t i=_units.size(); i<=index; ++i)
		{
			_units.emplace_back(GLuint(i));
		}
		return _units[index];
	}

	CurrentUnitTextures& operator [] (std::size_t index)
	{
		return Unit(index);
	}
};

// CurrentUnitSampler
class CurrentUnitSampler
 : public SettingStack<GLuint, GLuint>
{
private:
	typedef tag::Sampler ObjTag;

	static
	GLuint _do_get(GLuint tex_unit)
	{
		return GetGLName(ObjBindingOps<ObjTag>::Binding(tex_unit));
	}

	static
	void _do_bind(GLuint obj, GLuint tex_unit)
	{
		ObjBindingOps<ObjTag>::Bind(
			tex_unit,
			ObjectName<ObjTag>(obj)
		);
	}
public:
	CurrentUnitSampler(TextureUnitSelector tex_unit)
	 : SettingStack<GLuint, GLuint>(&_do_get, &_do_bind, GLuint(tex_unit))
	{ }

	ObjectName<ObjTag> Get(void) const
	OGLPLUS_NOEXCEPT(true)
	{
		return ObjectName<ObjTag>(this->_get());
	}

	operator ObjectName<ObjTag> (void) const
	OGLPLUS_NOEXCEPT(true)
	{
		return Get();
	}

	typedef SettingHolder<GLuint, GLuint> Holder;

	Holder Push(ObjectName<ObjTag> obj)
	{
		return this->_push(GetName(obj));
	}

	void Bind(ObjectName<ObjTag> obj)
	{
		this->_set(GetName(obj));
	}
};

// CurrentSamplers
class CurrentSamplers
{
private:
	std::vector<CurrentUnitSampler> _units;
public:
	CurrentSamplers(void) { }

	CurrentUnitSampler& Unit(std::size_t index)
	{
		for(std::size_t i=_units.size(); i<=index; ++i)
		{
			_units.emplace_back(GLuint(i));
		}
		return _units[index];
	}

	CurrentUnitSampler& operator [] (std::size_t index)
	{
		return Unit(index);
	}
};

} // namespace aux

class CurrentObjects
{
public:
	aux::CurrentObjectsWithTarget<tag::Buffer> Buffer;
	aux::CurrentObjectsWithTarget<tag::Framebuffer> Framebuffer;
	aux::CurrentObjectsWithTarget<tag::Renderbuffer> Renderbuffer;
	aux::CurrentObjectsWithTarget<tag::TransformFeedback> TransformFeedback;

	aux::CurrentObjectWithoutTarget<tag::Program> Program;
	aux::CurrentObjectWithoutTarget<tag::ProgramPipeline> ProgramPipeline;
	aux::CurrentObjectWithoutTarget<tag::VertexArray> VertexArray;

	aux::CurrentTextures Texture;
	aux::CurrentSamplers Sampler;
};

} // namespace client
} // namespace oglplus

#endif // include guard
