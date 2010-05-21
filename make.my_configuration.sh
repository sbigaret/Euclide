#! /bin/bash

LNS="/bin/ln -s"
MKDIR=/bin/mkdir

$MKDIR -p my_configuration
cd my_configuration
$LNS ../configuration/images ./
#$LNS ../configuration/server/allowed_resources.xml ./
$LNS ../configuration/platform/platform_shell.xml ./
$LNS ../configuration/server/resources.clientTree.xml ./
$LNS ../configuration/types.cfg ./
$LNS ../configuration/client/viewer_formats.xml ./
$LNS ../configuration/client/viewers.xml ./
