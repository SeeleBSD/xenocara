/*
 * Copyright 2021 Alyssa Rosenzweig
 * SPDX-License-Identifier: MIT
 */

#ifndef _AGX_BUILDER_
#define _AGX_BUILDER_

#include "agx_compiler.h"

static inline agx_instr *
agx_alloc_instr(agx_builder *b, enum agx_opcode op, uint8_t nr_dests, uint8_t nr_srcs)
{
   size_t size = sizeof(agx_instr);
   size += sizeof(agx_index) * nr_dests;
   size += sizeof(agx_index) * nr_srcs;

   agx_instr *I = (agx_instr *) rzalloc_size(b->shader, size);
   I->dest = (agx_index *) (I + 1);
   I->src = I->dest + nr_dests;

   I->op = op;
   I->nr_dests = nr_dests;
   I->nr_srcs = nr_srcs;
   return I;
}



static inline agx_instr *
agx_floor_to(agx_builder *b


   , agx_index dst0


   , agx_index src0


) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_FLOOR, 1, 1);

   I->dest[0] = dst0;

   I->src[0] = src0;


   agx_builder_insert(&b->cursor, I);
   return I;
}

static inline agx_index
agx_floor(agx_builder *b


   , agx_index src0


) {

   agx_index tmp = agx_temp(b->shader, src0.size);
   agx_floor_to(b, tmp, src0);
   return tmp;
}



static inline agx_instr *
agx_srsqrt_to(agx_builder *b


   , agx_index dst0


   , agx_index src0


) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_SRSQRT, 1, 1);

   I->dest[0] = dst0;

   I->src[0] = src0;


   agx_builder_insert(&b->cursor, I);
   return I;
}

static inline agx_index
agx_srsqrt(agx_builder *b


   , agx_index src0


) {

   agx_index tmp = agx_temp(b->shader, src0.size);
   agx_srsqrt_to(b, tmp, src0);
   return tmp;
}



static inline agx_instr *
agx_dfdx_to(agx_builder *b


   , agx_index dst0


   , agx_index src0


) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_DFDX, 1, 1);

   I->dest[0] = dst0;

   I->src[0] = src0;


   agx_builder_insert(&b->cursor, I);
   return I;
}

static inline agx_index
agx_dfdx(agx_builder *b


   , agx_index src0


) {

   agx_index tmp = agx_temp(b->shader, src0.size);
   agx_dfdx_to(b, tmp, src0);
   return tmp;
}



static inline agx_instr *
agx_dfdy_to(agx_builder *b


   , agx_index dst0


   , agx_index src0


) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_DFDY, 1, 1);

   I->dest[0] = dst0;

   I->src[0] = src0;


   agx_builder_insert(&b->cursor, I);
   return I;
}

static inline agx_index
agx_dfdy(agx_builder *b


   , agx_index src0


) {

   agx_index tmp = agx_temp(b->shader, src0.size);
   agx_dfdy_to(b, tmp, src0);
   return tmp;
}



static inline agx_instr *
agx_rcp_to(agx_builder *b


   , agx_index dst0


   , agx_index src0


) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_RCP, 1, 1);

   I->dest[0] = dst0;

   I->src[0] = src0;


   agx_builder_insert(&b->cursor, I);
   return I;
}

static inline agx_index
agx_rcp(agx_builder *b


   , agx_index src0


) {

   agx_index tmp = agx_temp(b->shader, src0.size);
   agx_rcp_to(b, tmp, src0);
   return tmp;
}



static inline agx_instr *
agx_rsqrt_to(agx_builder *b


   , agx_index dst0


   , agx_index src0


) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_RSQRT, 1, 1);

   I->dest[0] = dst0;

   I->src[0] = src0;


   agx_builder_insert(&b->cursor, I);
   return I;
}

static inline agx_index
agx_rsqrt(agx_builder *b


   , agx_index src0


) {

   agx_index tmp = agx_temp(b->shader, src0.size);
   agx_rsqrt_to(b, tmp, src0);
   return tmp;
}



static inline agx_instr *
agx_sin_pt_1_to(agx_builder *b


   , agx_index dst0


   , agx_index src0


) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_SIN_PT_1, 1, 1);

   I->dest[0] = dst0;

   I->src[0] = src0;


   agx_builder_insert(&b->cursor, I);
   return I;
}

static inline agx_index
agx_sin_pt_1(agx_builder *b


   , agx_index src0


) {

   agx_index tmp = agx_temp(b->shader, src0.size);
   agx_sin_pt_1_to(b, tmp, src0);
   return tmp;
}



static inline agx_instr *
agx_log2_to(agx_builder *b


   , agx_index dst0


   , agx_index src0


) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_LOG2, 1, 1);

   I->dest[0] = dst0;

   I->src[0] = src0;


   agx_builder_insert(&b->cursor, I);
   return I;
}

static inline agx_index
agx_log2(agx_builder *b


   , agx_index src0


) {

   agx_index tmp = agx_temp(b->shader, src0.size);
   agx_log2_to(b, tmp, src0);
   return tmp;
}



static inline agx_instr *
agx_exp2_to(agx_builder *b


   , agx_index dst0


   , agx_index src0


) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_EXP2, 1, 1);

   I->dest[0] = dst0;

   I->src[0] = src0;


   agx_builder_insert(&b->cursor, I);
   return I;
}

static inline agx_index
agx_exp2(agx_builder *b


   , agx_index src0


) {

   agx_index tmp = agx_temp(b->shader, src0.size);
   agx_exp2_to(b, tmp, src0);
   return tmp;
}



static inline agx_instr *
agx_sin_pt_2_to(agx_builder *b


   , agx_index dst0


   , agx_index src0


) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_SIN_PT_2, 1, 1);

   I->dest[0] = dst0;

   I->src[0] = src0;


   agx_builder_insert(&b->cursor, I);
   return I;
}

static inline agx_index
agx_sin_pt_2(agx_builder *b


   , agx_index src0


) {

   agx_index tmp = agx_temp(b->shader, src0.size);
   agx_sin_pt_2_to(b, tmp, src0);
   return tmp;
}



static inline agx_instr *
agx_ceil_to(agx_builder *b


   , agx_index dst0


   , agx_index src0


) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_CEIL, 1, 1);

   I->dest[0] = dst0;

   I->src[0] = src0;


   agx_builder_insert(&b->cursor, I);
   return I;
}

