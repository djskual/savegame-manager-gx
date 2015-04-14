/****************************************************************************
 * HomebrewXML Class
 * for USB Loader GX
 ***************************************************************************/
#ifndef ___HOMEBREWXML_H_
#define ___HOMEBREWXML_H_

#include <string>
#include <vector>

class HomebrewXML
{
	public:
		HomebrewXML() { };
		HomebrewXML(const char* xmlPath) { LoadHomebrewXMLData(xmlPath); };
		
		int LoadHomebrewXMLData(const char* filename);
		
		const char * GetName() const { return Name.c_str(); };
		void SetName(char * newName) { Name = newName; };
		const char * GetCoder() const { return Coder.c_str(); };
		const char * GetVersion() const { return Version.c_str(); };
		const char * GetReleasedate() const { return Releasedate.c_str(); };
		const char * GetShortDescription() const { return ShortDescription.c_str(); };
		const char * GetLongDescription() const { return LongDescription.c_str(); };
		const std::vector<std::string> & GetArguments() const { return Arguments; };
		
	protected:
		std::string Name;
		std::string Coder;
		std::string Version;
		std::string Releasedate;
		std::string ShortDescription;
		std::string LongDescription;
		std::vector<std::string> Arguments;
};

#endif
