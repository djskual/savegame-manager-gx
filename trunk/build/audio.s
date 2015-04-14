	.file	"audio.cpp"
	.section	".text"
.Ltext0:
	.align 2
	.globl _Z9InitAudiov
	.type	_Z9InitAudiov, @function
_Z9InitAudiov:
.LFB61:
	.file 1 "d:/Projects/SaveGameManager/savegame-manager-gx/trunk/source/System/audio.cpp"
	.loc 1 19 0
	.cfi_startproc
	mflr 0
	stwu 1,-8(1)
.LCFI0:
	.cfi_def_cfa_offset 8
	.cfi_register 65, 0
	.loc 1 20 0
	li 3,0
	.loc 1 19 0
	stw 0,12(1)
	.loc 1 20 0
	.cfi_offset 65, 4
	bl AUDIO_Init
	.loc 1 21 0
	bl ASND_Init
	.loc 1 22 0
	li 3,0
	bl ASND_Pause
	.loc 1 23 0
	lwz 0,12(1)
	addi 1,1,8
.LCFI1:
	.cfi_def_cfa_offset 0
	mtlr 0
	blr
	.cfi_endproc
.LFE61:
	.size	_Z9InitAudiov, .-_Z9InitAudiov
	.align 2
	.globl _Z13ShutdownAudiov
	.type	_Z13ShutdownAudiov, @function
_Z13ShutdownAudiov:
.LFB62:
	.loc 1 32 0
	.cfi_startproc
	mflr 0
	stwu 1,-8(1)
.LCFI2:
	.cfi_def_cfa_offset 8
	.cfi_register 65, 0
	.loc 1 33 0
	li 3,1
	.loc 1 32 0
	stw 0,12(1)
	.loc 1 33 0
	.cfi_offset 65, 4
	bl ASND_Pause
	.loc 1 34 0
	bl ASND_End
	.loc 1 35 0
	lwz 0,12(1)
	addi 1,1,8
.LCFI3:
	.cfi_def_cfa_offset 0
	mtlr 0
	blr
	.cfi_endproc
.LFE62:
	.size	_Z13ShutdownAudiov, .-_Z13ShutdownAudiov
