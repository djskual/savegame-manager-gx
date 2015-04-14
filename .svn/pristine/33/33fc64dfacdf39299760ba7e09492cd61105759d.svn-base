/****************************************************************************
 * SaveGame manager 2010
 * Dj_Skual 2010
 *
 * mii_list.h
 ***************************************************************************/

#ifndef _MII_LIST_H_
#define _MII_LIST_H_

#define MII_NAME_LENGTH				10
#define MII_CREATOR_LENGTH			10
#define MII_SIZE 					74
#define MII_MAX						100
#define MII_HEADER					4

typedef struct
{
	int Cnt; // # of entries in wiiSaveList
	int selIndex; // currently selected index of miiList
	int pageIndex; // starting index of miiList page display
	
} MIILISTINFO;

typedef struct
{
	int reference;		// reference to the place in wii miiFile
	int female;
	int month;
	int day;
	int favColor;		// 0 - 11 (changing to 1111, along with setting the preceeding bit
						// results in a grey shirt, some values over 11 will crash the Wii
						// when trying to change the favorite color).
	
	char displayname[MII_NAME_LENGTH*3 + 1];
	char creator[MII_NAME_LENGTH*3 + 1];

	/*
	int invalid;		// doesn't seem to have any effect?
	int favorite;

	int height;                 // 0 - 127
	int weight;                 // 0 - 127

	int miiID1; 	           // Unique Mii identifier. Seems to increment with time. Also can
	int miiID2; 	           // be used to change colour of Mii Trousers (see 'See Also' links)
	int miiID3;
	int miiID4;

	int systemID0;	           // Checksum8 of first 3 bytes of mac addr
	int systemID1;	           // mac addr 3rd-to-last byte
	int systemID2;	           // mac addr 2nd-to-last byte
	int systemID3;	           // mac addr last byte

	int faceShape;           // 0 - 7
	int skinColor;           // 0 - 5
	int facialFeature;       // 0 - 11
	int downloaded;           // 0 = Mingle, 1 = Don't Mingle

	int hairType;            // 0 - 71, Value is non-sequential with regard to page, row and column
	int hairColor;           // 0 - 7
	int hairPart;            // 0 = Normal, 1 = Reversed

	int eyebrowType;         // 0 - 23, Value is non-sequential with regard to page, row and column
	int eyebrowRotation;     // 0 - 11, Default value varies based on eyebrow type
	int eyebrowColor;        // 0 - 7
	int eyebrowSize;	   // 0 - 8, Default = 4
	int eyebrowVertPos;      // 3 - 18, Default = 10
	int eyebrowHorizSpacing; // 0 - 12, Default = 2

	int eyeType;             // 0 - 47, Value is non-sequential with regard to page, row and column
	int eyeRotation;         // 0 - 7, Default value varies based on eye type
	int eyeVertPos;          // 0 - 18, Default = 12
	int eyeColor;            // 0 - 5
	int eyeSize;             // 0 - 7, Default = 4
	int eyeHorizSpacing;     // 0 - 12, Default = 2

	int noseType;            // 0 - 11, Value is non-sequential with regard to row and column
	int noseSize;            // 0 - 8, Default = 4
	int noseVertPos;         // 0 - 18, Default = 9

	int lipType;             // 0 - 23, Value is non-sequential with regard to page, row and column
	int lipColor;            // 0 - 2
	int lipSize;             // 0 - 8, Default = 4
	int lipVertPos;          // 0 - 18, Default = 13

	int glassesType;         // 0 - 8
	int glassesColor;        // 0 - 5
	int glassesSize;         // 0 - 7, Default = 4
	int glassesVertPos;      // 0 - 20, Default = 10

	int mustacheType;        // 0 - 3
	int beardType;           // 0 - 3
	int facialHairColor;     // 0 - 7
	int mustacheSize;        // 0 - 8, Default = 4
	int mustacheVertPos;     // 0 - 16, Default = 10

	int mole;              // 0 = No Mole, 1 = Has Mole
	int moleSize;            // 0 - 8, Default = 4
	int moleVertPos;         // 0 - 30, Default = 20
	int moleHorizPos;        // 0 - 16, Default = 2*/
	
} MIILISTENTRY;

extern MIILISTINFO miiListInfo;
extern MIILISTENTRY * miiList;

void ResetMiiList();
int CreateMiiList();

#endif
