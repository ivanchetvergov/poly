	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 14, 0	sdk_version 15, 2
	.intel_syntax noprefix
	.file	1 "/Users/ivan/prog/sem3/cvm/lab6" "01.c"
	.globl	_main                           ## -- Begin function main
	.p2align	4, 0x90
_main:                                  ## @main
Lfunc_begin0:
	.loc	1 6 0                           ## 01.c:6:0
	.cfi_startproc
## %bb.0:
	push	ebp
	.cfi_def_cfa_offset 8
	.cfi_offset ebp, -8
	mov	ebp, esp
	.cfi_def_cfa_register ebp
	push	edi
	push	esi
	sub	esp, 32
	.cfi_offset esi, -16
	.cfi_offset edi, -12
	call	L0$pb
L0$pb:
	pop	eax
	mov	dword ptr [ebp - 20], eax       ## 4-byte Spill
	mov	dword ptr [ebp - 12], 0
Ltmp0:
	.loc	1 8 7 prologue_end              ## 01.c:8:7
	mov	ecx, dword ptr [eax + L_i$non_lazy_ptr-L0$pb]
	mov	dword ptr [ecx], 0
	.loc	1 9 7                           ## 01.c:9:7
	mov	eax, dword ptr [eax + L_cMas$non_lazy_ptr-L0$pb]
	mov	dword ptr [ebp - 16], eax
LBB0_1:                                 ## =>This Inner Loop Header: Depth=1
	.loc	1 0 7 is_stmt 0                 ## 01.c:0:7
	mov	eax, dword ptr [ebp - 20]       ## 4-byte Reload
Ltmp1:
	.loc	1 13 16 is_stmt 1               ## 01.c:13:16
	mov	eax, dword ptr [eax + L_i$non_lazy_ptr-L0$pb]
	cmp	dword ptr [eax], 3
	.loc	1 13 21 is_stmt 0               ## 01.c:13:21
	jbe	LBB0_4
## %bb.2:                               ##   in Loop: Header=BB0_1 Depth=1
	.loc	1 0 21                          ## 01.c:0:21
	mov	eax, dword ptr [ebp - 20]       ## 4-byte Reload
	.loc	1 13 27                         ## 01.c:13:27
	mov	eax, dword ptr [eax + L_i$non_lazy_ptr-L0$pb]
	cmp	dword ptr [eax], 6
Ltmp2:
	.loc	1 13 13                         ## 01.c:13:13
	je	LBB0_4
## %bb.3:                               ##   in Loop: Header=BB0_1 Depth=1
	.loc	1 0 13                          ## 01.c:0:13
	mov	eax, dword ptr [ebp - 20]       ## 4-byte Reload
Ltmp3:
	.loc	1 14 23 is_stmt 1               ## 01.c:14:23
	mov	ecx, dword ptr [eax + L_i$non_lazy_ptr-L0$pb]
	mov	ecx, dword ptr [ecx]
	.loc	1 14 21 is_stmt 0               ## 01.c:14:21
	mov	eax, dword ptr [eax + L_i$non_lazy_ptr-L0$pb]
	shl	ecx, 3
	.loc	1 14 26                         ## 01.c:14:26
	or	ecx, 3
	.loc	1 14 14                         ## 01.c:14:14
	mov	eax, dword ptr [ebp - 16]
	.loc	1 14 16                         ## 01.c:14:16
	mov	dword ptr [eax], ecx
	.loc	1 14 13                         ## 01.c:14:13
	jmp	LBB0_5
LBB0_4:                                 ##   in Loop: Header=BB0_1 Depth=1
	.loc	1 0 13                          ## 01.c:0:13
	mov	eax, dword ptr [ebp - 20]       ## 4-byte Reload
	.loc	1 16 26 is_stmt 1               ## 01.c:16:26
	mov	eax, dword ptr [eax + L_i$non_lazy_ptr-L0$pb]
	imul	eax, dword ptr [eax], 25
	.loc	1 16 30 is_stmt 0               ## 01.c:16:30
	mov	ecx, 3
	xor	edx, edx
	div	ecx
	mov	ecx, eax
	.loc	1 16 14                         ## 01.c:16:14
	mov	eax, dword ptr [ebp - 16]
	.loc	1 16 16                         ## 01.c:16:16
	mov	dword ptr [eax], ecx
