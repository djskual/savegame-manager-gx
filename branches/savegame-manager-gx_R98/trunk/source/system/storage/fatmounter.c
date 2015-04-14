#include <string.h>
#include <ogc/lwp_watchdog.h>
#include <ogc/mutex.h>
#include <ogc/system.h>
#include <fat.h>
#include <ogc/usbstorage.h>
#include <sdcard/wiisd_io.h>
#include <locale.h>

#include "system/storage/sdhc.h"
#include "system/storage/usbstorage.h"

//these are the only stable and speed is good
#define CACHE 32
#define SECTORS 64
#define SECTORS_SD 32

#define MOUNT_NONE 0
#define MOUNT_SD   1
#define MOUNT_SDHC 2

/* Disc interfaces */
extern const DISC_INTERFACE __io_sdhc;
// read-only
extern const DISC_INTERFACE __io_sdhc_ro;
extern const DISC_INTERFACE __io_usbstorage_ro;

void _FAT_mem_init();
extern sec_t _FAT_startSector;

int   fat_sd_mount = MOUNT_NONE;

int   fat_usb_mount = 0;

/*int   fs_ntfs_mount = 0;
sec_t fs_ntfs_sec = 0;*/

int USBDevice_Init() {
	
	//closing all open Files write back the cache and then shutdown em!
    fatUnmount("usb:/");
    //right now mounts first FAT-partition

	//try first mount with cIOS
    if (!fatMount("usb", &__io_wiiums, 0, CACHE, SECTORS)) {
		//try now mount with libogc
		if (!fatMount("usb", &__io_usbstorage, 0, CACHE, SECTORS)) {
			return -1;
		}
	}
	
	fat_usb_mount = 1;
	
	return 0;
}

void USBDevice_deInit() {

    //closing all open Files write back the cache and then shutdown em!
    fatUnmount("usb:/");

	fat_usb_mount = 0;
}

int USBDevice_Inserted()
{
    return __io_usb_IsInserted();
}

int SDCard_Inserted()
{
    return __io_sdhc.isInserted() || __io_wiisd.isInserted();
}

int Device_Inserted()
{
	if(SDCard_Inserted() || USBDevice_Inserted())
		return 1;
	else
		return 0;
}

int SDCard_Init() {

    //closing all open Files write back the cache and then shutdown em!
    fatUnmount("sd:/");
    //right now mounts first FAT-partition
	if (fatMount("sd", &__io_wiisd, 0, CACHE, SECTORS)) {
		fat_sd_mount = MOUNT_SD;
		return 1;
	}
	else if (fatMount("sd", &__io_sdhc, 0, CACHE, SDHC_SECTOR_SIZE)) {
		fat_sd_mount = MOUNT_SDHC;
		return 1;
	}
	return -1;
}

void SDCard_deInit() {

    //closing all open Files write back the cache and then shutdown em!
    fatUnmount("sd:/");

	fat_sd_mount = MOUNT_NONE;
}

/*void ntfsInit();

s32 MountNTFS(u32 sector)
{
	s32 ret;

	if (fs_ntfs_mount) return 0;
	//printf("mounting NTFS\n");
	//Wpad_WaitButtons();
	_FAT_mem_init();
	ntfsInit();
	// ntfsInit resets locale settings
	// which breaks unicode in console
	// so we change it back to C-UTF-8
	setlocale(LC_CTYPE, "C-UTF-8");
	setlocale(LC_MESSAGES, "C-UTF-8");

	if (wbfsDev == WBFS_DEVICE_USB) {
		// Initialize WBFS interface //
		if (!__io_wiiums.startup()) {
			ret = __io_usbstorage.startup();
			if (!ret) {
				return -1;
			}
		}
		// Mount device //
		if (!ntfsMount("NTFS", &__io_wiiums_ro, sector, CACHE, SECTORS, NTFS_DEFAULT)) {
			ret = ntfsMount("NTFS", &__io_usbstorage_ro, sector, CACHE, SECTORS, NTFS_DEFAULT);
			if (!ret) {
				return -2;
			}
		}
	} else if (wbfsDev == WBFS_DEVICE_SDHC) {
		if (sdhc_mode_sd == 0) {
			ret = ntfsMount("NTFS", &__io_sdhc_ro, 0, CACHE, SECTORS, NTFS_DEFAULT);
		} else {
			ret = ntfsMount("NTFS", &__io_sdhc_ro, 0, CACHE, SECTORS_SD, NTFS_DEFAULT);
		}
		if (!ret) {
			return -5;
		}

	}

	fs_ntfs_mount = 1;
	fs_ntfs_sec = sector; //_FAT_startSector;
	
	return 0;
}

s32 UnmountNTFS(void)
{
	// Unmount device //
	fatUnmount("NTFS:/");

	fs_ntfs_mount = 0;
	fs_ntfs_sec = 0;

	return 0;
}*/

/*void _FAT_mem_init()
{
}

void* _FAT_mem_allocate(size_t size)
{
	return malloc(size);
}

void* _FAT_mem_align(size_t size)
{
	return memalign(32, size);		
}

void _FAT_mem_free(void *mem)
{
	free(mem);
}*/

/*void* ntfs_alloc (size_t size)
{
	return _FAT_mem_allocate(size);
}

void* ntfs_align (size_t size)
{
	return _FAT_mem_align(size);
}

void ntfs_free (void* mem)
{
	_FAT_mem_free(mem);
}*/