static inline agx_index
agx_ceil(agx_builder *b


   , agx_index src0


) {

   agx_index tmp = agx_temp(b->shader, src0.size);
   agx_ceil_to(b, tmp, src0);
   return tmp;
}



static inline agx_instr *
agx_trunc_to(agx_builder *b


   , agx_index dst0


   , agx_index src0


) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_TRUNC, 1, 1);

   I->dest[0] = dst0;

   I->src[0] = src0;


   agx_builder_insert(&b->cursor, I);
   return I;
}

static inline agx_index
agx_trunc(agx_builder *b


   , agx_index src0


) {

   agx_index tmp = agx_temp(b->shader, src0.size);
   agx_trunc_to(b, tmp, src0);
   return tmp;
}



static inline agx_instr *
agx_roundeven_to(agx_builder *b


   , agx_index dst0


   , agx_index src0


) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_ROUNDEVEN, 1, 1);

   I->dest[0] = dst0;

   I->src[0] = src0;


   agx_builder_insert(&b->cursor, I);
   return I;
}

static inline agx_index
agx_roundeven(agx_builder *b


   , agx_index src0


) {

   agx_index tmp = agx_temp(b->shader, src0.size);
   agx_roundeven_to(b, tmp, src0);
   return tmp;
}



static inline agx_instr *
agx_bitrev_to(agx_builder *b


   , agx_index dst0


   , agx_index src0


) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_BITREV, 1, 1);

   I->dest[0] = dst0;

   I->src[0] = src0;


   agx_builder_insert(&b->cursor, I);
   return I;
}

static inline agx_index
agx_bitrev(agx_builder *b


   , agx_index src0


) {

   agx_index tmp = agx_temp(b->shader, src0.size);
   agx_bitrev_to(b, tmp, src0);
   return tmp;
}



static inline agx_instr *
agx_popcount_to(agx_builder *b


   , agx_index dst0


   , agx_index src0


) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_POPCOUNT, 1, 1);

   I->dest[0] = dst0;

   I->src[0] = src0;


   agx_builder_insert(&b->cursor, I);
   return I;
}

static inline agx_index
agx_popcount(agx_builder *b


   , agx_index src0


) {

   agx_index tmp = agx_temp(b->shader, src0.size);
   agx_popcount_to(b, tmp, src0);
   return tmp;
}



static inline agx_instr *
agx_ffs_to(agx_builder *b


   , agx_index dst0


   , agx_index src0


) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_FFS, 1, 1);

   I->dest[0] = dst0;

   I->src[0] = src0;


   agx_builder_insert(&b->cursor, I);
   return I;
}

static inline agx_index
agx_ffs(agx_builder *b


   , agx_index src0


) {

   agx_index tmp = agx_temp(b->shader, src0.size);
   agx_ffs_to(b, tmp, src0);
   return tmp;
}



static inline agx_instr *
agx_fadd_to(agx_builder *b


   , agx_index dst0


   , agx_index src0
   , agx_index src1


) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_FADD, 1, 2);

   I->dest[0] = dst0;

   I->src[0] = src0;
   I->src[1] = src1;


   agx_builder_insert(&b->cursor, I);
   return I;
}

static inline agx_index
agx_fadd(agx_builder *b


   , agx_index src0
   , agx_index src1


) {

   agx_index tmp = agx_temp(b->shader, src0.size);
   agx_fadd_to(b, tmp, src0, src1);
   return tmp;
}



static inline agx_instr *
agx_fma_to(agx_builder *b


   , agx_index dst0


   , agx_index src0
   , agx_index src1
   , agx_index src2


) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_FMA, 1, 3);

   I->dest[0] = dst0;

   I->src[0] = src0;
   I->src[1] = src1;
   I->src[2] = src2;


   agx_builder_insert(&b->cursor, I);
   return I;
}

static inline agx_index
agx_fma(agx_builder *b


   , agx_index src0
   , agx_index src1
   , agx_index src2


) {

   agx_index tmp = agx_temp(b->shader, src0.size);
   agx_fma_to(b, tmp, src0, src1, src2);
   return tmp;
}



static inline agx_instr *
agx_fmul_to(agx_builder *b


   , agx_index dst0


   , agx_index src0
   , agx_index src1


) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_FMUL, 1, 2);

   I->dest[0] = dst0;

   I->src[0] = src0;
   I->src[1] = src1;


   agx_builder_insert(&b->cursor, I);
   return I;
}

static inline agx_index
agx_fmul(agx_builder *b


   , agx_index src0
   , agx_index src1


) {

   agx_index tmp = agx_temp(b->shader, src0.size);
   agx_fmul_to(b, tmp, src0, src1);
   return tmp;
}



static inline agx_instr *
agx_mov_imm_to(agx_builder *b


   , agx_index dst0



   , uint64_t imm

) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_MOV_IMM, 1, 0);

   I->dest[0] = dst0;


   I->imm = imm;

   agx_builder_insert(&b->cursor, I);
   return I;
}

static inline agx_index
agx_mov_imm(agx_builder *b

   , unsigned size


   , uint64_t imm

) {

   agx_index tmp = agx_temp(b->shader, agx_size_for_bits(size));
   agx_mov_imm_to(b, tmp, imm);
   return tmp;
}



static inline agx_instr *
agx_iadd_to(agx_builder *b


   , agx_index dst0


   , agx_index src0
   , agx_index src1

   , uint32_t shift

) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_IADD, 1, 2);

   I->dest[0] = dst0;

   I->src[0] = src0;
   I->src[1] = src1;

   I->shift = shift;

   agx_builder_insert(&b->cursor, I);
   return I;
}

static inline agx_index
agx_iadd(agx_builder *b


   , agx_index src0
   , agx_index src1

   , uint32_t shift

) {

   agx_index tmp = agx_temp(b->shader, src0.size);
   agx_iadd_to(b, tmp, src0, src1, shift);
   return tmp;
}



static inline agx_instr *
agx_imad_to(agx_builder *b


   , agx_index dst0


   , agx_index src0
   , agx_index src1
   , agx_index src2

   , uint32_t shift

) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_IMAD, 1, 3);

   I->dest[0] = dst0;

   I->src[0] = src0;
   I->src[1] = src1;
   I->src[2] = src2;

   I->shift = shift;

   agx_builder_insert(&b->cursor, I);
   return I;
}

