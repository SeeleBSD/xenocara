/*
 * Copyright 2021 Alyssa Rosenzweig
 * SPDX-License-Identifier: MIT
 */

#ifndef _AGX_OPCODES_
#define _AGX_OPCODES_

#include <stdbool.h>
#include <stdint.h>
#include "util/macros.h"

enum agx_schedule_class {
   AGX_SCHEDULE_CLASS_INVALID,
   AGX_SCHEDULE_CLASS_NONE,
   AGX_SCHEDULE_CLASS_LOAD,
   AGX_SCHEDULE_CLASS_STORE,
   AGX_SCHEDULE_CLASS_ATOMIC,
   AGX_SCHEDULE_CLASS_COVERAGE,
   AGX_SCHEDULE_CLASS_PRELOAD,
   AGX_SCHEDULE_CLASS_BARRIER,
};

/* Listing of opcodes */

enum agx_opcode {
   AGX_OPCODE_FLOOR,
   AGX_OPCODE_SRSQRT,
   AGX_OPCODE_DFDX,
   AGX_OPCODE_DFDY,
   AGX_OPCODE_RCP,
   AGX_OPCODE_RSQRT,
   AGX_OPCODE_SIN_PT_1,
   AGX_OPCODE_LOG2,
   AGX_OPCODE_EXP2,
   AGX_OPCODE_SIN_PT_2,
   AGX_OPCODE_CEIL,
   AGX_OPCODE_TRUNC,
   AGX_OPCODE_ROUNDEVEN,
   AGX_OPCODE_BITREV,
   AGX_OPCODE_POPCOUNT,
   AGX_OPCODE_FFS,
   AGX_OPCODE_FADD,
   AGX_OPCODE_FMA,
   AGX_OPCODE_FMUL,
   AGX_OPCODE_MOV_IMM,
   AGX_OPCODE_IADD,
   AGX_OPCODE_IMAD,
   AGX_OPCODE_BFI,
   AGX_OPCODE_BFEIL,
   AGX_OPCODE_EXTR,
   AGX_OPCODE_ASR,
   AGX_OPCODE_ICMPSEL,
   AGX_OPCODE_FCMPSEL,
   AGX_OPCODE_ICMP,
   AGX_OPCODE_FCMP,
   AGX_OPCODE_TEXTURE_SAMPLE,
   AGX_OPCODE_TEXTURE_LOAD,
   AGX_OPCODE_IMAGE_LOAD,
   AGX_OPCODE_DEVICE_LOAD,
   AGX_OPCODE_LOCAL_LOAD,
   AGX_OPCODE_DEVICE_STORE,
   AGX_OPCODE_LOCAL_STORE,
   AGX_OPCODE_UNIFORM_STORE,
   AGX_OPCODE_ATOMIC,
   AGX_OPCODE_LOCAL_ATOMIC,
   AGX_OPCODE_WAIT,
   AGX_OPCODE_GET_SR,
   AGX_OPCODE_GET_SR_COVERAGE,
   AGX_OPCODE_SAMPLE_MASK,
   AGX_OPCODE_ZS_EMIT,
   AGX_OPCODE_LD_TILE,
   AGX_OPCODE_ST_TILE,
   AGX_OPCODE_JMP_EXEC_ANY,
   AGX_OPCODE_JMP_EXEC_NONE,
   AGX_OPCODE_JMP_EXEC_NONE_AFTER,
   AGX_OPCODE_POP_EXEC,
   AGX_OPCODE_IF_ICMP,
   AGX_OPCODE_ELSE_ICMP,
   AGX_OPCODE_WHILE_ICMP,
   AGX_OPCODE_IF_FCMP,
   AGX_OPCODE_ELSE_FCMP,
   AGX_OPCODE_WHILE_FCMP,
   AGX_OPCODE_BITOP,
   AGX_OPCODE_INTL,
   AGX_OPCODE_CONVERT,
   AGX_OPCODE_ITER,
   AGX_OPCODE_ITERPROJ,
   AGX_OPCODE_LDCF,
   AGX_OPCODE_ST_VARY,
   AGX_OPCODE_NO_VARYINGS,
   AGX_OPCODE_STOP,
   AGX_OPCODE_TRAP,
   AGX_OPCODE_WAIT_PIX,
   AGX_OPCODE_SIGNAL_PIX,
   AGX_OPCODE_IMAGE_WRITE,
   AGX_OPCODE_BLOCK_IMAGE_STORE,
   AGX_OPCODE_THREADGROUP_BARRIER,
   AGX_OPCODE_MEMORY_BARRIER,
   AGX_OPCODE_IMAGE_BARRIER_1,
   AGX_OPCODE_IMAGE_BARRIER_2,
   AGX_OPCODE_IMAGE_BARRIER_3,
   AGX_OPCODE_IMAGE_BARRIER_4,
   AGX_OPCODE_FLUSH_MEMORY_TO_TEXTURE,
   AGX_OPCODE_MOV,
   AGX_OPCODE_NOT,
   AGX_OPCODE_XOR,
   AGX_OPCODE_AND,
   AGX_OPCODE_OR,
   AGX_OPCODE_COLLECT,
   AGX_OPCODE_SPLIT,
   AGX_OPCODE_PHI,
   AGX_OPCODE_UNIT_TEST,
   AGX_OPCODE_PRELOAD,
   AGX_OPCODE_BEGIN_CF,
   AGX_OPCODE_BREAK,
   AGX_OPCODE_BREAK_IF_ICMP,
   AGX_OPCODE_BREAK_IF_FCMP,
   AGX_NUM_OPCODES
};

