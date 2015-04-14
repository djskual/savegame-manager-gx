#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "mii/mii_list.h"
#include "text/wstring.hpp"
#include "main.h"

#define FACELIB_Wii "/shared2/menu/FaceLib/RFL_DB.dat"
#define round_up(x,n)   (-(-(x) & -(n)))

MIILISTINFO miiListInfo;
MIILISTENTRY * miiList = NULL;

/****************************************************************************
 * ResetMiiList()
 *
 * Clears the mii browser memory, and allocates one initial entry
 ***************************************************************************/
void ResetMiiList()
{
	miiListInfo.Cnt = 0;
	miiListInfo.selIndex = 0;
	miiListInfo.pageIndex = 0;

	// Clear any existing values
	if(miiList != NULL)
	{
		free(miiList);
		miiList = NULL;
	}
	// set aside space for 1 entry
	miiList = (MIILISTENTRY *)malloc(sizeof(MIILISTENTRY));
	memset(miiList, 0, sizeof(MIILISTENTRY));
}

/****************************************************************************
 * GetMiiNandName()
 *
 * Get the utf-16 name of the mii
 ***************************************************************************/
int GetMiiNandName(char* name, int reference) {

	memset(name, 0, sizeof(name));
	
	s32 fh = ISFS_Open(FACELIB_Wii, ISFS_OPEN_READ);
	
	if (fh < 0)
		return -1;
	
	ISFS_Seek(fh, 4+2+(reference*74), 0);
	u16* namedata = (u16*)memalign(32, round_up(20, 32));
	memset(namedata, 0, round_up(20, 32));
	
	if (ISFS_Read(fh, (char*)namedata, 20) < 0)
	{
		ISFS_Close(fh);
		free(namedata);
		namedata = NULL;
		return -2;
	}
	
	ISFS_Close(fh);
	
	wchar_t wname[11];
	memset(wname, 0, sizeof(wname));
	
	int i = 0;
	for (i = 0; i < 10; i++)
		wname[i] = (wchar_t)namedata[i];
	
	free(namedata);
	namedata = NULL;
	
	wString *ws_name = new wString(wname);
	
	snprintf(name, strlen((ws_name->toUTF8()).c_str())+1, "%s", (ws_name->toUTF8()).c_str());
	
	delete ws_name;
	
	return 1;
}
 
/****************************************************************************
 * GetMiiNandCreator()
 *
 * Get the utf-16 Creator name of the mii
 ***************************************************************************/
int GetMiiNandCreator(char* creator, int reference) {
	
	memset(creator, 0, sizeof(creator));
	
	s32 fh = ISFS_Open(FACELIB_Wii, ISFS_OPEN_READ);
	
	if (fh < 0)
		return -1;
	
	ISFS_Seek(fh, 4+0x36+(reference*74), 0);
	u16* creatordata = (u16*)memalign(32, round_up(20, 32));
	memset(creatordata, 0, round_up(20, 32));
	
	if (ISFS_Read(fh, (char*)creatordata, 20) < 0)
	{
		ISFS_Close(fh);
		free(creatordata);
		creatordata = NULL;
		return -2;
	}
	
	ISFS_Close(fh);
	
	wchar_t wcreator[11];
	memset(wcreator, 0, sizeof(wcreator));
	
	int i = 0;
	for (i = 0; i < 10; i++)
		wcreator[i] = (wchar_t)creatordata[i];
	
	free(creatordata);
	creatordata = NULL;
	
	wString *ws_creator = new wString(wcreator);
	
	snprintf(creator, strlen((ws_creator->toUTF8()).c_str())+1, "%s", (ws_creator->toUTF8()).c_str());
	
	delete ws_creator;
	
	return 1;
}

/****************************************************************************
 * GetMiiNandInfos
 *
 * Return infos of mii installed in the wii (from libmii)
 ***************************************************************************/
