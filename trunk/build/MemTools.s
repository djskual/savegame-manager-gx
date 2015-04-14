	.file	"MemTools.cpp"
	.section	".text"
.Ltext0:
	.align 2
	.globl _ZN8MemTools17AllocateMemory_32Ej
	.type	_ZN8MemTools17AllocateMemory_32Ej, @function
_ZN8MemTools17AllocateMemory_32Ej:
.LFB0:
	.file 1 "d:/Projects/SaveGameManager/savegame-manager-gx/trunk/source/Memory/MemTools.cpp"
	.loc 1 6 0
	.cfi_startproc
.LVL0:
	mflr 0
	stwu 1,-8(1)
.LCFI0:
	.cfi_def_cfa_offset 8
	.cfi_register 65, 0
	.loc 1 7 0
	addi 4,3,31
	li 3,32
.LVL1:
	rlwinm 4,4,0,0,26
.LVL2:
	.loc 1 6 0
	stw 0,12(1)
	.loc 1 7 0
	.cfi_offset 65, 4
	bl memalign
	.loc 1 8 0
	lwz 0,12(1)
	addi 1,1,8
.LCFI1:
	.cfi_def_cfa_offset 0
	mtlr 0
	blr
	.cfi_endproc
.LFE0:
	.size	_ZN8MemTools17AllocateMemory_32Ej, .-_ZN8MemTools17AllocateMemory_32Ej
	.align 2
	.globl _ZN8MemTools17AllocateMemory_64Ej
	.type	_ZN8MemTools17AllocateMemory_64Ej, @function
_ZN8MemTools17AllocateMemory_64Ej:
.LFB1:
	.loc 1 11 0
	.cfi_startproc
.LVL3:
	mflr 0
	stwu 1,-8(1)
.LCFI2:
	.cfi_def_cfa_offset 8
	.cfi_register 65, 0
	.loc 1 12 0
	addi 4,3,63
	li 3,64
.LVL4:
	rlwinm 4,4,0,0,25
.LVL5:
	.loc 1 11 0
	stw 0,12(1)
	.loc 1 12 0
	.cfi_offset 65, 4
	bl memalign
	.loc 1 13 0
	lwz 0,12(1)
	addi 1,1,8
.LCFI3:
	.cfi_def_cfa_offset 0
	mtlr 0
	blr
	.cfi_endproc
.LFE1:
	.size	_ZN8MemTools17AllocateMemory_64Ej, .-_ZN8MemTools17AllocateMemory_64Ej
	.align 2
	.globl _ZN8MemTools10MemIsEmptyEPci
	.type	_ZN8MemTools10MemIsEmptyEPci, @function
_ZN8MemTools10MemIsEmptyEPci:
.LFB2:
	.loc 1 16 0
	.cfi_startproc
.LVL6:
.LBB2:
	.loc 1 19 0
	cmpwi 0,4,0
.LBE2:
	.loc 1 16 0
	mr 11,3
.LBB3:
	.loc 1 25 0
	li 3,1
.LVL7:
	.loc 1 19 0
	blelr- 0
	.loc 1 21 0
	lbz 0,0(11)
	.loc 1 22 0
	li 3,0
	.loc 1 21 0
	li 9,0
	mtctr 4
	cmpwi 7,0,0
	beq+ 7,.L6
	blr
.LVL8:
.L7:
	lbzx 0,11,9
	cmpwi 7,0,0
	bne- 7,.L10
.LVL9:
.L6:
	.loc 1 19 0
	addi 9,9,1
.LVL10:
	bdnz .L7
	.loc 1 25 0
	li 3,1
	blr
.L10:
	.loc 1 22 0
	li 3,0
.LBE3:
	.loc 1 26 0
	blr
	.cfi_endproc
.LFE2:
	.size	_ZN8MemTools10MemIsEmptyEPci, .-_ZN8MemTools10MemIsEmptyEPci
