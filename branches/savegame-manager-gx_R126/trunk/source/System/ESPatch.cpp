#include <gccore.h>
#include <ogc/machine/processor.h>
#include <string.h>
#include <stdio.h>

#include "ESPatch.h"
//#include "../Tools/gecko.h"

#define HAVE_AHBPROT ((*(vu32*)0xcd800064 == 0xFFFFFFFF) ? 1 : 0)
#define MEM_REG_BASE 0xd8b4000
#define MEM_PROT (MEM_REG_BASE + 0x20a)

void ESPatch::disable_memory_protection()
{
	write32(MEM_PROT, read32(MEM_PROT) & 0x0000FFFF);
}

u32 ESPatch::apply_patch(const char *name, const u8 *old, u32 old_size, const u8 *patch, u32 patch_size, u32 patch_offset)
{
	//gprintf("\n\tES Applying patch: %s", name);
	u8 *ptr_start = (u8*)*((u32*)0x80003134);
	u8 *ptr_end = (u8*)0x94000000;
	u32 found = 0;
	u8 *location = NULL;
	while (ptr_start < (ptr_end - patch_size))
	{
		if (!memcmp(ptr_start, old, old_size))
		{
			found++;
			location = ptr_start + patch_offset;
			u8 *start = location;
			u32 i;
			for (i = 0; i < patch_size; i++)
			{
				*location++ = patch[i];
			}
			DCFlushRange((u8 *)(((u32)start) >> 5 << 5), (patch_size >> 5 << 5) + 64);
			ICInvalidateRange((u8 *)(((u32)start) >> 5 << 5), (patch_size >> 5 << 5) + 64);
		}
		ptr_start++;
	}
	return found;
}

const u8 ESPatch::es_set_ahbprot_old[] = { 0x68, 0x5B, 0x22, 0xEC, 0x00, 0x52, 0x18, 0x9B, 0x68, 0x1B, 0x46, 0x98, 0x07, 0xDB };
const u8 ESPatch::es_set_ahbprot_patch[]   = { 0x01 };

u32 ESPatch::AHBPROT()
{
	if (HAVE_AHBPROT)
	{
		disable_memory_protection();
		return PrintResult(apply_patch("ES_set_ahbprot...", es_set_ahbprot_old, sizeof(es_set_ahbprot_old), es_set_ahbprot_patch, sizeof(es_set_ahbprot_patch), 25));
	}
	return 0;
}

u32 ESPatch::PrintResult(u32 successful)
{
	/*if (successful)
		gprintf(" DONE");
	else
		gprintf(" FAILED");*/
	
	return successful;
}