.Letext0:
	.file 2 "d:\\devkitpro\\devkitppc\\bin\\../lib/gcc/powerpc-eabi/4.6.3/../../../../powerpc-eabi/include/stdint.h"
	.file 3 "d:/devkitPro/libogc/include/gctypes.h"
	.file 4 "d:/devkitPro/libogc/include/ogc/gx.h"
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.4byte	0x227
	.2byte	0x2
	.4byte	.Ldebug_abbrev0
	.byte	0x4
	.uleb128 0x1
	.4byte	.LASF29
	.byte	0x4
	.4byte	.LASF30
	.4byte	.Ltext0
	.4byte	.Letext0
	.4byte	.Ldebug_line0
	.uleb128 0x2
	.4byte	.LASF0
	.byte	0x2
	.byte	0x29
	.4byte	0x2c
	.uleb128 0x3
	.byte	0x1
	.byte	0x6
	.4byte	.LASF2
	.uleb128 0x2
	.4byte	.LASF1
	.byte	0x2
	.byte	0x2a
	.4byte	0x3e
	.uleb128 0x3
	.byte	0x1
	.byte	0x8
	.4byte	.LASF3
	.uleb128 0x2
	.4byte	.LASF4
	.byte	0x2
	.byte	0x35
	.4byte	0x50
	.uleb128 0x3
	.byte	0x2
	.byte	0x5
	.4byte	.LASF5
	.uleb128 0x2
	.4byte	.LASF6
	.byte	0x2
	.byte	0x36
	.4byte	0x62
	.uleb128 0x3
	.byte	0x2
	.byte	0x7
	.4byte	.LASF7
	.uleb128 0x2
	.4byte	.LASF8
	.byte	0x2
	.byte	0x4f
	.4byte	0x74
	.uleb128 0x4
	.byte	0x4
	.byte	0x5
	.string	"int"
	.uleb128 0x2
	.4byte	.LASF9
	.byte	0x2
	.byte	0x50
	.4byte	0x86
	.uleb128 0x3
	.byte	0x4
	.byte	0x7
	.4byte	.LASF10
	.uleb128 0x3
	.byte	0x8
	.byte	0x5
	.4byte	.LASF11
	.uleb128 0x3
	.byte	0x8
	.byte	0x7
	.4byte	.LASF12
	.uleb128 0x5
	.string	"u8"
	.byte	0x3
	.byte	0x11
	.4byte	0x33
	.uleb128 0x5
	.string	"u16"
	.byte	0x3
	.byte	0x12
	.4byte	0x57
	.uleb128 0x5
	.string	"u32"
	.byte	0x3
	.byte	0x13
	.4byte	0x7b
	.uleb128 0x5
	.string	"s8"
	.byte	0x3
	.byte	0x16
	.4byte	0x21
	.uleb128 0x5
	.string	"s16"
	.byte	0x3
	.byte	0x17
	.4byte	0x45
	.uleb128 0x5
	.string	"s32"
	.byte	0x3
	.byte	0x18
	.4byte	0x69
	.uleb128 0x5
	.string	"vu8"
	.byte	0x3
	.byte	0x1b
	.4byte	0xe6
	.uleb128 0x6
	.4byte	0x9b
	.uleb128 0x2
	.4byte	.LASF13
	.byte	0x3
	.byte	0x1c
	.4byte	0xf6
	.uleb128 0x6
	.4byte	0xa5
	.uleb128 0x2
	.4byte	.LASF14
	.byte	0x3
	.byte	0x1d
	.4byte	0x106
	.uleb128 0x6
	.4byte	0xb0
	.uleb128 0x5
	.string	"vs8"
	.byte	0x3
	.byte	0x20
	.4byte	0x116
	.uleb128 0x6
	.4byte	0xbb
	.uleb128 0x2
	.4byte	.LASF15
	.byte	0x3
	.byte	0x21
	.4byte	0x126
	.uleb128 0x6
	.4byte	0xc5
	.uleb128 0x2
	.4byte	.LASF16
	.byte	0x3
	.byte	0x22
	.4byte	0x136
	.uleb128 0x6
	.4byte	0xd0
	.uleb128 0x3
	.byte	0x4
	.byte	0x4
	.4byte	.LASF17
	.uleb128 0x3
	.byte	0x8
	.byte	0x4
	.4byte	.LASF18
	.uleb128 0x2
	.4byte	.LASF19
	.byte	0x3
	.byte	0x2e
	.4byte	0x154
	.uleb128 0x6
	.4byte	0x13b
	.uleb128 0x3
	.byte	0x1
	.byte	0x8
	.4byte	.LASF20
	.uleb128 0x3
	.byte	0x1
	.byte	0x2
	.4byte	.LASF21
	.uleb128 0x3
	.byte	0x4
	.byte	0x7
	.4byte	.LASF22
	.uleb128 0x7
	.4byte	.LASF31
	.byte	0x4
	.byte	0x4
	.2byte	0x490
	.4byte	0x1ce
	.uleb128 0x8
	.string	"U8"
	.byte	0x4
	.2byte	0x492
	.4byte	0xdb
	.uleb128 0x8
	.string	"S8"
	.byte	0x4
	.2byte	0x493
	.4byte	0x10b
	.uleb128 0x8
	.string	"U16"
	.byte	0x4
	.2byte	0x494
	.4byte	0xeb
	.uleb128 0x8
	.string	"S16"
	.byte	0x4
	.2byte	0x495
	.4byte	0x11b
	.uleb128 0x8
	.string	"U32"
	.byte	0x4
	.2byte	0x496
	.4byte	0xfb
	.uleb128 0x8
	.string	"S32"
	.byte	0x4
	.2byte	0x497
	.4byte	0x12b
	.uleb128 0x8
	.string	"F32"
	.byte	0x4
	.2byte	0x498
	.4byte	0x149
	.byte	0
	.uleb128 0x9
	.4byte	.LASF23
	.byte	0x4
	.2byte	0x499
	.4byte	0x16e
	.uleb128 0x3
	.byte	0x4
	.byte	0x5
	.4byte	.LASF24
	.uleb128 0xa
	.byte	0x1
	.4byte	.LASF25
	.byte	0x1
	.byte	0x12
	.4byte	.LASF27
	.4byte	.LFB61
	.4byte	.LFE61
	.4byte	.LLST0
	.uleb128 0xa
	.byte	0x1
	.4byte	.LASF26
	.byte	0x1
	.byte	0x1f
	.4byte	.LASF28
	.4byte	.LFB62
	.4byte	.LFE62
	.4byte	.LLST1
	.uleb128 0xb
	.4byte	.LASF32
	.byte	0x4
	.2byte	0x548
	.4byte	0x21f
	.byte	0x1
	.byte	0x1
	.uleb128 0xc
	.4byte	0x224
	.uleb128 0xd
	.byte	0x4
	.4byte	0x1ce
	.byte	0
	.section	.debug_abbrev,"",@progbits
