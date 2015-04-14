	.file	"mem2.cpp"
	.section	".text"
.Ltext0:
	.align 2
	.globl MEM2_takeBigOnes
	.type	MEM2_takeBigOnes, @function
MEM2_takeBigOnes:
.LFB6:
	.file 1 "d:/Projects/SaveGameManager/savegame-manager-gx/trunk/source/Memory/mem2.cpp"
	.loc 1 31 0
	.cfi_startproc
.LVL0:
	.loc 1 32 0
	lis 9,.LANCHOR0@ha
	.loc 1 33 0
	.loc 1 32 0
	stb 3,.LANCHOR0@l(9)
	.loc 1 33 0
	blr
	.cfi_endproc
.LFE6:
	.size	MEM2_takeBigOnes, .-MEM2_takeBigOnes
	.align 2
	.globl MEM2_init
	.type	MEM2_init, @function
MEM2_init:
.LFB7:
	.loc 1 36 0
	.cfi_startproc
.LVL1:
	mflr 0
	mr 4,3
	.loc 1 37 0
	lis 3,.LANCHOR0@ha
.LVL2:
	.loc 1 36 0
	stwu 1,-8(1)
.LCFI0:
	.cfi_def_cfa_offset 8
	.cfi_register 65, 0
	.loc 1 37 0
	la 3,.LANCHOR0@l(3)
	addi 3,3,4
	.loc 1 36 0
	stw 0,12(1)
	.loc 1 37 0
	.cfi_offset 65, 4
	bl _ZN10CMEM2Alloc4initEj
.LVL3:
	.loc 1 38 0
	lwz 0,12(1)
	addi 1,1,8
.LCFI1:
	.cfi_def_cfa_offset 0
	mtlr 0
	blr
	.cfi_endproc
.LFE7:
	.size	MEM2_init, .-MEM2_init
	.align 2
	.globl MEM2_cleanup
	.type	MEM2_cleanup, @function
MEM2_cleanup:
.LFB8:
	.loc 1 41 0
	.cfi_startproc
	mflr 0
	.loc 1 42 0
	lis 3,.LANCHOR0@ha
	.loc 1 41 0
	stwu 1,-8(1)
.LCFI2:
	.cfi_def_cfa_offset 8
	.cfi_register 65, 0
	.loc 1 42 0
	la 3,.LANCHOR0@l(3)
	addi 3,3,4
	.loc 1 41 0
	stw 0,12(1)
	.loc 1 42 0
	.cfi_offset 65, 4
	bl _ZN10CMEM2Alloc7cleanupEv
	.loc 1 43 0
	lwz 0,12(1)
	addi 1,1,8
.LCFI3:
	.cfi_def_cfa_offset 0
	mtlr 0
	blr
	.cfi_endproc
.LFE8:
	.size	MEM2_cleanup, .-MEM2_cleanup
	.align 2
	.globl MEM2_alloc
	.type	MEM2_alloc, @function
MEM2_alloc:
.LFB9:
	.loc 1 46 0
	.cfi_startproc
.LVL4:
	mflr 0
	mr 4,3
	.loc 1 47 0
	lis 3,.LANCHOR0@ha
.LVL5:
	.loc 1 46 0
	stwu 1,-8(1)
.LCFI4:
	.cfi_def_cfa_offset 8
	.cfi_register 65, 0
	.loc 1 47 0
	la 3,.LANCHOR0@l(3)
	addi 3,3,4
	.loc 1 46 0
	stw 0,12(1)
	.loc 1 47 0
	.cfi_offset 65, 4
	bl _ZN10CMEM2Alloc8allocateEj
.LVL6:
	.loc 1 48 0
	lwz 0,12(1)
	addi 1,1,8
.LCFI5:
	.cfi_def_cfa_offset 0
	mtlr 0
	blr
	.cfi_endproc
.LFE9:
	.size	MEM2_alloc, .-MEM2_alloc
	.align 2
	.globl MEM2_free
	.type	MEM2_free, @function
MEM2_free:
.LFB10:
	.loc 1 51 0
	.cfi_startproc
.LVL7:
	mflr 0
	mr 4,3
	.loc 1 52 0
	lis 3,.LANCHOR0@ha
.LVL8:
	.loc 1 51 0
	stwu 1,-8(1)
.LCFI6:
	.cfi_def_cfa_offset 8
	.cfi_register 65, 0
	.loc 1 52 0
	la 3,.LANCHOR0@l(3)
	addi 3,3,4
	.loc 1 51 0
	stw 0,12(1)
	.loc 1 52 0
	.cfi_offset 65, 4
	bl _ZN10CMEM2Alloc7releaseEPv
.LVL9:
	.loc 1 53 0
	lwz 0,12(1)
	addi 1,1,8
.LCFI7:
	.cfi_def_cfa_offset 0
	mtlr 0
	blr
	.cfi_endproc
.LFE10:
	.size	MEM2_free, .-MEM2_free
	.align 2
	.globl MEM2_realloc
	.type	MEM2_realloc, @function
MEM2_realloc:
.LFB11:
	.loc 1 56 0
	.cfi_startproc
.LVL10:
	mflr 0
	stwu 1,-8(1)
.LCFI8:
	.cfi_def_cfa_offset 8
	.cfi_register 65, 0
	mr 5,4
	stw 0,12(1)
	.loc 1 56 0
	mr 0,3
	.cfi_offset 65, 4
	.loc 1 57 0
	lis 3,.LANCHOR0@ha
.LVL11:
	mr 4,0
.LVL12:
	la 3,.LANCHOR0@l(3)
	addi 3,3,4
	bl _ZN10CMEM2Alloc10reallocateEPvj
.LVL13:
	.loc 1 58 0
	lwz 0,12(1)
	addi 1,1,8
.LCFI9:
	.cfi_def_cfa_offset 0
	mtlr 0
	blr
	.cfi_endproc
.LFE11:
	.size	MEM2_realloc, .-MEM2_realloc
	.align 2
	.globl MEM2_usableSize
	.type	MEM2_usableSize, @function
MEM2_usableSize:
.LFB12:
	.loc 1 61 0
	.cfi_startproc
.LVL14:
	mflr 0
	stwu 1,-8(1)
.LCFI10:
	.cfi_def_cfa_offset 8
	.cfi_register 65, 0
	stw 0,12(1)
	.loc 1 62 0
	.cfi_offset 65, 4
	bl _ZN10CMEM2Alloc10usableSizeEPv
.LVL15:
	.loc 1 63 0
	lwz 0,12(1)
	addi 1,1,8
.LCFI11:
	.cfi_def_cfa_offset 0
	mtlr 0
	blr
	.cfi_endproc
.LFE12:
	.size	MEM2_usableSize, .-MEM2_usableSize
	.align 2
	.globl MEM2_freesize
	.type	MEM2_freesize, @function
MEM2_freesize:
.LFB13:
	.loc 1 66 0
	.cfi_startproc
	mflr 0
	.loc 1 67 0
	lis 3,.LANCHOR0@ha
	.loc 1 66 0
	stwu 1,-8(1)
.LCFI12:
	.cfi_def_cfa_offset 8
	.cfi_register 65, 0
	.loc 1 67 0
	la 3,.LANCHOR0@l(3)
	addi 3,3,4
	.loc 1 66 0
	stw 0,12(1)
	.loc 1 67 0
	.cfi_offset 65, 4
	bl _ZN10CMEM2Alloc8FreeSizeEv
	.loc 1 68 0
	lwz 0,12(1)
	addi 1,1,8
.LCFI13:
	.cfi_def_cfa_offset 0
	mtlr 0
	blr
	.cfi_endproc
.LFE13:
	.size	MEM2_freesize, .-MEM2_freesize
	.align 2
	.globl MEM1_alloc
	.type	MEM1_alloc, @function
MEM1_alloc:
.LFB14:
	.loc 1 71 0
	.cfi_startproc
.LVL16:
	mflr 0
	stwu 1,-8(1)
.LCFI14:
	.cfi_def_cfa_offset 8
	.cfi_register 65, 0
	stw 0,12(1)
	.loc 1 72 0
	.cfi_offset 65, 4
	bl __real_malloc
.LVL17:
	.loc 1 73 0
	lwz 0,12(1)
	addi 1,1,8
.LCFI15:
	.cfi_def_cfa_offset 0
	mtlr 0
	blr
	.cfi_endproc
.LFE14:
	.size	MEM1_alloc, .-MEM1_alloc
	.align 2
	.globl MEM1_memalign
	.type	MEM1_memalign, @function
MEM1_memalign:
.LFB15:
	.loc 1 76 0
	.cfi_startproc
.LVL18:
	mflr 0
	stwu 1,-8(1)
.LCFI16:
	.cfi_def_cfa_offset 8
	.cfi_register 65, 0
	stw 0,12(1)
	.loc 1 77 0
	.cfi_offset 65, 4
	bl __real_memalign
.LVL19:
	.loc 1 78 0
	lwz 0,12(1)
	addi 1,1,8
.LCFI17:
	.cfi_def_cfa_offset 0
	mtlr 0
	blr
	.cfi_endproc
.LFE15:
	.size	MEM1_memalign, .-MEM1_memalign
	.align 2
	.globl MEM1_free
	.type	MEM1_free, @function
MEM1_free:
.LFB16:
	.loc 1 81 0
	.cfi_startproc
.LVL20:
	mflr 0
	stwu 1,-8(1)
.LCFI18:
	.cfi_def_cfa_offset 8
	.cfi_register 65, 0
	stw 0,12(1)
	.loc 1 82 0
	.cfi_offset 65, 4
	bl __real_free
.LVL21:
	.loc 1 83 0
	lwz 0,12(1)
	addi 1,1,8
.LCFI19:
	.cfi_def_cfa_offset 0
	mtlr 0
	blr
	.cfi_endproc
.LFE16:
	.size	MEM1_free, .-MEM1_free
	.align 2
	.globl MEM1_realloc
	.type	MEM1_realloc, @function
MEM1_realloc:
.LFB17:
	.loc 1 86 0
	.cfi_startproc
.LVL22:
	mflr 0
	stwu 1,-8(1)
