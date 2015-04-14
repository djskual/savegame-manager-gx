#ifndef _NAND_HPP_
#define _NAND_HPP_

#include <ogcsys.h>
#include <gctypes.h>
#include <string>
#include <vector>

class Nand
{	
	public:
		static int Startup();
		static void Shutdown();
		
		static int CreateDir(std::string path);
		static int CreateFile(std::string filepath, u8 attributes, u8 ownerPerm, u8 groupPerm, u8 otherPerm, u32 uid=0, u16 gid=0);
		
		static int Open(std::string filepath, u8 mode);
		static int OpenRead(std::string filepath);
		static int OpenReadWrite(std::string filepath);
		
		static u32 GetFileSize(int fp);
		static u32 GetFileSize(std::string filepath);
		static int GetDirSize(std::string filepath);
		
		static int Close(int fp);
		
		static bool CheckFile(std::string filepath);
		static int Delete(std::string filepath);
		static int Write(int file, u8 *buffer, u32 length);
		static int Write(std::string filepath, u8 *buffer, u32 length);
		static int Read(int fp, u8 *buffer, u32 length);
		static int Seek(int fp, int where, int whence);
		static int ReadDir(std::string path, std::vector<std::string> &list);
		static int LoadFile(std::string filepath, u8 **outbuffer, u32 *outfilesize);
		
		static bool ExtractFile(std::string nandPath, std::string filepath);
		static bool ExtractDir(std::string nandPath, std::string filepath);
		
	private:
		static bool Initialized;
};

#endif

