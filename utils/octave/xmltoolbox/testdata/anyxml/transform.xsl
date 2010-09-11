<?xml version="1.0"?>
<!-- 
  Portions of this file Copyright 1999-2005 University of Chicago
  Portions of this file Copyright 1999-2005 The University of Southern California.
  
  This file or a portion of this file is licensed under the
  terms of the Globus Toolkit Public License, found at
  http://www.globus.org/toolkit/download/license.html.
  If you redistribute this file, with or without
  modifications, you must include this notice in the file.
-->
<xsl:stylesheet
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  version="1.0">
<xsl:import href="local/style.xsl" />
<xsl:import href="local/footer.xsl" />
<xsl:import href="xslfooter.xsl" />
<xsl:variable name="baseID" select="translate('$Id: transform.xsl,v 1.1 2007/06/25 11:16:30 gtonic Exp $', ':', '|')" />

<xsl:template match="//*">
<html>
<head>
<title>Generic XML Display</title>
<style>
<xsl:call-template name="style" />
</style>
</head>
<body>
<h1>Generic XML Display</h1>

<xsl:apply-templates select="*" mode="node"/>

<xsl:call-template name="footer" />
<xsl:call-template name="xslFooter" />
</body>
</html>
</xsl:template>

<xsl:template name="node" match="*" mode="node">
<li><xsl:value-of select="local-name()" />:
<xsl:value-of select="text()"/></li>
<ul>
<xsl:for-each select="@*">
<li><xsl:value-of select="local-name()"/>: 
<xsl:value-of select="."/></li>
</xsl:for-each>
</ul>
<xsl:for-each select="child::node()[@* or text()]">
<ul>
        <xsl:call-template name="node"/>
</ul>
     </xsl:for-each>
</xsl:template>
</xsl:stylesheet>