void GetMiiNandInfos(int start, char * data, int m){
	
	miiList[m].day = 0;

	if (data[start+0x00] & 1) miiList[m].day += 8;
	if (data[start+0x00] & 2) miiList[m].day += 16;

	miiList[m].month = 0;
	if (data[start+0x00] & 4) miiList[m].month += 1;
	if (data[start+0x00] & 8) miiList[m].month += 2;
	if (data[start+0x00] & 16) miiList[m].month += 4;
	if (data[start+0x00] & 32) miiList[m].month += 8;

	miiList[m].female = data[start+0x00] & 64;

	/*
	miiList[m].invalid = data[start+0x00] & 128;

	miiList[m].favorite = data[start+0x01] & 1;*/

	miiList[m].favColor = 0;
	if (data[start+0x01] & 2) miiList[m].favColor += 1;
	if (data[start+0x01] & 4) miiList[m].favColor += 2;
	if (data[start+0x01] & 8) miiList[m].favColor += 4;
	if (data[start+0x01] & 16) miiList[m].favColor += 8;

	if (data[start+0x01] & 32) miiList[m].day += 1;
	if (data[start+0x01] & 64) miiList[m].day += 2;
	if (data[start+0x01] & 128) miiList[m].day += 4;

	/*
	miiList[m].height = data[start + 0x16];
	miiList[m].weight = data[start + 0x17];
	miiList[m].miiID1 = data[start + 0x18];
	miiList[m].miiID2 = data[start + 0x19];
	miiList[m].miiID3 = data[start + 0x1A];
	miiList[m].miiID4 = data[start + 0x1B];
	miiList[m].systemID0 = data[start + 0x1C];
	miiList[m].systemID1 = data[start + 0x1D];
	miiList[m].systemID2 = data[start + 0x1E];
	miiList[m].systemID3 = data[start + 0x1F];

	miiList[m].faceShape = 0;
	if (data[start+0x20] & 1) miiList[m].faceShape += 1;
	if (data[start+0x20] & 2) miiList[m].faceShape += 2;
	if (data[start+0x20] & 4) miiList[m].faceShape += 4;

	miiList[m].skinColor = 0;
	if (data[start+0x20] & 8) miiList[m].skinColor += 1;
	if (data[start+0x20] & 16) miiList[m].skinColor += 2;
	if (data[start+0x20] & 32) miiList[m].skinColor += 4;

	miiList[m].facialFeature = 0;
	if (data[start+0x20] & 64) miiList[m].facialFeature += 1;
	if (data[start+0x20] & 128) miiList[m].facialFeature += 2;
	if (data[start+0x21] & 1) miiList[m].facialFeature += 4;
	if (data[start+0x21] & 2) miiList[m].facialFeature += 8;

	miiList[m].downloaded = data[start+0x21] & 32;

	miiList[m].hairType = 0;
	if (data[start+0x22] & 1) miiList[m].hairType += 1;
	if (data[start+0x22] & 2) miiList[m].hairType += 2;
	if (data[start+0x22] & 4) miiList[m].hairType += 4;
	if (data[start+0x22] & 8) miiList[m].hairType += 8;
	if (data[start+0x22] & 16) miiList[m].hairType += 16;
	if (data[start+0x22] & 32) miiList[m].hairType += 32;
	if (data[start+0x22] & 64) miiList[m].hairType += 64;

	miiList[m].hairColor = 0;
	if (data[start+0x22] & 128) miiList[m].hairColor += 1;
	if (data[start+0x23] & 1) miiList[m].hairColor += 2;
	if (data[start+0x23] & 2) miiList[m].hairColor += 4;

	miiList[m].hairPart = data[start+0x23] & 128;

	miiList[m].eyebrowType = 0;
	if (data[start+0x24] & 1) miiList[m].eyebrowType+=1;
	if (data[start+0x24] & 2) miiList[m].eyebrowType+=2;
	if (data[start+0x24] & 4) miiList[m].eyebrowType+=4;
	if (data[start+0x24] & 8) miiList[m].eyebrowType+=8;
	if (data[start+0x24] & 16) miiList[m].eyebrowType+=16;

	miiList[m].eyebrowRotation = 0;
	if (data[start+0x24] & 64) miiList[m].eyebrowType+=1;
	if (data[start+0x24] & 128) miiList[m].eyebrowType+=2;
	if (data[start+0x25] & 1) miiList[m].eyebrowType+=4;
	if (data[start+0x25] & 2) miiList[m].eyebrowType+=8;

	miiList[m].eyebrowColor = 0;
	if (data[start+0x26] & 1) miiList[m].eyebrowColor += 1;
	if (data[start+0x26] & 2) miiList[m].eyebrowColor += 2;
	if (data[start+0x26] & 4) miiList[m].eyebrowColor += 4;

	miiList[m].eyebrowSize = 0;
	if (data[start+0x26] & 8) miiList[m].eyebrowSize += 1;
	if (data[start+0x26] & 16) miiList[m].eyebrowSize += 2;
	if (data[start+0x26] & 32) miiList[m].eyebrowSize += 4;
	if (data[start+0x26] & 64) miiList[m].eyebrowSize += 8;

	miiList[m].eyebrowVertPos = 0;
	if (data[start+0x26] & 128) miiList[m].eyebrowVertPos += 1;
	if (data[start+0x27] & 1) miiList[m].eyebrowVertPos += 2;
	if (data[start+0x27] & 2) miiList[m].eyebrowVertPos += 4;
	if (data[start+0x27] & 4) miiList[m].eyebrowVertPos += 8;
	if (data[start+0x27] & 8) miiList[m].eyebrowVertPos += 16;

	miiList[m].eyebrowHorizSpacing = 0;
	if (data[start+0x27] & 16) miiList[m].eyebrowHorizSpacing += 1;
	if (data[start+0x27] & 32) miiList[m].eyebrowHorizSpacing += 2;
	if (data[start+0x27] & 64) miiList[m].eyebrowHorizSpacing += 4;
	if (data[start+0x27] & 128) miiList[m].eyebrowHorizSpacing += 8;

	miiList[m].eyeType = 0;
	if (data[start+0x28] & 1) miiList[m].eyeType += 1;
	if (data[start+0x28] & 2) miiList[m].eyeType += 2;
	if (data[start+0x28] & 4) miiList[m].eyeType += 4;
	if (data[start+0x28] & 8) miiList[m].eyeType += 8;
	if (data[start+0x28] & 16) miiList[m].eyeType += 16;
	if (data[start+0x28] & 32) miiList[m].eyeType += 32;

	miiList[m].eyeRotation = 0;
	if (data[start+0x29] & 1) miiList[m].eyeRotation += 1;
	if (data[start+0x29] & 2) miiList[m].eyeRotation += 2;
	if (data[start+0x29] & 4) miiList[m].eyeRotation += 4;

	miiList[m].eyeVertPos = 0;
	if (data[start+0x29] & 8) miiList[m].eyeVertPos += 1;
	if (data[start+0x29] & 16) miiList[m].eyeVertPos += 2;
	if (data[start+0x29] & 32) miiList[m].eyeVertPos += 4;
	if (data[start+0x29] & 64) miiList[m].eyeVertPos += 8;
	if (data[start+0x29] & 128) miiList[m].eyeVertPos += 16;

	miiList[m].eyeColor = 0;
	if (data[start+0x2A] & 1) miiList[m].eyeColor += 1;
	if (data[start+0x2A] & 2) miiList[m].eyeColor += 2;
	if (data[start+0x2A] & 4) miiList[m].eyeColor += 4;

	miiList[m].eyeSize = 0;
	if (data[start+0x2A] & 16) miiList[m].eyeSize += 1;
	if (data[start+0x2A] & 32) miiList[m].eyeSize += 2;
	if (data[start+0x2A] & 64) miiList[m].eyeSize += 4;

	miiList[m].eyeHorizSpacing = 0;
	if (data[start+0x2A] & 128) miiList[m].eyeHorizSpacing += 1;
	if (data[start+0x2B] & 1) miiList[m].eyeHorizSpacing += 2;
	if (data[start+0x2B] & 2) miiList[m].eyeHorizSpacing += 4;
	if (data[start+0x2B] & 4) miiList[m].eyeHorizSpacing += 8;

	miiList[m].noseType = 0;
	if (data[start+0x2C] & 1) miiList[m].noseType += 1;
	if (data[start+0x2C] & 2) miiList[m].noseType += 2;
	if (data[start+0x2C] & 4) miiList[m].noseType += 4;
	if (data[start+0x2C] & 8) miiList[m].noseType += 8;

	miiList[m].noseSize = 0;
	if (data[start+0x2C] & 1) miiList[m].noseSize += 1;
	if (data[start+0x2C] & 2) miiList[m].noseSize += 2;
	if (data[start+0x2C] & 4) miiList[m].noseSize += 4;
	if (data[start+0x2C] & 8) miiList[m].noseSize += 8;

	miiList[m].noseVertPos = 0;
	if (data[start+0x2C] & 16) miiList[m].noseVertPos += 1;
	if (data[start+0x2C] & 32) miiList[m].noseVertPos += 2;
	if (data[start+0x2C] & 64) miiList[m].noseVertPos += 4;
	if (data[start+0x2C] & 128) miiList[m].noseVertPos += 8;

	miiList[m].lipType = 0;
	if (data[start+0x2E] & 1) miiList[m].lipType += 1;
	if (data[start+0x2E] & 2) miiList[m].lipType += 2;
	if (data[start+0x2E] & 4) miiList[m].lipType += 4;
	if (data[start+0x2E] & 8) miiList[m].lipType += 8;
	if (data[start+0x2E] & 16) miiList[m].lipType += 16;

	miiList[m].lipColor = 0;
	if (data[start+0x2E] & 32) miiList[m].lipColor += 1;
	if (data[start+0x2E] & 64) miiList[m].lipColor += 2;

	miiList[m].lipSize = 0;
	if (data[start+0x2E] & 128) miiList[m].lipSize += 1;
	if (data[start+0x2F] & 1) miiList[m].lipSize += 2;
	if (data[start+0x2F] & 2) miiList[m].lipSize += 4;
	if (data[start+0x2F] & 4) miiList[m].lipSize += 8;

	miiList[m].lipVertPos = 0;
	if (data[start+0x2F] & 8) miiList[m].lipVertPos += 1;
	if (data[start+0x2F] & 16) miiList[m].lipVertPos += 2;
	if (data[start+0x2F] & 32) miiList[m].lipVertPos += 4;
	if (data[start+0x2F] & 64) miiList[m].lipVertPos += 8;
	if (data[start+0x2F] & 128) miiList[m].lipVertPos += 16;

	miiList[m].glassesType = 0;
	if (data[start+0x30] & 1) miiList[m].glassesType += 1;
	if (data[start+0x30] & 2) miiList[m].glassesType += 2;
	if (data[start+0x30] & 4) miiList[m].glassesType += 4;
	if (data[start+0x30] & 8) miiList[m].glassesType += 8;

	miiList[m].glassesColor = 0;
	if (data[start+0x30] & 16) miiList[m].glassesColor += 1;
	if (data[start+0x30] & 32) miiList[m].glassesColor += 2;
	if (data[start+0x30] & 64) miiList[m].glassesColor += 4;

	miiList[m].glassesSize = 0;
	if (data[start+0x30] & 1) miiList[m].glassesSize += 1;
	if (data[start+0x31] & 2) miiList[m].glassesSize += 2;
	if (data[start+0x31] & 4) miiList[m].glassesSize += 4;

	miiList[m].glassesVertPos = 0;
	if (data[start+0x31] & 8) miiList[m].glassesVertPos += 1;
	if (data[start+0x31] & 16) miiList[m].glassesVertPos += 2;
	if (data[start+0x31] & 32) miiList[m].glassesVertPos += 4;
	if (data[start+0x31] & 64) miiList[m].glassesVertPos += 8;
	if (data[start+0x31] & 128) miiList[m].glassesVertPos += 16;

	miiList[m].mustacheType = 0;
	if (data[start+0x32] & 1) miiList[m].mustacheType += 1;
	if (data[start+0x32] & 2) miiList[m].mustacheType += 2;

	miiList[m].beardType = 0;
	if (data[start+0x32] & 4) miiList[m].beardType += 1;
	if (data[start+0x32] & 8) miiList[m].beardType += 2;

	miiList[m].facialHairColor = 0;
	if (data[start+0x32] & 16) miiList[m].facialHairColor += 1;
	if (data[start+0x32] & 32) miiList[m].facialHairColor += 2;
	if (data[start+0x32] & 64) miiList[m].facialHairColor += 4;

	miiList[m].mustacheSize = 0;
	if (data[start+0x32] & 128) miiList[m].mustacheSize += 1;
	if (data[start+0x33] & 1) miiList[m].mustacheSize += 2;
	if (data[start+0x33] & 2) miiList[m].mustacheSize += 4;
	if (data[start+0x33] & 4) miiList[m].mustacheSize += 8;

	miiList[m].mustacheVertPos = 0;
	if (data[start+0x33] & 8) mii.mustacheVertPos += 1;
	if (data[start+0x33] & 16) mii.mustacheVertPos += 2;
	if (data[start+0x33] & 32) mii.mustacheVertPos += 4;
	if (data[start+0x33] & 64) mii.mustacheVertPos += 8;
	if (data[start+0x33] & 128) mii.mustacheVertPos += 16;

	//mii.mole = data[start+0x34] & 1;
	miiList[m].mole = 0;

	miiList[m].moleSize = 0;
	if (data[start+0x34] & 2) mii.moleSize += 1;
	if (data[start+0x34] & 4) mii.moleSize += 2;
	if (data[start+0x34] & 8) mii.moleSize += 4;
	if (data[start+0x34] & 16) mii.moleSize += 8;

	miiList[m].moleVertPos = 0;
	if (data[start+0x34] & 32) mii.moleVertPos += 1;
	if (data[start+0x34] & 64) mii.moleVertPos += 2;
	if (data[start+0x34] & 128) mii.moleVertPos += 4;
	if (data[start+0x35] & 1) mii.moleVertPos += 8;
	if (data[start+0x35] & 2) mii.moleVertPos += 16;

	miiList[m].moleHorizPos = 0;
	if (data[start+0x35] & 4) mii.moleHorizPos += 1;
	if (data[start+0x35] & 8) mii.moleHorizPos += 2;
	if (data[start+0x35] & 16) mii.moleHorizPos += 4;
	if (data[start+0x35] & 32) mii.moleHorizPos += 8;
	if (data[start+0x35] & 64) mii.moleHorizPos += 16;*/

}