enum agx_dim {
   AGX_DIM_1D = 0,
   AGX_DIM_1D_ARRAY = 1,
   AGX_DIM_2D = 2,
   AGX_DIM_2D_ARRAY = 3,
   AGX_DIM_2D_MS = 4,
   AGX_DIM_3D = 5,
   AGX_DIM_CUBE = 6,
   AGX_DIM_CUBE_ARRAY = 7,
   AGX_DIM_2D_MS_ARRAY = 8,
};

static inline const char *
agx_dim_as_str(enum agx_dim x)
{
    switch (x) {
    case AGX_DIM_1D: return "1d";
    case AGX_DIM_1D_ARRAY: return "1d_array";
    case AGX_DIM_2D: return "2d";
    case AGX_DIM_2D_ARRAY: return "2d_array";
    case AGX_DIM_2D_MS: return "2d_ms";
    case AGX_DIM_3D: return "3d";
    case AGX_DIM_CUBE: return "cube";
    case AGX_DIM_CUBE_ARRAY: return "cube_array";
    case AGX_DIM_2D_MS_ARRAY: return "2d_ms_array";
    default: unreachable("Nonexhaustive enum");
    }
}

enum agx_gather {
   AGX_GATHER_NONE = 0,
   AGX_GATHER_R = 1,
   AGX_GATHER_G = 3,
   AGX_GATHER_B = 5,
   AGX_GATHER_A = 7,
};

static inline const char *
agx_gather_as_str(enum agx_gather x)
{
    switch (x) {
    case AGX_GATHER_NONE: return "none";
    case AGX_GATHER_R: return "r";
    case AGX_GATHER_G: return "g";
    case AGX_GATHER_B: return "b";
    case AGX_GATHER_A: return "a";
    default: unreachable("Nonexhaustive enum");
    }
}

enum agx_sr {
   AGX_SR_THREADGROUP_POSITION_IN_GRID_X = 0,
   AGX_SR_THREADGROUP_POSITION_IN_GRID_Y = 1,
   AGX_SR_THREADGROUP_POSITION_IN_GRID_Z = 2,
   AGX_SR_THREADS_PER_THREADGROUP_X = 4,
   AGX_SR_THREADS_PER_THREADGROUP_Y = 5,
   AGX_SR_THREADS_PER_THREADGROUP_Z = 6,
   AGX_SR_DISPATCH_THREADS_PER_THREADGROUP_X = 8,
   AGX_SR_DISPATCH_THREADS_PER_THREADGROUP_Y = 9,
   AGX_SR_DISPATCH_THREADS_PER_THREADGROUP_Z = 10,
   AGX_SR_CORE_INDEX = 20,
   AGX_SR_VM_SLOT = 21,
   AGX_SR_THREAD_POSITION_IN_THREADGROUP_X = 48,
   AGX_SR_THREAD_POSITION_IN_THREADGROUP_Y = 49,
   AGX_SR_THREAD_POSITION_IN_THREADGROUP_Z = 50,
   AGX_SR_THREAD_INDEX_IN_THREADGROUP = 51,
   AGX_SR_THREAD_INDEX_IN_SUBGROUP = 52,
   AGX_SR_SUBGROUP_INDEX_IN_THREADGROUP = 53,
   AGX_SR_ACTIVE_THREAD_INDEX_IN_QUAD = 56,
   AGX_SR_ACTIVE_THREAD_INDEX_IN_SUBGROUP = 58,
   AGX_SR_COVERAGE_MASK = 60,
   AGX_SR_BACKFACING = 62,
   AGX_SR_IS_ACTIVE_THREAD = 63,
   AGX_SR_THREAD_POSITION_IN_GRID_X = 80,
   AGX_SR_THREAD_POSITION_IN_GRID_Y = 81,
   AGX_SR_THREAD_POSITION_IN_GRID_Z = 82,
   AGX_SR_INPUT_SAMPLE_MASK = 124,
   AGX_SR_OPFIFO_CMD = 144,
   AGX_SR_OPFIFO_DATA_L = 146,
   AGX_SR_OPFIFO_DATA_H = 147,
};

