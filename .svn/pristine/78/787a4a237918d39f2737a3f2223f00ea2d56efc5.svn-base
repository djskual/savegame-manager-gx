/****************************************************************************
 * Copyright (C) 2011
 * by Dj_Skual
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any
 * damages arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any
 * purpose, including commercial applications, and to alter it and
 * redistribute it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you
 * must not claim that you wrote the original software. If you use
 * this software in a product, an acknowledgment in the product
 * documentation would be appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and
 * must not be misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source
 * distribution.
 *
 * SysKeys.hpp
 *
 * for SaveGame Manager GX 2011
 ***************************************************************************/
#ifndef _SYSKEYS_HPP_
#define _SYSKEYS_HPP_

#include <gctypes.h>

class SysKeys
{
	public:
		SysKeys();
		~SysKeys();
		
		void FindKeys();
		bool IsOK() { return _ok; }
		
		u32 GetNgID() { return ngID; }
		u32 GetNgKeyID() { return ngKeyID; }
		u8* GetNgMac() { return ngMac; }
		u8* GetNgPriv() { return ngPriv; }
		u8* GetNgSig() { return ngSig; }
	
	protected:
		//bool NgID();
		bool NgMac();
		bool Otp();
		bool Seeprom();
		
		bool _ok;
		
		u32 ngID;
		u32 ngKeyID;
		u8 ngPriv[32];
		u8 ngSig[60];
		u8 ngMac[6];
};

#endif
