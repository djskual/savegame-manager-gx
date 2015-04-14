	.file	"mem2alloc.cpp"
	.section	".text"
.Ltext0:
	.align 2
	.globl _ZN10CMEM2Alloc4initEj
	.type	_ZN10CMEM2Alloc4initEj, @function
_ZN10CMEM2Alloc4initEj:
.LFB371:
	.file 1 "d:/Projects/SaveGameManager/savegame-manager-gx/trunk/source/Memory/mem2alloc.cpp"
	.loc 1 18 0
	.cfi_startproc
.LVL0:
	mflr 0
	stwu 1,-24(1)
.LCFI0:
	.cfi_def_cfa_offset 24
	.cfi_register 65, 0
	stw 29,12(1)
	mr 29,4
	.cfi_offset 29, -12
	stw 0,28(1)
	.loc 1 20 0
	slwi 29,29,20
	.loc 1 18 0
	stw 30,16(1)
	stw 31,20(1)
	.loc 1 18 0
	mr 31,3
	.cfi_offset 31, -4
	.cfi_offset 30, -8
	.cfi_offset 65, 4
	.loc 1 19 0
	bl SYS_GetArena2Lo
.LVL1:
	addi 3,3,31
	rlwinm 30,3,0,0,26
	stw 30,0(31)
	.loc 1 20 0
	bl SYS_GetArena2Size
.LVL2:
	rlwinm 3,3,0,0,26
	cmplw 7,3,29
	ble- 7,.L2
	mr 3,29
.L2:
	add 3,30,3
	.loc 1 21 0
	lis 0,0x9330
	cmplw 7,3,0
	.loc 1 20 0
	stw 3,4(31)
	.loc 1 21 0
	ble- 7,.L3
	.loc 1 22 0
	stw 0,4(31)
	lis 3,0x9330
.L3:
	.loc 1 23 0
	bl SYS_SetArena2Lo
	.loc 1 24 0
	addi 3,31,12
	li 4,0
	bl LWP_MutexInit
	.loc 1 25 0
	lwz 0,28(1)
	lwz 29,12(1)
	mtlr 0
	lwz 30,16(1)
	lwz 31,20(1)
.LVL3:
	addi 1,1,24
.LCFI1:
	.cfi_def_cfa_offset 0
	.cfi_restore 31
	.cfi_restore 30
	.cfi_restore 29
	blr
	.cfi_endproc
.LFE371:
	.size	_ZN10CMEM2Alloc4initEj, .-_ZN10CMEM2Alloc4initEj
	.align 2
	.globl _ZN10CMEM2Alloc4initEPvS0_
	.type	_ZN10CMEM2Alloc4initEPvS0_, @function
_ZN10CMEM2Alloc4initEPvS0_:
.LFB372:
	.loc 1 28 0
	.cfi_startproc
.LVL4:
	mflr 0
	stwu 1,-8(1)
.LCFI2:
	.cfi_def_cfa_offset 8
	.cfi_register 65, 0
	.loc 1 29 0
	addi 4,4,31
.LVL5:
	.loc 1 30 0
	rlwinm 5,5,0,0,26
.LVL6:
	.loc 1 29 0
	rlwinm 4,4,0,0,26
.LVL7:
	.loc 1 28 0
	stw 0,12(1)
	.loc 1 29 0
	stw 4,0(3)
	.loc 1 31 0
	li 4,0
	.loc 1 30 0
	stw 5,4(3)
	.loc 1 31 0
	addi 3,3,12
.LVL8:
	.cfi_offset 65, 4
	bl LWP_MutexInit
.LVL9:
	.loc 1 32 0
	lwz 0,12(1)
	addi 1,1,8
.LCFI3:
	.cfi_def_cfa_offset 0
	mtlr 0
	blr
	.cfi_endproc
.LFE372:
	.size	_ZN10CMEM2Alloc4initEPvS0_, .-_ZN10CMEM2Alloc4initEPvS0_
	.align 2
	.globl _ZN10CMEM2Alloc7cleanupEv
	.type	_ZN10CMEM2Alloc7cleanupEv, @function
_ZN10CMEM2Alloc7cleanupEv:
.LFB373:
	.loc 1 35 0
	.cfi_startproc
.LVL10:
	mflr 0
	stwu 1,-16(1)
.LCFI4:
	.cfi_def_cfa_offset 16
	.cfi_register 65, 0
	stw 31,12(1)
	mr 31,3
	.cfi_offset 31, -4
	stw 0,20(1)
	.loc 1 36 0
	lwz 3,12(3)
.LVL11:
	.cfi_offset 65, 4
	bl LWP_MutexDestroy
	.loc 1 37 0
	li 0,0
	stw 0,12(31)
	.loc 1 38 0
	stw 0,8(31)
	.loc 1 40 0
	bl SYS_GetArena2Lo
	lwz 0,4(31)
	cmpw 7,0,3
	beq- 7,.L8
	.loc 1 42 0
	li 0,0
	stw 0,0(31)
	.loc 1 43 0
	stw 0,4(31)
	.loc 1 44 0
	lwz 0,20(1)
	lwz 31,12(1)
.LVL12:
	mtlr 0
	addi 1,1,16
	.cfi_remember_state
.LCFI5:
	.cfi_def_cfa_offset 0
	.cfi_restore 31
	blr
.LVL13:
.L8:
.LCFI6:
	.cfi_restore_state
	.loc 1 41 0
	lwz 3,0(31)
	bl SYS_SetArena2Lo
	.loc 1 42 0
	li 0,0
	stw 0,0(31)
	.loc 1 43 0
	stw 0,4(31)
	.loc 1 44 0
	lwz 0,20(1)
	lwz 31,12(1)
.LVL14:
	mtlr 0
	addi 1,1,16
	.cfi_restore 31
.LCFI7:
	.cfi_def_cfa_offset 0
	blr
	.cfi_endproc
.LFE373:
	.size	_ZN10CMEM2Alloc7cleanupEv, .-_ZN10CMEM2Alloc7cleanupEv
	.align 2
	.globl _ZN10CMEM2Alloc5clearEv
	.type	_ZN10CMEM2Alloc5clearEv, @function
_ZN10CMEM2Alloc5clearEv:
.LFB374:
	.loc 1 47 0
	.cfi_startproc
.LVL15:
	.loc 1 48 0
	li 0,0
	.loc 1 50 0
	.loc 1 48 0
	stw 0,8(3)
	.loc 1 50 0
	blr
	.cfi_endproc
.LFE374:
	.size	_ZN10CMEM2Alloc5clearEv, .-_ZN10CMEM2Alloc5clearEv
	.align 2
	.globl _ZN10CMEM2Alloc10usableSizeEPv
	.type	_ZN10CMEM2Alloc10usableSizeEPv, @function
_ZN10CMEM2Alloc10usableSizeEPv:
.LFB375:
	.loc 1 53 0
	.cfi_startproc
.LVL16:
	.loc 1 54 0
	mr. 9,3
	li 3,0
.LVL17:
	beqlr- 0
	.loc 1 54 0 is_stmt 0 discriminator 1
	lwz 3,-32(9)
	slwi 3,3,5
	.loc 1 55 0 is_stmt 1 discriminator 1
	blr
	.cfi_endproc
.LFE375:
	.size	_ZN10CMEM2Alloc10usableSizeEPv, .-_ZN10CMEM2Alloc10usableSizeEPv
	.align 2
	.globl _ZN10CMEM2Alloc8allocateEj
	.type	_ZN10CMEM2Alloc8allocateEj, @function
_ZN10CMEM2Alloc8allocateEj:
.LFB376:
	.loc 1 58 0
	.cfi_startproc
.LVL18:
.LBB13:
	.loc 1 59 0
	cmpwi 0,4,0
.LBE13:
	.loc 1 58 0
	mflr 0
	stwu 1,-24(1)
.LCFI8:
	.cfi_def_cfa_offset 24
	.cfi_register 65, 0
	stw 29,12(1)
.LBB28:
	.loc 1 59 0
	li 29,2
	.cfi_offset 29, -12
.LBE28:
	.loc 1 58 0
	stw 30,16(1)
	mr 30,3
	.cfi_offset 30, -8
	stw 31,20(1)
.LBB29:
	.loc 1 59 0
	li 31,1
	.cfi_offset 31, -4
.LBE29:
	.loc 1 58 0
	stw 0,28(1)
.LBB30:
	.loc 1 59 0
	beq- 0,.L14
	.cfi_offset 65, 4
	addi 29,4,-1
	srwi 29,29,5
	addi 31,29,1
	addi 29,29,2
.L14:
.LVL19:
.LBB14:
.LBB15:
	.loc 1 13 0
	lwz 3,12(30)
.LVL20:
	bl LWP_MutexLock
.LVL21:
.LBE15:
.LBE14:
	.loc 1 66 0
	lwz 9,8(30)
	cmpwi 7,9,0
	bne+ 7,.L29
	b .L33
.LVL22:
.L27:
	.loc 1 80 0
	mr 9,0
.LVL23:
.L29:
	.loc 1 82 0
	lbz 0,12(9)
	cmpwi 7,0,0
	beq- 7,.L17
	.loc 1 82 0 is_stmt 0 discriminator 1
	lwz 0,0(9)
	cmplw 7,31,0
	ble- 7,.L18
.L17:
.LVL24:
	.loc 1 80 0 is_stmt 1
	lwz 0,4(9)
.LVL25:
	cmpwi 7,0,0
	bne+ 7,.L27
	.loc 1 89 0
	lwz 11,0(9)
	.loc 1 90 0
	slwi 29,29,5
	lwz 10,4(30)
	.loc 1 89 0
	addi 0,11,1
	slwi 0,0,5
	add 11,9,0
.LVL26:
	.loc 1 90 0
	add 29,11,29
	cmplw 7,10,29
	.loc 1 91 0
	li 29,0
	.loc 1 90 0
	ble- 7,.L16
	.loc 1 93 0
	stw 9,8(11)
	.loc 1 95 0
	stwx 31,9,0
	.loc 1 92 0
	stw 11,4(9)
	.loc 1 96 0
	stb 29,12(11)
	.loc 1 94 0
	stw 29,4(11)
	.loc 1 97 0
	addi 29,11,32
.LVL27:
.L16:
.LBB16:
.LBB17:
.LBB18:
	.loc 1 14 0
	lwz 3,12(30)
	bl LWP_MutexUnlock
.LBE18:
.LBE17:
.LBE16:
.LBE30:
	.loc 1 115 0
	lwz 0,28(1)
	mr 3,29
	lwz 30,16(1)
.LVL28:
	mtlr 0
	lwz 29,12(1)
	lwz 31,20(1)
.LVL29:
	addi 1,1,24
	.cfi_remember_state
.LCFI9:
	.cfi_def_cfa_offset 0
	.cfi_restore 31
	.cfi_restore 30
	.cfi_restore 29
	blr
.LVL30:
.L18:
.LCFI10:
	.cfi_restore_state
.LBB31:
	.loc 1 102 0
	cmplw 7,29,0
	.loc 1 100 0
	li 11,0
	stb 11,12(9)
	.loc 1 102 0
	bge- 7,.L22
	.loc 1 108 0
	lwz 10,4(9)
	.loc 1 104 0
	slwi 29,29,5
	add 11,9,29
.LVL31:
	.loc 1 106 0
	nor 8,31,31
	.loc 1 108 0
	stw 10,4(11)
	.loc 1 106 0
	add 0,8,0
	.loc 1 110 0
	stw 11,4(9)
	.loc 1 106 0
	stwx 0,9,29
	.loc 1 105 0
	li 0,1
	.loc 1 111 0
	lwz 10,4(11)
	.loc 1 105 0
	stb 0,12(11)
	.loc 1 111 0
	cmpwi 7,10,0
	.loc 1 107 0
	stw 31,0(9)
	.loc 1 109 0
	stw 9,8(11)
	.loc 1 111 0
	beq- 7,.L22
	.loc 1 112 0
	stw 11,8(10)
.LVL32:
.L22:
.LBB25:
.LBB22:
.LBB19:
	.loc 1 14 0
	lwz 3,12(30)
.LBE19:
.LBE22:
.LBE25:
	.loc 1 114 0
	addi 29,9,32
.LVL33:
.LBB26:
.LBB23:
.LBB20:
	.loc 1 14 0
	bl LWP_MutexUnlock
.LBE20:
.LBE23:
.LBE26:
.LBE31:
	.loc 1 115 0
	lwz 0,28(1)
	mr 3,29
	lwz 30,16(1)
.LVL34:
	mtlr 0
	lwz 29,12(1)
	lwz 31,20(1)
.LVL35:
	addi 1,1,24
	.cfi_remember_state
	.cfi_restore 29
	.cfi_restore 30
	.cfi_restore 31
.LCFI11:
	.cfi_def_cfa_offset 0
	blr
.LVL36:
.L33:
.LCFI12:
	.cfi_restore_state
.LBB32:
	.loc 1 68 0
	lwz 9,0(30)
	slwi 0,29,5
	lwz 11,4(30)
	.loc 1 69 0
	li 29,0
	.loc 1 68 0
	add 0,9,0
	cmplw 7,11,0
	ble- 7,.L16
	.loc 1 70 0
	stw 9,8(30)
	.loc 1 73 0
	stw 31,0(9)
	.loc 1 71 0
	stw 29,4(9)
	.loc 1 72 0
	stw 29,8(9)
	.loc 1 74 0
	stb 29,12(9)
	.loc 1 75 0
	addi 29,9,32
.LVL37:
.LBB27:
.LBB24:
.LBB21:
	.loc 1 14 0
	lwz 3,12(30)
	bl LWP_MutexUnlock