.Ldebug_abbrev0:
	.uleb128 0x1
	.uleb128 0x11
	.byte	0x1
	.uleb128 0x25
	.uleb128 0xe
	.uleb128 0x13
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x10
	.uleb128 0x6
	.byte	0
	.byte	0
	.uleb128 0x2
	.uleb128 0x16
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x3
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.byte	0
	.byte	0
	.uleb128 0x4
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0x8
	.byte	0
	.byte	0
	.uleb128 0x5
	.uleb128 0x16
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x6
	.uleb128 0x35
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x7
	.uleb128 0x17
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x8
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x9
	.uleb128 0x16
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xa
	.uleb128 0x2e
	.byte	0
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x2007
	.uleb128 0xe
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x40
	.uleb128 0x6
	.byte	0
	.byte	0
	.uleb128 0xb
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3c
	.uleb128 0xc
	.byte	0
	.byte	0
	.uleb128 0xc
	.uleb128 0x26
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xd
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.byte	0
	.section	.debug_loc,"",@progbits
.Ldebug_loc0:
.LLST0:
	.4byte	.LFB61-.Ltext0
	.4byte	.LCFI0-.Ltext0
	.2byte	0x2
	.byte	0x71
	.sleb128 0
	.4byte	.LCFI0-.Ltext0
	.4byte	.LCFI1-.Ltext0
	.2byte	0x2
	.byte	0x71
	.sleb128 8
	.4byte	.LCFI1-.Ltext0
	.4byte	.LFE61-.Ltext0
	.2byte	0x2
	.byte	0x71
	.sleb128 0
	.4byte	0
	.4byte	0
.LLST1:
	.4byte	.LFB62-.Ltext0
	.4byte	.LCFI2-.Ltext0
	.2byte	0x2
	.byte	0x71
	.sleb128 0
	.4byte	.LCFI2-.Ltext0
	.4byte	.LCFI3-.Ltext0
	.2byte	0x2
	.byte	0x71
	.sleb128 8
	.4byte	.LCFI3-.Ltext0
	.4byte	.LFE62-.Ltext0
	.2byte	0x2
	.byte	0x71
	.sleb128 0
	.4byte	0
	.4byte	0
	.section	.debug_aranges,"",@progbits
	.4byte	0x1c
	.2byte	0x2
	.4byte	.Ldebug_info0
	.byte	0x4
	.byte	0
	.2byte	0
	.2byte	0
	.4byte	.Ltext0
	.4byte	.Letext0-.Ltext0
	.4byte	0
	.4byte	0
	.section	.debug_line,"",@progbits
.Ldebug_line0:
	.section	.debug_str,"MS",@progbits,1
.LASF21:
	.string	"bool"
.LASF19:
	.string	"vf32"
.LASF15:
	.string	"vs16"
.LASF32:
	.string	"wgPipe"
.LASF1:
	.string	"uint8_t"
.LASF13:
	.string	"vu16"
.LASF0:
	.string	"int8_t"
.LASF14:
	.string	"vu32"
.LASF23:
	.string	"WGPipe"
.LASF17:
	.string	"float"
.LASF29:
	.string	"GNU C++ 4.6.3"
.LASF27:
	.string	"_Z9InitAudiov"
.LASF3:
	.string	"unsigned char"
.LASF22:
	.string	"long unsigned int"
.LASF7:
	.string	"short unsigned int"
.LASF31:
	.string	"_wgpipe"
.LASF18:
	.string	"double"
.LASF16:
	.string	"vs32"
.LASF10:
	.string	"unsigned int"
.LASF20:
	.string	"char"
.LASF4:
	.string	"int16_t"
.LASF8:
	.string	"int32_t"
.LASF30:
	.string	"d:/Projects/SaveGameManager/savegame-manager-gx/trunk/source/System/audio.cpp"
.LASF11:
	.string	"long long int"
.LASF28:
	.string	"_Z13ShutdownAudiov"
.LASF26:
	.string	"ShutdownAudio"
.LASF5:
	.string	"short int"
.LASF6:
	.string	"uint16_t"
.LASF9:
	.string	"uint32_t"
.LASF24:
	.string	"long int"
.LASF2:
	.string	"signed char"
.LASF25:
	.string	"InitAudio"
.LASF12:
	.string	"long long unsigned int"
	.ident	"GCC: (devkitPPC release 26) 4.6.3"
