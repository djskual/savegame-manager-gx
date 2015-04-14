#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <gccore.h>
#include <ogcsys.h>

#include "saves/title.h"
#include "saves/utils.h"

/*-------------------------------------------------------------
 from savegame-manger by waninkoko
titles.c

Copyright (C) 2009 Waninkoko
-------------------------------------------------------------*/

s32 Title_GetList(u64 **outbuf, u32 *outlen)
{
	u64 *titles = NULL;

	u32 len, nb_titles;
	s32 ret;

	/* Get number of titles */
	ret = ES_GetNumTitles(&nb_titles);
	if (ret < 0)
		return ret;

	/* Calculate buffer lenght */
	len = round_up(sizeof(u64) * nb_titles, 32);

	/* Allocate memory */
	titles = memalign(32, len);
	if (!titles)
		return -1;

	/* Get titles */
	ret = ES_GetTitles(titles, nb_titles);
	if (ret < 0)
		goto err;

	/* Set values */
	*outbuf = titles;
	*outlen = nb_titles;

	return 0;

err:
	/* Free memory */
	if (titles)
		free(titles);

	return ret;
}

s32 Title_GetTicketViews(u64 tid, tikview **outbuf, u32 *outlen)
{
	tikview *views = NULL;

	u32 nb_views;
	s32 ret;

	/* Get number of ticket views */
	ret = ES_GetNumTicketViews(tid, &nb_views);
	if (ret < 0)
		return ret;

	/* Allocate memory */
	views = (tikview *)memalign(32, sizeof(tikview) * nb_views);
	if (!views)
		return -1;

	/* Get ticket views */
	ret = ES_GetTicketViews(tid, views, nb_views);
	if (ret < 0)
		goto err;

	/* Set values */
	*outbuf = views;
	*outlen = nb_views;

	return 0;

err:
	/* Free memory */
	if (views)
		free(views);

	return ret;
}

s32 Title_GetTMD(u64 tid, signed_blob **outbuf, u32 *outlen)
{
	void *p_tmd = NULL;

	u32 len;
	s32 ret;

	/* Get TMD size */
	ret = ES_GetStoredTMDSize(tid, &len);
	if (ret < 0)
		return ret;

	/* Allocate memory */
	p_tmd = memalign(32, round_up(len, 32));
	if (!p_tmd)
		return -1;

	/* Read TMD */
	ret = ES_GetStoredTMD(tid, p_tmd, len);
	if (ret < 0)
		goto err;

	/* Set values */
	*outbuf = p_tmd;
	*outlen = len;

	return 0;

err:
	/* Free memory */
	if (p_tmd)
		free(p_tmd);

	return ret;
}

s32 Title_GetVersion(u64 tid, u16 *outbuf)
{
	signed_blob *p_tmd = NULL;
	tmd      *tmd_data = NULL;

	u32 len;
	s32 ret;

	/* Get title TMD */
	ret = Title_GetTMD(tid, &p_tmd, &len);
	if (ret < 0)
		return ret;

	/* Retrieve TMD info */
	tmd_data = (tmd *)SIGNATURE_PAYLOAD(p_tmd);

	/* Set values */
	*outbuf = tmd_data->title_version;

	/* Free memory */
	free(p_tmd);

	return 0;
}

s32 Title_GetSysVersion(u64 tid, u64 *outbuf)
{
	signed_blob *p_tmd = NULL;
	tmd      *tmd_data = NULL;

	u32 len;
	s32 ret;

	/* Get title TMD */
	ret = Title_GetTMD(tid, &p_tmd, &len);
	if (ret < 0)
		return ret;

	/* Retrieve TMD info */
	tmd_data = (tmd *)SIGNATURE_PAYLOAD(p_tmd);

	/* Set values */
	*outbuf = tmd_data->sys_version;

	/* Free memory */
	free(p_tmd);

	return 0;
}

s32 Title_GetSize(u64 tid, u32 *outbuf)
{
	signed_blob *p_tmd = NULL;
	tmd      *tmd_data = NULL;

	u32 cnt, len, size = 0;
	s32 ret;

	/* Get title TMD */
	ret = Title_GetTMD(tid, &p_tmd, &len);
	if (ret < 0)
		return ret;

	/* Retrieve TMD info */
	tmd_data = (tmd *)SIGNATURE_PAYLOAD(p_tmd);

	/* Calculate title size */
	for (cnt = 0; cnt < tmd_data->num_contents; cnt++) {
		tmd_content *content = &tmd_data->contents[cnt];

		/* Add content size */
		size += content->size;
	}

	/* Set values */
	*outbuf = size;

	/* Free memory */
	free(p_tmd);

	return 0;
}

