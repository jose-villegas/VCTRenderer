/**
 *  @file oglplus/x11/display.hpp
 *  @brief Wrapper for X11/Display class
 *
 *  @author Matus Chochlik
 *
 *  Copyright 2010-2013 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef UTILS_OGLPLUS_X11_DISPLAY_1107121519_HPP
#define UTILS_OGLPLUS_X11_DISPLAY_1107121519_HPP

#include <X11/Xlib.h>
#include <stdexcept>
#include <vector>
#include <string>
#include <cstdio>
#include <cassert>

namespace oglplus {
namespace x11 {

template <typename ObjectType, typename Deleter = int(ObjectType*)>
class Object
{
private:
	ObjectType* _pimpl;

	Deleter* _deleter;
protected:
	Object(
		ObjectType* pimpl,
		Deleter* deleter,
		const char* error_message
	): _pimpl(pimpl)
	 , _deleter(deleter)
	{
		assert(_deleter);
		if(!_pimpl)
		{
			throw std::runtime_error(error_message);
		}
	}
public:
	Object(const Object&) = delete;
	Object(Object&& temp)
	 : _pimpl(temp._pimpl)
	 , _deleter(temp._deleter)
	{
		temp._pimpl= 0;
	}

	~Object(void)
	{
		if(_pimpl) _deleter(_pimpl);
	}

	ObjectType* Get(void) const
	{
		assert(_pimpl);
		return _pimpl;
	}

	operator ObjectType* (void) const
	{
		return Get();
	}

	ObjectType* operator -> (void) const
	{
		return Get();
	}
};

class Display
 : public Object< ::Display>
{
private:
	static Bool _any_event(::Display*, ::XEvent*, ::XPointer)
	{
		return True;
	}
public:
	Display(const char* name = 0)
	 : Object< ::Display>(
		::XOpenDisplay(name),
		::XCloseDisplay,
		"Error opening X Display"
	){ }

	bool NextEvent(XEvent& event) const
	{
		return ::XCheckIfEvent(
			this->Get(),
			&event,
			&_any_event,
			::XPointer()
		) == True;
	}
};

class ScreenNames
 : public std::vector<std::string>
{
public:
	ScreenNames(void)
	{
		char name[16];
		int display = 0;
		while(true)
		{
			int screen = 0;
			while(true)
			{
				std::snprintf(
					name,
					sizeof(name)/sizeof(name[0]),
					":%d.%d",
					display,
					screen
				);
				::Display* tmp = ::XOpenDisplay(name);
				if(tmp)
				{
					push_back(name);
					::XCloseDisplay(tmp);
				}
				else if(screen != 0) break;
				else return;
				++screen;
			}
			++display;
		}
	}
};

template <typename HandleType, typename Deleter = int(::Display*, HandleType)>
class DisplayObject
{
private:
	const Display& _display;
	HandleType _handle;

	Deleter* _deleter;
protected:

	const Display& DisplayRef(void) const
	{
		return _display;
	}

	DisplayObject(
		const Display& display,
		HandleType handle,
		Deleter* deleter,
		const char* error_message
	): _display(display)
	 , _handle(handle)
	 , _deleter(deleter)
	{
		assert(_deleter);
		if(!_handle)
		{
			throw std::runtime_error(error_message);
		}
	}
public:
	DisplayObject(const DisplayObject&) = delete;

	DisplayObject(DisplayObject&& temp)
	 : _display(temp._display)
	 , _handle(temp._handle)
	 , _deleter(temp._deleter)
	{
		temp._handle = 0;
	}

	~DisplayObject(void)
	{
		if(_handle)
		{
			_deleter(_display, _handle);
		}
	}

	HandleType Handle(void) const
	{
		return _handle;
	}

	operator HandleType(void) const
	{
		return Handle();
	}
};

template <typename HandleType>
class BaseDisplayObject
{
private:
	HandleType _handle;
public:
	template <typename Derived, typename Deleter>
	BaseDisplayObject(const DisplayObject<Derived, Deleter>& derived)
	 : _handle(derived.Handle())
	{ }

	HandleType Handle(void) const
	{
		return _handle;
	}

	operator HandleType(void) const
	{
		return Handle();
	}
};

} // namespace x11
} // namespace oglplus

#endif // include guard