.LBE21:
.LBE24:
.LBE27:
.LBE32:
	.loc 1 115 0
	lwz 0,28(1)
	mr 3,29
	lwz 30,16(1)
.LVL38:
	mtlr 0
	lwz 29,12(1)
	lwz 31,20(1)
.LVL39:
	addi 1,1,24
	.cfi_restore 29
	.cfi_restore 30
	.cfi_restore 31
.LCFI13:
	.cfi_def_cfa_offset 0
	blr
	.cfi_endproc
.LFE376:
	.size	_ZN10CMEM2Alloc8allocateEj, .-_ZN10CMEM2Alloc8allocateEj
	.align 2
	.globl _ZN10CMEM2Alloc7releaseEPv
	.type	_ZN10CMEM2Alloc7releaseEPv, @function
_ZN10CMEM2Alloc7releaseEPv:
.LFB377:
	.loc 1 118 0
	.cfi_startproc
.LVL40:
	stwu 1,-16(1)
.LCFI14:
	.cfi_def_cfa_offset 16
	mflr 0
	stw 31,12(1)
.LBB40:
	.loc 1 119 0
	mr. 31,4
	.cfi_offset 31, -4
	.cfi_register 65, 0
.LBE40:
	.loc 1 118 0
	stw 30,8(1)
	mr 30,3
	.cfi_offset 30, -8
	stw 0,20(1)
.LBB46:
	.loc 1 119 0
	beq- 0,.L34
	.cfi_offset 65, 4
.LVL41:
.LBB41:
.LBB42:
	.loc 1 13 0
	lwz 3,12(3)
.LVL42:
	bl LWP_MutexLock
.LVL43:
.LBE42:
.LBE41:
	.loc 1 128 0
	lwz 9,-28(31)
	.loc 1 124 0
	li 0,1
	.loc 1 123 0
	addi 10,31,-32
.LVL44:
	.loc 1 128 0
	cmpwi 7,9,0
	.loc 1 124 0
	stb 0,-20(31)
	.loc 1 128 0
	beq- 7,.L40
.L36:
	.loc 1 132 0
	lwz 11,-24(31)
	cmpwi 6,11,0
	beq- 6,.L37
	.loc 1 132 0 is_stmt 0 discriminator 1
	lbz 0,12(11)
	cmpwi 6,0,0
	bne- 6,.L41
.L37:
	.loc 1 141 0 is_stmt 1
	beq- 7,.L38
.L39:
	.loc 1 141 0 is_stmt 0 discriminator 1
	lbz 0,12(9)
	cmpwi 7,0,0
	beq- 7,.L38
	.loc 1 144 0 is_stmt 1
	lwz 11,4(9)
	.loc 1 143 0
	lwz 0,0(9)
	lwz 8,0(10)
	.loc 1 145 0
	cmpwi 7,11,0
	.loc 1 144 0
	stw 11,4(10)
	.loc 1 143 0
	add 9,8,0
	addi 0,9,1
	stw 0,0(10)
	.loc 1 145 0
	beq- 7,.L38
	.loc 1 146 0
	stw 10,8(11)
.LVL45:
.L38:
.LBB43:
.LBB44:
.LBB45:
	.loc 1 14 0
	lwz 3,12(30)
	bl LWP_MutexUnlock
.LVL46:
.L34:
.LBE45:
.LBE44:
.LBE43:
.LBE46:
	.loc 1 148 0
	lwz 0,20(1)
	lwz 30,8(1)
.LVL47:
	mtlr 0
	lwz 31,12(1)
.LVL48:
	addi 1,1,16
	.cfi_remember_state
.LCFI15:
	.cfi_def_cfa_offset 0
	.cfi_restore 31
	.cfi_restore 30
	blr
.LVL49:
.L41:
.LCFI16:
	.cfi_restore_state
.LBB47:
	.loc 1 135 0
	lwz 10,4(11)
	lwz 8,0(11)
	.loc 1 136 0
	lwz 9,4(10)
	.loc 1 135 0
	lwz 0,0(10)
	.loc 1 137 0
	cmpwi 7,9,0
	.loc 1 136 0
	stw 9,4(11)
	.loc 1 135 0
	add 10,8,0
	addi 0,10,1
	stw 0,0(11)
	.loc 1 137 0
	beq- 7,.L38
	.loc 1 138 0
	stw 11,8(9)
	mr 10,11
	b .L39
.LVL50:
.L40:
	.loc 1 129 0
	lwz 11,4(30)
	subf 11,10,11
	srawi 11,11,5
	addi 0,11,-1
	stw 0,-32(31)
	b .L36
.LBE47:
	.cfi_endproc
.LFE377:
	.size	_ZN10CMEM2Alloc7releaseEPv, .-_ZN10CMEM2Alloc7releaseEPv
	.align 2
	.globl _ZN10CMEM2Alloc10reallocateEPvj
	.type	_ZN10CMEM2Alloc10reallocateEPvj, @function
_ZN10CMEM2Alloc10reallocateEPvj:
.LFB378:
	.loc 1 151 0
	.cfi_startproc
.LVL51:
.LBB59:
	.loc 1 156 0
	cmpwi 0,5,0
.LBE59:
	.loc 1 151 0
	mflr 0
	stwu 1,-24(1)
.LCFI17:
	.cfi_def_cfa_offset 24
	.cfi_register 65, 0
	stw 30,16(1)
	mr 30,3
	.cfi_offset 30, -8
	stw 31,20(1)
	mr 31,4
	.cfi_offset 31, -4
	stw 0,28(1)
	stw 28,8(1)
	stw 29,12(1)
.LBB77:
	.loc 1 156 0
	bne- 0,.L43
	.cfi_offset 29, -12
	.cfi_offset 28, -16
	.cfi_offset 65, 4
	.loc 1 157 0
	li 5,1
.LVL52:
.L43:
	.loc 1 158 0
	cmpwi 7,31,0
	beq- 7,.L57
	.loc 1 162 0
	addi 5,5,-1
.LVL53:
.LBB60:
.LBB61:
.LBB62:
	.loc 1 13 0
	lwz 3,12(30)
.LVL54:
.LBE62:
.LBE61:
.LBE60:
	.loc 1 162 0
	srwi 29,5,5
.LBB71:
.LBB64:
.LBB63:
	.loc 1 13 0
	bl LWP_MutexLock
.LVL55:
.LBE63:
.LBE64:
	.loc 1 167 0
	addi 10,29,2
.LBE71:
	.loc 1 161 0
	addi 11,31,-32
.LVL56:
.LBB72:
	.loc 1 167 0
	slwi 8,10,5
	lwz 0,4(30)
	add 9,11,8
	cmplw 7,9,0
	bge- 7,.L54
	.loc 1 173 0
	lwz 7,-28(31)
.LBE72:
	.loc 1 162 0
	addi 28,29,1
.LBB73:
	.loc 1 173 0
	cmpwi 7,7,0
	beq- 7,.L58
	.loc 1 179 0 discriminator 1
	lwz 0,-32(31)
	cmplw 7,28,0
	bgt- 7,.L59
.L47:
	.loc 1 191 0
	cmplw 7,10,0
	bge- 7,.L45
.LVL57:
	.loc 1 197 0
	lwz 10,-28(31)
	.loc 1 195 0
	subfic 29,29,-2
.LVL58:
	add 0,29,0
	.loc 1 198 0
	stw 11,8(9)
	.loc 1 197 0
	stw 10,4(9)
	.loc 1 199 0
	stw 9,-28(31)
	.loc 1 195 0
	stwx 0,11,8
	.loc 1 194 0
	li 0,1
	.loc 1 200 0
	lwz 10,4(9)
	.loc 1 194 0
	stb 0,12(9)
	.loc 1 200 0
	cmpwi 7,10,0
	.loc 1 196 0
	stw 28,-32(31)
	.loc 1 200 0
	beq- 7,.L45
	.loc 1 201 0
	stw 9,8(10)
.LVL59:
.L45:
.LBB65:
.LBB66:
.LBB67:
	.loc 1 14 0
	lwz 3,12(30)
	bl LWP_MutexUnlock
.LVL60:
.L51:
.LBE67:
.LBE66:
.LBE65:
.LBE73:
.LBE77:
	.loc 1 213 0
	lwz 0,28(1)
	mr 3,31
	lwz 28,8(1)
	mtlr 0
	lwz 29,12(1)
	lwz 30,16(1)
.LVL61:
	lwz 31,20(1)
	addi 1,1,24
	.cfi_remember_state
.LCFI18:
	.cfi_def_cfa_offset 0
	.cfi_restore 31
	.cfi_restore 30
	.cfi_restore 29
	.cfi_restore 28
	blr
.LVL62:
.L59:
.LCFI19:
	.cfi_restore_state
.LBB78:
.LBB74:
	.loc 1 179 0 discriminator 2
	lbz 6,12(7)
	cmpwi 7,6,0
	beq- 7,.L48
	.loc 1 179 0 is_stmt 0 discriminator 3
	lwz 6,0(7)
	add 6,0,6
	addi 0,6,1
	cmplw 7,28,0
	bgt- 7,.L48
	.loc 1 183 0 is_stmt 1
	lwz 7,4(7)
	.loc 1 182 0
	stw 0,-32(31)
	.loc 1 184 0
	cmpwi 7,7,0
	.loc 1 183 0
	stw 7,-28(31)
	.loc 1 184 0
	beq- 7,.L47
	.loc 1 185 0
	stw 11,8(7)
	b .L47
.L48:
.LVL63:
.LBB68:
.LBB69:
.LBB70:
	.loc 1 14 0
	lwz 3,12(30)
	bl LWP_MutexUnlock
.LVL64:
.LBE70:
.LBE69:
.LBE68:
.LBE74:
	.loc 1 207 0
	mr 3,30
	slwi 4,28,5
	bl _ZN10CMEM2Alloc8allocateEj
.LVL65:
	.loc 1 208 0
	mr. 29,3
	beq- 0,.L60
	.loc 1 210 0
	lwz 5,-32(31)
	mr 4,31
	slwi 5,5,5
	bl memcpy
.LVL66:
	.loc 1 211 0
	mr 3,30
	mr 4,31
	.loc 1 212 0
	mr 31,29
.LVL67:
	.loc 1 211 0
	bl _ZN10CMEM2Alloc7releaseEPv
.LVL68:
.LBE78:
	.loc 1 213 0
	lwz 0,28(1)
	mr 3,31
	lwz 28,8(1)
.LVL69:
	mtlr 0
	lwz 29,12(1)
.LVL70:
	lwz 30,16(1)
.LVL71:
	lwz 31,20(1)
	addi 1,1,24
	.cfi_remember_state
	.cfi_restore 28
	.cfi_restore 29
	.cfi_restore 30
	.cfi_restore 31
.LCFI20:
	.cfi_def_cfa_offset 0
	blr
.LVL72:
.L54:
.LCFI21:
	.cfi_restore_state
.LBB79:
.LBB75:
	.loc 1 169 0
	li 31,0
.LVL73:
	b .L45
.LVL74:
.L57:
.LBE75:
.LBE79:
	.loc 1 213 0
	lwz 0,28(1)
.LBB80:
	.loc 1 159 0
	mr 3,30
.LBE80:
	.loc 1 213 0
	lwz 28,8(1)
.LBB81:
	.loc 1 159 0
	mr 4,5
.LVL75:
.LBE81:
	.loc 1 213 0
	lwz 29,12(1)
	mtlr 0
	lwz 30,16(1)
.LVL76:
	lwz 31,20(1)
.LVL77:
	addi 1,1,24
	.cfi_remember_state
.LCFI22:
	.cfi_def_cfa_offset 0
	.cfi_restore 31
	.cfi_restore 30
	.cfi_restore 29
	.cfi_restore 28
.LBB82:
	.loc 1 159 0
	b _ZN10CMEM2Alloc8allocateEj
.LVL78:
.L58:
.LCFI23:
	.cfi_restore_state
.LBB76:
	.loc 1 175 0
	stw 28,-32(31)
	b .L45
.LVL79:
.L60:
.LBE76:
	.loc 1 209 0
	li 31,0
.LVL80:
	b .L51
.LBE82:
	.cfi_endproc
.LFE378:
	.size	_ZN10CMEM2Alloc10reallocateEPvj, .-_ZN10CMEM2Alloc10reallocateEPvj
	.align 2
	.globl _ZN10CMEM2Alloc8FreeSizeEv
	.type	_ZN10CMEM2Alloc8FreeSizeEv, @function
_ZN10CMEM2Alloc8FreeSizeEv:
.LFB379:
	.loc 1 216 0
	.cfi_startproc
.LVL81:
	mflr 0
	stwu 1,-16(1)
.LCFI24:
	.cfi_def_cfa_offset 16
	.cfi_register 65, 0
	stw 31,12(1)
	mr 31,3
	.cfi_offset 31, -4
.LVL82:
	stw 0,20(1)
	stw 30,8(1)
.LBB90:
	.loc 1 219 0
	li 30,0
	.cfi_offset 30, -8
	.cfi_offset 65, 4
.LBB91:
.LBB92:
	.loc 1 13 0
	lwz 3,12(3)
.LVL83:
	bl LWP_MutexLock
.LBE92:
.LBE91:
	.loc 1 219 0
	lwz 9,8(31)
	cmpwi 7,9,0
	beq- 7,.L71
	.loc 1 227 0
	lbz 0,12(9)
	cmpwi 7,0,0
	lwz 0,4(9)
	bne- 7,.L72
.LVL84:
.L64:
	.loc 1 233 0 discriminator 1
	cmpwi 7,0,0
	beq- 7,.L73
.LVL85:
	.loc 1 225 0
	mr 9,0
.L74:
	.loc 1 227 0
	lbz 0,12(9)
.LVL86:
	cmpwi 7,0,0
	lwz 0,4(9)
	beq- 7,.L64
