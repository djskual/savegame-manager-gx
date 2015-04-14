#ifndef _ESPATCH_H
#define _ESPATCH_H

#include <gccore.h>

class ESPatch
{
  public:
	static u32 AHBPROT();
	
  private:
	static void disable_memory_protection();
	static u32 apply_patch(const char *name, const u8 *old, u32 old_size, const u8 *patch, u32 patch_size, u32 patch_offset);
	static u32 PrintResult(u32 successful);
	
	static const u8 es_set_ahbprot_old[];
	static const u8 es_set_ahbprot_patch[];
};

#endif /* _IOSPATCH_H */