static inline const char *
agx_sr_as_str(enum agx_sr x)
{
    switch (x) {
    case AGX_SR_THREADGROUP_POSITION_IN_GRID_X: return "threadgroup_position_in_grid.x";
    case AGX_SR_THREADGROUP_POSITION_IN_GRID_Y: return "threadgroup_position_in_grid.y";
    case AGX_SR_THREADGROUP_POSITION_IN_GRID_Z: return "threadgroup_position_in_grid.z";
    case AGX_SR_THREADS_PER_THREADGROUP_X: return "threads_per_threadgroup.x";
    case AGX_SR_THREADS_PER_THREADGROUP_Y: return "threads_per_threadgroup.y";
    case AGX_SR_THREADS_PER_THREADGROUP_Z: return "threads_per_threadgroup.z";
    case AGX_SR_DISPATCH_THREADS_PER_THREADGROUP_X: return "dispatch_threads_per_threadgroup.x";
    case AGX_SR_DISPATCH_THREADS_PER_THREADGROUP_Y: return "dispatch_threads_per_threadgroup.y";
    case AGX_SR_DISPATCH_THREADS_PER_THREADGROUP_Z: return "dispatch_threads_per_threadgroup.z";
    case AGX_SR_CORE_INDEX: return "core_index";
    case AGX_SR_VM_SLOT: return "vm_slot";
    case AGX_SR_THREAD_POSITION_IN_THREADGROUP_X: return "thread_position_in_threadgroup.x";
    case AGX_SR_THREAD_POSITION_IN_THREADGROUP_Y: return "thread_position_in_threadgroup.y";
    case AGX_SR_THREAD_POSITION_IN_THREADGROUP_Z: return "thread_position_in_threadgroup.z";
    case AGX_SR_THREAD_INDEX_IN_THREADGROUP: return "thread_index_in_threadgroup";
    case AGX_SR_THREAD_INDEX_IN_SUBGROUP: return "thread_index_in_subgroup";
    case AGX_SR_SUBGROUP_INDEX_IN_THREADGROUP: return "subgroup_index_in_threadgroup";
    case AGX_SR_ACTIVE_THREAD_INDEX_IN_QUAD: return "active_thread_index_in_quad";
    case AGX_SR_ACTIVE_THREAD_INDEX_IN_SUBGROUP: return "active_thread_index_in_subgroup";
    case AGX_SR_COVERAGE_MASK: return "coverage_mask";
    case AGX_SR_BACKFACING: return "backfacing";
    case AGX_SR_IS_ACTIVE_THREAD: return "is_active_thread";
    case AGX_SR_THREAD_POSITION_IN_GRID_X: return "thread_position_in_grid.x";
    case AGX_SR_THREAD_POSITION_IN_GRID_Y: return "thread_position_in_grid.y";
    case AGX_SR_THREAD_POSITION_IN_GRID_Z: return "thread_position_in_grid.z";
    case AGX_SR_INPUT_SAMPLE_MASK: return "input_sample_mask";
    case AGX_SR_OPFIFO_CMD: return "opfifo_cmd";
    case AGX_SR_OPFIFO_DATA_L: return "opfifo_data_l";
    case AGX_SR_OPFIFO_DATA_H: return "opfifo_data_h";
    default: unreachable("Nonexhaustive enum");
    }
}

enum agx_atomic_opc {
   AGX_ATOMIC_OPC_ADD = 0,
   AGX_ATOMIC_OPC_SUB = 1,
   AGX_ATOMIC_OPC_XCHG = 2,
   AGX_ATOMIC_OPC_CMPXCHG = 3,
   AGX_ATOMIC_OPC_UMIN = 4,
   AGX_ATOMIC_OPC_IMIN = 5,
   AGX_ATOMIC_OPC_UMAX = 6,
   AGX_ATOMIC_OPC_IMAX = 7,
   AGX_ATOMIC_OPC_AND = 8,
   AGX_ATOMIC_OPC_OR = 9,
   AGX_ATOMIC_OPC_XOR = 10,
};

