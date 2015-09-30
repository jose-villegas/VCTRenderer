<!--
   - Copyright 2010-2014 Matus Chochlik. Distributed under the Boost
   - Software License, Version 1.0. (See accompanying file
   - LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
   -
   - XSLT template used for formatting of the XML output from the ToXML
   - filter for the ARB_debug_output extension wrapper.
  -->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
	<xsl:output method="text"/>
	<xsl:strip-space elements="*"/>
	<xsl:preserve-space elements="xsl:text"/>

	<xsl:param name="width"/>
	<xsl:param name="box-offs" select="9"/>

	<xsl:template name="newline"><xsl:text>&#x0A;</xsl:text></xsl:template>

	<xsl:template match="*">
	</xsl:template>

	<xsl:template name="print-padding">
		<xsl:param name="char"/>
		<xsl:param name="count"/>
		<xsl:if test="$count > 0">
			<xsl:value-of select="$char"/>
			<xsl:call-template name="print-padding">
				<xsl:with-param name="char" select="$char"/>
				<xsl:with-param name="count" select="$count - 1"/>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>

	<xsl:template name="print-message">
		<xsl:param name="message"/>
		<xsl:param name="box-width"/>
		<xsl:param name="first"/>
		<xsl:if test="$message != ''">
			<xsl:choose>
				<xsl:when test="$first = 1">
					<xsl:text> ┠─┬──┤</xsl:text>
				</xsl:when>
				<xsl:otherwise>
					<xsl:text> ┃ │  │</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
			<xsl:value-of select="substring($message, 1, $box-width - $box-offs)"/>

			<xsl:call-template name="print-padding">
				<xsl:with-param name="char" select="' '"/>
				<xsl:with-param
					name="count"
					select="$box-width - $box-offs - string-length($message)"
				/>
			</xsl:call-template>

			<xsl:text>│</xsl:text>
			<xsl:call-template name="newline"/>

			<xsl:call-template name="print-message">
				<xsl:with-param
					name="message"
					select="substring($message, $box-width - $box-offs + 1)"
				/>
				<xsl:with-param name="box-width" select="$box-width"/>
				<xsl:with-param name="first" select="0"/>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>


	<xsl:template match="entry">
		<xsl:text> ┃</xsl:text>
		<xsl:call-template name="newline"/>

		<xsl:variable name="box-width">
			<xsl:choose>
				<xsl:when test="$width > string-length(message/text()) + $box-offs">
					<xsl:value-of select="string-length(message/text()) + $box-offs"/>
				</xsl:when>
				<xsl:otherwise>
					<xsl:value-of select="$width"/>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>

		<xsl:text> ┃    ┌</xsl:text>
		<xsl:call-template name="print-padding">
			<xsl:with-param name="char" select="'─'"/>
			<xsl:with-param name="count" select="$box-width - $box-offs"/>
		</xsl:call-template>
		<xsl:text>┐</xsl:text>
		<xsl:call-template name="newline"/>

		<xsl:call-template name="print-message">
			<xsl:with-param name="message" select="message/text()"/>
			<xsl:with-param name="box-width" select="$box-width"/>
			<xsl:with-param name="first" select="1"/>
		</xsl:call-template>

		<xsl:text> ┃ │  └</xsl:text>
		<xsl:call-template name="print-padding">
			<xsl:with-param name="char" select="'─'"/>
			<xsl:with-param name="count" select="$box-width - $box-offs"/>
		</xsl:call-template>
		<xsl:text>┘</xsl:text>
		<xsl:call-template name="newline"/>

		<xsl:text> ┃ ├──[Id]       </xsl:text>
		<xsl:value-of select="id/text()"/>
		<xsl:call-template name="newline"/>

		<xsl:text> ┃ ├──[Source]   </xsl:text>
		<xsl:value-of select="
			substring-after(
				substring-before(
					source/text(),
					'_ARB'
				),
				'DEBUG_SOURCE_'
			)
		"/>
		<xsl:call-template name="newline"/>

		<xsl:text> ┃ ├──[Type]     </xsl:text>
		<xsl:value-of select="
			substring-after(
				substring-before(
					type/text(),
					'_ARB'
				),
				'DEBUG_TYPE_'
			)
		"/>
		<xsl:call-template name="newline"/>

		<xsl:text> ┃ └──[Severity] </xsl:text>
		<xsl:value-of select="
			substring-after(
				substring-before(
					severity/text(),
					'_ARB'
				),
				'DEBUG_SEVERITY_'
			)
		"/>
		<xsl:call-template name="newline"/>
	</xsl:template>

	<xsl:template match="ARB_debug_output|KHR_debug">
		<xsl:text>━┳━━[Begin]</xsl:text>
		<xsl:call-template name="newline"/>

		<xsl:apply-templates/>
		<xsl:text> ┃</xsl:text>
		<xsl:call-template name="newline"/>
		<xsl:text> ┗━━[Done]</xsl:text>
		<xsl:call-template name="newline"/>
	</xsl:template>
</xsl:stylesheet>