.LVL87:
.L72:
	.loc 1 227 0 is_stmt 0 discriminator 1
	cmpwi 7,0,0
	beq- 7,.L65
	.loc 1 228 0 is_stmt 1
	lwz 9,0(9)
	add 30,30,9
.LVL88:
	.loc 1 225 0
	mr 9,0
	b .L74
.LVL89:
.L65:
	.loc 1 231 0 discriminator 1
	lwz 0,4(31)
	addi 30,30,-1
	subf 9,9,0
	srawi 9,9,5
	add 30,30,9
.LVL90:
.L67:
.LBB93:
.LBB94:
.LBB95:
	.loc 1 14 0
	lwz 3,12(31)
.LBE95:
.LBE94:
.LBE93:
	.loc 1 237 0
	slwi 30,30,5
.LVL91:
.LBB102:
.LBB99:
.LBB96:
	.loc 1 14 0
	bl LWP_MutexUnlock
.LBE96:
.LBE99:
.LBE102:
.LBE90:
	.loc 1 238 0
	lwz 0,20(1)
	mr 3,30
	lwz 31,12(1)
.LVL92:
	mtlr 0
	lwz 30,8(1)
	addi 1,1,16
	.cfi_remember_state
.LCFI25:
	.cfi_def_cfa_offset 0
	.cfi_restore 31
	.cfi_restore 30
	blr
.LVL93:
.L73:
.LCFI26:
	.cfi_restore_state
.LBB105:
	.loc 1 234 0
	lwz 0,0(9)
	lwz 11,4(31)
	nor 0,0,0
	subf 9,9,11
	add 30,0,30
	srawi 9,9,5
	add 30,30,9
.LVL94:
	b .L67
.LVL95:
.L71:
	.loc 1 220 0
	lwz 0,0(31)
	lwz 30,4(31)
.LBB103:
.LBB100:
.LBB97:
	.loc 1 14 0
	lwz 3,12(31)
.LBE97:
.LBE100:
.LBE103:
	.loc 1 220 0
	subf 30,0,30
.LVL96:
.LBB104:
.LBB101:
.LBB98:
	.loc 1 14 0
	bl LWP_MutexUnlock
.LBE98:
.LBE101:
.LBE104:
.LBE105:
	.loc 1 238 0
	lwz 0,20(1)
	mr 3,30
	lwz 31,12(1)
.LVL97:
	mtlr 0
	lwz 30,8(1)
	addi 1,1,16
	.cfi_restore 30
	.cfi_restore 31
.LCFI27:
	.cfi_def_cfa_offset 0
	blr
	.cfi_endproc
.LFE379:
	.size	_ZN10CMEM2Alloc8FreeSizeEv, .-_ZN10CMEM2Alloc8FreeSizeEv