Ltmp4:
LBB0_5:                                 ##   in Loop: Header=BB0_1 Depth=1
	.loc	1 0 16                          ## 01.c:0:16
	mov	eax, dword ptr [ebp - 20]       ## 4-byte Reload
	.loc	1 18 10 is_stmt 1               ## 01.c:18:10
	mov	ecx, dword ptr [eax + L_i$non_lazy_ptr-L0$pb]
	mov	ecx, dword ptr [ecx]
	mov	edx, dword ptr [eax + L_i$non_lazy_ptr-L0$pb]
	add	ecx, 1
	mov	eax, dword ptr [eax + L_i$non_lazy_ptr-L0$pb]
	mov	dword ptr [eax], ecx
	.loc	1 19 10                         ## 01.c:19:10
	mov	eax, dword ptr [ebp - 16]
	add	eax, 4
	mov	dword ptr [ebp - 16], eax
Ltmp5:
## %bb.6:                               ##   in Loop: Header=BB0_1 Depth=1
	.loc	1 0 10 is_stmt 0                ## 01.c:0:10
	mov	eax, dword ptr [ebp - 20]       ## 4-byte Reload
	.loc	1 20 16 is_stmt 1               ## 01.c:20:16
	mov	eax, dword ptr [eax + L_i$non_lazy_ptr-L0$pb]
	cmp	dword ptr [eax], 9
Ltmp6:
	.loc	1 20 5 is_stmt 0                ## 01.c:20:5
	jb	LBB0_1
Ltmp7:
## %bb.7:
	.loc	1 0 5                           ## 01.c:0:5
	mov	eax, dword ptr [ebp - 20]       ## 4-byte Reload
Ltmp8:
	.loc	1 22 12 is_stmt 1               ## 01.c:22:12
	mov	eax, dword ptr [eax + L_i$non_lazy_ptr-L0$pb]
	mov	dword ptr [eax], 0
LBB0_8:                                 ## =>This Inner Loop Header: Depth=1
	.loc	1 0 12 is_stmt 0                ## 01.c:0:12
	mov	eax, dword ptr [ebp - 20]       ## 4-byte Reload
Ltmp9:
	.loc	1 22 19                         ## 01.c:22:19
	mov	eax, dword ptr [eax + L_i$non_lazy_ptr-L0$pb]
	cmp	dword ptr [eax], 9
Ltmp10:
	.loc	1 22 5                          ## 01.c:22:5
	jae	LBB0_11
## %bb.9:                               ##   in Loop: Header=BB0_8 Depth=1
	.loc	1 0 5                           ## 01.c:0:5
	mov	esi, dword ptr [ebp - 20]       ## 4-byte Reload
Ltmp11:
	.loc	1 23 35 is_stmt 1               ## 01.c:23:35
	mov	eax, dword ptr [esi + L_i$non_lazy_ptr-L0$pb]
	mov	ecx, dword ptr [eax]
	.loc	1 23 43 is_stmt 0               ## 01.c:23:43
	mov	eax, dword ptr [esi + L_i$non_lazy_ptr-L0$pb]
	mov	edx, dword ptr [eax]
	.loc	1 23 38                         ## 01.c:23:38
	mov	eax, dword ptr [esi + L_cMas$non_lazy_ptr-L0$pb]
	mov	edi, dword ptr [esi + L_i$non_lazy_ptr-L0$pb]
	mov	eax, dword ptr [eax + 4*edx]
	.loc	1 23 9                          ## 01.c:23:9
	lea	edx, [esi + L_.str-L0$pb]
	mov	esi, dword ptr [esi + L_cMas$non_lazy_ptr-L0$pb]
	mov	dword ptr [esp], edx
	mov	dword ptr [esp + 4], ecx
	mov	dword ptr [esp + 8], eax
	call	_printf