.LCFI20:
	.cfi_def_cfa_offset 8
	.cfi_register 65, 0
	stw 0,12(1)
	.loc 1 87 0
	.cfi_offset 65, 4
	bl __real_realloc
.LVL23:
	.loc 1 88 0
	lwz 0,12(1)
	addi 1,1,8
.LCFI21:
	.cfi_def_cfa_offset 0
	mtlr 0
	blr
	.cfi_endproc
.LFE17:
	.size	MEM1_realloc, .-MEM1_realloc
	.align 2
	.globl __wrap_malloc
	.type	__wrap_malloc, @function
__wrap_malloc:
.LFB18:
	.loc 1 91 0
	.cfi_startproc
.LVL24:
	mflr 0
	stwu 1,-16(1)
.LCFI22:
	.cfi_def_cfa_offset 16
	.cfi_register 65, 0
	stw 30,8(1)
	lis 30,.LANCHOR0@ha
	.cfi_offset 30, -8
	stw 0,20(1)
	stw 31,12(1)
	.loc 1 91 0
	mr 31,3
	.cfi_offset 31, -4
	.cfi_offset 65, 4
.LBB14:
	.loc 1 93 0
	bl SYS_GetArena1Lo
.LVL25:
	lis 0,0x8170
	subf 0,31,0
	cmplw 7,3,0
	ble- 7,.L21
.L14:
	.loc 1 95 0 discriminator 5
	la 3,.LANCHOR0@l(30)
	mr 4,31
	addi 3,3,4
	bl _ZN10CMEM2Alloc8allocateEj
.LVL26:
	.loc 1 96 0 discriminator 5
	cmpwi 0,3,0
	beq- 0,.L22
.LVL27:
.L16:
.LBE14:
	.loc 1 106 0
	lwz 0,20(1)
	lwz 30,8(1)
	mtlr 0
	lwz 31,12(1)
.LVL28:
	addi 1,1,16
	.cfi_remember_state
.LCFI23:
	.cfi_def_cfa_offset 0
	.cfi_restore 31
	.cfi_restore 30
	blr
.LVL29:
.L21:
.LCFI24:
	.cfi_restore_state
.LBB15:
	.loc 1 93 0 discriminator 2
	lbz 0,.LANCHOR0@l(30)
	cmpwi 7,0,0
	beq- 7,.L15
	.loc 1 93 0 is_stmt 0 discriminator 3
	cmplwi 7,31,30720
	bgt- 7,.L14
.L15:
	.loc 1 101 0 is_stmt 1
	mr 3,31
	bl __real_malloc
.LVL30:
	.loc 1 102 0
	cmpwi 0,3,0
	bne+ 0,.L16
	.loc 1 105 0
	la 3,.LANCHOR0@l(30)
.LVL31:
	mr 4,31
	addi 3,3,4
	bl _ZN10CMEM2Alloc8allocateEj
	b .L16
.LVL32:
.L22:
	.loc 1 99 0
	mr 3,31
.LVL33:
	bl __real_malloc
	b .L16
.LBE15:
	.cfi_endproc
.LFE18:
	.size	__wrap_malloc, .-__wrap_malloc
	.align 2
	.globl __wrap_calloc
	.type	__wrap_calloc, @function
__wrap_calloc:
.LFB19:
	.loc 1 109 0
	.cfi_startproc
.LVL34:
	mflr 0
	stwu 1,-32(1)
.LCFI25:
	.cfi_def_cfa_offset 32
	.cfi_register 65, 0
	stw 28,16(1)
	mr 28,3
	.cfi_offset 28, -16
	stw 0,36(1)
	stw 29,20(1)
	lis 29,.LANCHOR0@ha
	.cfi_offset 29, -12
	.cfi_offset 65, 4
	stw 30,24(1)
	mr 30,4
	.cfi_offset 30, -8
	stw 31,28(1)
.LBB16:
	.loc 1 111 0
	.cfi_offset 31, -4
	bl SYS_GetArena1Lo
.LVL35:
	lis 0,0x8170
	subf 0,30,0
	cmplw 7,3,0
	ble- 7,.L31
.L24:
	.loc 1 113 0 discriminator 5
	mullw 5,30,28
	la 3,.LANCHOR0@l(29)
	addi 3,3,4
	mr 4,5
	stw 5,8(1)
	bl _ZN10CMEM2Alloc8allocateEj
.LVL36:
	.loc 1 114 0 discriminator 5
	lwz 5,8(1)
	mr. 31,3
	beq- 0,.L26
	.loc 1 116 0
	li 4,0
	bl memset
.LVL37:
.LBE16:
	.loc 1 130 0
	lwz 0,36(1)
	mr 3,31
	lwz 28,16(1)
.LVL38:
	mtlr 0
	lwz 29,20(1)
	lwz 30,24(1)
.LVL39:
	lwz 31,28(1)
.LVL40:
	addi 1,1,32
	.cfi_remember_state
	.cfi_restore 28
	.cfi_restore 29
	.cfi_restore 30
	.cfi_restore 31
.LCFI26:
	.cfi_def_cfa_offset 0
	blr
.LVL41:
.L31:
.LCFI27:
	.cfi_restore_state
.LBB17:
	.loc 1 111 0 discriminator 2
	lbz 0,.LANCHOR0@l(29)
	cmpwi 7,0,0
	bne- 7,.L32
.L25:
	.loc 1 121 0
	mr 3,28
	mr 4,30
	bl __real_calloc
.LVL42:
	.loc 1 122 0
	mr. 31,3
	beq- 0,.L33
.LVL43:
.L27:
.LBE17:
	.loc 1 130 0
	lwz 0,36(1)
	mr 3,31
	lwz 28,16(1)
.LVL44:
	mtlr 0
	lwz 29,20(1)
	lwz 30,24(1)
	lwz 31,28(1)
	addi 1,1,32
	.cfi_remember_state
.LCFI28:
	.cfi_def_cfa_offset 0
	.cfi_restore 31
	.cfi_restore 30
	.cfi_restore 29
	.cfi_restore 28
	blr
.LVL45:
.L32:
.LCFI29:
	.cfi_restore_state
.LBB18:
	.loc 1 111 0 discriminator 3
	cmplwi 7,30,30720
	ble- 7,.L25
	b .L24
.LVL46:
.L26:
	.loc 1 119 0
	mr 3,28
.LVL47:
	mr 4,30
	bl __real_calloc
	mr 31,3
.LVL48:
	b .L27
.LVL49:
.L33:
	.loc 1 125 0
	mullw 30,30,28
.LVL50:
	la 3,.LANCHOR0@l(29)
.LVL51:
	addi 3,3,4
	mr 4,30
	bl _ZN10CMEM2Alloc8allocateEj
.LVL52:
	.loc 1 126 0
	mr. 31,3
	beq- 0,.L27
	.loc 1 127 0
	li 4,0
	mr 5,30
	bl memset
.LVL53:
	b .L27
.LBE18:
	.cfi_endproc
.LFE19:
	.size	__wrap_calloc, .-__wrap_calloc
	.align 2
	.globl __wrap_memalign
	.type	__wrap_memalign, @function
__wrap_memalign:
.LFB20:
	.loc 1 133 0
	.cfi_startproc
.LVL54:
	mflr 0
	stwu 1,-24(1)
.LCFI30:
	.cfi_def_cfa_offset 24
	.cfi_register 65, 0
	stw 30,16(1)
	mr 30,3
	.cfi_offset 30, -8
	stw 0,28(1)
	stw 31,20(1)
	mr 31,4
	.cfi_offset 31, -4
	.cfi_offset 65, 4
	stw 29,12(1)
.LBB19:
	.loc 1 135 0
	.cfi_offset 29, -12
	bl SYS_GetArena1Lo
.LVL55:
	lis 0,0x8170
	subf 0,31,0
	cmplw 7,3,0
	ble- 7,.L42
.L35:
	.loc 1 137 0 discriminator 5
	cmplwi 7,30,32
	bgt- 7,.L37
	.loc 1 137 0 is_stmt 0 discriminator 1
	li 0,32
	divwu 0,0,30
	mullw 0,0,30
	cmpwi 7,0,32
	beq- 7,.L43
.L37:
	.loc 1 144 0 is_stmt 1
	mr 3,30
	mr 4,31
	bl __real_memalign
.LVL56:
.L38:
.LBE19:
	.loc 1 152 0
	lwz 0,28(1)
	lwz 29,12(1)
	mtlr 0
	lwz 30,16(1)
	lwz 31,20(1)
.LVL57:
	addi 1,1,24
	.cfi_remember_state
.LCFI31:
	.cfi_def_cfa_offset 0
	.cfi_restore 31
	.cfi_restore 30
	.cfi_restore 29
	blr
.LVL58:
.L42:
.LCFI32:
	.cfi_restore_state
.LBB20:
	.loc 1 135 0 discriminator 2
	lis 29,.LANCHOR0@ha
	lbz 0,.LANCHOR0@l(29)
	cmpwi 7,0,0
	beq- 7,.L36
	.loc 1 135 0 is_stmt 0 discriminator 3
	cmplwi 7,31,30720
	bgt- 7,.L35
.L36:
	.loc 1 146 0 is_stmt 1
	mr 3,30
	mr 4,31
	bl __real_memalign
.LVL59:
	.loc 1 147 0
	cmpwi 0,3,0
	bne+ 0,.L38
	.loc 1 147 0 is_stmt 0 discriminator 1
	cmplwi 7,30,32
	bgt- 7,.L38
	.loc 1 147 0 discriminator 2
	li 0,32
	divwu 0,0,30
	mullw 30,0,30
.LVL60:
	cmpwi 7,30,32
	bne+ 7,.L38
	.loc 1 151 0 is_stmt 1
	la 3,.LANCHOR0@l(29)
.LVL61:
	mr 4,31
	addi 3,3,4
	bl _ZN10CMEM2Alloc8allocateEj
	b .L38
.LVL62:
.L43:
	.loc 1 139 0
	lis 3,.LANCHOR0@ha
	mr 4,31
	la 3,.LANCHOR0@l(3)
	addi 3,3,4
	bl _ZN10CMEM2Alloc8allocateEj