static inline agx_index
agx_imad(agx_builder *b


   , agx_index src0
   , agx_index src1
   , agx_index src2

   , uint32_t shift

) {

   agx_index tmp = agx_temp(b->shader, src0.size);
   agx_imad_to(b, tmp, src0, src1, src2, shift);
   return tmp;
}



static inline agx_instr *
agx_bfi_to(agx_builder *b


   , agx_index dst0


   , agx_index src0
   , agx_index src1
   , agx_index src2

   , uint32_t bfi_mask

) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_BFI, 1, 3);

   I->dest[0] = dst0;

   I->src[0] = src0;
   I->src[1] = src1;
   I->src[2] = src2;

   I->bfi_mask = bfi_mask;

   agx_builder_insert(&b->cursor, I);
   return I;
}

static inline agx_index
agx_bfi(agx_builder *b


   , agx_index src0
   , agx_index src1
   , agx_index src2

   , uint32_t bfi_mask

) {

   agx_index tmp = agx_temp(b->shader, src0.size);
   agx_bfi_to(b, tmp, src0, src1, src2, bfi_mask);
   return tmp;
}



static inline agx_instr *
agx_bfeil_to(agx_builder *b


   , agx_index dst0


   , agx_index src0
   , agx_index src1
   , agx_index src2

   , uint32_t bfi_mask

) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_BFEIL, 1, 3);

   I->dest[0] = dst0;

   I->src[0] = src0;
   I->src[1] = src1;
   I->src[2] = src2;

   I->bfi_mask = bfi_mask;

   agx_builder_insert(&b->cursor, I);
   return I;
}

static inline agx_index
agx_bfeil(agx_builder *b


   , agx_index src0
   , agx_index src1
   , agx_index src2

   , uint32_t bfi_mask

) {

   agx_index tmp = agx_temp(b->shader, src0.size);
   agx_bfeil_to(b, tmp, src0, src1, src2, bfi_mask);
   return tmp;
}



static inline agx_instr *
agx_extr_to(agx_builder *b


   , agx_index dst0


   , agx_index src0
   , agx_index src1
   , agx_index src2

   , uint32_t bfi_mask

) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_EXTR, 1, 3);

   I->dest[0] = dst0;

   I->src[0] = src0;
   I->src[1] = src1;
   I->src[2] = src2;

   I->bfi_mask = bfi_mask;

   agx_builder_insert(&b->cursor, I);
   return I;
}

static inline agx_index
agx_extr(agx_builder *b


   , agx_index src0
   , agx_index src1
   , agx_index src2

   , uint32_t bfi_mask

) {

   agx_index tmp = agx_temp(b->shader, src0.size);
   agx_extr_to(b, tmp, src0, src1, src2, bfi_mask);
   return tmp;
}



static inline agx_instr *
agx_asr_to(agx_builder *b


   , agx_index dst0


   , agx_index src0
   , agx_index src1


) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_ASR, 1, 2);

   I->dest[0] = dst0;

   I->src[0] = src0;
   I->src[1] = src1;


   agx_builder_insert(&b->cursor, I);
   return I;
}

static inline agx_index
agx_asr(agx_builder *b


   , agx_index src0
   , agx_index src1


) {

   agx_index tmp = agx_temp(b->shader, src0.size);
   agx_asr_to(b, tmp, src0, src1);
   return tmp;
}



static inline agx_instr *
agx_icmpsel_to(agx_builder *b


   , agx_index dst0


   , agx_index src0
   , agx_index src1
   , agx_index src2
   , agx_index src3

   , enum agx_icond icond

) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_ICMPSEL, 1, 4);

   I->dest[0] = dst0;

   I->src[0] = src0;
   I->src[1] = src1;
   I->src[2] = src2;
   I->src[3] = src3;

   I->icond = icond;

   agx_builder_insert(&b->cursor, I);
   return I;
}

static inline agx_index
agx_icmpsel(agx_builder *b


   , agx_index src0
   , agx_index src1
   , agx_index src2
   , agx_index src3

   , enum agx_icond icond

) {

   agx_index tmp = agx_temp(b->shader, src0.size);
   agx_icmpsel_to(b, tmp, src0, src1, src2, src3, icond);
   return tmp;
}



static inline agx_instr *
agx_fcmpsel_to(agx_builder *b


   , agx_index dst0


   , agx_index src0
   , agx_index src1
   , agx_index src2
   , agx_index src3

   , enum agx_fcond fcond

) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_FCMPSEL, 1, 4);

   I->dest[0] = dst0;

   I->src[0] = src0;
   I->src[1] = src1;
   I->src[2] = src2;
   I->src[3] = src3;

   I->fcond = fcond;

   agx_builder_insert(&b->cursor, I);
   return I;
}

static inline agx_index
agx_fcmpsel(agx_builder *b


   , agx_index src0
   , agx_index src1
   , agx_index src2
   , agx_index src3

   , enum agx_fcond fcond

) {

   agx_index tmp = agx_temp(b->shader, src0.size);
   agx_fcmpsel_to(b, tmp, src0, src1, src2, src3, fcond);
   return tmp;
}



static inline agx_instr *
agx_icmp_to(agx_builder *b


   , agx_index dst0


   , agx_index src0
   , agx_index src1

   , enum agx_icond icond
   , uint32_t invert_cond

) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_ICMP, 1, 2);

   I->dest[0] = dst0;

   I->src[0] = src0;
   I->src[1] = src1;

   I->icond = icond;
   I->invert_cond = invert_cond;

   agx_builder_insert(&b->cursor, I);
   return I;
}

static inline agx_index
agx_icmp(agx_builder *b


   , agx_index src0
   , agx_index src1

   , enum agx_icond icond
   , uint32_t invert_cond

) {

   agx_index tmp = agx_temp(b->shader, src0.size);
   agx_icmp_to(b, tmp, src0, src1, icond, invert_cond);
   return tmp;
}



static inline agx_instr *
agx_fcmp_to(agx_builder *b


   , agx_index dst0


   , agx_index src0
   , agx_index src1

   , enum agx_fcond fcond
   , uint32_t invert_cond

) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_FCMP, 1, 2);

   I->dest[0] = dst0;

   I->src[0] = src0;
   I->src[1] = src1;

   I->fcond = fcond;
   I->invert_cond = invert_cond;

   agx_builder_insert(&b->cursor, I);
   return I;
}

