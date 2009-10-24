<?xml version="1.0" ?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:output
    method="text"
/>

<xsl:param name="field-separator" select="';'"/>
<xsl:param name="line-separator" select="'&#10;'"/>
<xsl:param name="enclose" select="'&quot;'"/>
<xsl:param name="replace-enclose" select="'`'"/>
<xsl:param name="escape" select="'\'"/>

<xsl:template name="replace">
    <xsl:param name="subject"/>
    <xsl:param name="replace"/>
    <xsl:param name="with"/>
    <xsl:choose>
        <xsl:when test="contains($subject, $replace)"><xsl:call-template name="replace">
            <xsl:with-param 
                name="subject" 
                select="
                    concat(
                        substring-before($subject, $replace)
                    ,   $with
                    ,   substring-after($subject, $replace)
                    )
                "
            />
            <xsl:with-param name="replace" select="$replace"/>
            <xsl:with-param name="with" select="$with"/>
        </xsl:call-template></xsl:when>
        <xsl:otherwise><xsl:value-of select="$subject"/></xsl:otherwise>
    </xsl:choose>
</xsl:template>

<xsl:template name="prepare-text">
    <xsl:param name="text"/>
    <xsl:value-of select="$enclose"/><xsl:value-of select="normalize-space(translate($text,$enclose,$replace-enclose))"/><xsl:value-of select="$enclose"/>
</xsl:template>

</xsl:stylesheet>