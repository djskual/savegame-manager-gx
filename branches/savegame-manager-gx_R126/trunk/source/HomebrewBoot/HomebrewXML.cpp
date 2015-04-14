/****************************************************************************
 * HomebrewXML Class
 * for USB Loader GX
 ***************************************************************************/
#include <gctypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../FileOperations/fileops.h"
#include "../XML/tinyxml.h"
#include "../Tools/gecko.h"
#include "HomebrewXML.h"

#define ENTRIE_SIZE     8192

int HomebrewXML::LoadHomebrewXMLData(const char* xmlPath)
{
	Name.clear();
	Coder.clear();
	Version.clear();
	ShortDescription.clear();
	LongDescription.clear();
	Releasedate.clear();
	
	TiXmlDocument xmlDoc(xmlPath);
	if(!xmlDoc.LoadFile())
		return false;
	
	TiXmlElement *appNode =  xmlDoc.FirstChildElement("app");
	if(!appNode)
		return false;
	
	TiXmlElement *node = NULL;
	
	node = appNode->FirstChildElement("name");
	if(node && node->FirstChild() && node->FirstChild()->Value())
		Name = node->FirstChild()->Value();
	
	node = appNode->FirstChildElement("coder");
	if(node && node->FirstChild() && node->FirstChild()->Value())
		Coder = node->FirstChild()->Value();
	
	node = appNode->FirstChildElement("version");
	if(node && node->FirstChild() && node->FirstChild()->Value())
		Version = node->FirstChild()->Value();
	
	node = appNode->FirstChildElement("short_description");
	if(node && node->FirstChild() && node->FirstChild()->Value())
		ShortDescription = node->FirstChild()->Value();
	
	node = appNode->FirstChildElement("long_description");
	if(node && node->FirstChild() && node->FirstChild()->Value())
		LongDescription = node->FirstChild()->Value();
	
	char ReleaseText[200];
	memset(ReleaseText, 0, sizeof(ReleaseText));
	
	node = appNode->FirstChildElement("release_date");
	if(node && node->FirstChild() && node->FirstChild()->Value())
		snprintf(ReleaseText, sizeof(ReleaseText), node->FirstChild()->Value());
	
	int len = (strlen(ReleaseText) - 6); //!length of the date string without the 200000 at the end
	if (len == 8)
		snprintf(ReleaseText, sizeof(ReleaseText), "%c%c/%c%c/%c%c%c%c", ReleaseText[4], ReleaseText[5], ReleaseText[6], ReleaseText[7], ReleaseText[0], ReleaseText[1], ReleaseText[2], ReleaseText[3]);
	else if (len == 6)
		snprintf(ReleaseText, sizeof(ReleaseText), "%c%c/%c%c%c%c", ReleaseText[4], ReleaseText[5], ReleaseText[0], ReleaseText[1], ReleaseText[2], ReleaseText[3]);
	
	Releasedate = ReleaseText;
	
	node = appNode->FirstChildElement("arguments");
	if(node)
	{
		TiXmlElement *argNode = node->FirstChildElement("arg");
		
		while(argNode)
		{
			if(argNode->FirstChild() && argNode->FirstChild()->Value())
				Arguments.push_back(std::string(argNode->FirstChild()->Value()));
			
			argNode = argNode->NextSiblingElement();
		}
	}
	
	return 1;
}