static inline agx_index
agx_fcmp(agx_builder *b


   , agx_index src0
   , agx_index src1

   , enum agx_fcond fcond
   , uint32_t invert_cond

) {

   agx_index tmp = agx_temp(b->shader, src0.size);
   agx_fcmp_to(b, tmp, src0, src1, fcond, invert_cond);
   return tmp;
}



static inline agx_instr *
agx_texture_sample_to(agx_builder *b


   , agx_index dst0


   , agx_index src0
   , agx_index src1
   , agx_index src2
   , agx_index src3
   , agx_index src4
   , agx_index src5

   , enum agx_dim dim
   , enum agx_lod_mode lod_mode
   , uint32_t mask
   , uint32_t scoreboard
   , bool offset
   , bool shadow
   , enum agx_gather gather

) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_TEXTURE_SAMPLE, 1, 6);

   I->dest[0] = dst0;

   I->src[0] = src0;
   I->src[1] = src1;
   I->src[2] = src2;
   I->src[3] = src3;
   I->src[4] = src4;
   I->src[5] = src5;

   I->dim = dim;
   I->lod_mode = lod_mode;
   I->mask = mask;
   I->scoreboard = scoreboard;
   I->offset = offset;
   I->shadow = shadow;
   I->gather = gather;

   agx_builder_insert(&b->cursor, I);
   return I;
}

static inline agx_index
agx_texture_sample(agx_builder *b


   , agx_index src0
   , agx_index src1
   , agx_index src2
   , agx_index src3
   , agx_index src4
   , agx_index src5

   , enum agx_dim dim
   , enum agx_lod_mode lod_mode
   , uint32_t mask
   , uint32_t scoreboard
   , bool offset
   , bool shadow
   , enum agx_gather gather

) {

   agx_index tmp = agx_temp(b->shader, src0.size);
   agx_texture_sample_to(b, tmp, src0, src1, src2, src3, src4, src5, dim, lod_mode, mask, scoreboard, offset, shadow, gather);
   return tmp;
}



static inline agx_instr *
agx_texture_load_to(agx_builder *b


   , agx_index dst0


   , agx_index src0
   , agx_index src1
   , agx_index src2
   , agx_index src3
   , agx_index src4
   , agx_index src5

   , enum agx_dim dim
   , enum agx_lod_mode lod_mode
   , uint32_t mask
   , uint32_t scoreboard
   , bool offset

) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_TEXTURE_LOAD, 1, 6);

   I->dest[0] = dst0;

   I->src[0] = src0;
   I->src[1] = src1;
   I->src[2] = src2;
   I->src[3] = src3;
   I->src[4] = src4;
   I->src[5] = src5;

   I->dim = dim;
   I->lod_mode = lod_mode;
   I->mask = mask;
   I->scoreboard = scoreboard;
   I->offset = offset;

   agx_builder_insert(&b->cursor, I);
   return I;
}

static inline agx_index
agx_texture_load(agx_builder *b


   , agx_index src0
   , agx_index src1
   , agx_index src2
   , agx_index src3
   , agx_index src4
   , agx_index src5

   , enum agx_dim dim
   , enum agx_lod_mode lod_mode
   , uint32_t mask
   , uint32_t scoreboard
   , bool offset

) {

   agx_index tmp = agx_temp(b->shader, src0.size);
   agx_texture_load_to(b, tmp, src0, src1, src2, src3, src4, src5, dim, lod_mode, mask, scoreboard, offset);
   return tmp;
}



static inline agx_instr *
agx_image_load_to(agx_builder *b


   , agx_index dst0


   , agx_index src0
   , agx_index src1
   , agx_index src2
   , agx_index src3
   , agx_index src4
   , agx_index src5

   , enum agx_dim dim
   , enum agx_lod_mode lod_mode
   , uint32_t mask
   , uint32_t scoreboard
   , bool offset

) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_IMAGE_LOAD, 1, 6);

   I->dest[0] = dst0;

   I->src[0] = src0;
   I->src[1] = src1;
   I->src[2] = src2;
   I->src[3] = src3;
   I->src[4] = src4;
   I->src[5] = src5;

   I->dim = dim;
   I->lod_mode = lod_mode;
   I->mask = mask;
   I->scoreboard = scoreboard;
   I->offset = offset;

   agx_builder_insert(&b->cursor, I);
   return I;
}

static inline agx_index
agx_image_load(agx_builder *b


   , agx_index src0
   , agx_index src1
   , agx_index src2
   , agx_index src3
   , agx_index src4
   , agx_index src5

   , enum agx_dim dim
   , enum agx_lod_mode lod_mode
   , uint32_t mask
   , uint32_t scoreboard
   , bool offset

) {

   agx_index tmp = agx_temp(b->shader, src0.size);
   agx_image_load_to(b, tmp, src0, src1, src2, src3, src4, src5, dim, lod_mode, mask, scoreboard, offset);
   return tmp;
}



static inline agx_instr *
agx_device_load_to(agx_builder *b


   , agx_index dst0


   , agx_index src0
   , agx_index src1

   , enum agx_format format
   , uint32_t mask
   , uint32_t shift
   , uint32_t scoreboard

) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_DEVICE_LOAD, 1, 2);

   I->dest[0] = dst0;

   I->src[0] = src0;
   I->src[1] = src1;

   I->format = format;
   I->mask = mask;
   I->shift = shift;
   I->scoreboard = scoreboard;

   agx_builder_insert(&b->cursor, I);
   return I;
}

static inline agx_index
agx_device_load(agx_builder *b


   , agx_index src0
   , agx_index src1

   , enum agx_format format
   , uint32_t mask
   , uint32_t shift
   , uint32_t scoreboard

) {

   agx_index tmp = agx_temp(b->shader, src0.size);
   agx_device_load_to(b, tmp, src0, src1, format, mask, shift, scoreboard);
   return tmp;
}



static inline agx_instr *
agx_local_load_to(agx_builder *b


   , agx_index dst0


   , agx_index src0
   , agx_index src1

   , enum agx_format format
   , uint32_t mask

) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_LOCAL_LOAD, 1, 2);

   I->dest[0] = dst0;

   I->src[0] = src0;
   I->src[1] = src1;

   I->format = format;
   I->mask = mask;

   agx_builder_insert(&b->cursor, I);
   return I;
}

static inline agx_index
agx_local_load(agx_builder *b


   , agx_index src0
   , agx_index src1

   , enum agx_format format
   , uint32_t mask

) {

   agx_index tmp = agx_temp(b->shader, src0.size);
   agx_local_load_to(b, tmp, src0, src1, format, mask);
   return tmp;
}