.Letext0:
	.file 2 "d:/Projects/SaveGameManager/savegame-manager-gx/trunk/source/Memory/MemTools.h"
	.file 3 "d:\\devkitpro\\devkitppc\\bin\\../lib/gcc/powerpc-eabi/4.6.3/../../../../powerpc-eabi/include/stdint.h"
	.file 4 "d:/devkitPro/libogc/include/gctypes.h"
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.4byte	0x186
	.2byte	0x2
	.4byte	.Ldebug_abbrev0
	.byte	0x4
	.uleb128 0x1
	.4byte	.LASF18
	.byte	0x4
	.4byte	.LASF19
	.4byte	.Ltext0
	.4byte	.Letext0
	.4byte	.Ldebug_line0
	.uleb128 0x2
	.byte	0x1
	.byte	0x6
	.4byte	.LASF0
	.uleb128 0x2
	.byte	0x1
	.byte	0x8
	.4byte	.LASF1
	.uleb128 0x2
	.byte	0x2
	.byte	0x5
	.4byte	.LASF2
	.uleb128 0x2
	.byte	0x2
	.byte	0x7
	.4byte	.LASF3
	.uleb128 0x3
	.byte	0x4
	.byte	0x5
	.string	"int"
	.uleb128 0x2
	.byte	0x4
	.byte	0x7
	.4byte	.LASF4
	.uleb128 0x2
	.byte	0x8
	.byte	0x5
	.4byte	.LASF5
	.uleb128 0x2
	.byte	0x8
	.byte	0x7
	.4byte	.LASF6
	.uleb128 0x4
	.byte	0x4
	.uleb128 0x2
	.byte	0x4
	.byte	0x7
	.4byte	.LASF7
	.uleb128 0x5
	.byte	0x4
	.4byte	0x68
	.uleb128 0x2
	.byte	0x1
	.byte	0x8
	.4byte	.LASF8
	.uleb128 0x6
	.4byte	.LASF9
	.byte	0x3
	.byte	0x50
	.4byte	0x44
	.uleb128 0x7
	.string	"u32"
	.byte	0x4
	.byte	0x13
	.4byte	0x6f
	.uleb128 0x2
	.byte	0x4
	.byte	0x4
	.4byte	.LASF10
	.uleb128 0x2
	.byte	0x8
	.byte	0x4
	.4byte	.LASF11
	.uleb128 0x8
	.4byte	.LASF20
	.byte	0x1
	.byte	0x2
	.byte	0xb
	.4byte	0xf2
	.uleb128 0x9
	.byte	0x1
	.4byte	.LASF12
	.byte	0x2
	.byte	0xe
	.4byte	.LASF14
	.4byte	0x59
	.byte	0x1
	.4byte	0xba
	.uleb128 0xa
	.4byte	0x7a
	.byte	0
	.uleb128 0x9
	.byte	0x1
	.4byte	.LASF13
	.byte	0x2
	.byte	0xf
	.4byte	.LASF15
	.4byte	0x59
	.byte	0x1
	.4byte	0xd5
	.uleb128 0xa
	.4byte	0x7a
	.byte	0
	.uleb128 0xb
	.byte	0x1
	.4byte	.LASF21
	.byte	0x2
	.byte	0x10
	.4byte	.LASF22
	.4byte	0xf2
	.byte	0x1
	.uleb128 0xa
	.4byte	0x62
	.uleb128 0xa
	.4byte	0x3d
	.byte	0
	.byte	0
	.uleb128 0x2
	.byte	0x1
	.byte	0x2
	.4byte	.LASF16
	.uleb128 0xc
	.4byte	0x9f
	.byte	0x1
	.byte	0x5
	.4byte	.LFB0
	.4byte	.LFE0
	.4byte	.LLST0
	.4byte	0x120
	.uleb128 0xd
	.4byte	.LASF17
	.byte	0x1
	.byte	0x5
	.4byte	0x7a
	.4byte	.LLST1
	.byte	0
	.uleb128 0xc
	.4byte	0xba
	.byte	0x1
	.byte	0xa
	.4byte	.LFB1
	.4byte	.LFE1
	.4byte	.LLST2
	.4byte	0x147
	.uleb128 0xd
	.4byte	.LASF17
	.byte	0x1
	.byte	0xa
	.4byte	0x7a
	.4byte	.LLST3
	.byte	0
	.uleb128 0xe
	.4byte	0xd5
	.byte	0x1
	.byte	0xf
	.4byte	.LFB2
	.4byte	.LFE2
	.byte	0x2
	.byte	0x71
	.sleb128 0
	.uleb128 0xf
	.string	"buf"
	.byte	0x1
	.byte	0xf
	.4byte	0x62
	.4byte	.LLST4
	.uleb128 0x10
	.4byte	.LASF17
	.byte	0x1
	.byte	0xf
	.4byte	0x3d
	.byte	0x1
	.byte	0x54
	.uleb128 0x11
	.4byte	.Ldebug_ranges0+0
	.uleb128 0x12
	.string	"i"
	.byte	0x1
	.byte	0x11
	.4byte	0x3d
	.4byte	.LLST5
	.byte	0
	.byte	0
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
	.uleb128 0x3
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
	.uleb128 0x4
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x5
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x6
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
	.uleb128 0x7
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
	.uleb128 0x8
	.uleb128 0x2
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x9
	.uleb128 0x2e
	.byte	0x1
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
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3c
	.uleb128 0xc
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xa
	.uleb128 0x5
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xb
	.uleb128 0x2e
	.byte	0x1
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
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3c
	.uleb128 0xc
	.byte	0
	.byte	0
	.uleb128 0xc
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x47
	.uleb128 0x13
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x40
	.uleb128 0x6
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xd
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x6
	.byte	0
	.byte	0
	.uleb128 0xe
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x47
	.uleb128 0x13
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x40
	.uleb128 0xa
	.byte	0
	.byte	0
	.uleb128 0xf
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x6
	.byte	0
	.byte	0
	.uleb128 0x10
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0xa
	.byte	0
	.byte	0
	.uleb128 0x11
	.uleb128 0xb
	.byte	0x1
	.uleb128 0x55
	.uleb128 0x6
	.byte	0
	.byte	0
	.uleb128 0x12
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x6
	.byte	0
	.byte	0
	.byte	0
	.section	.debug_loc,"",@progbits