.Letext0:
	.file 2 "d:\\devkitpro\\devkitppc\\bin\\../lib/gcc/powerpc-eabi/4.6.3/../../../../powerpc-eabi/include/stdint.h"
	.file 3 "d:/devkitPro/libogc/include/gctypes.h"
	.file 4 "d:/devkitPro/libogc/include/ogc/mutex.h"
	.file 5 "d:/Projects/SaveGameManager/savegame-manager-gx/trunk/source/Memory/mem2alloc.hpp"
	.file 6 "d:\\devkitpro\\devkitppc\\bin\\../lib/gcc/powerpc-eabi/4.6.3/include/stddef.h"
	.file 7 "d:\\devkitpro\\devkitppc\\bin\\../lib/gcc/powerpc-eabi/4.6.3/../../../../powerpc-eabi/include/c++/4.6.3/cstdlib"
	.file 8 "d:\\devkitpro\\devkitppc\\bin\\../lib/gcc/powerpc-eabi/4.6.3/../../../../powerpc-eabi/include/c++/4.6.3/bits/stl_algobase.h"
	.file 9 "d:\\devkitpro\\devkitppc\\bin\\../lib/gcc/powerpc-eabi/4.6.3/../../../../powerpc-eabi/include/c++/4.6.3/bits/cpp_type_traits.h"
	.file 10 "d:\\devkitpro\\devkitppc\\bin\\../lib/gcc/powerpc-eabi/4.6.3/../../../../powerpc-eabi/include/c++/4.6.3/debug/debug.h"
	.file 11 "d:\\devkitpro\\devkitppc\\bin\\../lib/gcc/powerpc-eabi/4.6.3/../../../../powerpc-eabi/include/stdlib.h"
	.file 12 "d:\\devkitpro\\devkitppc\\bin\\../lib/gcc/powerpc-eabi/4.6.3/../../../../powerpc-eabi/include/c++/4.6.3/ext/numeric_traits.h"
	.file 13 "<built-in>"
	.file 14 "d:\\devkitpro\\devkitppc\\bin\\../lib/gcc/powerpc-eabi/4.6.3/../../../../powerpc-eabi/include/c++/4.6.3/new"
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.4byte	0xc64
	.2byte	0x2
	.4byte	.Ldebug_abbrev0
	.byte	0x4
	.uleb128 0x1
	.4byte	.LASF89
	.byte	0x4
	.4byte	.LASF90
	.4byte	.LASF91
	.4byte	.Ltext0
	.4byte	.Letext0
	.4byte	.Ldebug_line0
	.uleb128 0x2
	.byte	0x1
	.byte	0x6
	.4byte	.LASF0
	.uleb128 0x3
	.4byte	.LASF4
	.byte	0x2
	.byte	0x2a
	.4byte	0x37
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
	.uleb128 0x4
	.byte	0x4
	.byte	0x5
	.string	"int"
	.uleb128 0x3
	.4byte	.LASF5
	.byte	0x2
	.byte	0x50
	.4byte	0x5e
	.uleb128 0x2
	.byte	0x4
	.byte	0x7
	.4byte	.LASF6
	.uleb128 0x2
	.byte	0x8
	.byte	0x5
	.4byte	.LASF7
	.uleb128 0x2
	.byte	0x8
	.byte	0x7
	.4byte	.LASF8
	.uleb128 0x5
	.string	"u8"
	.byte	0x3
	.byte	0x11
	.4byte	0x2c
	.uleb128 0x5
	.string	"u32"
	.byte	0x3
	.byte	0x13
	.4byte	0x53
	.uleb128 0x2
	.byte	0x4
	.byte	0x4
	.4byte	.LASF9
	.uleb128 0x2
	.byte	0x8
	.byte	0x4
	.4byte	.LASF10
	.uleb128 0x6
	.byte	0x4
	.4byte	0x9c
	.uleb128 0x2
	.byte	0x1
	.byte	0x8
	.4byte	.LASF11
	.uleb128 0x6
	.byte	0x4
	.4byte	0x96
	.uleb128 0x3
	.4byte	.LASF12
	.byte	0x4
	.byte	0x33
	.4byte	0x7d
	.uleb128 0x7
	.4byte	.LASF42
	.byte	0x10
	.byte	0x5
	.byte	0x9
	.4byte	0x302
	.uleb128 0x8
	.4byte	.LASF92
	.byte	0x20
	.byte	0x5
	.byte	0x1b
	.byte	0x3
	.4byte	0x102
	.uleb128 0x9
	.string	"s"
	.byte	0x5
	.byte	0x1d
	.4byte	0x5e
	.byte	0x2
	.byte	0x23
	.uleb128 0
	.uleb128 0xa
	.4byte	.LASF13
	.byte	0x5
	.byte	0x1e
	.4byte	0x302
	.byte	0x2
	.byte	0x23
	.uleb128 0x4
	.uleb128 0xa
	.4byte	.LASF14
	.byte	0x5
	.byte	0x1f
	.4byte	0x302
	.byte	0x2
	.byte	0x23
	.uleb128 0x8
	.uleb128 0x9
	.string	"f"
	.byte	0x5
	.byte	0x20
	.4byte	0x308
	.byte	0x2
	.byte	0x23
	.uleb128 0xc
	.byte	0
	.uleb128 0xb
	.4byte	.LASF15
	.byte	0x5
	.byte	0x22
	.4byte	0x30f
	.byte	0x2
	.byte	0x23
	.uleb128 0
	.byte	0x3
	.uleb128 0xb
	.4byte	.LASF16
	.byte	0x5
	.byte	0x23
	.4byte	0x30f
	.byte	0x2
	.byte	0x23
	.uleb128 0x4
	.byte	0x3
	.uleb128 0xb
	.4byte	.LASF17
	.byte	0x5
	.byte	0x24
	.4byte	0x30f
	.byte	0x2
	.byte	0x23
	.uleb128 0x8
	.byte	0x3
	.uleb128 0xb
	.4byte	.LASF18
	.byte	0x5
	.byte	0x25
	.4byte	0xa9
	.byte	0x2
	.byte	0x23
	.uleb128 0xc
	.byte	0x3
	.uleb128 0xc
	.byte	0x1
	.4byte	.LASF19
	.byte	0x5
	.byte	0xc
	.4byte	.LASF21
	.4byte	0x315
	.byte	0x1
	.4byte	0x157
	.4byte	0x163
	.uleb128 0xd
	.4byte	0x317
	.byte	0x1
	.uleb128 0xe
	.4byte	0x5e
	.byte	0
	.uleb128 0xf
	.byte	0x1
	.4byte	.LASF23
	.byte	0x5
	.byte	0xd
	.4byte	.LASF25
	.byte	0x1
	.4byte	0x178
	.4byte	0x184
	.uleb128 0xd
	.4byte	0x317
	.byte	0x1
	.uleb128 0xe
	.4byte	0x315
	.byte	0
	.uleb128 0xc
	.byte	0x1
	.4byte	.LASF20
	.byte	0x5
	.byte	0xe
	.4byte	.LASF22
	.4byte	0x315
	.byte	0x1
	.4byte	0x19d
	.4byte	0x1ae
	.uleb128 0xd
	.4byte	0x317
	.byte	0x1
	.uleb128 0xe
	.4byte	0x315
	.uleb128 0xe
	.4byte	0x5e
	.byte	0
	.uleb128 0xf
	.byte	0x1
	.4byte	.LASF24
	.byte	0x5
	.byte	0xf
	.4byte	.LASF26
	.byte	0x1
	.4byte	0x1c3
	.4byte	0x1cf
	.uleb128 0xd
	.4byte	0x317
	.byte	0x1
	.uleb128 0xe
	.4byte	0x5e
	.byte	0
	.uleb128 0xf
	.byte	0x1
	.4byte	.LASF24
	.byte	0x5
	.byte	0x10
	.4byte	.LASF27
	.byte	0x1
	.4byte	0x1e4
	.4byte	0x1f5
	.uleb128 0xd
	.4byte	0x317
	.byte	0x1
	.uleb128 0xe
	.4byte	0x315
	.uleb128 0xe
	.4byte	0x315
	.byte	0
	.uleb128 0xf
	.byte	0x1
	.4byte	.LASF28
	.byte	0x5
	.byte	0x11
	.4byte	.LASF29
	.byte	0x1
	.4byte	0x20a
	.4byte	0x211
	.uleb128 0xd
	.4byte	0x317
	.byte	0x1
	.byte	0
	.uleb128 0xf
	.byte	0x1
	.4byte	.LASF30
	.byte	0x5
	.byte	0x12
	.4byte	.LASF31
	.byte	0x1
	.4byte	0x226
	.4byte	0x22d
	.uleb128 0xd
	.4byte	0x317
	.byte	0x1
	.byte	0
	.uleb128 0x10
	.byte	0x1
	.4byte	.LASF32
	.byte	0x5
	.byte	0x13
	.4byte	.LASF33
	.4byte	0x5e
	.byte	0x1
	.4byte	0x248
	.uleb128 0xe
	.4byte	0x315
	.byte	0
	.uleb128 0xf
	.byte	0x1
	.4byte	.LASF34
	.byte	0x5
	.byte	0x14
	.4byte	.LASF35
	.byte	0x1
	.4byte	0x25d
	.4byte	0x269
	.uleb128 0xd
	.4byte	0x317
	.byte	0x1
	.uleb128 0xe
	.4byte	0x315
	.byte	0
	.uleb128 0xc
	.byte	0x1
	.4byte	.LASF36
	.byte	0x5
	.byte	0x15
	.4byte	.LASF37
	.4byte	0x315
	.byte	0x1
	.4byte	0x282
	.4byte	0x289
	.uleb128 0xd
	.4byte	0x31d
	.byte	0x1
	.byte	0
	.uleb128 0xf
	.byte	0x1
	.4byte	.LASF38
	.byte	0x5
	.byte	0x16
	.4byte	.LASF39
	.byte	0x1
	.4byte	0x29e
	.4byte	0x2af
	.uleb128 0xd
	.4byte	0x31d
	.byte	0x1
	.uleb128 0xe
	.4byte	0x328
	.uleb128 0xe
	.4byte	0x32e
	.byte	0
	.uleb128 0xc
	.byte	0x1
	.4byte	.LASF40
	.byte	0x5
	.byte	0x17
	.4byte	.LASF41
	.4byte	0x5e
	.byte	0x1
	.4byte	0x2c8
	.4byte	0x2cf
	.uleb128 0xd
	.4byte	0x317
	.byte	0x1
	.byte	0
	.uleb128 0x11
	.byte	0x1
	.4byte	.LASF42
	.byte	0x5
	.byte	0x19
	.byte	0x1
	.4byte	0x2e0
	.4byte	0x2e7
	.uleb128 0xd
	.4byte	0x317
	.byte	0x1
	.byte	0
	.uleb128 0x12
	.byte	0x1
	.4byte	.LASF42
	.byte	0x5
	.byte	0x27
	.byte	0x3
	.byte	0x1
	.4byte	0x2f5
	.uleb128 0xd
	.4byte	0x317
	.byte	0x1
	.uleb128 0xe
	.4byte	0x334
	.byte	0
	.byte	0
	.uleb128 0x6
	.byte	0x4
	.4byte	0xc0
	.uleb128 0x2
	.byte	0x1
	.byte	0x2
	.4byte	.LASF43
	.uleb128 0x6
	.byte	0x4
	.4byte	0xc0
	.uleb128 0x13
	.byte	0x4
	.uleb128 0x6
	.byte	0x4
	.4byte	0xb4
	.uleb128 0x6
	.byte	0x4
	.4byte	0x323
	.uleb128 0x14
	.4byte	0xb4
	.uleb128 0x15
	.byte	0x4
	.4byte	0x315
	.uleb128 0x15
	.byte	0x4
	.4byte	0x5e
	.uleb128 0x15
	.byte	0x4
	.4byte	0x33a
	.uleb128 0x14
	.4byte	0xb4
	.uleb128 0x2
	.byte	0x4
	.byte	0x7
	.4byte	.LASF44
	.uleb128 0x16
	.uleb128 0x6
	.byte	0x4
	.4byte	0x346
	.uleb128 0x6
	.byte	0x4
	.4byte	0x353
	.uleb128 0x14
	.4byte	0x9c
	.uleb128 0x3
	.4byte	.LASF45
	.byte	0x6
	.byte	0xd4
	.4byte	0x5e
	.uleb128 0x2
	.byte	0x4
	.byte	0x5
	.4byte	.LASF46
	.uleb128 0x17
	.string	"std"
	.byte	0xd
	.byte	0
	.4byte	0x453
	.uleb128 0x18
	.4byte	.LASF93
	.byte	0xa
	.byte	0x31
	.uleb128 0x19
	.byte	0x7
	.byte	0x66
	.4byte	0x4a8
	.uleb128 0x19
	.byte	0x7
	.byte	0x67
	.4byte	0x4dc
	.uleb128 0x19
	.byte	0x7
	.byte	0x6b
	.4byte	0x4e7
	.uleb128 0x19
	.byte	0x7
	.byte	0x6c
	.4byte	0x4fe
	.uleb128 0x19
	.byte	0x7
	.byte	0x6d
	.4byte	0x515
	.uleb128 0x19
	.byte	0x7
	.byte	0x6e
	.4byte	0x52c
	.uleb128 0x19
	.byte	0x7
	.byte	0x6f
	.4byte	0x543
	.uleb128 0x19
	.byte	0x7
	.byte	0x71
	.4byte	0x58f
	.uleb128 0x19
	.byte	0x7
	.byte	0x74
	.4byte	0x5ab
	.uleb128 0x19
	.byte	0x7
	.byte	0x76
	.4byte	0x5c2
	.uleb128 0x19
	.byte	0x7
	.byte	0x79
	.4byte	0x5de
	.uleb128 0x19
	.byte	0x7
	.byte	0x7a
	.4byte	0x5fa
	.uleb128 0x19
	.byte	0x7
	.byte	0x7b
	.4byte	0x628
	.uleb128 0x19
	.byte	0x7
	.byte	0x7d
	.4byte	0x649
	.uleb128 0x19
	.byte	0x7
	.byte	0x7e
	.4byte	0x66b
	.uleb128 0x19
	.byte	0x7
	.byte	0x80
	.4byte	0x678
	.uleb128 0x19
	.byte	0x7
	.byte	0x81
	.4byte	0x68b
	.uleb128 0x19
	.byte	0x7
	.byte	0x82
	.4byte	0x6a7
	.uleb128 0x19
	.byte	0x7
	.byte	0x83
	.4byte	0x6c8
	.uleb128 0x19
	.byte	0x7
	.byte	0x84
	.4byte	0x6e9
	.uleb128 0x19
	.byte	0x7
	.byte	0x86
	.4byte	0x700
	.uleb128 0x19
	.byte	0x7
	.byte	0x87
	.4byte	0x72c
	.uleb128 0x1a
	.4byte	.LASF49
	.byte	0x1
	.uleb128 0x1b
	.byte	0x1
	.4byte	.LASF47
	.byte	0x8
	.byte	0xbb
	.4byte	0x811
	.byte	0x1
	.4byte	0x441
	.uleb128 0x1c
	.string	"_Tp"
	.4byte	0x5e
	.uleb128 0xe
	.4byte	0x811
	.uleb128 0xe
	.4byte	0x811
	.byte	0
	.uleb128 0x1d
	.4byte	.LASF94
	.byte	0xe
	.byte	0x47
	.4byte	.LASF95
	.4byte	0xc55
	.byte	0x1
	.byte	0x1
	.byte	0
	.uleb128 0x1e
	.4byte	.LASF48
	.byte	0x9
	.byte	0x46
	.4byte	0x465
	.uleb128 0x1a
	.4byte	.LASF50
	.byte	0x1
	.byte	0
	.uleb128 0x2
	.byte	0x8
	.byte	0x4
	.4byte	.LASF51
	.uleb128 0x1e
	.4byte	.LASF52
	.byte	0xa
	.byte	0x38
	.4byte	0x47f
	.uleb128 0x1f
	.byte	0xa
	.byte	0x39
	.4byte	0x375
	.byte	0
	.uleb128 0x20
	.byte	0x8
	.byte	0xb
	.byte	0x1e
	.4byte	.LASF55
	.4byte	0x4a8
	.uleb128 0xa
	.4byte	.LASF53
	.byte	0xb
	.byte	0x1f
	.4byte	0x4c
	.byte	0x2
	.byte	0x23
	.uleb128 0
	.uleb128 0x9
	.string	"rem"
	.byte	0xb
	.byte	0x20
	.4byte	0x4c
	.byte	0x2
	.byte	0x23
	.uleb128 0x4
	.byte	0
	.uleb128 0x3
	.4byte	.LASF54
	.byte	0xb
	.byte	0x21
	.4byte	0x47f
	.uleb128 0x20
	.byte	0x8
	.byte	0xb
	.byte	0x24
	.4byte	.LASF56
	.4byte	0x4dc
	.uleb128 0xa
	.4byte	.LASF53
	.byte	0xb
	.byte	0x25
	.4byte	0x363
	.byte	0x2
	.byte	0x23
	.uleb128 0
	.uleb128 0x9
	.string	"rem"
	.byte	0xb
	.byte	0x26
	.4byte	0x363
	.byte	0x2
	.byte	0x23
	.uleb128 0x4
	.byte	0
	.uleb128 0x3
	.4byte	.LASF57
	.byte	0xb
	.byte	0x27
	.4byte	0x4b3
	.uleb128 0x1b
	.byte	0x1
	.4byte	.LASF58
	.byte	0xb
	.byte	0x40
	.4byte	0x4c
	.byte	0x1
	.4byte	0x4fe
	.uleb128 0xe
	.4byte	0x347
	.byte	0
	.uleb128 0x1b
	.byte	0x1
	.4byte	.LASF59
	.byte	0xb
	.byte	0x41
	.4byte	0x8f
	.byte	0x1
	.4byte	0x515
	.uleb128 0xe
	.4byte	0x34d
	.byte	0
	.uleb128 0x1b
	.byte	0x1
	.4byte	.LASF60
	.byte	0xb
	.byte	0x45
	.4byte	0x4c
	.byte	0x1
	.4byte	0x52c
	.uleb128 0xe
	.4byte	0x34d
	.byte	0
	.uleb128 0x1b
	.byte	0x1
	.4byte	.LASF61
	.byte	0xb
	.byte	0x47
	.4byte	0x363
	.byte	0x1
	.4byte	0x543
	.uleb128 0xe
	.4byte	0x34d
	.byte	0
	.uleb128 0x1b
	.byte	0x1
	.4byte	.LASF62
	.byte	0xb
	.byte	0x49
	.4byte	0x315
	.byte	0x1
	.4byte	0x56e
	.uleb128 0xe
	.4byte	0x56e
	.uleb128 0xe
	.4byte	0x56e
	.uleb128 0xe
	.4byte	0x358
	.uleb128 0xe
	.4byte	0x358
	.uleb128 0xe
	.4byte	0x575
	.byte	0
	.uleb128 0x6
	.byte	0x4
	.4byte	0x574
	.uleb128 0x21
	.uleb128 0x6
	.byte	0x4
	.4byte	0x57b
	.uleb128 0x22
	.4byte	0x4c
	.4byte	0x58f
	.uleb128 0xe
	.4byte	0x56e
	.uleb128 0xe
	.4byte	0x56e
	.byte	0
	.uleb128 0x23
	.byte	0x1
	.string	"div"
	.byte	0xb
	.byte	0x4f
	.4byte	0x4a8
	.byte	0x1
	.4byte	0x5ab
	.uleb128 0xe
	.4byte	0x4c
	.uleb128 0xe
	.4byte	0x4c
	.byte	0
	.uleb128 0x1b
	.byte	0x1
	.4byte	.LASF63
	.byte	0xb
	.byte	0x52
	.4byte	0x96
	.byte	0x1
	.4byte	0x5c2
	.uleb128 0xe
	.4byte	0x34d
	.byte	0
	.uleb128 0x1b
	.byte	0x1
	.4byte	.LASF64
	.byte	0xb
	.byte	0x5b
	.4byte	0x4dc
	.byte	0x1
	.4byte	0x5de
	.uleb128 0xe
	.4byte	0x363
	.uleb128 0xe
	.4byte	0x363
	.byte	0
	.uleb128 0x1b
	.byte	0x1
	.4byte	.LASF65
	.byte	0xb
	.byte	0x5d
	.4byte	0x4c
	.byte	0x1
	.4byte	0x5fa
	.uleb128 0xe
	.4byte	0x34d
	.uleb128 0xe
	.4byte	0x358
	.byte	0
	.uleb128 0x1b
	.byte	0x1
	.4byte	.LASF66
	.byte	0xb
	.byte	0x63
	.4byte	0x358
	.byte	0x1
	.4byte	0x61b
	.uleb128 0xe
	.4byte	0x61b
	.uleb128 0xe
	.4byte	0x34d
	.uleb128 0xe
	.4byte	0x358
	.byte	0
	.uleb128 0x6
	.byte	0x4
	.4byte	0x621
	.uleb128 0x2
	.byte	0x4
	.byte	0x5
	.4byte	.LASF67
	.uleb128 0x1b
	.byte	0x1
	.4byte	.LASF68
	.byte	0xb
	.byte	0x5f
	.4byte	0x4c
	.byte	0x1
	.4byte	0x649
	.uleb128 0xe
	.4byte	0x61b
	.uleb128 0xe
	.4byte	0x34d
	.uleb128 0xe
	.4byte	0x358
	.byte	0
	.uleb128 0x24
	.byte	0x1
	.4byte	.LASF69
	.byte	0xb
	.byte	0x77
	.byte	0x1
	.4byte	0x66b
	.uleb128 0xe
	.4byte	0x315
	.uleb128 0xe
	.4byte	0x358
	.uleb128 0xe
	.4byte	0x358
	.uleb128 0xe
	.4byte	0x575
	.byte	0
	.uleb128 0x25
	.byte	0x1
	.4byte	.LASF96
	.byte	0xb
	.byte	0x78
	.4byte	0x4c
	.byte	0x1
	.uleb128 0x24
	.byte	0x1
	.4byte	.LASF70
	.byte	0xb
	.byte	0x7d
	.byte	0x1
	.4byte	0x68b
	.uleb128 0xe
	.4byte	0x5e
	.byte	0
	.uleb128 0x1b
	.byte	0x1
	.4byte	.LASF71
	.byte	0xb
	.byte	0x7e
	.4byte	0x8f
	.byte	0x1
	.4byte	0x6a7
	.uleb128 0xe
	.4byte	0x34d
	.uleb128 0xe
	.4byte	0xa3
	.byte	0
	.uleb128 0x1b
	.byte	0x1
	.4byte	.LASF72
	.byte	0xb
	.byte	0x87
	.4byte	0x363
	.byte	0x1
	.4byte	0x6c8
	.uleb128 0xe
	.4byte	0x34d
	.uleb128 0xe
	.4byte	0xa3
	.uleb128 0xe
	.4byte	0x4c
	.byte	0
	.uleb128 0x1b
	.byte	0x1
	.4byte	.LASF73
	.byte	0xb
	.byte	0x89
	.4byte	0x33f
	.byte	0x1
	.4byte	0x6e9
	.uleb128 0xe
	.4byte	0x34d
	.uleb128 0xe
	.4byte	0xa3
	.uleb128 0xe
	.4byte	0x4c
	.byte	0
	.uleb128 0x1b
	.byte	0x1
	.4byte	.LASF74
	.byte	0xb
	.byte	0x8c
	.4byte	0x4c
	.byte	0x1
	.4byte	0x700
	.uleb128 0xe
	.4byte	0x34d
	.byte	0
	.uleb128 0x1b
	.byte	0x1
	.4byte	.LASF75
	.byte	0xb
	.byte	0x65
	.4byte	0x358
	.byte	0x1
	.4byte	0x721
	.uleb128 0xe
	.4byte	0x96
	.uleb128 0xe
	.4byte	0x721
	.uleb128 0xe
	.4byte	0x358
	.byte	0
	.uleb128 0x6
	.byte	0x4
	.4byte	0x727
	.uleb128 0x14
	.4byte	0x621
	.uleb128 0x1b
	.byte	0x1
	.4byte	.LASF76
	.byte	0xb
	.byte	0x61
	.4byte	0x4c
	.byte	0x1
	.4byte	0x748
	.uleb128 0xe
	.4byte	0x96
	.uleb128 0xe
	.4byte	0x621
	.byte	0
	.uleb128 0x26
	.4byte	0x416
	.byte	0x1
	.byte	0xe
	.byte	0x45
	.uleb128 0x27
	.4byte	0x45e
	.byte	0x1
	.byte	0xc
	.byte	0x37
	.4byte	0x7a3
	.uleb128 0x28
	.4byte	.LASF77
	.byte	0xc
	.byte	0x3a
	.4byte	0x7a3
	.byte	0x1
	.byte	0x1
	.uleb128 0x28
	.4byte	.LASF78
	.byte	0xc
	.byte	0x3b
	.4byte	0x7a3
	.byte	0x1
	.byte	0x1
	.uleb128 0x28
	.4byte	.LASF79
	.byte	0xc
	.byte	0x3f
	.4byte	0x7a8
	.byte	0x1
	.byte	0x1
	.uleb128 0x28
	.4byte	.LASF80
	.byte	0xc
	.byte	0x40
	.4byte	0x7a3
	.byte	0x1
	.byte	0x1
	.uleb128 0x29
	.4byte	.LASF81
	.4byte	0x4c
	.uleb128 0x29
	.4byte	.LASF81
	.4byte	0x4c
	.byte	0
	.uleb128 0x14
	.4byte	0x4c
	.uleb128 0x14
	.4byte	0x308
	.uleb128 0x7
	.4byte	.LASF82
	.byte	0x4
	.byte	0x1
	.byte	0x9
	.4byte	0x800
	.uleb128 0xb
	.4byte	.LASF18
	.byte	0x1
	.byte	0xb
	.4byte	0x800
	.byte	0x2
	.byte	0x23
	.uleb128 0
	.byte	0x3
	.uleb128 0x11
	.byte	0x1
	.4byte	.LASF82
	.byte	0x1
	.byte	0xd
	.byte	0x1
	.4byte	0x7d9
	.4byte	0x7e5
	.uleb128 0xd
	.4byte	0x80b
	.byte	0x1
	.uleb128 0xe
	.4byte	0x805
	.byte	0
	.uleb128 0x2a
	.byte	0x1
	.4byte	.LASF83
	.byte	0x1
	.byte	0xe
	.byte	0x1
	.4byte	0x7f2
	.uleb128 0xd
	.4byte	0x80b
	.byte	0x1
	.uleb128 0xd
	.4byte	0x4c
	.byte	0x1
	.byte	0
	.byte	0
	.uleb128 0x14
	.4byte	0x805
	.uleb128 0x15
	.byte	0x4
	.4byte	0xa9
	.uleb128 0x6
	.byte	0x4
	.4byte	0x7ad
	.uleb128 0x15
	.byte	0x4
	.4byte	0x817
	.uleb128 0x14
	.4byte	0x5e
	.uleb128 0x2b
	.4byte	0x41c
	.byte	0x3
	.4byte	0x846
	.uleb128 0x1c
	.string	"_Tp"
	.4byte	0x5e
	.uleb128 0x2c
	.string	"__a"
	.byte	0x8
	.byte	0xbb
	.4byte	0x846
	.uleb128 0x2c
	.string	"__b"
	.byte	0x8
	.byte	0xbb
	.4byte	0x84b
	.byte	0
	.uleb128 0x14
	.4byte	0x811
	.uleb128 0x14
	.4byte	0x811
	.uleb128 0x2d
	.4byte	0x7c8
	.byte	0x2
	.4byte	0x85e
	.4byte	0x872
	.uleb128 0x2e
	.4byte	.LASF84
	.4byte	0x872
	.byte	0x1
	.uleb128 0x2c
	.string	"m"
	.byte	0x1
	.byte	0xd
	.4byte	0x877
	.byte	0
	.uleb128 0x14
	.4byte	0x80b
	.uleb128 0x14
	.4byte	0x805
	.uleb128 0x2d
	.4byte	0x7e5
	.byte	0x2
	.4byte	0x88a
	.4byte	0x89f
	.uleb128 0x2e
	.4byte	.LASF84
	.4byte	0x872
	.byte	0x1
	.uleb128 0x2e
	.4byte	.LASF85
	.4byte	0x7a3
	.byte	0x1
	.byte	0
	.uleb128 0x2f
	.4byte	0x1ae
	.byte	0x1
	.byte	0x11
	.4byte	.LFB371
	.4byte	.LFE371
	.4byte	.LLST0
	.4byte	0x8ba
	.4byte	0x8d8
	.uleb128 0x30
	.4byte	.LASF84
	.4byte	0x8d8
	.byte	0x1
	.4byte	.LLST1
	.uleb128 0x31
	.4byte	.LASF86
	.byte	0x1
	.byte	0x11
	.4byte	0x5e
	.4byte	.LLST2
	.byte	0
	.uleb128 0x14
	.4byte	0x317
	.uleb128 0x2f
	.4byte	0x1cf
	.byte	0x1
	.byte	0x1b
	.4byte	.LFB372
	.4byte	.LFE372
	.4byte	.LLST3
	.4byte	0x8f8
	.4byte	0x925
	.uleb128 0x30
	.4byte	.LASF84
	.4byte	0x8d8
	.byte	0x1
	.4byte	.LLST4
	.uleb128 0x31
	.4byte	.LASF87
	.byte	0x1
	.byte	0x1b
	.4byte	0x315
	.4byte	.LLST5
	.uleb128 0x32
	.string	"end"
	.byte	0x1
	.byte	0x1b
	.4byte	0x315
	.4byte	.LLST6
	.byte	0
	.uleb128 0x2f
	.4byte	0x1f5
	.byte	0x1
	.byte	0x22
	.4byte	.LFB373
	.4byte	.LFE373
	.4byte	.LLST7
	.4byte	0x940
	.4byte	0x94f
	.uleb128 0x30
	.4byte	.LASF84
	.4byte	0x8d8
	.byte	0x1
	.4byte	.LLST8
	.byte	0
	.uleb128 0x33
	.4byte	0x211
	.byte	0x1
	.byte	0x2e
	.4byte	.LFB374
	.4byte	.LFE374
	.byte	0x2
	.byte	0x71
	.sleb128 0
	.4byte	0x969
	.4byte	0x976
	.uleb128 0x34
	.4byte	.LASF84
	.4byte	0x8d8
	.byte	0x1
	.byte	0x1
	.byte	0x53
	.byte	0
	.uleb128 0x35
	.4byte	0x22d
	.byte	0x1
	.byte	0x34
	.4byte	.LFB375
	.4byte	.LFE375
	.byte	0x2
	.byte	0x71
	.sleb128 0
	.4byte	0x99a
	.uleb128 0x32
	.string	"p"
	.byte	0x1
	.byte	0x34
	.4byte	0x315
	.4byte	.LLST9
	.byte	0
	.uleb128 0x2f
	.4byte	0x13e
	.byte	0x1
	.byte	0x39
	.4byte	.LFB376
	.4byte	.LFE376
	.4byte	.LLST10
	.4byte	0x9b5
	.4byte	0xa42
	.uleb128 0x30
	.4byte	.LASF84
	.4byte	0x8d8
	.byte	0x1
	.4byte	.LLST11
	.uleb128 0x32
	.string	"s"
	.byte	0x1
	.byte	0x39
	.4byte	0x5e
	.4byte	.LLST12
	.uleb128 0x36
	.4byte	.Ldebug_ranges0+0
	.uleb128 0x37
	.4byte	.LASF88
	.byte	0x1
	.byte	0x3e
	.4byte	0x7ad
	.4byte	.LLST13
	.uleb128 0x38
	.string	"i"
	.byte	0x1
	.byte	0x4e
	.4byte	0x30f
	.4byte	.LLST14
	.uleb128 0x38
	.string	"j"
	.byte	0x1
	.byte	0x4f
	.4byte	0x30f
	.4byte	.LLST15
	.uleb128 0x39
	.4byte	0x850
	.4byte	.LBB14
	.4byte	.LBE14
	.byte	0x1
	.byte	0x3e
	.4byte	0xa27
	.uleb128 0x3a
	.4byte	0x868
	.4byte	.LLST16
	.uleb128 0x3b
	.4byte	0x85e
	.byte	0x6
	.byte	0xf2
	.4byte	.Ldebug_info0+2517
	.sleb128 0
	.byte	0
	.uleb128 0x3c
	.4byte	0x87c
	.4byte	.LBB16
	.4byte	.Ldebug_ranges0+0x38
	.byte	0x1
	.byte	0x72
	.uleb128 0x3a
	.4byte	0x88a
	.4byte	.LLST17
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0x2f
	.4byte	0x163
	.byte	0x1
	.byte	0x75
	.4byte	.LFB377
	.4byte	.LFE377
	.4byte	.LLST18
	.4byte	0xa5d
	.4byte	0xada
	.uleb128 0x30
	.4byte	.LASF84
	.4byte	0x8d8
	.byte	0x1
	.4byte	.LLST19
	.uleb128 0x32
	.string	"p"
	.byte	0x1
	.byte	0x75
	.4byte	0x315
	.4byte	.LLST20
	.uleb128 0x36
	.4byte	.Ldebug_ranges0+0x60
	.uleb128 0x37
	.4byte	.LASF88
	.byte	0x1
	.byte	0x7a
	.4byte	0x7ad
	.4byte	.LLST21
	.uleb128 0x38
	.string	"i"
	.byte	0x1
	.byte	0x7b
	.4byte	0x30f
	.4byte	.LLST22
	.uleb128 0x39
	.4byte	0x850
	.4byte	.LBB41
	.4byte	.LBE41
	.byte	0x1
	.byte	0x7a
	.4byte	0xabf
	.uleb128 0x3a
	.4byte	0x868
	.4byte	.LLST23
	.uleb128 0x3a
	.4byte	0x85e
	.4byte	.LLST24
	.byte	0
	.uleb128 0x3d
	.4byte	0x87c
	.4byte	.LBB43
	.4byte	.LBE43
	.byte	0x1
	.byte	0x93
	.uleb128 0x3a
	.4byte	0x88a
	.4byte	.LLST25
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0x2f
	.4byte	0x184
	.byte	0x1
	.byte	0x96
	.4byte	.LFB378
	.4byte	.LFE378
	.4byte	.LLST26
	.4byte	0xaf5
	.4byte	0xbbc
	.uleb128 0x30
	.4byte	.LASF84
	.4byte	0x8d8
	.byte	0x1
	.4byte	.LLST27
	.uleb128 0x32
	.string	"p"
	.byte	0x1
	.byte	0x96
	.4byte	0x315
	.4byte	.LLST28
	.uleb128 0x32
	.string	"s"
	.byte	0x1
	.byte	0x96
	.4byte	0x5e
	.4byte	.LLST29
	.uleb128 0x36
	.4byte	.Ldebug_ranges0+0x80
	.uleb128 0x38
	.string	"i"
	.byte	0x1
	.byte	0x98
	.4byte	0x30f
	.4byte	.LLST30
	.uleb128 0x38
	.string	"j"
	.byte	0x1
	.byte	0x99
	.4byte	0x30f
	.4byte	.LLST31
	.uleb128 0x38
	.string	"n"
	.byte	0x1
	.byte	0x9a
	.4byte	0x315
	.4byte	.LLST32
	.uleb128 0x36
	.4byte	.Ldebug_ranges0+0xc0
	.uleb128 0x37
	.4byte	.LASF88
	.byte	0x1
	.byte	0xa4
	.4byte	0x7ad
	.4byte	.LLST33
	.uleb128 0x3e
	.4byte	0x850
	.4byte	.LBB61
	.4byte	.Ldebug_ranges0+0x100
	.byte	0x1
	.byte	0xa4
	.4byte	0xb83
	.uleb128 0x3a
	.4byte	0x868
	.4byte	.LLST34
	.uleb128 0x3a
	.4byte	0x85e
	.4byte	.LLST35
	.byte	0
	.uleb128 0x39
	.4byte	0x87c
	.4byte	.LBB65
	.4byte	.LBE65
	.byte	0x1
	.byte	0xcc
	.4byte	0xba0
	.uleb128 0x3a
	.4byte	0x88a
	.4byte	.LLST36
	.byte	0
	.uleb128 0x3d
	.4byte	0x87c
	.4byte	.LBB68
	.4byte	.LBE68
	.byte	0x1
	.byte	0xcc
	.uleb128 0x3a
	.4byte	0x88a
	.4byte	.LLST37
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0x2f
	.4byte	0x2af
	.byte	0x1
	.byte	0xd7
	.4byte	.LFB379
	.4byte	.LFE379
	.4byte	.LLST38
	.4byte	0xbd7
	.4byte	0xc55
	.uleb128 0x30
	.4byte	.LASF84
	.4byte	0x8d8
	.byte	0x1
	.4byte	.LLST39
	.uleb128 0x36
	.4byte	.Ldebug_ranges0+0x118
	.uleb128 0x3f
	.4byte	.LASF88
	.byte	0x1
	.byte	0xd9
	.4byte	0x7ad
	.uleb128 0x38
	.string	"i"
	.byte	0x1
	.byte	0xde
	.4byte	0x30f
	.4byte	.LLST40
	.uleb128 0x37
	.4byte	.LASF86
	.byte	0x1
	.byte	0xdf
	.4byte	0x5e
	.4byte	.LLST41
	.uleb128 0x39
	.4byte	0x850
	.4byte	.LBB91
	.4byte	.LBE91
	.byte	0x1
	.byte	0xd9
	.4byte	0xc3a
	.uleb128 0x3a
	.4byte	0x868
	.4byte	.LLST42
	.uleb128 0x3b
	.4byte	0x85e
	.byte	0x6
	.byte	0xf2
	.4byte	.Ldebug_info0+3050
	.sleb128 0
	.byte	0
	.uleb128 0x3c
	.4byte	0x87c
	.4byte	.LBB93
	.4byte	.Ldebug_ranges0+0x130
	.byte	0x1
	.byte	0xed
	.uleb128 0x3a
	.4byte	0x88a
	.4byte	.LLST43
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0x14
	.4byte	0x748
	.uleb128 0x40
	.4byte	0x769
	.4byte	.LASF97
	.4byte	0x7fffffff
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
	.uleb128 0x1b
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
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x7
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
	.uleb128 0x8
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
	.uleb128 0x9
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
	.byte	0
	.byte	0
	.uleb128 0xb
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
	.uleb128 0xc
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
	.uleb128 0xd
	.uleb128 0x5
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x34
	.uleb128 0xc
	.byte	0
	.byte	0
	.uleb128 0xe
	.uleb128 0x5
	.byte	0
	.uleb128 0x49
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
	.uleb128 0x3c
	.uleb128 0xc
	.uleb128 0x64
	.uleb128 0x13
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
	.uleb128 0x3c
	.uleb128 0xc
	.uleb128 0x64
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x12
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
	.uleb128 0x15
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0x17
	.uleb128 0x39
	.byte	0x1
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x18
	.uleb128 0x39
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x19
	.uleb128 0x8
	.byte	0
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x18
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x1a
	.uleb128 0x13
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3c
	.uleb128 0xc
	.byte	0
	.byte	0
	.uleb128 0x1b
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
	.uleb128 0x3c
	.uleb128 0xc
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x1c
	.uleb128 0x2f
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x1d
	.uleb128 0x34
	.byte	0
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
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3c
	.uleb128 0xc
	.byte	0
	.byte	0
	.uleb128 0x1e
	.uleb128 0x39
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x1f
	.uleb128 0x3a
	.byte	0
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x18
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x20
	.uleb128 0x13
	.byte	0x1
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x2007
	.uleb128 0xe
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x21
	.uleb128 0x26
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0x22
	.uleb128 0x15
	.byte	0x1
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x23
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3c
	.uleb128 0xc
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x24
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
	.uleb128 0x1
	.uleb128 0x13
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
	.uleb128 0x3c
	.uleb128 0xc
	.byte	0
	.byte	0
	.uleb128 0x26
	.uleb128 0x13
	.byte	0
	.uleb128 0x47
	.uleb128 0x13
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x27
	.uleb128 0x13
	.byte	0x1
	.uleb128 0x47
	.uleb128 0x13
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
	.uleb128 0x28
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
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3c
	.uleb128 0xc
	.byte	0
	.byte	0
	.uleb128 0x29
	.uleb128 0x2f
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x2a
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
	.byte	0
	.byte	0
	.uleb128 0x2b
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x47
	.uleb128 0x13
	.uleb128 0x20
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x2c
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
	.uleb128 0x2d
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
	.uleb128 0x2e
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
	.uleb128 0x2f
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
	.uleb128 0x64
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x30
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x34
	.uleb128 0xc
	.uleb128 0x2
	.uleb128 0x6
	.byte	0
	.byte	0
	.uleb128 0x31
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
	.uleb128 0x32
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
	.uleb128 0x33
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
	.uleb128 0x64
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x34
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x34
	.uleb128 0xc
	.uleb128 0x2
	.uleb128 0xa
	.byte	0
	.byte	0
	.uleb128 0x35
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
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x36
	.uleb128 0xb
	.byte	0x1
	.uleb128 0x55
	.uleb128 0x6
	.byte	0
	.byte	0
	.uleb128 0x37
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
	.uleb128 0x6
	.byte	0
	.byte	0
	.uleb128 0x38
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
	.uleb128 0x39
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
	.uleb128 0x3a
	.uleb128 0x5
	.byte	0
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x6
	.byte	0
	.byte	0
	.uleb128 0x3b
	.uleb128 0x5
	.byte	0
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0xa
	.byte	0
	.byte	0
	.uleb128 0x3c
	.uleb128 0x1d
	.byte	0x1
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x52
	.uleb128 0x1
	.uleb128 0x55
	.uleb128 0x6
	.uleb128 0x58
	.uleb128 0xb
	.uleb128 0x59
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x3d
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
	.uleb128 0x3e
	.uleb128 0x1d
	.byte	0x1
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x52
	.uleb128 0x1
	.uleb128 0x55
	.uleb128 0x6
	.uleb128 0x58
	.uleb128 0xb
	.uleb128 0x59
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x3f
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
	.byte	0
	.byte	0
	.uleb128 0x40
	.uleb128 0x34
	.byte	0
	.uleb128 0x47
	.uleb128 0x13
	.uleb128 0x2007
	.uleb128 0xe
	.uleb128 0x1c
	.uleb128 0x6
	.byte	0
	.byte	0
	.byte	0
	.section	.debug_loc,"",@progbits