static inline const char *
agx_atomic_opc_as_str(enum agx_atomic_opc x)
{
    switch (x) {
    case AGX_ATOMIC_OPC_ADD: return "add";
    case AGX_ATOMIC_OPC_SUB: return "sub";
    case AGX_ATOMIC_OPC_XCHG: return "xchg";
    case AGX_ATOMIC_OPC_CMPXCHG: return "cmpxchg";
    case AGX_ATOMIC_OPC_UMIN: return "umin";
    case AGX_ATOMIC_OPC_IMIN: return "imin";
    case AGX_ATOMIC_OPC_UMAX: return "umax";
    case AGX_ATOMIC_OPC_IMAX: return "imax";
    case AGX_ATOMIC_OPC_AND: return "and";
    case AGX_ATOMIC_OPC_OR: return "or";
    case AGX_ATOMIC_OPC_XOR: return "xor";
    default: unreachable("Nonexhaustive enum");
    }
}

enum agx_interpolation {
   AGX_INTERPOLATION_CENTER = 0,
   AGX_INTERPOLATION_SAMPLE = 1,
   AGX_INTERPOLATION_CENTROID = 2,
   AGX_INTERPOLATION_SAMPLE_REGISTER = 3,
};

static inline const char *
agx_interpolation_as_str(enum agx_interpolation x)
{
    switch (x) {
    case AGX_INTERPOLATION_CENTER: return "center";
    case AGX_INTERPOLATION_SAMPLE: return "sample";
    case AGX_INTERPOLATION_CENTROID: return "centroid";
    case AGX_INTERPOLATION_SAMPLE_REGISTER: return "sample_register";
    default: unreachable("Nonexhaustive enum");
    }
}


/* Runtime accessible info on each defined opcode */



enum agx_immediate {
   AGX_IMMEDIATE_FORMAT = (1 << 0),
   AGX_IMMEDIATE_IMM = (1 << 1),
   AGX_IMMEDIATE_WRITEOUT = (1 << 2),
   AGX_IMMEDIATE_INDEX = (1 << 3),
   AGX_IMMEDIATE_COMPONENT = (1 << 4),
   AGX_IMMEDIATE_CHANNELS = (1 << 5),
   AGX_IMMEDIATE_TRUTH_TABLE = (1 << 6),
   AGX_IMMEDIATE_ROUND = (1 << 7),
   AGX_IMMEDIATE_SHIFT = (1 << 8),
   AGX_IMMEDIATE_MASK = (1 << 9),
   AGX_IMMEDIATE_BFI_MASK = (1 << 10),
   AGX_IMMEDIATE_LOD_MODE = (1 << 11),
   AGX_IMMEDIATE_PIXEL_OFFSET = (1 << 12),
   AGX_IMMEDIATE_DIM = (1 << 13),
   AGX_IMMEDIATE_GATHER = (1 << 14),
   AGX_IMMEDIATE_OFFSET = (1 << 15),
   AGX_IMMEDIATE_SHADOW = (1 << 16),
   AGX_IMMEDIATE_SCOREBOARD = (1 << 17),
   AGX_IMMEDIATE_ICOND = (1 << 18),
   AGX_IMMEDIATE_FCOND = (1 << 19),
   AGX_IMMEDIATE_NEST = (1 << 20),
   AGX_IMMEDIATE_INVERT_COND = (1 << 21),
   AGX_IMMEDIATE_TARGET = (1 << 22),
   AGX_IMMEDIATE_ZS = (1 << 23),
   AGX_IMMEDIATE_SR = (1 << 24),
   AGX_IMMEDIATE_ATOMIC_OPC = (1 << 25),
   AGX_IMMEDIATE_INTERPOLATION = (1 << 26),
};

struct agx_encoding {
   uint64_t exact;
   unsigned length_short : 4;
   bool extensible : 1;
};

struct agx_opcode_info {
   const char *name;
   unsigned nr_srcs;
   unsigned nr_dests;
   enum agx_immediate immediates;
   struct agx_encoding encoding;
   struct agx_encoding encoding_16;
   enum agx_schedule_class schedule_class;
   bool is_float : 1;
   bool can_eliminate : 1;
   bool can_reorder : 1;
};

extern const struct agx_opcode_info agx_opcodes_info[AGX_NUM_OPCODES];

#endif