## %bb.10:                              ##   in Loop: Header=BB0_8 Depth=1
	.loc	1 0 9                           ## 01.c:0:9
	mov	eax, dword ptr [ebp - 20]       ## 4-byte Reload
	.loc	1 22 25 is_stmt 1               ## 01.c:22:25
	mov	ecx, dword ptr [eax + L_i$non_lazy_ptr-L0$pb]
	mov	ecx, dword ptr [ecx]
	mov	edx, dword ptr [eax + L_i$non_lazy_ptr-L0$pb]
	add	ecx, 1
	mov	eax, dword ptr [eax + L_i$non_lazy_ptr-L0$pb]
	mov	dword ptr [eax], ecx
	.loc	1 22 5 is_stmt 0                ## 01.c:22:5
	jmp	LBB0_8
Ltmp12:
LBB0_11:
	.loc	1 25 5 is_stmt 1                ## 01.c:25:5
	xor	eax, eax
	.loc	1 25 5 epilogue_begin is_stmt 0 ## 01.c:25:5
	add	esp, 32
	pop	esi
	pop	edi
	pop	ebp
	ret
Ltmp13:
Lfunc_end0:
	.cfi_endproc
                                        ## -- End function
	.comm	_i,4,2                          ## @i
	.comm	_cMas,40,2                      ## @cMas
	.section	__TEXT,__cstring,cstring_literals
L_.str:                                 ## @.str
	.asciz	"cMas[%u] = %u\n"

	.section	__DWARF,__debug_abbrev,regular,debug