.Ldebug_loc0:
.LLST0:
	.4byte	.LFB371-.Ltext0
	.4byte	.LCFI0-.Ltext0
	.2byte	0x2
	.byte	0x71
	.sleb128 0
	.4byte	.LCFI0-.Ltext0
	.4byte	.LCFI1-.Ltext0
	.2byte	0x2
	.byte	0x71
	.sleb128 24
	.4byte	.LCFI1-.Ltext0
	.4byte	.LFE371-.Ltext0
	.2byte	0x2
	.byte	0x71
	.sleb128 0
	.4byte	0
	.4byte	0
.LLST1:
	.4byte	.LVL0-.Ltext0
	.4byte	.LVL1-1-.Ltext0
	.2byte	0x1
	.byte	0x53
	.4byte	.LVL1-1-.Ltext0
	.4byte	.LVL3-.Ltext0
	.2byte	0x1
	.byte	0x6f
	.4byte	0
	.4byte	0
.LLST2:
	.4byte	.LVL0-.Ltext0
	.4byte	.LVL1-1-.Ltext0
	.2byte	0x1
	.byte	0x54
	.4byte	0
	.4byte	0
.LLST3:
	.4byte	.LFB372-.Ltext0
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
	.4byte	.LFE372-.Ltext0
	.2byte	0x2
	.byte	0x71
	.sleb128 0
	.4byte	0
	.4byte	0