.Ldebug_loc0:
.LLST0:
	.4byte	.LFB0-.Ltext0
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
	.4byte	.LFE0-.Ltext0
	.2byte	0x2
	.byte	0x71
	.sleb128 0
	.4byte	0
	.4byte	0
.LLST1:
	.4byte	.LVL0-.Ltext0
	.4byte	.LVL1-.Ltext0
	.2byte	0x1
	.byte	0x53
	.4byte	.LVL1-.Ltext0
	.4byte	.LVL2-.Ltext0
	.2byte	0x3
	.byte	0x74
	.sleb128 -31
	.byte	0x9f
	.4byte	0
	.4byte	0
.LLST2:
	.4byte	.LFB1-.Ltext0
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
	.4byte	.LFE1-.Ltext0
	.2byte	0x2
	.byte	0x71
	.sleb128 0
	.4byte	0
	.4byte	0
.LLST3:
	.4byte	.LVL3-.Ltext0
	.4byte	.LVL4-.Ltext0
	.2byte	0x1
	.byte	0x53
	.4byte	.LVL4-.Ltext0
	.4byte	.LVL5-.Ltext0
	.2byte	0x3
	.byte	0x74
	.sleb128 -63
	.byte	0x9f
	.4byte	0
	.4byte	0
.LLST4:
	.4byte	.LVL6-.Ltext0
	.4byte	.LVL7-.Ltext0
	.2byte	0x1
	.byte	0x53
	.4byte	.LVL7-.Ltext0
	.4byte	.LFE2-.Ltext0
	.2byte	0x1
	.byte	0x5b
	.4byte	0
	.4byte	0
.LLST5:
	.4byte	.LVL6-.Ltext0
	.4byte	.LVL8-.Ltext0
	.2byte	0x2
	.byte	0x30
	.byte	0x9f
	.4byte	.LVL8-.Ltext0
	.4byte	.LVL9-.Ltext0
	.2byte	0x1
	.byte	0x59
	.4byte	.LVL10-.Ltext0
	.4byte	.LFE2-.Ltext0
	.2byte	0x1
	.byte	0x59
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
	.section	.debug_ranges,"",@progbits
.Ldebug_ranges0:
	.4byte	.LBB2-.Ltext0
	.4byte	.LBE2-.Ltext0
	.4byte	.LBB3-.Ltext0
	.4byte	.LBE3-.Ltext0
	.4byte	0
	.4byte	0
	.section	.debug_line,"",@progbits
.Ldebug_line0:
	.section	.debug_str,"MS",@progbits,1
.LASF5:
	.string	"long long int"
.LASF20:
	.string	"MemTools"
.LASF4:
	.string	"unsigned int"
.LASF7:
	.string	"long unsigned int"
.LASF6:
	.string	"long long unsigned int"
.LASF13:
	.string	"AllocateMemory_64"
.LASF16:
	.string	"bool"
.LASF19:
	.string	"d:/Projects/SaveGameManager/savegame-manager-gx/trunk/source/Memory/MemTools.cpp"
.LASF1:
	.string	"unsigned char"
.LASF8:
	.string	"char"
.LASF9:
	.string	"uint32_t"
.LASF18:
	.string	"GNU C++ 4.6.3"
.LASF12:
	.string	"AllocateMemory_32"
.LASF11:
	.string	"double"
.LASF22:
	.string	"_ZN8MemTools10MemIsEmptyEPci"
.LASF3:
	.string	"short unsigned int"
.LASF0:
	.string	"signed char"
.LASF17:
	.string	"size"
.LASF15:
	.string	"_ZN8MemTools17AllocateMemory_64Ej"
.LASF10:
	.string	"float"
.LASF21:
	.string	"MemIsEmpty"
.LASF2:
	.string	"short int"
.LASF14:
	.string	"_ZN8MemTools17AllocateMemory_32Ej"
	.ident	"GCC: (devkitPPC release 26) 4.6.3"
