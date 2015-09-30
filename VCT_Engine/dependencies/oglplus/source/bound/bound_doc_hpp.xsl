<!--
   - Copyright 2010-2015 Matus Chochlik. Distributed under the Boost
   - Software License, Version 1.0. (See accompanying file
   - LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
   -
   - XSLT template used for generating header files implementing
   - specializations of the Bound<Object> template.
  -->
<xsl:stylesheet
	version="1.0"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
>
	<xsl:output method="text"/>
	<xsl:strip-space elements="*"/>
	<xsl:preserve-space elements="xsl:text"/>

	<xsl:template name="Newline"><xsl:text>&#x0A;</xsl:text></xsl:template>

	<xsl:param name="object"/>
	<xsl:param name="year"/>

	<xsl:template name="Capitalize">
		<xsl:param name="String"/>
		<xsl:value-of select="translate(
			$String,
			'abcdefghijklmnopqrstuvwxyz',
			'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
		)"/>
	</xsl:template>

	<xsl:template name="CapitalizeFirst">
		<xsl:param name="String"/>
		<xsl:call-template name="Capitalize">
			<xsl:with-param name="String" select="substring($String, 1, 1)"/>
		</xsl:call-template>
		<xsl:value-of select="substring($String, 2)"/>
	</xsl:template>

	<xsl:variable name="Object">
		<xsl:call-template name="CapitalizeFirst">
			<xsl:with-param name="String" select="$object"/>
		</xsl:call-template>
	</xsl:variable>

	<xsl:variable name="OBJECT">
		<xsl:call-template name="Capitalize">
			<xsl:with-param name="String" select="$object"/>
		</xsl:call-template>
	</xsl:variable>

	<xsl:template match="*">
		<xsl:apply-templates/>
	</xsl:template>

	<xsl:template match="ref" mode="ParamTypeExpr">
		<xsl:variable name="RefId" select="@refid"/>
		<xsl:if test="../../../../../sectiondef/memberdef[@id = $RefId]">
			<xsl:text>ExplicitOps::</xsl:text>
		</xsl:if>
		<xsl:if test="../../../../../../compounddef[
			contains(compoundname, concat('tag::', $Object))
		]/sectiondef/memberdef[@id = $RefId]">
			<xsl:text>ExplicitOps::</xsl:text>
		</xsl:if>
		<xsl:apply-templates mode="ParamTypeExpr"/>
	</xsl:template>

	<xsl:template match="text()" mode="ParamTypeExpr">
		<xsl:variable name="IsSpecRef" select="substring(., string-length(.)-2) = '(&amp;)'"/>
		<xsl:choose>
			<xsl:when test="$IsSpecRef">
				<xsl:value-of select="substring-before(., '(&amp;)')"/>
			</xsl:when>
			<xsl:otherwise>
				<xsl:value-of select="."/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<xsl:template match="*" mode="ParamTypeExpr">
		<xsl:apply-templates mode="ParamTypeExpr"/>
	</xsl:template>

	<xsl:template match="text()" mode="ParamNameExpr">
		<xsl:variable name="Type" select="../../type/descendant-or-self::*/text()"/>
		<xsl:variable name="IsSpecRef" select="substring($Type, string-length($Type)-2) = '(&amp;)'"/>
		<xsl:if test="$IsSpecRef">(&amp;</xsl:if>
		<xsl:value-of select="."/>
		<xsl:if test="$IsSpecRef">)</xsl:if>
	</xsl:template>

	<xsl:template match="*" mode="ParamNameExpr">
		<xsl:apply-templates mode="ParamNameExpr"/>
	</xsl:template>

	<xsl:template name="GenerateFunction">
		<xsl:variable name="DetailedDescription">
			<xsl:for-each select="detaileddescription/descendant-or-self::node()">
				<xsl:value-of select="text()"/><xsl:text> </xsl:text>
			</xsl:for-each>
		</xsl:variable>
		<xsl:variable name="RequiredGLVersion">
			<xsl:if test="contains(
				$DetailedDescription,
				'_oglplus_req_gl_ver('
			)">
				<xsl:value-of select="substring-before(
					substring-after(
						$DetailedDescription,
						'_oglplus_req_gl_ver('
					), ')'
				)"/>
			</xsl:if>
		</xsl:variable>
		<xsl:variable name="RequiredGLExtension">
			<xsl:if test="contains(
				$DetailedDescription,
				'_oglplus_req_gl_ext('
			)">
				<xsl:value-of select="substring-before(
					substring-after(
						$DetailedDescription,
						'_oglplus_req_gl_ext('
					), ')'
				)"/>
			</xsl:if>
		</xsl:variable>
		<xsl:if test="
			(string-length($RequiredGLVersion) != 0) or
			(string-length($RequiredGLExtension) != 0)
		">
			<xsl:text>#if </xsl:text>
			<xsl:if test="string-length($RequiredGLVersion) != 0">
				<xsl:text>GL_VERSION_</xsl:text>
				<xsl:value-of select="$RequiredGLVersion"/>
			</xsl:if>
			<xsl:if test="string-length($RequiredGLExtension) != 0">
				<xsl:if test="string-length($RequiredGLVersion) != 0">
					<xsl:text> || </xsl:text>
				</xsl:if>
				<xsl:text>GL_</xsl:text>
				<xsl:value-of select="$RequiredGLExtension"/>
			</xsl:if>
			<xsl:call-template name="Newline"/>
		</xsl:if>
		<xsl:for-each select="templateparamlist">
			<xsl:text>	template &lt;</xsl:text>
			<xsl:for-each select="param">
				<xsl:value-of select="type/text()"/>
				<xsl:if test="declname">
					<xsl:text> </xsl:text>
					<xsl:value-of select="declname/text()"/>
				</xsl:if>
				<xsl:if test="type/ref">
					<xsl:value-of select="type/ref/text()"/>
				</xsl:if>
				<xsl:if test="position() != last()">, </xsl:if>
			</xsl:for-each>
			<xsl:text>&gt;</xsl:text>
			<xsl:call-template name="Newline"/>
		</xsl:for-each>
		<xsl:text>	</xsl:text>

		<xsl:variable name="ResultType">
			<xsl:apply-templates mode="ParamTypeExpr" select="type"/>
		</xsl:variable>

		<xsl:choose>
			<xsl:when test="$ResultType = 'void'">const BoundObjOps&amp;</xsl:when>
			<xsl:otherwise><xsl:value-of select="$ResultType"/></xsl:otherwise>
		</xsl:choose>

		<xsl:text> </xsl:text>
		<xsl:value-of select="name/text()"/>
		<xsl:text>(</xsl:text>

		<xsl:variable
			name="HasParams"
			select="count(param[type/descendant-or-self::text() != 'Target']) != 0"
		/>

		<xsl:choose>
			<xsl:when test="$HasParams">
				<xsl:call-template name="Newline"/>
			</xsl:when>
			<xsl:otherwise>
				<xsl:text>void</xsl:text>
			</xsl:otherwise>
		</xsl:choose>
		
		<xsl:for-each select="param">
			<xsl:if test="type/descendant-or-self::text() != 'Target'">
				<xsl:text>		</xsl:text>
				<xsl:apply-templates mode="ParamTypeExpr" select="type"/>
				<xsl:if test="declname/text() != ''">
					<xsl:text> </xsl:text>
					<xsl:apply-templates mode="ParamNameExpr" select="declname"/>
				</xsl:if>
				<xsl:if test="array">
					<xsl:value-of select="array/text()"/>
				</xsl:if>
				<xsl:if test="defval">
					<xsl:text> = </xsl:text>
					<xsl:apply-templates mode="ParamTypeExpr" select="defval"/>
				</xsl:if>
				<xsl:if test="position() != last()">,</xsl:if>
				<xsl:call-template name="Newline"/>
			</xsl:if>
		</xsl:for-each>

		<xsl:if test="$HasParams">
			<xsl:text>	</xsl:text>
		</xsl:if>
		<xsl:text>) const;</xsl:text>
		<xsl:call-template name="Newline"/>

		<xsl:if test="
			(string-length($RequiredGLVersion) != 0) or
			(string-length($RequiredGLExtension) != 0)
		">
			<xsl:text>#endif</xsl:text>
			<xsl:call-template name="Newline"/>
		</xsl:if>
	</xsl:template>

	<xsl:template name="GenerateFile">