char *titleText(u32 kind, u32 title) {
    static char text[10];

    if (kind == 1) {
        // If we're dealing with System Titles, use custom names
        switch (title) {
        case 1:
            strcpy(text, "BOOT2");
            break;
        case 2:
            strcpy(text, "SYSMENU");
            break;
        case 0x100:
            strcpy(text, "BC");
            break;
        case 0x101:
            strcpy(text, "MIOS");
            break;
        default:
            sprintf(text, "IOS%u", title);
            break;
        }
    } else {
        // Otherwise, just convert the title to ASCII
        int i =32, j = 0;
        do {
            u8 temp;
            i -= 8;
            temp = (title >> i) & 0x000000FF;
            if (temp < 32 || temp > 126)
                text[j] = '.';
            else
                text[j] = temp;
            j++;
        } while (i > 0);
        text[4] = 0;
    }
    return text;
}

/*-------------------------------------------------------------
 from any title deleter
titles.c -- functions for grabbing all titles of a certain type

Copyright (C) 2008 tona
-------------------------------------------------------------*/

u32 __titles_init = 0;
u32 __num_titles;
static u64 __title_list[MAX_TITLES] ATTRIBUTE_ALIGN(32);

s32 __getTitles() {
    s32 ret;
    ret = ES_GetNumTitles(&__num_titles);
    if (ret <0)
        return ret;
    if (__num_titles > MAX_TITLES)
        return -1;
    ret = ES_GetTitles(__title_list, __num_titles);
    if (ret <0)
        return ret;
    __titles_init = 1;
    return 0;
}

s32 getTitles_TypeCount(u32 type, u32 *count) {
    s32 ret = 0;
    u32 type_count;
    if (!__titles_init)
        ret = __getTitles();
    if (ret <0)
        return ret;
    int i;
    type_count = 0;
    for (i=0; i < (int)__num_titles; i++) {
        u32 upper, lower;
        upper = __title_list[i] >> 32;
        lower = __title_list[i] & 0xFFFFFFFF;
        if ((upper == type)&&
                ((lower !=0x48414741)&&//this filters out haga,haaa, hafa.  dupe factory channels that don't load
                 (lower !=0x48414141)&&//since we dont care about apps that dont load for what we are doing
                 (lower !=0x48414641)))
            type_count++;
    }
    *count = type_count;
    return ret;
}

s32 getTitles_Type(u32 type, u32 *titles, u32 count) {
    s32 ret = 0;
    u32 type_count;
    if (!__titles_init)
        ret = __getTitles();
    if (ret <0)
        return ret;
    int i;
    type_count = 0;
    for (i=0; type_count < count && i < (int)__num_titles; i++) {
        u32 upper, lower;
        upper = __title_list[i] >> 32;
        lower = __title_list[i] & 0xFFFFFFFF;
        if ((upper == type)&&
                ((lower !=0x48414741)&&
                 (lower !=0x48414141)&&
                 (lower !=0x48414641))) {
            titles[type_count]=lower;
            type_count++;
        }
    }
    if (type_count < count)
        return -2;
    __titles_init = 0;
    return 0;
}


//this function expects initialize be called before it is called
// if not, it will fail miserably and catch the wii on fire and kick you in the nuts
s32 WII_BootHBC()
{
	u32 tmdsize;
	u64 tid = 0;
	u64 *list;
	u32 titlecount;
	s32 ret;
	u32 i;

	ret = ES_GetNumTitles(&titlecount);
	if(ret < 0)
		return WII_EINTERNAL;

	list = memalign(32, titlecount * sizeof(u64) + 32);

	ret = ES_GetTitles(list, titlecount);
	if(ret < 0) {
		free(list);
		return WII_EINTERNAL;
	}
	
	for(i=0; i<titlecount; i++) {
		if (list[i]==TITLE_ID(0x00010001,0x4A4F4449) 
			|| list[i]==TITLE_ID(0x00010001,0x48415858))
		{
			tid = list[i];
			break;
		}
	}
	free(list);

	if(!tid)
		return WII_EINSTALL;

	if(ES_GetStoredTMDSize(tid, &tmdsize) < 0)
		return WII_EINSTALL;

	return WII_LaunchTitle(tid);
}
