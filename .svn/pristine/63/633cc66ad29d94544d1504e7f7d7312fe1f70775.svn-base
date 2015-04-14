#ifndef _MII_INSTALLER_H_
#define _MII_INSTALLER_H_

/* Constants */
#define MII_FILE	"/shared2/menu/FaceLib/RFL_DB.dat"
#define MII_MAX		100

/* Structs */
struct mii_t {
	/* Mii data */
	u8 data1[2];

	/* Mii name */
	u16 name[10];

	/* Mii data */
	u8 data2[32];

	/* Mii creator */
	u16 creator[10];
} ATTRIBUTE_PACKED;

struct mii_db {
	/* Database header */
	u8 header[4];

	/* Mii entries */
	struct mii_t mii[MII_MAX];
	u8 fill[20];

	/* More database data */
	u8 db_data[0x1D4DE];

	/* CRC16 */
	u16 crc;
} ATTRIBUTE_PACKED;

#ifdef __cplusplus
extern "C" {
#endif

/* Prototypes */
u16 crc16(const u8 *buf, u32 len);
s32 install_mii(const char * miiPath, int reference);

#ifdef __cplusplus
}
#endif

#endif