/****************************************************************************
 * CreateMiiList
 *
 * Create a list of mii installed in the wii
 ***************************************************************************/
int CreateMiiList() {

	ResetMiiList();
	
	s32 fh = ISFS_Open(FACELIB_Wii, ISFS_OPEN_READ);
	
	if (fh < 0) {
		miiListInfo.Cnt = 0;
		return 0;
	}
	
	char* miidata = (char*)memalign(32, round_up(7404, 32));
	memset(miidata, 0, round_up(7404, 32));
	
	if (ISFS_Read(fh, (char*)miidata, 7404) < 0) {
		ISFS_Close(fh);
		free(miidata);
		miidata = NULL;
		miiListInfo.Cnt = 0;
		return 0;
	}
	
	ISFS_Close(fh);
	
	if (miidata[0] == 'R' && miidata[1] == 'N' && miidata[2] == 'O' && miidata[3] == 'D'){
		int start;
		int n = 0;
		int m = 0;
		
		for (n=0; n<MII_MAX; n++) {
			MIILISTENTRY * newMiiList = (MIILISTENTRY *)realloc(miiList, (m+1) * sizeof(MIILISTENTRY));
			
			if(!newMiiList) {
				ResetMiiList();
				return -1;
			}
			else {
				miiList = newMiiList;
			}
			
			GetMiiNandName(miiList[m].displayname, n);
			if(strlen(miiList[m].displayname) > 0) {
				start = n*MII_SIZE+MII_HEADER;
				miiList[m].reference = n;
				GetMiiNandCreator(miiList[m].creator, n);
				GetMiiNandInfos(start, (char*)miidata, m);
				miiListInfo.Cnt++;
				m++;
			}
		}
	} else {
		gprintf("Mii version %c%c%c%c is not compatible with libmii\n", miidata[0], miidata[1], miidata[2], miidata[3]);
	}
	
	return miiListInfo.Cnt;
}