.LVL63:
	.loc 1 140 0
	cmpwi 0,3,0
	beq- 0,.L37
.LBE20:
	.loc 1 152 0
	lwz 0,28(1)
	lwz 29,12(1)
	mtlr 0
	lwz 30,16(1)
.LVL64:
	lwz 31,20(1)
.LVL65:
	addi 1,1,24
	.cfi_restore 29
	.cfi_restore 30
	.cfi_restore 31
.LCFI33:
	.cfi_def_cfa_offset 0
	blr
	.cfi_endproc
.LFE20:
	.size	__wrap_memalign, .-__wrap_memalign
	.align 2
	.globl __wrap_free
	.type	__wrap_free, @function
__wrap_free:
.LFB21:
	.loc 1 155 0
	.cfi_startproc
.LVL66:
	.loc 1 156 0
	mr. 4,3
	.loc 1 155 0
	mflr 0
	stwu 1,-8(1)
.LCFI34:
	.cfi_def_cfa_offset 8
	.cfi_register 65, 0
	stw 0,12(1)
	.loc 1 156 0
	beq- 0,.L44
	.cfi_offset 65, 4
	.loc 1 159 0
	andis. 0,4,4096
	bne- 0,.L47
	.loc 1 165 0
	bl __real_free
.LVL67:
.L44:
	.loc 1 167 0
	lwz 0,12(1)
	addi 1,1,8
	.cfi_remember_state
.LCFI35:
	.cfi_def_cfa_offset 0
	mtlr 0
	blr
.LVL68:
.L47:
.LCFI36:
	.cfi_restore_state
	.loc 1 161 0
	lis 3,.LANCHOR0@ha
.LVL69:
	la 3,.LANCHOR0@l(3)
	addi 3,3,4
	bl _ZN10CMEM2Alloc7releaseEPv
.LVL70:
	.loc 1 167 0
	lwz 0,12(1)
	addi 1,1,8
.LCFI37:
	.cfi_def_cfa_offset 0
	mtlr 0
	blr
	.cfi_endproc
.LFE21:
	.size	__wrap_free, .-__wrap_free
	.align 2
	.globl __wrap_realloc
	.type	__wrap_realloc, @function
__wrap_realloc:
.LFB22:
	.loc 1 170 0
	.cfi_startproc
.LVL71:
	mflr 0
	stwu 1,-32(1)
.LCFI38:
	.cfi_def_cfa_offset 32
	.cfi_register 65, 0
	mfcr 12
	stw 28,16(1)
	lis 28,.LANCHOR0@ha
	.cfi_offset 28, -16
	.cfi_register 70, 12
	stw 0,36(1)
.LBB21:
	.loc 1 173 0
	andis. 0,3,4096
	.cfi_offset 65, 4
.LBE21:
	.loc 1 170 0
	stw 30,24(1)
	mr 30,4
	.cfi_offset 30, -8
	stw 31,28(1)
	mr 31,3
	.cfi_offset 31, -4
	stw 29,20(1)
	stw 12,12(1)
.LBB26:
	.loc 1 173 0
	bne- 0,.L49
	.cfi_offset 70, -20
	.cfi_offset 29, -12
	.loc 1 173 0 is_stmt 0 discriminator 1
	cmpwi 4,3,0
	beq- 4,.L56
.L50:
	.loc 1 191 0 is_stmt 1
	mr 3,31
.LVL72:
	mr 4,30
.LVL73:
	bl __real_realloc
.LVL74:
	.loc 1 192 0
	mr. 29,3
	beq- 0,.L57
.LVL75:
.L51:
.LBE26:
	.loc 1 205 0
	lwz 0,36(1)
	mr 3,29
	lwz 12,12(1)
	mtlr 0
	lwz 28,16(1)
	lwz 29,20(1)
.LVL76:
	mtcrf 8,12
	lwz 30,24(1)
	lwz 31,28(1)
.LVL77:
	addi 1,1,32
	.cfi_remember_state
.LCFI39:
	.cfi_def_cfa_offset 0
	.cfi_restore 70
	.cfi_restore 31
	.cfi_restore 30
	.cfi_restore 29
	.cfi_restore 28
	blr
.LVL78:
.L56:
.LCFI40:
	.cfi_restore_state
.LBB27:
	.loc 1 173 0 discriminator 2
	lis 28,.LANCHOR0@ha
	lbz 0,.LANCHOR0@l(28)
	cmpwi 7,0,0
	beq- 7,.L50
	.loc 1 173 0 is_stmt 0 discriminator 3
	cmplwi 7,4,30720
	ble- 7,.L50
.L49:
	.loc 1 175 0 is_stmt 1
	la 28,.LANCHOR0@l(28)
	mr 4,31
.LVL79:
	addi 28,28,4
	mr 5,30
	mr 3,28
.LVL80:
	bl _ZN10CMEM2Alloc10reallocateEPvj
.LVL81:
	.loc 1 176 0
	mr. 29,3
	bne+ 0,.L51
	.loc 1 179 0
	mr 3,30
.LVL82:
	bl __real_malloc
.LVL83:
	.loc 1 180 0
	mr. 29,3
	beq- 0,.L51
	.loc 1 183 0
	cmpwi 7,31,0
	beq- 7,.L51
.LVL84:
.LBB22:
.LBB23:
	.loc 1 62 0
	mr 3,31
.LVL85:
	bl _ZN10CMEM2Alloc10usableSizeEPv
.LBE23:
.LBE22:
	.loc 1 185 0
	cmplw 7,30,3
	ble+ 7,.L52
.LVL86:
.LBB24:
.LBB25:
	.loc 1 62 0
	mr 3,31
	bl _ZN10CMEM2Alloc10usableSizeEPv
	mr 30,3
.LVL87:
.L52:
.LBE25:
.LBE24:
	.loc 1 185 0 discriminator 3
	mr 4,31
	mr 3,29
	mr 5,30
	bl memcpy
	.loc 1 186 0 discriminator 3
	mr 3,28
	mr 4,31
	bl _ZN10CMEM2Alloc7releaseEPv
	b .L51
.LVL88:
.L57:
	.loc 1 195 0
	lis 3,.LANCHOR0@ha
.LVL89:
	mr 4,30
	la 3,.LANCHOR0@l(3)
	addi 3,3,4
	bl _ZN10CMEM2Alloc8allocateEj
.LVL90:
	.loc 1 196 0
	mr. 29,3
	beq- 0,.L51
	.loc 1 199 0
	beq- 4,.L51
	.loc 1 201 0
	mr 3,31
.LVL91:
	bl __real_malloc_usable_size
	cmplw 7,30,3
	bgt- 7,.L58
.LVL92:
.L53:
	.loc 1 201 0 is_stmt 0 discriminator 3
	mr 3,29
	mr 4,31
	mr 5,30
	bl memcpy
	.loc 1 202 0 is_stmt 1 discriminator 3
	mr 3,31
	bl __real_free
	b .L51
.LVL93:
.L58:
	.loc 1 201 0 discriminator 1
	mr 3,31
	bl __real_malloc_usable_size
	mr 30,3
.LVL94:
	b .L53
.LBE27:
	.cfi_endproc
.LFE22:
	.size	__wrap_realloc, .-__wrap_realloc
	.align 2
	.globl __wrap_malloc_usable_size
	.type	__wrap_malloc_usable_size, @function
__wrap_malloc_usable_size:
.LFB23:
	.loc 1 208 0
	.cfi_startproc
.LVL95:
	mflr 0
	stwu 1,-8(1)
.LCFI41:
	.cfi_def_cfa_offset 8
	.cfi_register 65, 0
	stw 0,12(1)
	.loc 1 209 0
	andis. 0,3,4096
	.cfi_offset 65, 4
	bne- 0,.L62
	.loc 1 211 0
	bl __real_malloc_usable_size
.LVL96:
	.loc 1 212 0
	lwz 0,12(1)
	addi 1,1,8
	.cfi_remember_state
.LCFI42:
	.cfi_def_cfa_offset 0
	mtlr 0
	blr
.LVL97:
.L62:
.LCFI43:
	.cfi_restore_state
	.loc 1 210 0
	bl _ZN10CMEM2Alloc10usableSizeEPv
.LVL98:
	.loc 1 212 0
	lwz 0,12(1)
	addi 1,1,8
.LCFI44:
	.cfi_def_cfa_offset 0
	mtlr 0
	blr
	.cfi_endproc
.LFE23:
	.size	__wrap_malloc_usable_size, .-__wrap_malloc_usable_size
	.section	.text.startup,"ax",@progbits
	.align 2
	.type	_GLOBAL__sub_I_MALLOC_MEM2, @function
_GLOBAL__sub_I_MALLOC_MEM2:
.LFB25:
	.loc 1 214 0
	.cfi_startproc
.LVL99:
.LBB28:
.LBB29:
.LBB30:
	.file 2 "d:/Projects/SaveGameManager/savegame-manager-gx/trunk/source/Memory/mem2alloc.hpp"
	.loc 2 25 0
	lis 9,.LANCHOR0@ha
	li 0,0
	la 9,.LANCHOR0@l(9)
	stw 0,4(9)
	stw 0,8(9)
	stw 0,12(9)
	stw 0,16(9)
.LBE30:
.LBE29:
.LBE28:
	.loc 1 214 0
	blr
	.cfi_endproc
.LFE25:
	.size	_GLOBAL__sub_I_MALLOC_MEM2, .-_GLOBAL__sub_I_MALLOC_MEM2
	.section	.ctors,"aw",@progbits
	.align 2
	.long	_GLOBAL__sub_I_MALLOC_MEM2
	.globl MALLOC_MEM2
	.section	.sbss,"aw",@nobits
	.align 2
	.type	MALLOC_MEM2, @object
	.size	MALLOC_MEM2, 4
MALLOC_MEM2:
	.zero	4
	.section	".bss"
	.align 2
	.set	.LANCHOR0,. + 0
	.type	_ZL15g_bigGoesToMem2, @object
	.size	_ZL15g_bigGoesToMem2, 1
_ZL15g_bigGoesToMem2:
	.zero	1
	.zero	3
	.type	_ZL8g_mem2gp, @object
	.size	_ZL8g_mem2gp, 16