static inline agx_instr *
agx_device_store(agx_builder *b




   , agx_index src0
   , agx_index src1
   , agx_index src2

   , enum agx_format format
   , uint32_t mask
   , uint32_t shift
   , uint32_t scoreboard

) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_DEVICE_STORE, 0, 3);


   I->src[0] = src0;
   I->src[1] = src1;
   I->src[2] = src2;

   I->format = format;
   I->mask = mask;
   I->shift = shift;
   I->scoreboard = scoreboard;

   agx_builder_insert(&b->cursor, I);
   return I;
}




static inline agx_instr *
agx_local_store(agx_builder *b




   , agx_index src0
   , agx_index src1
   , agx_index src2

   , enum agx_format format
   , uint32_t mask

) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_LOCAL_STORE, 0, 3);


   I->src[0] = src0;
   I->src[1] = src1;
   I->src[2] = src2;

   I->format = format;
   I->mask = mask;

   agx_builder_insert(&b->cursor, I);
   return I;
}




static inline agx_instr *
agx_uniform_store(agx_builder *b




   , agx_index src0
   , agx_index src1


) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_UNIFORM_STORE, 0, 2);


   I->src[0] = src0;
   I->src[1] = src1;


   agx_builder_insert(&b->cursor, I);
   return I;
}




static inline agx_instr *
agx_atomic_to(agx_builder *b


   , agx_index dst0


   , agx_index src0
   , agx_index src1
   , agx_index src2

   , enum agx_atomic_opc atomic_opc
   , uint32_t scoreboard

) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_ATOMIC, 1, 3);

   I->dest[0] = dst0;

   I->src[0] = src0;
   I->src[1] = src1;
   I->src[2] = src2;

   I->atomic_opc = atomic_opc;
   I->scoreboard = scoreboard;

   agx_builder_insert(&b->cursor, I);
   return I;
}

static inline agx_index
agx_atomic(agx_builder *b


   , agx_index src0
   , agx_index src1
   , agx_index src2

   , enum agx_atomic_opc atomic_opc
   , uint32_t scoreboard

) {

   agx_index tmp = agx_temp(b->shader, src0.size);
   agx_atomic_to(b, tmp, src0, src1, src2, atomic_opc, scoreboard);
   return tmp;
}



static inline agx_instr *
agx_local_atomic_to(agx_builder *b


   , agx_index dst0


   , agx_index src0
   , agx_index src1
   , agx_index src2

   , enum agx_atomic_opc atomic_opc

) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_LOCAL_ATOMIC, 1, 3);

   I->dest[0] = dst0;

   I->src[0] = src0;
   I->src[1] = src1;
   I->src[2] = src2;

   I->atomic_opc = atomic_opc;

   agx_builder_insert(&b->cursor, I);
   return I;
}

static inline agx_index
agx_local_atomic(agx_builder *b


   , agx_index src0
   , agx_index src1
   , agx_index src2

   , enum agx_atomic_opc atomic_opc

) {

   agx_index tmp = agx_temp(b->shader, src0.size);
   agx_local_atomic_to(b, tmp, src0, src1, src2, atomic_opc);
   return tmp;
}



static inline agx_instr *
agx_wait(agx_builder *b





   , uint32_t scoreboard

) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_WAIT, 0, 0);



   I->scoreboard = scoreboard;

   agx_builder_insert(&b->cursor, I);
   return I;
}




static inline agx_instr *
agx_get_sr_to(agx_builder *b


   , agx_index dst0



   , enum agx_sr sr

) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_GET_SR, 1, 0);

   I->dest[0] = dst0;


   I->sr = sr;

   agx_builder_insert(&b->cursor, I);
   return I;
}

static inline agx_index
agx_get_sr(agx_builder *b

   , unsigned size


   , enum agx_sr sr

) {

   agx_index tmp = agx_temp(b->shader, agx_size_for_bits(size));
   agx_get_sr_to(b, tmp, sr);
   return tmp;
}



static inline agx_instr *
agx_get_sr_coverage_to(agx_builder *b


   , agx_index dst0



   , enum agx_sr sr

) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_GET_SR_COVERAGE, 1, 0);

   I->dest[0] = dst0;


   I->sr = sr;

   agx_builder_insert(&b->cursor, I);
   return I;
}

static inline agx_index
agx_get_sr_coverage(agx_builder *b

   , unsigned size


   , enum agx_sr sr

) {

   agx_index tmp = agx_temp(b->shader, agx_size_for_bits(size));
   agx_get_sr_coverage_to(b, tmp, sr);
   return tmp;
}



static inline agx_instr *
agx_sample_mask(agx_builder *b




   , agx_index src0
   , agx_index src1


) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_SAMPLE_MASK, 0, 2);


   I->src[0] = src0;
   I->src[1] = src1;


   agx_builder_insert(&b->cursor, I);
   return I;
}




static inline agx_instr *
agx_zs_emit(agx_builder *b




   , agx_index src0
   , agx_index src1

   , uint32_t zs

) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_ZS_EMIT, 0, 2);


   I->src[0] = src0;
   I->src[1] = src1;

   I->zs = zs;

   agx_builder_insert(&b->cursor, I);
   return I;
}




static inline agx_instr *
agx_ld_tile_to(agx_builder *b


   , agx_index dst0


   , agx_index src0

   , enum agx_format format
   , uint32_t mask
   , uint32_t pixel_offset

) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_LD_TILE, 1, 1);

   I->dest[0] = dst0;

   I->src[0] = src0;

   I->format = format;
   I->mask = mask;
   I->pixel_offset = pixel_offset;

   agx_builder_insert(&b->cursor, I);
   return I;
}

static inline agx_index
agx_ld_tile(agx_builder *b


   , agx_index src0

   , enum agx_format format
   , uint32_t mask
   , uint32_t pixel_offset

) {

   agx_index tmp = agx_temp(b->shader, src0.size);
   agx_ld_tile_to(b, tmp, src0, format, mask, pixel_offset);
   return tmp;
}



static inline agx_instr *
agx_st_tile(agx_builder *b




   , agx_index src0
   , agx_index src1

   , enum agx_format format
   , uint32_t mask
   , uint32_t pixel_offset

) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_ST_TILE, 0, 2);


   I->src[0] = src0;
   I->src[1] = src1;

   I->format = format;
   I->mask = mask;
   I->pixel_offset = pixel_offset;

   agx_builder_insert(&b->cursor, I);
   return I;
}