Lsection_abbrev:
	.byte	1                               ## Abbreviation Code
	.byte	17                              ## DW_TAG_compile_unit
	.byte	1                               ## DW_CHILDREN_yes
	.byte	37                              ## DW_AT_producer
	.byte	14                              ## DW_FORM_strp
	.byte	19                              ## DW_AT_language
	.byte	5                               ## DW_FORM_data2
	.byte	3                               ## DW_AT_name
	.byte	14                              ## DW_FORM_strp
	.ascii	"\202|"                         ## DW_AT_LLVM_sysroot
	.byte	14                              ## DW_FORM_strp
	.ascii	"\357\177"                      ## DW_AT_APPLE_sdk
	.byte	14                              ## DW_FORM_strp
	.byte	16                              ## DW_AT_stmt_list
	.byte	23                              ## DW_FORM_sec_offset
	.byte	27                              ## DW_AT_comp_dir
	.byte	14                              ## DW_FORM_strp
	.byte	17                              ## DW_AT_low_pc
	.byte	1                               ## DW_FORM_addr
	.byte	18                              ## DW_AT_high_pc
	.byte	6                               ## DW_FORM_data4
	.byte	0                               ## EOM(1)
	.byte	0                               ## EOM(2)
	.byte	2                               ## Abbreviation Code
	.byte	52                              ## DW_TAG_variable
	.byte	0                               ## DW_CHILDREN_no
	.byte	73                              ## DW_AT_type
	.byte	19                              ## DW_FORM_ref4
	.byte	58                              ## DW_AT_decl_file
	.byte	11                              ## DW_FORM_data1
	.byte	59                              ## DW_AT_decl_line
	.byte	11                              ## DW_FORM_data1
	.byte	2                               ## DW_AT_location
	.byte	24                              ## DW_FORM_exprloc
	.byte	0                               ## EOM(1)
	.byte	0                               ## EOM(2)
	.byte	3                               ## Abbreviation Code
	.byte	1                               ## DW_TAG_array_type
	.byte	1                               ## DW_CHILDREN_yes
	.byte	73                              ## DW_AT_type
	.byte	19                              ## DW_FORM_ref4
	.byte	0                               ## EOM(1)
	.byte	0                               ## EOM(2)
	.byte	4                               ## Abbreviation Code
	.byte	33                              ## DW_TAG_subrange_type
	.byte	0                               ## DW_CHILDREN_no
	.byte	73                              ## DW_AT_type
	.byte	19                              ## DW_FORM_ref4
	.byte	55                              ## DW_AT_count
	.byte	11                              ## DW_FORM_data1
	.byte	0                               ## EOM(1)
	.byte	0                               ## EOM(2)
	.byte	5                               ## Abbreviation Code
	.byte	36                              ## DW_TAG_base_type
	.byte	0                               ## DW_CHILDREN_no
	.byte	3                               ## DW_AT_name
	.byte	14                              ## DW_FORM_strp
	.byte	62                              ## DW_AT_encoding
	.byte	11                              ## DW_FORM_data1
	.byte	11                              ## DW_AT_byte_size
	.byte	11                              ## DW_FORM_data1
	.byte	0                               ## EOM(1)
	.byte	0                               ## EOM(2)
	.byte	6                               ## Abbreviation Code
	.byte	36                              ## DW_TAG_base_type
	.byte	0                               ## DW_CHILDREN_no
	.byte	3                               ## DW_AT_name
	.byte	14                              ## DW_FORM_strp
	.byte	11                              ## DW_AT_byte_size
	.byte	11                              ## DW_FORM_data1
	.byte	62                              ## DW_AT_encoding
	.byte	11                              ## DW_FORM_data1
	.byte	0                               ## EOM(1)
	.byte	0                               ## EOM(2)
	.byte	7                               ## Abbreviation Code
	.byte	52                              ## DW_TAG_variable
	.byte	0                               ## DW_CHILDREN_no
	.byte	3                               ## DW_AT_name
	.byte	14                              ## DW_FORM_strp
	.byte	73                              ## DW_AT_type
	.byte	19                              ## DW_FORM_ref4
	.byte	63                              ## DW_AT_external
	.byte	25                              ## DW_FORM_flag_present
	.byte	58                              ## DW_AT_decl_file
	.byte	11                              ## DW_FORM_data1
	.byte	59                              ## DW_AT_decl_line
	.byte	11                              ## DW_FORM_data1
	.byte	2                               ## DW_AT_location
	.byte	24                              ## DW_FORM_exprloc
	.byte	0                               ## EOM(1)
	.byte	0                               ## EOM(2)
	.byte	8                               ## Abbreviation Code
	.byte	46                              ## DW_TAG_subprogram
	.byte	1                               ## DW_CHILDREN_yes
	.byte	17                              ## DW_AT_low_pc
	.byte	1                               ## DW_FORM_addr
	.byte	18                              ## DW_AT_high_pc
	.byte	6                               ## DW_FORM_data4
	.byte	64                              ## DW_AT_frame_base
	.byte	24                              ## DW_FORM_exprloc
	.byte	3                               ## DW_AT_name
	.byte	14                              ## DW_FORM_strp
	.byte	58                              ## DW_AT_decl_file
	.byte	11                              ## DW_FORM_data1
	.byte	59                              ## DW_AT_decl_line
	.byte	11                              ## DW_FORM_data1
	.byte	39                              ## DW_AT_prototyped
	.byte	25                              ## DW_FORM_flag_present
	.byte	73                              ## DW_AT_type
	.byte	19                              ## DW_FORM_ref4
	.byte	63                              ## DW_AT_external
	.byte	25                              ## DW_FORM_flag_present
	.byte	0                               ## EOM(1)
	.byte	0                               ## EOM(2)
	.byte	9                               ## Abbreviation Code
	.byte	52                              ## DW_TAG_variable
	.byte	0                               ## DW_CHILDREN_no
	.byte	2                               ## DW_AT_location
	.byte	24                              ## DW_FORM_exprloc
	.byte	3                               ## DW_AT_name
	.byte	14                              ## DW_FORM_strp
	.byte	58                              ## DW_AT_decl_file
	.byte	11                              ## DW_FORM_data1
	.byte	59                              ## DW_AT_decl_line
	.byte	11                              ## DW_FORM_data1
	.byte	73                              ## DW_AT_type
	.byte	19                              ## DW_FORM_ref4
	.byte	0                               ## EOM(1)
	.byte	0                               ## EOM(2)
	.byte	10                              ## Abbreviation Code
	.byte	15                              ## DW_TAG_pointer_type
	.byte	0                               ## DW_CHILDREN_no
	.byte	73                              ## DW_AT_type
	.byte	19                              ## DW_FORM_ref4
	.byte	0                               ## EOM(1)
	.byte	0                               ## EOM(2)
	.byte	0                               ## EOM(3)
	.section	__DWARF,__debug_info,regular,debug