.LLST4:
	.4byte	.LVL4-.Ltext0
	.4byte	.LVL8-.Ltext0
	.2byte	0x1
	.byte	0x53
	.4byte	.LVL8-.Ltext0
	.4byte	.LVL9-1-.Ltext0
	.2byte	0x3
	.byte	0x73
	.sleb128 -12
	.byte	0x9f
	.4byte	0
	.4byte	0
.LLST5:
	.4byte	.LVL4-.Ltext0
	.4byte	.LVL5-.Ltext0
	.2byte	0x1
	.byte	0x54
	.4byte	.LVL5-.Ltext0
	.4byte	.LVL7-.Ltext0
	.2byte	0x3
	.byte	0x74
	.sleb128 -31
	.byte	0x9f
	.4byte	0
	.4byte	0
.LLST6:
	.4byte	.LVL4-.Ltext0
	.4byte	.LVL6-.Ltext0
	.2byte	0x1
	.byte	0x55
	.4byte	0
	.4byte	0
.LLST7:
	.4byte	.LFB373-.Ltext0
	.4byte	.LCFI4-.Ltext0
	.2byte	0x2
	.byte	0x71
	.sleb128 0
	.4byte	.LCFI4-.Ltext0
	.4byte	.LCFI5-.Ltext0
	.2byte	0x2
	.byte	0x71
	.sleb128 16
	.4byte	.LCFI5-.Ltext0
	.4byte	.LCFI6-.Ltext0
	.2byte	0x2
	.byte	0x71
	.sleb128 0
	.4byte	.LCFI6-.Ltext0
	.4byte	.LCFI7-.Ltext0
	.2byte	0x2
	.byte	0x71
	.sleb128 16
	.4byte	.LCFI7-.Ltext0
	.4byte	.LFE373-.Ltext0
	.2byte	0x2
	.byte	0x71
	.sleb128 0
	.4byte	0
	.4byte	0
.LLST8:
	.4byte	.LVL10-.Ltext0
	.4byte	.LVL11-.Ltext0
	.2byte	0x1
	.byte	0x53
	.4byte	.LVL11-.Ltext0
	.4byte	.LVL12-.Ltext0
	.2byte	0x1
	.byte	0x6f
	.4byte	.LVL13-.Ltext0
	.4byte	.LVL14-.Ltext0
	.2byte	0x1
	.byte	0x6f
	.4byte	0
	.4byte	0
.LLST9:
	.4byte	.LVL16-.Ltext0
	.4byte	.LVL17-.Ltext0
	.2byte	0x1
	.byte	0x53
	.4byte	.LVL17-.Ltext0
	.4byte	.LFE375-.Ltext0
	.2byte	0x1
	.byte	0x59
	.4byte	0
	.4byte	0
.LLST10:
	.4byte	.LFB376-.Ltext0
	.4byte	.LCFI8-.Ltext0
	.2byte	0x2
	.byte	0x71
	.sleb128 0
	.4byte	.LCFI8-.Ltext0
	.4byte	.LCFI9-.Ltext0
	.2byte	0x2
	.byte	0x71
	.sleb128 24
	.4byte	.LCFI9-.Ltext0
	.4byte	.LCFI10-.Ltext0
	.2byte	0x2
	.byte	0x71
	.sleb128 0
	.4byte	.LCFI10-.Ltext0
	.4byte	.LCFI11-.Ltext0
	.2byte	0x2
	.byte	0x71
	.sleb128 24
	.4byte	.LCFI11-.Ltext0
	.4byte	.LCFI12-.Ltext0
	.2byte	0x2
	.byte	0x71
	.sleb128 0
	.4byte	.LCFI12-.Ltext0
	.4byte	.LCFI13-.Ltext0
	.2byte	0x2
	.byte	0x71
	.sleb128 24
	.4byte	.LCFI13-.Ltext0
	.4byte	.LFE376-.Ltext0
	.2byte	0x2
	.byte	0x71
	.sleb128 0
	.4byte	0
	.4byte	0
.LLST11:
	.4byte	.LVL18-.Ltext0
	.4byte	.LVL20-.Ltext0
	.2byte	0x1
	.byte	0x53
	.4byte	.LVL20-.Ltext0
	.4byte	.LVL28-.Ltext0
	.2byte	0x1
	.byte	0x6e
	.4byte	.LVL30-.Ltext0
	.4byte	.LVL34-.Ltext0
	.2byte	0x1
	.byte	0x6e
	.4byte	.LVL36-.Ltext0
	.4byte	.LVL38-.Ltext0
	.2byte	0x1
	.byte	0x6e
	.4byte	0
	.4byte	0
.LLST12:
	.4byte	.LVL18-.Ltext0
	.4byte	.LVL19-.Ltext0
	.2byte	0x1
	.byte	0x54
	.4byte	.LVL21-.Ltext0
	.4byte	.LVL29-.Ltext0
	.2byte	0x1
	.byte	0x6f
	.4byte	.LVL30-.Ltext0
	.4byte	.LVL35-.Ltext0
	.2byte	0x1
	.byte	0x6f
	.4byte	.LVL36-.Ltext0
	.4byte	.LVL39-.Ltext0
	.2byte	0x1
	.byte	0x6f
	.4byte	0
	.4byte	0
.LLST13:
	.4byte	.LVL19-.Ltext0
	.4byte	.LVL20-.Ltext0
	.2byte	0x5
	.byte	0x73
	.sleb128 12
	.byte	0x9f
	.byte	0x93
	.uleb128 0x4
	.4byte	.LVL20-.Ltext0
	.4byte	.LVL28-.Ltext0
	.2byte	0x5
	.byte	0x8e
	.sleb128 12
	.byte	0x9f
	.byte	0x93
	.uleb128 0x4
	.4byte	.LVL30-.Ltext0
	.4byte	.LVL34-.Ltext0
	.2byte	0x5
	.byte	0x8e
	.sleb128 12
	.byte	0x9f
	.byte	0x93
	.uleb128 0x4
	.4byte	.LVL36-.Ltext0
	.4byte	.LVL38-.Ltext0
	.2byte	0x5
	.byte	0x8e
	.sleb128 12
	.byte	0x9f
	.byte	0x93
	.uleb128 0x4
	.4byte	0
	.4byte	0
.LLST14:
	.4byte	.LVL22-.Ltext0
	.4byte	.LVL23-.Ltext0
	.2byte	0x1
	.byte	0x50
	.4byte	.LVL25-.Ltext0
	.4byte	.LVL26-.Ltext0
	.2byte	0x2
	.byte	0x79
	.sleb128 4
	.4byte	.LVL26-.Ltext0
	.4byte	.LVL27-.Ltext0
	.2byte	0x1
	.byte	0x5b
	.4byte	0
	.4byte	0
.LLST15:
	.4byte	.LVL22-.Ltext0
	.4byte	.LVL23-.Ltext0
	.2byte	0x1
	.byte	0x59
	.4byte	.LVL24-.Ltext0
	.4byte	.LVL27-.Ltext0
	.2byte	0x1
	.byte	0x59
	.4byte	.LVL31-.Ltext0
	.4byte	.LVL32-.Ltext0
	.2byte	0x1
	.byte	0x5b
	.4byte	0
	.4byte	0
.LLST16:
	.4byte	.LVL19-.Ltext0
	.4byte	.LVL20-.Ltext0
	.2byte	0x3
	.byte	0x73
	.sleb128 12
	.byte	0x9f
	.4byte	.LVL20-.Ltext0
	.4byte	.LVL28-.Ltext0
	.2byte	0x3
	.byte	0x8e
	.sleb128 12
	.byte	0x9f
	.4byte	.LVL30-.Ltext0
	.4byte	.LVL34-.Ltext0
	.2byte	0x3
	.byte	0x8e
	.sleb128 12
	.byte	0x9f
	.4byte	.LVL36-.Ltext0
	.4byte	.LVL38-.Ltext0
	.2byte	0x3
	.byte	0x8e
	.sleb128 12
	.byte	0x9f
	.4byte	0
	.4byte	0