static inline agx_instr *
agx_jmp_exec_any(agx_builder *b





   , agx_block * target

) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_JMP_EXEC_ANY, 0, 0);



   I->target = target;

   agx_builder_insert(&b->cursor, I);
   return I;
}




static inline agx_instr *
agx_jmp_exec_none(agx_builder *b





   , agx_block * target

) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_JMP_EXEC_NONE, 0, 0);



   I->target = target;

   agx_builder_insert(&b->cursor, I);
   return I;
}




static inline agx_instr *
agx_jmp_exec_none_after(agx_builder *b





   , agx_block * target

) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_JMP_EXEC_NONE_AFTER, 0, 0);



   I->target = target;

   agx_builder_insert(&b->cursor, I);
   return I;
}




static inline agx_instr *
agx_pop_exec(agx_builder *b





   , uint32_t nest

) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_POP_EXEC, 0, 0);



   I->nest = nest;

   agx_builder_insert(&b->cursor, I);
   return I;
}




static inline agx_instr *
agx_if_icmp(agx_builder *b




   , agx_index src0
   , agx_index src1

   , uint32_t nest
   , enum agx_icond icond
   , uint32_t invert_cond
   , agx_block * target

) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_IF_ICMP, 0, 2);


   I->src[0] = src0;
   I->src[1] = src1;

   I->nest = nest;
   I->icond = icond;
   I->invert_cond = invert_cond;
   I->target = target;

   agx_builder_insert(&b->cursor, I);
   return I;
}




static inline agx_instr *
agx_else_icmp(agx_builder *b




   , agx_index src0
   , agx_index src1

   , uint32_t nest
   , enum agx_icond icond
   , uint32_t invert_cond
   , agx_block * target

) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_ELSE_ICMP, 0, 2);


   I->src[0] = src0;
   I->src[1] = src1;

   I->nest = nest;
   I->icond = icond;
   I->invert_cond = invert_cond;
   I->target = target;

   agx_builder_insert(&b->cursor, I);
   return I;
}




static inline agx_instr *
agx_while_icmp(agx_builder *b




   , agx_index src0
   , agx_index src1

   , uint32_t nest
   , enum agx_icond icond
   , uint32_t invert_cond
   , agx_block * target

) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_WHILE_ICMP, 0, 2);


   I->src[0] = src0;
   I->src[1] = src1;

   I->nest = nest;
   I->icond = icond;
   I->invert_cond = invert_cond;
   I->target = target;

   agx_builder_insert(&b->cursor, I);
   return I;
}




static inline agx_instr *
agx_if_fcmp(agx_builder *b




   , agx_index src0
   , agx_index src1

   , uint32_t nest
   , enum agx_fcond fcond
   , uint32_t invert_cond
   , agx_block * target

) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_IF_FCMP, 0, 2);


   I->src[0] = src0;
   I->src[1] = src1;

   I->nest = nest;
   I->fcond = fcond;
   I->invert_cond = invert_cond;
   I->target = target;

   agx_builder_insert(&b->cursor, I);
   return I;
}




static inline agx_instr *
agx_else_fcmp(agx_builder *b




   , agx_index src0
   , agx_index src1

   , uint32_t nest
   , enum agx_fcond fcond
   , uint32_t invert_cond
   , agx_block * target

) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_ELSE_FCMP, 0, 2);


   I->src[0] = src0;
   I->src[1] = src1;

   I->nest = nest;
   I->fcond = fcond;
   I->invert_cond = invert_cond;
   I->target = target;

   agx_builder_insert(&b->cursor, I);
   return I;
}




static inline agx_instr *
agx_while_fcmp(agx_builder *b




   , agx_index src0
   , agx_index src1

   , uint32_t nest
   , enum agx_fcond fcond
   , uint32_t invert_cond
   , agx_block * target

) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_WHILE_FCMP, 0, 2);


   I->src[0] = src0;
   I->src[1] = src1;

   I->nest = nest;
   I->fcond = fcond;
   I->invert_cond = invert_cond;
   I->target = target;

   agx_builder_insert(&b->cursor, I);
   return I;
}




static inline agx_instr *
agx_bitop_to(agx_builder *b


   , agx_index dst0


   , agx_index src0
   , agx_index src1

   , uint32_t truth_table

) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_BITOP, 1, 2);

   I->dest[0] = dst0;

   I->src[0] = src0;
   I->src[1] = src1;

   I->truth_table = truth_table;

   agx_builder_insert(&b->cursor, I);
   return I;
}

static inline agx_index
agx_bitop(agx_builder *b


   , agx_index src0
   , agx_index src1

   , uint32_t truth_table

) {

   agx_index tmp = agx_temp(b->shader, src0.size);
   agx_bitop_to(b, tmp, src0, src1, truth_table);
   return tmp;
}



static inline agx_instr *
agx_intl_to(agx_builder *b


   , agx_index dst0


   , agx_index src0
   , agx_index src1


) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_INTL, 1, 2);

   I->dest[0] = dst0;

   I->src[0] = src0;
   I->src[1] = src1;


   agx_builder_insert(&b->cursor, I);
   return I;
}

static inline agx_index
agx_intl(agx_builder *b


   , agx_index src0
   , agx_index src1


) {

   agx_index tmp = agx_temp(b->shader, src0.size);
   agx_intl_to(b, tmp, src0, src1);
   return tmp;
}



static inline agx_instr *
agx_convert_to(agx_builder *b


   , agx_index dst0


   , agx_index src0
   , agx_index src1

   , enum agx_round round

) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_CONVERT, 1, 2);

   I->dest[0] = dst0;

   I->src[0] = src0;
   I->src[1] = src1;

   I->round = round;

   agx_builder_insert(&b->cursor, I);
   return I;
}

static inline agx_index
agx_convert(agx_builder *b


   , agx_index src0
   , agx_index src1

   , enum agx_round round

) {

   agx_index tmp = agx_temp(b->shader, src0.size);
   agx_convert_to(b, tmp, src0, src1, round);
   return tmp;
}