Lsection_info:
Lcu_begin0:
.set Lset0, Ldebug_info_end0-Ldebug_info_start0 ## Length of Unit
	.long	Lset0
Ldebug_info_start0:
	.short	4                               ## DWARF version number
.set Lset1, Lsection_abbrev-Lsection_abbrev ## Offset Into Abbrev. Section
	.long	Lset1
	.byte	4                               ## Address Size (in bytes)
	.byte	1                               ## Abbrev [1] 0xb:0xb0 DW_TAG_compile_unit
	.long	0                               ## DW_AT_producer
	.short	29                              ## DW_AT_language
	.long	47                              ## DW_AT_name
	.long	52                              ## DW_AT_LLVM_sysroot
	.long	104                             ## DW_AT_APPLE_sdk
.set Lset2, Lline_table_start0-Lsection_line ## DW_AT_stmt_list
	.long	Lset2
	.long	115                             ## DW_AT_comp_dir
	.long	Lfunc_begin0                    ## DW_AT_low_pc
.set Lset3, Lfunc_end0-Lfunc_begin0     ## DW_AT_high_pc
	.long	Lset3
	.byte	2                               ## Abbrev [2] 0x2e:0xd DW_TAG_variable
	.long	59                              ## DW_AT_type
	.byte	1                               ## DW_AT_decl_file
	.byte	23                              ## DW_AT_decl_line
	.byte	5                               ## DW_AT_location
	.byte	3
	.long	L_.str
	.byte	3                               ## Abbrev [3] 0x3b:0xc DW_TAG_array_type
	.long	71                              ## DW_AT_type
	.byte	4                               ## Abbrev [4] 0x40:0x6 DW_TAG_subrange_type
	.long	78                              ## DW_AT_type
	.byte	15                              ## DW_AT_count
	.byte	0                               ## End Of Children Mark
	.byte	5                               ## Abbrev [5] 0x47:0x7 DW_TAG_base_type
	.long	146                             ## DW_AT_name
	.byte	6                               ## DW_AT_encoding
	.byte	1                               ## DW_AT_byte_size
	.byte	6                               ## Abbrev [6] 0x4e:0x7 DW_TAG_base_type
	.long	151                             ## DW_AT_name
	.byte	8                               ## DW_AT_byte_size
	.byte	7                               ## DW_AT_encoding
	.byte	7                               ## Abbrev [7] 0x55:0x11 DW_TAG_variable
	.long	171                             ## DW_AT_name
	.long	102                             ## DW_AT_type
                                        ## DW_AT_external
	.byte	1                               ## DW_AT_decl_file
	.byte	3                               ## DW_AT_decl_line
	.byte	5                               ## DW_AT_location
	.byte	3
	.long	_cMas
	.byte	3                               ## Abbrev [3] 0x66:0xc DW_TAG_array_type
	.long	114                             ## DW_AT_type
	.byte	4                               ## Abbrev [4] 0x6b:0x6 DW_TAG_subrange_type
	.long	78                              ## DW_AT_type
	.byte	10                              ## DW_AT_count
	.byte	0                               ## End Of Children Mark
	.byte	5                               ## Abbrev [5] 0x72:0x7 DW_TAG_base_type
	.long	176                             ## DW_AT_name
	.byte	7                               ## DW_AT_encoding
	.byte	4                               ## DW_AT_byte_size
	.byte	7                               ## Abbrev [7] 0x79:0x11 DW_TAG_variable
	.long	189                             ## DW_AT_name
	.long	114                             ## DW_AT_type
                                        ## DW_AT_external
	.byte	1                               ## DW_AT_decl_file
	.byte	4                               ## DW_AT_decl_line
	.byte	5                               ## DW_AT_location
	.byte	3
	.long	_i
	.byte	5                               ## Abbrev [5] 0x8a:0x7 DW_TAG_base_type
	.long	191                             ## DW_AT_name
	.byte	5                               ## DW_AT_encoding
	.byte	4                               ## DW_AT_byte_size
	.byte	8                               ## Abbrev [8] 0x91:0x24 DW_TAG_subprogram
	.long	Lfunc_begin0                    ## DW_AT_low_pc
