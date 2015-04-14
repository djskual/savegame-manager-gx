#ifndef _FATMOUNTER_H_
#define _FATMOUNTER_H_

#ifdef __cplusplus
extern "C" {
#endif

	extern int   fat_sd_mount;
	extern sec_t fat_sd_sec;
	extern int   fat_usb_mount;
	extern sec_t fat_usb_sec;
	/*extern int   fs_ntfs_mount;
	extern sec_t fs_ntfs_sec;*/

	/*s32 MountNTFS(u32 sector);
	s32 UnmountNTFS(void);*/

    int USBDevice_Init();
    void USBDevice_deInit();
    int SDCard_Init();
    void SDCard_deInit();
	int USBDevice_Inserted();
	int SDCard_Inserted();
	int Device_Inserted();

#ifdef __cplusplus
}
#endif

#endif