static inline agx_instr *
agx_iter_to(agx_builder *b


   , agx_index dst0


   , agx_index src0
   , agx_index src1

   , uint32_t channels
   , enum agx_interpolation interpolation

) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_ITER, 1, 2);

   I->dest[0] = dst0;

   I->src[0] = src0;
   I->src[1] = src1;

   I->channels = channels;
   I->interpolation = interpolation;

   agx_builder_insert(&b->cursor, I);
   return I;
}

static inline agx_index
agx_iter(agx_builder *b


   , agx_index src0
   , agx_index src1

   , uint32_t channels
   , enum agx_interpolation interpolation

) {

   agx_index tmp = agx_temp(b->shader, src0.size);
   agx_iter_to(b, tmp, src0, src1, channels, interpolation);
   return tmp;
}



static inline agx_instr *
agx_iterproj_to(agx_builder *b


   , agx_index dst0


   , agx_index src0
   , agx_index src1
   , agx_index src2

   , uint32_t channels
   , enum agx_interpolation interpolation

) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_ITERPROJ, 1, 3);

   I->dest[0] = dst0;

   I->src[0] = src0;
   I->src[1] = src1;
   I->src[2] = src2;

   I->channels = channels;
   I->interpolation = interpolation;

   agx_builder_insert(&b->cursor, I);
   return I;
}

static inline agx_index
agx_iterproj(agx_builder *b


   , agx_index src0
   , agx_index src1
   , agx_index src2

   , uint32_t channels
   , enum agx_interpolation interpolation

) {

   agx_index tmp = agx_temp(b->shader, src0.size);
   agx_iterproj_to(b, tmp, src0, src1, src2, channels, interpolation);
   return tmp;
}



static inline agx_instr *
agx_ldcf_to(agx_builder *b


   , agx_index dst0


   , agx_index src0

   , uint32_t channels

) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_LDCF, 1, 1);

   I->dest[0] = dst0;

   I->src[0] = src0;

   I->channels = channels;

   agx_builder_insert(&b->cursor, I);
   return I;
}

static inline agx_index
agx_ldcf(agx_builder *b


   , agx_index src0

   , uint32_t channels

) {

   agx_index tmp = agx_temp(b->shader, src0.size);
   agx_ldcf_to(b, tmp, src0, channels);
   return tmp;
}



static inline agx_instr *
agx_st_vary(agx_builder *b




   , agx_index src0
   , agx_index src1


) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_ST_VARY, 0, 2);


   I->src[0] = src0;
   I->src[1] = src1;


   agx_builder_insert(&b->cursor, I);
   return I;
}




static inline agx_instr *
agx_no_varyings(agx_builder *b






) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_NO_VARYINGS, 0, 0);




   agx_builder_insert(&b->cursor, I);
   return I;
}




static inline agx_instr *
agx_stop(agx_builder *b






) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_STOP, 0, 0);




   agx_builder_insert(&b->cursor, I);
   return I;
}




static inline agx_instr *
agx_trap(agx_builder *b






) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_TRAP, 0, 0);




   agx_builder_insert(&b->cursor, I);
   return I;
}




static inline agx_instr *
agx_wait_pix(agx_builder *b





   , uint32_t writeout

) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_WAIT_PIX, 0, 0);



   I->writeout = writeout;

   agx_builder_insert(&b->cursor, I);
   return I;
}




static inline agx_instr *
agx_signal_pix(agx_builder *b





   , uint32_t writeout

) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_SIGNAL_PIX, 0, 0);



   I->writeout = writeout;

   agx_builder_insert(&b->cursor, I);
   return I;
}




static inline agx_instr *
agx_image_write(agx_builder *b




   , agx_index src0
   , agx_index src1
   , agx_index src2
   , agx_index src3
   , agx_index src4

   , enum agx_dim dim

) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_IMAGE_WRITE, 0, 5);


   I->src[0] = src0;
   I->src[1] = src1;
   I->src[2] = src2;
   I->src[3] = src3;
   I->src[4] = src4;

   I->dim = dim;

   agx_builder_insert(&b->cursor, I);
   return I;
}




static inline agx_instr *
agx_block_image_store(agx_builder *b




   , agx_index src0
   , agx_index src1
   , agx_index src2

   , enum agx_format format
   , enum agx_dim dim

) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_BLOCK_IMAGE_STORE, 0, 3);


   I->src[0] = src0;
   I->src[1] = src1;
   I->src[2] = src2;

   I->format = format;
   I->dim = dim;

   agx_builder_insert(&b->cursor, I);
   return I;
}




static inline agx_instr *
agx_threadgroup_barrier(agx_builder *b






) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_THREADGROUP_BARRIER, 0, 0);




   agx_builder_insert(&b->cursor, I);
   return I;
}




static inline agx_instr *
agx_memory_barrier(agx_builder *b






) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_MEMORY_BARRIER, 0, 0);




   agx_builder_insert(&b->cursor, I);
   return I;
}




static inline agx_instr *
agx_image_barrier_1(agx_builder *b






) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_IMAGE_BARRIER_1, 0, 0);




   agx_builder_insert(&b->cursor, I);
   return I;
}




static inline agx_instr *
agx_image_barrier_2(agx_builder *b






) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_IMAGE_BARRIER_2, 0, 0);




   agx_builder_insert(&b->cursor, I);
   return I;
}




static inline agx_instr *
agx_image_barrier_3(agx_builder *b






) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_IMAGE_BARRIER_3, 0, 0);




   agx_builder_insert(&b->cursor, I);
   return I;
}




static inline agx_instr *
agx_image_barrier_4(agx_builder *b






) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_IMAGE_BARRIER_4, 0, 0);




   agx_builder_insert(&b->cursor, I);
   return I;
}




static inline agx_instr *
agx_flush_memory_to_texture(agx_builder *b






) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_FLUSH_MEMORY_TO_TEXTURE, 0, 0);




   agx_builder_insert(&b->cursor, I);
   return I;
}




static inline agx_instr *
agx_mov_to(agx_builder *b


   , agx_index dst0


   , agx_index src0


) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_MOV, 1, 1);

   I->dest[0] = dst0;

   I->src[0] = src0;


   agx_builder_insert(&b->cursor, I);
   return I;
}

static inline agx_index
agx_mov(agx_builder *b


   , agx_index src0


) {

   agx_index tmp = agx_temp(b->shader, src0.size);
   agx_mov_to(b, tmp, src0);
   return tmp;
}