.set Lset4, Lfunc_end0-Lfunc_begin0     ## DW_AT_high_pc
	.long	Lset4
	.byte	1                               ## DW_AT_frame_base
	.byte	85
	.long	195                             ## DW_AT_name
	.byte	1                               ## DW_AT_decl_file
	.byte	6                               ## DW_AT_decl_line
                                        ## DW_AT_prototyped
	.long	138                             ## DW_AT_type
                                        ## DW_AT_external
	.byte	9                               ## Abbrev [9] 0xa6:0xe DW_TAG_variable
	.byte	2                               ## DW_AT_location
	.byte	145
	.byte	112
	.long	200                             ## DW_AT_name
	.byte	1                               ## DW_AT_decl_file
	.byte	7                               ## DW_AT_decl_line
	.long	181                             ## DW_AT_type
	.byte	0                               ## End Of Children Mark
	.byte	10                              ## Abbrev [10] 0xb5:0x5 DW_TAG_pointer_type
	.long	114                             ## DW_AT_type
	.byte	0                               ## End Of Children Mark
Ldebug_info_end0:
	.section	__DWARF,__debug_str,regular,debug
Linfo_string:
	.asciz	"Apple clang version 16.0.0 (clang-1600.0.26.6)" ## string offset=0
	.asciz	"01.c"                          ## string offset=47
	.asciz	"/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk" ## string offset=52
	.asciz	"MacOSX.sdk"                    ## string offset=104
	.asciz	"/Users/ivan/prog/sem3/cvm/lab6" ## string offset=115
	.asciz	"char"                          ## string offset=146
	.asciz	"__ARRAY_SIZE_TYPE__"           ## string offset=151
	.asciz	"cMas"                          ## string offset=171
	.asciz	"unsigned int"                  ## string offset=176
	.asciz	"i"                             ## string offset=189
	.asciz	"int"                           ## string offset=191
	.asciz	"main"                          ## string offset=195
	.asciz	"p"                             ## string offset=200
	.section	__DWARF,__apple_names,regular,debug
Lnames_begin:
	.long	1212240712                      ## Header Magic
	.short	1                               ## Header Version
	.short	0                               ## Header Hash Function
	.long	3                               ## Header Bucket Count
	.long	3                               ## Header Hash Count
	.long	12                              ## Header Data Length
	.long	0                               ## HeaderData Die Offset Base
	.long	1                               ## HeaderData Atom Count
	.short	1                               ## DW_ATOM_die_offset
	.short	6                               ## DW_FORM_data4
	.long	0                               ## Bucket 0
	.long	2                               ## Bucket 1
	.long	-1                              ## Bucket 2
	.long	177678                          ## Hash in Bucket 0
	.long	2090118537                      ## Hash in Bucket 0
	.long	2090499946                      ## Hash in Bucket 1
.set Lset5, LNames1-Lnames_begin        ## Offset in Bucket 0
	.long	Lset5
.set Lset6, LNames0-Lnames_begin        ## Offset in Bucket 0
	.long	Lset6
.set Lset7, LNames2-Lnames_begin        ## Offset in Bucket 1
	.long	Lset7
LNames1:
	.long	189                             ## i
	.long	1                               ## Num DIEs
	.long	121
	.long	0