_ZL8g_mem2gp:
	.zero	16
	.section	".text"
.Letext0:
	.file 3 "d:\\devkitpro\\devkitppc\\bin\\../lib/gcc/powerpc-eabi/4.6.3/../../../../powerpc-eabi/include/stdint.h"
	.file 4 "d:/devkitPro/libogc/include/ogc/mutex.h"
	.file 5 "d:\\devkitpro\\devkitppc\\bin\\../lib/gcc/powerpc-eabi/4.6.3/include/stddef.h"
	.file 6 "d:/devkitPro/libogc/include/gctypes.h"
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.4byte	0x796
	.2byte	0x2
	.4byte	.Ldebug_abbrev0
	.byte	0x4
	.uleb128 0x1
	.4byte	.LASF68
	.byte	0x4
	.4byte	.LASF69
	.4byte	0
	.4byte	0
	.4byte	.Ldebug_ranges0+0x70
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
	.uleb128 0x4
	.4byte	.LASF7
	.byte	0x3
	.byte	0x50
	.4byte	0x53
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
	.uleb128 0x5
	.string	"u32"
	.byte	0x6
	.byte	0x13
	.4byte	0x48
	.uleb128 0x2
	.byte	0x4
	.byte	0x4
	.4byte	.LASF8
	.uleb128 0x2
	.byte	0x8
	.byte	0x4
	.4byte	.LASF9
	.uleb128 0x2
	.byte	0x1
	.byte	0x8
	.4byte	.LASF10
	.uleb128 0x4
	.4byte	.LASF11
	.byte	0x4
	.byte	0x33
	.4byte	0x68
	.uleb128 0x6
	.4byte	.LASF41
	.byte	0x10
	.byte	0x2
	.byte	0x9
	.4byte	0x2e1
	.uleb128 0x7
	.4byte	.LASF70
	.byte	0x20
	.byte	0x2
	.byte	0x1b
	.byte	0x3
	.4byte	0xe1
	.uleb128 0x8
	.string	"s"
	.byte	0x2
	.byte	0x1d
	.4byte	0x53
	.byte	0x2
	.byte	0x23
	.uleb128 0
	.uleb128 0x9
	.4byte	.LASF12
	.byte	0x2
	.byte	0x1e
	.4byte	0x2e1
	.byte	0x2
	.byte	0x23
	.uleb128 0x4
	.uleb128 0x9
	.4byte	.LASF13
	.byte	0x2
	.byte	0x1f
	.4byte	0x2e1
	.byte	0x2
	.byte	0x23
	.uleb128 0x8
	.uleb128 0x8
	.string	"f"
	.byte	0x2
	.byte	0x20
	.4byte	0x2e7
	.byte	0x2
	.byte	0x23
	.uleb128 0xc
	.byte	0
	.uleb128 0xa
	.4byte	.LASF14
	.byte	0x2
	.byte	0x22
	.4byte	0x2ee
	.byte	0x2
	.byte	0x23
	.uleb128 0
	.byte	0x3
	.uleb128 0xa
	.4byte	.LASF15
	.byte	0x2
	.byte	0x23
	.4byte	0x2ee
	.byte	0x2
	.byte	0x23
	.uleb128 0x4
	.byte	0x3
	.uleb128 0xa
	.4byte	.LASF16
	.byte	0x2
	.byte	0x24
	.4byte	0x2ee
	.byte	0x2
	.byte	0x23
	.uleb128 0x8
	.byte	0x3
	.uleb128 0xa
	.4byte	.LASF17
	.byte	0x2
	.byte	0x25
	.4byte	0x88
	.byte	0x2
	.byte	0x23
	.uleb128 0xc
	.byte	0x3
	.uleb128 0xb
	.byte	0x1
	.4byte	.LASF18
	.byte	0x2
	.byte	0xc
	.4byte	.LASF20
	.4byte	0x2f4
	.byte	0x1
	.4byte	0x136
	.4byte	0x142
	.uleb128 0xc
	.4byte	0x2f6
	.byte	0x1
	.uleb128 0xd
	.4byte	0x53
	.byte	0
	.uleb128 0xe
	.byte	0x1
	.4byte	.LASF22
	.byte	0x2
	.byte	0xd
	.4byte	.LASF24
	.byte	0x1
	.4byte	0x157
	.4byte	0x163
	.uleb128 0xc
	.4byte	0x2f6
	.byte	0x1
	.uleb128 0xd
	.4byte	0x2f4
	.byte	0
	.uleb128 0xb
	.byte	0x1
	.4byte	.LASF19
	.byte	0x2
	.byte	0xe
	.4byte	.LASF21
	.4byte	0x2f4
	.byte	0x1
	.4byte	0x17c
	.4byte	0x18d
	.uleb128 0xc
	.4byte	0x2f6
	.byte	0x1
	.uleb128 0xd
	.4byte	0x2f4
	.uleb128 0xd
	.4byte	0x53
	.byte	0
	.uleb128 0xe
	.byte	0x1
	.4byte	.LASF23
	.byte	0x2
	.byte	0xf
	.4byte	.LASF25
	.byte	0x1
	.4byte	0x1a2
	.4byte	0x1ae
	.uleb128 0xc
	.4byte	0x2f6
	.byte	0x1
	.uleb128 0xd
	.4byte	0x53
	.byte	0
	.uleb128 0xe
	.byte	0x1
	.4byte	.LASF23
	.byte	0x2
	.byte	0x10
	.4byte	.LASF26
	.byte	0x1
	.4byte	0x1c3
	.4byte	0x1d4
	.uleb128 0xc
	.4byte	0x2f6
	.byte	0x1
	.uleb128 0xd
	.4byte	0x2f4
	.uleb128 0xd
	.4byte	0x2f4
	.byte	0
	.uleb128 0xe
	.byte	0x1
	.4byte	.LASF27
	.byte	0x2
	.byte	0x11
	.4byte	.LASF28
	.byte	0x1
	.4byte	0x1e9
	.4byte	0x1f0
	.uleb128 0xc
	.4byte	0x2f6
	.byte	0x1
	.byte	0
	.uleb128 0xe
	.byte	0x1
	.4byte	.LASF29
	.byte	0x2
	.byte	0x12
	.4byte	.LASF30
	.byte	0x1
	.4byte	0x205
	.4byte	0x20c
	.uleb128 0xc
	.4byte	0x2f6
	.byte	0x1
	.byte	0
	.uleb128 0xf
	.byte	0x1
	.4byte	.LASF31
	.byte	0x2
	.byte	0x13
	.4byte	.LASF32
	.4byte	0x53
	.byte	0x1
	.4byte	0x227
	.uleb128 0xd
	.4byte	0x2f4
	.byte	0
	.uleb128 0xe
	.byte	0x1
	.4byte	.LASF33
	.byte	0x2
	.byte	0x14
	.4byte	.LASF34
	.byte	0x1
	.4byte	0x23c
	.4byte	0x248
	.uleb128 0xc
	.4byte	0x2f6
	.byte	0x1
	.uleb128 0xd
	.4byte	0x2f4
	.byte	0
	.uleb128 0xb
	.byte	0x1
	.4byte	.LASF35
	.byte	0x2
	.byte	0x15
	.4byte	.LASF36
	.4byte	0x2f4
	.byte	0x1
	.4byte	0x261
	.4byte	0x268
	.uleb128 0xc
	.4byte	0x2fc
	.byte	0x1
	.byte	0
	.uleb128 0xe
	.byte	0x1
	.4byte	.LASF37
	.byte	0x2
	.byte	0x16
	.4byte	.LASF38
	.byte	0x1
	.4byte	0x27d
	.4byte	0x28e
	.uleb128 0xc
	.4byte	0x2fc
	.byte	0x1
	.uleb128 0xd
	.4byte	0x307
	.uleb128 0xd
	.4byte	0x30d
	.byte	0
	.uleb128 0xb
	.byte	0x1
	.4byte	.LASF39
	.byte	0x2
	.byte	0x17
	.4byte	.LASF40
	.4byte	0x53
	.byte	0x1
	.4byte	0x2a7
	.4byte	0x2ae
	.uleb128 0xc
	.4byte	0x2f6
	.byte	0x1
	.byte	0
	.uleb128 0x10
	.byte	0x1
	.4byte	.LASF41
	.byte	0x2
	.byte	0x19
	.byte	0x1
	.4byte	0x2bf
	.4byte	0x2c6
	.uleb128 0xc
	.4byte	0x2f6
	.byte	0x1
	.byte	0
	.uleb128 0x11
	.byte	0x1
	.4byte	.LASF41
	.byte	0x2
	.byte	0x27
	.byte	0x3
	.byte	0x1
	.4byte	0x2d4
	.uleb128 0xc
	.4byte	0x2f6
	.byte	0x1
	.uleb128 0xd
	.4byte	0x313
	.byte	0
	.byte	0
	.uleb128 0x12
	.byte	0x4
	.4byte	0x9f
	.uleb128 0x2
	.byte	0x1
	.byte	0x2
	.4byte	.LASF42
	.uleb128 0x12
	.byte	0x4
	.4byte	0x9f
	.uleb128 0x13
	.byte	0x4
	.uleb128 0x12
	.byte	0x4
	.4byte	0x93
	.uleb128 0x12
	.byte	0x4
	.4byte	0x302
	.uleb128 0x14
	.4byte	0x93
	.uleb128 0x15
	.byte	0x4
	.4byte	0x2f4
	.uleb128 0x15
	.byte	0x4
	.4byte	0x53
	.uleb128 0x15
	.byte	0x4
	.4byte	0x319
	.uleb128 0x14
	.4byte	0x93
	.uleb128 0x2
	.byte	0x4
	.byte	0x7
	.4byte	.LASF43
	.uleb128 0x4
	.4byte	.LASF44
	.byte	0x5
	.byte	0xd4
	.4byte	0x53
	.uleb128 0x2
	.byte	0x4
	.byte	0x5
	.4byte	.LASF45
	.uleb128 0x16
	.4byte	0x2ae
	.byte	0x3
	.4byte	0x345
	.4byte	0x350
	.uleb128 0x17
	.4byte	.LASF71
	.4byte	0x350
	.byte	0x1
	.byte	0
	.uleb128 0x14
	.4byte	0x2f6
	.uleb128 0x18
	.byte	0x1
	.4byte	.LASF46
	.byte	0x1
	.byte	0x3c
	.4byte	0x53
	.byte	0x1
	.4byte	0x370
	.uleb128 0x19
	.string	"p"
	.byte	0x1
	.byte	0x3c
	.4byte	0x2f4
	.byte	0
	.uleb128 0x1a
	.4byte	.LASF72
	.byte	0x1
	.byte	0x1
	.4byte	0x392
	.uleb128 0x1b
	.4byte	.LASF47
	.byte	0x1
	.byte	0xd6
	.4byte	0x41
	.uleb128 0x1b
	.4byte	.LASF48
	.byte	0x1
	.byte	0xd6
	.4byte	0x41
	.byte	0
	.uleb128 0x1c
	.byte	0x1
	.4byte	.LASF49
	.byte	0x1
	.byte	0x1e
	.4byte	.LFB6
	.4byte	.LFE6
	.byte	0x2
	.byte	0x71
	.sleb128 0
	.4byte	0x3b5
	.uleb128 0x1d
	.string	"b"
	.byte	0x1
	.byte	0x1e
	.4byte	0x2e7
	.byte	0x1
	.byte	0x53
	.byte	0
	.uleb128 0x1e
	.byte	0x1
	.4byte	.LASF50
	.byte	0x1
	.byte	0x23
	.4byte	.LFB7
	.4byte	.LFE7
	.4byte	.LLST0
	.4byte	0x3dd
	.uleb128 0x1f
	.4byte	.LASF51
	.byte	0x1
	.byte	0x23
	.4byte	0x53
	.4byte	.LLST1
	.byte	0
	.uleb128 0x20
	.byte	0x1
	.4byte	.LASF73
	.byte	0x1
	.byte	0x28
	.4byte	.LFB8
	.4byte	.LFE8
	.4byte	.LLST2
	.uleb128 0x21
	.byte	0x1
	.4byte	.LASF52
	.byte	0x1
	.byte	0x2d
	.4byte	0x2f4
	.4byte	.LFB9
	.4byte	.LFE9
	.4byte	.LLST3
	.4byte	0x41b
	.uleb128 0x22
	.string	"s"
	.byte	0x1
	.byte	0x2d
	.4byte	0x53
	.4byte	.LLST4
	.byte	0
	.uleb128 0x1e
	.byte	0x1
	.4byte	.LASF53
	.byte	0x1
	.byte	0x32
	.4byte	.LFB10
	.4byte	.LFE10
	.4byte	.LLST5
	.4byte	0x441
	.uleb128 0x22
	.string	"p"
	.byte	0x1
	.byte	0x32
	.4byte	0x2f4
	.4byte	.LLST6
	.byte	0
	.uleb128 0x21
	.byte	0x1
	.4byte	.LASF54
	.byte	0x1
	.byte	0x37
	.4byte	0x2f4
	.4byte	.LFB11
	.4byte	.LFE11
	.4byte	.LLST7
	.4byte	0x478
	.uleb128 0x22
	.string	"p"
	.byte	0x1
	.byte	0x37
	.4byte	0x2f4
	.4byte	.LLST8
	.uleb128 0x22
	.string	"s"
	.byte	0x1
	.byte	0x37
	.4byte	0x53
	.4byte	.LLST9
	.byte	0
	.uleb128 0x23
	.4byte	0x355
	.4byte	.LFB12
	.4byte	.LFE12
	.4byte	.LLST10
	.4byte	0x497
	.uleb128 0x24
	.4byte	0x366
	.4byte	.LLST11
	.byte	0
	.uleb128 0x25
	.byte	0x1
	.4byte	.LASF74
	.byte	0x1
	.byte	0x41
	.4byte	0x53
	.4byte	.LFB13
	.4byte	.LFE13
	.4byte	.LLST12
	.uleb128 0x21
	.byte	0x1
	.4byte	.LASF55
	.byte	0x1
	.byte	0x46
	.4byte	0x2f4
	.4byte	.LFB14
	.4byte	.LFE14
	.4byte	.LLST13
	.4byte	0x4d9
	.uleb128 0x22
	.string	"s"
	.byte	0x1
	.byte	0x46
	.4byte	0x53
	.4byte	.LLST14
	.byte	0
	.uleb128 0x21
	.byte	0x1
	.4byte	.LASF56
	.byte	0x1
	.byte	0x4b
	.4byte	0x2f4
	.4byte	.LFB15
	.4byte	.LFE15
	.4byte	.LLST15
	.4byte	0x510
	.uleb128 0x22
	.string	"a"
	.byte	0x1
	.byte	0x4b
	.4byte	0x53
	.4byte	.LLST16
	.uleb128 0x22
	.string	"s"
	.byte	0x1
	.byte	0x4b
	.4byte	0x53
	.4byte	.LLST17
	.byte	0
	.uleb128 0x1e
	.byte	0x1
	.4byte	.LASF57
	.byte	0x1
	.byte	0x50
	.4byte	.LFB16
	.4byte	.LFE16
	.4byte	.LLST18
	.4byte	0x536
	.uleb128 0x22
	.string	"p"
	.byte	0x1
	.byte	0x50
	.4byte	0x2f4
	.4byte	.LLST19
	.byte	0
	.uleb128 0x21
	.byte	0x1
	.4byte	.LASF58
	.byte	0x1
	.byte	0x55
	.4byte	0x2f4
	.4byte	.LFB17
	.4byte	.LFE17
	.4byte	.LLST20
	.4byte	0x56d
	.uleb128 0x22
	.string	"p"
	.byte	0x1
	.byte	0x55
	.4byte	0x2f4
	.4byte	.LLST21
	.uleb128 0x22
	.string	"s"
	.byte	0x1
	.byte	0x55
	.4byte	0x53
	.4byte	.LLST22
	.byte	0
	.uleb128 0x21
	.byte	0x1
	.4byte	.LASF59
	.byte	0x1
	.byte	0x5a
	.4byte	0x2f4
	.4byte	.LFB18
	.4byte	.LFE18
	.4byte	.LLST23
	.4byte	0x5ac
	.uleb128 0x1f
	.4byte	.LASF60
	.byte	0x1
	.byte	0x5a
	.4byte	0x325
	.4byte	.LLST24
	.uleb128 0x26
	.4byte	.Ldebug_ranges0+0
	.uleb128 0x27
	.string	"p"
	.byte	0x1
	.byte	0x5c
	.4byte	0x2f4
	.4byte	.LLST25
	.byte	0
	.byte	0
	.uleb128 0x21
	.byte	0x1
	.4byte	.LASF61
	.byte	0x1
	.byte	0x6c
	.4byte	0x2f4
	.4byte	.LFB19
	.4byte	.LFE19
	.4byte	.LLST26
	.4byte	0x5f8
	.uleb128 0x22
	.string	"n"
	.byte	0x1
	.byte	0x6c
	.4byte	0x325
	.4byte	.LLST27
	.uleb128 0x1f
	.4byte	.LASF60
	.byte	0x1
	.byte	0x6c
	.4byte	0x325
	.4byte	.LLST28
	.uleb128 0x26
	.4byte	.Ldebug_ranges0+0x18
	.uleb128 0x27
	.string	"p"
	.byte	0x1
	.byte	0x6e
	.4byte	0x2f4
	.4byte	.LLST29
	.byte	0
	.byte	0
	.uleb128 0x21
	.byte	0x1
	.4byte	.LASF62
	.byte	0x1
	.byte	0x84
	.4byte	0x2f4
	.4byte	.LFB20
	.4byte	.LFE20
	.4byte	.LLST30
	.4byte	0x644
	.uleb128 0x22
	.string	"a"
	.byte	0x1
	.byte	0x84
	.4byte	0x325
	.4byte	.LLST31
	.uleb128 0x1f
	.4byte	.LASF60
	.byte	0x1
	.byte	0x84
	.4byte	0x325
	.4byte	.LLST32
	.uleb128 0x26
	.4byte	.Ldebug_ranges0+0x38
	.uleb128 0x27
	.string	"p"
	.byte	0x1
	.byte	0x86
	.4byte	0x2f4
	.4byte	.LLST33
	.byte	0
	.byte	0
	.uleb128 0x1e
	.byte	0x1
	.4byte	.LASF63
	.byte	0x1
	.byte	0x9a
	.4byte	.LFB21
	.4byte	.LFE21
	.4byte	.LLST34
	.4byte	0x66a
	.uleb128 0x22
	.string	"p"
	.byte	0x1
	.byte	0x9a
	.4byte	0x2f4
	.4byte	.LLST35
	.byte	0
	.uleb128 0x21
	.byte	0x1
	.4byte	.LASF64
	.byte	0x1
	.byte	0xa9
	.4byte	0x2f4
	.4byte	.LFB22
	.4byte	.LFE22
	.4byte	.LLST36
	.4byte	0x6ec
	.uleb128 0x22
	.string	"p"
	.byte	0x1
	.byte	0xa9
	.4byte	0x2f4
	.4byte	.LLST37
	.uleb128 0x1f
	.4byte	.LASF60
	.byte	0x1
	.byte	0xa9
	.4byte	0x325
	.4byte	.LLST38
	.uleb128 0x26
	.4byte	.Ldebug_ranges0+0x50
	.uleb128 0x27
	.string	"n"
	.byte	0x1
	.byte	0xab
	.4byte	0x2f4
	.4byte	.LLST39
	.uleb128 0x28
	.4byte	0x355
	.4byte	.LBB22
	.4byte	.LBE22
	.byte	0x1
	.byte	0xb9
	.4byte	0x6d1
	.uleb128 0x24
	.4byte	0x366
	.4byte	.LLST40
	.byte	0
	.uleb128 0x29
	.4byte	0x355
	.4byte	.LBB24
	.4byte	.LBE24
	.byte	0x1
	.byte	0xb9
	.uleb128 0x24
	.4byte	0x366
	.4byte	.LLST41
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0x21
	.byte	0x1
	.4byte	.LASF65
	.byte	0x1
	.byte	0xcf
	.4byte	0x325
	.4byte	.LFB23
	.4byte	.LFE23
	.4byte	.LLST42
	.4byte	0x716
	.uleb128 0x22
	.string	"p"
	.byte	0x1
	.byte	0xcf
	.4byte	0x2f4
	.4byte	.LLST43
	.byte	0
	.uleb128 0x2a
	.4byte	.LASF75
	.byte	0x1
	.4byte	.LFB25
	.4byte	.LFE25
	.byte	0x2
	.byte	0x71
	.sleb128 0
	.4byte	0x765
	.uleb128 0x29
	.4byte	0x370
	.4byte	.LBB28
	.4byte	.LBE28
	.byte	0x1
	.byte	0xd6
	.uleb128 0x2b
	.4byte	0x386
	.2byte	0xffff
	.uleb128 0x2c
	.4byte	0x37b
	.byte	0x1
	.uleb128 0x29
	.4byte	0x337
	.4byte	.LBB29
	.4byte	.LBE29
	.byte	0x1
	.byte	0xf
	.uleb128 0x2d
	.4byte	0x345
	.byte	0x6
	.byte	0x3
	.4byte	_ZL8g_mem2gp
	.byte	0x9f
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0x2e
	.4byte	.LASF76
	.byte	0x1
	.byte	0xd
	.4byte	0x68
	.byte	0x1
	.byte	0x5
	.byte	0x3
	.4byte	MALLOC_MEM2
	.uleb128 0x2f
	.4byte	.LASF66
	.byte	0x1
	.byte	0xf
	.4byte	0x93
	.byte	0x5
	.byte	0x3
	.4byte	_ZL8g_mem2gp
	.uleb128 0x2f
	.4byte	.LASF67
	.byte	0x1
	.byte	0x11
	.4byte	0x2e7
	.byte	0x5
	.byte	0x3
	.4byte	_ZL15g_bigGoesToMem2
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
	.uleb128 0x52
	.uleb128 0x1
	.uleb128 0x55
	.uleb128 0x6
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
	.uleb128 0x7
	.uleb128 0x13
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x32
	.uleb128 0xb
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
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xa
	.byte	0
	.byte	0
	.uleb128 0x9
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xa
	.byte	0
	.byte	0
	.uleb128 0xa
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xa
	.uleb128 0x32
	.uleb128 0xb
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
	.uleb128 0x64
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xc
	.uleb128 0x5
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x34
	.uleb128 0xc
	.byte	0
	.byte	0
	.uleb128 0xd
	.uleb128 0x5
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xe
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
	.uleb128 0x3c
	.uleb128 0xc
	.uleb128 0x64
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xf
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
	.uleb128 0x10
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
	.uleb128 0x3c
	.uleb128 0xc
	.uleb128 0x64
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x11
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
	.uleb128 0x32
	.uleb128 0xb
	.uleb128 0x3c
	.uleb128 0xc
	.uleb128 0x64
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x12
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x13
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x14
	.uleb128 0x26
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x15
	.uleb128 0x10
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x16
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x47
	.uleb128 0x13
	.uleb128 0x20
	.uleb128 0xb
	.uleb128 0x64
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x17
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x34
	.uleb128 0xc
	.byte	0
	.byte	0
	.uleb128 0x18
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
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x20
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x19
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
	.byte	0
	.byte	0
	.uleb128 0x1a
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x34
	.uleb128 0xc
	.uleb128 0x20
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x1b
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
	.byte	0
	.byte	0
	.uleb128 0x1c
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
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x40
	.uleb128 0xa
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x1d
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
	.uleb128 0xa
	.byte	0
	.byte	0
	.uleb128 0x1e
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
	.uleb128 0x1f
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
	.uleb128 0x20
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
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x40
	.uleb128 0x6
	.byte	0
	.byte	0
	.uleb128 0x21
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
	.uleb128 0x49
	.uleb128 0x13
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
	.uleb128 0x22
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
	.uleb128 0x23
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x31
	.uleb128 0x13
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
	.uleb128 0x24
	.uleb128 0x5
	.byte	0
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x6
	.byte	0
	.byte	0
	.uleb128 0x25
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
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x40
	.uleb128 0x6
	.byte	0
	.byte	0
	.uleb128 0x26
	.uleb128 0xb
	.byte	0x1
	.uleb128 0x55
	.uleb128 0x6
	.byte	0
	.byte	0
	.uleb128 0x27
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
	.uleb128 0x28
	.uleb128 0x1d
	.byte	0x1
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x58
	.uleb128 0xb
	.uleb128 0x59
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x29
	.uleb128 0x1d
	.byte	0x1
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x58
	.uleb128 0xb
	.uleb128 0x59
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x2a
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x34
	.uleb128 0xc
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x40
	.uleb128 0xa
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x2b
	.uleb128 0x5
	.byte	0
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x1c
	.uleb128 0x5
	.byte	0
	.byte	0
	.uleb128 0x2c
	.uleb128 0x5
	.byte	0
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x1c
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x2d
	.uleb128 0x5
	.byte	0
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0xa
	.byte	0
	.byte	0
	.uleb128 0x2e
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x2
	.uleb128 0xa
	.byte	0
	.byte	0
	.uleb128 0x2f
	.uleb128 0x34
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
	.byte	0
	.section	.debug_loc,"",@progbits