<xsl:text>
/*
 *  Automatically generated file, do not edit manually!
 *
 *  Copyright 2010-</xsl:text>
	<xsl:value-of select="$year"/>
<xsl:text> Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
//[oglplus_object_BoundObjOps_</xsl:text><xsl:value-of select="$Object"/><xsl:text>

template &lt;&gt;
class __BoundObjOps&lt;__tag_</xsl:text><xsl:value-of select="$Object"/><xsl:text>&gt;
{
private:
	typedef typename __ObjectOps_Explicit_</xsl:text><xsl:value-of select="$Object"/>
	<xsl:text>&lt;__tag_ExplicitSel, __tag_</xsl:text><xsl:value-of select="$Object"/>
	<xsl:text>&gt; ExplicitOps;
public:
	typedef typename ExplicitOps::Target Target;

	Target target;

	BoundObjOps(void);

	BoundObjOps(Target init_tgt);
</xsl:text>
	<xsl:variable name="ObjZeroOps" select="concat('oglplus::ObjZeroOps&lt; tag::ExplicitSel, tag::', $Object,' &gt;')"/>
	<xsl:variable name="ObjectOps"  select="concat('oglplus::ObjectOps&lt; tag::ExplicitSel, tag::', $Object,' &gt;')"/>
	<xsl:for-each select="compounddef">
		<xsl:if test="compoundname/text()=$ObjZeroOps or compoundname/text()=$ObjectOps">
			<xsl:for-each select="sectiondef/memberdef[
				@kind='function' and
				@static='yes' and
				@prot='public' and
				param/declname/text()='target' and
				(param/type/text()='Target' or param/type/ref/text()='Target') and
				name/text() != 'Bind' and
				name/text() != 'Unbind'
			]">
				<xsl:call-template name="GenerateFunction"/>
				<xsl:call-template name="Newline"/>
			</xsl:for-each>
		</xsl:if>
	</xsl:for-each>
<xsl:text>
};
//]
</xsl:text>
	</xsl:template>

	<xsl:template match="/doxygen">
		<xsl:call-template name="GenerateFile"/>
	</xsl:template>
</xsl:stylesheet>
