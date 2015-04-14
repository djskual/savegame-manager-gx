/***************************************************************************
 * Copyright (C) 2011
 * by Dj Skual
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any
 * damages arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any
 * purpose, including commercial applications, and to alter it and
 * redistribute it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you
 * must not claim that you wrote the original software. If you use
 * this software in a product, an acknowledgment in the product
 * documentation would be appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and
 * must not be misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source
 * distribution.
 *
 * WiiSave_List Class
 * for SaveGame Manager GX
 ***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gctypes.h>

#include "../../Network/networkops.h"
#include "../../Network/http.h"
#include "../../XML/tinyxml.h"
#include "WiiSave_List.h"

WiiSave_List::WiiSave_List(const char * url)
{
	if (!IsNetworkInit())
		return;
	
	struct block file = downloadfile(url);
	if(!file.data)
		return;
	
	ParseXML((char*)file.data);
}

WiiSave_List::~WiiSave_List()
{
}

std::string WiiSave_List::CleanText(std::string in_text)
{
	if(in_text.empty())
		return "";
	
	//! More replacements can be added if needed
	std::vector<std::pair<std::string, std::string> > Replacements;
	Replacements.push_back(std::pair<std::string, std::string>("<br />", "\n"));
	Replacements.push_back(std::pair<std::string, std::string>("&#33;", "!"));
	Replacements.push_back(std::pair<std::string, std::string>("&#34;", "\""));  
	Replacements.push_back(std::pair<std::string, std::string>("&#35;", "#"));
	Replacements.push_back(std::pair<std::string, std::string>("&#36;", "$"));
	Replacements.push_back(std::pair<std::string, std::string>("&#37;", "%"));
	Replacements.push_back(std::pair<std::string, std::string>("&#38;", "&"));
	Replacements.push_back(std::pair<std::string, std::string>("&#39;", "\'"));
	Replacements.push_back(std::pair<std::string, std::string>("&#40;", "("));
	Replacements.push_back(std::pair<std::string, std::string>("&#41;", ")"));
	Replacements.push_back(std::pair<std::string, std::string>("&#42;", "*"));
	Replacements.push_back(std::pair<std::string, std::string>("&#43;", "+"));
	Replacements.push_back(std::pair<std::string, std::string>("&#44;", ","));
	Replacements.push_back(std::pair<std::string, std::string>("&#45;", "-"));
	Replacements.push_back(std::pair<std::string, std::string>("&#46;", "."));
	Replacements.push_back(std::pair<std::string, std::string>("&#47;", "/"));
	
	for(int i = 0; i < (int) Replacements.size(); i++)
	{
		int pos = in_text.find(Replacements[i].first);
		if (pos != (int) std::string::npos)
		{
			bool end = (pos+Replacements[i].first.size() >= in_text.size());
			in_text.replace(pos, Replacements[i].first.size(), Replacements[i].second);
			if(!end) i--;
		}
		
	}
	Replacements.clear();
	
	return in_text;
}

bool WiiSave_List::ParseXML(const char * xmlfile)
{
	TiXmlDocument xmlDoc;
	
	//if(!xmlDoc.Parse(xmlfile))
		//return false;
	xmlDoc.Parse(xmlfile);//! skipped false return as this one is somtime wrong
	
	TiXmlElement *gameNode =  xmlDoc.FirstChildElement("game");
	if (!gameNode)
		return false;
	
	TiXmlElement *save = gameNode->FirstChildElement("save");
	
	while(save)
	{
		int i = WiiSaveList.size();
		WiiSaveList.resize(i+1);
		
		TiXmlElement *node = NULL;
		
		node = save->FirstChildElement("title");
		if(node && node->FirstChild() && node->FirstChild()->Value())
			WiiSaveList[i].savetitle = node->FirstChild()->Value();
		
		node = save->FirstChildElement("synopsis");
		if(node && node->FirstChild() && node->FirstChild()->Value())
			WiiSaveList[i].synopsis = CleanText(node->FirstChild()->Value());
		
		node = save->FirstChildElement("region");
		if(node && node->FirstChild() && node->FirstChild()->Value())
			WiiSaveList[i].region = node->FirstChild()->Value();
		
		node = save->FirstChildElement("filename");
		if(node && node->FirstChild() && node->FirstChild()->Value())
			WiiSaveList[i].filename = node->FirstChild()->Value();
		
		node = save->FirstChildElement("download_link");
		if(node && node->FirstChild() && node->FirstChild()->Value())
			WiiSaveList[i].download_link = node->FirstChild()->Value();
		
		save = save->NextSiblingElement();
	}
	
	return true;
}


std::string WiiSave_List::GetTitle(int ind)
{
	if (ind < 0 || ind >= (int) WiiSaveList.size())
		return "";
	
	return WiiSaveList[ind].savetitle;
}

std::string WiiSave_List::GetSynopsis(int ind)
{
	if (ind < 0 || ind >= (int) WiiSaveList.size())
		return "";
	
	return WiiSaveList[ind].synopsis;
}

std::string WiiSave_List::GetRegion(int ind)
{
	if (ind < 0 || ind >= (int) WiiSaveList.size())
		return "";
	
	return WiiSaveList[ind].region;
}

std::string WiiSave_List::GetFilename(int ind)
{
	if (ind < 0 || ind >= (int) WiiSaveList.size())
		return "";
	
	return WiiSaveList[ind].filename;
}

std::string WiiSave_List::GetDownloadLink(int ind)
{
	if (ind < 0 || ind >= (int) WiiSaveList.size())
		return "";
	
	return WiiSaveList[ind].download_link;
}

int WiiSave_List::FilterList(std::string inputRegion)
{
	for(u32 i = 0; i < WiiSaveList.size(); i++)
	{
		if(!inputRegion.compare(WiiSaveList.at(i).region))
		{
			int n = FilteredList.size();
			FilteredList.resize(n+1);
			
			FilteredList.at(n).savetitle = WiiSaveList.at(i).savetitle;
			FilteredList.at(n).synopsis = WiiSaveList.at(i).synopsis;
			FilteredList.at(n).region = WiiSaveList.at(i).region;
			FilteredList.at(n).filename = WiiSaveList.at(i).filename;
			FilteredList.at(n).download_link = WiiSaveList.at(i).download_link;
		}
	}
	
	return FilteredList.size();
}

std::string WiiSave_List::GetFilteredTitle(int ind)
{
	if (ind < 0 || ind >= (int) FilteredList.size())
		return "";
	
	return FilteredList[ind].savetitle;
}

std::string WiiSave_List::GetFilteredSynopsis(int ind)
{
	if (ind < 0 || ind >= (int) FilteredList.size())
		return "";
	
	return FilteredList[ind].synopsis;
}

std::string WiiSave_List::GetFilteredRegion(int ind)
{
	if (ind < 0 || ind >= (int) FilteredList.size())
		return "";
	
	return FilteredList[ind].region;
}

std::string WiiSave_List::GetFilteredFilename(int ind)
{
	if (ind < 0 || ind >= (int) FilteredList.size())
		return "";
	
	return FilteredList[ind].filename;
}

std::string WiiSave_List::GetFilteredDownloadLink(int ind)
{
	if (ind < 0 || ind >= (int) FilteredList.size())
		return "";
	
	return FilteredList[ind].download_link;
}