.Ldebug_loc0:
.LLST0:
	.4byte	.LFB7
	.4byte	.LCFI0
	.2byte	0x2
	.byte	0x71
	.sleb128 0
	.4byte	.LCFI0
	.4byte	.LCFI1
	.2byte	0x2
	.byte	0x71
	.sleb128 8
	.4byte	.LCFI1
	.4byte	.LFE7
	.2byte	0x2
	.byte	0x71
	.sleb128 0
	.4byte	0
	.4byte	0
.LLST1:
	.4byte	.LVL1
	.4byte	.LVL2
	.2byte	0x1
	.byte	0x53
	.4byte	.LVL2
	.4byte	.LVL3-1
	.2byte	0x1
	.byte	0x54
	.4byte	0
	.4byte	0
.LLST2:
	.4byte	.LFB8
	.4byte	.LCFI2
	.2byte	0x2
	.byte	0x71
	.sleb128 0
	.4byte	.LCFI2
	.4byte	.LCFI3
	.2byte	0x2
	.byte	0x71
	.sleb128 8
	.4byte	.LCFI3
	.4byte	.LFE8
	.2byte	0x2
	.byte	0x71
	.sleb128 0
	.4byte	0
	.4byte	0
.LLST3:
	.4byte	.LFB9
	.4byte	.LCFI4
	.2byte	0x2
	.byte	0x71
	.sleb128 0
	.4byte	.LCFI4
	.4byte	.LCFI5
	.2byte	0x2
	.byte	0x71
	.sleb128 8
	.4byte	.LCFI5
	.4byte	.LFE9
	.2byte	0x2
	.byte	0x71
	.sleb128 0
	.4byte	0
	.4byte	0
