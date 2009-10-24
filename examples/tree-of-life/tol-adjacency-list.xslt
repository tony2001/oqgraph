<?xml version="1.0" ?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:include href="tol-shared.xslt"/>

<xsl:template match="/TREE">
<xsl:value-of select="'ID'"/>
<xsl:value-of select="$field-separator"/>
<xsl:value-of select="'PARENT_ID'"/>
<xsl:value-of select="$field-separator"/>
<xsl:value-of select="'POSITION'"/>
<xsl:value-of select="$field-separator"/>
<xsl:value-of select="'NAME'"/>
<xsl:value-of select="$field-separator"/>
<xsl:value-of select="'DESCRIPTION'"/>
<xsl:value-of select="$line-separator"/>
<xsl:apply-templates select="NODE"/>
</xsl:template>

<xsl:template match="//NODE">
<xsl:value-of select="@ID"/>
<xsl:value-of select="$field-separator"/>
<xsl:value-of select="../../@ID"/>
<xsl:value-of select="$field-separator"/>
<xsl:value-of select="position()"/>
<xsl:value-of select="$field-separator"/>
<xsl:call-template name="prepare-text">
    <xsl:with-param name="text" select="NAME"/>
</xsl:call-template>
<xsl:value-of select="$field-separator"/>
<xsl:call-template name="prepare-text">
    <xsl:with-param name="text" select="DESCRIPTION"/>
</xsl:call-template>
<xsl:value-of select="$line-separator"/>
<xsl:apply-templates select="NODES/NODE"/>
</xsl:template>

</xsl:stylesheet>