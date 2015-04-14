#ifndef PLAYLOG_H_
#define PLAYLOG_H_

#include <gctypes.h>

typedef struct _PlayRec
{
	u32 checksum;
	union
	{
		u32 data[31];
		struct
		{
			u16 name[42];
			u64 ticks_boot;
			u64 ticks_last;
			char title_id[6];
			char unknown[18];
		} ATTRIBUTE_PACKED;
	};
} PlayRec;

class Playlog
{
	public:
		static int Update(const char * ID, const u16 * title);
		static int Delete();
	
	protected:
		static int Create();
};

#endif