.LLST4:
	.4byte	.LVL4
	.4byte	.LVL5
	.2byte	0x1
	.byte	0x53
	.4byte	.LVL5
	.4byte	.LVL6-1
	.2byte	0x1
	.byte	0x54
	.4byte	0
	.4byte	0
.LLST5:
	.4byte	.LFB10
	.4byte	.LCFI6
	.2byte	0x2
	.byte	0x71
	.sleb128 0
	.4byte	.LCFI6
	.4byte	.LCFI7
	.2byte	0x2
	.byte	0x71
	.sleb128 8
	.4byte	.LCFI7
	.4byte	.LFE10
	.2byte	0x2
	.byte	0x71
	.sleb128 0
	.4byte	0
	.4byte	0
.LLST6:
	.4byte	.LVL7
	.4byte	.LVL8
	.2byte	0x1
	.byte	0x53
	.4byte	.LVL8
	.4byte	.LVL9-1
	.2byte	0x1
	.byte	0x54
	.4byte	0
	.4byte	0
.LLST7:
	.4byte	.LFB11
	.4byte	.LCFI8
	.2byte	0x2
	.byte	0x71
	.sleb128 0
	.4byte	.LCFI8
	.4byte	.LCFI9
	.2byte	0x2
	.byte	0x71
	.sleb128 8
	.4byte	.LCFI9
	.4byte	.LFE11
	.2byte	0x2
	.byte	0x71
	.sleb128 0
	.4byte	0
	.4byte	0
.LLST8:
	.4byte	.LVL10
	.4byte	.LVL11
	.2byte	0x1
	.byte	0x53
	.4byte	.LVL11
	.4byte	.LVL13-1
	.2byte	0x1
	.byte	0x50
	.4byte	0
	.4byte	0
.LLST9:
	.4byte	.LVL10
	.4byte	.LVL12
	.2byte	0x1
	.byte	0x54
	.4byte	.LVL12
	.4byte	.LVL13-1
	.2byte	0x1
	.byte	0x55
	.4byte	0
	.4byte	0
.LLST10:
	.4byte	.LFB12
	.4byte	.LCFI10
	.2byte	0x2
	.byte	0x71
	.sleb128 0
	.4byte	.LCFI10
	.4byte	.LCFI11
	.2byte	0x2
	.byte	0x71
	.sleb128 8
	.4byte	.LCFI11
	.4byte	.LFE12
	.2byte	0x2
	.byte	0x71
	.sleb128 0
	.4byte	0
	.4byte	0
.LLST11:
	.4byte	.LVL14
	.4byte	.LVL15-1
	.2byte	0x1
	.byte	0x53
	.4byte	0
	.4byte	0
.LLST12:
	.4byte	.LFB13
	.4byte	.LCFI12
	.2byte	0x2
	.byte	0x71
	.sleb128 0
	.4byte	.LCFI12
	.4byte	.LCFI13
	.2byte	0x2
	.byte	0x71
	.sleb128 8
	.4byte	.LCFI13
	.4byte	.LFE13
	.2byte	0x2
	.byte	0x71
	.sleb128 0
	.4byte	0
	.4byte	0
