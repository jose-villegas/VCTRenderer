<!--
   - Copyright 2014 Matus Chochlik. Distributed under the Boost
   - Software License, Version 1.0. (See accompanying file
   - LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
   -
   - This XSL template generates the config/gl/assert_GL_VER_symbols.ipp file
   - that is used to detect the supported GL version.
   - This template is intended to be used with the gl.xml file from the OpenGL
   - XML registry.
  -->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
	<xsl:output method="text"/>
	<xsl:strip-space elements="*"/>
	<xsl:preserve-space elements="xsl:text"/>

	<xsl:param name="year"/>

	<xsl:template name="Newline"><xsl:text>&#x0A;</xsl:text></xsl:template>

	<xsl:template match="*">
	</xsl:template>

	<xsl:template match="/registry">
		
<xsl:text>/**
 *  .file config/gl/assert_GL_VER_symbols.ipp.in
 *  .brief Helper file used by GL version detection.
 *
 *  Automatically generated file, do not edit manually!
 *
 *  Copyright </xsl:text><xsl:value-of select="$year"/>
<xsl:text> Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */</xsl:text>
		<xsl:call-template name="Newline"/>

		<xsl:apply-templates select="feature"/>
	</xsl:template>

	<xsl:template match="/registry/feature[@api='gl' and @number>=3.0]">
		<xsl:variable name="version_major" select="substring-before(@number,'.')"/>
		<xsl:variable name="version_minor" select="substring-after(@number,'.')"/>
		<xsl:text>#if (</xsl:text>
		<xsl:value-of select="@name"/>
		<xsl:text>) &amp;&amp; (OGLPLUS_CONFIG_QUERY_GL_VER_MAJOR >= </xsl:text>
		<xsl:value-of select="$version_major"/>
		<xsl:text>) &amp;&amp; (OGLPLUS_CONFIG_QUERY_GL_VER_MINOR >= </xsl:text>
		<xsl:value-of select="$version_minor"/>
		<xsl:text>)</xsl:text>
		<xsl:call-template name="Newline"/>

		<xsl:text>if(req_major >= </xsl:text>
		<xsl:value-of select="$version_major"/>
		<xsl:text> &amp;&amp; req_minor >= </xsl:text>
		<xsl:value-of select="$version_minor"/>
		<xsl:text>)</xsl:text>
		<xsl:call-template name="Newline"/>

		<xsl:text>{</xsl:text>
		<xsl:call-template name="Newline"/>

		<xsl:apply-templates select="require/command"/>
		<xsl:call-template name="Newline"/>
		<xsl:apply-templates select="require/enum"/>

		<xsl:text>}</xsl:text>
		<xsl:call-template name="Newline"/>
		<xsl:text>#endif // </xsl:text>
		<xsl:value-of select="@name"/>
		<xsl:call-template name="Newline"/>

	</xsl:template>

	<xsl:template match="/registry/feature/require[not(@profile = 'compatibility')]/command">

		<xsl:variable name="name" select="@name"/>
		<xsl:for-each select="/registry/feature[remove[@profile = 'core']/command/@name = $name]">
			<xsl:variable name="rem_ver_major" select="substring-before(@number,'.')"/>
			<xsl:variable name="rem_ver_minor" select="substring-after(@number,'.')"/>
			<xsl:text>#	if not((OGLPLUS_CONFIG_QUERY_GL_VER_MAJOR >= </xsl:text>
			<xsl:value-of select="$rem_ver_major"/>
			<xsl:text>) &amp;&amp; (OGLPLUS_CONFIG_QUERY_GL_VER_MINOR >= </xsl:text>
			<xsl:value-of select="$rem_ver_minor"/>
			<xsl:text>))</xsl:text>
			<xsl:call-template name="Newline"/>
		</xsl:for-each>

		<xsl:text>	OGLPLUS_CONFIG_ASSERT_GL_FUNC(</xsl:text>
		<xsl:value-of select="@name"/>
		<xsl:text>);</xsl:text>
		<xsl:call-template name="Newline"/>

		<xsl:if test="/registry/feature/remove[@profile = 'core']/command/@name = $name">
			<xsl:text>#	endif</xsl:text>
			<xsl:call-template name="Newline"/>
		</xsl:if>
	</xsl:template>

	<xsl:template match="/registry/feature/require[not(@profile = 'compatibility')]/enum">
		<xsl:text>#	ifndef </xsl:text>
		<xsl:value-of select="@name"/>
		<xsl:call-template name="Newline"/>

		<xsl:variable name="name" select="@name"/>
		<xsl:for-each select="/registry/feature[remove[@profile = 'core']/enum/@name = $name]">
			<xsl:variable name="rem_ver_major" select="substring-before(@number,'.')"/>
			<xsl:variable name="rem_ver_minor" select="substring-after(@number,'.')"/>
			<xsl:text>#	if not((OGLPLUS_CONFIG_QUERY_GL_VER_MAJOR >= </xsl:text>
			<xsl:value-of select="$rem_ver_major"/>
			<xsl:text>) &amp;&amp; (OGLPLUS_CONFIG_QUERY_GL_VER_MINOR >= </xsl:text>
			<xsl:value-of select="$rem_ver_minor"/>
			<xsl:text>))</xsl:text>
			<xsl:call-template name="Newline"/>
		</xsl:for-each>

		<xsl:text>	OGLPLUS_CONFIG_MISSING_GL_CONST(</xsl:text>
		<xsl:value-of select="@name"/>
		<xsl:text>);</xsl:text>
		<xsl:call-template name="Newline"/>

		<xsl:if test="/registry/feature/remove[@profile = 'core']/enum/@name = $name">
			<xsl:text>#	endif</xsl:text>
			<xsl:call-template name="Newline"/>
		</xsl:if>

		<xsl:text>#	endif</xsl:text>
		<xsl:call-template name="Newline"/>
	</xsl:template>

</xsl:stylesheet>

