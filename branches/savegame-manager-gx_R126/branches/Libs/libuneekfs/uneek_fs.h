/*-------------------------------------------------------------

uneek_fs.H -- USB mass storage support, using a modified uneek fs-usb

rev. 1.00		first draft

rev. 1.01		second draft
added detection and support for SDHC access in SNEEK
added check_uneek_fs_type function to see if it's sneek or uneek

rev. 1.02		third draft
added the UNEEK_FS_BOTH option to the init_uneek_fs function.
when the parameter is set, both sd and usb access are rerouted to the sd card.
it can only be used with sneek.
the exit_uneek_fs function needs to be called when that setup is used.
since I can't disable usb, not knowing if sd access is still needed, and
I can't disable sd if usb access if sd is still needed.
It's probably exceptional that one will be shutdown and the other still used,
but it's better to stay on the safe area.

rev. 1.05 fifth draft
added "is_uneek" function

rev. 1.06 sixth draft
added "is_neek2" and "is_neek3" functions
Crediar changed the boot2 version back from 5 to 4. 
The is_uneek function will not detect those neek versions anymore.
Stfour created the is_neek2 method to detect if neek is running.
JoostinOnline and GiantPune created the is_neek3 which is a little less code.
So, you should use either is_neek2 or is_neek3

rev. 1.07 seventh draft
added "is_neek4" function
Dj_Skual created the is_neek4 method to detect if neek is running.

added WII_launch_Channel. It's not really uneek_fs related, but it could be handy 


Copyright (C) 2011 Obcd

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any
damages arising from the use of this software.

Permission is granted to anyone to use this software for any
purpose, including commercial applications, and to alter it and
redistribute it freely, subject to the following restrictions:

1.	The origin of this software must not be misrepresented; you
must not claim that you wrote the original software. If you use
this software in a product, an acknowledgment in the product
documentation would be appreciated but is not required.

2.	Altered source versions must be plainly marked as such, and
must not be misrepresented as being the original software.

3.	This notice may not be removed or altered from any source
distribution.

-------------------------------------------------------------*/

#ifndef _UNEEK_FS_H_
#define _UNEEK_FS_H_ 

#include <ogcsys.h>
#include <ogc/isfs.h>


#define	UNEEK_FS_NONE				0
#define UNEEK_FS_USB				1
#define UNEEK_FS_SD					2
#define UNEEK_FS_BOTH				4

#define ISFS_OPEN_ALL				64

#ifdef __cplusplus
extern "C" {
#endif  //__cplusplus
bool init_uneek_fs(u32 mode);
bool check_uneek_fs(void);
s32 check_uneek_fs_type(void);
bool exit_uneek_fs(void);
bool is_neek(void);
bool is_neek2 (bool isfsinit); 
bool is_neek3 (bool isfsinit); 
bool is_neek4 (bool isfsinit); 
bool WII_Launch_Channel(char* which);




#ifdef __cplusplus
}
#endif  //__cplusplus

#endif