static inline agx_instr *
agx_not_to(agx_builder *b


   , agx_index dst0


   , agx_index src0


) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_NOT, 1, 1);

   I->dest[0] = dst0;

   I->src[0] = src0;


   agx_builder_insert(&b->cursor, I);
   return I;
}

static inline agx_index
agx_not(agx_builder *b


   , agx_index src0


) {

   agx_index tmp = agx_temp(b->shader, src0.size);
   agx_not_to(b, tmp, src0);
   return tmp;
}



static inline agx_instr *
agx_xor_to(agx_builder *b


   , agx_index dst0


   , agx_index src0
   , agx_index src1


) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_XOR, 1, 2);

   I->dest[0] = dst0;

   I->src[0] = src0;
   I->src[1] = src1;


   agx_builder_insert(&b->cursor, I);
   return I;
}

static inline agx_index
agx_xor(agx_builder *b


   , agx_index src0
   , agx_index src1


) {

   agx_index tmp = agx_temp(b->shader, src0.size);
   agx_xor_to(b, tmp, src0, src1);
   return tmp;
}



static inline agx_instr *
agx_and_to(agx_builder *b


   , agx_index dst0


   , agx_index src0
   , agx_index src1


) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_AND, 1, 2);

   I->dest[0] = dst0;

   I->src[0] = src0;
   I->src[1] = src1;


   agx_builder_insert(&b->cursor, I);
   return I;
}

static inline agx_index
agx_and(agx_builder *b


   , agx_index src0
   , agx_index src1


) {

   agx_index tmp = agx_temp(b->shader, src0.size);
   agx_and_to(b, tmp, src0, src1);
   return tmp;
}



static inline agx_instr *
agx_or_to(agx_builder *b


   , agx_index dst0


   , agx_index src0
   , agx_index src1


) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_OR, 1, 2);

   I->dest[0] = dst0;

   I->src[0] = src0;
   I->src[1] = src1;


   agx_builder_insert(&b->cursor, I);
   return I;
}

static inline agx_index
agx_or(agx_builder *b


   , agx_index src0
   , agx_index src1


) {

   agx_index tmp = agx_temp(b->shader, src0.size);
   agx_or_to(b, tmp, src0, src1);
   return tmp;
}



static inline agx_instr *
agx_collect_to(agx_builder *b


   , agx_index dst0

   , unsigned nr_srcs



) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_COLLECT, 1, nr_srcs);

   I->dest[0] = dst0;



   agx_builder_insert(&b->cursor, I);
   return I;
}




static inline agx_instr *
agx_split(agx_builder *b

   , unsigned nr_dests



   , agx_index src0


) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_SPLIT, nr_dests, 1);


   I->src[0] = src0;


   agx_builder_insert(&b->cursor, I);
   return I;
}




static inline agx_instr *
agx_phi_to(agx_builder *b


   , agx_index dst0

   , unsigned nr_srcs



) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_PHI, 1, nr_srcs);

   I->dest[0] = dst0;



   agx_builder_insert(&b->cursor, I);
   return I;
}




static inline agx_instr *
agx_unit_test(agx_builder *b




   , agx_index src0


) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_UNIT_TEST, 0, 1);


   I->src[0] = src0;


   agx_builder_insert(&b->cursor, I);
   return I;
}




static inline agx_instr *
agx_preload_to(agx_builder *b


   , agx_index dst0


   , agx_index src0


) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_PRELOAD, 1, 1);

   I->dest[0] = dst0;

   I->src[0] = src0;


   agx_builder_insert(&b->cursor, I);
   return I;
}

static inline agx_index
agx_preload(agx_builder *b


   , agx_index src0


) {

   agx_index tmp = agx_temp(b->shader, src0.size);
   agx_preload_to(b, tmp, src0);
   return tmp;
}



static inline agx_instr *
agx_begin_cf(agx_builder *b






) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_BEGIN_CF, 0, 0);




   agx_builder_insert(&b->cursor, I);
   return I;
}




static inline agx_instr *
agx_break(agx_builder *b





   , uint32_t nest
   , agx_block * target

) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_BREAK, 0, 0);



   I->nest = nest;
   I->target = target;

   agx_builder_insert(&b->cursor, I);
   return I;
}




static inline agx_instr *
agx_break_if_icmp(agx_builder *b




   , agx_index src0
   , agx_index src1

   , uint32_t nest
   , uint32_t invert_cond
   , enum agx_icond icond
   , agx_block * target

) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_BREAK_IF_ICMP, 0, 2);


   I->src[0] = src0;
   I->src[1] = src1;

   I->nest = nest;
   I->invert_cond = invert_cond;
   I->icond = icond;
   I->target = target;

   agx_builder_insert(&b->cursor, I);
   return I;
}




static inline agx_instr *
agx_break_if_fcmp(agx_builder *b




   , agx_index src0
   , agx_index src1

   , uint32_t nest
   , uint32_t invert_cond
   , enum agx_fcond fcond
   , agx_block * target

) {
   agx_instr *I = agx_alloc_instr(b, AGX_OPCODE_BREAK_IF_FCMP, 0, 2);


   I->src[0] = src0;
   I->src[1] = src1;

   I->nest = nest;
   I->invert_cond = invert_cond;
   I->fcond = fcond;
   I->target = target;

   agx_builder_insert(&b->cursor, I);
   return I;
}



/* Convenience methods */

enum agx_bitop_table {
   AGX_BITOP_NOT = 0x5,
   AGX_BITOP_XOR = 0x6,
   AGX_BITOP_AND = 0x8,
   AGX_BITOP_MOV = 0xA,
   AGX_BITOP_OR  = 0xE
};

static inline agx_instr *
agx_fmov_to(agx_builder *b, agx_index dst0, agx_index src0)
{
   return agx_fadd_to(b, dst0, src0, agx_negzero());
}

static inline agx_instr *
agx_push_exec(agx_builder *b, unsigned n)
{
   return agx_if_fcmp(b, agx_zero(), agx_zero(), n, AGX_FCOND_EQ, false, NULL);
}

static inline agx_instr *
agx_ushr_to(agx_builder *b, agx_index dst, agx_index s0, agx_index s1)
{
    return agx_bfeil_to(b, dst, agx_zero(), s0, s1, 0);
}

static inline agx_index
agx_ushr(agx_builder *b, agx_index s0, agx_index s1)
{
    agx_index tmp = agx_temp(b->shader, s0.size);
    agx_ushr_to(b, tmp, s0, s1);
    return tmp;
}

#endif