.LLST13:
	.4byte	.LFB14
	.4byte	.LCFI14
	.2byte	0x2
	.byte	0x71
	.sleb128 0
	.4byte	.LCFI14
	.4byte	.LCFI15
	.2byte	0x2
	.byte	0x71
	.sleb128 8
	.4byte	.LCFI15
	.4byte	.LFE14
	.2byte	0x2
	.byte	0x71
	.sleb128 0
	.4byte	0
	.4byte	0
.LLST14:
	.4byte	.LVL16
	.4byte	.LVL17-1
	.2byte	0x1
	.byte	0x53
	.4byte	0
	.4byte	0
.LLST15:
	.4byte	.LFB15
	.4byte	.LCFI16
	.2byte	0x2
	.byte	0x71
	.sleb128 0
	.4byte	.LCFI16
	.4byte	.LCFI17
	.2byte	0x2
	.byte	0x71
	.sleb128 8
	.4byte	.LCFI17
	.4byte	.LFE15
	.2byte	0x2
	.byte	0x71
	.sleb128 0
	.4byte	0
	.4byte	0
.LLST16:
	.4byte	.LVL18
	.4byte	.LVL19-1
	.2byte	0x1
	.byte	0x53
	.4byte	0
	.4byte	0
.LLST17:
	.4byte	.LVL18
	.4byte	.LVL19-1
	.2byte	0x1
	.byte	0x54
	.4byte	0
	.4byte	0
.LLST18:
	.4byte	.LFB16
	.4byte	.LCFI18
	.2byte	0x2
	.byte	0x71
	.sleb128 0
	.4byte	.LCFI18
	.4byte	.LCFI19
	.2byte	0x2
	.byte	0x71
	.sleb128 8
	.4byte	.LCFI19
	.4byte	.LFE16
	.2byte	0x2
	.byte	0x71
	.sleb128 0
	.4byte	0
	.4byte	0
.LLST19:
	.4byte	.LVL20
	.4byte	.LVL21-1
	.2byte	0x1
	.byte	0x53
	.4byte	0
	.4byte	0
.LLST20:
	.4byte	.LFB17
	.4byte	.LCFI20
	.2byte	0x2
	.byte	0x71
	.sleb128 0
	.4byte	.LCFI20
	.4byte	.LCFI21
	.2byte	0x2
	.byte	0x71
	.sleb128 8
	.4byte	.LCFI21
	.4byte	.LFE17
	.2byte	0x2
	.byte	0x71
	.sleb128 0
	.4byte	0
	.4byte	0
.LLST21:
	.4byte	.LVL22
	.4byte	.LVL23-1
	.2byte	0x1
	.byte	0x53
	.4byte	0
	.4byte	0
.LLST22:
	.4byte	.LVL22
	.4byte	.LVL23-1
	.2byte	0x1
	.byte	0x54
	.4byte	0
	.4byte	0
.LLST23:
	.4byte	.LFB18
	.4byte	.LCFI22
	.2byte	0x2
	.byte	0x71
	.sleb128 0
	.4byte	.LCFI22
	.4byte	.LCFI23
	.2byte	0x2
	.byte	0x71
	.sleb128 16
	.4byte	.LCFI23
	.4byte	.LCFI24
	.2byte	0x2
	.byte	0x71
	.sleb128 0
	.4byte	.LCFI24
	.4byte	.LFE18
	.2byte	0x2
	.byte	0x71
	.sleb128 16
	.4byte	0
	.4byte	0
.LLST24:
	.4byte	.LVL24
	.4byte	.LVL25-1
	.2byte	0x1
	.byte	0x53
	.4byte	.LVL25-1
	.4byte	.LVL28
	.2byte	0x1
	.byte	0x6f
	.4byte	.LVL29
	.4byte	.LFE18
	.2byte	0x1
	.byte	0x6f
	.4byte	0
	.4byte	0
.LLST25:
	.4byte	.LVL26
	.4byte	.LVL27
	.2byte	0x1
	.byte	0x53
	.4byte	.LVL30
	.4byte	.LVL31
	.2byte	0x1
	.byte	0x53
	.4byte	.LVL32
	.4byte	.LVL33
	.2byte	0x1
	.byte	0x53
	.4byte	0
	.4byte	0
.LLST26:
	.4byte	.LFB19
	.4byte	.LCFI25
	.2byte	0x2
	.byte	0x71
	.sleb128 0
	.4byte	.LCFI25
	.4byte	.LCFI26
	.2byte	0x2
	.byte	0x71
	.sleb128 32
	.4byte	.LCFI26
	.4byte	.LCFI27
	.2byte	0x2
	.byte	0x71
	.sleb128 0
	.4byte	.LCFI27
	.4byte	.LCFI28
	.2byte	0x2
	.byte	0x71
	.sleb128 32
	.4byte	.LCFI28
	.4byte	.LCFI29
	.2byte	0x2
	.byte	0x71
	.sleb128 0
	.4byte	.LCFI29
	.4byte	.LFE19
	.2byte	0x2
	.byte	0x71
	.sleb128 32
	.4byte	0
	.4byte	0
.LLST27:
	.4byte	.LVL34
	.4byte	.LVL35-1
	.2byte	0x1
	.byte	0x53
	.4byte	.LVL35-1
	.4byte	.LVL38
	.2byte	0x1
	.byte	0x6c
	.4byte	.LVL41
	.4byte	.LVL44
	.2byte	0x1
	.byte	0x6c
	.4byte	.LVL45
	.4byte	.LFE19
	.2byte	0x1
	.byte	0x6c
	.4byte	0
	.4byte	0
.LLST28:
	.4byte	.LVL34
	.4byte	.LVL35-1
	.2byte	0x1
	.byte	0x54
	.4byte	.LVL35-1
	.4byte	.LVL39
	.2byte	0x1
	.byte	0x6e
	.4byte	.LVL41
	.4byte	.LVL43
	.2byte	0x1
	.byte	0x6e
	.4byte	.LVL45
	.4byte	.LVL50
	.2byte	0x1
	.byte	0x6e
	.4byte	0
	.4byte	0
.LLST29:
	.4byte	.LVL36
	.4byte	.LVL37-1
	.2byte	0x1
	.byte	0x53
	.4byte	.LVL37-1
	.4byte	.LVL40
	.2byte	0x1
	.byte	0x6f
	.4byte	.LVL40
	.4byte	.LVL41
	.2byte	0x1
	.byte	0x53
	.4byte	.LVL42
	.4byte	.LVL43
	.2byte	0x1
	.byte	0x53
	.4byte	.LVL46
	.4byte	.LVL47
	.2byte	0x1
	.byte	0x53
	.4byte	.LVL47
	.4byte	.LVL48
	.2byte	0x1
	.byte	0x6f
	.4byte	.LVL49
	.4byte	.LVL51
	.2byte	0x1
	.byte	0x53
	.4byte	.LVL51
	.4byte	.LVL52
	.2byte	0x1
	.byte	0x6f
	.4byte	.LVL52
	.4byte	.LVL53-1
	.2byte	0x1
	.byte	0x53
	.4byte	.LVL53-1
	.4byte	.LFE19
	.2byte	0x1
	.byte	0x6f
	.4byte	0
	.4byte	0
.LLST30:
	.4byte	.LFB20
	.4byte	.LCFI30
	.2byte	0x2
	.byte	0x71
	.sleb128 0
	.4byte	.LCFI30
	.4byte	.LCFI31
	.2byte	0x2
	.byte	0x71
	.sleb128 24
	.4byte	.LCFI31
	.4byte	.LCFI32
	.2byte	0x2
	.byte	0x71
	.sleb128 0
	.4byte	.LCFI32
	.4byte	.LCFI33
	.2byte	0x2
	.byte	0x71
	.sleb128 24
	.4byte	.LCFI33
	.4byte	.LFE20
	.2byte	0x2
	.byte	0x71
	.sleb128 0
	.4byte	0
	.4byte	0
.LLST31:
	.4byte	.LVL54
	.4byte	.LVL55-1
	.2byte	0x1
	.byte	0x53
	.4byte	.LVL55-1
	.4byte	.LVL56
	.2byte	0x1
	.byte	0x6e
	.4byte	.LVL58
	.4byte	.LVL60
	.2byte	0x1
	.byte	0x6e
	.4byte	.LVL62
	.4byte	.LVL64
	.2byte	0x1
	.byte	0x6e
	.4byte	0
	.4byte	0
.LLST32:
	.4byte	.LVL54
	.4byte	.LVL55-1
	.2byte	0x1
	.byte	0x54
	.4byte	.LVL55-1
	.4byte	.LVL57
	.2byte	0x1
	.byte	0x6f
	.4byte	.LVL58
	.4byte	.LVL65
	.2byte	0x1
	.byte	0x6f
	.4byte	0
	.4byte	0
.LLST33:
	.4byte	.LVL59
	.4byte	.LVL61
	.2byte	0x1
	.byte	0x53
	.4byte	.LVL63
	.4byte	.LFE20
	.2byte	0x1
	.byte	0x53
	.4byte	0
	.4byte	0
.LLST34:
	.4byte	.LFB21
	.4byte	.LCFI34
	.2byte	0x2
	.byte	0x71
	.sleb128 0
	.4byte	.LCFI34
	.4byte	.LCFI35
	.2byte	0x2
	.byte	0x71
	.sleb128 8
	.4byte	.LCFI35
	.4byte	.LCFI36
	.2byte	0x2
	.byte	0x71
	.sleb128 0
	.4byte	.LCFI36
	.4byte	.LCFI37
	.2byte	0x2
	.byte	0x71
	.sleb128 8
	.4byte	.LCFI37
	.4byte	.LFE21
	.2byte	0x2
	.byte	0x71
	.sleb128 0
	.4byte	0
	.4byte	0
.LLST35:
	.4byte	.LVL66
	.4byte	.LVL67-1
	.2byte	0x1
	.byte	0x53
	.4byte	.LVL68
	.4byte	.LVL69
	.2byte	0x1
	.byte	0x53
	.4byte	.LVL69
	.4byte	.LVL70-1
	.2byte	0x1
	.byte	0x54
	.4byte	0
	.4byte	0
