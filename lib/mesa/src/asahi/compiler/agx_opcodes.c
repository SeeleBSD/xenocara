/*
 * Copyright 2021 Alyssa Rosenzweig
 * SPDX-License-Identifier: MIT
 */

#include "agx_opcodes.h"



const struct agx_opcode_info agx_opcodes_info[AGX_NUM_OPCODES] = {

   [AGX_OPCODE_FLOOR] = {
      "floor", 1, 1, 0,
      { 0x800a, 6, 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_NONE,
      1,
      1,
      1,
   },

   [AGX_OPCODE_SRSQRT] = {
      "srsqrt", 1, 1, 0,
      { 0x1000800a, 6, 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_NONE,
      1,
      1,
      1,
   },

   [AGX_OPCODE_DFDX] = {
      "dfdx", 1, 1, 0,
      { 0x4000800a, 6, 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_COVERAGE,
      1,
      1,
      1,
   },

   [AGX_OPCODE_DFDY] = {
      "dfdy", 1, 1, 0,
      { 0x6000800a, 6, 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_COVERAGE,
      1,
      1,
      1,
   },

   [AGX_OPCODE_RCP] = {
      "rcp", 1, 1, 0,
      { 0x8000800a, 6, 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_NONE,
      1,
      1,
      1,
   },

   [AGX_OPCODE_RSQRT] = {
      "rsqrt", 1, 1, 0,
      { 0x9000800a, 6, 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_NONE,
      1,
      1,
      1,
   },

   [AGX_OPCODE_SIN_PT_1] = {
      "sin_pt_1", 1, 1, 0,
      { 0xa000800a, 6, 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_NONE,
      1,
      1,
      1,
   },

   [AGX_OPCODE_LOG2] = {
      "log2", 1, 1, 0,
      { 0xc000800a, 6, 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_NONE,
      1,
      1,
      1,
   },

   [AGX_OPCODE_EXP2] = {
      "exp2", 1, 1, 0,
      { 0xd000800a, 6, 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_NONE,
      1,
      1,
      1,
   },

   [AGX_OPCODE_SIN_PT_2] = {
      "sin_pt_2", 1, 1, 0,
      { 0xe000800a, 6, 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_NONE,
      1,
      1,
      1,
   },

   [AGX_OPCODE_CEIL] = {
      "ceil", 1, 1, 0,
      { 0x10000800a, 6, 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_NONE,
      1,
      1,
      1,
   },

   [AGX_OPCODE_TRUNC] = {
      "trunc", 1, 1, 0,
      { 0x20000800a, 6, 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_NONE,
      1,
      1,
      1,
   },

   [AGX_OPCODE_ROUNDEVEN] = {
      "roundeven", 1, 1, 0,
      { 0x30000800a, 6, 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_NONE,
      1,
      1,
      1,
   },

   [AGX_OPCODE_BITREV] = {
      "bitrev", 1, 1, 0,
      { 0x400003e, 6, 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_NONE,
      0,
      1,
      1,
   },

   [AGX_OPCODE_POPCOUNT] = {
      "popcount", 1, 1, 0,
      { 0x800003e, 6, 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_NONE,
      0,
      1,
      1,
   },

   [AGX_OPCODE_FFS] = {
      "ffs", 1, 1, 0,
      { 0xc00003e, 6, 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_NONE,
      0,
      1,
      1,
   },

   [AGX_OPCODE_FADD] = {
      "fadd", 2, 1, 0,
      { 0x2a, 4, 1 },
      { 0x26, 4, 1 },
      AGX_SCHEDULE_CLASS_NONE,
      1,
      1,
      1,
   },

   [AGX_OPCODE_FMA] = {
      "fma", 3, 1, 0,
      { 0x3a, 6, 1 },
      { 0x36, 6, 1 },
      AGX_SCHEDULE_CLASS_NONE,
      1,
      1,
      1,
   },

   [AGX_OPCODE_FMUL] = {
      "fmul", 2, 1, 0,
      { 0x1a, 4, 1 },
      { 0x16, 4, 1 },
      AGX_SCHEDULE_CLASS_NONE,
      1,
      1,
      1,
   },

   [AGX_OPCODE_MOV_IMM] = {
      "mov_imm", 0, 1, AGX_IMMEDIATE_IMM,
      { 0x62, 6, 1 },
      { 0x62, 4, 1 },
      AGX_SCHEDULE_CLASS_NONE,
      0,
      1,
      1,
   },

   [AGX_OPCODE_IADD] = {
      "iadd", 2, 1, AGX_IMMEDIATE_SHIFT,
      { 0xe, 8, 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_NONE,
      0,
      1,
      1,
   },

   [AGX_OPCODE_IMAD] = {
      "imad", 3, 1, AGX_IMMEDIATE_SHIFT,
      { 0x1e, 8, 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_NONE,
      0,
      1,
      1,
   },

   [AGX_OPCODE_BFI] = {
      "bfi", 3, 1, AGX_IMMEDIATE_BFI_MASK,
      { 0x2e, 8, 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_NONE,
      0,
      1,
      1,
   },

   [AGX_OPCODE_BFEIL] = {
      "bfeil", 3, 1, AGX_IMMEDIATE_BFI_MASK,
      { 0x802e, 8, 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_NONE,
      0,
      1,
      1,
   },

   [AGX_OPCODE_EXTR] = {
      "extr", 3, 1, AGX_IMMEDIATE_BFI_MASK,
      { 0x400002e, 8, 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_NONE,
      0,
      1,
      1,
   },

   [AGX_OPCODE_ASR] = {
      "asr", 2, 1, 0,
      { 0x400802e, 8, 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_NONE,
      0,
      1,
      1,
   },

   [AGX_OPCODE_ICMPSEL] = {
      "icmpsel", 4, 1, AGX_IMMEDIATE_ICOND,
      { 0x12, 8, 1 },
      { 0 },
      AGX_SCHEDULE_CLASS_NONE,
      0,
      1,
      1,
   },

   [AGX_OPCODE_FCMPSEL] = {
      "fcmpsel", 4, 1, AGX_IMMEDIATE_FCOND,
      { 0x2, 8, 1 },
      { 0 },
      AGX_SCHEDULE_CLASS_NONE,
      0,
      1,
      1,
   },

   [AGX_OPCODE_ICMP] = {
      "icmp", 2, 1, AGX_IMMEDIATE_ICOND | AGX_IMMEDIATE_INVERT_COND,
      { 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_NONE,
      0,
      1,
      1,
   },

   [AGX_OPCODE_FCMP] = {
      "fcmp", 2, 1, AGX_IMMEDIATE_FCOND | AGX_IMMEDIATE_INVERT_COND,
      { 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_NONE,
      0,
      1,
      1,
   },

   [AGX_OPCODE_TEXTURE_SAMPLE] = {
      "texture_sample", 6, 1, AGX_IMMEDIATE_DIM | AGX_IMMEDIATE_LOD_MODE | AGX_IMMEDIATE_MASK | AGX_IMMEDIATE_SCOREBOARD | AGX_IMMEDIATE_OFFSET | AGX_IMMEDIATE_SHADOW | AGX_IMMEDIATE_GATHER,
      { 0x31, 8, 1 },
      { 0 },
      AGX_SCHEDULE_CLASS_NONE,
      0,
      1,
      1,
   },

   [AGX_OPCODE_TEXTURE_LOAD] = {
      "texture_load", 6, 1, AGX_IMMEDIATE_DIM | AGX_IMMEDIATE_LOD_MODE | AGX_IMMEDIATE_MASK | AGX_IMMEDIATE_SCOREBOARD | AGX_IMMEDIATE_OFFSET,
      { 0x71, 8, 1 },
      { 0 },
      AGX_SCHEDULE_CLASS_NONE,
      0,
      1,
      1,
   },

   [AGX_OPCODE_IMAGE_LOAD] = {
      "image_load", 6, 1, AGX_IMMEDIATE_DIM | AGX_IMMEDIATE_LOD_MODE | AGX_IMMEDIATE_MASK | AGX_IMMEDIATE_SCOREBOARD | AGX_IMMEDIATE_OFFSET,
      { 0x71, 8, 1 },
      { 0 },
      AGX_SCHEDULE_CLASS_LOAD,
      0,
      1,
      0,
   },

   [AGX_OPCODE_DEVICE_LOAD] = {
      "device_load", 2, 1, AGX_IMMEDIATE_FORMAT | AGX_IMMEDIATE_MASK | AGX_IMMEDIATE_SHIFT | AGX_IMMEDIATE_SCOREBOARD,
      { 0x5, 6, 1 },
      { 0 },
      AGX_SCHEDULE_CLASS_LOAD,
      0,
      1,
      0,
   },

   [AGX_OPCODE_LOCAL_LOAD] = {
      "local_load", 2, 1, AGX_IMMEDIATE_FORMAT | AGX_IMMEDIATE_MASK,
      { 0x69, 6, 1 },
      { 0 },
      AGX_SCHEDULE_CLASS_LOAD,
      0,
      1,
      0,
   },

   [AGX_OPCODE_DEVICE_STORE] = {
      "device_store", 3, 0, AGX_IMMEDIATE_FORMAT | AGX_IMMEDIATE_MASK | AGX_IMMEDIATE_SHIFT | AGX_IMMEDIATE_SCOREBOARD,
      { 0x800000000045, 8, 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_STORE,
      0,
      0,
      1,
   },

   [AGX_OPCODE_LOCAL_STORE] = {
      "local_store", 3, 0, AGX_IMMEDIATE_FORMAT | AGX_IMMEDIATE_MASK,
      { 0x29, 6, 1 },
      { 0 },
      AGX_SCHEDULE_CLASS_STORE,
      0,
      0,
      1,
   },

   [AGX_OPCODE_UNIFORM_STORE] = {
      "uniform_store", 2, 0, 0,
      { 0x800038000045, 8, 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_NONE,
      0,
      0,
      1,
   },

   [AGX_OPCODE_ATOMIC] = {
      "atomic", 3, 1, AGX_IMMEDIATE_ATOMIC_OPC | AGX_IMMEDIATE_SCOREBOARD,
      { 0x500084000015, 8, 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_ATOMIC,
      0,
      0,
      1,
   },

   [AGX_OPCODE_LOCAL_ATOMIC] = {
      "local_atomic", 3, 1, AGX_IMMEDIATE_ATOMIC_OPC,
      { 0x801000008019, 10, 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_ATOMIC,
      0,
      0,
      1,
   },

   [AGX_OPCODE_WAIT] = {
      "wait", 0, 0, AGX_IMMEDIATE_SCOREBOARD,
      { 0x38, 2, 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_INVALID,
      0,
      0,
      1,
   },

   [AGX_OPCODE_GET_SR] = {
      "get_sr", 0, 1, AGX_IMMEDIATE_SR,
      { 0x72, 4, 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_NONE,
      0,
      1,
      1,
   },

   [AGX_OPCODE_GET_SR_COVERAGE] = {
      "get_sr_coverage", 0, 1, AGX_IMMEDIATE_SR,
      { 0x72, 4, 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_COVERAGE,
      0,
      1,
      0,
   },

   [AGX_OPCODE_SAMPLE_MASK] = {
      "sample_mask", 2, 0, 0,
      { 0x7fc1, 6, 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_COVERAGE,
      0,
      0,
      1,
   },

   [AGX_OPCODE_ZS_EMIT] = {
      "zs_emit", 2, 0, AGX_IMMEDIATE_ZS,
      { 0x41, 4, 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_COVERAGE,
      0,
      0,
      1,
   },

   [AGX_OPCODE_LD_TILE] = {
      "ld_tile", 1, 1, AGX_IMMEDIATE_FORMAT | AGX_IMMEDIATE_MASK | AGX_IMMEDIATE_PIXEL_OFFSET,
      { 0x49, 8, 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_COVERAGE,
      0,
      1,
      0,
   },

   [AGX_OPCODE_ST_TILE] = {
      "st_tile", 2, 0, AGX_IMMEDIATE_FORMAT | AGX_IMMEDIATE_MASK | AGX_IMMEDIATE_PIXEL_OFFSET,
      { 0x9, 8, 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_COVERAGE,
      0,
      0,
      1,
   },

   [AGX_OPCODE_JMP_EXEC_ANY] = {
      "jmp_exec_any", 0, 0, AGX_IMMEDIATE_TARGET,
      { 0xc000, 6, 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_INVALID,
      0,
      0,
      1,
   },

   [AGX_OPCODE_JMP_EXEC_NONE] = {
      "jmp_exec_none", 0, 0, AGX_IMMEDIATE_TARGET,
      { 0xc020, 6, 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_INVALID,
      0,
      0,
      1,
   },

   [AGX_OPCODE_JMP_EXEC_NONE_AFTER] = {
      "jmp_exec_none_after", 0, 0, AGX_IMMEDIATE_TARGET,
      { 0xc020, 6, 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_INVALID,
      0,
      0,
      1,
   },

   [AGX_OPCODE_POP_EXEC] = {
      "pop_exec", 0, 0, AGX_IMMEDIATE_NEST,
      { 0x652, 6, 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_INVALID,
      0,
      0,
      1,
   },

   [AGX_OPCODE_IF_ICMP] = {
      "if_icmp", 2, 0, AGX_IMMEDIATE_NEST | AGX_IMMEDIATE_ICOND | AGX_IMMEDIATE_INVERT_COND | AGX_IMMEDIATE_TARGET,
      { 0x52, 6, 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_INVALID,
      0,
      0,
      1,
   },

   [AGX_OPCODE_ELSE_ICMP] = {
      "else_icmp", 2, 0, AGX_IMMEDIATE_NEST | AGX_IMMEDIATE_ICOND | AGX_IMMEDIATE_INVERT_COND | AGX_IMMEDIATE_TARGET,
      { 0x252, 6, 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_PRELOAD,
      0,
      0,
      1,
   },

   [AGX_OPCODE_WHILE_ICMP] = {
      "while_icmp", 2, 0, AGX_IMMEDIATE_NEST | AGX_IMMEDIATE_ICOND | AGX_IMMEDIATE_INVERT_COND | AGX_IMMEDIATE_TARGET,
      { 0x452, 6, 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_INVALID,
      0,
      0,
      1,
   },

   [AGX_OPCODE_IF_FCMP] = {
      "if_fcmp", 2, 0, AGX_IMMEDIATE_NEST | AGX_IMMEDIATE_FCOND | AGX_IMMEDIATE_INVERT_COND | AGX_IMMEDIATE_TARGET,
      { 0x42, 6, 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_INVALID,
      1,
      0,
      1,
   },

   [AGX_OPCODE_ELSE_FCMP] = {
      "else_fcmp", 2, 0, AGX_IMMEDIATE_NEST | AGX_IMMEDIATE_FCOND | AGX_IMMEDIATE_INVERT_COND | AGX_IMMEDIATE_TARGET,
      { 0x242, 6, 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_PRELOAD,
      1,
      0,
      1,
   },

   [AGX_OPCODE_WHILE_FCMP] = {
      "while_fcmp", 2, 0, AGX_IMMEDIATE_NEST | AGX_IMMEDIATE_FCOND | AGX_IMMEDIATE_INVERT_COND | AGX_IMMEDIATE_TARGET,
      { 0x442, 6, 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_INVALID,
      1,
      0,
      1,
   },

   [AGX_OPCODE_BITOP] = {
      "bitop", 2, 1, AGX_IMMEDIATE_TRUTH_TABLE,
      { 0x7e, 6, 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_NONE,
      0,
      1,
      1,
   },

   [AGX_OPCODE_INTL] = {
      "intl", 2, 1, 0,
      { 0x3e, 6, 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_NONE,
      0,
      1,
      1,
   },

   [AGX_OPCODE_CONVERT] = {
      "convert", 2, 1, AGX_IMMEDIATE_ROUND,
      { 0x803e, 6, 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_NONE,
      0,
      1,
      1,
   },

   [AGX_OPCODE_ITER] = {
      "iter", 2, 1, AGX_IMMEDIATE_CHANNELS | AGX_IMMEDIATE_INTERPOLATION,
      { 0x21, 8, 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_NONE,
      0,
      1,
      1,
   },

   [AGX_OPCODE_ITERPROJ] = {
      "iterproj", 3, 1, AGX_IMMEDIATE_CHANNELS | AGX_IMMEDIATE_INTERPOLATION,
      { 0x21, 8, 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_NONE,
      0,
      1,
      1,
   },

   [AGX_OPCODE_LDCF] = {
      "ldcf", 1, 1, AGX_IMMEDIATE_CHANNELS,
      { 0xa1, 8, 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_NONE,
      0,
      1,
      1,
   },

   [AGX_OPCODE_ST_VARY] = {
      "st_vary", 2, 0, 0,
      { 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_NONE,
      0,
      0,
      1,
   },

   [AGX_OPCODE_NO_VARYINGS] = {
      "no_varyings", 0, 0, 0,
      { 0x80000051, 4, 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_NONE,
      0,
      0,
      1,
   },

   [AGX_OPCODE_STOP] = {
      "stop", 0, 0, 0,
      { 0x88, 2, 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_INVALID,
      0,
      0,
      1,
   },

   [AGX_OPCODE_TRAP] = {
      "trap", 0, 0, 0,
      { 0x8, 2, 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_INVALID,
      0,
      0,
      1,
   },

   [AGX_OPCODE_WAIT_PIX] = {
      "wait_pix", 0, 0, AGX_IMMEDIATE_WRITEOUT,
      { 0x48, 4, 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_COVERAGE,
      0,
      0,
      1,
   },

   [AGX_OPCODE_SIGNAL_PIX] = {
      "signal_pix", 0, 0, AGX_IMMEDIATE_WRITEOUT,
      { 0x58, 4, 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_COVERAGE,
      0,
      0,
      1,
   },

   [AGX_OPCODE_IMAGE_WRITE] = {
      "image_write", 5, 0, AGX_IMMEDIATE_DIM,
      { 0x4800008000f1, 6, 1 },
      { 0 },
      AGX_SCHEDULE_CLASS_STORE,
      0,
      0,
      1,
   },

   [AGX_OPCODE_BLOCK_IMAGE_STORE] = {
      "block_image_store", 3, 0, AGX_IMMEDIATE_FORMAT | AGX_IMMEDIATE_DIM,
      { 0xb1, 10, 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_STORE,
      0,
      0,
      1,
   },

   [AGX_OPCODE_THREADGROUP_BARRIER] = {
      "threadgroup_barrier", 0, 0, 0,
      { 0x68, 2, 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_BARRIER,
      0,
      0,
      1,
   },

   [AGX_OPCODE_MEMORY_BARRIER] = {
      "memory_barrier", 0, 0, 0,
      { 0x96f5, 2, 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_BARRIER,
      0,
      0,
      1,
   },

   [AGX_OPCODE_IMAGE_BARRIER_1] = {
      "image_barrier_1", 0, 0, 0,
      { 0xaaf5, 2, 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_BARRIER,
      0,
      0,
      1,
   },

   [AGX_OPCODE_IMAGE_BARRIER_2] = {
      "image_barrier_2", 0, 0, 0,
      { 0xaef5, 2, 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_BARRIER,
      0,
      0,
      1,
   },

   [AGX_OPCODE_IMAGE_BARRIER_3] = {
      "image_barrier_3", 0, 0, 0,
      { 0xa9f5, 2, 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_BARRIER,
      0,
      0,
      1,
   },

   [AGX_OPCODE_IMAGE_BARRIER_4] = {
      "image_barrier_4", 0, 0, 0,
      { 0xadf5, 2, 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_BARRIER,
      0,
      0,
      1,
   },

   [AGX_OPCODE_FLUSH_MEMORY_TO_TEXTURE] = {
      "flush_memory_to_texture", 0, 0, 0,
      { 0x40f5, 2, 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_BARRIER,
      0,
      0,
      1,
   },

   [AGX_OPCODE_MOV] = {
      "mov", 1, 1, 0,
      { 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_NONE,
      0,
      1,
      1,
   },

   [AGX_OPCODE_NOT] = {
      "not", 1, 1, 0,
      { 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_NONE,
      0,
      1,
      1,
   },

   [AGX_OPCODE_XOR] = {
      "xor", 2, 1, 0,
      { 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_NONE,
      0,
      1,
      1,
   },

   [AGX_OPCODE_AND] = {
      "and", 2, 1, 0,
      { 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_NONE,
      0,
      1,
      1,
   },

   [AGX_OPCODE_OR] = {
      "or", 2, 1, 0,
      { 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_NONE,
      0,
      1,
      1,
   },

   [AGX_OPCODE_COLLECT] = {
      "collect", 0, 1, 0,
      { 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_NONE,
      0,
      1,
      1,
   },

   [AGX_OPCODE_SPLIT] = {
      "split", 1, 0, 0,
      { 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_NONE,
      0,
      1,
      1,
   },

   [AGX_OPCODE_PHI] = {
      "phi", 0, 1, 0,
      { 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_PRELOAD,
      0,
      1,
      1,
   },

   [AGX_OPCODE_UNIT_TEST] = {
      "unit_test", 1, 0, 0,
      { 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_NONE,
      0,
      0,
      1,
   },

   [AGX_OPCODE_PRELOAD] = {
      "preload", 1, 1, 0,
      { 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_PRELOAD,
      0,
      1,
      1,
   },

   [AGX_OPCODE_BEGIN_CF] = {
      "begin_cf", 0, 0, 0,
      { 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_NONE,
      0,
      0,
      1,
   },

   [AGX_OPCODE_BREAK] = {
      "break", 0, 0, AGX_IMMEDIATE_NEST | AGX_IMMEDIATE_TARGET,
      { 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_INVALID,
      0,
      0,
      1,
   },

   [AGX_OPCODE_BREAK_IF_ICMP] = {
      "break_if_icmp", 2, 0, AGX_IMMEDIATE_NEST | AGX_IMMEDIATE_INVERT_COND | AGX_IMMEDIATE_ICOND | AGX_IMMEDIATE_TARGET,
      { 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_INVALID,
      0,
      0,
      1,
   },

   [AGX_OPCODE_BREAK_IF_FCMP] = {
      "break_if_fcmp", 2, 0, AGX_IMMEDIATE_NEST | AGX_IMMEDIATE_INVERT_COND | AGX_IMMEDIATE_FCOND | AGX_IMMEDIATE_TARGET,
      { 0 },
      { 0 },
      AGX_SCHEDULE_CLASS_INVALID,
      0,
      0,
      1,
   },
};