.LLST17:
	.4byte	.LVL27-.Ltext0
	.4byte	.LVL30-.Ltext0
	.2byte	0x6
	.byte	0xf2
	.4byte	.Ldebug_info0+2517
	.sleb128 0
	.4byte	.LVL33-.Ltext0
	.4byte	.LVL36-.Ltext0
	.2byte	0x6
	.byte	0xf2
	.4byte	.Ldebug_info0+2517
	.sleb128 0
	.4byte	.LVL37-.Ltext0
	.4byte	.LFE376-.Ltext0
	.2byte	0x6
	.byte	0xf2
	.4byte	.Ldebug_info0+2517
	.sleb128 0
	.4byte	0
	.4byte	0
.LLST18:
	.4byte	.LFB377-.Ltext0
	.4byte	.LCFI14-.Ltext0
	.2byte	0x2
	.byte	0x71
	.sleb128 0
	.4byte	.LCFI14-.Ltext0
	.4byte	.LCFI15-.Ltext0
	.2byte	0x2
	.byte	0x71
	.sleb128 16
	.4byte	.LCFI15-.Ltext0
	.4byte	.LCFI16-.Ltext0
	.2byte	0x2
	.byte	0x71
	.sleb128 0
	.4byte	.LCFI16-.Ltext0
	.4byte	.LFE377-.Ltext0
	.2byte	0x2
	.byte	0x71
	.sleb128 16
	.4byte	0
	.4byte	0
.LLST19:
	.4byte	.LVL40-.Ltext0
	.4byte	.LVL42-.Ltext0
	.2byte	0x1
	.byte	0x53
	.4byte	.LVL42-.Ltext0
	.4byte	.LVL47-.Ltext0
	.2byte	0x1
	.byte	0x6e
	.4byte	.LVL49-.Ltext0
	.4byte	.LFE377-.Ltext0
	.2byte	0x1
	.byte	0x6e
	.4byte	0
	.4byte	0
.LLST20:
	.4byte	.LVL40-.Ltext0
	.4byte	.LVL43-1-.Ltext0
	.2byte	0x1
	.byte	0x54
	.4byte	.LVL43-1-.Ltext0
	.4byte	.LVL48-.Ltext0
	.2byte	0x1
	.byte	0x6f
	.4byte	.LVL49-.Ltext0
	.4byte	.LFE377-.Ltext0
	.2byte	0x1
	.byte	0x6f
	.4byte	0
	.4byte	0
.LLST21:
	.4byte	.LVL41-.Ltext0
	.4byte	.LVL42-.Ltext0
	.2byte	0x5
	.byte	0x73
	.sleb128 12
	.byte	0x9f
	.byte	0x93
	.uleb128 0x4
	.4byte	.LVL42-.Ltext0
	.4byte	.LVL46-.Ltext0
	.2byte	0x5
	.byte	0x8e
	.sleb128 12
	.byte	0x9f
	.byte	0x93
	.uleb128 0x4
	.4byte	.LVL49-.Ltext0
	.4byte	.LFE377-.Ltext0
	.2byte	0x5
	.byte	0x8e
	.sleb128 12
	.byte	0x9f
	.byte	0x93
	.uleb128 0x4
	.4byte	0
	.4byte	0
.LLST22:
	.4byte	.LVL44-.Ltext0
	.4byte	.LVL45-.Ltext0
	.2byte	0x1
	.byte	0x5a
	.4byte	.LVL49-.Ltext0
	.4byte	.LVL50-.Ltext0
	.2byte	0x1
	.byte	0x5b
	.4byte	.LVL50-.Ltext0
	.4byte	.LFE377-.Ltext0
	.2byte	0x1
	.byte	0x5a
	.4byte	0
	.4byte	0
.LLST23:
	.4byte	.LVL41-.Ltext0
	.4byte	.LVL42-.Ltext0
	.2byte	0x3
	.byte	0x73
	.sleb128 12
	.byte	0x9f
	.4byte	.LVL42-.Ltext0
	.4byte	.LVL46-.Ltext0
	.2byte	0x3
	.byte	0x8e
	.sleb128 12
	.byte	0x9f
	.4byte	.LVL49-.Ltext0
	.4byte	.LFE377-.Ltext0
	.2byte	0x3
	.byte	0x8e
	.sleb128 12
	.byte	0x9f
	.4byte	0
	.4byte	0
.LLST24:
	.4byte	.LVL41-.Ltext0
	.4byte	.LVL46-.Ltext0
	.2byte	0x6
	.byte	0xf2
	.4byte	.Ldebug_info0+2685
	.sleb128 0
	.4byte	.LVL49-.Ltext0
	.4byte	.LFE377-.Ltext0
	.2byte	0x6
	.byte	0xf2
	.4byte	.Ldebug_info0+2685
	.sleb128 0
	.4byte	0
	.4byte	0
.LLST25:
	.4byte	.LVL45-.Ltext0
	.4byte	.LVL46-.Ltext0
	.2byte	0x6
	.byte	0xf2
	.4byte	.Ldebug_info0+2685
	.sleb128 0
	.4byte	0
	.4byte	0
.LLST26:
	.4byte	.LFB378-.Ltext0
	.4byte	.LCFI17-.Ltext0
	.2byte	0x2
	.byte	0x71
	.sleb128 0
	.4byte	.LCFI17-.Ltext0
	.4byte	.LCFI18-.Ltext0
	.2byte	0x2
	.byte	0x71
	.sleb128 24
	.4byte	.LCFI18-.Ltext0
	.4byte	.LCFI19-.Ltext0
	.2byte	0x2
	.byte	0x71
	.sleb128 0
	.4byte	.LCFI19-.Ltext0
	.4byte	.LCFI20-.Ltext0
	.2byte	0x2
	.byte	0x71
	.sleb128 24
	.4byte	.LCFI20-.Ltext0
	.4byte	.LCFI21-.Ltext0
	.2byte	0x2
	.byte	0x71
	.sleb128 0
	.4byte	.LCFI21-.Ltext0
	.4byte	.LCFI22-.Ltext0
	.2byte	0x2
	.byte	0x71
	.sleb128 24
	.4byte	.LCFI22-.Ltext0
	.4byte	.LCFI23-.Ltext0
	.2byte	0x2
	.byte	0x71
	.sleb128 0
	.4byte	.LCFI23-.Ltext0
	.4byte	.LFE378-.Ltext0
	.2byte	0x2
	.byte	0x71
	.sleb128 24
	.4byte	0
	.4byte	0
.LLST27:
	.4byte	.LVL51-.Ltext0
	.4byte	.LVL54-.Ltext0
	.2byte	0x1
	.byte	0x53
	.4byte	.LVL54-.Ltext0
	.4byte	.LVL61-.Ltext0
	.2byte	0x1
	.byte	0x6e
	.4byte	.LVL62-.Ltext0
	.4byte	.LVL71-.Ltext0
	.2byte	0x1
	.byte	0x6e
	.4byte	.LVL72-.Ltext0
	.4byte	.LVL76-.Ltext0
	.2byte	0x1
	.byte	0x6e
	.4byte	.LVL76-.Ltext0
	.4byte	.LVL78-1-.Ltext0
	.2byte	0x1
	.byte	0x53
	.4byte	.LVL78-.Ltext0
	.4byte	.LFE378-.Ltext0
	.2byte	0x1
	.byte	0x6e
	.4byte	0
	.4byte	0
.LLST28:
	.4byte	.LVL51-.Ltext0
	.4byte	.LVL55-1-.Ltext0
	.2byte	0x1
	.byte	0x54
	.4byte	.LVL55-1-.Ltext0
	.4byte	.LVL59-.Ltext0
	.2byte	0x1
	.byte	0x6f
	.4byte	.LVL59-.Ltext0
	.4byte	.LVL60-1-.Ltext0
	.2byte	0x3
	.byte	0x7b
	.sleb128 32
	.byte	0x9f
	.4byte	.LVL62-.Ltext0
	.4byte	.LVL67-.Ltext0
	.2byte	0x1
	.byte	0x6f
	.4byte	.LVL67-.Ltext0
	.4byte	.LVL68-1-.Ltext0
	.2byte	0x1
	.byte	0x54
	.4byte	.LVL72-.Ltext0
	.4byte	.LVL73-.Ltext0
	.2byte	0x1
	.byte	0x6f
	.4byte	.LVL73-.Ltext0
	.4byte	.LVL74-.Ltext0
	.2byte	0x3
	.byte	0x7b
	.sleb128 32
	.byte	0x9f
	.4byte	.LVL74-.Ltext0
	.4byte	.LVL75-.Ltext0
	.2byte	0x1
	.byte	0x54
	.4byte	.LVL75-.Ltext0
	.4byte	.LVL77-.Ltext0
	.2byte	0x1
	.byte	0x6f
	.4byte	.LVL78-.Ltext0
	.4byte	.LVL80-.Ltext0
	.2byte	0x1
	.byte	0x6f
	.4byte	0
	.4byte	0
.LLST29:
	.4byte	.LVL51-.Ltext0
	.4byte	.LVL53-.Ltext0
	.2byte	0x1
	.byte	0x55
	.4byte	.LVL53-.Ltext0
	.4byte	.LVL55-1-.Ltext0
	.2byte	0x3
	.byte	0x75
	.sleb128 1
	.byte	0x9f
	.4byte	.LVL56-.Ltext0
	.4byte	.LVL58-.Ltext0
	.2byte	0x3
	.byte	0x8d
	.sleb128 1
	.byte	0x9f
	.4byte	.LVL58-.Ltext0
	.4byte	.LVL59-.Ltext0
	.2byte	0x1
	.byte	0x6c
	.4byte	.LVL62-.Ltext0
	.4byte	.LVL69-.Ltext0
	.2byte	0x1
	.byte	0x6c
	.4byte	.LVL72-.Ltext0
	.4byte	.LVL74-.Ltext0
	.2byte	0x3
	.byte	0x8d
	.sleb128 1
	.byte	0x9f
	.4byte	.LVL74-.Ltext0
	.4byte	.LVL78-1-.Ltext0
	.2byte	0x1
	.byte	0x55
	.4byte	.LVL78-.Ltext0
	.4byte	.LFE378-.Ltext0
	.2byte	0x1
	.byte	0x6c
	.4byte	0
	.4byte	0
.LLST30:
	.4byte	.LVL56-.Ltext0
	.4byte	.LVL60-1-.Ltext0
	.2byte	0x1
	.byte	0x5b
	.4byte	.LVL62-.Ltext0
	.4byte	.LVL64-1-.Ltext0
	.2byte	0x1
	.byte	0x5b
	.4byte	.LVL72-.Ltext0
	.4byte	.LVL74-.Ltext0
	.2byte	0x1
	.byte	0x5b
	.4byte	.LVL78-.Ltext0
	.4byte	.LVL79-.Ltext0
	.2byte	0x1
	.byte	0x5b
	.4byte	0
	.4byte	0
.LLST31:
	.4byte	.LVL57-.Ltext0
	.4byte	.LVL59-.Ltext0
	.2byte	0x1
	.byte	0x59
	.4byte	0
	.4byte	0
.LLST32:
	.4byte	.LVL65-.Ltext0
	.4byte	.LVL66-1-.Ltext0
	.2byte	0x1
	.byte	0x53
	.4byte	.LVL66-1-.Ltext0
	.4byte	.LVL70-.Ltext0
	.2byte	0x1
	.byte	0x6d
	.4byte	.LVL70-.Ltext0
	.4byte	.LVL72-.Ltext0
	.2byte	0x1
	.byte	0x53
	.4byte	.LVL79-.Ltext0
	.4byte	.LFE378-.Ltext0
	.2byte	0x1
	.byte	0x53
	.4byte	0
	.4byte	0
.LLST33:
	.4byte	.LVL56-.Ltext0
	.4byte	.LVL61-.Ltext0
	.2byte	0x5
	.byte	0x8e
	.sleb128 12
	.byte	0x9f
	.byte	0x93
	.uleb128 0x4
	.4byte	.LVL62-.Ltext0
	.4byte	.LVL71-.Ltext0
	.2byte	0x5
	.byte	0x8e
	.sleb128 12
	.byte	0x9f
	.byte	0x93
	.uleb128 0x4
	.4byte	.LVL72-.Ltext0
	.4byte	.LVL74-.Ltext0
	.2byte	0x5
	.byte	0x8e
	.sleb128 12
	.byte	0x9f
	.byte	0x93
	.uleb128 0x4
	.4byte	.LVL78-.Ltext0
	.4byte	.LFE378-.Ltext0
	.2byte	0x5
	.byte	0x8e
	.sleb128 12
	.byte	0x9f
	.byte	0x93
	.uleb128 0x4
	.4byte	0
	.4byte	0
.LLST34:
	.4byte	.LVL56-.Ltext0
	.4byte	.LVL61-.Ltext0
	.2byte	0x3
	.byte	0x8e
	.sleb128 12
	.byte	0x9f
	.4byte	.LVL62-.Ltext0
	.4byte	.LVL71-.Ltext0
	.2byte	0x3
	.byte	0x8e
	.sleb128 12
	.byte	0x9f
	.4byte	.LVL72-.Ltext0
	.4byte	.LVL74-.Ltext0
	.2byte	0x3
	.byte	0x8e
	.sleb128 12
	.byte	0x9f
	.4byte	.LVL78-.Ltext0
	.4byte	.LFE378-.Ltext0
	.2byte	0x3
	.byte	0x8e
	.sleb128 12
	.byte	0x9f
	.4byte	0
	.4byte	0