.LLST36:
	.4byte	.LFB22
	.4byte	.LCFI38
	.2byte	0x2
	.byte	0x71
	.sleb128 0
	.4byte	.LCFI38
	.4byte	.LCFI39
	.2byte	0x2
	.byte	0x71
	.sleb128 32
	.4byte	.LCFI39
	.4byte	.LCFI40
	.2byte	0x2
	.byte	0x71
	.sleb128 0
	.4byte	.LCFI40
	.4byte	.LFE22
	.2byte	0x2
	.byte	0x71
	.sleb128 32
	.4byte	0
	.4byte	0
.LLST37:
	.4byte	.LVL71
	.4byte	.LVL72
	.2byte	0x1
	.byte	0x53
	.4byte	.LVL72
	.4byte	.LVL77
	.2byte	0x1
	.byte	0x6f
	.4byte	.LVL78
	.4byte	.LVL80
	.2byte	0x1
	.byte	0x53
	.4byte	.LVL80
	.4byte	.LFE22
	.2byte	0x1
	.byte	0x6f
	.4byte	0
	.4byte	0
.LLST38:
	.4byte	.LVL71
	.4byte	.LVL73
	.2byte	0x1
	.byte	0x54
	.4byte	.LVL73
	.4byte	.LVL75
	.2byte	0x1
	.byte	0x6e
	.4byte	.LVL78
	.4byte	.LVL79
	.2byte	0x1
	.byte	0x54
	.4byte	.LVL79
	.4byte	.LVL87
	.2byte	0x1
	.byte	0x6e
	.4byte	.LVL88
	.4byte	.LVL92
	.2byte	0x1
	.byte	0x6e
	.4byte	.LVL93
	.4byte	.LVL94
	.2byte	0x1
	.byte	0x6e
	.4byte	0
	.4byte	0
.LLST39:
	.4byte	.LVL74
	.4byte	.LVL75
	.2byte	0x1
	.byte	0x53
	.4byte	.LVL75
	.4byte	.LVL76
	.2byte	0x1
	.byte	0x6d
	.4byte	.LVL76
	.4byte	.LVL78
	.2byte	0x1
	.byte	0x53
	.4byte	.LVL81
	.4byte	.LVL82
	.2byte	0x1
	.byte	0x53
	.4byte	.LVL82
	.4byte	.LVL83
	.2byte	0x1
	.byte	0x6d
	.4byte	.LVL83
	.4byte	.LVL85
	.2byte	0x1
	.byte	0x53
	.4byte	.LVL85
	.4byte	.LVL88
	.2byte	0x1
	.byte	0x6d
	.4byte	.LVL88
	.4byte	.LVL89
	.2byte	0x1
	.byte	0x53
	.4byte	.LVL89
	.4byte	.LVL90
	.2byte	0x1
	.byte	0x6d
	.4byte	.LVL90
	.4byte	.LVL91
	.2byte	0x1
	.byte	0x53
	.4byte	.LVL91
	.4byte	.LFE22
	.2byte	0x1
	.byte	0x6d
	.4byte	0
	.4byte	0
.LLST40:
	.4byte	.LVL84
	.4byte	.LVL88
	.2byte	0x1
	.byte	0x6f
	.4byte	0
	.4byte	0
.LLST41:
	.4byte	.LVL86
	.4byte	.LVL87
	.2byte	0x1
	.byte	0x6f
	.4byte	0
	.4byte	0
.LLST42:
	.4byte	.LFB23
	.4byte	.LCFI41
	.2byte	0x2
	.byte	0x71
	.sleb128 0
	.4byte	.LCFI41
	.4byte	.LCFI42
	.2byte	0x2
	.byte	0x71
	.sleb128 8
	.4byte	.LCFI42
	.4byte	.LCFI43
	.2byte	0x2
	.byte	0x71
	.sleb128 0
	.4byte	.LCFI43
	.4byte	.LCFI44
	.2byte	0x2
	.byte	0x71
	.sleb128 8
	.4byte	.LCFI44
	.4byte	.LFE23
	.2byte	0x2
	.byte	0x71
	.sleb128 0
	.4byte	0
	.4byte	0
.LLST43:
	.4byte	.LVL95
	.4byte	.LVL96-1
	.2byte	0x1
	.byte	0x53
	.4byte	.LVL97
	.4byte	.LVL98-1
	.2byte	0x1
	.byte	0x53
	.4byte	0
	.4byte	0
	.section	.debug_aranges,"",@progbits
	.4byte	0x24
	.2byte	0x2
	.4byte	.Ldebug_info0
	.byte	0x4
	.byte	0
	.2byte	0
	.2byte	0
	.4byte	.Ltext0
	.4byte	.Letext0-.Ltext0
	.4byte	.LFB25
	.4byte	.LFE25-.LFB25
	.4byte	0
	.4byte	0
	.section	.debug_ranges,"",@progbits
.Ldebug_ranges0:
	.4byte	.LBB14
	.4byte	.LBE14
	.4byte	.LBB15
	.4byte	.LBE15
	.4byte	0
	.4byte	0
	.4byte	.LBB16
	.4byte	.LBE16
	.4byte	.LBB17
	.4byte	.LBE17
	.4byte	.LBB18
	.4byte	.LBE18
	.4byte	0
	.4byte	0
	.4byte	.LBB19
	.4byte	.LBE19
	.4byte	.LBB20
	.4byte	.LBE20
	.4byte	0
	.4byte	0
	.4byte	.LBB21
	.4byte	.LBE21
	.4byte	.LBB26
	.4byte	.LBE26
	.4byte	.LBB27
	.4byte	.LBE27
	.4byte	0
	.4byte	0
	.4byte	.Ltext0
	.4byte	.Letext0
	.4byte	.LFB25
	.4byte	.LFE25
	.4byte	0
	.4byte	0
	.section	.debug_line,"",@progbits
.Ldebug_line0:
	.section	.debug_str,"MS",@progbits,1
.LASF34:
	.string	"_ZN10CMEM2Alloc15forceEndAddressEPv"
.LASF36:
	.string	"_ZNK10CMEM2Alloc13getEndAddressEv"
.LASF67:
	.string	"g_bigGoesToMem2"
.LASF51:
	.string	"mem2Size"
.LASF17:
	.string	"m_mutex"
.LASF64:
	.string	"__wrap_realloc"
.LASF2:
	.string	"short int"
.LASF44:
	.string	"size_t"
.LASF76:
	.string	"MALLOC_MEM2"
.LASF56:
	.string	"MEM1_memalign"
.LASF68:
	.string	"GNU C++ 4.6.3"
.LASF52:
	.string	"MEM2_alloc"
.LASF73:
	.string	"MEM2_cleanup"
.LASF26:
	.string	"_ZN10CMEM2Alloc4initEPvS0_"
.LASF66:
	.string	"g_mem2gp"
.LASF49:
	.string	"MEM2_takeBigOnes"
.LASF59:
	.string	"__wrap_malloc"
.LASF19:
	.string	"reallocate"
.LASF21:
	.string	"_ZN10CMEM2Alloc10reallocateEPvj"
.LASF42:
	.string	"bool"
.LASF8:
	.string	"float"
.LASF5:
	.string	"long long int"
.LASF63:
	.string	"__wrap_free"
.LASF33:
	.string	"forceEndAddress"
.LASF41:
	.string	"CMEM2Alloc"
.LASF31:
	.string	"usableSize"
.LASF74:
	.string	"MEM2_freesize"
.LASF48:
	.string	"__priority"
.LASF29:
	.string	"clear"
.LASF61:
	.string	"__wrap_calloc"
.LASF47:
	.string	"__initialize_p"
.LASF53:
	.string	"MEM2_free"
.LASF16:
	.string	"m_first"
.LASF24:
	.string	"_ZN10CMEM2Alloc7releaseEPv"
.LASF1:
	.string	"unsigned char"
.LASF14:
	.string	"m_baseAddress"
.LASF27:
	.string	"cleanup"
.LASF0:
	.string	"signed char"
.LASF6:
	.string	"long long unsigned int"
.LASF7:
	.string	"uint32_t"
.LASF75:
	.string	"_GLOBAL__sub_I_MALLOC_MEM2"
.LASF25:
	.string	"_ZN10CMEM2Alloc4initEj"
.LASF4:
	.string	"unsigned int"
.LASF11:
	.string	"mutex_t"
.LASF35:
	.string	"getEndAddress"
.LASF55:
	.string	"MEM1_alloc"
.LASF20:
	.string	"_ZN10CMEM2Alloc8allocateEj"
.LASF18:
	.string	"allocate"
.LASF46:
	.string	"MEM2_usableSize"
.LASF3:
	.string	"short unsigned int"
.LASF32:
	.string	"_ZN10CMEM2Alloc10usableSizeEPv"
.LASF62:
	.string	"__wrap_memalign"
.LASF10:
	.string	"char"
.LASF39:
	.string	"FreeSize"
.LASF45:
	.string	"long int"
.LASF40:
	.string	"_ZN10CMEM2Alloc8FreeSizeEv"
.LASF69:
	.string	"d:/Projects/SaveGameManager/savegame-manager-gx/trunk/source/Memory/mem2.cpp"
.LASF72:
	.string	"__static_initialization_and_destruction_0"
.LASF28:
	.string	"_ZN10CMEM2Alloc7cleanupEv"
.LASF71:
	.string	"this"
.LASF43:
	.string	"long unsigned int"
.LASF9:
	.string	"double"
.LASF22:
	.string	"release"
.LASF58:
	.string	"MEM1_realloc"
.LASF60:
	.string	"size"
.LASF50:
	.string	"MEM2_init"
.LASF37:
	.string	"info"
.LASF54:
	.string	"MEM2_realloc"
.LASF30:
	.string	"_ZN10CMEM2Alloc5clearEv"
.LASF65:
	.string	"__wrap_malloc_usable_size"
.LASF15:
	.string	"m_endAddress"
.LASF70:
	.string	"SBlock"
.LASF38:
	.string	"_ZNK10CMEM2Alloc4infoERPvRj"
.LASF13:
	.string	"prev"
.LASF57:
	.string	"MEM1_free"
.LASF12:
	.string	"next"
.LASF23:
	.string	"init"
	.ident	"GCC: (devkitPPC release 26) 4.6.3"
