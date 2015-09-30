<?xml version="1.0" encoding="UTF-8" ?>
<xsl:stylesheet
	version="1.0"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
>
<xsl:param name="feed-name" select="news.rss"/>

<xsl:template match="item">
	<div class="rss-item">
		<xsl:element name="a">
			<xsl:attribute name="href">
				<xsl:value-of select="link/text()"/>
			</xsl:attribute>
			<xsl:element name="h4">
				<xsl:value-of select="title/text()"/>
			</xsl:element>
		</xsl:element>
		<p>
			<span class="rss-item-date">
				<xsl:value-of select="substring(pubDate/text(), 6, 11)"/>
			</span>
			<xsl:if test="description">
				<xsl:value-of select="description/text()"/>
			</xsl:if>
			<br/>
			<xsl:if test="link">
				<span class="rss-item-link">
					<xsl:element name="a">
						<xsl:attribute name="href">
							<xsl:value-of select="link/text()"/>
						</xsl:attribute>
						more>>>
					</xsl:element>
				</span>
			</xsl:if>
		</p>
	</div>
</xsl:template>

<xsl:template match="channel">
	<div class="rss-feed">
		<xsl:element name="span">
		<xsl:element name="a">
			<xsl:attribute name="href">
				<xsl:value-of select="$feed-name"/>
			</xsl:attribute>
			<xsl:element name="img">
				<xsl:attribute name="src">feed-icon-28x28.png</xsl:attribute>
			</xsl:element>
		</xsl:element>
		<h3><xsl:value-of select="title/text()"/></h3>
		</xsl:element>
		<div class="rss-items">
			<xsl:apply-templates select="item"/>
		</div>
	</div>
</xsl:template>

<xsl:template match="/rss">
	<xsl:apply-templates select="channel"/>
</xsl:template>
</xsl:stylesheet>