.LLST35:
	.4byte	.LVL56-.Ltext0
	.4byte	.LVL74-.Ltext0
	.2byte	0x6
	.byte	0xf2
	.4byte	.Ldebug_info0+2894
	.sleb128 0
	.4byte	.LVL78-.Ltext0
	.4byte	.LFE378-.Ltext0
	.2byte	0x6
	.byte	0xf2
	.4byte	.Ldebug_info0+2894
	.sleb128 0
	.4byte	0
	.4byte	0
.LLST36:
	.4byte	.LVL59-.Ltext0
	.4byte	.LVL60-.Ltext0
	.2byte	0x6
	.byte	0xf2
	.4byte	.Ldebug_info0+2894
	.sleb128 0
	.4byte	0
	.4byte	0
.LLST37:
	.4byte	.LVL63-.Ltext0
	.4byte	.LVL72-.Ltext0
	.2byte	0x6
	.byte	0xf2
	.4byte	.Ldebug_info0+2894
	.sleb128 0
	.4byte	.LVL79-.Ltext0
	.4byte	.LFE378-.Ltext0
	.2byte	0x6
	.byte	0xf2
	.4byte	.Ldebug_info0+2894
	.sleb128 0
	.4byte	0
	.4byte	0
.LLST38:
	.4byte	.LFB379-.Ltext0
	.4byte	.LCFI24-.Ltext0
	.2byte	0x2
	.byte	0x71
	.sleb128 0
	.4byte	.LCFI24-.Ltext0
	.4byte	.LCFI25-.Ltext0
	.2byte	0x2
	.byte	0x71
	.sleb128 16
	.4byte	.LCFI25-.Ltext0
	.4byte	.LCFI26-.Ltext0
	.2byte	0x2
	.byte	0x71
	.sleb128 0
	.4byte	.LCFI26-.Ltext0
	.4byte	.LCFI27-.Ltext0
	.2byte	0x2
	.byte	0x71
	.sleb128 16
	.4byte	.LCFI27-.Ltext0
	.4byte	.LFE379-.Ltext0
	.2byte	0x2
	.byte	0x71
	.sleb128 0
	.4byte	0
	.4byte	0
.LLST39:
	.4byte	.LVL81-.Ltext0
	.4byte	.LVL83-.Ltext0
	.2byte	0x1
	.byte	0x53
	.4byte	.LVL84-.Ltext0
	.4byte	.LVL92-.Ltext0
	.2byte	0x1
	.byte	0x6f
	.4byte	.LVL93-.Ltext0
	.4byte	.LVL97-.Ltext0
	.2byte	0x1
	.byte	0x6f
	.4byte	0
	.4byte	0
.LLST40:
	.4byte	.LVL85-.Ltext0
	.4byte	.LVL86-.Ltext0
	.2byte	0x1
	.byte	0x50
	.4byte	.LVL86-.Ltext0
	.4byte	.LVL87-.Ltext0
	.2byte	0x1
	.byte	0x59
	.4byte	.LVL88-.Ltext0
	.4byte	.LVL89-.Ltext0
	.2byte	0x1
	.byte	0x50
	.4byte	0
	.4byte	0
.LLST41:
	.4byte	.LVL85-.Ltext0
	.4byte	.LVL87-.Ltext0
	.2byte	0x1
	.byte	0x6e
	.4byte	.LVL88-.Ltext0
	.4byte	.LVL89-.Ltext0
	.2byte	0x1
	.byte	0x6e
	.4byte	.LVL90-.Ltext0
	.4byte	.LVL91-.Ltext0
	.2byte	0x1
	.byte	0x6e
	.4byte	.LVL94-.Ltext0
	.4byte	.LVL95-.Ltext0
	.2byte	0x1
	.byte	0x6e
	.4byte	0
	.4byte	0
.LLST42:
	.4byte	.LVL82-.Ltext0
	.4byte	.LVL83-.Ltext0
	.2byte	0x3
	.byte	0x73
	.sleb128 12
	.byte	0x9f
	.4byte	.LVL83-.Ltext0
	.4byte	.LVL92-.Ltext0
	.2byte	0x3
	.byte	0x8f
	.sleb128 12
	.byte	0x9f
	.4byte	.LVL93-.Ltext0
	.4byte	.LVL97-.Ltext0
	.2byte	0x3
	.byte	0x8f
	.sleb128 12
	.byte	0x9f
	.4byte	0
	.4byte	0
.LLST43:
	.4byte	.LVL90-.Ltext0
	.4byte	.LVL93-.Ltext0
	.2byte	0x6
	.byte	0xf2
	.4byte	.Ldebug_info0+3050
	.sleb128 0
	.4byte	.LVL96-.Ltext0
	.4byte	.LFE379-.Ltext0
	.2byte	0x6
	.byte	0xf2
	.4byte	.Ldebug_info0+3050
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
	.section	.debug_ranges,"",@progbits
.Ldebug_ranges0:
	.4byte	.LBB13-.Ltext0
	.4byte	.LBE13-.Ltext0
	.4byte	.LBB28-.Ltext0
	.4byte	.LBE28-.Ltext0
	.4byte	.LBB29-.Ltext0
	.4byte	.LBE29-.Ltext0
	.4byte	.LBB30-.Ltext0
	.4byte	.LBE30-.Ltext0
	.4byte	.LBB31-.Ltext0
	.4byte	.LBE31-.Ltext0
	.4byte	.LBB32-.Ltext0
	.4byte	.LBE32-.Ltext0
	.4byte	0
	.4byte	0
	.4byte	.LBB16-.Ltext0
	.4byte	.LBE16-.Ltext0
	.4byte	.LBB25-.Ltext0
	.4byte	.LBE25-.Ltext0
	.4byte	.LBB26-.Ltext0
	.4byte	.LBE26-.Ltext0
	.4byte	.LBB27-.Ltext0
	.4byte	.LBE27-.Ltext0
	.4byte	0
	.4byte	0
	.4byte	.LBB40-.Ltext0
	.4byte	.LBE40-.Ltext0
	.4byte	.LBB46-.Ltext0
	.4byte	.LBE46-.Ltext0
	.4byte	.LBB47-.Ltext0
	.4byte	.LBE47-.Ltext0
	.4byte	0
	.4byte	0
	.4byte	.LBB59-.Ltext0
	.4byte	.LBE59-.Ltext0
	.4byte	.LBB77-.Ltext0
	.4byte	.LBE77-.Ltext0
	.4byte	.LBB78-.Ltext0
	.4byte	.LBE78-.Ltext0
	.4byte	.LBB79-.Ltext0
	.4byte	.LBE79-.Ltext0
	.4byte	.LBB80-.Ltext0
	.4byte	.LBE80-.Ltext0
	.4byte	.LBB81-.Ltext0
	.4byte	.LBE81-.Ltext0
	.4byte	.LBB82-.Ltext0
	.4byte	.LBE82-.Ltext0
	.4byte	0
	.4byte	0
	.4byte	.LBB60-.Ltext0
	.4byte	.LBE60-.Ltext0
	.4byte	.LBB71-.Ltext0
	.4byte	.LBE71-.Ltext0
	.4byte	.LBB72-.Ltext0
	.4byte	.LBE72-.Ltext0
	.4byte	.LBB73-.Ltext0
	.4byte	.LBE73-.Ltext0
	.4byte	.LBB74-.Ltext0
	.4byte	.LBE74-.Ltext0
	.4byte	.LBB75-.Ltext0
	.4byte	.LBE75-.Ltext0
	.4byte	.LBB76-.Ltext0
	.4byte	.LBE76-.Ltext0
	.4byte	0
	.4byte	0
	.4byte	.LBB61-.Ltext0
	.4byte	.LBE61-.Ltext0
	.4byte	.LBB64-.Ltext0
	.4byte	.LBE64-.Ltext0
	.4byte	0
	.4byte	0
	.4byte	.LBB90-.Ltext0
	.4byte	.LBE90-.Ltext0
	.4byte	.LBB105-.Ltext0
	.4byte	.LBE105-.Ltext0
	.4byte	0
	.4byte	0
	.4byte	.LBB93-.Ltext0
	.4byte	.LBE93-.Ltext0
	.4byte	.LBB102-.Ltext0
	.4byte	.LBE102-.Ltext0
	.4byte	.LBB103-.Ltext0
	.4byte	.LBE103-.Ltext0
	.4byte	.LBB104-.Ltext0
	.4byte	.LBE104-.Ltext0
	.4byte	0
	.4byte	0
	.section	.debug_line,"",@progbits
.Ldebug_line0:
	.section	.debug_str,"MS",@progbits,1
.LASF35:
	.string	"_ZN10CMEM2Alloc15forceEndAddressEPv"
.LASF37:
	.string	"_ZNK10CMEM2Alloc13getEndAddressEv"
.LASF69:
	.string	"qsort"
.LASF83:
	.string	"~LockMutex"
.LASF18:
	.string	"m_mutex"
.LASF50:
	.string	"__numeric_traits_integer<int>"
.LASF66:
	.string	"mbstowcs"
.LASF2:
	.string	"short int"
.LASF45:
	.string	"size_t"
.LASF62:
	.string	"bsearch"
.LASF81:
	.string	"_Value"
.LASF58:
	.string	"atexit"
.LASF27:
	.string	"_ZN10CMEM2Alloc4initEPvS0_"
.LASF90:
	.string	"d:/Projects/SaveGameManager/savegame-manager-gx/trunk/source/Memory/mem2alloc.cpp"
.LASF91:
	.string	"d:\\\\Projects\\\\SaveGameManager\\\\savegame-manager-gx\\\\trunk\\\\build"
.LASF63:
	.string	"getenv"
.LASF49:
	.string	"nothrow_t"
.LASF96:
	.string	"rand"
.LASF20:
	.string	"reallocate"
.LASF47:
	.string	"min<unsigned int>"
.LASF22:
	.string	"_ZN10CMEM2Alloc10reallocateEPvj"
.LASF43:
	.string	"bool"
.LASF0:
	.string	"signed char"
.LASF9:
	.string	"float"
.LASF68:
	.string	"mbtowc"
.LASF79:
	.string	"__is_signed"
.LASF7:
	.string	"long long int"
.LASF97:
	.string	"_ZN9__gnu_cxx24__numeric_traits_integerIiE5__maxE"
.LASF34:
	.string	"forceEndAddress"
.LASF42:
	.string	"CMEM2Alloc"
.LASF32:
	.string	"usableSize"
.LASF29:
	.string	"_ZN10CMEM2Alloc7cleanupEv"
.LASF87:
	.string	"addr"
.LASF72:
	.string	"strtol"
.LASF30:
	.string	"clear"
.LASF76:
	.string	"wctomb"
.LASF1:
	.string	"unsigned char"
.LASF59:
	.string	"atof"
.LASF89:
	.string	"GNU C++ 4.6.3"
.LASF61:
	.string	"atol"
.LASF75:
	.string	"wcstombs"
.LASF17:
	.string	"m_first"
.LASF51:
	.string	"long double"
.LASF15:
	.string	"m_baseAddress"
.LASF25:
	.string	"_ZN10CMEM2Alloc7releaseEPv"
.LASF54:
	.string	"div_t"
.LASF28:
	.string	"cleanup"
.LASF52:
	.string	"__gnu_debug"
.LASF48:
	.string	"__gnu_cxx"
.LASF56:
	.string	"6ldiv_t"
.LASF8:
	.string	"long long unsigned int"
.LASF5:
	.string	"uint32_t"
.LASF78:
	.string	"__max"
.LASF26:
	.string	"_ZN10CMEM2Alloc4initEj"
.LASF6:
	.string	"unsigned int"
.LASF60:
	.string	"atoi"
.LASF12:
	.string	"mutex_t"
.LASF65:
	.string	"mblen"
.LASF44:
	.string	"long unsigned int"
.LASF57:
	.string	"ldiv_t"
.LASF64:
	.string	"ldiv"
.LASF21:
	.string	"_ZN10CMEM2Alloc8allocateEj"
.LASF19:
	.string	"allocate"
.LASF53:
	.string	"quot"
.LASF3:
	.string	"short unsigned int"
.LASF33:
	.string	"_ZN10CMEM2Alloc10usableSizeEPv"
.LASF67:
	.string	"wchar_t"
.LASF11:
	.string	"char"
.LASF40:
	.string	"FreeSize"
.LASF46:
	.string	"long int"
.LASF41:
	.string	"_ZN10CMEM2Alloc8FreeSizeEv"
.LASF88:
	.string	"lock"
.LASF85:
	.string	"__in_chrg"
.LASF84:
	.string	"this"
.LASF36:
	.string	"getEndAddress"
.LASF71:
	.string	"strtod"
.LASF95:
	.string	"_ZSt7nothrow"
.LASF74:
	.string	"system"
.LASF94:
	.string	"nothrow"
.LASF10:
	.string	"double"
.LASF23:
	.string	"release"
.LASF86:
	.string	"size"
.LASF38:
	.string	"info"
.LASF77:
	.string	"__min"
.LASF31:
	.string	"_ZN10CMEM2Alloc5clearEv"
.LASF4:
	.string	"uint8_t"
.LASF82:
	.string	"LockMutex"
.LASF16:
	.string	"m_endAddress"
.LASF92:
	.string	"SBlock"
.LASF55:
	.string	"5div_t"
.LASF39:
	.string	"_ZNK10CMEM2Alloc4infoERPvRj"
.LASF14:
	.string	"prev"
.LASF93:
	.string	"__debug"
.LASF70:
	.string	"srand"
.LASF80:
	.string	"__digits"
.LASF13:
	.string	"next"
.LASF24:
	.string	"init"
.LASF73:
	.string	"strtoul"
	.ident	"GCC: (devkitPPC release 26) 4.6.3"
