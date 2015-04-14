/****************************************************************************
 * WiiSave_List Class
 * for SaveGame Manager GX
 * by dj_skual
 ***************************************************************************/
#ifndef _WIISAVE_LIST_H_
#define _WIISAVE_LIST_H_

#include <vector>
#include <string>

typedef struct _wiisave_Info
{
		std::string savetitle;
		std::string synopsis;
		std::string region;
		std::string filename;
		std::string download_link;
} WiiSave_Info;

class WiiSave_List
{
	public:
		WiiSave_List(const char *url);
		~WiiSave_List();
		
		std::string GetTitle(int index);
		std::string GetSynopsis(int index);
		std::string GetRegion(int index);
		std::string GetFilename(int index);
		std::string GetDownloadLink(int index);
		int GetCount() const { return WiiSaveList.size(); };
		
		int FilterList(std::string inputRegion);
		std::string GetFilteredTitle(int index);
		std::string GetFilteredSynopsis(int index);
		std::string GetFilteredRegion(int index);
		std::string GetFilteredFilename(int index);
		std::string GetFilteredDownloadLink(int index);
		int GetFilteredCount() const { return FilteredList.size(); };
		
	protected:
		bool ParseXML(const char * xmlfile);
		std::string CleanText(std::string in_text);
		
		std::vector<WiiSave_Info> WiiSaveList;
		std::vector<WiiSave_Info> FilteredList;
};

#endif