LNames0:
	.long	171                             ## cMas
	.long	1                               ## Num DIEs
	.long	85
	.long	0
LNames2:
	.long	195                             ## main
	.long	1                               ## Num DIEs
	.long	145
	.long	0
	.section	__DWARF,__apple_objc,regular,debug
Lobjc_begin:
	.long	1212240712                      ## Header Magic
	.short	1                               ## Header Version
	.short	0                               ## Header Hash Function
	.long	1                               ## Header Bucket Count
	.long	0                               ## Header Hash Count
	.long	12                              ## Header Data Length
	.long	0                               ## HeaderData Die Offset Base
	.long	1                               ## HeaderData Atom Count
	.short	1                               ## DW_ATOM_die_offset
	.short	6                               ## DW_FORM_data4
	.long	-1                              ## Bucket 0
	.section	__DWARF,__apple_namespac,regular,debug
Lnamespac_begin:
	.long	1212240712                      ## Header Magic
	.short	1                               ## Header Version
	.short	0                               ## Header Hash Function
	.long	1                               ## Header Bucket Count
	.long	0                               ## Header Hash Count
	.long	12                              ## Header Data Length
	.long	0                               ## HeaderData Die Offset Base
	.long	1                               ## HeaderData Atom Count
	.short	1                               ## DW_ATOM_die_offset
	.short	6                               ## DW_FORM_data4
	.long	-1                              ## Bucket 0
	.section	__DWARF,__apple_types,regular,debug
Ltypes_begin:
	.long	1212240712                      ## Header Magic
	.short	1                               ## Header Version
	.short	0                               ## Header Hash Function
	.long	4                               ## Header Bucket Count
	.long	4                               ## Header Hash Count
	.long	20                              ## Header Data Length
	.long	0                               ## HeaderData Die Offset Base
	.long	3                               ## HeaderData Atom Count
	.short	1                               ## DW_ATOM_die_offset
	.short	6                               ## DW_FORM_data4
	.short	3                               ## DW_ATOM_die_tag
	.short	5                               ## DW_FORM_data2
	.short	4                               ## DW_ATOM_type_flags
	.short	11                              ## DW_FORM_data1
	.long	0                               ## Bucket 0
	.long	1                               ## Bucket 1
	.long	-1                              ## Bucket 2
	.long	2                               ## Bucket 3
	.long	193495088                       ## Hash in Bucket 0
	.long	-1304652851                     ## Hash in Bucket 1
	.long	2090147939                      ## Hash in Bucket 3
	.long	-594775205                      ## Hash in Bucket 3
.set Lset8, Ltypes3-Ltypes_begin        ## Offset in Bucket 0
	.long	Lset8
.set Lset9, Ltypes2-Ltypes_begin        ## Offset in Bucket 1
	.long	Lset9
.set Lset10, Ltypes0-Ltypes_begin       ## Offset in Bucket 3
	.long	Lset10
.set Lset11, Ltypes1-Ltypes_begin       ## Offset in Bucket 3
	.long	Lset11
Ltypes3:
	.long	191                             ## int
	.long	1                               ## Num DIEs
	.long	138
	.short	36
	.byte	0
	.long	0
Ltypes2:
	.long	176                             ## unsigned int
	.long	1                               ## Num DIEs
	.long	114
	.short	36
	.byte	0
	.long	0
Ltypes0:
	.long	146                             ## char
	.long	1                               ## Num DIEs
	.long	71
	.short	36
	.byte	0
	.long	0
Ltypes1:
	.long	151                             ## __ARRAY_SIZE_TYPE__
	.long	1                               ## Num DIEs
	.long	78
	.short	36
	.byte	0
	.long	0
	.section	__IMPORT,__pointers,non_lazy_symbol_pointers
L_cMas$non_lazy_ptr:
	.indirect_symbol	_cMas
	.long	0
L_i$non_lazy_ptr:
	.indirect_symbol	_i
	.long	0

.subsections_via_symbols
	.section	__DWARF,__debug_line,regular,debug
Lsection_line:
Lline_table_start0:
