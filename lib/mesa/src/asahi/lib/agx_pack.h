
/* Generated code, see midgard.xml and gen_pack_header.py
 *
 * Packets, enums and structures for Panfrost.
 *
 * This file has been generated, do not hand edit.
 */

#ifndef AGX_PACK_H
#define AGX_PACK_H

#include <stdio.h>
#include <inttypes.h>

#include "util/bitpack_helpers.h"

#define __gen_unpack_float(x, y, z) uif(__gen_unpack_uint(x, y, z))

static inline uint64_t
__gen_unpack_uint(const uint8_t *restrict cl, uint32_t start, uint32_t end)
{
   uint64_t val = 0;
   const int width = end - start + 1;
   const uint64_t mask = (width == 64 ? ~0 : (1ull << width) - 1 );

   for (unsigned byte = start / 8; byte <= end / 8; byte++) {
      val |= ((uint64_t) cl[byte]) << ((byte - start / 8) * 8);
   }

   return (val >> (start % 8)) & mask;
}

/*
 * LODs are 4:6 fixed point. We must clamp before converting to integers to
 * avoid undefined behaviour for out-of-bounds inputs like +/- infinity.
 */
static inline uint32_t
__gen_pack_lod(float f, uint32_t start, uint32_t end)
{
    uint32_t fixed = CLAMP(f * (1 << 6), 0 /* 0.0 */, 0x380 /* 14.0 */);
    return util_bitpack_uint(fixed, start, end);
}

static inline float
__gen_unpack_lod(const uint8_t *restrict cl, uint32_t start, uint32_t end)
{
    return ((float) __gen_unpack_uint(cl, start, end)) / (1 << 6);
}

static inline uint64_t
__gen_unpack_sint(const uint8_t *restrict cl, uint32_t start, uint32_t end)
{
   int size = end - start + 1;
   int64_t val = __gen_unpack_uint(cl, start, end);

   return util_sign_extend(val, size);
}

static inline uint64_t
__gen_to_groups(uint32_t value, uint32_t group_size, uint32_t length)
{
    /* Zero is not representable, clamp to minimum */
    if (value == 0)
        return 1;

    /* Round up to the nearest number of groups */
    uint32_t groups = DIV_ROUND_UP(value, group_size);

    /* The 0 encoding means "all" */
    if (groups == (1ull << length))
        return 0;

    /* Otherwise it's encoded as the identity */
    assert(groups < (1u << length) && "out of bounds");
    assert(groups >= 1 && "exhaustive");
    return groups;
}

static inline uint64_t
__gen_from_groups(uint32_t value, uint32_t group_size, uint32_t length)
{
    return group_size * (value ? value: (1 << length));
}

#define agx_pack(dst, T, name)                              \
   for (struct AGX_ ## T name = { AGX_ ## T ## _header }, \
        *_loop_terminate = (void *) (dst);                  \
        __builtin_expect(_loop_terminate != NULL, 1);       \
        ({ AGX_ ## T ## _pack((uint32_t *) (dst), &name);  \
           _loop_terminate = NULL; }))

#define agx_unpack(fp, src, T, name)                        \
        struct AGX_ ## T name;                         \
        AGX_ ## T ## _unpack(fp, (uint8_t *)(src), &name)

#define agx_print(fp, T, var, indent)                   \
        AGX_ ## T ## _print(fp, &(var), indent)

static inline void agx_merge_helper(uint32_t *dst, const uint32_t *src, size_t bytes)
{
        assert((bytes & 3) == 0);

        for (unsigned i = 0; i < (bytes / 4); ++i)
                dst[i] |= src[i];
}

#define agx_merge(packed1, packed2, type)         agx_merge_helper((packed1).opaque, (packed2).opaque, AGX_##type##_LENGTH)

enum agx_channel {
        AGX_CHANNEL_R                        =      0,
        AGX_CHANNEL_G                        =      1,
        AGX_CHANNEL_B                        =      2,
        AGX_CHANNEL_A                        =      3,
        AGX_CHANNEL_1                        =      4,
        AGX_CHANNEL_0                        =      5,
};

static inline const char *
agx_channel_as_str(enum agx_channel imm)
{
    switch (imm) {
    case AGX_CHANNEL_R: return "R";
    case AGX_CHANNEL_G: return "G";
    case AGX_CHANNEL_B: return "B";
    case AGX_CHANNEL_A: return "A";
    case AGX_CHANNEL_1: return "1";
    case AGX_CHANNEL_0: return "0";
    default: break;
    }
    return NULL;
}

enum agx_zs_func {
        AGX_ZS_FUNC_NEVER                    =      0,
        AGX_ZS_FUNC_LESS                     =      1,
        AGX_ZS_FUNC_EQUAL                    =      2,
        AGX_ZS_FUNC_LEQUAL                   =      3,
        AGX_ZS_FUNC_GREATER                  =      4,
        AGX_ZS_FUNC_NOT_EQUAL                =      5,
        AGX_ZS_FUNC_GEQUAL                   =      6,
        AGX_ZS_FUNC_ALWAYS                   =      7,
};

static inline const char *
agx_zs_func_as_str(enum agx_zs_func imm)
{
    switch (imm) {
    case AGX_ZS_FUNC_NEVER: return "Never";
    case AGX_ZS_FUNC_LESS: return "Less";
    case AGX_ZS_FUNC_EQUAL: return "Equal";
    case AGX_ZS_FUNC_LEQUAL: return "Lequal";
    case AGX_ZS_FUNC_GREATER: return "Greater";
    case AGX_ZS_FUNC_NOT_EQUAL: return "Not Equal";
    case AGX_ZS_FUNC_GEQUAL: return "Gequal";
    case AGX_ZS_FUNC_ALWAYS: return "Always";
    default: break;
    }
    return NULL;
}

enum agx_compare_func {
        AGX_COMPARE_FUNC_LEQUAL              =      0,
        AGX_COMPARE_FUNC_GEQUAL              =      1,
        AGX_COMPARE_FUNC_LESS                =      2,
        AGX_COMPARE_FUNC_GREATER             =      3,
        AGX_COMPARE_FUNC_EQUAL               =      4,
        AGX_COMPARE_FUNC_NOT_EQUAL           =      5,
        AGX_COMPARE_FUNC_ALWAYS              =      6,
        AGX_COMPARE_FUNC_NEVER               =      7,
};

static inline const char *
agx_compare_func_as_str(enum agx_compare_func imm)
{
    switch (imm) {
    case AGX_COMPARE_FUNC_LEQUAL: return "Lequal";
    case AGX_COMPARE_FUNC_GEQUAL: return "Gequal";
    case AGX_COMPARE_FUNC_LESS: return "Less";
    case AGX_COMPARE_FUNC_GREATER: return "Greater";
    case AGX_COMPARE_FUNC_EQUAL: return "Equal";
    case AGX_COMPARE_FUNC_NOT_EQUAL: return "Not Equal";
    case AGX_COMPARE_FUNC_ALWAYS: return "Always";
    case AGX_COMPARE_FUNC_NEVER: return "Never";
    default: break;
    }
    return NULL;
}

enum agx_stencil_op {
        AGX_STENCIL_OP_KEEP                  =      0,
        AGX_STENCIL_OP_ZERO                  =      1,
        AGX_STENCIL_OP_REPLACE               =      2,
        AGX_STENCIL_OP_INCR_SAT              =      3,
        AGX_STENCIL_OP_DECR_SAT              =      4,
        AGX_STENCIL_OP_INVERT                =      5,
        AGX_STENCIL_OP_INCR_WRAP             =      6,
        AGX_STENCIL_OP_DECR_WRAP             =      7,
};

static inline const char *
agx_stencil_op_as_str(enum agx_stencil_op imm)
{
    switch (imm) {
    case AGX_STENCIL_OP_KEEP: return "Keep";
    case AGX_STENCIL_OP_ZERO: return "Zero";
    case AGX_STENCIL_OP_REPLACE: return "Replace";
    case AGX_STENCIL_OP_INCR_SAT: return "Incr Sat";
    case AGX_STENCIL_OP_DECR_SAT: return "Decr Sat";
    case AGX_STENCIL_OP_INVERT: return "Invert";
    case AGX_STENCIL_OP_INCR_WRAP: return "Incr Wrap";
    case AGX_STENCIL_OP_DECR_WRAP: return "Decr Wrap";
    default: break;
    }
    return NULL;
}

enum agx_visibility_mode {
        AGX_VISIBILITY_MODE_NONE             =      0,
        AGX_VISIBILITY_MODE_COUNTING         =      2,
        AGX_VISIBILITY_MODE_BOOLEAN          =      3,
};

static inline const char *
agx_visibility_mode_as_str(enum agx_visibility_mode imm)
{
    switch (imm) {
    case AGX_VISIBILITY_MODE_NONE: return "None";
    case AGX_VISIBILITY_MODE_COUNTING: return "Counting";
    case AGX_VISIBILITY_MODE_BOOLEAN: return "Boolean";
    default: break;
    }
    return NULL;
}

enum agx_polygon_mode {
        AGX_POLYGON_MODE_FILL                =      0,
        AGX_POLYGON_MODE_LINE                =      1,
        AGX_POLYGON_MODE_POINT               =      2,
};

static inline const char *
agx_polygon_mode_as_str(enum agx_polygon_mode imm)
{
    switch (imm) {
    case AGX_POLYGON_MODE_FILL: return "Fill";
    case AGX_POLYGON_MODE_LINE: return "Line";
    case AGX_POLYGON_MODE_POINT: return "Point";
    default: break;
    }
    return NULL;
}

enum agx_primitive {
        AGX_PRIMITIVE_POINTS                 =      0,
        AGX_PRIMITIVE_LINES                  =      1,
        AGX_PRIMITIVE_LINE_STRIP             =      3,
        AGX_PRIMITIVE_LINE_LOOP              =      5,
        AGX_PRIMITIVE_TRIANGLES              =      6,
        AGX_PRIMITIVE_TRIANGLE_STRIP         =      9,
        AGX_PRIMITIVE_TRIANGLE_FAN           =     10,
        AGX_PRIMITIVE_QUADS                  =     14,
        AGX_PRIMITIVE_QUAD_STRIP             =     15,
};

static inline const char *
agx_primitive_as_str(enum agx_primitive imm)
{
    switch (imm) {
    case AGX_PRIMITIVE_POINTS: return "Points";
    case AGX_PRIMITIVE_LINES: return "Lines";
    case AGX_PRIMITIVE_LINE_STRIP: return "Line strip";
    case AGX_PRIMITIVE_LINE_LOOP: return "Line loop";
    case AGX_PRIMITIVE_TRIANGLES: return "Triangles";
    case AGX_PRIMITIVE_TRIANGLE_STRIP: return "Triangle strip";
    case AGX_PRIMITIVE_TRIANGLE_FAN: return "Triangle fan";
    case AGX_PRIMITIVE_QUADS: return "Quads";
    case AGX_PRIMITIVE_QUAD_STRIP: return "Quad strip";
    default: break;
    }
    return NULL;
}

enum agx_object_type {
        AGX_OBJECT_TYPE_TRIANGLE             =      0,
        AGX_OBJECT_TYPE_LINE                 =      1,
        AGX_OBJECT_TYPE_POINT_SPRITE_UV10    =      2,
        AGX_OBJECT_TYPE_POINT_SPRITE_UV01    =      4,
};

static inline const char *
agx_object_type_as_str(enum agx_object_type imm)
{
    switch (imm) {
    case AGX_OBJECT_TYPE_TRIANGLE: return "Triangle";
    case AGX_OBJECT_TYPE_LINE: return "Line";
    case AGX_OBJECT_TYPE_POINT_SPRITE_UV10: return "Point sprite UV=10";
    case AGX_OBJECT_TYPE_POINT_SPRITE_UV01: return "Point sprite UV=01";
    default: break;
    }
    return NULL;
}

enum agx_layout {
        AGX_LAYOUT_LINEAR                    =      0,
        AGX_LAYOUT_NULL                      =      1,
        AGX_LAYOUT_TWIDDLED                  =      2,
        AGX_LAYOUT_COMPRESSED                =      3,
};

static inline const char *
agx_layout_as_str(enum agx_layout imm)
{
    switch (imm) {
    case AGX_LAYOUT_LINEAR: return "Linear";
    case AGX_LAYOUT_NULL: return "Null";
    case AGX_LAYOUT_TWIDDLED: return "Twiddled";
    case AGX_LAYOUT_COMPRESSED: return "Compressed";
    default: break;
    }
    return NULL;
}

enum agx_channels {
        AGX_CHANNELS_R8                      =      0,
        AGX_CHANNELS_R16                     =      9,
        AGX_CHANNELS_R8G8                    =     10,
        AGX_CHANNELS_R5G6B5                  =     11,
        AGX_CHANNELS_R4G4B4A4                =     12,
        AGX_CHANNELS_A1R5G5B5                =     13,
        AGX_CHANNELS_R5G5B5A1                =     14,
        AGX_CHANNELS_R32                     =     33,
        AGX_CHANNELS_R16G16                  =     35,
        AGX_CHANNELS_R11G11B10               =     37,
        AGX_CHANNELS_R10G10B10A2             =     38,
        AGX_CHANNELS_R9G9B9E5                =     39,
        AGX_CHANNELS_R8G8B8A8                =     40,
        AGX_CHANNELS_R32G32                  =     49,
        AGX_CHANNELS_R16G16B16A16            =     50,
        AGX_CHANNELS_R32G32B32_EMULATED      =     54,
        AGX_CHANNELS_R32G32B32A32            =     56,
        AGX_CHANNELS_GBGR_422                =     64,
        AGX_CHANNELS_BGRG_422                =     65,
        AGX_CHANNELS_PVRTC_2BPP              =     80,
        AGX_CHANNELS_PVRTC_4BPP              =     81,
        AGX_CHANNELS_ETC2_RGB8               =     88,
        AGX_CHANNELS_ETC2_RGBA8              =     89,
        AGX_CHANNELS_ETC2_RGB8A1             =     90,
        AGX_CHANNELS_EAC_R11                 =     91,
        AGX_CHANNELS_EAC_RG11                =     92,
        AGX_CHANNELS_ASTC_4X4                =     96,
        AGX_CHANNELS_ASTC_5X4                =     97,
        AGX_CHANNELS_ASTC_5X5                =     98,
        AGX_CHANNELS_ASTC_6X5                =     99,
        AGX_CHANNELS_ASTC_6X6                =    100,
        AGX_CHANNELS_ASTC_8X5                =    101,
        AGX_CHANNELS_ASTC_8X6                =    102,
        AGX_CHANNELS_ASTC_8X8                =    103,
        AGX_CHANNELS_ASTC_10X5               =    104,
        AGX_CHANNELS_ASTC_10X6               =    105,
        AGX_CHANNELS_ASTC_10X8               =    106,
        AGX_CHANNELS_ASTC_10X10              =    107,
        AGX_CHANNELS_ASTC_12X10              =    108,
        AGX_CHANNELS_ASTC_12X12              =    109,
        AGX_CHANNELS_BC1                     =    116,
        AGX_CHANNELS_BC2                     =    117,
        AGX_CHANNELS_BC3                     =    118,
        AGX_CHANNELS_BC4                     =    119,
        AGX_CHANNELS_BC5                     =    120,
        AGX_CHANNELS_BC6H                    =    121,
        AGX_CHANNELS_BC6H_UFLOAT             =    122,
        AGX_CHANNELS_BC7                     =    123,
};

static inline const char *
agx_channels_as_str(enum agx_channels imm)
{
    switch (imm) {
    case AGX_CHANNELS_R8: return "R8";
    case AGX_CHANNELS_R16: return "R16";
    case AGX_CHANNELS_R8G8: return "R8G8";
    case AGX_CHANNELS_R5G6B5: return "R5G6B5";
    case AGX_CHANNELS_R4G4B4A4: return "R4G4B4A4";
    case AGX_CHANNELS_A1R5G5B5: return "A1R5G5B5";
    case AGX_CHANNELS_R5G5B5A1: return "R5G5B5A1";
    case AGX_CHANNELS_R32: return "R32";
    case AGX_CHANNELS_R16G16: return "R16G16";
    case AGX_CHANNELS_R11G11B10: return "R11G11B10";
    case AGX_CHANNELS_R10G10B10A2: return "R10G10B10A2";
    case AGX_CHANNELS_R9G9B9E5: return "R9G9B9E5";
    case AGX_CHANNELS_R8G8B8A8: return "R8G8B8A8";
    case AGX_CHANNELS_R32G32: return "R32G32";
    case AGX_CHANNELS_R16G16B16A16: return "R16G16B16A16";
    case AGX_CHANNELS_R32G32B32_EMULATED: return "R32G32B32 (Emulated)";
    case AGX_CHANNELS_R32G32B32A32: return "R32G32B32A32";
    case AGX_CHANNELS_GBGR_422: return "GBGR 422";
    case AGX_CHANNELS_BGRG_422: return "BGRG 422";
    case AGX_CHANNELS_PVRTC_2BPP: return "PVRTC 2bpp";
    case AGX_CHANNELS_PVRTC_4BPP: return "PVRTC 4bpp";
    case AGX_CHANNELS_ETC2_RGB8: return "ETC2 RGB8";
    case AGX_CHANNELS_ETC2_RGBA8: return "ETC2 RGBA8";
    case AGX_CHANNELS_ETC2_RGB8A1: return "ETC2 RGB8A1";
    case AGX_CHANNELS_EAC_R11: return "EAC R11";
    case AGX_CHANNELS_EAC_RG11: return "EAC RG11";
    case AGX_CHANNELS_ASTC_4X4: return "ASTC 4x4";
    case AGX_CHANNELS_ASTC_5X4: return "ASTC 5x4";
    case AGX_CHANNELS_ASTC_5X5: return "ASTC 5x5";
    case AGX_CHANNELS_ASTC_6X5: return "ASTC 6x5";
    case AGX_CHANNELS_ASTC_6X6: return "ASTC 6x6";
    case AGX_CHANNELS_ASTC_8X5: return "ASTC 8x5";
    case AGX_CHANNELS_ASTC_8X6: return "ASTC 8x6";
    case AGX_CHANNELS_ASTC_8X8: return "ASTC 8x8";
    case AGX_CHANNELS_ASTC_10X5: return "ASTC 10x5";
    case AGX_CHANNELS_ASTC_10X6: return "ASTC 10x6";
    case AGX_CHANNELS_ASTC_10X8: return "ASTC 10x8";
    case AGX_CHANNELS_ASTC_10X10: return "ASTC 10x10";
    case AGX_CHANNELS_ASTC_12X10: return "ASTC 12x10";
    case AGX_CHANNELS_ASTC_12X12: return "ASTC 12x12";
    case AGX_CHANNELS_BC1: return "BC1";
    case AGX_CHANNELS_BC2: return "BC2";
    case AGX_CHANNELS_BC3: return "BC3";
    case AGX_CHANNELS_BC4: return "BC4";
    case AGX_CHANNELS_BC5: return "BC5";
    case AGX_CHANNELS_BC6H: return "BC6H";
    case AGX_CHANNELS_BC6H_UFLOAT: return "BC6H Ufloat";
    case AGX_CHANNELS_BC7: return "BC7";
    default: break;
    }
    return NULL;
}

enum agx_texture_type {
        AGX_TEXTURE_TYPE_UNORM               =      0,
        AGX_TEXTURE_TYPE_SNORM               =      1,
        AGX_TEXTURE_TYPE_UINT                =      2,
        AGX_TEXTURE_TYPE_SINT                =      3,
        AGX_TEXTURE_TYPE_FLOAT               =      4,
        AGX_TEXTURE_TYPE_XR                  =      5,
};

static inline const char *
agx_texture_type_as_str(enum agx_texture_type imm)
{
    switch (imm) {
    case AGX_TEXTURE_TYPE_UNORM: return "Unorm";
    case AGX_TEXTURE_TYPE_SNORM: return "Snorm";
    case AGX_TEXTURE_TYPE_UINT: return "Uint";
    case AGX_TEXTURE_TYPE_SINT: return "Sint";
    case AGX_TEXTURE_TYPE_FLOAT: return "Float";
    case AGX_TEXTURE_TYPE_XR: return "XR";
    default: break;
    }
    return NULL;
}

struct AGX_CF_BINDING_HEADER {
   uint32_t                             number_of_32_bit_slots;
   uint32_t                             number_of_coefficient_registers;
};

#define AGX_CF_BINDING_HEADER_header            \
   0

static inline void
AGX_CF_BINDING_HEADER_pack(uint32_t * restrict cl,
                           const struct AGX_CF_BINDING_HEADER * restrict values)
{
   cl[ 0] = util_bitpack_uint(values->number_of_32_bit_slots, 0, 7) |
            util_bitpack_uint(values->number_of_coefficient_registers, 8, 15);
}


#define AGX_CF_BINDING_HEADER_LENGTH 4
struct agx_cf_binding_header_packed { uint32_t opaque[1]; };
static inline void
AGX_CF_BINDING_HEADER_unpack(FILE *fp, const uint8_t * restrict cl,
                             struct AGX_CF_BINDING_HEADER * restrict values)
{
   if (((const uint32_t *) cl)[0] & 0xffff0000) fprintf(fp, "XXX: Unknown field of CF binding header unpacked at word 0: got %X, bad mask %X\n", ((const uint32_t *) cl)[0], ((const uint32_t *) cl)[0] & 0xffff0000);
   values->number_of_32_bit_slots = __gen_unpack_uint(cl, 0, 7);
   values->number_of_coefficient_registers = __gen_unpack_uint(cl, 8, 15);
}

static inline void
AGX_CF_BINDING_HEADER_print(FILE *fp, const struct AGX_CF_BINDING_HEADER * values, unsigned indent)
{
   fprintf(fp, "%*sNumber of 32-bit slots: %u\n", indent, "", values->number_of_32_bit_slots);
   fprintf(fp, "%*sNumber of coefficient registers: %u\n", indent, "", values->number_of_coefficient_registers);
}

enum agx_shade_model {
        AGX_SHADE_MODEL_FLAT_VERTEX_0        =      0,
        AGX_SHADE_MODEL_FLAT_VERTEX_2        =      2,
        AGX_SHADE_MODEL_GOURAUD              =      3,
};

static inline const char *
agx_shade_model_as_str(enum agx_shade_model imm)
{
    switch (imm) {
    case AGX_SHADE_MODEL_FLAT_VERTEX_0: return "Flat vertex 0";
    case AGX_SHADE_MODEL_FLAT_VERTEX_2: return "Flat vertex 2";
    case AGX_SHADE_MODEL_GOURAUD: return "Gouraud";
    default: break;
    }
    return NULL;
}

struct AGX_CF_BINDING {
   uint32_t                             components;
   enum agx_shade_model                 shade_model;
   bool                                 perspective;
   bool                                 fragcoord_z;
   bool                                 point_sprite;
   bool                                 barycentric_coordinates;
   uint32_t                             base_slot;
   uint32_t                             base_coefficient_register;
};

#define AGX_CF_BINDING_header                   \
   0

static inline void
AGX_CF_BINDING_pack(uint32_t * restrict cl,
                    const struct AGX_CF_BINDING * restrict values)
{
   assert(values->components >= 1);
   cl[ 0] = util_bitpack_uint(values->components - 1, 0, 1) |
            util_bitpack_uint(values->shade_model, 2, 3) |
            util_bitpack_uint(values->perspective, 4, 4) |
            util_bitpack_uint(values->fragcoord_z, 5, 5) |
            util_bitpack_uint(values->point_sprite, 6, 6) |
            util_bitpack_uint(values->barycentric_coordinates, 7, 7) |
            util_bitpack_uint(values->base_slot, 8, 15) |
            util_bitpack_uint(values->base_coefficient_register, 16, 23);
}


#define AGX_CF_BINDING_LENGTH 4
struct agx_cf_binding_packed { uint32_t opaque[1]; };
static inline void
AGX_CF_BINDING_unpack(FILE *fp, const uint8_t * restrict cl,
                      struct AGX_CF_BINDING * restrict values)
{
   if (((const uint32_t *) cl)[0] & 0xff000000) fprintf(fp, "XXX: Unknown field of CF binding unpacked at word 0: got %X, bad mask %X\n", ((const uint32_t *) cl)[0], ((const uint32_t *) cl)[0] & 0xff000000);
   values->components = __gen_unpack_uint(cl, 0, 1) + 1;
   values->shade_model = (enum agx_shade_model) __gen_unpack_uint(cl, 2, 3);
   values->perspective = __gen_unpack_uint(cl, 4, 4);
   values->fragcoord_z = __gen_unpack_uint(cl, 5, 5);
   values->point_sprite = __gen_unpack_uint(cl, 6, 6);
   values->barycentric_coordinates = __gen_unpack_uint(cl, 7, 7);
   values->base_slot = __gen_unpack_uint(cl, 8, 15);
   values->base_coefficient_register = __gen_unpack_uint(cl, 16, 23);
}

static inline void
AGX_CF_BINDING_print(FILE *fp, const struct AGX_CF_BINDING * values, unsigned indent)
{
   fprintf(fp, "%*sComponents: %u\n", indent, "", values->components);
   if (agx_shade_model_as_str(values->shade_model))
     fprintf(fp, "%*sShade model: %s\n", indent, "", agx_shade_model_as_str(values->shade_model));
    else
     fprintf(fp, "%*sShade model: unknown %X (XXX)\n", indent, "", values->shade_model);
   fprintf(fp, "%*sPerspective: %s\n", indent, "", values->perspective ? "true" : "false");
   fprintf(fp, "%*sFragcoord Z: %s\n", indent, "", values->fragcoord_z ? "true" : "false");
   fprintf(fp, "%*sPoint sprite: %s\n", indent, "", values->point_sprite ? "true" : "false");
   fprintf(fp, "%*sBarycentric coordinates: %s\n", indent, "", values->barycentric_coordinates ? "true" : "false");
   fprintf(fp, "%*sBase slot: %u\n", indent, "", values->base_slot);
   fprintf(fp, "%*sBase coefficient register: %u\n", indent, "", values->base_coefficient_register);
}

enum agx_texture_dimension {
        AGX_TEXTURE_DIMENSION_1D             =      0,
        AGX_TEXTURE_DIMENSION_1D_ARRAY       =      1,
        AGX_TEXTURE_DIMENSION_2D             =      2,
        AGX_TEXTURE_DIMENSION_2D_ARRAY       =      3,
        AGX_TEXTURE_DIMENSION_2D_MULTISAMPLED =      4,
        AGX_TEXTURE_DIMENSION_3D             =      5,
        AGX_TEXTURE_DIMENSION_CUBE           =      6,
        AGX_TEXTURE_DIMENSION_CUBE_ARRAY     =      7,
        AGX_TEXTURE_DIMENSION_2D_ARRAY_MULTISAMPLED =      8,
};

static inline const char *
agx_texture_dimension_as_str(enum agx_texture_dimension imm)
{
    switch (imm) {
    case AGX_TEXTURE_DIMENSION_1D: return "1D";
    case AGX_TEXTURE_DIMENSION_1D_ARRAY: return "1D Array";
    case AGX_TEXTURE_DIMENSION_2D: return "2D";
    case AGX_TEXTURE_DIMENSION_2D_ARRAY: return "2D Array";
    case AGX_TEXTURE_DIMENSION_2D_MULTISAMPLED: return "2D Multisampled";
    case AGX_TEXTURE_DIMENSION_3D: return "3D";
    case AGX_TEXTURE_DIMENSION_CUBE: return "Cube";
    case AGX_TEXTURE_DIMENSION_CUBE_ARRAY: return "Cube Array";
    case AGX_TEXTURE_DIMENSION_2D_ARRAY_MULTISAMPLED: return "2D Array Multisampled";
    default: break;
    }
    return NULL;
}

enum agx_sample_count {
        AGX_SAMPLE_COUNT_2                   =      0,
        AGX_SAMPLE_COUNT_4                   =      1,
};

static inline const char *
agx_sample_count_as_str(enum agx_sample_count imm)
{
    switch (imm) {
    case AGX_SAMPLE_COUNT_2: return "2";
    case AGX_SAMPLE_COUNT_4: return "4";
    default: break;
    }
    return NULL;
}

struct AGX_ATOMIC_SOFTWARE {
   uint64_t                             base;
   uint32_t                             tile_width;
   uint32_t                             tile_height;
   uint32_t                             tiles_per_row;
   uint32_t                             sample_count;
   uint32_t                             layer_stride_pixels;
};

#define AGX_ATOMIC_SOFTWARE_header              \
   .tile_width = 1,  \
   .tile_height = 1,  \
   .sample_count = 1

static inline void
AGX_ATOMIC_SOFTWARE_pack(uint32_t * restrict cl,
                         const struct AGX_ATOMIC_SOFTWARE * restrict values)
{
   assert((values->base & 0x7f) == 0);
   assert(util_is_power_of_two_nonzero(values->tile_width));
   assert(util_is_power_of_two_nonzero(values->tile_height));
   assert(util_is_power_of_two_nonzero(values->sample_count));
   cl[ 0] = util_bitpack_uint(values->base >> 7, 0, 32);
   cl[ 1] = util_bitpack_uint(values->base >> 7, 0, 32) >> 32 |
            util_bitpack_uint(util_logbase2(values->tile_width), 1, 3) |
            util_bitpack_uint(util_logbase2(values->tile_height), 4, 6) |
            util_bitpack_uint(values->tiles_per_row, 7, 21) |
            util_bitpack_uint(util_logbase2(values->sample_count), 22, 23);
   cl[ 2] = util_bitpack_uint(values->layer_stride_pixels, 0, 31);
   cl[ 3] = 0;
}


#define AGX_ATOMIC_SOFTWARE_LENGTH 16
struct agx_atomic_software_packed { uint32_t opaque[4]; };
static inline void
AGX_ATOMIC_SOFTWARE_unpack(FILE *fp, const uint8_t * restrict cl,
                           struct AGX_ATOMIC_SOFTWARE * restrict values)
{
   if (((const uint32_t *) cl)[1] & 0xff000000) fprintf(fp, "XXX: Unknown field of Atomic (software) unpacked at word 1: got %X, bad mask %X\n", ((const uint32_t *) cl)[1], ((const uint32_t *) cl)[1] & 0xff000000);
   if (((const uint32_t *) cl)[3] & 0xffffffff) fprintf(fp, "XXX: Unknown field of Atomic (software) unpacked at word 3: got %X, bad mask %X\n", ((const uint32_t *) cl)[3], ((const uint32_t *) cl)[3] & 0xffffffff);
   values->base = __gen_unpack_uint(cl, 0, 32) << 7;
   values->tile_width = 1 << __gen_unpack_uint(cl, 33, 35);
   values->tile_height = 1 << __gen_unpack_uint(cl, 36, 38);
   values->tiles_per_row = __gen_unpack_uint(cl, 39, 53);
   values->sample_count = 1 << __gen_unpack_uint(cl, 54, 55);
   values->layer_stride_pixels = __gen_unpack_uint(cl, 64, 95);
}

static inline void
AGX_ATOMIC_SOFTWARE_print(FILE *fp, const struct AGX_ATOMIC_SOFTWARE * values, unsigned indent)
{
   fprintf(fp, "%*sBase: 0x%" PRIx64 "\n", indent, "", values->base);
   fprintf(fp, "%*sTile width: %u\n", indent, "", values->tile_width);
   fprintf(fp, "%*sTile height: %u\n", indent, "", values->tile_height);
   fprintf(fp, "%*sTiles per row: %u\n", indent, "", values->tiles_per_row);
   fprintf(fp, "%*sSample count: %u\n", indent, "", values->sample_count);
   fprintf(fp, "%*sLayer stride (pixels): %u\n", indent, "", values->layer_stride_pixels);
}

struct AGX_PBE_BUFFER_SOFTWARE {
   uint64_t                             base;
};

#define AGX_PBE_BUFFER_SOFTWARE_header          \
   0

static inline void
AGX_PBE_BUFFER_SOFTWARE_pack(uint32_t * restrict cl,
                             const struct AGX_PBE_BUFFER_SOFTWARE * restrict values)
{
   cl[ 0] = util_bitpack_uint(values->base, 0, 39);
   cl[ 1] = util_bitpack_uint(values->base, 0, 39) >> 32;
}


#define AGX_PBE_BUFFER_SOFTWARE_LENGTH 8
struct agx_pbe_buffer_software_packed { uint32_t opaque[2]; };
static inline void
AGX_PBE_BUFFER_SOFTWARE_unpack(FILE *fp, const uint8_t * restrict cl,
                               struct AGX_PBE_BUFFER_SOFTWARE * restrict values)
{
   if (((const uint32_t *) cl)[1] & 0xffffff00) fprintf(fp, "XXX: Unknown field of PBE Buffer (software) unpacked at word 1: got %X, bad mask %X\n", ((const uint32_t *) cl)[1], ((const uint32_t *) cl)[1] & 0xffffff00);
   values->base = __gen_unpack_uint(cl, 0, 39);
}

static inline void
AGX_PBE_BUFFER_SOFTWARE_print(FILE *fp, const struct AGX_PBE_BUFFER_SOFTWARE * values, unsigned indent)
{
   fprintf(fp, "%*sBase: 0x%" PRIx64 "\n", indent, "", values->base);
}

struct AGX_PBE {
   enum agx_texture_dimension           dimension;
   enum agx_layout                      layout;
   enum agx_channels                    channels;
   enum agx_texture_type                type;
   enum agx_channel                     swizzle_r;
   enum agx_channel                     swizzle_g;
   enum agx_channel                     swizzle_b;
   enum agx_channel                     swizzle_a;
   uint32_t                             width;
   uint32_t                             height;
   bool                                 unk_52;
   bool                                 rotate_90;
   bool                                 flip_vertical;
   enum agx_sample_count                samples;
   bool                                 unk_mipmapped;
   bool                                 compressed_1;
   uint64_t                             buffer;
   uint32_t                             level;
   uint32_t                             levels;
   uint32_t                             layers;
   bool                                 page_aligned_layers;
   bool                                 srgb;
   bool                                 extended;
   uint32_t                             stride;
   uint64_t                             acceleration_buffer;
   uint32_t                             depth_linear;
   uint32_t                             layer_stride_linear;
   uint64_t                             software_defined;
};

#define AGX_PBE_header                          \
   .dimension = AGX_TEXTURE_DIMENSION_2D,  \
   .rotate_90 = false,  \
   .flip_vertical = false,  \
   .layers = 1,  \
   .depth_linear = 1

static inline void
AGX_PBE_pack(uint32_t * restrict cl,
             const struct AGX_PBE * restrict values)
{
   assert(values->width >= 1);
   assert(values->height >= 1);
   assert((values->buffer & 0xf) == 0);
   assert(values->levels >= 1);
   assert(values->layers >= 1);
   assert((values->acceleration_buffer & 0xf) == 0);
   assert(values->depth_linear >= 1);
   assert((values->layer_stride_linear & 0x7f) == 0);
   cl[ 0] = util_bitpack_uint(values->dimension, 0, 3) |
            util_bitpack_uint(values->layout, 4, 5) |
            util_bitpack_uint(values->channels, 6, 12) |
            util_bitpack_uint(values->type, 13, 15) |
            util_bitpack_uint(values->swizzle_r, 16, 17) |
            util_bitpack_uint(values->swizzle_g, 18, 19) |
            util_bitpack_uint(values->swizzle_b, 20, 21) |
            util_bitpack_uint(values->swizzle_a, 22, 23) |
            util_bitpack_uint(values->width - 1, 24, 37);
   cl[ 1] = util_bitpack_uint(values->width - 1, 24, 37) >> 32 |
            util_bitpack_uint(values->height - 1, 6, 19) |
            util_bitpack_uint(values->unk_52, 20, 20) |
            util_bitpack_uint(values->rotate_90, 21, 21) |
            util_bitpack_uint(values->flip_vertical, 22, 22) |
            util_bitpack_uint(values->samples, 24, 24) |
            util_bitpack_uint(values->unk_mipmapped, 26, 26) |
            util_bitpack_uint(values->compressed_1, 27, 27);
   cl[ 2] = util_bitpack_uint(values->buffer >> 4, 0, 35);
   cl[ 3] = util_bitpack_uint(values->buffer >> 4, 0, 35) >> 32 |
            util_bitpack_uint(values->level, 4, 7) |
            util_bitpack_uint(values->levels - 1, 8, 11) |
            util_bitpack_uint(values->layers - 1, 12, 25) |
            util_bitpack_uint(values->page_aligned_layers, 28, 28) |
            util_bitpack_uint(values->srgb, 29, 29) |
            util_bitpack_uint(values->extended, 31, 31) |
            util_bitpack_uint(values->stride, 8, 28);
   cl[ 4] = util_bitpack_uint(values->acceleration_buffer >> 4, 0, 63) |
            util_bitpack_uint(values->depth_linear - 1, 0, 10) |
            util_bitpack_uint(values->layer_stride_linear >> 7, 11, 37) |
            util_bitpack_uint(values->software_defined, 0, 63);
   cl[ 5] = util_bitpack_uint(values->acceleration_buffer >> 4, 0, 63) >> 32 |
            util_bitpack_uint(values->layer_stride_linear >> 7, 11, 37) >> 32 |
            util_bitpack_uint(values->software_defined, 0, 63) >> 32;
}


#define AGX_PBE_LENGTH 24
struct agx_pbe_packed { uint32_t opaque[6]; };
static inline void
AGX_PBE_unpack(FILE *fp, const uint8_t * restrict cl,
               struct AGX_PBE * restrict values)
{
   if (((const uint32_t *) cl)[1] & 0xf2800000) fprintf(fp, "XXX: Unknown field of PBE unpacked at word 1: got %X, bad mask %X\n", ((const uint32_t *) cl)[1], ((const uint32_t *) cl)[1] & 0xf2800000);
   if (((const uint32_t *) cl)[3] & 0x40000000) fprintf(fp, "XXX: Unknown field of PBE unpacked at word 3: got %X, bad mask %X\n", ((const uint32_t *) cl)[3], ((const uint32_t *) cl)[3] & 0x40000000);
   values->dimension = (enum agx_texture_dimension) __gen_unpack_uint(cl, 0, 3);
   values->layout = (enum agx_layout) __gen_unpack_uint(cl, 4, 5);
   values->channels = (enum agx_channels) __gen_unpack_uint(cl, 6, 12);
   values->type = (enum agx_texture_type) __gen_unpack_uint(cl, 13, 15);
   values->swizzle_r = (enum agx_channel) __gen_unpack_uint(cl, 16, 17);
   values->swizzle_g = (enum agx_channel) __gen_unpack_uint(cl, 18, 19);
   values->swizzle_b = (enum agx_channel) __gen_unpack_uint(cl, 20, 21);
   values->swizzle_a = (enum agx_channel) __gen_unpack_uint(cl, 22, 23);
   values->width = __gen_unpack_uint(cl, 24, 37) + 1;
   values->height = __gen_unpack_uint(cl, 38, 51) + 1;
   values->unk_52 = __gen_unpack_uint(cl, 52, 52);
   values->rotate_90 = __gen_unpack_uint(cl, 53, 53);
   values->flip_vertical = __gen_unpack_uint(cl, 54, 54);
   values->samples = (enum agx_sample_count) __gen_unpack_uint(cl, 56, 56);
   values->unk_mipmapped = __gen_unpack_uint(cl, 58, 58);
   values->compressed_1 = __gen_unpack_uint(cl, 59, 59);
   values->buffer = __gen_unpack_uint(cl, 64, 99) << 4;
   values->level = __gen_unpack_uint(cl, 100, 103);
   values->levels = __gen_unpack_uint(cl, 104, 107) + 1;
   values->layers = __gen_unpack_uint(cl, 108, 121) + 1;
   values->page_aligned_layers = __gen_unpack_uint(cl, 124, 124);
   values->srgb = __gen_unpack_uint(cl, 125, 125);
   values->extended = __gen_unpack_uint(cl, 127, 127);
   values->stride = __gen_unpack_uint(cl, 104, 124);
   values->acceleration_buffer = __gen_unpack_uint(cl, 128, 191) << 4;
   values->depth_linear = __gen_unpack_uint(cl, 128, 138) + 1;
   values->layer_stride_linear = __gen_unpack_uint(cl, 139, 165) << 7;
   values->software_defined = __gen_unpack_uint(cl, 128, 191);
}

static inline void
AGX_PBE_print(FILE *fp, const struct AGX_PBE * values, unsigned indent)
{
   if (agx_texture_dimension_as_str(values->dimension))
     fprintf(fp, "%*sDimension: %s\n", indent, "", agx_texture_dimension_as_str(values->dimension));
    else
     fprintf(fp, "%*sDimension: unknown %X (XXX)\n", indent, "", values->dimension);
   if (agx_layout_as_str(values->layout))
     fprintf(fp, "%*sLayout: %s\n", indent, "", agx_layout_as_str(values->layout));
    else
     fprintf(fp, "%*sLayout: unknown %X (XXX)\n", indent, "", values->layout);
   if (agx_channels_as_str(values->channels))
     fprintf(fp, "%*sChannels: %s\n", indent, "", agx_channels_as_str(values->channels));
    else
     fprintf(fp, "%*sChannels: unknown %X (XXX)\n", indent, "", values->channels);
   if (agx_texture_type_as_str(values->type))
     fprintf(fp, "%*sType: %s\n", indent, "", agx_texture_type_as_str(values->type));
    else
     fprintf(fp, "%*sType: unknown %X (XXX)\n", indent, "", values->type);
   if (agx_channel_as_str(values->swizzle_r))
     fprintf(fp, "%*sSwizzle R: %s\n", indent, "", agx_channel_as_str(values->swizzle_r));
    else
     fprintf(fp, "%*sSwizzle R: unknown %X (XXX)\n", indent, "", values->swizzle_r);
   if (agx_channel_as_str(values->swizzle_g))
     fprintf(fp, "%*sSwizzle G: %s\n", indent, "", agx_channel_as_str(values->swizzle_g));
    else
     fprintf(fp, "%*sSwizzle G: unknown %X (XXX)\n", indent, "", values->swizzle_g);
   if (agx_channel_as_str(values->swizzle_b))
     fprintf(fp, "%*sSwizzle B: %s\n", indent, "", agx_channel_as_str(values->swizzle_b));
    else
     fprintf(fp, "%*sSwizzle B: unknown %X (XXX)\n", indent, "", values->swizzle_b);
   if (agx_channel_as_str(values->swizzle_a))
     fprintf(fp, "%*sSwizzle A: %s\n", indent, "", agx_channel_as_str(values->swizzle_a));
    else
     fprintf(fp, "%*sSwizzle A: unknown %X (XXX)\n", indent, "", values->swizzle_a);
   fprintf(fp, "%*sWidth: %u\n", indent, "", values->width);
   fprintf(fp, "%*sHeight: %u\n", indent, "", values->height);
   fprintf(fp, "%*sUnk 52: %s\n", indent, "", values->unk_52 ? "true" : "false");
   fprintf(fp, "%*sRotate 90: %s\n", indent, "", values->rotate_90 ? "true" : "false");
   fprintf(fp, "%*sFlip vertical: %s\n", indent, "", values->flip_vertical ? "true" : "false");
   if (agx_sample_count_as_str(values->samples))
     fprintf(fp, "%*sSamples: %s\n", indent, "", agx_sample_count_as_str(values->samples));
    else
     fprintf(fp, "%*sSamples: unknown %X (XXX)\n", indent, "", values->samples);
   fprintf(fp, "%*sUnk mipmapped: %s\n", indent, "", values->unk_mipmapped ? "true" : "false");
   fprintf(fp, "%*sCompressed 1: %s\n", indent, "", values->compressed_1 ? "true" : "false");
   fprintf(fp, "%*sBuffer: 0x%" PRIx64 "\n", indent, "", values->buffer);
   fprintf(fp, "%*sLevel: %u\n", indent, "", values->level);
   fprintf(fp, "%*sLevels: %u\n", indent, "", values->levels);
   fprintf(fp, "%*sLayers: %u\n", indent, "", values->layers);
   fprintf(fp, "%*sPage-aligned layers: %s\n", indent, "", values->page_aligned_layers ? "true" : "false");
   fprintf(fp, "%*ssRGB: %s\n", indent, "", values->srgb ? "true" : "false");
   fprintf(fp, "%*sExtended: %s\n", indent, "", values->extended ? "true" : "false");
   fprintf(fp, "%*sStride: 0x%" PRIx32 "\n", indent, "", values->stride);
   fprintf(fp, "%*sAcceleration buffer: 0x%" PRIx64 "\n", indent, "", values->acceleration_buffer);
   fprintf(fp, "%*sDepth (linear): %u\n", indent, "", values->depth_linear);
   fprintf(fp, "%*sLayer stride (linear): %u\n", indent, "", values->layer_stride_linear);
   fprintf(fp, "%*sSoftware-defined: 0x%" PRIx64 "\n", indent, "", values->software_defined);
}

struct AGX_TEXTURE {
   enum agx_texture_dimension           dimension;
   enum agx_layout                      layout;
   enum agx_channels                    channels;
   enum agx_texture_type                type;
   enum agx_channel                     swizzle_r;
   enum agx_channel                     swizzle_g;
   enum agx_channel                     swizzle_b;
   enum agx_channel                     swizzle_a;
   uint32_t                             width;
   uint32_t                             height;
   uint32_t                             first_level;
   uint32_t                             last_level;
   enum agx_sample_count                samples;
   uint64_t                             address;
   bool                                 unk_mipmapped;
   bool                                 compressed_1;
   bool                                 null;
   uint32_t                             compression;
   bool                                 srgb;
   bool                                 srgb_2_channel;
   uint32_t                             stride;
   uint32_t                             depth;
   bool                                 page_aligned_layers;
   bool                                 extended;
   uint64_t                             acceleration_buffer;
   uint32_t                             depth_linear;
   uint32_t                             layer_stride_linear;
};

#define AGX_TEXTURE_header                      \
   .width = 1,  \
   .height = 1,  \
   .depth = 1,  \
   .depth_linear = 1

static inline void
AGX_TEXTURE_pack(uint32_t * restrict cl,
                 const struct AGX_TEXTURE * restrict values)
{
   assert(values->width >= 1);
   assert(values->height >= 1);
   assert((values->address & 0xf) == 0);
   assert((values->stride & 0xf) == 0);
   assert(values->depth >= 1);
   assert((values->acceleration_buffer & 0xf) == 0);
   assert(values->depth_linear >= 1);
   assert((values->layer_stride_linear & 0x7f) == 0);
   cl[ 0] = util_bitpack_uint(values->dimension, 0, 3) |
            util_bitpack_uint(values->layout, 4, 5) |
            util_bitpack_uint(values->channels, 6, 12) |
            util_bitpack_uint(values->type, 13, 15) |
            util_bitpack_uint(values->swizzle_r, 16, 18) |
            util_bitpack_uint(values->swizzle_g, 19, 21) |
            util_bitpack_uint(values->swizzle_b, 22, 24) |
            util_bitpack_uint(values->swizzle_a, 25, 27) |
            util_bitpack_uint(values->width - 1, 28, 41);
   cl[ 1] = util_bitpack_uint(values->width - 1, 28, 41) >> 32 |
            util_bitpack_uint(values->height - 1, 10, 23) |
            util_bitpack_uint(values->first_level, 24, 27) |
            util_bitpack_uint(values->last_level, 28, 31);
   cl[ 2] = util_bitpack_uint(values->samples, 0, 0) |
            util_bitpack_uint(values->address >> 4, 2, 37);
   cl[ 3] = util_bitpack_uint(values->address >> 4, 2, 37) >> 32 |
            util_bitpack_uint(values->unk_mipmapped, 6, 6) |
            util_bitpack_uint(values->compressed_1, 7, 7) |
            util_bitpack_uint(values->null, 9, 9) |
            util_bitpack_uint(values->compression, 10, 11) |
            util_bitpack_uint(values->srgb, 12, 12) |
            util_bitpack_uint(values->srgb_2_channel, 13, 13) |
            util_bitpack_uint(values->stride >> 4, 14, 31) |
            util_bitpack_uint(values->depth - 1, 14, 27) |
            util_bitpack_uint(values->page_aligned_layers, 30, 30) |
            util_bitpack_uint(values->extended, 31, 31);
   cl[ 4] = util_bitpack_uint(values->acceleration_buffer >> 4, 0, 63) |
            util_bitpack_uint(values->depth_linear - 1, 0, 10) |
            util_bitpack_uint(values->layer_stride_linear >> 7, 11, 37);
   cl[ 5] = util_bitpack_uint(values->acceleration_buffer >> 4, 0, 63) >> 32 |
            util_bitpack_uint(values->layer_stride_linear >> 7, 11, 37) >> 32;
}


#define AGX_TEXTURE_LENGTH 24
struct agx_texture_packed { uint32_t opaque[6]; };
static inline void
AGX_TEXTURE_unpack(FILE *fp, const uint8_t * restrict cl,
                   struct AGX_TEXTURE * restrict values)
{
   if (((const uint32_t *) cl)[2] & 0x2) fprintf(fp, "XXX: Unknown field of Texture unpacked at word 2: got %X, bad mask %X\n", ((const uint32_t *) cl)[2], ((const uint32_t *) cl)[2] & 0x2);
   if (((const uint32_t *) cl)[3] & 0x100) fprintf(fp, "XXX: Unknown field of Texture unpacked at word 3: got %X, bad mask %X\n", ((const uint32_t *) cl)[3], ((const uint32_t *) cl)[3] & 0x100);
   values->dimension = (enum agx_texture_dimension) __gen_unpack_uint(cl, 0, 3);
   values->layout = (enum agx_layout) __gen_unpack_uint(cl, 4, 5);
   values->channels = (enum agx_channels) __gen_unpack_uint(cl, 6, 12);
   values->type = (enum agx_texture_type) __gen_unpack_uint(cl, 13, 15);
   values->swizzle_r = (enum agx_channel) __gen_unpack_uint(cl, 16, 18);
   values->swizzle_g = (enum agx_channel) __gen_unpack_uint(cl, 19, 21);
   values->swizzle_b = (enum agx_channel) __gen_unpack_uint(cl, 22, 24);
   values->swizzle_a = (enum agx_channel) __gen_unpack_uint(cl, 25, 27);
   values->width = __gen_unpack_uint(cl, 28, 41) + 1;
   values->height = __gen_unpack_uint(cl, 42, 55) + 1;
   values->first_level = __gen_unpack_uint(cl, 56, 59);
   values->last_level = __gen_unpack_uint(cl, 60, 63);
   values->samples = (enum agx_sample_count) __gen_unpack_uint(cl, 64, 64);
   values->address = __gen_unpack_uint(cl, 66, 101) << 4;
   values->unk_mipmapped = __gen_unpack_uint(cl, 102, 102);
   values->compressed_1 = __gen_unpack_uint(cl, 103, 103);
   values->null = __gen_unpack_uint(cl, 105, 105);
   values->compression = __gen_unpack_uint(cl, 106, 107);
   values->srgb = __gen_unpack_uint(cl, 108, 108);
   values->srgb_2_channel = __gen_unpack_uint(cl, 109, 109);
   values->stride = __gen_unpack_uint(cl, 110, 127) << 4;
   values->depth = __gen_unpack_uint(cl, 110, 123) + 1;
   values->page_aligned_layers = __gen_unpack_uint(cl, 126, 126);
   values->extended = __gen_unpack_uint(cl, 127, 127);
   values->acceleration_buffer = __gen_unpack_uint(cl, 128, 191) << 4;
   values->depth_linear = __gen_unpack_uint(cl, 128, 138) + 1;
   values->layer_stride_linear = __gen_unpack_uint(cl, 139, 165) << 7;
}

static inline void
AGX_TEXTURE_print(FILE *fp, const struct AGX_TEXTURE * values, unsigned indent)
{
   if (agx_texture_dimension_as_str(values->dimension))
     fprintf(fp, "%*sDimension: %s\n", indent, "", agx_texture_dimension_as_str(values->dimension));
    else
     fprintf(fp, "%*sDimension: unknown %X (XXX)\n", indent, "", values->dimension);
   if (agx_layout_as_str(values->layout))
     fprintf(fp, "%*sLayout: %s\n", indent, "", agx_layout_as_str(values->layout));
    else
     fprintf(fp, "%*sLayout: unknown %X (XXX)\n", indent, "", values->layout);
   if (agx_channels_as_str(values->channels))
     fprintf(fp, "%*sChannels: %s\n", indent, "", agx_channels_as_str(values->channels));
    else
     fprintf(fp, "%*sChannels: unknown %X (XXX)\n", indent, "", values->channels);
   if (agx_texture_type_as_str(values->type))
     fprintf(fp, "%*sType: %s\n", indent, "", agx_texture_type_as_str(values->type));
    else
     fprintf(fp, "%*sType: unknown %X (XXX)\n", indent, "", values->type);
   if (agx_channel_as_str(values->swizzle_r))
     fprintf(fp, "%*sSwizzle R: %s\n", indent, "", agx_channel_as_str(values->swizzle_r));
    else
     fprintf(fp, "%*sSwizzle R: unknown %X (XXX)\n", indent, "", values->swizzle_r);
   if (agx_channel_as_str(values->swizzle_g))
     fprintf(fp, "%*sSwizzle G: %s\n", indent, "", agx_channel_as_str(values->swizzle_g));
    else
     fprintf(fp, "%*sSwizzle G: unknown %X (XXX)\n", indent, "", values->swizzle_g);
   if (agx_channel_as_str(values->swizzle_b))
     fprintf(fp, "%*sSwizzle B: %s\n", indent, "", agx_channel_as_str(values->swizzle_b));
    else
     fprintf(fp, "%*sSwizzle B: unknown %X (XXX)\n", indent, "", values->swizzle_b);
   if (agx_channel_as_str(values->swizzle_a))
     fprintf(fp, "%*sSwizzle A: %s\n", indent, "", agx_channel_as_str(values->swizzle_a));
    else
     fprintf(fp, "%*sSwizzle A: unknown %X (XXX)\n", indent, "", values->swizzle_a);
   fprintf(fp, "%*sWidth: %u\n", indent, "", values->width);
   fprintf(fp, "%*sHeight: %u\n", indent, "", values->height);
   fprintf(fp, "%*sFirst level: %u\n", indent, "", values->first_level);
   fprintf(fp, "%*sLast level: %u\n", indent, "", values->last_level);
   if (agx_sample_count_as_str(values->samples))
     fprintf(fp, "%*sSamples: %s\n", indent, "", agx_sample_count_as_str(values->samples));
    else
     fprintf(fp, "%*sSamples: unknown %X (XXX)\n", indent, "", values->samples);
   fprintf(fp, "%*sAddress: 0x%" PRIx64 "\n", indent, "", values->address);
   fprintf(fp, "%*sUnk mipmapped: %s\n", indent, "", values->unk_mipmapped ? "true" : "false");
   fprintf(fp, "%*sCompressed 1: %s\n", indent, "", values->compressed_1 ? "true" : "false");
   fprintf(fp, "%*sNull: %s\n", indent, "", values->null ? "true" : "false");
   fprintf(fp, "%*sCompression: 0x%" PRIx32 "\n", indent, "", values->compression);
   fprintf(fp, "%*ssRGB: %s\n", indent, "", values->srgb ? "true" : "false");
   fprintf(fp, "%*ssRGB 2-channel: %s\n", indent, "", values->srgb_2_channel ? "true" : "false");
   fprintf(fp, "%*sStride: 0x%" PRIx32 "\n", indent, "", values->stride);
   fprintf(fp, "%*sDepth: %u\n", indent, "", values->depth);
   fprintf(fp, "%*sPage-aligned layers: %s\n", indent, "", values->page_aligned_layers ? "true" : "false");
   fprintf(fp, "%*sExtended: %s\n", indent, "", values->extended ? "true" : "false");
   fprintf(fp, "%*sAcceleration buffer: 0x%" PRIx64 "\n", indent, "", values->acceleration_buffer);
   fprintf(fp, "%*sDepth (linear): %u\n", indent, "", values->depth_linear);
   fprintf(fp, "%*sLayer stride (linear): %u\n", indent, "", values->layer_stride_linear);
}

enum agx_wrap {
        AGX_WRAP_CLAMP_TO_EDGE               =      0,
        AGX_WRAP_REPEAT                      =      1,
        AGX_WRAP_MIRRORED_REPEAT             =      2,
        AGX_WRAP_CLAMP_TO_BORDER             =      3,
        AGX_WRAP_CLAMP_GL                    =      4,
        AGX_WRAP_MIRRORED_CLAMP_TO_EDGE      =      5,
};

static inline const char *
agx_wrap_as_str(enum agx_wrap imm)
{
    switch (imm) {
    case AGX_WRAP_CLAMP_TO_EDGE: return "Clamp to edge";
    case AGX_WRAP_REPEAT: return "Repeat";
    case AGX_WRAP_MIRRORED_REPEAT: return "Mirrored repeat";
    case AGX_WRAP_CLAMP_TO_BORDER: return "Clamp to border";
    case AGX_WRAP_CLAMP_GL: return "Clamp (GL)";
    case AGX_WRAP_MIRRORED_CLAMP_TO_EDGE: return "Mirrored clamp to edge";
    default: break;
    }
    return NULL;
}

enum agx_mip_filter {
        AGX_MIP_FILTER_NONE                  =      0,
        AGX_MIP_FILTER_NEAREST               =      1,
        AGX_MIP_FILTER_LINEAR                =      2,
};

static inline const char *
agx_mip_filter_as_str(enum agx_mip_filter imm)
{
    switch (imm) {
    case AGX_MIP_FILTER_NONE: return "None";
    case AGX_MIP_FILTER_NEAREST: return "Nearest";
    case AGX_MIP_FILTER_LINEAR: return "Linear";
    default: break;
    }
    return NULL;
}

enum agx_border_colour {
        AGX_BORDER_COLOUR_TRANSPARENT_BLACK  =      0,
        AGX_BORDER_COLOUR_OPAQUE_BLACK       =      1,
        AGX_BORDER_COLOUR_OPAQUE_WHITE       =      2,
        AGX_BORDER_COLOUR_CUSTOM             =      3,
};

static inline const char *
agx_border_colour_as_str(enum agx_border_colour imm)
{
    switch (imm) {
    case AGX_BORDER_COLOUR_TRANSPARENT_BLACK: return "Transparent black";
    case AGX_BORDER_COLOUR_OPAQUE_BLACK: return "Opaque black";
    case AGX_BORDER_COLOUR_OPAQUE_WHITE: return "Opaque white";
    case AGX_BORDER_COLOUR_CUSTOM: return "Custom";
    default: break;
    }
    return NULL;
}

struct AGX_SAMPLER {
   float                                minimum_lod;
   float                                maximum_lod;
   uint32_t                             maximum_anisotropy;
   bool                                 magnify_linear;
   bool                                 minify_linear;
   enum agx_mip_filter                  mip_filter;
   enum agx_wrap                        wrap_s;
   enum agx_wrap                        wrap_t;
   enum agx_wrap                        wrap_r;
   bool                                 pixel_coordinates;
   enum agx_compare_func                compare_func;
   bool                                 compare_enable;
   enum agx_border_colour               border_colour;
   bool                                 seamful_cube_maps;
};

#define AGX_SAMPLER_header                      \
   .minimum_lod = 0.0,  \
   .maximum_lod = INFINITY,  \
   .maximum_anisotropy = 1

static inline void
AGX_SAMPLER_pack(uint32_t * restrict cl,
                 const struct AGX_SAMPLER * restrict values)
{
   assert(util_is_power_of_two_nonzero(values->maximum_anisotropy));
   cl[ 0] = __gen_pack_lod(values->minimum_lod, 0, 9) |
            __gen_pack_lod(values->maximum_lod, 10, 19) |
            util_bitpack_uint(util_logbase2(values->maximum_anisotropy), 20, 22) |
            util_bitpack_uint(values->magnify_linear, 23, 23) |
            util_bitpack_uint(values->minify_linear, 25, 25) |
            util_bitpack_uint(values->mip_filter, 27, 28) |
            util_bitpack_uint(values->wrap_s, 29, 31);
   cl[ 1] = util_bitpack_uint(values->wrap_t, 0, 2) |
            util_bitpack_uint(values->wrap_r, 3, 5) |
            util_bitpack_uint(values->pixel_coordinates, 6, 6) |
            util_bitpack_uint(values->compare_func, 7, 9) |
            util_bitpack_uint(values->compare_enable, 10, 10) |
            util_bitpack_uint(values->border_colour, 23, 24) |
            util_bitpack_uint(values->seamful_cube_maps, 25, 25);
}


#define AGX_SAMPLER_LENGTH 8
struct agx_sampler_packed { uint32_t opaque[2]; };
static inline void
AGX_SAMPLER_unpack(FILE *fp, const uint8_t * restrict cl,
                   struct AGX_SAMPLER * restrict values)
{
   if (((const uint32_t *) cl)[0] & 0x5000000) fprintf(fp, "XXX: Unknown field of Sampler unpacked at word 0: got %X, bad mask %X\n", ((const uint32_t *) cl)[0], ((const uint32_t *) cl)[0] & 0x5000000);
   if (((const uint32_t *) cl)[1] & 0xfc7ff800) fprintf(fp, "XXX: Unknown field of Sampler unpacked at word 1: got %X, bad mask %X\n", ((const uint32_t *) cl)[1], ((const uint32_t *) cl)[1] & 0xfc7ff800);
   values->minimum_lod = __gen_unpack_lod(cl, 0, 9);
   values->maximum_lod = __gen_unpack_lod(cl, 10, 19);
   values->maximum_anisotropy = 1 << __gen_unpack_uint(cl, 20, 22);
   values->magnify_linear = __gen_unpack_uint(cl, 23, 23);
   values->minify_linear = __gen_unpack_uint(cl, 25, 25);
   values->mip_filter = (enum agx_mip_filter) __gen_unpack_uint(cl, 27, 28);
   values->wrap_s = (enum agx_wrap) __gen_unpack_uint(cl, 29, 31);
   values->wrap_t = (enum agx_wrap) __gen_unpack_uint(cl, 32, 34);
   values->wrap_r = (enum agx_wrap) __gen_unpack_uint(cl, 35, 37);
   values->pixel_coordinates = __gen_unpack_uint(cl, 38, 38);
   values->compare_func = (enum agx_compare_func) __gen_unpack_uint(cl, 39, 41);
   values->compare_enable = __gen_unpack_uint(cl, 42, 42);
   values->border_colour = (enum agx_border_colour) __gen_unpack_uint(cl, 55, 56);
   values->seamful_cube_maps = __gen_unpack_uint(cl, 57, 57);
}

static inline void
AGX_SAMPLER_print(FILE *fp, const struct AGX_SAMPLER * values, unsigned indent)
{
   fprintf(fp, "%*sMinimum LOD: %f\n", indent, "", values->minimum_lod);
   fprintf(fp, "%*sMaximum LOD: %f\n", indent, "", values->maximum_lod);
   fprintf(fp, "%*sMaximum anisotropy: %u\n", indent, "", values->maximum_anisotropy);
   fprintf(fp, "%*sMagnify linear: %s\n", indent, "", values->magnify_linear ? "true" : "false");
   fprintf(fp, "%*sMinify linear: %s\n", indent, "", values->minify_linear ? "true" : "false");
   if (agx_mip_filter_as_str(values->mip_filter))
     fprintf(fp, "%*sMip filter: %s\n", indent, "", agx_mip_filter_as_str(values->mip_filter));
    else
     fprintf(fp, "%*sMip filter: unknown %X (XXX)\n", indent, "", values->mip_filter);
   if (agx_wrap_as_str(values->wrap_s))
     fprintf(fp, "%*sWrap S: %s\n", indent, "", agx_wrap_as_str(values->wrap_s));
    else
     fprintf(fp, "%*sWrap S: unknown %X (XXX)\n", indent, "", values->wrap_s);
   if (agx_wrap_as_str(values->wrap_t))
     fprintf(fp, "%*sWrap T: %s\n", indent, "", agx_wrap_as_str(values->wrap_t));
    else
     fprintf(fp, "%*sWrap T: unknown %X (XXX)\n", indent, "", values->wrap_t);
   if (agx_wrap_as_str(values->wrap_r))
     fprintf(fp, "%*sWrap R: %s\n", indent, "", agx_wrap_as_str(values->wrap_r));
    else
     fprintf(fp, "%*sWrap R: unknown %X (XXX)\n", indent, "", values->wrap_r);
   fprintf(fp, "%*sPixel coordinates: %s\n", indent, "", values->pixel_coordinates ? "true" : "false");
   if (agx_compare_func_as_str(values->compare_func))
     fprintf(fp, "%*sCompare func: %s\n", indent, "", agx_compare_func_as_str(values->compare_func));
    else
     fprintf(fp, "%*sCompare func: unknown %X (XXX)\n", indent, "", values->compare_func);
   fprintf(fp, "%*sCompare enable: %s\n", indent, "", values->compare_enable ? "true" : "false");
   if (agx_border_colour_as_str(values->border_colour))
     fprintf(fp, "%*sBorder colour: %s\n", indent, "", agx_border_colour_as_str(values->border_colour));
    else
     fprintf(fp, "%*sBorder colour: unknown %X (XXX)\n", indent, "", values->border_colour);
   fprintf(fp, "%*sSeamful cube maps: %s\n", indent, "", values->seamful_cube_maps ? "true" : "false");
}

struct AGX_BORDER {
   uint32_t                             channel_0;
   uint32_t                             channel_1;
   uint32_t                             channel_2;
   uint32_t                             channel_3;
};

#define AGX_BORDER_header                       \
   0

static inline void
AGX_BORDER_pack(uint32_t * restrict cl,
                const struct AGX_BORDER * restrict values)
{
   cl[ 0] = util_bitpack_uint(values->channel_0, 0, 31);
   cl[ 1] = util_bitpack_uint(values->channel_1, 0, 31);
   cl[ 2] = util_bitpack_uint(values->channel_2, 0, 31);
   cl[ 3] = util_bitpack_uint(values->channel_3, 0, 31);
}


#define AGX_BORDER_LENGTH 16
struct agx_border_packed { uint32_t opaque[4]; };
static inline void
AGX_BORDER_unpack(FILE *fp, const uint8_t * restrict cl,
                  struct AGX_BORDER * restrict values)
{
   values->channel_0 = __gen_unpack_uint(cl, 0, 31);
   values->channel_1 = __gen_unpack_uint(cl, 32, 63);
   values->channel_2 = __gen_unpack_uint(cl, 64, 95);
   values->channel_3 = __gen_unpack_uint(cl, 96, 127);
}

static inline void
AGX_BORDER_print(FILE *fp, const struct AGX_BORDER * values, unsigned indent)
{
   fprintf(fp, "%*sChannel 0: 0x%" PRIx32 "\n", indent, "", values->channel_0);
   fprintf(fp, "%*sChannel 1: 0x%" PRIx32 "\n", indent, "", values->channel_1);
   fprintf(fp, "%*sChannel 2: 0x%" PRIx32 "\n", indent, "", values->channel_2);
   fprintf(fp, "%*sChannel 3: 0x%" PRIx32 "\n", indent, "", values->channel_3);
}

struct AGX_SCISSOR {
   uint32_t                             max_x;
   uint32_t                             min_x;
   uint32_t                             max_y;
   uint32_t                             min_y;
   float                                min_z;
   float                                max_z;
};

#define AGX_SCISSOR_header                      \
   0

static inline void
AGX_SCISSOR_pack(uint32_t * restrict cl,
                 const struct AGX_SCISSOR * restrict values)
{
   cl[ 0] = util_bitpack_uint(values->max_x, 0, 15) |
            util_bitpack_uint(values->min_x, 16, 31);
   cl[ 1] = util_bitpack_uint(values->max_y, 0, 15) |
            util_bitpack_uint(values->min_y, 16, 31);
   cl[ 2] = util_bitpack_float(values->min_z);
   cl[ 3] = util_bitpack_float(values->max_z);
}


#define AGX_SCISSOR_LENGTH 16
struct agx_scissor_packed { uint32_t opaque[4]; };
static inline void
AGX_SCISSOR_unpack(FILE *fp, const uint8_t * restrict cl,
                   struct AGX_SCISSOR * restrict values)
{
   values->max_x = __gen_unpack_uint(cl, 0, 15);
   values->min_x = __gen_unpack_uint(cl, 16, 31);
   values->max_y = __gen_unpack_uint(cl, 32, 47);
   values->min_y = __gen_unpack_uint(cl, 48, 63);
   values->min_z = __gen_unpack_float(cl, 64, 95);
   values->max_z = __gen_unpack_float(cl, 96, 127);
}

static inline void
AGX_SCISSOR_print(FILE *fp, const struct AGX_SCISSOR * values, unsigned indent)
{
   fprintf(fp, "%*sMax X: %u\n", indent, "", values->max_x);
   fprintf(fp, "%*sMin X: %u\n", indent, "", values->min_x);
   fprintf(fp, "%*sMax Y: %u\n", indent, "", values->max_y);
   fprintf(fp, "%*sMin Y: %u\n", indent, "", values->min_y);
   fprintf(fp, "%*sMin Z: %f\n", indent, "", values->min_z);
   fprintf(fp, "%*sMax Z: %f\n", indent, "", values->max_z);
}

struct AGX_DEPTH_BIAS {
   float                                depth_bias;
   float                                slope_scale;
   float                                clamp;
};

#define AGX_DEPTH_BIAS_header                   \
   0

static inline void
AGX_DEPTH_BIAS_pack(uint32_t * restrict cl,
                    const struct AGX_DEPTH_BIAS * restrict values)
{
   cl[ 0] = util_bitpack_float(values->depth_bias);
   cl[ 1] = util_bitpack_float(values->slope_scale);
   cl[ 2] = util_bitpack_float(values->clamp);
}


#define AGX_DEPTH_BIAS_LENGTH 12
struct agx_depth_bias_packed { uint32_t opaque[3]; };
static inline void
AGX_DEPTH_BIAS_unpack(FILE *fp, const uint8_t * restrict cl,
                      struct AGX_DEPTH_BIAS * restrict values)
{
   values->depth_bias = __gen_unpack_float(cl, 0, 31);
   values->slope_scale = __gen_unpack_float(cl, 32, 63);
   values->clamp = __gen_unpack_float(cl, 64, 95);
}

static inline void
AGX_DEPTH_BIAS_print(FILE *fp, const struct AGX_DEPTH_BIAS * values, unsigned indent)
{
   fprintf(fp, "%*sDepth bias: %f\n", indent, "", values->depth_bias);
   fprintf(fp, "%*sSlope scale: %f\n", indent, "", values->slope_scale);
   fprintf(fp, "%*sClamp: %f\n", indent, "", values->clamp);
}

enum agx_sampler_states {
        AGX_SAMPLER_STATES_0                 =      0,
        AGX_SAMPLER_STATES_4_COMPACT         =      1,
        AGX_SAMPLER_STATES_8_COMPACT         =      2,
        AGX_SAMPLER_STATES_12_COMPACT        =      3,
        AGX_SAMPLER_STATES_16_COMPACT        =      4,
        AGX_SAMPLER_STATES_8_EXTENDED        =      6,
        AGX_SAMPLER_STATES_16_EXTENDED       =      7,
};

static inline const char *
agx_sampler_states_as_str(enum agx_sampler_states imm)
{
    switch (imm) {
    case AGX_SAMPLER_STATES_0: return "0";
    case AGX_SAMPLER_STATES_4_COMPACT: return "4 compact";
    case AGX_SAMPLER_STATES_8_COMPACT: return "8 compact";
    case AGX_SAMPLER_STATES_12_COMPACT: return "12 compact";
    case AGX_SAMPLER_STATES_16_COMPACT: return "16 compact";
    case AGX_SAMPLER_STATES_8_EXTENDED: return "8 extended";
    case AGX_SAMPLER_STATES_16_EXTENDED: return "16 extended";
    default: break;
    }
    return NULL;
}

struct AGX_COUNTS {
   uint32_t                             unknown_0;
   uint32_t                             uniforms;
   uint32_t                             texture_states;
   enum agx_sampler_states              sampler_states;
   uint32_t                             preshader_gprs;
   uint32_t                             unknown;
};

#define AGX_COUNTS_header                       \
   .unknown_0 = 0

static inline void
AGX_COUNTS_pack(uint32_t * restrict cl,
                const struct AGX_COUNTS * restrict values)
{
   cl[ 0] = util_bitpack_uint(values->unknown_0, 0, 0) |
            util_bitpack_uint(__gen_to_groups(values->uniforms, 64, 3), 1, 3) |
            util_bitpack_uint(__gen_to_groups(values->texture_states, 8, 5), 4, 8) |
            util_bitpack_uint(values->sampler_states, 9, 11) |
            util_bitpack_uint(__gen_to_groups(values->preshader_gprs, 16, 4), 12, 15) |
            util_bitpack_uint(values->unknown, 16, 31);
}


#define AGX_COUNTS_LENGTH 4
struct agx_counts_packed { uint32_t opaque[1]; };
static inline void
AGX_COUNTS_unpack(FILE *fp, const uint8_t * restrict cl,
                  struct AGX_COUNTS * restrict values)
{
   values->unknown_0 = __gen_unpack_uint(cl, 0, 0);
   values->uniforms = __gen_from_groups(__gen_unpack_uint(cl, 1, 3), 64, 3);
   values->texture_states = __gen_from_groups(__gen_unpack_uint(cl, 4, 8), 8, 5);
   values->sampler_states = (enum agx_sampler_states) __gen_unpack_uint(cl, 9, 11);
   values->preshader_gprs = __gen_from_groups(__gen_unpack_uint(cl, 12, 15), 16, 4);
   values->unknown = __gen_unpack_uint(cl, 16, 31);
}

static inline void
AGX_COUNTS_print(FILE *fp, const struct AGX_COUNTS * values, unsigned indent)
{
   fprintf(fp, "%*sUnknown 0: 0x%" PRIx32 "\n", indent, "", values->unknown_0);
   fprintf(fp, "%*sUniforms: %u\n", indent, "", values->uniforms);
   fprintf(fp, "%*sTexture states: %u\n", indent, "", values->texture_states);
   if (agx_sampler_states_as_str(values->sampler_states))
     fprintf(fp, "%*sSampler states: %s\n", indent, "", agx_sampler_states_as_str(values->sampler_states));
    else
     fprintf(fp, "%*sSampler states: unknown %X (XXX)\n", indent, "", values->sampler_states);
   fprintf(fp, "%*sPreshader GPRs: %u\n", indent, "", values->preshader_gprs);
   fprintf(fp, "%*sUnknown: 0x%" PRIx32 "\n", indent, "", values->unknown);
}

struct AGX_PPP_HEADER {
   bool                                 fragment_control;
   bool                                 fragment_control_2;
   bool                                 fragment_front_face;
   bool                                 fragment_front_face_2;
   bool                                 fragment_front_stencil;
   bool                                 fragment_back_face;
   bool                                 fragment_back_face_2;
   bool                                 fragment_back_stencil;
   bool                                 depth_bias_scissor;
   bool                                 region_clip;
   bool                                 viewport;
   bool                                 w_clamp;
   bool                                 output_select;
   bool                                 varying_counts_32;
   bool                                 varying_counts_16;
   bool                                 cull;
   bool                                 cull_2;
   bool                                 fragment_shader;
   bool                                 occlusion_query;
   bool                                 occlusion_query_2;
   bool                                 output_unknown;
   bool                                 output_size;
   bool                                 varying_word_2;
};

#define AGX_PPP_HEADER_header                   \
   0

static inline void
AGX_PPP_HEADER_pack(uint32_t * restrict cl,
                    const struct AGX_PPP_HEADER * restrict values)
{
   cl[ 0] = util_bitpack_uint(values->fragment_control, 0, 0) |
            util_bitpack_uint(values->fragment_control_2, 1, 1) |
            util_bitpack_uint(values->fragment_front_face, 2, 2) |
            util_bitpack_uint(values->fragment_front_face_2, 3, 3) |
            util_bitpack_uint(values->fragment_front_stencil, 4, 4) |
            util_bitpack_uint(values->fragment_back_face, 5, 5) |
            util_bitpack_uint(values->fragment_back_face_2, 6, 6) |
            util_bitpack_uint(values->fragment_back_stencil, 7, 7) |
            util_bitpack_uint(values->depth_bias_scissor, 8, 8) |
            util_bitpack_uint(values->region_clip, 10, 10) |
            util_bitpack_uint(values->viewport, 11, 11) |
            util_bitpack_uint(values->w_clamp, 16, 16) |
            util_bitpack_uint(values->output_select, 17, 17) |
            util_bitpack_uint(values->varying_counts_32, 18, 18) |
            util_bitpack_uint(values->varying_counts_16, 19, 19) |
            util_bitpack_uint(values->cull, 21, 21) |
            util_bitpack_uint(values->cull_2, 22, 22) |
            util_bitpack_uint(values->fragment_shader, 23, 23) |
            util_bitpack_uint(values->occlusion_query, 24, 24) |
            util_bitpack_uint(values->occlusion_query_2, 25, 25) |
            util_bitpack_uint(values->output_unknown, 26, 26) |
            util_bitpack_uint(values->output_size, 27, 27) |
            util_bitpack_uint(values->varying_word_2, 28, 28);
}


#define AGX_PPP_HEADER_LENGTH 4
struct agx_ppp_header_packed { uint32_t opaque[1]; };
static inline void
AGX_PPP_HEADER_unpack(FILE *fp, const uint8_t * restrict cl,
                      struct AGX_PPP_HEADER * restrict values)
{
   if (((const uint32_t *) cl)[0] & 0xe010f200) fprintf(fp, "XXX: Unknown field of PPP Header unpacked at word 0: got %X, bad mask %X\n", ((const uint32_t *) cl)[0], ((const uint32_t *) cl)[0] & 0xe010f200);
   values->fragment_control = __gen_unpack_uint(cl, 0, 0);
   values->fragment_control_2 = __gen_unpack_uint(cl, 1, 1);
   values->fragment_front_face = __gen_unpack_uint(cl, 2, 2);
   values->fragment_front_face_2 = __gen_unpack_uint(cl, 3, 3);
   values->fragment_front_stencil = __gen_unpack_uint(cl, 4, 4);
   values->fragment_back_face = __gen_unpack_uint(cl, 5, 5);
   values->fragment_back_face_2 = __gen_unpack_uint(cl, 6, 6);
   values->fragment_back_stencil = __gen_unpack_uint(cl, 7, 7);
   values->depth_bias_scissor = __gen_unpack_uint(cl, 8, 8);
   values->region_clip = __gen_unpack_uint(cl, 10, 10);
   values->viewport = __gen_unpack_uint(cl, 11, 11);
   values->w_clamp = __gen_unpack_uint(cl, 16, 16);
   values->output_select = __gen_unpack_uint(cl, 17, 17);
   values->varying_counts_32 = __gen_unpack_uint(cl, 18, 18);
   values->varying_counts_16 = __gen_unpack_uint(cl, 19, 19);
   values->cull = __gen_unpack_uint(cl, 21, 21);
   values->cull_2 = __gen_unpack_uint(cl, 22, 22);
   values->fragment_shader = __gen_unpack_uint(cl, 23, 23);
   values->occlusion_query = __gen_unpack_uint(cl, 24, 24);
   values->occlusion_query_2 = __gen_unpack_uint(cl, 25, 25);
   values->output_unknown = __gen_unpack_uint(cl, 26, 26);
   values->output_size = __gen_unpack_uint(cl, 27, 27);
   values->varying_word_2 = __gen_unpack_uint(cl, 28, 28);
}

static inline void
AGX_PPP_HEADER_print(FILE *fp, const struct AGX_PPP_HEADER * values, unsigned indent)
{
   fprintf(fp, "%*sFragment control: %s\n", indent, "", values->fragment_control ? "true" : "false");
   fprintf(fp, "%*sFragment control 2: %s\n", indent, "", values->fragment_control_2 ? "true" : "false");
   fprintf(fp, "%*sFragment front face: %s\n", indent, "", values->fragment_front_face ? "true" : "false");
   fprintf(fp, "%*sFragment front face 2: %s\n", indent, "", values->fragment_front_face_2 ? "true" : "false");
   fprintf(fp, "%*sFragment front stencil: %s\n", indent, "", values->fragment_front_stencil ? "true" : "false");
   fprintf(fp, "%*sFragment back face: %s\n", indent, "", values->fragment_back_face ? "true" : "false");
   fprintf(fp, "%*sFragment back face 2: %s\n", indent, "", values->fragment_back_face_2 ? "true" : "false");
   fprintf(fp, "%*sFragment back stencil: %s\n", indent, "", values->fragment_back_stencil ? "true" : "false");
   fprintf(fp, "%*sDepth bias/scissor: %s\n", indent, "", values->depth_bias_scissor ? "true" : "false");
   fprintf(fp, "%*sRegion clip: %s\n", indent, "", values->region_clip ? "true" : "false");
   fprintf(fp, "%*sViewport: %s\n", indent, "", values->viewport ? "true" : "false");
   fprintf(fp, "%*sW clamp: %s\n", indent, "", values->w_clamp ? "true" : "false");
   fprintf(fp, "%*sOutput select: %s\n", indent, "", values->output_select ? "true" : "false");
   fprintf(fp, "%*sVarying counts 32: %s\n", indent, "", values->varying_counts_32 ? "true" : "false");
   fprintf(fp, "%*sVarying counts 16: %s\n", indent, "", values->varying_counts_16 ? "true" : "false");
   fprintf(fp, "%*sCull: %s\n", indent, "", values->cull ? "true" : "false");
   fprintf(fp, "%*sCull 2: %s\n", indent, "", values->cull_2 ? "true" : "false");
   fprintf(fp, "%*sFragment shader: %s\n", indent, "", values->fragment_shader ? "true" : "false");
   fprintf(fp, "%*sOcclusion query: %s\n", indent, "", values->occlusion_query ? "true" : "false");
   fprintf(fp, "%*sOcclusion query 2: %s\n", indent, "", values->occlusion_query_2 ? "true" : "false");
   fprintf(fp, "%*sOutput unknown: %s\n", indent, "", values->output_unknown ? "true" : "false");
   fprintf(fp, "%*sOutput size: %s\n", indent, "", values->output_size ? "true" : "false");
   fprintf(fp, "%*sVarying word 2: %s\n", indent, "", values->varying_word_2 ? "true" : "false");
}

struct AGX_REGION_CLIP {
   uint32_t                             max_x;
   uint32_t                             min_x;
   bool                                 enable;
   uint32_t                             max_y;
   uint32_t                             min_y;
};

#define AGX_REGION_CLIP_header                  \
   0

static inline void
AGX_REGION_CLIP_pack(uint32_t * restrict cl,
                     const struct AGX_REGION_CLIP * restrict values)
{
   assert(values->max_x >= 1);
   assert(values->max_y >= 1);
   cl[ 0] = util_bitpack_uint(values->max_x - 1, 0, 8) |
            util_bitpack_uint(values->min_x, 16, 24) |
            util_bitpack_uint(values->enable, 31, 31);
   cl[ 1] = util_bitpack_uint(values->max_y - 1, 0, 8) |
            util_bitpack_uint(values->min_y, 16, 24);
   cl[ 2] = 0;
}


#define AGX_REGION_CLIP_LENGTH 12
struct agx_region_clip_packed { uint32_t opaque[3]; };
static inline void
AGX_REGION_CLIP_unpack(FILE *fp, const uint8_t * restrict cl,
                       struct AGX_REGION_CLIP * restrict values)
{
   if (((const uint32_t *) cl)[0] & 0x7e00fe00) fprintf(fp, "XXX: Unknown field of Region clip unpacked at word 0: got %X, bad mask %X\n", ((const uint32_t *) cl)[0], ((const uint32_t *) cl)[0] & 0x7e00fe00);
   if (((const uint32_t *) cl)[1] & 0xfe00fe00) fprintf(fp, "XXX: Unknown field of Region clip unpacked at word 1: got %X, bad mask %X\n", ((const uint32_t *) cl)[1], ((const uint32_t *) cl)[1] & 0xfe00fe00);
   if (((const uint32_t *) cl)[2] & 0xffffffff) fprintf(fp, "XXX: Unknown field of Region clip unpacked at word 2: got %X, bad mask %X\n", ((const uint32_t *) cl)[2], ((const uint32_t *) cl)[2] & 0xffffffff);
   values->max_x = __gen_unpack_uint(cl, 0, 8) + 1;
   values->min_x = __gen_unpack_uint(cl, 16, 24);
   values->enable = __gen_unpack_uint(cl, 31, 31);
   values->max_y = __gen_unpack_uint(cl, 32, 40) + 1;
   values->min_y = __gen_unpack_uint(cl, 48, 56);
}

static inline void
AGX_REGION_CLIP_print(FILE *fp, const struct AGX_REGION_CLIP * values, unsigned indent)
{
   fprintf(fp, "%*sMax X: %u\n", indent, "", values->max_x);
   fprintf(fp, "%*sMin X: %u\n", indent, "", values->min_x);
   fprintf(fp, "%*sEnable: %s\n", indent, "", values->enable ? "true" : "false");
   fprintf(fp, "%*sMax Y: %u\n", indent, "", values->max_y);
   fprintf(fp, "%*sMin Y: %u\n", indent, "", values->min_y);
}

struct AGX_VIEWPORT {
   float                                translate_x;
   float                                scale_x;
   float                                translate_y;
   float                                scale_y;
   float                                translate_z;
   float                                scale_z;
};

#define AGX_VIEWPORT_header                     \
   0

static inline void
AGX_VIEWPORT_pack(uint32_t * restrict cl,
                  const struct AGX_VIEWPORT * restrict values)
{
   cl[ 0] = util_bitpack_float(values->translate_x);
   cl[ 1] = util_bitpack_float(values->scale_x);
   cl[ 2] = util_bitpack_float(values->translate_y);
   cl[ 3] = util_bitpack_float(values->scale_y);
   cl[ 4] = util_bitpack_float(values->translate_z);
   cl[ 5] = util_bitpack_float(values->scale_z);
}


#define AGX_VIEWPORT_LENGTH 24
struct agx_viewport_packed { uint32_t opaque[6]; };
static inline void
AGX_VIEWPORT_unpack(FILE *fp, const uint8_t * restrict cl,
                    struct AGX_VIEWPORT * restrict values)
{
   values->translate_x = __gen_unpack_float(cl, 0, 31);
   values->scale_x = __gen_unpack_float(cl, 32, 63);
   values->translate_y = __gen_unpack_float(cl, 64, 95);
   values->scale_y = __gen_unpack_float(cl, 96, 127);
   values->translate_z = __gen_unpack_float(cl, 128, 159);
   values->scale_z = __gen_unpack_float(cl, 160, 191);
}

static inline void
AGX_VIEWPORT_print(FILE *fp, const struct AGX_VIEWPORT * values, unsigned indent)
{
   fprintf(fp, "%*sTranslate X: %f\n", indent, "", values->translate_x);
   fprintf(fp, "%*sScale X: %f\n", indent, "", values->scale_x);
   fprintf(fp, "%*sTranslate Y: %f\n", indent, "", values->translate_y);
   fprintf(fp, "%*sScale Y: %f\n", indent, "", values->scale_y);
   fprintf(fp, "%*sTranslate Z: %f\n", indent, "", values->translate_z);
   fprintf(fp, "%*sScale Z: %f\n", indent, "", values->scale_z);
}

struct AGX_FRAGMENT_FACE {
   uint32_t                             stencil_reference;
   uint32_t                             line_width;
   enum agx_polygon_mode                polygon_mode;
   bool                                 disable_depth_write;
   uint32_t                             unk___visibility_test_internal;
   enum agx_zs_func                     depth_function;
};

#define AGX_FRAGMENT_FACE_header                \
   0

static inline void
AGX_FRAGMENT_FACE_pack(uint32_t * restrict cl,
                       const struct AGX_FRAGMENT_FACE * restrict values)
{
   cl[ 0] = util_bitpack_uint(values->stencil_reference, 0, 7) |
            util_bitpack_uint(values->line_width, 8, 15) |
            util_bitpack_uint(values->polygon_mode, 18, 19) |
            util_bitpack_uint(values->disable_depth_write, 21, 21) |
            util_bitpack_uint(values->unk___visibility_test_internal, 22, 23) |
            util_bitpack_uint(values->depth_function, 24, 26);
}


#define AGX_FRAGMENT_FACE_LENGTH 4
struct agx_fragment_face_packed { uint32_t opaque[1]; };
static inline void
AGX_FRAGMENT_FACE_unpack(FILE *fp, const uint8_t * restrict cl,
                         struct AGX_FRAGMENT_FACE * restrict values)
{
   if (((const uint32_t *) cl)[0] & 0xf8130000) fprintf(fp, "XXX: Unknown field of Fragment face unpacked at word 0: got %X, bad mask %X\n", ((const uint32_t *) cl)[0], ((const uint32_t *) cl)[0] & 0xf8130000);
   values->stencil_reference = __gen_unpack_uint(cl, 0, 7);
   values->line_width = __gen_unpack_uint(cl, 8, 15);
   values->polygon_mode = (enum agx_polygon_mode) __gen_unpack_uint(cl, 18, 19);
   values->disable_depth_write = __gen_unpack_uint(cl, 21, 21);
   values->unk___visibility_test_internal = __gen_unpack_uint(cl, 22, 23);
   values->depth_function = (enum agx_zs_func) __gen_unpack_uint(cl, 24, 26);
}

static inline void
AGX_FRAGMENT_FACE_print(FILE *fp, const struct AGX_FRAGMENT_FACE * values, unsigned indent)
{
   fprintf(fp, "%*sStencil reference: 0x%" PRIx32 "\n", indent, "", values->stencil_reference);
   fprintf(fp, "%*sLine width: 0x%" PRIx32 "\n", indent, "", values->line_width);
   if (agx_polygon_mode_as_str(values->polygon_mode))
     fprintf(fp, "%*sPolygon mode: %s\n", indent, "", agx_polygon_mode_as_str(values->polygon_mode));
    else
     fprintf(fp, "%*sPolygon mode: unknown %X (XXX)\n", indent, "", values->polygon_mode);
   fprintf(fp, "%*sDisable depth write: %s\n", indent, "", values->disable_depth_write ? "true" : "false");
   fprintf(fp, "%*sUnk - visibility test internal: 0x%" PRIx32 "\n", indent, "", values->unk___visibility_test_internal);
   if (agx_zs_func_as_str(values->depth_function))
     fprintf(fp, "%*sDepth function: %s\n", indent, "", agx_zs_func_as_str(values->depth_function));
    else
     fprintf(fp, "%*sDepth function: unknown %X (XXX)\n", indent, "", values->depth_function);
}

enum agx_conservative_depth {
        AGX_CONSERVATIVE_DEPTH_ANY           =      0,
        AGX_CONSERVATIVE_DEPTH_GREATER       =      1,
        AGX_CONSERVATIVE_DEPTH_LESS          =      2,
        AGX_CONSERVATIVE_DEPTH_UNCHANGED     =      3,
};

static inline const char *
agx_conservative_depth_as_str(enum agx_conservative_depth imm)
{
    switch (imm) {
    case AGX_CONSERVATIVE_DEPTH_ANY: return "Any";
    case AGX_CONSERVATIVE_DEPTH_GREATER: return "Greater";
    case AGX_CONSERVATIVE_DEPTH_LESS: return "Less";
    case AGX_CONSERVATIVE_DEPTH_UNCHANGED: return "Unchanged";
    default: break;
    }
    return NULL;
}

struct AGX_FRAGMENT_FACE_2 {
   bool                                 disable_depth_write;
   enum agx_conservative_depth          conservative_depth;
   enum agx_zs_func                     depth_function;
   enum agx_object_type                 object_type;
};

#define AGX_FRAGMENT_FACE_2_header              \
   .disable_depth_write = true,  \
   .conservative_depth = AGX_CONSERVATIVE_DEPTH_UNCHANGED,  \
   .depth_function = AGX_ZS_FUNC_ALWAYS

static inline void
AGX_FRAGMENT_FACE_2_pack(uint32_t * restrict cl,
                         const struct AGX_FRAGMENT_FACE_2 * restrict values)
{
   cl[ 0] = util_bitpack_uint(values->disable_depth_write, 21, 21) |
            util_bitpack_uint(values->conservative_depth, 22, 23) |
            util_bitpack_uint(values->depth_function, 24, 26) |
            util_bitpack_uint(values->object_type, 28, 31);
}


#define AGX_FRAGMENT_FACE_2_LENGTH 4
struct agx_fragment_face_2_packed { uint32_t opaque[1]; };
static inline void
AGX_FRAGMENT_FACE_2_unpack(FILE *fp, const uint8_t * restrict cl,
                           struct AGX_FRAGMENT_FACE_2 * restrict values)
{
   if (((const uint32_t *) cl)[0] & 0x81fffff) fprintf(fp, "XXX: Unknown field of Fragment face 2 unpacked at word 0: got %X, bad mask %X\n", ((const uint32_t *) cl)[0], ((const uint32_t *) cl)[0] & 0x81fffff);
   values->disable_depth_write = __gen_unpack_uint(cl, 21, 21);
   values->conservative_depth = (enum agx_conservative_depth) __gen_unpack_uint(cl, 22, 23);
   values->depth_function = (enum agx_zs_func) __gen_unpack_uint(cl, 24, 26);
   values->object_type = (enum agx_object_type) __gen_unpack_uint(cl, 28, 31);
}

static inline void
AGX_FRAGMENT_FACE_2_print(FILE *fp, const struct AGX_FRAGMENT_FACE_2 * values, unsigned indent)
{
   fprintf(fp, "%*sDisable depth write: %s\n", indent, "", values->disable_depth_write ? "true" : "false");
   if (agx_conservative_depth_as_str(values->conservative_depth))
     fprintf(fp, "%*sConservative depth: %s\n", indent, "", agx_conservative_depth_as_str(values->conservative_depth));
    else
     fprintf(fp, "%*sConservative depth: unknown %X (XXX)\n", indent, "", values->conservative_depth);
   if (agx_zs_func_as_str(values->depth_function))
     fprintf(fp, "%*sDepth function: %s\n", indent, "", agx_zs_func_as_str(values->depth_function));
    else
     fprintf(fp, "%*sDepth function: unknown %X (XXX)\n", indent, "", values->depth_function);
   if (agx_object_type_as_str(values->object_type))
     fprintf(fp, "%*sObject type: %s\n", indent, "", agx_object_type_as_str(values->object_type));
    else
     fprintf(fp, "%*sObject type: unknown %X (XXX)\n", indent, "", values->object_type);
}

struct AGX_FRAGMENT_STENCIL {
   uint32_t                             write_mask;
   uint32_t                             read_mask;
   enum agx_stencil_op                  depth_pass;
   enum agx_stencil_op                  depth_fail;
   enum agx_stencil_op                  stencil_fail;
   enum agx_zs_func                     compare;
};

#define AGX_FRAGMENT_STENCIL_header             \
   0

static inline void
AGX_FRAGMENT_STENCIL_pack(uint32_t * restrict cl,
                          const struct AGX_FRAGMENT_STENCIL * restrict values)
{
   cl[ 0] = util_bitpack_uint(values->write_mask, 0, 7) |
            util_bitpack_uint(values->read_mask, 8, 15) |
            util_bitpack_uint(values->depth_pass, 16, 18) |
            util_bitpack_uint(values->depth_fail, 19, 21) |
            util_bitpack_uint(values->stencil_fail, 22, 24) |
            util_bitpack_uint(values->compare, 25, 27);
}


#define AGX_FRAGMENT_STENCIL_LENGTH 4
struct agx_fragment_stencil_packed { uint32_t opaque[1]; };
static inline void
AGX_FRAGMENT_STENCIL_unpack(FILE *fp, const uint8_t * restrict cl,
                            struct AGX_FRAGMENT_STENCIL * restrict values)
{
   if (((const uint32_t *) cl)[0] & 0xf0000000) fprintf(fp, "XXX: Unknown field of Fragment stencil unpacked at word 0: got %X, bad mask %X\n", ((const uint32_t *) cl)[0], ((const uint32_t *) cl)[0] & 0xf0000000);
   values->write_mask = __gen_unpack_uint(cl, 0, 7);
   values->read_mask = __gen_unpack_uint(cl, 8, 15);
   values->depth_pass = (enum agx_stencil_op) __gen_unpack_uint(cl, 16, 18);
   values->depth_fail = (enum agx_stencil_op) __gen_unpack_uint(cl, 19, 21);
   values->stencil_fail = (enum agx_stencil_op) __gen_unpack_uint(cl, 22, 24);
   values->compare = (enum agx_zs_func) __gen_unpack_uint(cl, 25, 27);
}

static inline void
AGX_FRAGMENT_STENCIL_print(FILE *fp, const struct AGX_FRAGMENT_STENCIL * values, unsigned indent)
{
   fprintf(fp, "%*sWrite mask: 0x%" PRIx32 "\n", indent, "", values->write_mask);
   fprintf(fp, "%*sRead mask: 0x%" PRIx32 "\n", indent, "", values->read_mask);
   if (agx_stencil_op_as_str(values->depth_pass))
     fprintf(fp, "%*sDepth pass: %s\n", indent, "", agx_stencil_op_as_str(values->depth_pass));
    else
     fprintf(fp, "%*sDepth pass: unknown %X (XXX)\n", indent, "", values->depth_pass);
   if (agx_stencil_op_as_str(values->depth_fail))
     fprintf(fp, "%*sDepth fail: %s\n", indent, "", agx_stencil_op_as_str(values->depth_fail));
    else
     fprintf(fp, "%*sDepth fail: unknown %X (XXX)\n", indent, "", values->depth_fail);
   if (agx_stencil_op_as_str(values->stencil_fail))
     fprintf(fp, "%*sStencil fail: %s\n", indent, "", agx_stencil_op_as_str(values->stencil_fail));
    else
     fprintf(fp, "%*sStencil fail: unknown %X (XXX)\n", indent, "", values->stencil_fail);
   if (agx_zs_func_as_str(values->compare))
     fprintf(fp, "%*sCompare: %s\n", indent, "", agx_zs_func_as_str(values->compare));
    else
     fprintf(fp, "%*sCompare: unknown %X (XXX)\n", indent, "", values->compare);
}

enum agx_pass_type {
        AGX_PASS_TYPE_OPAQUE                 =      0,
        AGX_PASS_TYPE_TRANSLUCENT            =      1,
        AGX_PASS_TYPE_PUNCH_THROUGH          =      2,
        AGX_PASS_TYPE_TRANSLUCENT_PUNCH_THROUGH =      3,
};

static inline const char *
agx_pass_type_as_str(enum agx_pass_type imm)
{
    switch (imm) {
    case AGX_PASS_TYPE_OPAQUE: return "Opaque";
    case AGX_PASS_TYPE_TRANSLUCENT: return "Translucent";
    case AGX_PASS_TYPE_PUNCH_THROUGH: return "Punch through";
    case AGX_PASS_TYPE_TRANSLUCENT_PUNCH_THROUGH: return "Translucent punch through";
    default: break;
    }
    return NULL;
}

struct AGX_FRAGMENT_CONTROL {
   bool                                 unk_1;
   enum agx_visibility_mode             visibility_mode;
   bool                                 scissor_enable;
   bool                                 depth_bias_enable;
   bool                                 stencil_test_enable;
   bool                                 two_sided_stencil;
   bool                                 tag_write_disable;
   bool                                 sample_mask_after_depth_stencil;
   bool                                 disable_tri_merging;
   enum agx_pass_type                   pass_type;
};

#define AGX_FRAGMENT_CONTROL_header             \
   .unk_1 = true

static inline void
AGX_FRAGMENT_CONTROL_pack(uint32_t * restrict cl,
                          const struct AGX_FRAGMENT_CONTROL * restrict values)
{
   cl[ 0] = util_bitpack_uint(values->unk_1, 9, 9) |
            util_bitpack_uint(values->visibility_mode, 14, 15) |
            util_bitpack_uint(values->scissor_enable, 16, 16) |
            util_bitpack_uint(values->depth_bias_enable, 17, 17) |
            util_bitpack_uint(values->stencil_test_enable, 18, 18) |
            util_bitpack_uint(values->two_sided_stencil, 19, 19) |
            util_bitpack_uint(values->tag_write_disable, 21, 21) |
            util_bitpack_uint(values->sample_mask_after_depth_stencil, 25, 25) |
            util_bitpack_uint(values->disable_tri_merging, 26, 26) |
            util_bitpack_uint(values->pass_type, 29, 31);
}


#define AGX_FRAGMENT_CONTROL_LENGTH 4
struct agx_fragment_control_packed { uint32_t opaque[1]; };
static inline void
AGX_FRAGMENT_CONTROL_unpack(FILE *fp, const uint8_t * restrict cl,
                            struct AGX_FRAGMENT_CONTROL * restrict values)
{
   if (((const uint32_t *) cl)[0] & 0x19d03dff) fprintf(fp, "XXX: Unknown field of Fragment control unpacked at word 0: got %X, bad mask %X\n", ((const uint32_t *) cl)[0], ((const uint32_t *) cl)[0] & 0x19d03dff);
   values->unk_1 = __gen_unpack_uint(cl, 9, 9);
   values->visibility_mode = (enum agx_visibility_mode) __gen_unpack_uint(cl, 14, 15);
   values->scissor_enable = __gen_unpack_uint(cl, 16, 16);
   values->depth_bias_enable = __gen_unpack_uint(cl, 17, 17);
   values->stencil_test_enable = __gen_unpack_uint(cl, 18, 18);
   values->two_sided_stencil = __gen_unpack_uint(cl, 19, 19);
   values->tag_write_disable = __gen_unpack_uint(cl, 21, 21);
   values->sample_mask_after_depth_stencil = __gen_unpack_uint(cl, 25, 25);
   values->disable_tri_merging = __gen_unpack_uint(cl, 26, 26);
   values->pass_type = (enum agx_pass_type) __gen_unpack_uint(cl, 29, 31);
}

static inline void
AGX_FRAGMENT_CONTROL_print(FILE *fp, const struct AGX_FRAGMENT_CONTROL * values, unsigned indent)
{
   fprintf(fp, "%*sUnk 1: %s\n", indent, "", values->unk_1 ? "true" : "false");
   if (agx_visibility_mode_as_str(values->visibility_mode))
     fprintf(fp, "%*sVisibility mode: %s\n", indent, "", agx_visibility_mode_as_str(values->visibility_mode));
    else
     fprintf(fp, "%*sVisibility mode: unknown %X (XXX)\n", indent, "", values->visibility_mode);
   fprintf(fp, "%*sScissor enable: %s\n", indent, "", values->scissor_enable ? "true" : "false");
   fprintf(fp, "%*sDepth bias enable: %s\n", indent, "", values->depth_bias_enable ? "true" : "false");
   fprintf(fp, "%*sStencil test enable: %s\n", indent, "", values->stencil_test_enable ? "true" : "false");
   fprintf(fp, "%*sTwo-sided stencil: %s\n", indent, "", values->two_sided_stencil ? "true" : "false");
   fprintf(fp, "%*sTag write disable: %s\n", indent, "", values->tag_write_disable ? "true" : "false");
   fprintf(fp, "%*sSample mask after depth/stencil: %s\n", indent, "", values->sample_mask_after_depth_stencil ? "true" : "false");
   fprintf(fp, "%*sDisable tri merging: %s\n", indent, "", values->disable_tri_merging ? "true" : "false");
   if (agx_pass_type_as_str(values->pass_type))
     fprintf(fp, "%*sPass type: %s\n", indent, "", agx_pass_type_as_str(values->pass_type));
    else
     fprintf(fp, "%*sPass type: unknown %X (XXX)\n", indent, "", values->pass_type);
}

struct AGX_FRAGMENT_OCCLUSION_QUERY {
   uint32_t                             index;
};

#define AGX_FRAGMENT_OCCLUSION_QUERY_header     \
   0

static inline void
AGX_FRAGMENT_OCCLUSION_QUERY_pack(uint32_t * restrict cl,
                                  const struct AGX_FRAGMENT_OCCLUSION_QUERY * restrict values)
{
   cl[ 0] = util_bitpack_uint(values->index, 17, 31);
}


#define AGX_FRAGMENT_OCCLUSION_QUERY_LENGTH 4
struct agx_fragment_occlusion_query_packed { uint32_t opaque[1]; };
static inline void
AGX_FRAGMENT_OCCLUSION_QUERY_unpack(FILE *fp, const uint8_t * restrict cl,
                                    struct AGX_FRAGMENT_OCCLUSION_QUERY * restrict values)
{
   if (((const uint32_t *) cl)[0] & 0x1ffff) fprintf(fp, "XXX: Unknown field of Fragment occlusion query unpacked at word 0: got %X, bad mask %X\n", ((const uint32_t *) cl)[0], ((const uint32_t *) cl)[0] & 0x1ffff);
   values->index = __gen_unpack_uint(cl, 17, 31);
}

static inline void
AGX_FRAGMENT_OCCLUSION_QUERY_print(FILE *fp, const struct AGX_FRAGMENT_OCCLUSION_QUERY * values, unsigned indent)
{
   fprintf(fp, "%*sIndex: %u\n", indent, "", values->index);
}

struct AGX_FRAGMENT_OCCLUSION_QUERY_2 {
   uint32_t                             unknown;
};

#define AGX_FRAGMENT_OCCLUSION_QUERY_2_header   \
   0

static inline void
AGX_FRAGMENT_OCCLUSION_QUERY_2_pack(uint32_t * restrict cl,
                                    const struct AGX_FRAGMENT_OCCLUSION_QUERY_2 * restrict values)
{
   cl[ 0] = util_bitpack_uint(values->unknown, 0, 16);
}


#define AGX_FRAGMENT_OCCLUSION_QUERY_2_LENGTH 4
struct agx_fragment_occlusion_query_2_packed { uint32_t opaque[1]; };
static inline void
AGX_FRAGMENT_OCCLUSION_QUERY_2_unpack(FILE *fp, const uint8_t * restrict cl,
                                      struct AGX_FRAGMENT_OCCLUSION_QUERY_2 * restrict values)
{
   if (((const uint32_t *) cl)[0] & 0xfffe0000) fprintf(fp, "XXX: Unknown field of Fragment occlusion query 2 unpacked at word 0: got %X, bad mask %X\n", ((const uint32_t *) cl)[0], ((const uint32_t *) cl)[0] & 0xfffe0000);
   values->unknown = __gen_unpack_uint(cl, 0, 16);
}

static inline void
AGX_FRAGMENT_OCCLUSION_QUERY_2_print(FILE *fp, const struct AGX_FRAGMENT_OCCLUSION_QUERY_2 * values, unsigned indent)
{
   fprintf(fp, "%*sUnknown: 0x%" PRIx32 "\n", indent, "", values->unknown);
}

struct AGX_W_CLAMP {
   float                                w_clamp;
};

#define AGX_W_CLAMP_header                      \
   0

static inline void
AGX_W_CLAMP_pack(uint32_t * restrict cl,
                 const struct AGX_W_CLAMP * restrict values)
{
   cl[ 0] = util_bitpack_float(values->w_clamp);
}


#define AGX_W_CLAMP_LENGTH 4
struct agx_w_clamp_packed { uint32_t opaque[1]; };
static inline void
AGX_W_CLAMP_unpack(FILE *fp, const uint8_t * restrict cl,
                   struct AGX_W_CLAMP * restrict values)
{
   values->w_clamp = __gen_unpack_float(cl, 0, 31);
}

static inline void
AGX_W_CLAMP_print(FILE *fp, const struct AGX_W_CLAMP * values, unsigned indent)
{
   fprintf(fp, "%*sW Clamp: %f\n", indent, "", values->w_clamp);
}

enum agx_ppp_vertex {
        AGX_PPP_VERTEX_0                     =      1,
        AGX_PPP_VERTEX_2                     =      3,
};

static inline const char *
agx_ppp_vertex_as_str(enum agx_ppp_vertex imm)
{
    switch (imm) {
    case AGX_PPP_VERTEX_0: return "0";
    case AGX_PPP_VERTEX_2: return "2";
    default: break;
    }
    return NULL;
}

struct AGX_CULL {
   bool                                 cull_front;
   bool                                 cull_back;
   enum agx_ppp_vertex                  flat_shading_vertex;
   bool                                 depth_clip;
   bool                                 depth_clamp;
   bool                                 front_face_ccw;
   bool                                 rasterizer_discard;
};

#define AGX_CULL_header                         \
   0

static inline void
AGX_CULL_pack(uint32_t * restrict cl,
              const struct AGX_CULL * restrict values)
{
   cl[ 0] = util_bitpack_uint(values->cull_front, 0, 0) |
            util_bitpack_uint(values->cull_back, 1, 1) |
            util_bitpack_uint(values->flat_shading_vertex, 7, 8) |
            util_bitpack_uint(values->depth_clip, 10, 10) |
            util_bitpack_uint(values->depth_clamp, 11, 11) |
            util_bitpack_uint(values->front_face_ccw, 16, 16) |
            util_bitpack_uint(values->rasterizer_discard, 17, 17);
}


#define AGX_CULL_LENGTH 4
struct agx_cull_packed { uint32_t opaque[1]; };
static inline void
AGX_CULL_unpack(FILE *fp, const uint8_t * restrict cl,
                struct AGX_CULL * restrict values)
{
   if (((const uint32_t *) cl)[0] & 0xfffcf27c) fprintf(fp, "XXX: Unknown field of Cull unpacked at word 0: got %X, bad mask %X\n", ((const uint32_t *) cl)[0], ((const uint32_t *) cl)[0] & 0xfffcf27c);
   values->cull_front = __gen_unpack_uint(cl, 0, 0);
   values->cull_back = __gen_unpack_uint(cl, 1, 1);
   values->flat_shading_vertex = (enum agx_ppp_vertex) __gen_unpack_uint(cl, 7, 8);
   values->depth_clip = __gen_unpack_uint(cl, 10, 10);
   values->depth_clamp = __gen_unpack_uint(cl, 11, 11);
   values->front_face_ccw = __gen_unpack_uint(cl, 16, 16);
   values->rasterizer_discard = __gen_unpack_uint(cl, 17, 17);
}

static inline void
AGX_CULL_print(FILE *fp, const struct AGX_CULL * values, unsigned indent)
{
   fprintf(fp, "%*sCull front: %s\n", indent, "", values->cull_front ? "true" : "false");
   fprintf(fp, "%*sCull back: %s\n", indent, "", values->cull_back ? "true" : "false");
   if (agx_ppp_vertex_as_str(values->flat_shading_vertex))
     fprintf(fp, "%*sFlat shading vertex: %s\n", indent, "", agx_ppp_vertex_as_str(values->flat_shading_vertex));
    else
     fprintf(fp, "%*sFlat shading vertex: unknown %X (XXX)\n", indent, "", values->flat_shading_vertex);
   fprintf(fp, "%*sDepth clip: %s\n", indent, "", values->depth_clip ? "true" : "false");
   fprintf(fp, "%*sDepth clamp: %s\n", indent, "", values->depth_clamp ? "true" : "false");
   fprintf(fp, "%*sFront face CCW: %s\n", indent, "", values->front_face_ccw ? "true" : "false");
   fprintf(fp, "%*sRasterizer discard: %s\n", indent, "", values->rasterizer_discard ? "true" : "false");
}

struct AGX_CULL_2 {
   uint32_t                             unknown_2;
   bool                                 draw_clipped_edges;
   bool                                 primitive_msaa;
   bool                                 rasterizer_discard;
};

#define AGX_CULL_2_header                       \
   .unknown_2 = 0xa0

static inline void
AGX_CULL_2_pack(uint32_t * restrict cl,
                const struct AGX_CULL_2 * restrict values)
{
   cl[ 0] = util_bitpack_uint(values->unknown_2, 0, 7) |
            util_bitpack_uint(values->draw_clipped_edges, 9, 9) |
            util_bitpack_uint(values->primitive_msaa, 15, 15) |
            util_bitpack_uint(values->rasterizer_discard, 17, 17);
}


#define AGX_CULL_2_LENGTH 4
struct agx_cull_2_packed { uint32_t opaque[1]; };
static inline void
AGX_CULL_2_unpack(FILE *fp, const uint8_t * restrict cl,
                  struct AGX_CULL_2 * restrict values)
{
   if (((const uint32_t *) cl)[0] & 0xfffd7d00) fprintf(fp, "XXX: Unknown field of Cull 2 unpacked at word 0: got %X, bad mask %X\n", ((const uint32_t *) cl)[0], ((const uint32_t *) cl)[0] & 0xfffd7d00);
   values->unknown_2 = __gen_unpack_uint(cl, 0, 7);
   values->draw_clipped_edges = __gen_unpack_uint(cl, 9, 9);
   values->primitive_msaa = __gen_unpack_uint(cl, 15, 15);
   values->rasterizer_discard = __gen_unpack_uint(cl, 17, 17);
}

static inline void
AGX_CULL_2_print(FILE *fp, const struct AGX_CULL_2 * values, unsigned indent)
{
   fprintf(fp, "%*sUnknown 2: 0x%" PRIx32 "\n", indent, "", values->unknown_2);
   fprintf(fp, "%*sDraw clipped edges: %s\n", indent, "", values->draw_clipped_edges ? "true" : "false");
   fprintf(fp, "%*sPrimitive MSAA: %s\n", indent, "", values->primitive_msaa ? "true" : "false");
   fprintf(fp, "%*sRasterizer discard: %s\n", indent, "", values->rasterizer_discard ? "true" : "false");
}

struct AGX_VARYING_COUNTS {
   uint32_t                             smooth;
   uint32_t                             flat;
   uint32_t                             linear;
};

#define AGX_VARYING_COUNTS_header               \
   0

static inline void
AGX_VARYING_COUNTS_pack(uint32_t * restrict cl,
                        const struct AGX_VARYING_COUNTS * restrict values)
{
   cl[ 0] = util_bitpack_uint(values->smooth, 0, 7) |
            util_bitpack_uint(values->flat, 8, 15) |
            util_bitpack_uint(values->linear, 16, 23);
}


#define AGX_VARYING_COUNTS_LENGTH 4
struct agx_varying_counts_packed { uint32_t opaque[1]; };
static inline void
AGX_VARYING_COUNTS_unpack(FILE *fp, const uint8_t * restrict cl,
                          struct AGX_VARYING_COUNTS * restrict values)
{
   if (((const uint32_t *) cl)[0] & 0xff000000) fprintf(fp, "XXX: Unknown field of Varying Counts unpacked at word 0: got %X, bad mask %X\n", ((const uint32_t *) cl)[0], ((const uint32_t *) cl)[0] & 0xff000000);
   values->smooth = __gen_unpack_uint(cl, 0, 7);
   values->flat = __gen_unpack_uint(cl, 8, 15);
   values->linear = __gen_unpack_uint(cl, 16, 23);
}

static inline void
AGX_VARYING_COUNTS_print(FILE *fp, const struct AGX_VARYING_COUNTS * values, unsigned indent)
{
   fprintf(fp, "%*sSmooth: %u\n", indent, "", values->smooth);
   fprintf(fp, "%*sFlat: %u\n", indent, "", values->flat);
   fprintf(fp, "%*sLinear: %u\n", indent, "", values->linear);
}

struct AGX_VARYING_2 {
   int dummy;
};

#define AGX_VARYING_2_header                    \
   0

static inline void
AGX_VARYING_2_pack(uint32_t * restrict cl,
                   const struct AGX_VARYING_2 * restrict values)
{
   cl[ 0] = 0;
   cl[ 1] = 0;
}


#define AGX_VARYING_2_LENGTH 8
struct agx_varying_2_packed { uint32_t opaque[2]; };
static inline void
AGX_VARYING_2_unpack(FILE *fp, const uint8_t * restrict cl,
                     struct AGX_VARYING_2 * restrict values)
{
   if (((const uint32_t *) cl)[0] & 0xffffffff) fprintf(fp, "XXX: Unknown field of Varying 2 unpacked at word 0: got %X, bad mask %X\n", ((const uint32_t *) cl)[0], ((const uint32_t *) cl)[0] & 0xffffffff);
   if (((const uint32_t *) cl)[1] & 0xffffffff) fprintf(fp, "XXX: Unknown field of Varying 2 unpacked at word 1: got %X, bad mask %X\n", ((const uint32_t *) cl)[1], ((const uint32_t *) cl)[1] & 0xffffffff);
}

static inline void
AGX_VARYING_2_print(FILE *fp, const struct AGX_VARYING_2 * values, unsigned indent)
{
}

struct AGX_OUTPUT_SELECT {
   bool                                 clip_distance_plane_0;
   bool                                 clip_distance_plane_1;
   bool                                 clip_distance_plane_2;
   bool                                 clip_distance_plane_3;
   bool                                 clip_distance_plane_4;
   bool                                 clip_distance_plane_5;
   bool                                 clip_distance_plane_6;
   bool                                 clip_distance_plane_7;
   bool                                 clip_distance_plane_8;
   bool                                 clip_distance_plane_9;
   bool                                 clip_distance_plane_10;
   bool                                 clip_distance_plane_11;
   bool                                 clip_distance_plane_12;
   bool                                 clip_distance_plane_13;
   bool                                 clip_distance_plane_14;
   bool                                 clip_distance_plane_15;
   bool                                 varyings;
   bool                                 point_size;
   bool                                 viewport_target;
   bool                                 render_target;
   bool                                 frag_coord_z;
   bool                                 barycentric_coordinates;
};

#define AGX_OUTPUT_SELECT_header                \
   0

static inline void
AGX_OUTPUT_SELECT_pack(uint32_t * restrict cl,
                       const struct AGX_OUTPUT_SELECT * restrict values)
{
   cl[ 0] = util_bitpack_uint(values->clip_distance_plane_0, 0, 0) |
            util_bitpack_uint(values->clip_distance_plane_1, 1, 1) |
            util_bitpack_uint(values->clip_distance_plane_2, 2, 2) |
            util_bitpack_uint(values->clip_distance_plane_3, 3, 3) |
            util_bitpack_uint(values->clip_distance_plane_4, 4, 4) |
            util_bitpack_uint(values->clip_distance_plane_5, 5, 5) |
            util_bitpack_uint(values->clip_distance_plane_6, 6, 6) |
            util_bitpack_uint(values->clip_distance_plane_7, 7, 7) |
            util_bitpack_uint(values->clip_distance_plane_8, 8, 8) |
            util_bitpack_uint(values->clip_distance_plane_9, 9, 9) |
            util_bitpack_uint(values->clip_distance_plane_10, 10, 10) |
            util_bitpack_uint(values->clip_distance_plane_11, 11, 11) |
            util_bitpack_uint(values->clip_distance_plane_12, 12, 12) |
            util_bitpack_uint(values->clip_distance_plane_13, 13, 13) |
            util_bitpack_uint(values->clip_distance_plane_14, 14, 14) |
            util_bitpack_uint(values->clip_distance_plane_15, 15, 15) |
            util_bitpack_uint(values->varyings, 16, 16) |
            util_bitpack_uint(values->point_size, 18, 18) |
            util_bitpack_uint(values->viewport_target, 19, 19) |
            util_bitpack_uint(values->render_target, 20, 20) |
            util_bitpack_uint(values->frag_coord_z, 21, 21) |
            util_bitpack_uint(values->barycentric_coordinates, 22, 22);
}


#define AGX_OUTPUT_SELECT_LENGTH 4
struct agx_output_select_packed { uint32_t opaque[1]; };
static inline void
AGX_OUTPUT_SELECT_unpack(FILE *fp, const uint8_t * restrict cl,
                         struct AGX_OUTPUT_SELECT * restrict values)
{
   if (((const uint32_t *) cl)[0] & 0xff820000) fprintf(fp, "XXX: Unknown field of Output Select unpacked at word 0: got %X, bad mask %X\n", ((const uint32_t *) cl)[0], ((const uint32_t *) cl)[0] & 0xff820000);
   values->clip_distance_plane_0 = __gen_unpack_uint(cl, 0, 0);
   values->clip_distance_plane_1 = __gen_unpack_uint(cl, 1, 1);
   values->clip_distance_plane_2 = __gen_unpack_uint(cl, 2, 2);
   values->clip_distance_plane_3 = __gen_unpack_uint(cl, 3, 3);
   values->clip_distance_plane_4 = __gen_unpack_uint(cl, 4, 4);
   values->clip_distance_plane_5 = __gen_unpack_uint(cl, 5, 5);
   values->clip_distance_plane_6 = __gen_unpack_uint(cl, 6, 6);
   values->clip_distance_plane_7 = __gen_unpack_uint(cl, 7, 7);
   values->clip_distance_plane_8 = __gen_unpack_uint(cl, 8, 8);
   values->clip_distance_plane_9 = __gen_unpack_uint(cl, 9, 9);
   values->clip_distance_plane_10 = __gen_unpack_uint(cl, 10, 10);
   values->clip_distance_plane_11 = __gen_unpack_uint(cl, 11, 11);
   values->clip_distance_plane_12 = __gen_unpack_uint(cl, 12, 12);
   values->clip_distance_plane_13 = __gen_unpack_uint(cl, 13, 13);
   values->clip_distance_plane_14 = __gen_unpack_uint(cl, 14, 14);
   values->clip_distance_plane_15 = __gen_unpack_uint(cl, 15, 15);
   values->varyings = __gen_unpack_uint(cl, 16, 16);
   values->point_size = __gen_unpack_uint(cl, 18, 18);
   values->viewport_target = __gen_unpack_uint(cl, 19, 19);
   values->render_target = __gen_unpack_uint(cl, 20, 20);
   values->frag_coord_z = __gen_unpack_uint(cl, 21, 21);
   values->barycentric_coordinates = __gen_unpack_uint(cl, 22, 22);
}

static inline void
AGX_OUTPUT_SELECT_print(FILE *fp, const struct AGX_OUTPUT_SELECT * values, unsigned indent)
{
   fprintf(fp, "%*sClip distance plane 0: %s\n", indent, "", values->clip_distance_plane_0 ? "true" : "false");
   fprintf(fp, "%*sClip distance plane 1: %s\n", indent, "", values->clip_distance_plane_1 ? "true" : "false");
   fprintf(fp, "%*sClip distance plane 2: %s\n", indent, "", values->clip_distance_plane_2 ? "true" : "false");
   fprintf(fp, "%*sClip distance plane 3: %s\n", indent, "", values->clip_distance_plane_3 ? "true" : "false");
   fprintf(fp, "%*sClip distance plane 4: %s\n", indent, "", values->clip_distance_plane_4 ? "true" : "false");
   fprintf(fp, "%*sClip distance plane 5: %s\n", indent, "", values->clip_distance_plane_5 ? "true" : "false");
   fprintf(fp, "%*sClip distance plane 6: %s\n", indent, "", values->clip_distance_plane_6 ? "true" : "false");
   fprintf(fp, "%*sClip distance plane 7: %s\n", indent, "", values->clip_distance_plane_7 ? "true" : "false");
   fprintf(fp, "%*sClip distance plane 8: %s\n", indent, "", values->clip_distance_plane_8 ? "true" : "false");
   fprintf(fp, "%*sClip distance plane 9: %s\n", indent, "", values->clip_distance_plane_9 ? "true" : "false");
   fprintf(fp, "%*sClip distance plane 10: %s\n", indent, "", values->clip_distance_plane_10 ? "true" : "false");
   fprintf(fp, "%*sClip distance plane 11: %s\n", indent, "", values->clip_distance_plane_11 ? "true" : "false");
   fprintf(fp, "%*sClip distance plane 12: %s\n", indent, "", values->clip_distance_plane_12 ? "true" : "false");
   fprintf(fp, "%*sClip distance plane 13: %s\n", indent, "", values->clip_distance_plane_13 ? "true" : "false");
   fprintf(fp, "%*sClip distance plane 14: %s\n", indent, "", values->clip_distance_plane_14 ? "true" : "false");
   fprintf(fp, "%*sClip distance plane 15: %s\n", indent, "", values->clip_distance_plane_15 ? "true" : "false");
   fprintf(fp, "%*sVaryings: %s\n", indent, "", values->varyings ? "true" : "false");
   fprintf(fp, "%*sPoint size: %s\n", indent, "", values->point_size ? "true" : "false");
   fprintf(fp, "%*sViewport target: %s\n", indent, "", values->viewport_target ? "true" : "false");
   fprintf(fp, "%*sRender target: %s\n", indent, "", values->render_target ? "true" : "false");
   fprintf(fp, "%*sFrag coord Z: %s\n", indent, "", values->frag_coord_z ? "true" : "false");
   fprintf(fp, "%*sBarycentric coordinates: %s\n", indent, "", values->barycentric_coordinates ? "true" : "false");
}

struct AGX_OUTPUT_UNKNOWN {
   int dummy;
};

#define AGX_OUTPUT_UNKNOWN_header               \
   0

static inline void
AGX_OUTPUT_UNKNOWN_pack(uint32_t * restrict cl,
                        const struct AGX_OUTPUT_UNKNOWN * restrict values)
{
   cl[ 0] = 0;
}


#define AGX_OUTPUT_UNKNOWN_LENGTH 4
struct agx_output_unknown_packed { uint32_t opaque[1]; };
static inline void
AGX_OUTPUT_UNKNOWN_unpack(FILE *fp, const uint8_t * restrict cl,
                          struct AGX_OUTPUT_UNKNOWN * restrict values)
{
   if (((const uint32_t *) cl)[0] & 0xffffffff) fprintf(fp, "XXX: Unknown field of Output Unknown unpacked at word 0: got %X, bad mask %X\n", ((const uint32_t *) cl)[0], ((const uint32_t *) cl)[0] & 0xffffffff);
}

static inline void
AGX_OUTPUT_UNKNOWN_print(FILE *fp, const struct AGX_OUTPUT_UNKNOWN * values, unsigned indent)
{
}

struct AGX_OUTPUT_SIZE {
   uint32_t                             count;
};

#define AGX_OUTPUT_SIZE_header                  \
   0

static inline void
AGX_OUTPUT_SIZE_pack(uint32_t * restrict cl,
                     const struct AGX_OUTPUT_SIZE * restrict values)
{
   cl[ 0] = util_bitpack_uint(values->count, 0, 31);
}


#define AGX_OUTPUT_SIZE_LENGTH 4
struct agx_output_size_packed { uint32_t opaque[1]; };
static inline void
AGX_OUTPUT_SIZE_unpack(FILE *fp, const uint8_t * restrict cl,
                       struct AGX_OUTPUT_SIZE * restrict values)
{
   values->count = __gen_unpack_uint(cl, 0, 31);
}

static inline void
AGX_OUTPUT_SIZE_print(FILE *fp, const struct AGX_OUTPUT_SIZE * values, unsigned indent)
{
   fprintf(fp, "%*sCount: %u\n", indent, "", values->count);
}

struct AGX_DEPTH_BIAS_SCISSOR {
   uint32_t                             scissor;
   uint32_t                             depth_bias;
};

#define AGX_DEPTH_BIAS_SCISSOR_header           \
   0

static inline void
AGX_DEPTH_BIAS_SCISSOR_pack(uint32_t * restrict cl,
                            const struct AGX_DEPTH_BIAS_SCISSOR * restrict values)
{
   cl[ 0] = util_bitpack_uint(values->scissor, 0, 15) |
            util_bitpack_uint(values->depth_bias, 16, 31);
}


#define AGX_DEPTH_BIAS_SCISSOR_LENGTH 4
struct agx_depth_bias_scissor_packed { uint32_t opaque[1]; };
static inline void
AGX_DEPTH_BIAS_SCISSOR_unpack(FILE *fp, const uint8_t * restrict cl,
                              struct AGX_DEPTH_BIAS_SCISSOR * restrict values)
{
   values->scissor = __gen_unpack_uint(cl, 0, 15);
   values->depth_bias = __gen_unpack_uint(cl, 16, 31);
}

static inline void
AGX_DEPTH_BIAS_SCISSOR_print(FILE *fp, const struct AGX_DEPTH_BIAS_SCISSOR * values, unsigned indent)
{
   fprintf(fp, "%*sScissor: %u\n", indent, "", values->scissor);
   fprintf(fp, "%*sDepth bias: %u\n", indent, "", values->depth_bias);
}

struct AGX_FRAGMENT_SHADER {
   uint32_t                             unknown_0;
   uint32_t                             uniform_register_count;
   uint32_t                             texture_state_register_count;
   enum agx_sampler_states              sampler_state_register_count;
   uint32_t                             preshader_register_count;
   uint32_t                             cf_binding_count;
   uint32_t                             unknown_10;
   uint64_t                             pipeline;
   uint64_t                             cf_bindings;
   uint32_t                             unknown_30;
};

#define AGX_FRAGMENT_SHADER_header              \
   .unknown_0 = 0

static inline void
AGX_FRAGMENT_SHADER_pack(uint32_t * restrict cl,
                         const struct AGX_FRAGMENT_SHADER * restrict values)
{
   assert((values->pipeline & 0x3f) == 0);
   assert((values->cf_bindings & 0x3) == 0);
   cl[ 0] = util_bitpack_uint(values->unknown_0, 0, 0) |
            util_bitpack_uint(__gen_to_groups(values->uniform_register_count, 64, 3), 1, 3) |
            util_bitpack_uint(__gen_to_groups(values->texture_state_register_count, 8, 5), 4, 8) |
            util_bitpack_uint(values->sampler_state_register_count, 9, 11) |
            util_bitpack_uint(__gen_to_groups(values->preshader_register_count, 16, 4), 12, 15) |
            util_bitpack_uint(values->cf_binding_count, 16, 22);
   cl[ 1] = util_bitpack_uint(values->unknown_10, 0, 1) |
            util_bitpack_uint(values->pipeline >> 6, 6, 31);
   cl[ 2] = util_bitpack_uint(values->cf_bindings >> 2, 2, 31);
   cl[ 3] = util_bitpack_uint(values->unknown_30, 0, 3);
}


#define AGX_FRAGMENT_SHADER_LENGTH 16
struct agx_fragment_shader_packed { uint32_t opaque[4]; };
static inline void
AGX_FRAGMENT_SHADER_unpack(FILE *fp, const uint8_t * restrict cl,
                           struct AGX_FRAGMENT_SHADER * restrict values)
{
   if (((const uint32_t *) cl)[0] & 0xff800000) fprintf(fp, "XXX: Unknown field of Fragment shader unpacked at word 0: got %X, bad mask %X\n", ((const uint32_t *) cl)[0], ((const uint32_t *) cl)[0] & 0xff800000);
   if (((const uint32_t *) cl)[1] & 0x3c) fprintf(fp, "XXX: Unknown field of Fragment shader unpacked at word 1: got %X, bad mask %X\n", ((const uint32_t *) cl)[1], ((const uint32_t *) cl)[1] & 0x3c);
   if (((const uint32_t *) cl)[2] & 0x3) fprintf(fp, "XXX: Unknown field of Fragment shader unpacked at word 2: got %X, bad mask %X\n", ((const uint32_t *) cl)[2], ((const uint32_t *) cl)[2] & 0x3);
   if (((const uint32_t *) cl)[3] & 0xfffffff0) fprintf(fp, "XXX: Unknown field of Fragment shader unpacked at word 3: got %X, bad mask %X\n", ((const uint32_t *) cl)[3], ((const uint32_t *) cl)[3] & 0xfffffff0);
   values->unknown_0 = __gen_unpack_uint(cl, 0, 0);
   values->uniform_register_count = __gen_from_groups(__gen_unpack_uint(cl, 1, 3), 64, 3);
   values->texture_state_register_count = __gen_from_groups(__gen_unpack_uint(cl, 4, 8), 8, 5);
   values->sampler_state_register_count = (enum agx_sampler_states) __gen_unpack_uint(cl, 9, 11);
   values->preshader_register_count = __gen_from_groups(__gen_unpack_uint(cl, 12, 15), 16, 4);
   values->cf_binding_count = __gen_unpack_uint(cl, 16, 22);
   values->unknown_10 = __gen_unpack_uint(cl, 32, 33);
   values->pipeline = __gen_unpack_uint(cl, 38, 63) << 6;
   values->cf_bindings = __gen_unpack_uint(cl, 66, 95) << 2;
   values->unknown_30 = __gen_unpack_uint(cl, 96, 99);
}

static inline void
AGX_FRAGMENT_SHADER_print(FILE *fp, const struct AGX_FRAGMENT_SHADER * values, unsigned indent)
{
   fprintf(fp, "%*sUnknown 0: 0x%" PRIx32 "\n", indent, "", values->unknown_0);
   fprintf(fp, "%*sUniform register count: %u\n", indent, "", values->uniform_register_count);
   fprintf(fp, "%*sTexture state register count: %u\n", indent, "", values->texture_state_register_count);
   if (agx_sampler_states_as_str(values->sampler_state_register_count))
     fprintf(fp, "%*sSampler state register count: %s\n", indent, "", agx_sampler_states_as_str(values->sampler_state_register_count));
    else
     fprintf(fp, "%*sSampler state register count: unknown %X (XXX)\n", indent, "", values->sampler_state_register_count);
   fprintf(fp, "%*sPreshader register count: %u\n", indent, "", values->preshader_register_count);
   fprintf(fp, "%*sCF binding count: %u\n", indent, "", values->cf_binding_count);
   fprintf(fp, "%*sUnknown 1:0: 0x%" PRIx32 "\n", indent, "", values->unknown_10);
   fprintf(fp, "%*sPipeline: 0x%" PRIx64 "\n", indent, "", values->pipeline);
   fprintf(fp, "%*sCF bindings: 0x%" PRIx64 "\n", indent, "", values->cf_bindings);
   fprintf(fp, "%*sUnknown 3:0: 0x%" PRIx32 "\n", indent, "", values->unknown_30);
}

enum agx_usc_control {
        AGX_USC_CONTROL_PRESHADER            =     56,
        AGX_USC_CONTROL_FRAGMENT_PROPERTIES  =     88,
        AGX_USC_CONTROL_NO_PRESHADER         =    136,
        AGX_USC_CONTROL_SHADER               =     13,
        AGX_USC_CONTROL_UNIFORM              =     29,
        AGX_USC_CONTROL_UNIFORM_HIGH         =     61,
        AGX_USC_CONTROL_SHARED               =     77,
        AGX_USC_CONTROL_REGISTERS            =    141,
        AGX_USC_CONTROL_SAMPLER              =    157,
        AGX_USC_CONTROL_TEXTURE              =    221,
};

static inline const char *
agx_usc_control_as_str(enum agx_usc_control imm)
{
    switch (imm) {
    case AGX_USC_CONTROL_PRESHADER: return "Preshader";
    case AGX_USC_CONTROL_FRAGMENT_PROPERTIES: return "Fragment properties";
    case AGX_USC_CONTROL_NO_PRESHADER: return "No preshader";
    case AGX_USC_CONTROL_SHADER: return "Shader";
    case AGX_USC_CONTROL_UNIFORM: return "Uniform";
    case AGX_USC_CONTROL_UNIFORM_HIGH: return "Uniform high";
    case AGX_USC_CONTROL_SHARED: return "Shared";
    case AGX_USC_CONTROL_REGISTERS: return "Registers";
    case AGX_USC_CONTROL_SAMPLER: return "Sampler";
    case AGX_USC_CONTROL_TEXTURE: return "Texture";
    default: break;
    }
    return NULL;
}

struct AGX_USC_UNIFORM {
   enum agx_usc_control                 tag;
   uint32_t                             start_halfs;
   uint32_t                             size_halfs;
   uint64_t                             buffer;
};

#define AGX_USC_UNIFORM_header                  \
   .tag = AGX_USC_CONTROL_UNIFORM

static inline void
AGX_USC_UNIFORM_pack(uint32_t * restrict cl,
                     const struct AGX_USC_UNIFORM * restrict values)
{
   assert((values->buffer & 0x3) == 0);
   cl[ 0] = util_bitpack_uint(values->tag, 0, 7) |
            util_bitpack_uint(values->start_halfs, 8, 15) |
            util_bitpack_uint(__gen_to_groups(values->size_halfs, 1, 6), 20, 25) |
            util_bitpack_uint(values->buffer >> 2, 26, 63);
   cl[ 1] = util_bitpack_uint(values->buffer >> 2, 26, 63) >> 32;
}


#define AGX_USC_UNIFORM_LENGTH 8
struct agx_usc_uniform_packed { uint32_t opaque[2]; };
static inline void
AGX_USC_UNIFORM_unpack(FILE *fp, const uint8_t * restrict cl,
                       struct AGX_USC_UNIFORM * restrict values)
{
   if (((const uint32_t *) cl)[0] & 0xf0000) fprintf(fp, "XXX: Unknown field of USC Uniform unpacked at word 0: got %X, bad mask %X\n", ((const uint32_t *) cl)[0], ((const uint32_t *) cl)[0] & 0xf0000);
   values->tag = (enum agx_usc_control) __gen_unpack_uint(cl, 0, 7);
   values->start_halfs = __gen_unpack_uint(cl, 8, 15);
   values->size_halfs = __gen_from_groups(__gen_unpack_uint(cl, 20, 25), 1, 6);
   values->buffer = __gen_unpack_uint(cl, 26, 63) << 2;
}

static inline void
AGX_USC_UNIFORM_print(FILE *fp, const struct AGX_USC_UNIFORM * values, unsigned indent)
{
   if (agx_usc_control_as_str(values->tag))
     fprintf(fp, "%*sTag: %s\n", indent, "", agx_usc_control_as_str(values->tag));
    else
     fprintf(fp, "%*sTag: unknown %X (XXX)\n", indent, "", values->tag);
   fprintf(fp, "%*sStart (halfs): %u\n", indent, "", values->start_halfs);
   fprintf(fp, "%*sSize (halfs): %u\n", indent, "", values->size_halfs);
   fprintf(fp, "%*sBuffer: 0x%" PRIx64 "\n", indent, "", values->buffer);
}

struct AGX_USC_UNIFORM_HIGH {
   enum agx_usc_control                 tag;
   uint32_t                             start_halfs;
   uint32_t                             size_halfs;
   uint64_t                             buffer;
};

#define AGX_USC_UNIFORM_HIGH_header             \
   .tag = AGX_USC_CONTROL_UNIFORM_HIGH

static inline void
AGX_USC_UNIFORM_HIGH_pack(uint32_t * restrict cl,
                          const struct AGX_USC_UNIFORM_HIGH * restrict values)
{
   assert((values->buffer & 0x3) == 0);
   cl[ 0] = util_bitpack_uint(values->tag, 0, 7) |
            util_bitpack_uint(values->start_halfs, 8, 15) |
            util_bitpack_uint(__gen_to_groups(values->size_halfs, 1, 6), 20, 25) |
            util_bitpack_uint(values->buffer >> 2, 26, 63);
   cl[ 1] = util_bitpack_uint(values->buffer >> 2, 26, 63) >> 32;
}


#define AGX_USC_UNIFORM_HIGH_LENGTH 8
struct agx_usc_uniform_high_packed { uint32_t opaque[2]; };
static inline void
AGX_USC_UNIFORM_HIGH_unpack(FILE *fp, const uint8_t * restrict cl,
                            struct AGX_USC_UNIFORM_HIGH * restrict values)
{
   if (((const uint32_t *) cl)[0] & 0xf0000) fprintf(fp, "XXX: Unknown field of USC Uniform High unpacked at word 0: got %X, bad mask %X\n", ((const uint32_t *) cl)[0], ((const uint32_t *) cl)[0] & 0xf0000);
   values->tag = (enum agx_usc_control) __gen_unpack_uint(cl, 0, 7);
   values->start_halfs = __gen_unpack_uint(cl, 8, 15);
   values->size_halfs = __gen_from_groups(__gen_unpack_uint(cl, 20, 25), 1, 6);
   values->buffer = __gen_unpack_uint(cl, 26, 63) << 2;
}

static inline void
AGX_USC_UNIFORM_HIGH_print(FILE *fp, const struct AGX_USC_UNIFORM_HIGH * values, unsigned indent)
{
   if (agx_usc_control_as_str(values->tag))
     fprintf(fp, "%*sTag: %s\n", indent, "", agx_usc_control_as_str(values->tag));
    else
     fprintf(fp, "%*sTag: unknown %X (XXX)\n", indent, "", values->tag);
   fprintf(fp, "%*sStart (halfs): %u\n", indent, "", values->start_halfs);
   fprintf(fp, "%*sSize (halfs): %u\n", indent, "", values->size_halfs);
   fprintf(fp, "%*sBuffer: 0x%" PRIx64 "\n", indent, "", values->buffer);
}

struct AGX_USC_TEXTURE {
   enum agx_usc_control                 tag;
   uint32_t                             start;
   uint32_t                             count;
   uint64_t                             buffer;
};

#define AGX_USC_TEXTURE_header                  \
   .tag = AGX_USC_CONTROL_TEXTURE

static inline void
AGX_USC_TEXTURE_pack(uint32_t * restrict cl,
                     const struct AGX_USC_TEXTURE * restrict values)
{
   assert((values->buffer & 0x7) == 0);
   cl[ 0] = util_bitpack_uint(values->tag, 0, 7) |
            util_bitpack_uint(values->start, 8, 15) |
            util_bitpack_uint(values->count, 20, 26) |
            util_bitpack_uint(values->buffer >> 3, 27, 62);
   cl[ 1] = util_bitpack_uint(values->buffer >> 3, 27, 62) >> 32;
}


#define AGX_USC_TEXTURE_LENGTH 8
struct agx_usc_texture_packed { uint32_t opaque[2]; };
static inline void
AGX_USC_TEXTURE_unpack(FILE *fp, const uint8_t * restrict cl,
                       struct AGX_USC_TEXTURE * restrict values)
{
   if (((const uint32_t *) cl)[0] & 0xf0000) fprintf(fp, "XXX: Unknown field of USC Texture unpacked at word 0: got %X, bad mask %X\n", ((const uint32_t *) cl)[0], ((const uint32_t *) cl)[0] & 0xf0000);
   if (((const uint32_t *) cl)[1] & 0x80000000) fprintf(fp, "XXX: Unknown field of USC Texture unpacked at word 1: got %X, bad mask %X\n", ((const uint32_t *) cl)[1], ((const uint32_t *) cl)[1] & 0x80000000);
   values->tag = (enum agx_usc_control) __gen_unpack_uint(cl, 0, 7);
   values->start = __gen_unpack_uint(cl, 8, 15);
   values->count = __gen_unpack_uint(cl, 20, 26);
   values->buffer = __gen_unpack_uint(cl, 27, 62) << 3;
}

static inline void
AGX_USC_TEXTURE_print(FILE *fp, const struct AGX_USC_TEXTURE * values, unsigned indent)
{
   if (agx_usc_control_as_str(values->tag))
     fprintf(fp, "%*sTag: %s\n", indent, "", agx_usc_control_as_str(values->tag));
    else
     fprintf(fp, "%*sTag: unknown %X (XXX)\n", indent, "", values->tag);
   fprintf(fp, "%*sStart: %u\n", indent, "", values->start);
   fprintf(fp, "%*sCount: %u\n", indent, "", values->count);
   fprintf(fp, "%*sBuffer: 0x%" PRIx64 "\n", indent, "", values->buffer);
}

struct AGX_USC_SAMPLER {
   enum agx_usc_control                 tag;
   uint32_t                             start;
   uint32_t                             count;
   uint64_t                             buffer;
};

#define AGX_USC_SAMPLER_header                  \
   .tag = AGX_USC_CONTROL_SAMPLER

static inline void
AGX_USC_SAMPLER_pack(uint32_t * restrict cl,
                     const struct AGX_USC_SAMPLER * restrict values)
{
   assert((values->buffer & 0x7) == 0);
   cl[ 0] = util_bitpack_uint(values->tag, 0, 7) |
            util_bitpack_uint(values->start, 8, 15) |
            util_bitpack_uint(values->count, 20, 26) |
            util_bitpack_uint(values->buffer >> 3, 27, 62);
   cl[ 1] = util_bitpack_uint(values->buffer >> 3, 27, 62) >> 32;
}


#define AGX_USC_SAMPLER_LENGTH 8
struct agx_usc_sampler_packed { uint32_t opaque[2]; };
static inline void
AGX_USC_SAMPLER_unpack(FILE *fp, const uint8_t * restrict cl,
                       struct AGX_USC_SAMPLER * restrict values)
{
   if (((const uint32_t *) cl)[0] & 0xf0000) fprintf(fp, "XXX: Unknown field of USC Sampler unpacked at word 0: got %X, bad mask %X\n", ((const uint32_t *) cl)[0], ((const uint32_t *) cl)[0] & 0xf0000);
   if (((const uint32_t *) cl)[1] & 0x80000000) fprintf(fp, "XXX: Unknown field of USC Sampler unpacked at word 1: got %X, bad mask %X\n", ((const uint32_t *) cl)[1], ((const uint32_t *) cl)[1] & 0x80000000);
   values->tag = (enum agx_usc_control) __gen_unpack_uint(cl, 0, 7);
   values->start = __gen_unpack_uint(cl, 8, 15);
   values->count = __gen_unpack_uint(cl, 20, 26);
   values->buffer = __gen_unpack_uint(cl, 27, 62) << 3;
}

static inline void
AGX_USC_SAMPLER_print(FILE *fp, const struct AGX_USC_SAMPLER * values, unsigned indent)
{
   if (agx_usc_control_as_str(values->tag))
     fprintf(fp, "%*sTag: %s\n", indent, "", agx_usc_control_as_str(values->tag));
    else
     fprintf(fp, "%*sTag: unknown %X (XXX)\n", indent, "", values->tag);
   fprintf(fp, "%*sStart: %u\n", indent, "", values->start);
   fprintf(fp, "%*sCount: %u\n", indent, "", values->count);
   fprintf(fp, "%*sBuffer: 0x%" PRIx64 "\n", indent, "", values->buffer);
}

enum agx_shared_layout {
        AGX_SHARED_LAYOUT_VERTEX_COMPUTE     =     36,
        AGX_SHARED_LAYOUT_32X32              =     47,
        AGX_SHARED_LAYOUT_32X16              =     63,
        AGX_SHARED_LAYOUT_16X16              =     54,
};

static inline const char *
agx_shared_layout_as_str(enum agx_shared_layout imm)
{
    switch (imm) {
    case AGX_SHARED_LAYOUT_VERTEX_COMPUTE: return "Vertex/compute";
    case AGX_SHARED_LAYOUT_32X32: return "32x32";
    case AGX_SHARED_LAYOUT_32X16: return "32x16";
    case AGX_SHARED_LAYOUT_16X16: return "16x16";
    default: break;
    }
    return NULL;
}

struct AGX_USC_SHARED {
   enum agx_usc_control                 tag;
   bool                                 uses_shared_memory;
   enum agx_shared_layout               layout;
   uint32_t                             sample_count;
   uint32_t                             sample_stride_in_8_bytes;
   uint32_t                             bytes_per_threadgroup;
};

#define AGX_USC_SHARED_header                   \
   .tag = AGX_USC_CONTROL_SHARED,  \
   .sample_count = 1

static inline void
AGX_USC_SHARED_pack(uint32_t * restrict cl,
                    const struct AGX_USC_SHARED * restrict values)
{
   assert(util_is_power_of_two_nonzero(values->sample_count));
   cl[ 0] = util_bitpack_uint(values->tag, 0, 7) |
            util_bitpack_uint(values->uses_shared_memory, 8, 8) |
            util_bitpack_uint(values->layout, 10, 15) |
            util_bitpack_uint(util_logbase2(values->sample_count), 16, 17) |
            util_bitpack_uint(values->sample_stride_in_8_bytes, 20, 23) |
            util_bitpack_uint(__gen_to_groups(values->bytes_per_threadgroup, 256, 8), 24, 31);
}


#define AGX_USC_SHARED_LENGTH 4
struct agx_usc_shared_packed { uint32_t opaque[1]; };
static inline void
AGX_USC_SHARED_unpack(FILE *fp, const uint8_t * restrict cl,
                      struct AGX_USC_SHARED * restrict values)
{
   if (((const uint32_t *) cl)[0] & 0xc0200) fprintf(fp, "XXX: Unknown field of USC Shared unpacked at word 0: got %X, bad mask %X\n", ((const uint32_t *) cl)[0], ((const uint32_t *) cl)[0] & 0xc0200);
   values->tag = (enum agx_usc_control) __gen_unpack_uint(cl, 0, 7);
   values->uses_shared_memory = __gen_unpack_uint(cl, 8, 8);
   values->layout = (enum agx_shared_layout) __gen_unpack_uint(cl, 10, 15);
   values->sample_count = 1 << __gen_unpack_uint(cl, 16, 17);
   values->sample_stride_in_8_bytes = __gen_unpack_uint(cl, 20, 23);
   values->bytes_per_threadgroup = __gen_from_groups(__gen_unpack_uint(cl, 24, 31), 256, 8);
}

static inline void
AGX_USC_SHARED_print(FILE *fp, const struct AGX_USC_SHARED * values, unsigned indent)
{
   if (agx_usc_control_as_str(values->tag))
     fprintf(fp, "%*sTag: %s\n", indent, "", agx_usc_control_as_str(values->tag));
    else
     fprintf(fp, "%*sTag: unknown %X (XXX)\n", indent, "", values->tag);
   fprintf(fp, "%*sUses shared memory: %s\n", indent, "", values->uses_shared_memory ? "true" : "false");
   if (agx_shared_layout_as_str(values->layout))
     fprintf(fp, "%*sLayout: %s\n", indent, "", agx_shared_layout_as_str(values->layout));
    else
     fprintf(fp, "%*sLayout: unknown %X (XXX)\n", indent, "", values->layout);
   fprintf(fp, "%*sSample count: %u\n", indent, "", values->sample_count);
   fprintf(fp, "%*sSample stride in 8 bytes: %u\n", indent, "", values->sample_stride_in_8_bytes);
   fprintf(fp, "%*sBytes per threadgroup: %u\n", indent, "", values->bytes_per_threadgroup);
}

struct AGX_USC_SHADER {
   enum agx_usc_control                 tag;
   bool                                 loads_varyings;
   bool                                 unk_1;
   uint32_t                             unk_2;
   uint64_t                             code;
};

#define AGX_USC_SHADER_header                   \
   .tag = AGX_USC_CONTROL_SHADER,  \
   .unk_1 = 0,  \
   .unk_2 = 3

static inline void
AGX_USC_SHADER_pack(uint32_t * restrict cl,
                    const struct AGX_USC_SHADER * restrict values)
{
   cl[ 0] = util_bitpack_uint(values->tag, 0, 7) |
            util_bitpack_uint(values->loads_varyings, 8, 8) |
            util_bitpack_uint(values->unk_1, 9, 9) |
            util_bitpack_uint(values->unk_2, 10, 15) |
            util_bitpack_uint(values->code, 16, 47);
   cl[ 1] = util_bitpack_uint(values->code, 16, 47) >> 32;
}


#define AGX_USC_SHADER_LENGTH 6
struct agx_usc_shader_packed { uint32_t opaque[1]; };
static inline void
AGX_USC_SHADER_unpack(FILE *fp, const uint8_t * restrict cl,
                      struct AGX_USC_SHADER * restrict values)
{
   values->tag = (enum agx_usc_control) __gen_unpack_uint(cl, 0, 7);
   values->loads_varyings = __gen_unpack_uint(cl, 8, 8);
   values->unk_1 = __gen_unpack_uint(cl, 9, 9);
   values->unk_2 = __gen_unpack_uint(cl, 10, 15);
   values->code = __gen_unpack_uint(cl, 16, 47);
}

static inline void
AGX_USC_SHADER_print(FILE *fp, const struct AGX_USC_SHADER * values, unsigned indent)
{
   if (agx_usc_control_as_str(values->tag))
     fprintf(fp, "%*sTag: %s\n", indent, "", agx_usc_control_as_str(values->tag));
    else
     fprintf(fp, "%*sTag: unknown %X (XXX)\n", indent, "", values->tag);
   fprintf(fp, "%*sLoads varyings: %s\n", indent, "", values->loads_varyings ? "true" : "false");
   fprintf(fp, "%*sUnk 1: %s\n", indent, "", values->unk_1 ? "true" : "false");
   fprintf(fp, "%*sUnk 2: %u\n", indent, "", values->unk_2);
   fprintf(fp, "%*sCode: 0x%" PRIx64 "\n", indent, "", values->code);
}

struct AGX_USC_REGISTERS {
   enum agx_usc_control                 tag;
   uint32_t                             register_count;
   bool                                 unk_1;
   uint32_t                             spill_size;
   uint32_t                             unk_4;
};

#define AGX_USC_REGISTERS_header                \
   .tag = AGX_USC_CONTROL_REGISTERS,  \
   .unk_1 = false,  \
   .spill_size = 0,  \
   .unk_4 = 0x1

static inline void
AGX_USC_REGISTERS_pack(uint32_t * restrict cl,
                       const struct AGX_USC_REGISTERS * restrict values)
{
   cl[ 0] = util_bitpack_uint(values->tag, 0, 7) |
            util_bitpack_uint(__gen_to_groups(values->register_count, 8, 5), 8, 12) |
            util_bitpack_uint(values->unk_1, 13, 13) |
            util_bitpack_uint(values->spill_size, 16, 23) |
            util_bitpack_uint(values->unk_4, 24, 31);
}


#define AGX_USC_REGISTERS_LENGTH 4
struct agx_usc_registers_packed { uint32_t opaque[1]; };
static inline void
AGX_USC_REGISTERS_unpack(FILE *fp, const uint8_t * restrict cl,
                         struct AGX_USC_REGISTERS * restrict values)
{
   if (((const uint32_t *) cl)[0] & 0xc000) fprintf(fp, "XXX: Unknown field of USC Registers unpacked at word 0: got %X, bad mask %X\n", ((const uint32_t *) cl)[0], ((const uint32_t *) cl)[0] & 0xc000);
   values->tag = (enum agx_usc_control) __gen_unpack_uint(cl, 0, 7);
   values->register_count = __gen_from_groups(__gen_unpack_uint(cl, 8, 12), 8, 5);
   values->unk_1 = __gen_unpack_uint(cl, 13, 13);
   values->spill_size = __gen_unpack_uint(cl, 16, 23);
   values->unk_4 = __gen_unpack_uint(cl, 24, 31);
}

static inline void
AGX_USC_REGISTERS_print(FILE *fp, const struct AGX_USC_REGISTERS * values, unsigned indent)
{
   if (agx_usc_control_as_str(values->tag))
     fprintf(fp, "%*sTag: %s\n", indent, "", agx_usc_control_as_str(values->tag));
    else
     fprintf(fp, "%*sTag: unknown %X (XXX)\n", indent, "", values->tag);
   fprintf(fp, "%*sRegister count: %u\n", indent, "", values->register_count);
   fprintf(fp, "%*sUnk 1: %s\n", indent, "", values->unk_1 ? "true" : "false");
   fprintf(fp, "%*sSpill size: 0x%" PRIx32 "\n", indent, "", values->spill_size);
   fprintf(fp, "%*sUnk 4: 0x%" PRIx32 "\n", indent, "", values->unk_4);
}

struct AGX_USC_NO_PRESHADER {
   enum agx_usc_control                 tag;
};

#define AGX_USC_NO_PRESHADER_header             \
   .tag = AGX_USC_CONTROL_NO_PRESHADER

static inline void
AGX_USC_NO_PRESHADER_pack(uint32_t * restrict cl,
                          const struct AGX_USC_NO_PRESHADER * restrict values)
{
   cl[ 0] = util_bitpack_uint(values->tag, 0, 7);
}


#define AGX_USC_NO_PRESHADER_LENGTH 2
struct agx_usc_no_preshader_packed { uint32_t opaque[0]; };
static inline void
AGX_USC_NO_PRESHADER_unpack(FILE *fp, const uint8_t * restrict cl,
                            struct AGX_USC_NO_PRESHADER * restrict values)
{
   values->tag = (enum agx_usc_control) __gen_unpack_uint(cl, 0, 7);
}

static inline void
AGX_USC_NO_PRESHADER_print(FILE *fp, const struct AGX_USC_NO_PRESHADER * values, unsigned indent)
{
   if (agx_usc_control_as_str(values->tag))
     fprintf(fp, "%*sTag: %s\n", indent, "", agx_usc_control_as_str(values->tag));
    else
     fprintf(fp, "%*sTag: unknown %X (XXX)\n", indent, "", values->tag);
}

struct AGX_USC_PRESHADER {
   enum agx_usc_control                 tag;
   uint32_t                             unk;
   uint64_t                             code;
};

#define AGX_USC_PRESHADER_header                \
   .tag = AGX_USC_CONTROL_PRESHADER,  \
   .unk = 0xc08000

static inline void
AGX_USC_PRESHADER_pack(uint32_t * restrict cl,
                       const struct AGX_USC_PRESHADER * restrict values)
{
   cl[ 0] = util_bitpack_uint(values->tag, 0, 7) |
            util_bitpack_uint(values->unk, 8, 31);
   cl[ 1] = util_bitpack_uint(values->code, 0, 31);
}


#define AGX_USC_PRESHADER_LENGTH 8
struct agx_usc_preshader_packed { uint32_t opaque[2]; };
static inline void
AGX_USC_PRESHADER_unpack(FILE *fp, const uint8_t * restrict cl,
                         struct AGX_USC_PRESHADER * restrict values)
{
   values->tag = (enum agx_usc_control) __gen_unpack_uint(cl, 0, 7);
   values->unk = __gen_unpack_uint(cl, 8, 31);
   values->code = __gen_unpack_uint(cl, 32, 63);
}

static inline void
AGX_USC_PRESHADER_print(FILE *fp, const struct AGX_USC_PRESHADER * values, unsigned indent)
{
   if (agx_usc_control_as_str(values->tag))
     fprintf(fp, "%*sTag: %s\n", indent, "", agx_usc_control_as_str(values->tag));
    else
     fprintf(fp, "%*sTag: unknown %X (XXX)\n", indent, "", values->tag);
   fprintf(fp, "%*sUnk: 0x%" PRIx32 "\n", indent, "", values->unk);
   fprintf(fp, "%*sCode: 0x%" PRIx64 "\n", indent, "", values->code);
}

struct AGX_USC_FRAGMENT_PROPERTIES {
   enum agx_usc_control                 tag;
   bool                                 early_z_testing;
   bool                                 unk_2;
   bool                                 unconditional_discard_1;
   bool                                 unconditional_discard_2;
   uint32_t                             unk_3;
   uint32_t                             unk_4;
   uint32_t                             unk_5;
};

#define AGX_USC_FRAGMENT_PROPERTIES_header      \
   .tag = AGX_USC_CONTROL_FRAGMENT_PROPERTIES,  \
   .unk_2 = true,  \
   .unk_3 = 0xf,  \
   .unk_4 = 0,  \
   .unk_5 = 0x01

static inline void
AGX_USC_FRAGMENT_PROPERTIES_pack(uint32_t * restrict cl,
                                 const struct AGX_USC_FRAGMENT_PROPERTIES * restrict values)
{
   cl[ 0] = util_bitpack_uint(values->tag, 0, 7) |
            util_bitpack_uint(values->early_z_testing, 8, 8) |
            util_bitpack_uint(values->unk_2, 9, 9) |
            util_bitpack_uint(values->unconditional_discard_1, 10, 10) |
            util_bitpack_uint(values->unconditional_discard_2, 11, 11) |
            util_bitpack_uint(values->unk_3, 12, 15) |
            util_bitpack_uint(values->unk_4, 16, 23) |
            util_bitpack_uint(values->unk_5, 24, 31);
}


#define AGX_USC_FRAGMENT_PROPERTIES_LENGTH 4
struct agx_usc_fragment_properties_packed { uint32_t opaque[1]; };
static inline void
AGX_USC_FRAGMENT_PROPERTIES_unpack(FILE *fp, const uint8_t * restrict cl,
                                   struct AGX_USC_FRAGMENT_PROPERTIES * restrict values)
{
   values->tag = (enum agx_usc_control) __gen_unpack_uint(cl, 0, 7);
   values->early_z_testing = __gen_unpack_uint(cl, 8, 8);
   values->unk_2 = __gen_unpack_uint(cl, 9, 9);
   values->unconditional_discard_1 = __gen_unpack_uint(cl, 10, 10);
   values->unconditional_discard_2 = __gen_unpack_uint(cl, 11, 11);
   values->unk_3 = __gen_unpack_uint(cl, 12, 15);
   values->unk_4 = __gen_unpack_uint(cl, 16, 23);
   values->unk_5 = __gen_unpack_uint(cl, 24, 31);
}

static inline void
AGX_USC_FRAGMENT_PROPERTIES_print(FILE *fp, const struct AGX_USC_FRAGMENT_PROPERTIES * values, unsigned indent)
{
   if (agx_usc_control_as_str(values->tag))
     fprintf(fp, "%*sTag: %s\n", indent, "", agx_usc_control_as_str(values->tag));
    else
     fprintf(fp, "%*sTag: unknown %X (XXX)\n", indent, "", values->tag);
   fprintf(fp, "%*sEarly-z testing: %s\n", indent, "", values->early_z_testing ? "true" : "false");
   fprintf(fp, "%*sUnk 2: %s\n", indent, "", values->unk_2 ? "true" : "false");
   fprintf(fp, "%*sUnconditional discard 1: %s\n", indent, "", values->unconditional_discard_1 ? "true" : "false");
   fprintf(fp, "%*sUnconditional discard 2: %s\n", indent, "", values->unconditional_discard_2 ? "true" : "false");
   fprintf(fp, "%*sUnk 3: 0x%" PRIx32 "\n", indent, "", values->unk_3);
   fprintf(fp, "%*sUnk 4: 0x%" PRIx32 "\n", indent, "", values->unk_4);
   fprintf(fp, "%*sUnk 5: 0x%" PRIx32 "\n", indent, "", values->unk_5);
}

enum agx_vdm_block_type {
        AGX_VDM_BLOCK_TYPE_PPP_STATE_UPDATE  =      0,
        AGX_VDM_BLOCK_TYPE_BARRIER           =      1,
        AGX_VDM_BLOCK_TYPE_VDM_STATE_UPDATE  =      2,
        AGX_VDM_BLOCK_TYPE_INDEX_LIST        =      3,
        AGX_VDM_BLOCK_TYPE_STREAM_LINK       =      4,
        AGX_VDM_BLOCK_TYPE_STREAM_TERMINATE  =      6,
};

static inline const char *
agx_vdm_block_type_as_str(enum agx_vdm_block_type imm)
{
    switch (imm) {
    case AGX_VDM_BLOCK_TYPE_PPP_STATE_UPDATE: return "PPP State Update";
    case AGX_VDM_BLOCK_TYPE_BARRIER: return "Barrier";
    case AGX_VDM_BLOCK_TYPE_VDM_STATE_UPDATE: return "VDM State Update";
    case AGX_VDM_BLOCK_TYPE_INDEX_LIST: return "Index List";
    case AGX_VDM_BLOCK_TYPE_STREAM_LINK: return "Stream Link";
    case AGX_VDM_BLOCK_TYPE_STREAM_TERMINATE: return "Stream terminate";
    default: break;
    }
    return NULL;
}

struct AGX_PPP_STATE {
   uint32_t                             pointer_hi;
   uint32_t                             size_words;
   enum agx_vdm_block_type              block_type;
   uint64_t                             pointer_lo;
};

#define AGX_PPP_STATE_header                    \
   .block_type = AGX_VDM_BLOCK_TYPE_PPP_STATE_UPDATE

static inline void
AGX_PPP_STATE_pack(uint32_t * restrict cl,
                   const struct AGX_PPP_STATE * restrict values)
{
   cl[ 0] = util_bitpack_uint(values->pointer_hi, 0, 7) |
            util_bitpack_uint(values->size_words, 8, 15) |
            util_bitpack_uint(values->block_type, 29, 31);
   cl[ 1] = util_bitpack_uint(values->pointer_lo, 0, 31);
}


#define AGX_PPP_STATE_LENGTH 8
struct agx_ppp_state_packed { uint32_t opaque[2]; };
static inline void
AGX_PPP_STATE_unpack(FILE *fp, const uint8_t * restrict cl,
                     struct AGX_PPP_STATE * restrict values)
{
   if (((const uint32_t *) cl)[0] & 0x1fff0000) fprintf(fp, "XXX: Unknown field of PPP State unpacked at word 0: got %X, bad mask %X\n", ((const uint32_t *) cl)[0], ((const uint32_t *) cl)[0] & 0x1fff0000);
   values->pointer_hi = __gen_unpack_uint(cl, 0, 7);
   values->size_words = __gen_unpack_uint(cl, 8, 15);
   values->block_type = (enum agx_vdm_block_type) __gen_unpack_uint(cl, 29, 31);
   values->pointer_lo = __gen_unpack_uint(cl, 32, 63);
}

static inline void
AGX_PPP_STATE_print(FILE *fp, const struct AGX_PPP_STATE * values, unsigned indent)
{
   fprintf(fp, "%*sPointer (hi): 0x%" PRIx32 "\n", indent, "", values->pointer_hi);
   fprintf(fp, "%*sSize (words): %u\n", indent, "", values->size_words);
   if (agx_vdm_block_type_as_str(values->block_type))
     fprintf(fp, "%*sBlock Type: %s\n", indent, "", agx_vdm_block_type_as_str(values->block_type));
    else
     fprintf(fp, "%*sBlock Type: unknown %X (XXX)\n", indent, "", values->block_type);
   fprintf(fp, "%*sPointer (lo): 0x%" PRIx64 "\n", indent, "", values->pointer_lo);
}

struct AGX_VDM_BARRIER {
   bool                                 usc_cache_inval;
   bool                                 unk_4;
   bool                                 unk_5;
   bool                                 unk_6;
   bool                                 unk_8;
   bool                                 unk_11;
   bool                                 unk_20;
   bool                                 unk_24;
   bool                                 unk_26;
   bool                                 returns;
   enum agx_vdm_block_type              block_type;
};

#define AGX_VDM_BARRIER_header                  \
   .usc_cache_inval = false,  \
   .unk_4 = false,  \
   .unk_5 = false,  \
   .unk_6 = false,  \
   .unk_8 = false,  \
   .unk_11 = false,  \
   .unk_20 = false,  \
   .unk_24 = false,  \
   .unk_26 = false,  \
   .returns = false,  \
   .block_type = AGX_VDM_BLOCK_TYPE_BARRIER

static inline void
AGX_VDM_BARRIER_pack(uint32_t * restrict cl,
                     const struct AGX_VDM_BARRIER * restrict values)
{
   cl[ 0] = util_bitpack_uint(values->usc_cache_inval, 3, 3) |
            util_bitpack_uint(values->unk_4, 4, 4) |
            util_bitpack_uint(values->unk_5, 5, 5) |
            util_bitpack_uint(values->unk_6, 6, 6) |
            util_bitpack_uint(values->unk_8, 8, 8) |
            util_bitpack_uint(values->unk_11, 11, 11) |
            util_bitpack_uint(values->unk_20, 20, 20) |
            util_bitpack_uint(values->unk_24, 24, 24) |
            util_bitpack_uint(values->unk_26, 26, 26) |
            util_bitpack_uint(values->returns, 27, 27) |
            util_bitpack_uint(values->block_type, 29, 31);
}


#define AGX_VDM_BARRIER_LENGTH 4
struct agx_vdm_barrier_packed { uint32_t opaque[1]; };
static inline void
AGX_VDM_BARRIER_unpack(FILE *fp, const uint8_t * restrict cl,
                       struct AGX_VDM_BARRIER * restrict values)
{
   if (((const uint32_t *) cl)[0] & 0x12eff687) fprintf(fp, "XXX: Unknown field of VDM Barrier unpacked at word 0: got %X, bad mask %X\n", ((const uint32_t *) cl)[0], ((const uint32_t *) cl)[0] & 0x12eff687);
   values->usc_cache_inval = __gen_unpack_uint(cl, 3, 3);
   values->unk_4 = __gen_unpack_uint(cl, 4, 4);
   values->unk_5 = __gen_unpack_uint(cl, 5, 5);
   values->unk_6 = __gen_unpack_uint(cl, 6, 6);
   values->unk_8 = __gen_unpack_uint(cl, 8, 8);
   values->unk_11 = __gen_unpack_uint(cl, 11, 11);
   values->unk_20 = __gen_unpack_uint(cl, 20, 20);
   values->unk_24 = __gen_unpack_uint(cl, 24, 24);
   values->unk_26 = __gen_unpack_uint(cl, 26, 26);
   values->returns = __gen_unpack_uint(cl, 27, 27);
   values->block_type = (enum agx_vdm_block_type) __gen_unpack_uint(cl, 29, 31);
}

static inline void
AGX_VDM_BARRIER_print(FILE *fp, const struct AGX_VDM_BARRIER * values, unsigned indent)
{
   fprintf(fp, "%*sUSC cache inval: %s\n", indent, "", values->usc_cache_inval ? "true" : "false");
   fprintf(fp, "%*sUnk 4: %s\n", indent, "", values->unk_4 ? "true" : "false");
   fprintf(fp, "%*sUnk 5: %s\n", indent, "", values->unk_5 ? "true" : "false");
   fprintf(fp, "%*sUnk 6: %s\n", indent, "", values->unk_6 ? "true" : "false");
   fprintf(fp, "%*sUnk 8: %s\n", indent, "", values->unk_8 ? "true" : "false");
   fprintf(fp, "%*sUnk 11: %s\n", indent, "", values->unk_11 ? "true" : "false");
   fprintf(fp, "%*sUnk 20: %s\n", indent, "", values->unk_20 ? "true" : "false");
   fprintf(fp, "%*sUnk 24: %s\n", indent, "", values->unk_24 ? "true" : "false");
   fprintf(fp, "%*sUnk 26: %s\n", indent, "", values->unk_26 ? "true" : "false");
   fprintf(fp, "%*sReturns: %s\n", indent, "", values->returns ? "true" : "false");
   if (agx_vdm_block_type_as_str(values->block_type))
     fprintf(fp, "%*sBlock Type: %s\n", indent, "", agx_vdm_block_type_as_str(values->block_type));
    else
     fprintf(fp, "%*sBlock Type: unknown %X (XXX)\n", indent, "", values->block_type);
}

enum agx_index_size {
        AGX_INDEX_SIZE_U8                    =      0,
        AGX_INDEX_SIZE_U16                   =      1,
        AGX_INDEX_SIZE_U32                   =      2,
};

static inline const char *
agx_index_size_as_str(enum agx_index_size imm)
{
    switch (imm) {
    case AGX_INDEX_SIZE_U8: return "U8";
    case AGX_INDEX_SIZE_U16: return "U16";
    case AGX_INDEX_SIZE_U32: return "U32";
    default: break;
    }
    return NULL;
}

struct AGX_VDM_STATE {
   bool                                 restart_index_present;
   bool                                 vertex_shader_word_0_present;
   bool                                 vertex_shader_word_1_present;
   bool                                 vertex_outputs_present;
   bool                                 vertex_unknown_present;
   enum agx_vdm_block_type              block_type;
};

#define AGX_VDM_STATE_header                    \
   .block_type = AGX_VDM_BLOCK_TYPE_VDM_STATE_UPDATE

static inline void
AGX_VDM_STATE_pack(uint32_t * restrict cl,
                   const struct AGX_VDM_STATE * restrict values)
{
   cl[ 0] = util_bitpack_uint(values->restart_index_present, 0, 0) |
            util_bitpack_uint(values->vertex_shader_word_0_present, 1, 1) |
            util_bitpack_uint(values->vertex_shader_word_1_present, 2, 2) |
            util_bitpack_uint(values->vertex_outputs_present, 3, 3) |
            util_bitpack_uint(values->vertex_unknown_present, 5, 5) |
            util_bitpack_uint(values->block_type, 29, 31);
}


#define AGX_VDM_STATE_LENGTH 4
struct agx_vdm_state_packed { uint32_t opaque[1]; };
static inline void
AGX_VDM_STATE_unpack(FILE *fp, const uint8_t * restrict cl,
                     struct AGX_VDM_STATE * restrict values)
{
   if (((const uint32_t *) cl)[0] & 0x1fffffd0) fprintf(fp, "XXX: Unknown field of VDM State unpacked at word 0: got %X, bad mask %X\n", ((const uint32_t *) cl)[0], ((const uint32_t *) cl)[0] & 0x1fffffd0);
   values->restart_index_present = __gen_unpack_uint(cl, 0, 0);
   values->vertex_shader_word_0_present = __gen_unpack_uint(cl, 1, 1);
   values->vertex_shader_word_1_present = __gen_unpack_uint(cl, 2, 2);
   values->vertex_outputs_present = __gen_unpack_uint(cl, 3, 3);
   values->vertex_unknown_present = __gen_unpack_uint(cl, 5, 5);
   values->block_type = (enum agx_vdm_block_type) __gen_unpack_uint(cl, 29, 31);
}

static inline void
AGX_VDM_STATE_print(FILE *fp, const struct AGX_VDM_STATE * values, unsigned indent)
{
   fprintf(fp, "%*sRestart index present: %s\n", indent, "", values->restart_index_present ? "true" : "false");
   fprintf(fp, "%*sVertex shader word 0 present: %s\n", indent, "", values->vertex_shader_word_0_present ? "true" : "false");
   fprintf(fp, "%*sVertex shader word 1 present: %s\n", indent, "", values->vertex_shader_word_1_present ? "true" : "false");
   fprintf(fp, "%*sVertex outputs present: %s\n", indent, "", values->vertex_outputs_present ? "true" : "false");
   fprintf(fp, "%*sVertex unknown present: %s\n", indent, "", values->vertex_unknown_present ? "true" : "false");
   if (agx_vdm_block_type_as_str(values->block_type))
     fprintf(fp, "%*sBlock Type: %s\n", indent, "", agx_vdm_block_type_as_str(values->block_type));
    else
     fprintf(fp, "%*sBlock Type: unknown %X (XXX)\n", indent, "", values->block_type);
}

struct AGX_VDM_STATE_RESTART_INDEX {
   uint32_t                             value;
};

#define AGX_VDM_STATE_RESTART_INDEX_header      \
   0

static inline void
AGX_VDM_STATE_RESTART_INDEX_pack(uint32_t * restrict cl,
                                 const struct AGX_VDM_STATE_RESTART_INDEX * restrict values)
{
   cl[ 0] = util_bitpack_uint(values->value, 0, 31);
}


#define AGX_VDM_STATE_RESTART_INDEX_LENGTH 4
struct agx_vdm_state_restart_index_packed { uint32_t opaque[1]; };
static inline void
AGX_VDM_STATE_RESTART_INDEX_unpack(FILE *fp, const uint8_t * restrict cl,
                                   struct AGX_VDM_STATE_RESTART_INDEX * restrict values)
{
   values->value = __gen_unpack_uint(cl, 0, 31);
}

static inline void
AGX_VDM_STATE_RESTART_INDEX_print(FILE *fp, const struct AGX_VDM_STATE_RESTART_INDEX * values, unsigned indent)
{
   fprintf(fp, "%*sValue: 0x%" PRIx32 "\n", indent, "", values->value);
}

struct AGX_VDM_STATE_VERTEX_SHADER_WORD_0 {
   uint32_t                             unknown_0;
   uint32_t                             uniform_register_count;
   uint32_t                             texture_state_register_count;
   enum agx_sampler_states              sampler_state_register_count;
   uint32_t                             preshader_register_count;
};

#define AGX_VDM_STATE_VERTEX_SHADER_WORD_0_header\
   .unknown_0 = 0

static inline void
AGX_VDM_STATE_VERTEX_SHADER_WORD_0_pack(uint32_t * restrict cl,
                                        const struct AGX_VDM_STATE_VERTEX_SHADER_WORD_0 * restrict values)
{
   cl[ 0] = util_bitpack_uint(values->unknown_0, 0, 0) |
            util_bitpack_uint(__gen_to_groups(values->uniform_register_count, 64, 3), 1, 3) |
            util_bitpack_uint(__gen_to_groups(values->texture_state_register_count, 8, 5), 4, 8) |
            util_bitpack_uint(values->sampler_state_register_count, 9, 11) |
            util_bitpack_uint(__gen_to_groups(values->preshader_register_count, 16, 4), 12, 15);
}


#define AGX_VDM_STATE_VERTEX_SHADER_WORD_0_LENGTH 4
struct agx_vdm_state_vertex_shader_word_0_packed { uint32_t opaque[1]; };
static inline void
AGX_VDM_STATE_VERTEX_SHADER_WORD_0_unpack(FILE *fp, const uint8_t * restrict cl,
                                          struct AGX_VDM_STATE_VERTEX_SHADER_WORD_0 * restrict values)
{
   if (((const uint32_t *) cl)[0] & 0xffff0000) fprintf(fp, "XXX: Unknown field of VDM State Vertex Shader Word 0 unpacked at word 0: got %X, bad mask %X\n", ((const uint32_t *) cl)[0], ((const uint32_t *) cl)[0] & 0xffff0000);
   values->unknown_0 = __gen_unpack_uint(cl, 0, 0);
   values->uniform_register_count = __gen_from_groups(__gen_unpack_uint(cl, 1, 3), 64, 3);
   values->texture_state_register_count = __gen_from_groups(__gen_unpack_uint(cl, 4, 8), 8, 5);
   values->sampler_state_register_count = (enum agx_sampler_states) __gen_unpack_uint(cl, 9, 11);
   values->preshader_register_count = __gen_from_groups(__gen_unpack_uint(cl, 12, 15), 16, 4);
}

static inline void
AGX_VDM_STATE_VERTEX_SHADER_WORD_0_print(FILE *fp, const struct AGX_VDM_STATE_VERTEX_SHADER_WORD_0 * values, unsigned indent)
{
   fprintf(fp, "%*sUnknown 0: 0x%" PRIx32 "\n", indent, "", values->unknown_0);
   fprintf(fp, "%*sUniform register count: %u\n", indent, "", values->uniform_register_count);
   fprintf(fp, "%*sTexture state register count: %u\n", indent, "", values->texture_state_register_count);
   if (agx_sampler_states_as_str(values->sampler_state_register_count))
     fprintf(fp, "%*sSampler state register count: %s\n", indent, "", agx_sampler_states_as_str(values->sampler_state_register_count));
    else
     fprintf(fp, "%*sSampler state register count: unknown %X (XXX)\n", indent, "", values->sampler_state_register_count);
   fprintf(fp, "%*sPreshader register count: %u\n", indent, "", values->preshader_register_count);
}

struct AGX_VDM_STATE_VERTEX_SHADER_WORD_1 {
   uint32_t                             unknown_10;
   uint64_t                             pipeline;
};

#define AGX_VDM_STATE_VERTEX_SHADER_WORD_1_header\
   0

static inline void
AGX_VDM_STATE_VERTEX_SHADER_WORD_1_pack(uint32_t * restrict cl,
                                        const struct AGX_VDM_STATE_VERTEX_SHADER_WORD_1 * restrict values)
{
   assert((values->pipeline & 0x3f) == 0);
   cl[ 0] = util_bitpack_uint(values->unknown_10, 0, 1) |
            util_bitpack_uint(values->pipeline >> 6, 6, 31);
}


#define AGX_VDM_STATE_VERTEX_SHADER_WORD_1_LENGTH 4
struct agx_vdm_state_vertex_shader_word_1_packed { uint32_t opaque[1]; };
static inline void
AGX_VDM_STATE_VERTEX_SHADER_WORD_1_unpack(FILE *fp, const uint8_t * restrict cl,
                                          struct AGX_VDM_STATE_VERTEX_SHADER_WORD_1 * restrict values)
{
   if (((const uint32_t *) cl)[0] & 0x3c) fprintf(fp, "XXX: Unknown field of VDM State Vertex Shader Word 1 unpacked at word 0: got %X, bad mask %X\n", ((const uint32_t *) cl)[0], ((const uint32_t *) cl)[0] & 0x3c);
   values->unknown_10 = __gen_unpack_uint(cl, 0, 1);
   values->pipeline = __gen_unpack_uint(cl, 6, 31) << 6;
}

static inline void
AGX_VDM_STATE_VERTEX_SHADER_WORD_1_print(FILE *fp, const struct AGX_VDM_STATE_VERTEX_SHADER_WORD_1 * values, unsigned indent)
{
   fprintf(fp, "%*sUnknown 1:0: 0x%" PRIx32 "\n", indent, "", values->unknown_10);
   fprintf(fp, "%*sPipeline: 0x%" PRIx64 "\n", indent, "", values->pipeline);
}

struct AGX_VDM_STATE_VERTEX_OUTPUTS {
   uint32_t                             output_count_1;
   uint32_t                             output_count_2;
};

#define AGX_VDM_STATE_VERTEX_OUTPUTS_header     \
   .output_count_1 = 0,  \
   .output_count_2 = 0

static inline void
AGX_VDM_STATE_VERTEX_OUTPUTS_pack(uint32_t * restrict cl,
                                  const struct AGX_VDM_STATE_VERTEX_OUTPUTS * restrict values)
{
   cl[ 0] = util_bitpack_uint(values->output_count_1, 0, 7) |
            util_bitpack_uint(values->output_count_2, 8, 15);
}


#define AGX_VDM_STATE_VERTEX_OUTPUTS_LENGTH 4
struct agx_vdm_state_vertex_outputs_packed { uint32_t opaque[1]; };
static inline void
AGX_VDM_STATE_VERTEX_OUTPUTS_unpack(FILE *fp, const uint8_t * restrict cl,
                                    struct AGX_VDM_STATE_VERTEX_OUTPUTS * restrict values)
{
   if (((const uint32_t *) cl)[0] & 0xffff0000) fprintf(fp, "XXX: Unknown field of VDM State Vertex Outputs unpacked at word 0: got %X, bad mask %X\n", ((const uint32_t *) cl)[0], ((const uint32_t *) cl)[0] & 0xffff0000);
   values->output_count_1 = __gen_unpack_uint(cl, 0, 7);
   values->output_count_2 = __gen_unpack_uint(cl, 8, 15);
}

static inline void
AGX_VDM_STATE_VERTEX_OUTPUTS_print(FILE *fp, const struct AGX_VDM_STATE_VERTEX_OUTPUTS * values, unsigned indent)
{
   fprintf(fp, "%*sOutput count 1: %u\n", indent, "", values->output_count_1);
   fprintf(fp, "%*sOutput count 2: %u\n", indent, "", values->output_count_2);
}

enum agx_vdm_vertex {
        AGX_VDM_VERTEX_0                     =      0,
        AGX_VDM_VERTEX_2                     =      2,
};

static inline const char *
agx_vdm_vertex_as_str(enum agx_vdm_vertex imm)
{
    switch (imm) {
    case AGX_VDM_VERTEX_0: return "0";
    case AGX_VDM_VERTEX_2: return "2";
    default: break;
    }
    return NULL;
}

struct AGX_VDM_STATE_VERTEX_UNKNOWN {
   enum agx_vdm_vertex                  flat_shading_control;
   bool                                 unknown_4;
   bool                                 unknown_5;
};

#define AGX_VDM_STATE_VERTEX_UNKNOWN_header     \
   0

static inline void
AGX_VDM_STATE_VERTEX_UNKNOWN_pack(uint32_t * restrict cl,
                                  const struct AGX_VDM_STATE_VERTEX_UNKNOWN * restrict values)
{
   cl[ 0] = util_bitpack_uint(values->flat_shading_control, 0, 1) |
            util_bitpack_uint(values->unknown_4, 4, 4) |
            util_bitpack_uint(values->unknown_5, 5, 5);
}


#define AGX_VDM_STATE_VERTEX_UNKNOWN_LENGTH 4
struct agx_vdm_state_vertex_unknown_packed { uint32_t opaque[1]; };
static inline void
AGX_VDM_STATE_VERTEX_UNKNOWN_unpack(FILE *fp, const uint8_t * restrict cl,
                                    struct AGX_VDM_STATE_VERTEX_UNKNOWN * restrict values)
{
   if (((const uint32_t *) cl)[0] & 0xffffffcc) fprintf(fp, "XXX: Unknown field of VDM State Vertex Unknown unpacked at word 0: got %X, bad mask %X\n", ((const uint32_t *) cl)[0], ((const uint32_t *) cl)[0] & 0xffffffcc);
   values->flat_shading_control = (enum agx_vdm_vertex) __gen_unpack_uint(cl, 0, 1);
   values->unknown_4 = __gen_unpack_uint(cl, 4, 4);
   values->unknown_5 = __gen_unpack_uint(cl, 5, 5);
}

static inline void
AGX_VDM_STATE_VERTEX_UNKNOWN_print(FILE *fp, const struct AGX_VDM_STATE_VERTEX_UNKNOWN * values, unsigned indent)
{
   if (agx_vdm_vertex_as_str(values->flat_shading_control))
     fprintf(fp, "%*sFlat shading control: %s\n", indent, "", agx_vdm_vertex_as_str(values->flat_shading_control));
    else
     fprintf(fp, "%*sFlat shading control: unknown %X (XXX)\n", indent, "", values->flat_shading_control);
   fprintf(fp, "%*sUnknown 4: %s\n", indent, "", values->unknown_4 ? "true" : "false");
   fprintf(fp, "%*sUnknown 5: %s\n", indent, "", values->unknown_5 ? "true" : "false");
}

struct AGX_INDEX_LIST {
   uint32_t                             index_buffer_hi;
   enum agx_primitive                   primitive;
   bool                                 restart_enable;
   enum agx_index_size                  index_size;
   bool                                 index_buffer_size_present;
   bool                                 index_buffer_present;
   bool                                 index_count_present;
   bool                                 instance_count_present;
   bool                                 start_present;
   bool                                 unk_1_present;
   bool                                 indirect_buffer_present;
   bool                                 unk_2_present;
   enum agx_vdm_block_type              block_type;
};

#define AGX_INDEX_LIST_header                   \
   .block_type = AGX_VDM_BLOCK_TYPE_INDEX_LIST

static inline void
AGX_INDEX_LIST_pack(uint32_t * restrict cl,
                    const struct AGX_INDEX_LIST * restrict values)
{
   cl[ 0] = util_bitpack_uint(values->index_buffer_hi, 0, 7) |
            util_bitpack_uint(values->primitive, 8, 15) |
            util_bitpack_uint(values->restart_enable, 16, 16) |
            util_bitpack_uint(values->index_size, 17, 19) |
            util_bitpack_uint(values->index_buffer_size_present, 20, 20) |
            util_bitpack_uint(values->index_buffer_present, 21, 21) |
            util_bitpack_uint(values->index_count_present, 22, 22) |
            util_bitpack_uint(values->instance_count_present, 23, 23) |
            util_bitpack_uint(values->start_present, 24, 24) |
            util_bitpack_uint(values->unk_1_present, 25, 25) |
            util_bitpack_uint(values->indirect_buffer_present, 26, 26) |
            util_bitpack_uint(values->unk_2_present, 27, 27) |
            util_bitpack_uint(values->block_type, 29, 31);
}


#define AGX_INDEX_LIST_LENGTH 4
struct agx_index_list_packed { uint32_t opaque[1]; };
static inline void
AGX_INDEX_LIST_unpack(FILE *fp, const uint8_t * restrict cl,
                      struct AGX_INDEX_LIST * restrict values)
{
   if (((const uint32_t *) cl)[0] & 0x10000000) fprintf(fp, "XXX: Unknown field of Index List unpacked at word 0: got %X, bad mask %X\n", ((const uint32_t *) cl)[0], ((const uint32_t *) cl)[0] & 0x10000000);
   values->index_buffer_hi = __gen_unpack_uint(cl, 0, 7);
   values->primitive = (enum agx_primitive) __gen_unpack_uint(cl, 8, 15);
   values->restart_enable = __gen_unpack_uint(cl, 16, 16);
   values->index_size = (enum agx_index_size) __gen_unpack_uint(cl, 17, 19);
   values->index_buffer_size_present = __gen_unpack_uint(cl, 20, 20);
   values->index_buffer_present = __gen_unpack_uint(cl, 21, 21);
   values->index_count_present = __gen_unpack_uint(cl, 22, 22);
   values->instance_count_present = __gen_unpack_uint(cl, 23, 23);
   values->start_present = __gen_unpack_uint(cl, 24, 24);
   values->unk_1_present = __gen_unpack_uint(cl, 25, 25);
   values->indirect_buffer_present = __gen_unpack_uint(cl, 26, 26);
   values->unk_2_present = __gen_unpack_uint(cl, 27, 27);
   values->block_type = (enum agx_vdm_block_type) __gen_unpack_uint(cl, 29, 31);
}

static inline void
AGX_INDEX_LIST_print(FILE *fp, const struct AGX_INDEX_LIST * values, unsigned indent)
{
   fprintf(fp, "%*sIndex buffer hi: 0x%" PRIx32 "\n", indent, "", values->index_buffer_hi);
   if (agx_primitive_as_str(values->primitive))
     fprintf(fp, "%*sPrimitive: %s\n", indent, "", agx_primitive_as_str(values->primitive));
    else
     fprintf(fp, "%*sPrimitive: unknown %X (XXX)\n", indent, "", values->primitive);
   fprintf(fp, "%*sRestart enable: %s\n", indent, "", values->restart_enable ? "true" : "false");
   if (agx_index_size_as_str(values->index_size))
     fprintf(fp, "%*sIndex size: %s\n", indent, "", agx_index_size_as_str(values->index_size));
    else
     fprintf(fp, "%*sIndex size: unknown %X (XXX)\n", indent, "", values->index_size);
   fprintf(fp, "%*sIndex buffer size present: %s\n", indent, "", values->index_buffer_size_present ? "true" : "false");
   fprintf(fp, "%*sIndex buffer present: %s\n", indent, "", values->index_buffer_present ? "true" : "false");
   fprintf(fp, "%*sIndex count present: %s\n", indent, "", values->index_count_present ? "true" : "false");
   fprintf(fp, "%*sInstance count present: %s\n", indent, "", values->instance_count_present ? "true" : "false");
   fprintf(fp, "%*sStart present: %s\n", indent, "", values->start_present ? "true" : "false");
   fprintf(fp, "%*sUnk 1 present: %s\n", indent, "", values->unk_1_present ? "true" : "false");
   fprintf(fp, "%*sIndirect buffer present: %s\n", indent, "", values->indirect_buffer_present ? "true" : "false");
   fprintf(fp, "%*sUnk 2 present: %s\n", indent, "", values->unk_2_present ? "true" : "false");
   if (agx_vdm_block_type_as_str(values->block_type))
     fprintf(fp, "%*sBlock Type: %s\n", indent, "", agx_vdm_block_type_as_str(values->block_type));
    else
     fprintf(fp, "%*sBlock Type: unknown %X (XXX)\n", indent, "", values->block_type);
}

struct AGX_INDEX_LIST_BUFFER_LO {
   uint32_t                             buffer_lo;
};

#define AGX_INDEX_LIST_BUFFER_LO_header         \
   0

static inline void
AGX_INDEX_LIST_BUFFER_LO_pack(uint32_t * restrict cl,
                              const struct AGX_INDEX_LIST_BUFFER_LO * restrict values)
{
   cl[ 0] = util_bitpack_uint(values->buffer_lo, 0, 31);
}


#define AGX_INDEX_LIST_BUFFER_LO_LENGTH 4
struct agx_index_list_buffer_lo_packed { uint32_t opaque[1]; };
static inline void
AGX_INDEX_LIST_BUFFER_LO_unpack(FILE *fp, const uint8_t * restrict cl,
                                struct AGX_INDEX_LIST_BUFFER_LO * restrict values)
{
   values->buffer_lo = __gen_unpack_uint(cl, 0, 31);
}

static inline void
AGX_INDEX_LIST_BUFFER_LO_print(FILE *fp, const struct AGX_INDEX_LIST_BUFFER_LO * values, unsigned indent)
{
   fprintf(fp, "%*sBuffer lo: 0x%" PRIx32 "\n", indent, "", values->buffer_lo);
}

struct AGX_INDEX_LIST_COUNT {
   uint32_t                             count;
};

#define AGX_INDEX_LIST_COUNT_header             \
   0

static inline void
AGX_INDEX_LIST_COUNT_pack(uint32_t * restrict cl,
                          const struct AGX_INDEX_LIST_COUNT * restrict values)
{
   cl[ 0] = util_bitpack_uint(values->count, 0, 31);
}


#define AGX_INDEX_LIST_COUNT_LENGTH 4
struct agx_index_list_count_packed { uint32_t opaque[1]; };
static inline void
AGX_INDEX_LIST_COUNT_unpack(FILE *fp, const uint8_t * restrict cl,
                            struct AGX_INDEX_LIST_COUNT * restrict values)
{
   values->count = __gen_unpack_uint(cl, 0, 31);
}

static inline void
AGX_INDEX_LIST_COUNT_print(FILE *fp, const struct AGX_INDEX_LIST_COUNT * values, unsigned indent)
{
   fprintf(fp, "%*sCount: %u\n", indent, "", values->count);
}

struct AGX_INDEX_LIST_INSTANCES {
   uint32_t                             count;
};

#define AGX_INDEX_LIST_INSTANCES_header         \
   0

static inline void
AGX_INDEX_LIST_INSTANCES_pack(uint32_t * restrict cl,
                              const struct AGX_INDEX_LIST_INSTANCES * restrict values)
{
   cl[ 0] = util_bitpack_uint(values->count, 0, 31);
}


#define AGX_INDEX_LIST_INSTANCES_LENGTH 4
struct agx_index_list_instances_packed { uint32_t opaque[1]; };
static inline void
AGX_INDEX_LIST_INSTANCES_unpack(FILE *fp, const uint8_t * restrict cl,
                                struct AGX_INDEX_LIST_INSTANCES * restrict values)
{
   values->count = __gen_unpack_uint(cl, 0, 31);
}

static inline void
AGX_INDEX_LIST_INSTANCES_print(FILE *fp, const struct AGX_INDEX_LIST_INSTANCES * values, unsigned indent)
{
   fprintf(fp, "%*sCount: %u\n", indent, "", values->count);
}

struct AGX_INDEX_LIST_START {
   uint32_t                             start;
};

#define AGX_INDEX_LIST_START_header             \
   0

static inline void
AGX_INDEX_LIST_START_pack(uint32_t * restrict cl,
                          const struct AGX_INDEX_LIST_START * restrict values)
{
   cl[ 0] = util_bitpack_uint(values->start, 0, 31);
}


#define AGX_INDEX_LIST_START_LENGTH 4
struct agx_index_list_start_packed { uint32_t opaque[1]; };
static inline void
AGX_INDEX_LIST_START_unpack(FILE *fp, const uint8_t * restrict cl,
                            struct AGX_INDEX_LIST_START * restrict values)
{
   values->start = __gen_unpack_uint(cl, 0, 31);
}

static inline void
AGX_INDEX_LIST_START_print(FILE *fp, const struct AGX_INDEX_LIST_START * values, unsigned indent)
{
   fprintf(fp, "%*sStart: %u\n", indent, "", values->start);
}

struct AGX_INDEX_LIST_BUFFER_SIZE {
   uint32_t                             size;
};

#define AGX_INDEX_LIST_BUFFER_SIZE_header       \
   0

static inline void
AGX_INDEX_LIST_BUFFER_SIZE_pack(uint32_t * restrict cl,
                                const struct AGX_INDEX_LIST_BUFFER_SIZE * restrict values)
{
   assert((values->size & 0x3) == 0);
   cl[ 0] = util_bitpack_uint(values->size >> 2, 0, 31);
}


#define AGX_INDEX_LIST_BUFFER_SIZE_LENGTH 4
struct agx_index_list_buffer_size_packed { uint32_t opaque[1]; };
static inline void
AGX_INDEX_LIST_BUFFER_SIZE_unpack(FILE *fp, const uint8_t * restrict cl,
                                  struct AGX_INDEX_LIST_BUFFER_SIZE * restrict values)
{
   values->size = __gen_unpack_uint(cl, 0, 31) << 2;
}

static inline void
AGX_INDEX_LIST_BUFFER_SIZE_print(FILE *fp, const struct AGX_INDEX_LIST_BUFFER_SIZE * values, unsigned indent)
{
   fprintf(fp, "%*sSize: %u\n", indent, "", values->size);
}

struct AGX_INDEX_LIST_INDIRECT_BUFFER {
   uint32_t                             address_hi;
   uint32_t                             address_lo;
};

#define AGX_INDEX_LIST_INDIRECT_BUFFER_header   \
   0

static inline void
AGX_INDEX_LIST_INDIRECT_BUFFER_pack(uint32_t * restrict cl,
                                    const struct AGX_INDEX_LIST_INDIRECT_BUFFER * restrict values)
{
   cl[ 0] = util_bitpack_uint(values->address_hi, 0, 7);
   cl[ 1] = util_bitpack_uint(values->address_lo, 0, 31);
}


#define AGX_INDEX_LIST_INDIRECT_BUFFER_LENGTH 8
struct agx_index_list_indirect_buffer_packed { uint32_t opaque[2]; };
static inline void
AGX_INDEX_LIST_INDIRECT_BUFFER_unpack(FILE *fp, const uint8_t * restrict cl,
                                      struct AGX_INDEX_LIST_INDIRECT_BUFFER * restrict values)
{
   if (((const uint32_t *) cl)[0] & 0xffffff00) fprintf(fp, "XXX: Unknown field of Index List: Indirect buffer unpacked at word 0: got %X, bad mask %X\n", ((const uint32_t *) cl)[0], ((const uint32_t *) cl)[0] & 0xffffff00);
   values->address_hi = __gen_unpack_uint(cl, 0, 7);
   values->address_lo = __gen_unpack_uint(cl, 32, 63);
}

static inline void
AGX_INDEX_LIST_INDIRECT_BUFFER_print(FILE *fp, const struct AGX_INDEX_LIST_INDIRECT_BUFFER * values, unsigned indent)
{
   fprintf(fp, "%*sAddress hi: 0x%" PRIx32 "\n", indent, "", values->address_hi);
   fprintf(fp, "%*sAddress lo: 0x%" PRIx32 "\n", indent, "", values->address_lo);
}

struct AGX_VDM_STREAM_LINK {
   uint32_t                             target_hi;
   bool                                 with_return;
   enum agx_vdm_block_type              block_type;
   uint32_t                             target_lo;
};

#define AGX_VDM_STREAM_LINK_header              \
   .block_type = AGX_VDM_BLOCK_TYPE_STREAM_LINK

static inline void
AGX_VDM_STREAM_LINK_pack(uint32_t * restrict cl,
                         const struct AGX_VDM_STREAM_LINK * restrict values)
{
   cl[ 0] = util_bitpack_uint(values->target_hi, 0, 7) |
            util_bitpack_uint(values->with_return, 28, 28) |
            util_bitpack_uint(values->block_type, 29, 31);
   cl[ 1] = util_bitpack_uint(values->target_lo, 0, 31);
}


#define AGX_VDM_STREAM_LINK_LENGTH 8
struct agx_vdm_stream_link_packed { uint32_t opaque[2]; };
static inline void
AGX_VDM_STREAM_LINK_unpack(FILE *fp, const uint8_t * restrict cl,
                           struct AGX_VDM_STREAM_LINK * restrict values)
{
   if (((const uint32_t *) cl)[0] & 0xfffff00) fprintf(fp, "XXX: Unknown field of VDM Stream Link unpacked at word 0: got %X, bad mask %X\n", ((const uint32_t *) cl)[0], ((const uint32_t *) cl)[0] & 0xfffff00);
   values->target_hi = __gen_unpack_uint(cl, 0, 7);
   values->with_return = __gen_unpack_uint(cl, 28, 28);
   values->block_type = (enum agx_vdm_block_type) __gen_unpack_uint(cl, 29, 31);
   values->target_lo = __gen_unpack_uint(cl, 32, 63);
}

static inline void
AGX_VDM_STREAM_LINK_print(FILE *fp, const struct AGX_VDM_STREAM_LINK * values, unsigned indent)
{
   fprintf(fp, "%*sTarget hi: 0x%" PRIx32 "\n", indent, "", values->target_hi);
   fprintf(fp, "%*sWith return: %s\n", indent, "", values->with_return ? "true" : "false");
   if (agx_vdm_block_type_as_str(values->block_type))
     fprintf(fp, "%*sBlock Type: %s\n", indent, "", agx_vdm_block_type_as_str(values->block_type));
    else
     fprintf(fp, "%*sBlock Type: unknown %X (XXX)\n", indent, "", values->block_type);
   fprintf(fp, "%*sTarget lo: 0x%" PRIx32 "\n", indent, "", values->target_lo);
}

struct AGX_VDM_STREAM_TERMINATE {
   enum agx_vdm_block_type              block_type;
};

#define AGX_VDM_STREAM_TERMINATE_header         \
   .block_type = AGX_VDM_BLOCK_TYPE_STREAM_TERMINATE

static inline void
AGX_VDM_STREAM_TERMINATE_pack(uint32_t * restrict cl,
                              const struct AGX_VDM_STREAM_TERMINATE * restrict values)
{
   cl[ 0] = util_bitpack_uint(values->block_type, 29, 31);
   cl[ 1] = 0;
}


#define AGX_VDM_STREAM_TERMINATE_LENGTH 8
struct agx_vdm_stream_terminate_packed { uint32_t opaque[2]; };
static inline void
AGX_VDM_STREAM_TERMINATE_unpack(FILE *fp, const uint8_t * restrict cl,
                                struct AGX_VDM_STREAM_TERMINATE * restrict values)
{
   if (((const uint32_t *) cl)[0] & 0x1fffffff) fprintf(fp, "XXX: Unknown field of VDM Stream Terminate unpacked at word 0: got %X, bad mask %X\n", ((const uint32_t *) cl)[0], ((const uint32_t *) cl)[0] & 0x1fffffff);
   if (((const uint32_t *) cl)[1] & 0xffffffff) fprintf(fp, "XXX: Unknown field of VDM Stream Terminate unpacked at word 1: got %X, bad mask %X\n", ((const uint32_t *) cl)[1], ((const uint32_t *) cl)[1] & 0xffffffff);
   values->block_type = (enum agx_vdm_block_type) __gen_unpack_uint(cl, 29, 31);
}

static inline void
AGX_VDM_STREAM_TERMINATE_print(FILE *fp, const struct AGX_VDM_STREAM_TERMINATE * values, unsigned indent)
{
   if (agx_vdm_block_type_as_str(values->block_type))
     fprintf(fp, "%*sBlock Type: %s\n", indent, "", agx_vdm_block_type_as_str(values->block_type));
    else
     fprintf(fp, "%*sBlock Type: unknown %X (XXX)\n", indent, "", values->block_type);
}

enum agx_cdm_block_type {
        AGX_CDM_BLOCK_TYPE_HEADER            =      0,
        AGX_CDM_BLOCK_TYPE_STREAM_LINK       =      1,
        AGX_CDM_BLOCK_TYPE_STREAM_TERMINATE  =      2,
        AGX_CDM_BLOCK_TYPE_LAUNCH            =      3,
};

static inline const char *
agx_cdm_block_type_as_str(enum agx_cdm_block_type imm)
{
    switch (imm) {
    case AGX_CDM_BLOCK_TYPE_HEADER: return "Header";
    case AGX_CDM_BLOCK_TYPE_STREAM_LINK: return "Stream Link";
    case AGX_CDM_BLOCK_TYPE_STREAM_TERMINATE: return "Stream Terminate";
    case AGX_CDM_BLOCK_TYPE_LAUNCH: return "Launch";
    default: break;
    }
    return NULL;
}

enum agx_cdm_mode {
        AGX_CDM_MODE_DIRECT                  =      0,
        AGX_CDM_MODE_INDIRECT_GLOBAL         =      1,
        AGX_CDM_MODE_INDIRECT_LOCAL          =      2,
};

static inline const char *
agx_cdm_mode_as_str(enum agx_cdm_mode imm)
{
    switch (imm) {
    case AGX_CDM_MODE_DIRECT: return "Direct";
    case AGX_CDM_MODE_INDIRECT_GLOBAL: return "Indirect global";
    case AGX_CDM_MODE_INDIRECT_LOCAL: return "Indirect local";
    default: break;
    }
    return NULL;
}

struct AGX_CDM_HEADER {
   uint32_t                             uniform_register_count;
   uint32_t                             texture_state_register_count;
   enum agx_sampler_states              sampler_state_register_count;
   uint32_t                             preshader_register_count;
   enum agx_cdm_mode                    mode;
   enum agx_cdm_block_type              block_type;
   uint64_t                             pipeline;
};

#define AGX_CDM_HEADER_header                   \
   .block_type = AGX_CDM_BLOCK_TYPE_HEADER

static inline void
AGX_CDM_HEADER_pack(uint32_t * restrict cl,
                    const struct AGX_CDM_HEADER * restrict values)
{
   assert((values->pipeline & 0x3f) == 0);
   cl[ 0] = util_bitpack_uint(__gen_to_groups(values->uniform_register_count, 64, 3), 1, 3) |
            util_bitpack_uint(__gen_to_groups(values->texture_state_register_count, 8, 5), 4, 8) |
            util_bitpack_uint(values->sampler_state_register_count, 9, 11) |
            util_bitpack_uint(__gen_to_groups(values->preshader_register_count, 16, 4), 12, 15) |
            util_bitpack_uint(values->mode, 27, 28) |
            util_bitpack_uint(values->block_type, 29, 31);
   cl[ 1] = util_bitpack_uint(values->pipeline >> 6, 6, 31);
}


#define AGX_CDM_HEADER_LENGTH 8
struct agx_cdm_header_packed { uint32_t opaque[2]; };
static inline void
AGX_CDM_HEADER_unpack(FILE *fp, const uint8_t * restrict cl,
                      struct AGX_CDM_HEADER * restrict values)
{
   if (((const uint32_t *) cl)[0] & 0x7ff0001) fprintf(fp, "XXX: Unknown field of CDM Header unpacked at word 0: got %X, bad mask %X\n", ((const uint32_t *) cl)[0], ((const uint32_t *) cl)[0] & 0x7ff0001);
   if (((const uint32_t *) cl)[1] & 0x3f) fprintf(fp, "XXX: Unknown field of CDM Header unpacked at word 1: got %X, bad mask %X\n", ((const uint32_t *) cl)[1], ((const uint32_t *) cl)[1] & 0x3f);
   values->uniform_register_count = __gen_from_groups(__gen_unpack_uint(cl, 1, 3), 64, 3);
   values->texture_state_register_count = __gen_from_groups(__gen_unpack_uint(cl, 4, 8), 8, 5);
   values->sampler_state_register_count = (enum agx_sampler_states) __gen_unpack_uint(cl, 9, 11);
   values->preshader_register_count = __gen_from_groups(__gen_unpack_uint(cl, 12, 15), 16, 4);
   values->mode = (enum agx_cdm_mode) __gen_unpack_uint(cl, 27, 28);
   values->block_type = (enum agx_cdm_block_type) __gen_unpack_uint(cl, 29, 31);
   values->pipeline = __gen_unpack_uint(cl, 38, 63) << 6;
}

static inline void
AGX_CDM_HEADER_print(FILE *fp, const struct AGX_CDM_HEADER * values, unsigned indent)
{
   fprintf(fp, "%*sUniform register count: %u\n", indent, "", values->uniform_register_count);
   fprintf(fp, "%*sTexture state register count: %u\n", indent, "", values->texture_state_register_count);
   if (agx_sampler_states_as_str(values->sampler_state_register_count))
     fprintf(fp, "%*sSampler state register count: %s\n", indent, "", agx_sampler_states_as_str(values->sampler_state_register_count));
    else
     fprintf(fp, "%*sSampler state register count: unknown %X (XXX)\n", indent, "", values->sampler_state_register_count);
   fprintf(fp, "%*sPreshader register count: %u\n", indent, "", values->preshader_register_count);
   if (agx_cdm_mode_as_str(values->mode))
     fprintf(fp, "%*sMode: %s\n", indent, "", agx_cdm_mode_as_str(values->mode));
    else
     fprintf(fp, "%*sMode: unknown %X (XXX)\n", indent, "", values->mode);
   if (agx_cdm_block_type_as_str(values->block_type))
     fprintf(fp, "%*sBlock Type: %s\n", indent, "", agx_cdm_block_type_as_str(values->block_type));
    else
     fprintf(fp, "%*sBlock Type: unknown %X (XXX)\n", indent, "", values->block_type);
   fprintf(fp, "%*sPipeline: 0x%" PRIx64 "\n", indent, "", values->pipeline);
}

struct AGX_CDM_UNK_G14X {
   bool                                 unknown_30;
};

#define AGX_CDM_UNK_G14X_header                 \
   .unknown_30 = true

static inline void
AGX_CDM_UNK_G14X_pack(uint32_t * restrict cl,
                      const struct AGX_CDM_UNK_G14X * restrict values)
{
   cl[ 0] = 0;
   cl[ 1] = util_bitpack_uint(values->unknown_30, 30, 30);
}


#define AGX_CDM_UNK_G14X_LENGTH 8
struct agx_cdm_unk_g14x_packed { uint32_t opaque[2]; };
static inline void
AGX_CDM_UNK_G14X_unpack(FILE *fp, const uint8_t * restrict cl,
                        struct AGX_CDM_UNK_G14X * restrict values)
{
   if (((const uint32_t *) cl)[0] & 0xffffffff) fprintf(fp, "XXX: Unknown field of CDM Unk G14X unpacked at word 0: got %X, bad mask %X\n", ((const uint32_t *) cl)[0], ((const uint32_t *) cl)[0] & 0xffffffff);
   if (((const uint32_t *) cl)[1] & 0xbfffffff) fprintf(fp, "XXX: Unknown field of CDM Unk G14X unpacked at word 1: got %X, bad mask %X\n", ((const uint32_t *) cl)[1], ((const uint32_t *) cl)[1] & 0xbfffffff);
   values->unknown_30 = __gen_unpack_uint(cl, 62, 62);
}

static inline void
AGX_CDM_UNK_G14X_print(FILE *fp, const struct AGX_CDM_UNK_G14X * values, unsigned indent)
{
   fprintf(fp, "%*sUnknown 30: %s\n", indent, "", values->unknown_30 ? "true" : "false");
}

struct AGX_CDM_INDIRECT {
   uint32_t                             address_hi;
   uint32_t                             address_lo;
};

#define AGX_CDM_INDIRECT_header                 \
   0

static inline void
AGX_CDM_INDIRECT_pack(uint32_t * restrict cl,
                      const struct AGX_CDM_INDIRECT * restrict values)
{
   assert((values->address_lo & 0x3) == 0);
   cl[ 0] = util_bitpack_uint(values->address_hi, 0, 7);
   cl[ 1] = util_bitpack_uint(values->address_lo >> 2, 2, 31);
}


#define AGX_CDM_INDIRECT_LENGTH 8
struct agx_cdm_indirect_packed { uint32_t opaque[2]; };
static inline void
AGX_CDM_INDIRECT_unpack(FILE *fp, const uint8_t * restrict cl,
                        struct AGX_CDM_INDIRECT * restrict values)
{
   if (((const uint32_t *) cl)[0] & 0xffffff00) fprintf(fp, "XXX: Unknown field of CDM Indirect unpacked at word 0: got %X, bad mask %X\n", ((const uint32_t *) cl)[0], ((const uint32_t *) cl)[0] & 0xffffff00);
   if (((const uint32_t *) cl)[1] & 0x3) fprintf(fp, "XXX: Unknown field of CDM Indirect unpacked at word 1: got %X, bad mask %X\n", ((const uint32_t *) cl)[1], ((const uint32_t *) cl)[1] & 0x3);
   values->address_hi = __gen_unpack_uint(cl, 0, 7);
   values->address_lo = __gen_unpack_uint(cl, 34, 63) << 2;
}

static inline void
AGX_CDM_INDIRECT_print(FILE *fp, const struct AGX_CDM_INDIRECT * values, unsigned indent)
{
   fprintf(fp, "%*sAddress hi: 0x%" PRIx32 "\n", indent, "", values->address_hi);
   fprintf(fp, "%*sAddress lo: 0x%" PRIx32 "\n", indent, "", values->address_lo);
}

struct AGX_CDM_GLOBAL_SIZE {
   uint32_t                             x;
   uint32_t                             y;
   uint32_t                             z;
};

#define AGX_CDM_GLOBAL_SIZE_header              \
   0

static inline void
AGX_CDM_GLOBAL_SIZE_pack(uint32_t * restrict cl,
                         const struct AGX_CDM_GLOBAL_SIZE * restrict values)
{
   cl[ 0] = util_bitpack_uint(values->x, 0, 31);
   cl[ 1] = util_bitpack_uint(values->y, 0, 31);
   cl[ 2] = util_bitpack_uint(values->z, 0, 31);
}


#define AGX_CDM_GLOBAL_SIZE_LENGTH 12
struct agx_cdm_global_size_packed { uint32_t opaque[3]; };
static inline void
AGX_CDM_GLOBAL_SIZE_unpack(FILE *fp, const uint8_t * restrict cl,
                           struct AGX_CDM_GLOBAL_SIZE * restrict values)
{
   values->x = __gen_unpack_uint(cl, 0, 31);
   values->y = __gen_unpack_uint(cl, 32, 63);
   values->z = __gen_unpack_uint(cl, 64, 95);
}

static inline void
AGX_CDM_GLOBAL_SIZE_print(FILE *fp, const struct AGX_CDM_GLOBAL_SIZE * values, unsigned indent)
{
   fprintf(fp, "%*sX: %u\n", indent, "", values->x);
   fprintf(fp, "%*sY: %u\n", indent, "", values->y);
   fprintf(fp, "%*sZ: %u\n", indent, "", values->z);
}

struct AGX_CDM_LOCAL_SIZE {
   uint32_t                             x;
   uint32_t                             y;
   uint32_t                             z;
};

#define AGX_CDM_LOCAL_SIZE_header               \
   0

static inline void
AGX_CDM_LOCAL_SIZE_pack(uint32_t * restrict cl,
                        const struct AGX_CDM_LOCAL_SIZE * restrict values)
{
   cl[ 0] = util_bitpack_uint(values->x, 0, 31);
   cl[ 1] = util_bitpack_uint(values->y, 0, 31);
   cl[ 2] = util_bitpack_uint(values->z, 0, 31);
}


#define AGX_CDM_LOCAL_SIZE_LENGTH 12
struct agx_cdm_local_size_packed { uint32_t opaque[3]; };
static inline void
AGX_CDM_LOCAL_SIZE_unpack(FILE *fp, const uint8_t * restrict cl,
                          struct AGX_CDM_LOCAL_SIZE * restrict values)
{
   values->x = __gen_unpack_uint(cl, 0, 31);
   values->y = __gen_unpack_uint(cl, 32, 63);
   values->z = __gen_unpack_uint(cl, 64, 95);
}

static inline void
AGX_CDM_LOCAL_SIZE_print(FILE *fp, const struct AGX_CDM_LOCAL_SIZE * values, unsigned indent)
{
   fprintf(fp, "%*sX: %u\n", indent, "", values->x);
   fprintf(fp, "%*sY: %u\n", indent, "", values->y);
   fprintf(fp, "%*sZ: %u\n", indent, "", values->z);
}

struct AGX_CDM_LAUNCH {
   uint32_t                             unknown;
   enum agx_cdm_block_type              block_type;
};

#define AGX_CDM_LAUNCH_header                   \
   .unknown = 0x160,  \
   .block_type = AGX_CDM_BLOCK_TYPE_LAUNCH

static inline void
AGX_CDM_LAUNCH_pack(uint32_t * restrict cl,
                    const struct AGX_CDM_LAUNCH * restrict values)
{
   cl[ 0] = util_bitpack_uint(values->unknown, 0, 11) |
            util_bitpack_uint(values->block_type, 29, 31);
}


#define AGX_CDM_LAUNCH_LENGTH 4
struct agx_cdm_launch_packed { uint32_t opaque[1]; };
static inline void
AGX_CDM_LAUNCH_unpack(FILE *fp, const uint8_t * restrict cl,
                      struct AGX_CDM_LAUNCH * restrict values)
{
   if (((const uint32_t *) cl)[0] & 0x1ffff000) fprintf(fp, "XXX: Unknown field of CDM Launch unpacked at word 0: got %X, bad mask %X\n", ((const uint32_t *) cl)[0], ((const uint32_t *) cl)[0] & 0x1ffff000);
   values->unknown = __gen_unpack_uint(cl, 0, 11);
   values->block_type = (enum agx_cdm_block_type) __gen_unpack_uint(cl, 29, 31);
}

static inline void
AGX_CDM_LAUNCH_print(FILE *fp, const struct AGX_CDM_LAUNCH * values, unsigned indent)
{
   fprintf(fp, "%*sUnknown: 0x%" PRIx32 "\n", indent, "", values->unknown);
   if (agx_cdm_block_type_as_str(values->block_type))
     fprintf(fp, "%*sBlock Type: %s\n", indent, "", agx_cdm_block_type_as_str(values->block_type));
    else
     fprintf(fp, "%*sBlock Type: unknown %X (XXX)\n", indent, "", values->block_type);
}

struct AGX_CDM_STREAM_LINK {
   uint32_t                             target_hi;
   enum agx_cdm_block_type              block_type;
   uint32_t                             target_lo;
};

#define AGX_CDM_STREAM_LINK_header              \
   .block_type = AGX_CDM_BLOCK_TYPE_STREAM_LINK

static inline void
AGX_CDM_STREAM_LINK_pack(uint32_t * restrict cl,
                         const struct AGX_CDM_STREAM_LINK * restrict values)
{
   cl[ 0] = util_bitpack_uint(values->target_hi, 0, 7) |
            util_bitpack_uint(values->block_type, 29, 31);
   cl[ 1] = util_bitpack_uint(values->target_lo, 0, 31);
}


#define AGX_CDM_STREAM_LINK_LENGTH 8
struct agx_cdm_stream_link_packed { uint32_t opaque[2]; };
static inline void
AGX_CDM_STREAM_LINK_unpack(FILE *fp, const uint8_t * restrict cl,
                           struct AGX_CDM_STREAM_LINK * restrict values)
{
   if (((const uint32_t *) cl)[0] & 0x1fffff00) fprintf(fp, "XXX: Unknown field of CDM Stream Link unpacked at word 0: got %X, bad mask %X\n", ((const uint32_t *) cl)[0], ((const uint32_t *) cl)[0] & 0x1fffff00);
   values->target_hi = __gen_unpack_uint(cl, 0, 7);
   values->block_type = (enum agx_cdm_block_type) __gen_unpack_uint(cl, 29, 31);
   values->target_lo = __gen_unpack_uint(cl, 32, 63);
}

static inline void
AGX_CDM_STREAM_LINK_print(FILE *fp, const struct AGX_CDM_STREAM_LINK * values, unsigned indent)
{
   fprintf(fp, "%*sTarget hi: 0x%" PRIx32 "\n", indent, "", values->target_hi);
   if (agx_cdm_block_type_as_str(values->block_type))
     fprintf(fp, "%*sBlock Type: %s\n", indent, "", agx_cdm_block_type_as_str(values->block_type));
    else
     fprintf(fp, "%*sBlock Type: unknown %X (XXX)\n", indent, "", values->block_type);
   fprintf(fp, "%*sTarget lo: 0x%" PRIx32 "\n", indent, "", values->target_lo);
}

struct AGX_CDM_STREAM_TERMINATE {
   enum agx_cdm_block_type              block_type;
};

#define AGX_CDM_STREAM_TERMINATE_header         \
   .block_type = AGX_CDM_BLOCK_TYPE_STREAM_TERMINATE

static inline void
AGX_CDM_STREAM_TERMINATE_pack(uint32_t * restrict cl,
                              const struct AGX_CDM_STREAM_TERMINATE * restrict values)
{
   cl[ 0] = util_bitpack_uint(values->block_type, 29, 31);
   cl[ 1] = 0;
}


#define AGX_CDM_STREAM_TERMINATE_LENGTH 8
struct agx_cdm_stream_terminate_packed { uint32_t opaque[2]; };
static inline void
AGX_CDM_STREAM_TERMINATE_unpack(FILE *fp, const uint8_t * restrict cl,
                                struct AGX_CDM_STREAM_TERMINATE * restrict values)
{
   if (((const uint32_t *) cl)[0] & 0x1fffffff) fprintf(fp, "XXX: Unknown field of CDM Stream Terminate unpacked at word 0: got %X, bad mask %X\n", ((const uint32_t *) cl)[0], ((const uint32_t *) cl)[0] & 0x1fffffff);
   if (((const uint32_t *) cl)[1] & 0xffffffff) fprintf(fp, "XXX: Unknown field of CDM Stream Terminate unpacked at word 1: got %X, bad mask %X\n", ((const uint32_t *) cl)[1], ((const uint32_t *) cl)[1] & 0xffffffff);
   values->block_type = (enum agx_cdm_block_type) __gen_unpack_uint(cl, 29, 31);
}

static inline void
AGX_CDM_STREAM_TERMINATE_print(FILE *fp, const struct AGX_CDM_STREAM_TERMINATE * values, unsigned indent)
{
   if (agx_cdm_block_type_as_str(values->block_type))
     fprintf(fp, "%*sBlock Type: %s\n", indent, "", agx_cdm_block_type_as_str(values->block_type));
    else
     fprintf(fp, "%*sBlock Type: unknown %X (XXX)\n", indent, "", values->block_type);
}

enum agx_iogpu_attachment_type {
        AGX_IOGPU_ATTACHMENT_TYPE_COLOUR     =     14,
        AGX_IOGPU_ATTACHMENT_TYPE_DEPTH      =     16,
        AGX_IOGPU_ATTACHMENT_TYPE_STENCIL    =     17,
        AGX_IOGPU_ATTACHMENT_TYPE_VISIBILITY =     18,
};

static inline const char *
agx_iogpu_attachment_type_as_str(enum agx_iogpu_attachment_type imm)
{
    switch (imm) {
    case AGX_IOGPU_ATTACHMENT_TYPE_COLOUR: return "Colour";
    case AGX_IOGPU_ATTACHMENT_TYPE_DEPTH: return "Depth";
    case AGX_IOGPU_ATTACHMENT_TYPE_STENCIL: return "Stencil";
    case AGX_IOGPU_ATTACHMENT_TYPE_VISIBILITY: return "Visibility";
    default: break;
    }
    return NULL;
}

struct AGX_IOGPU_ATTACHMENT {
   uint32_t                             unk_0;
   uint64_t                             address;
   enum agx_iogpu_attachment_type       type;
   uint32_t                             size;
   uint32_t                             unk_3;
   uint32_t                             percent;
};

#define AGX_IOGPU_ATTACHMENT_header             \
   .unk_0 = 0x100,  \
   .unk_3 = 0xC

static inline void
AGX_IOGPU_ATTACHMENT_pack(uint32_t * restrict cl,
                          const struct AGX_IOGPU_ATTACHMENT * restrict values)
{
   cl[ 0] = util_bitpack_uint(values->unk_0, 0, 15) |
            util_bitpack_uint(values->address, 16, 63);
   cl[ 1] = util_bitpack_uint(values->address, 16, 63) >> 32;
   cl[ 2] = util_bitpack_uint(values->type, 16, 31);
   cl[ 3] = util_bitpack_uint(values->size, 9, 40);
   cl[ 4] = util_bitpack_uint(values->size, 9, 40) >> 32 |
            util_bitpack_uint(values->unk_3, 16, 19);
   cl[ 5] = util_bitpack_uint(values->percent, 16, 31);
}


#define AGX_IOGPU_ATTACHMENT_LENGTH 24
struct agx_iogpu_attachment_packed { uint32_t opaque[6]; };
static inline void
AGX_IOGPU_ATTACHMENT_unpack(FILE *fp, const uint8_t * restrict cl,
                            struct AGX_IOGPU_ATTACHMENT * restrict values)
{
   if (((const uint32_t *) cl)[2] & 0xffff) fprintf(fp, "XXX: Unknown field of IOGPU Attachment unpacked at word 2: got %X, bad mask %X\n", ((const uint32_t *) cl)[2], ((const uint32_t *) cl)[2] & 0xffff);
   if (((const uint32_t *) cl)[3] & 0x1ff) fprintf(fp, "XXX: Unknown field of IOGPU Attachment unpacked at word 3: got %X, bad mask %X\n", ((const uint32_t *) cl)[3], ((const uint32_t *) cl)[3] & 0x1ff);
   if (((const uint32_t *) cl)[4] & 0xfff0fe00) fprintf(fp, "XXX: Unknown field of IOGPU Attachment unpacked at word 4: got %X, bad mask %X\n", ((const uint32_t *) cl)[4], ((const uint32_t *) cl)[4] & 0xfff0fe00);
   if (((const uint32_t *) cl)[5] & 0xffff) fprintf(fp, "XXX: Unknown field of IOGPU Attachment unpacked at word 5: got %X, bad mask %X\n", ((const uint32_t *) cl)[5], ((const uint32_t *) cl)[5] & 0xffff);
   values->unk_0 = __gen_unpack_uint(cl, 0, 15);
   values->address = __gen_unpack_uint(cl, 16, 63);
   values->type = (enum agx_iogpu_attachment_type) __gen_unpack_uint(cl, 80, 95);
   values->size = __gen_unpack_uint(cl, 105, 136);
   values->unk_3 = __gen_unpack_uint(cl, 144, 147);
   values->percent = __gen_unpack_uint(cl, 176, 191);
}

static inline void
AGX_IOGPU_ATTACHMENT_print(FILE *fp, const struct AGX_IOGPU_ATTACHMENT * values, unsigned indent)
{
   fprintf(fp, "%*sUnk 0: 0x%" PRIx32 "\n", indent, "", values->unk_0);
   fprintf(fp, "%*sAddress: 0x%" PRIx64 "\n", indent, "", values->address);
   if (agx_iogpu_attachment_type_as_str(values->type))
     fprintf(fp, "%*sType: %s\n", indent, "", agx_iogpu_attachment_type_as_str(values->type));
    else
     fprintf(fp, "%*sType: unknown %X (XXX)\n", indent, "", values->type);
   fprintf(fp, "%*sSize: %u\n", indent, "", values->size);
   fprintf(fp, "%*sUnk 3: 0x%" PRIx32 "\n", indent, "", values->unk_3);
   fprintf(fp, "%*sPercent: %u\n", indent, "", values->percent);
}

enum agx_zls_format {
        AGX_ZLS_FORMAT_32F                   =      0,
        AGX_ZLS_FORMAT_16                    =      2,
};

static inline const char *
agx_zls_format_as_str(enum agx_zls_format imm)
{
    switch (imm) {
    case AGX_ZLS_FORMAT_32F: return "32F";
    case AGX_ZLS_FORMAT_16: return "16";
    default: break;
    }
    return NULL;
}

struct AGX_ZLS_CONTROL {
   bool                                 unknown_0;
   bool                                 unknown_1;
   bool                                 z_compress_1;
   bool                                 unknown_3;
   bool                                 s_compress_1;
   bool                                 unknown_5;
   bool                                 z_compress_2;
   bool                                 unknown_7;
   bool                                 s_compress_2;
   bool                                 s_load_enable;
   bool                                 z_load_enable;
   bool                                 s_store_enable;
   bool                                 z_store_enable;
   enum agx_zls_format                  z_format;
   bool                                 z_resolve;
   bool                                 s_resolve;
};

#define AGX_ZLS_CONTROL_header                  \
   0

static inline void
AGX_ZLS_CONTROL_pack(uint32_t * restrict cl,
                     const struct AGX_ZLS_CONTROL * restrict values)
{
   cl[ 0] = util_bitpack_uint(values->unknown_0, 0, 0) |
            util_bitpack_uint(values->unknown_1, 1, 1) |
            util_bitpack_uint(values->z_compress_1, 2, 2) |
            util_bitpack_uint(values->unknown_3, 3, 3) |
            util_bitpack_uint(values->s_compress_1, 4, 4) |
            util_bitpack_uint(values->unknown_5, 5, 5) |
            util_bitpack_uint(values->z_compress_2, 6, 6) |
            util_bitpack_uint(values->unknown_7, 7, 7) |
            util_bitpack_uint(values->s_compress_2, 8, 8) |
            util_bitpack_uint(values->s_load_enable, 14, 14) |
            util_bitpack_uint(values->z_load_enable, 15, 15) |
            util_bitpack_uint(values->s_store_enable, 18, 18) |
            util_bitpack_uint(values->z_store_enable, 19, 19) |
            util_bitpack_uint(values->z_format, 25, 26);
   cl[ 1] = util_bitpack_uint(values->z_resolve, 24, 24) |
            util_bitpack_uint(values->s_resolve, 26, 26);
}


#define AGX_ZLS_CONTROL_LENGTH 8
struct agx_zls_control_packed { uint32_t opaque[2]; };
static inline void
AGX_ZLS_CONTROL_unpack(FILE *fp, const uint8_t * restrict cl,
                       struct AGX_ZLS_CONTROL * restrict values)
{
   if (((const uint32_t *) cl)[0] & 0xf9f33e00) fprintf(fp, "XXX: Unknown field of ZLS Control unpacked at word 0: got %X, bad mask %X\n", ((const uint32_t *) cl)[0], ((const uint32_t *) cl)[0] & 0xf9f33e00);
   if (((const uint32_t *) cl)[1] & 0xfaffffff) fprintf(fp, "XXX: Unknown field of ZLS Control unpacked at word 1: got %X, bad mask %X\n", ((const uint32_t *) cl)[1], ((const uint32_t *) cl)[1] & 0xfaffffff);
   values->unknown_0 = __gen_unpack_uint(cl, 0, 0);
   values->unknown_1 = __gen_unpack_uint(cl, 1, 1);
   values->z_compress_1 = __gen_unpack_uint(cl, 2, 2);
   values->unknown_3 = __gen_unpack_uint(cl, 3, 3);
   values->s_compress_1 = __gen_unpack_uint(cl, 4, 4);
   values->unknown_5 = __gen_unpack_uint(cl, 5, 5);
   values->z_compress_2 = __gen_unpack_uint(cl, 6, 6);
   values->unknown_7 = __gen_unpack_uint(cl, 7, 7);
   values->s_compress_2 = __gen_unpack_uint(cl, 8, 8);
   values->s_load_enable = __gen_unpack_uint(cl, 14, 14);
   values->z_load_enable = __gen_unpack_uint(cl, 15, 15);
   values->s_store_enable = __gen_unpack_uint(cl, 18, 18);
   values->z_store_enable = __gen_unpack_uint(cl, 19, 19);
   values->z_format = (enum agx_zls_format) __gen_unpack_uint(cl, 25, 26);
   values->z_resolve = __gen_unpack_uint(cl, 56, 56);
   values->s_resolve = __gen_unpack_uint(cl, 58, 58);
}

static inline void
AGX_ZLS_CONTROL_print(FILE *fp, const struct AGX_ZLS_CONTROL * values, unsigned indent)
{
   fprintf(fp, "%*sUnknown 0: %s\n", indent, "", values->unknown_0 ? "true" : "false");
   fprintf(fp, "%*sUnknown 1: %s\n", indent, "", values->unknown_1 ? "true" : "false");
   fprintf(fp, "%*sZ Compress 1: %s\n", indent, "", values->z_compress_1 ? "true" : "false");
   fprintf(fp, "%*sUnknown 3: %s\n", indent, "", values->unknown_3 ? "true" : "false");
   fprintf(fp, "%*sS Compress 1: %s\n", indent, "", values->s_compress_1 ? "true" : "false");
   fprintf(fp, "%*sUnknown 5: %s\n", indent, "", values->unknown_5 ? "true" : "false");
   fprintf(fp, "%*sZ Compress 2: %s\n", indent, "", values->z_compress_2 ? "true" : "false");
   fprintf(fp, "%*sUnknown 7: %s\n", indent, "", values->unknown_7 ? "true" : "false");
   fprintf(fp, "%*sS Compress 2: %s\n", indent, "", values->s_compress_2 ? "true" : "false");
   fprintf(fp, "%*sS Load Enable: %s\n", indent, "", values->s_load_enable ? "true" : "false");
   fprintf(fp, "%*sZ Load Enable: %s\n", indent, "", values->z_load_enable ? "true" : "false");
   fprintf(fp, "%*sS Store Enable: %s\n", indent, "", values->s_store_enable ? "true" : "false");
   fprintf(fp, "%*sZ Store Enable: %s\n", indent, "", values->z_store_enable ? "true" : "false");
   if (agx_zls_format_as_str(values->z_format))
     fprintf(fp, "%*sZ Format: %s\n", indent, "", agx_zls_format_as_str(values->z_format));
    else
     fprintf(fp, "%*sZ Format: unknown %X (XXX)\n", indent, "", values->z_format);
   fprintf(fp, "%*sZ Resolve: %s\n", indent, "", values->z_resolve ? "true" : "false");
   fprintf(fp, "%*sS Resolve: %s\n", indent, "", values->s_resolve ? "true" : "false");
}

struct AGX_IOGPU_HEADER {
   uint32_t                             unk_0;
   uint32_t                             total_size;
   uint32_t                             unk_2;
   uint32_t                             attachment_length;
   uint32_t                             attachment_offset;
   uint32_t                             unknown_offset;
   uint32_t                             unk_4;
   uint32_t                             unk_5;
   uint64_t                             encoder;
};

#define AGX_IOGPU_HEADER_header                 \
   .unk_0 = 0x10000,  \
   .unk_2 = 0x4,  \
   .unk_4 = 0x30,  \
   .unk_5 = 0x01

static inline void
AGX_IOGPU_HEADER_pack(uint32_t * restrict cl,
                      const struct AGX_IOGPU_HEADER * restrict values)
{
   cl[ 0] = util_bitpack_uint(values->unk_0, 0, 31);
   cl[ 1] = util_bitpack_uint(values->total_size, 0, 31);
   cl[ 2] = util_bitpack_uint(values->unk_2, 0, 31);
   cl[ 3] = 0;
   cl[ 4] = 0;
   cl[ 5] = 0;
   cl[ 6] = 0;
   cl[ 7] = 0;
   cl[ 8] = 0;
   cl[ 9] = util_bitpack_uint(values->attachment_length, 0, 31);
   cl[10] = util_bitpack_uint(values->attachment_offset, 0, 31);
   cl[11] = util_bitpack_uint(values->unknown_offset, 0, 31);
   cl[12] = util_bitpack_uint(values->unk_4, 0, 31);
   cl[13] = util_bitpack_uint(values->unk_5, 0, 31);
   cl[14] = util_bitpack_uint(values->encoder, 0, 63);
   cl[15] = util_bitpack_uint(values->encoder, 0, 63) >> 32;
}


#define AGX_IOGPU_HEADER_LENGTH 64
struct agx_iogpu_header_packed { uint32_t opaque[16]; };
static inline void
AGX_IOGPU_HEADER_unpack(FILE *fp, const uint8_t * restrict cl,
                        struct AGX_IOGPU_HEADER * restrict values)
{
   if (((const uint32_t *) cl)[3] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Header unpacked at word 3: got %X, bad mask %X\n", ((const uint32_t *) cl)[3], ((const uint32_t *) cl)[3] & 0xffffffff);
   if (((const uint32_t *) cl)[4] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Header unpacked at word 4: got %X, bad mask %X\n", ((const uint32_t *) cl)[4], ((const uint32_t *) cl)[4] & 0xffffffff);
   if (((const uint32_t *) cl)[5] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Header unpacked at word 5: got %X, bad mask %X\n", ((const uint32_t *) cl)[5], ((const uint32_t *) cl)[5] & 0xffffffff);
   if (((const uint32_t *) cl)[6] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Header unpacked at word 6: got %X, bad mask %X\n", ((const uint32_t *) cl)[6], ((const uint32_t *) cl)[6] & 0xffffffff);
   if (((const uint32_t *) cl)[7] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Header unpacked at word 7: got %X, bad mask %X\n", ((const uint32_t *) cl)[7], ((const uint32_t *) cl)[7] & 0xffffffff);
   if (((const uint32_t *) cl)[8] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Header unpacked at word 8: got %X, bad mask %X\n", ((const uint32_t *) cl)[8], ((const uint32_t *) cl)[8] & 0xffffffff);
   values->unk_0 = __gen_unpack_uint(cl, 0, 31);
   values->total_size = __gen_unpack_uint(cl, 32, 63);
   values->unk_2 = __gen_unpack_uint(cl, 64, 95);
   values->attachment_length = __gen_unpack_uint(cl, 288, 319);
   values->attachment_offset = __gen_unpack_uint(cl, 320, 351);
   values->unknown_offset = __gen_unpack_uint(cl, 352, 383);
   values->unk_4 = __gen_unpack_uint(cl, 384, 415);
   values->unk_5 = __gen_unpack_uint(cl, 416, 447);
   values->encoder = __gen_unpack_uint(cl, 448, 511);
}

static inline void
AGX_IOGPU_HEADER_print(FILE *fp, const struct AGX_IOGPU_HEADER * values, unsigned indent)
{
   fprintf(fp, "%*sUnk 0: 0x%" PRIx32 "\n", indent, "", values->unk_0);
   fprintf(fp, "%*sTotal size: %u\n", indent, "", values->total_size);
   fprintf(fp, "%*sUnk 2: 0x%" PRIx32 "\n", indent, "", values->unk_2);
   fprintf(fp, "%*sAttachment length: %u\n", indent, "", values->attachment_length);
   fprintf(fp, "%*sAttachment offset: %u\n", indent, "", values->attachment_offset);
   fprintf(fp, "%*sUnknown offset: %u\n", indent, "", values->unknown_offset);
   fprintf(fp, "%*sUnk 4: 0x%" PRIx32 "\n", indent, "", values->unk_4);
   fprintf(fp, "%*sUnk 5: 0x%" PRIx32 "\n", indent, "", values->unk_5);
   fprintf(fp, "%*sEncoder: 0x%" PRIx64 "\n", indent, "", values->encoder);
}

struct AGX_SPILL_BUFFER_HISTOGRAM {
   uint32_t                             bin_0;
   uint32_t                             bin_1;
   uint32_t                             bin_2;
   uint32_t                             bin_3;
   uint32_t                             bin_4;
   uint32_t                             bin_5;
   uint32_t                             bin_6;
   uint32_t                             bin_7;
   uint32_t                             bin_8;
   uint32_t                             bin_9;
   uint32_t                             bin_10;
   uint32_t                             bin_11;
   uint32_t                             bin_12;
   uint32_t                             bin_13;
   uint32_t                             bin_14;
};

#define AGX_SPILL_BUFFER_HISTOGRAM_header       \
   0

static inline void
AGX_SPILL_BUFFER_HISTOGRAM_pack(uint32_t * restrict cl,
                                const struct AGX_SPILL_BUFFER_HISTOGRAM * restrict values)
{
   cl[ 0] = util_bitpack_uint(values->bin_0, 0, 3);
   cl[ 1] = util_bitpack_uint(values->bin_1, 0, 3);
   cl[ 2] = util_bitpack_uint(values->bin_2, 0, 3);
   cl[ 3] = util_bitpack_uint(values->bin_3, 0, 3);
   cl[ 4] = util_bitpack_uint(values->bin_4, 0, 3);
   cl[ 5] = util_bitpack_uint(values->bin_5, 0, 3);
   cl[ 6] = util_bitpack_uint(values->bin_6, 0, 3);
   cl[ 7] = util_bitpack_uint(values->bin_7, 0, 3);
   cl[ 8] = util_bitpack_uint(values->bin_8, 0, 3);
   cl[ 9] = util_bitpack_uint(values->bin_9, 0, 3);
   cl[10] = util_bitpack_uint(values->bin_10, 0, 3);
   cl[11] = util_bitpack_uint(values->bin_11, 0, 3);
   cl[12] = util_bitpack_uint(values->bin_12, 0, 3);
   cl[13] = util_bitpack_uint(values->bin_13, 0, 3);
   cl[14] = util_bitpack_uint(values->bin_14, 0, 3);
}


#define AGX_SPILL_BUFFER_HISTOGRAM_LENGTH 60
struct agx_spill_buffer_histogram_packed { uint32_t opaque[15]; };
static inline void
AGX_SPILL_BUFFER_HISTOGRAM_unpack(FILE *fp, const uint8_t * restrict cl,
                                  struct AGX_SPILL_BUFFER_HISTOGRAM * restrict values)
{
   if (((const uint32_t *) cl)[0] & 0xfffffff0) fprintf(fp, "XXX: Unknown field of Spill Buffer Histogram unpacked at word 0: got %X, bad mask %X\n", ((const uint32_t *) cl)[0], ((const uint32_t *) cl)[0] & 0xfffffff0);
   if (((const uint32_t *) cl)[1] & 0xfffffff0) fprintf(fp, "XXX: Unknown field of Spill Buffer Histogram unpacked at word 1: got %X, bad mask %X\n", ((const uint32_t *) cl)[1], ((const uint32_t *) cl)[1] & 0xfffffff0);
   if (((const uint32_t *) cl)[2] & 0xfffffff0) fprintf(fp, "XXX: Unknown field of Spill Buffer Histogram unpacked at word 2: got %X, bad mask %X\n", ((const uint32_t *) cl)[2], ((const uint32_t *) cl)[2] & 0xfffffff0);
   if (((const uint32_t *) cl)[3] & 0xfffffff0) fprintf(fp, "XXX: Unknown field of Spill Buffer Histogram unpacked at word 3: got %X, bad mask %X\n", ((const uint32_t *) cl)[3], ((const uint32_t *) cl)[3] & 0xfffffff0);
   if (((const uint32_t *) cl)[4] & 0xfffffff0) fprintf(fp, "XXX: Unknown field of Spill Buffer Histogram unpacked at word 4: got %X, bad mask %X\n", ((const uint32_t *) cl)[4], ((const uint32_t *) cl)[4] & 0xfffffff0);
   if (((const uint32_t *) cl)[5] & 0xfffffff0) fprintf(fp, "XXX: Unknown field of Spill Buffer Histogram unpacked at word 5: got %X, bad mask %X\n", ((const uint32_t *) cl)[5], ((const uint32_t *) cl)[5] & 0xfffffff0);
   if (((const uint32_t *) cl)[6] & 0xfffffff0) fprintf(fp, "XXX: Unknown field of Spill Buffer Histogram unpacked at word 6: got %X, bad mask %X\n", ((const uint32_t *) cl)[6], ((const uint32_t *) cl)[6] & 0xfffffff0);
   if (((const uint32_t *) cl)[7] & 0xfffffff0) fprintf(fp, "XXX: Unknown field of Spill Buffer Histogram unpacked at word 7: got %X, bad mask %X\n", ((const uint32_t *) cl)[7], ((const uint32_t *) cl)[7] & 0xfffffff0);
   if (((const uint32_t *) cl)[8] & 0xfffffff0) fprintf(fp, "XXX: Unknown field of Spill Buffer Histogram unpacked at word 8: got %X, bad mask %X\n", ((const uint32_t *) cl)[8], ((const uint32_t *) cl)[8] & 0xfffffff0);
   if (((const uint32_t *) cl)[9] & 0xfffffff0) fprintf(fp, "XXX: Unknown field of Spill Buffer Histogram unpacked at word 9: got %X, bad mask %X\n", ((const uint32_t *) cl)[9], ((const uint32_t *) cl)[9] & 0xfffffff0);
   if (((const uint32_t *) cl)[10] & 0xfffffff0) fprintf(fp, "XXX: Unknown field of Spill Buffer Histogram unpacked at word 10: got %X, bad mask %X\n", ((const uint32_t *) cl)[10], ((const uint32_t *) cl)[10] & 0xfffffff0);
   if (((const uint32_t *) cl)[11] & 0xfffffff0) fprintf(fp, "XXX: Unknown field of Spill Buffer Histogram unpacked at word 11: got %X, bad mask %X\n", ((const uint32_t *) cl)[11], ((const uint32_t *) cl)[11] & 0xfffffff0);
   if (((const uint32_t *) cl)[12] & 0xfffffff0) fprintf(fp, "XXX: Unknown field of Spill Buffer Histogram unpacked at word 12: got %X, bad mask %X\n", ((const uint32_t *) cl)[12], ((const uint32_t *) cl)[12] & 0xfffffff0);
   if (((const uint32_t *) cl)[13] & 0xfffffff0) fprintf(fp, "XXX: Unknown field of Spill Buffer Histogram unpacked at word 13: got %X, bad mask %X\n", ((const uint32_t *) cl)[13], ((const uint32_t *) cl)[13] & 0xfffffff0);
   if (((const uint32_t *) cl)[14] & 0xfffffff0) fprintf(fp, "XXX: Unknown field of Spill Buffer Histogram unpacked at word 14: got %X, bad mask %X\n", ((const uint32_t *) cl)[14], ((const uint32_t *) cl)[14] & 0xfffffff0);
   values->bin_0 = __gen_unpack_uint(cl, 0, 3);
   values->bin_1 = __gen_unpack_uint(cl, 32, 35);
   values->bin_2 = __gen_unpack_uint(cl, 64, 67);
   values->bin_3 = __gen_unpack_uint(cl, 96, 99);
   values->bin_4 = __gen_unpack_uint(cl, 128, 131);
   values->bin_5 = __gen_unpack_uint(cl, 160, 163);
   values->bin_6 = __gen_unpack_uint(cl, 192, 195);
   values->bin_7 = __gen_unpack_uint(cl, 224, 227);
   values->bin_8 = __gen_unpack_uint(cl, 256, 259);
   values->bin_9 = __gen_unpack_uint(cl, 288, 291);
   values->bin_10 = __gen_unpack_uint(cl, 320, 323);
   values->bin_11 = __gen_unpack_uint(cl, 352, 355);
   values->bin_12 = __gen_unpack_uint(cl, 384, 387);
   values->bin_13 = __gen_unpack_uint(cl, 416, 419);
   values->bin_14 = __gen_unpack_uint(cl, 448, 451);
}

static inline void
AGX_SPILL_BUFFER_HISTOGRAM_print(FILE *fp, const struct AGX_SPILL_BUFFER_HISTOGRAM * values, unsigned indent)
{
   fprintf(fp, "%*sBin 0: %u\n", indent, "", values->bin_0);
   fprintf(fp, "%*sBin 1: %u\n", indent, "", values->bin_1);
   fprintf(fp, "%*sBin 2: %u\n", indent, "", values->bin_2);
   fprintf(fp, "%*sBin 3: %u\n", indent, "", values->bin_3);
   fprintf(fp, "%*sBin 4: %u\n", indent, "", values->bin_4);
   fprintf(fp, "%*sBin 5: %u\n", indent, "", values->bin_5);
   fprintf(fp, "%*sBin 6: %u\n", indent, "", values->bin_6);
   fprintf(fp, "%*sBin 7: %u\n", indent, "", values->bin_7);
   fprintf(fp, "%*sBin 8: %u\n", indent, "", values->bin_8);
   fprintf(fp, "%*sBin 9: %u\n", indent, "", values->bin_9);
   fprintf(fp, "%*sBin 10: %u\n", indent, "", values->bin_10);
   fprintf(fp, "%*sBin 11: %u\n", indent, "", values->bin_11);
   fprintf(fp, "%*sBin 12: %u\n", indent, "", values->bin_12);
   fprintf(fp, "%*sBin 13: %u\n", indent, "", values->bin_13);
   fprintf(fp, "%*sBin 14: %u\n", indent, "", values->bin_14);
}

struct AGX_IOGPU_COMPUTE {
   uint64_t                             deflake_1;
   uint64_t                             terminate_of_encoder;
   uint64_t                             deflake_2;
   uint64_t                             deflake_3;
   uint64_t                             deflake_4;
   uint64_t                             deflake_5;
   uint32_t                             unk_34;
   uint64_t                             unk_address;
   uint32_t                             unk_40;
   uint32_t                             encoder_id;
   uint32_t                             unk_44;
   uint64_t                             context_switch_program;
   uint32_t                             context_switch_block_size;
   bool                                 spilling_unk_1;
   uint32_t                             number_of_context_switch_buffers;
   struct AGX_SPILL_BUFFER_HISTOGRAM    spill_buffer_histogram;
   bool                                 enable_context_switching;
   uint32_t                             unk_94;
   uint32_t                             unk_95;
};

#define AGX_IOGPU_COMPUTE_header                \
   .unk_34 = 1,  \
   .unk_40 = 0x1c,  \
   .unk_44 = 0xffffffff,  \
   .context_switch_block_size = 2,  \
   .number_of_context_switch_buffers = 1,  \
   .spill_buffer_histogram = { AGX_SPILL_BUFFER_HISTOGRAM_header },  \
   .enable_context_switching = true,  \
   .unk_94 = 0xffffffff,  \
   .unk_95 = 0xffffffff

static inline void
AGX_IOGPU_COMPUTE_pack(uint32_t * restrict cl,
                       const struct AGX_IOGPU_COMPUTE * restrict values)
{
   cl[ 0] = 0;
   cl[ 1] = 0;
   cl[ 2] = 0;
   cl[ 3] = 0;
   cl[ 4] = 0;
   cl[ 5] = 0;
   cl[ 6] = 0;
   cl[ 7] = 0;
   cl[ 8] = 0;
   cl[ 9] = 0;
   cl[10] = 0;
   cl[11] = 0;
   cl[12] = 0;
   cl[13] = 0;
   cl[14] = 0;
   cl[15] = 0;
   cl[16] = 0;
   cl[17] = 0;
   cl[18] = 0;
   cl[19] = 0;
   cl[20] = util_bitpack_uint(values->deflake_1, 0, 63);
   cl[21] = util_bitpack_uint(values->deflake_1, 0, 63) >> 32;
   cl[22] = util_bitpack_uint(values->terminate_of_encoder, 0, 63);
   cl[23] = util_bitpack_uint(values->terminate_of_encoder, 0, 63) >> 32;
   cl[24] = 0;
   cl[25] = 0;
   cl[26] = util_bitpack_uint(values->deflake_2, 0, 63);
   cl[27] = util_bitpack_uint(values->deflake_2, 0, 63) >> 32;
   cl[28] = util_bitpack_uint(values->deflake_3, 0, 63);
   cl[29] = util_bitpack_uint(values->deflake_3, 0, 63) >> 32;
   cl[30] = util_bitpack_uint(values->deflake_4, 0, 63);
   cl[31] = util_bitpack_uint(values->deflake_4, 0, 63) >> 32;
   cl[32] = util_bitpack_uint(values->deflake_5, 0, 63);
   cl[33] = util_bitpack_uint(values->deflake_5, 0, 63) >> 32;
   cl[34] = util_bitpack_uint(values->unk_34, 0, 31);
   cl[35] = 0;
   cl[36] = util_bitpack_uint(values->unk_address, 0, 63);
   cl[37] = util_bitpack_uint(values->unk_address, 0, 63) >> 32;
   cl[38] = 0;
   cl[39] = 0;
   cl[40] = util_bitpack_uint(values->unk_40, 0, 31);
   cl[41] = util_bitpack_uint(values->encoder_id, 0, 31);
   cl[42] = 0;
   cl[43] = 0;
   cl[44] = util_bitpack_uint(values->unk_44, 0, 31);
   cl[45] = 0;
   cl[46] = 0;
   cl[47] = 0;
   cl[48] = util_bitpack_uint(values->context_switch_program, 0, 31);
   cl[49] = 0;
   cl[50] = 0;
   cl[51] = 0;
   cl[52] = 0;
   cl[53] = 0;
   cl[54] = 0;
   cl[55] = 0;
   cl[56] = util_bitpack_uint(values->context_switch_block_size, 0, 3);
   cl[57] = util_bitpack_uint(values->spilling_unk_1, 3, 3);
   cl[58] = util_bitpack_uint(values->number_of_context_switch_buffers, 0, 31);
   cl[59] = 0;
   cl[60] = util_bitpack_uint(values->spill_buffer_histogram.bin_0, 0, 3);
   cl[61] = util_bitpack_uint(values->spill_buffer_histogram.bin_1, 0, 3);
   cl[62] = util_bitpack_uint(values->spill_buffer_histogram.bin_2, 0, 3);
   cl[63] = util_bitpack_uint(values->spill_buffer_histogram.bin_3, 0, 3);
   cl[64] = util_bitpack_uint(values->spill_buffer_histogram.bin_4, 0, 3);
   cl[65] = util_bitpack_uint(values->spill_buffer_histogram.bin_5, 0, 3);
   cl[66] = util_bitpack_uint(values->spill_buffer_histogram.bin_6, 0, 3);
   cl[67] = util_bitpack_uint(values->spill_buffer_histogram.bin_7, 0, 3);
   cl[68] = util_bitpack_uint(values->spill_buffer_histogram.bin_8, 0, 3);
   cl[69] = util_bitpack_uint(values->spill_buffer_histogram.bin_9, 0, 3);
   cl[70] = util_bitpack_uint(values->spill_buffer_histogram.bin_10, 0, 3);
   cl[71] = util_bitpack_uint(values->spill_buffer_histogram.bin_11, 0, 3);
   cl[72] = util_bitpack_uint(values->spill_buffer_histogram.bin_12, 0, 3);
   cl[73] = util_bitpack_uint(values->spill_buffer_histogram.bin_13, 0, 3);
   cl[74] = util_bitpack_uint(values->spill_buffer_histogram.bin_14, 0, 3);
   cl[75] = util_bitpack_uint(values->enable_context_switching, 16, 16);
   cl[76] = 0;
   cl[77] = 0;
   cl[78] = 0;
   cl[79] = 0;
   cl[80] = 0;
   cl[81] = 0;
   cl[82] = 0;
   cl[83] = 0;
   cl[84] = 0;
   cl[85] = 0;
   cl[86] = 0;
   cl[87] = 0;
   cl[88] = 0;
   cl[89] = 0;
   cl[90] = 0;
   cl[91] = 0;
   cl[92] = 0;
   cl[93] = 0;
   cl[94] = util_bitpack_uint(values->unk_94, 0, 31);
   cl[95] = util_bitpack_uint(values->unk_95, 0, 31);
}


#define AGX_IOGPU_COMPUTE_LENGTH 384
struct agx_iogpu_compute_packed { uint32_t opaque[96]; };
static inline void
AGX_IOGPU_COMPUTE_unpack(FILE *fp, const uint8_t * restrict cl,
                         struct AGX_IOGPU_COMPUTE * restrict values)
{
   if (((const uint32_t *) cl)[0] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Compute unpacked at word 0: got %X, bad mask %X\n", ((const uint32_t *) cl)[0], ((const uint32_t *) cl)[0] & 0xffffffff);
   if (((const uint32_t *) cl)[1] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Compute unpacked at word 1: got %X, bad mask %X\n", ((const uint32_t *) cl)[1], ((const uint32_t *) cl)[1] & 0xffffffff);
   if (((const uint32_t *) cl)[2] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Compute unpacked at word 2: got %X, bad mask %X\n", ((const uint32_t *) cl)[2], ((const uint32_t *) cl)[2] & 0xffffffff);
   if (((const uint32_t *) cl)[3] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Compute unpacked at word 3: got %X, bad mask %X\n", ((const uint32_t *) cl)[3], ((const uint32_t *) cl)[3] & 0xffffffff);
   if (((const uint32_t *) cl)[4] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Compute unpacked at word 4: got %X, bad mask %X\n", ((const uint32_t *) cl)[4], ((const uint32_t *) cl)[4] & 0xffffffff);
   if (((const uint32_t *) cl)[5] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Compute unpacked at word 5: got %X, bad mask %X\n", ((const uint32_t *) cl)[5], ((const uint32_t *) cl)[5] & 0xffffffff);
   if (((const uint32_t *) cl)[6] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Compute unpacked at word 6: got %X, bad mask %X\n", ((const uint32_t *) cl)[6], ((const uint32_t *) cl)[6] & 0xffffffff);
   if (((const uint32_t *) cl)[7] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Compute unpacked at word 7: got %X, bad mask %X\n", ((const uint32_t *) cl)[7], ((const uint32_t *) cl)[7] & 0xffffffff);
   if (((const uint32_t *) cl)[8] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Compute unpacked at word 8: got %X, bad mask %X\n", ((const uint32_t *) cl)[8], ((const uint32_t *) cl)[8] & 0xffffffff);
   if (((const uint32_t *) cl)[9] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Compute unpacked at word 9: got %X, bad mask %X\n", ((const uint32_t *) cl)[9], ((const uint32_t *) cl)[9] & 0xffffffff);
   if (((const uint32_t *) cl)[10] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Compute unpacked at word 10: got %X, bad mask %X\n", ((const uint32_t *) cl)[10], ((const uint32_t *) cl)[10] & 0xffffffff);
   if (((const uint32_t *) cl)[11] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Compute unpacked at word 11: got %X, bad mask %X\n", ((const uint32_t *) cl)[11], ((const uint32_t *) cl)[11] & 0xffffffff);
   if (((const uint32_t *) cl)[12] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Compute unpacked at word 12: got %X, bad mask %X\n", ((const uint32_t *) cl)[12], ((const uint32_t *) cl)[12] & 0xffffffff);
   if (((const uint32_t *) cl)[13] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Compute unpacked at word 13: got %X, bad mask %X\n", ((const uint32_t *) cl)[13], ((const uint32_t *) cl)[13] & 0xffffffff);
   if (((const uint32_t *) cl)[14] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Compute unpacked at word 14: got %X, bad mask %X\n", ((const uint32_t *) cl)[14], ((const uint32_t *) cl)[14] & 0xffffffff);
   if (((const uint32_t *) cl)[15] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Compute unpacked at word 15: got %X, bad mask %X\n", ((const uint32_t *) cl)[15], ((const uint32_t *) cl)[15] & 0xffffffff);
   if (((const uint32_t *) cl)[16] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Compute unpacked at word 16: got %X, bad mask %X\n", ((const uint32_t *) cl)[16], ((const uint32_t *) cl)[16] & 0xffffffff);
   if (((const uint32_t *) cl)[17] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Compute unpacked at word 17: got %X, bad mask %X\n", ((const uint32_t *) cl)[17], ((const uint32_t *) cl)[17] & 0xffffffff);
   if (((const uint32_t *) cl)[18] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Compute unpacked at word 18: got %X, bad mask %X\n", ((const uint32_t *) cl)[18], ((const uint32_t *) cl)[18] & 0xffffffff);
   if (((const uint32_t *) cl)[19] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Compute unpacked at word 19: got %X, bad mask %X\n", ((const uint32_t *) cl)[19], ((const uint32_t *) cl)[19] & 0xffffffff);
   if (((const uint32_t *) cl)[24] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Compute unpacked at word 24: got %X, bad mask %X\n", ((const uint32_t *) cl)[24], ((const uint32_t *) cl)[24] & 0xffffffff);
   if (((const uint32_t *) cl)[25] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Compute unpacked at word 25: got %X, bad mask %X\n", ((const uint32_t *) cl)[25], ((const uint32_t *) cl)[25] & 0xffffffff);
   if (((const uint32_t *) cl)[35] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Compute unpacked at word 35: got %X, bad mask %X\n", ((const uint32_t *) cl)[35], ((const uint32_t *) cl)[35] & 0xffffffff);
   if (((const uint32_t *) cl)[38] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Compute unpacked at word 38: got %X, bad mask %X\n", ((const uint32_t *) cl)[38], ((const uint32_t *) cl)[38] & 0xffffffff);
   if (((const uint32_t *) cl)[39] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Compute unpacked at word 39: got %X, bad mask %X\n", ((const uint32_t *) cl)[39], ((const uint32_t *) cl)[39] & 0xffffffff);
   if (((const uint32_t *) cl)[42] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Compute unpacked at word 42: got %X, bad mask %X\n", ((const uint32_t *) cl)[42], ((const uint32_t *) cl)[42] & 0xffffffff);
   if (((const uint32_t *) cl)[43] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Compute unpacked at word 43: got %X, bad mask %X\n", ((const uint32_t *) cl)[43], ((const uint32_t *) cl)[43] & 0xffffffff);
   if (((const uint32_t *) cl)[45] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Compute unpacked at word 45: got %X, bad mask %X\n", ((const uint32_t *) cl)[45], ((const uint32_t *) cl)[45] & 0xffffffff);
   if (((const uint32_t *) cl)[46] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Compute unpacked at word 46: got %X, bad mask %X\n", ((const uint32_t *) cl)[46], ((const uint32_t *) cl)[46] & 0xffffffff);
   if (((const uint32_t *) cl)[47] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Compute unpacked at word 47: got %X, bad mask %X\n", ((const uint32_t *) cl)[47], ((const uint32_t *) cl)[47] & 0xffffffff);
   if (((const uint32_t *) cl)[49] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Compute unpacked at word 49: got %X, bad mask %X\n", ((const uint32_t *) cl)[49], ((const uint32_t *) cl)[49] & 0xffffffff);
   if (((const uint32_t *) cl)[50] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Compute unpacked at word 50: got %X, bad mask %X\n", ((const uint32_t *) cl)[50], ((const uint32_t *) cl)[50] & 0xffffffff);
   if (((const uint32_t *) cl)[51] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Compute unpacked at word 51: got %X, bad mask %X\n", ((const uint32_t *) cl)[51], ((const uint32_t *) cl)[51] & 0xffffffff);
   if (((const uint32_t *) cl)[52] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Compute unpacked at word 52: got %X, bad mask %X\n", ((const uint32_t *) cl)[52], ((const uint32_t *) cl)[52] & 0xffffffff);
   if (((const uint32_t *) cl)[53] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Compute unpacked at word 53: got %X, bad mask %X\n", ((const uint32_t *) cl)[53], ((const uint32_t *) cl)[53] & 0xffffffff);
   if (((const uint32_t *) cl)[54] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Compute unpacked at word 54: got %X, bad mask %X\n", ((const uint32_t *) cl)[54], ((const uint32_t *) cl)[54] & 0xffffffff);
   if (((const uint32_t *) cl)[55] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Compute unpacked at word 55: got %X, bad mask %X\n", ((const uint32_t *) cl)[55], ((const uint32_t *) cl)[55] & 0xffffffff);
   if (((const uint32_t *) cl)[56] & 0xfffffff0) fprintf(fp, "XXX: Unknown field of IOGPU Compute unpacked at word 56: got %X, bad mask %X\n", ((const uint32_t *) cl)[56], ((const uint32_t *) cl)[56] & 0xfffffff0);
   if (((const uint32_t *) cl)[57] & 0xfffffff7) fprintf(fp, "XXX: Unknown field of IOGPU Compute unpacked at word 57: got %X, bad mask %X\n", ((const uint32_t *) cl)[57], ((const uint32_t *) cl)[57] & 0xfffffff7);
   if (((const uint32_t *) cl)[59] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Compute unpacked at word 59: got %X, bad mask %X\n", ((const uint32_t *) cl)[59], ((const uint32_t *) cl)[59] & 0xffffffff);
   if (((const uint32_t *) cl)[60] & 0xfffffff0) fprintf(fp, "XXX: Unknown field of IOGPU Compute unpacked at word 60: got %X, bad mask %X\n", ((const uint32_t *) cl)[60], ((const uint32_t *) cl)[60] & 0xfffffff0);
   if (((const uint32_t *) cl)[61] & 0xfffffff0) fprintf(fp, "XXX: Unknown field of IOGPU Compute unpacked at word 61: got %X, bad mask %X\n", ((const uint32_t *) cl)[61], ((const uint32_t *) cl)[61] & 0xfffffff0);
   if (((const uint32_t *) cl)[62] & 0xfffffff0) fprintf(fp, "XXX: Unknown field of IOGPU Compute unpacked at word 62: got %X, bad mask %X\n", ((const uint32_t *) cl)[62], ((const uint32_t *) cl)[62] & 0xfffffff0);
   if (((const uint32_t *) cl)[63] & 0xfffffff0) fprintf(fp, "XXX: Unknown field of IOGPU Compute unpacked at word 63: got %X, bad mask %X\n", ((const uint32_t *) cl)[63], ((const uint32_t *) cl)[63] & 0xfffffff0);
   if (((const uint32_t *) cl)[64] & 0xfffffff0) fprintf(fp, "XXX: Unknown field of IOGPU Compute unpacked at word 64: got %X, bad mask %X\n", ((const uint32_t *) cl)[64], ((const uint32_t *) cl)[64] & 0xfffffff0);
   if (((const uint32_t *) cl)[65] & 0xfffffff0) fprintf(fp, "XXX: Unknown field of IOGPU Compute unpacked at word 65: got %X, bad mask %X\n", ((const uint32_t *) cl)[65], ((const uint32_t *) cl)[65] & 0xfffffff0);
   if (((const uint32_t *) cl)[66] & 0xfffffff0) fprintf(fp, "XXX: Unknown field of IOGPU Compute unpacked at word 66: got %X, bad mask %X\n", ((const uint32_t *) cl)[66], ((const uint32_t *) cl)[66] & 0xfffffff0);
   if (((const uint32_t *) cl)[67] & 0xfffffff0) fprintf(fp, "XXX: Unknown field of IOGPU Compute unpacked at word 67: got %X, bad mask %X\n", ((const uint32_t *) cl)[67], ((const uint32_t *) cl)[67] & 0xfffffff0);
   if (((const uint32_t *) cl)[68] & 0xfffffff0) fprintf(fp, "XXX: Unknown field of IOGPU Compute unpacked at word 68: got %X, bad mask %X\n", ((const uint32_t *) cl)[68], ((const uint32_t *) cl)[68] & 0xfffffff0);
   if (((const uint32_t *) cl)[69] & 0xfffffff0) fprintf(fp, "XXX: Unknown field of IOGPU Compute unpacked at word 69: got %X, bad mask %X\n", ((const uint32_t *) cl)[69], ((const uint32_t *) cl)[69] & 0xfffffff0);
   if (((const uint32_t *) cl)[70] & 0xfffffff0) fprintf(fp, "XXX: Unknown field of IOGPU Compute unpacked at word 70: got %X, bad mask %X\n", ((const uint32_t *) cl)[70], ((const uint32_t *) cl)[70] & 0xfffffff0);
   if (((const uint32_t *) cl)[71] & 0xfffffff0) fprintf(fp, "XXX: Unknown field of IOGPU Compute unpacked at word 71: got %X, bad mask %X\n", ((const uint32_t *) cl)[71], ((const uint32_t *) cl)[71] & 0xfffffff0);
   if (((const uint32_t *) cl)[72] & 0xfffffff0) fprintf(fp, "XXX: Unknown field of IOGPU Compute unpacked at word 72: got %X, bad mask %X\n", ((const uint32_t *) cl)[72], ((const uint32_t *) cl)[72] & 0xfffffff0);
   if (((const uint32_t *) cl)[73] & 0xfffffff0) fprintf(fp, "XXX: Unknown field of IOGPU Compute unpacked at word 73: got %X, bad mask %X\n", ((const uint32_t *) cl)[73], ((const uint32_t *) cl)[73] & 0xfffffff0);
   if (((const uint32_t *) cl)[74] & 0xfffffff0) fprintf(fp, "XXX: Unknown field of IOGPU Compute unpacked at word 74: got %X, bad mask %X\n", ((const uint32_t *) cl)[74], ((const uint32_t *) cl)[74] & 0xfffffff0);
   if (((const uint32_t *) cl)[75] & 0xfffeffff) fprintf(fp, "XXX: Unknown field of IOGPU Compute unpacked at word 75: got %X, bad mask %X\n", ((const uint32_t *) cl)[75], ((const uint32_t *) cl)[75] & 0xfffeffff);
   if (((const uint32_t *) cl)[76] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Compute unpacked at word 76: got %X, bad mask %X\n", ((const uint32_t *) cl)[76], ((const uint32_t *) cl)[76] & 0xffffffff);
   if (((const uint32_t *) cl)[77] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Compute unpacked at word 77: got %X, bad mask %X\n", ((const uint32_t *) cl)[77], ((const uint32_t *) cl)[77] & 0xffffffff);
   if (((const uint32_t *) cl)[78] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Compute unpacked at word 78: got %X, bad mask %X\n", ((const uint32_t *) cl)[78], ((const uint32_t *) cl)[78] & 0xffffffff);
   if (((const uint32_t *) cl)[79] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Compute unpacked at word 79: got %X, bad mask %X\n", ((const uint32_t *) cl)[79], ((const uint32_t *) cl)[79] & 0xffffffff);
   if (((const uint32_t *) cl)[80] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Compute unpacked at word 80: got %X, bad mask %X\n", ((const uint32_t *) cl)[80], ((const uint32_t *) cl)[80] & 0xffffffff);
   if (((const uint32_t *) cl)[81] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Compute unpacked at word 81: got %X, bad mask %X\n", ((const uint32_t *) cl)[81], ((const uint32_t *) cl)[81] & 0xffffffff);
   if (((const uint32_t *) cl)[82] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Compute unpacked at word 82: got %X, bad mask %X\n", ((const uint32_t *) cl)[82], ((const uint32_t *) cl)[82] & 0xffffffff);
   if (((const uint32_t *) cl)[83] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Compute unpacked at word 83: got %X, bad mask %X\n", ((const uint32_t *) cl)[83], ((const uint32_t *) cl)[83] & 0xffffffff);
   if (((const uint32_t *) cl)[84] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Compute unpacked at word 84: got %X, bad mask %X\n", ((const uint32_t *) cl)[84], ((const uint32_t *) cl)[84] & 0xffffffff);
   if (((const uint32_t *) cl)[85] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Compute unpacked at word 85: got %X, bad mask %X\n", ((const uint32_t *) cl)[85], ((const uint32_t *) cl)[85] & 0xffffffff);
   if (((const uint32_t *) cl)[86] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Compute unpacked at word 86: got %X, bad mask %X\n", ((const uint32_t *) cl)[86], ((const uint32_t *) cl)[86] & 0xffffffff);
   if (((const uint32_t *) cl)[87] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Compute unpacked at word 87: got %X, bad mask %X\n", ((const uint32_t *) cl)[87], ((const uint32_t *) cl)[87] & 0xffffffff);
   if (((const uint32_t *) cl)[88] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Compute unpacked at word 88: got %X, bad mask %X\n", ((const uint32_t *) cl)[88], ((const uint32_t *) cl)[88] & 0xffffffff);
   if (((const uint32_t *) cl)[89] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Compute unpacked at word 89: got %X, bad mask %X\n", ((const uint32_t *) cl)[89], ((const uint32_t *) cl)[89] & 0xffffffff);
   if (((const uint32_t *) cl)[90] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Compute unpacked at word 90: got %X, bad mask %X\n", ((const uint32_t *) cl)[90], ((const uint32_t *) cl)[90] & 0xffffffff);
   if (((const uint32_t *) cl)[91] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Compute unpacked at word 91: got %X, bad mask %X\n", ((const uint32_t *) cl)[91], ((const uint32_t *) cl)[91] & 0xffffffff);
   if (((const uint32_t *) cl)[92] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Compute unpacked at word 92: got %X, bad mask %X\n", ((const uint32_t *) cl)[92], ((const uint32_t *) cl)[92] & 0xffffffff);
   if (((const uint32_t *) cl)[93] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Compute unpacked at word 93: got %X, bad mask %X\n", ((const uint32_t *) cl)[93], ((const uint32_t *) cl)[93] & 0xffffffff);
   values->deflake_1 = __gen_unpack_uint(cl, 640, 703);
   values->terminate_of_encoder = __gen_unpack_uint(cl, 704, 767);
   values->deflake_2 = __gen_unpack_uint(cl, 832, 895);
   values->deflake_3 = __gen_unpack_uint(cl, 896, 959);
   values->deflake_4 = __gen_unpack_uint(cl, 960, 1023);
   values->deflake_5 = __gen_unpack_uint(cl, 1024, 1087);
   values->unk_34 = __gen_unpack_uint(cl, 1088, 1119);
   values->unk_address = __gen_unpack_uint(cl, 1152, 1215);
   values->unk_40 = __gen_unpack_uint(cl, 1280, 1311);
   values->encoder_id = __gen_unpack_uint(cl, 1312, 1343);
   values->unk_44 = __gen_unpack_uint(cl, 1408, 1439);
   values->context_switch_program = __gen_unpack_uint(cl, 1536, 1567);
   values->context_switch_block_size = __gen_unpack_uint(cl, 1792, 1795);
   values->spilling_unk_1 = __gen_unpack_uint(cl, 1827, 1827);
   values->number_of_context_switch_buffers = __gen_unpack_uint(cl, 1856, 1887);
   values->spill_buffer_histogram.bin_0 = __gen_unpack_uint(cl, 1920, 1923);
   values->spill_buffer_histogram.bin_1 = __gen_unpack_uint(cl, 1952, 1955);
   values->spill_buffer_histogram.bin_2 = __gen_unpack_uint(cl, 1984, 1987);
   values->spill_buffer_histogram.bin_3 = __gen_unpack_uint(cl, 2016, 2019);
   values->spill_buffer_histogram.bin_4 = __gen_unpack_uint(cl, 2048, 2051);
   values->spill_buffer_histogram.bin_5 = __gen_unpack_uint(cl, 2080, 2083);
   values->spill_buffer_histogram.bin_6 = __gen_unpack_uint(cl, 2112, 2115);
   values->spill_buffer_histogram.bin_7 = __gen_unpack_uint(cl, 2144, 2147);
   values->spill_buffer_histogram.bin_8 = __gen_unpack_uint(cl, 2176, 2179);
   values->spill_buffer_histogram.bin_9 = __gen_unpack_uint(cl, 2208, 2211);
   values->spill_buffer_histogram.bin_10 = __gen_unpack_uint(cl, 2240, 2243);
   values->spill_buffer_histogram.bin_11 = __gen_unpack_uint(cl, 2272, 2275);
   values->spill_buffer_histogram.bin_12 = __gen_unpack_uint(cl, 2304, 2307);
   values->spill_buffer_histogram.bin_13 = __gen_unpack_uint(cl, 2336, 2339);
   values->spill_buffer_histogram.bin_14 = __gen_unpack_uint(cl, 2368, 2371);
   values->enable_context_switching = __gen_unpack_uint(cl, 2416, 2416);
   values->unk_94 = __gen_unpack_uint(cl, 3008, 3039);
   values->unk_95 = __gen_unpack_uint(cl, 3040, 3071);
}

static inline void
AGX_IOGPU_COMPUTE_print(FILE *fp, const struct AGX_IOGPU_COMPUTE * values, unsigned indent)
{
   fprintf(fp, "%*sDeflake 1: 0x%" PRIx64 "\n", indent, "", values->deflake_1);
   fprintf(fp, "%*sTerminate of encoder: 0x%" PRIx64 "\n", indent, "", values->terminate_of_encoder);
   fprintf(fp, "%*sDeflake 2: 0x%" PRIx64 "\n", indent, "", values->deflake_2);
   fprintf(fp, "%*sDeflake 3: 0x%" PRIx64 "\n", indent, "", values->deflake_3);
   fprintf(fp, "%*sDeflake 4: 0x%" PRIx64 "\n", indent, "", values->deflake_4);
   fprintf(fp, "%*sDeflake 5: 0x%" PRIx64 "\n", indent, "", values->deflake_5);
   fprintf(fp, "%*sUnk 34: 0x%" PRIx32 "\n", indent, "", values->unk_34);
   fprintf(fp, "%*sUnk address: 0x%" PRIx64 "\n", indent, "", values->unk_address);
   fprintf(fp, "%*sUnk 40: 0x%" PRIx32 "\n", indent, "", values->unk_40);
   fprintf(fp, "%*sEncoder ID: 0x%" PRIx32 "\n", indent, "", values->encoder_id);
   fprintf(fp, "%*sUnk 44: 0x%" PRIx32 "\n", indent, "", values->unk_44);
   fprintf(fp, "%*sContext switch program: 0x%" PRIx64 "\n", indent, "", values->context_switch_program);
   fprintf(fp, "%*sContext switch block size: 0x%" PRIx32 "\n", indent, "", values->context_switch_block_size);
   fprintf(fp, "%*sSpilling unk 1: %s\n", indent, "", values->spilling_unk_1 ? "true" : "false");
   fprintf(fp, "%*sNumber of context switch buffers: %u\n", indent, "", values->number_of_context_switch_buffers);
   fprintf(fp, "%*sSpill Buffer Histogram:\n", indent, "");
   AGX_SPILL_BUFFER_HISTOGRAM_print(fp, &values->spill_buffer_histogram, indent + 2);
   fprintf(fp, "%*sEnable context switching: %s\n", indent, "", values->enable_context_switching ? "true" : "false");
   fprintf(fp, "%*sUnk 94: 0x%" PRIx32 "\n", indent, "", values->unk_94);
   fprintf(fp, "%*sUnk 95: 0x%" PRIx32 "\n", indent, "", values->unk_95);
}

struct AGX_IOGPU_GRAPHICS {
   uint64_t                             deflake_1;
   uint64_t                             deflake_2;
   uint32_t                             unk_54;
   uint32_t                             unk_55;
   uint32_t                             unk_56;
   uint64_t                             deflake_3;
   uint32_t                             unk_112;
   uint32_t                             unk_114;
   bool                                 memoryless_render_targets_used;
   bool                                 opengl_depth_clipping;
   uint32_t                             unk_118;
   uint32_t                             unk_119;
   uint32_t                             unk_120;
   uint32_t                             clear_pipeline_bind;
   uint32_t                             clear_pipeline_unk;
   uint64_t                             clear_pipeline;
   uint32_t                             store_pipeline_bind;
   uint32_t                             store_pipeline_unk;
   uint64_t                             store_pipeline;
   uint64_t                             scissor_array;
   uint64_t                             depth_bias_array;
   struct AGX_ZLS_CONTROL               zls_control;
   uint32_t                             depth_width;
   uint32_t                             depth_height;
   uint64_t                             depth_buffer_1;
   uint32_t                             depth_unknown_1;
   uint64_t                             depth_acceleration_buffer_1;
   uint64_t                             depth_buffer_2;
   uint64_t                             depth_acceleration_buffer_2;
   uint32_t                             depth_unknown_2;
   uint64_t                             stencil_buffer_1;
   uint32_t                             stencil_unknown_1;
   uint64_t                             stencil_acceleration_buffer_1;
   uint64_t                             stencil_buffer_2;
   uint32_t                             stencil_unknown_2;
   uint64_t                             stencil_acceleration_buffer_2;
   uint32_t                             unk_212;
   uint32_t                             unk_214;
   bool                                 z16_unorm_attachment_1;
   uint32_t                             width_1;
   uint32_t                             height_1;
   uint64_t                             pointer;
   uint32_t                             spilling_unk_1;
   struct AGX_SPILL_BUFFER_HISTOGRAM    spill_buffer_histogram;
   uint32_t                             depth_clear_value;
   uint32_t                             stencil_clear_value;
   uint32_t                             unk_277;
   bool                                 set_when_reloading_z_or_s_1;
   bool                                 set_when_frag_shader_spills;
   bool                                 set_when_reloading_z_or_s_2;
   bool                                 z16_unorm_attachment_2;
   uint32_t                             unk_282;
   uint32_t                             unk_283;
   uint32_t                             unk_284;
   uint64_t                             visibility_result_buffer;
   uint32_t                             partial_reload_pipeline_bind;
   uint32_t                             partial_reload_pipeline_unk;
   uint64_t                             partial_reload_pipeline;
   uint32_t                             partial_store_pipeline_bind;
   uint32_t                             partial_store_pipeline_unk;
   uint64_t                             partial_store_pipeline;
   uint64_t                             depth_buffer_3;
   uint64_t                             depth_acceleration_buffer_3;
   uint64_t                             stencil_buffer_3;
   uint64_t                             stencil_acceleration_buffer_3;
   uint32_t                             unk_352;
   uint32_t                             unk_360;
   uint32_t                             encoder_id;
   uint64_t                             unk_365;
   uint64_t                             unknown_buffer;
   uint32_t                             width_2;
   uint32_t                             height_2;
   uint32_t                             sample_count;
   uint32_t                             sample_0_x;
   uint32_t                             sample_0_y;
   uint32_t                             sample_1_x;
   uint32_t                             sample_1_y;
   uint32_t                             sample_2_x;
   uint32_t                             sample_2_y;
   uint32_t                             sample_3_x;
   uint32_t                             sample_3_y;
   uint32_t                             unk_490;
   uint32_t                             tile_width;
   uint32_t                             tile_height;
   uint32_t                             framebuffer_layers;
   uint32_t                             unk_560;
   uint32_t                             unk_700;
};

#define AGX_IOGPU_GRAPHICS_header               \
   .unk_54 = 0x6b0003,  \
   .unk_55 = 0x3a0012,  \
   .unk_56 = 0x1,  \
   .unk_112 = 0x1,  \
   .unk_114 = 0x1c,  \
   .unk_118 = 0xffffffff,  \
   .unk_119 = 0xffffffff,  \
   .unk_120 = 0xffffffff,  \
   .clear_pipeline_unk = 4,  \
   .store_pipeline_unk = 4,  \
   .zls_control = { AGX_ZLS_CONTROL_header },  \
   .depth_width = 1,  \
   .depth_height = 1,  \
   .unk_212 = 0x4,  \
   .unk_214 = 0xc000,  \
   .spill_buffer_histogram = { AGX_SPILL_BUFFER_HISTOGRAM_header },  \
   .unk_277 = 3,  \
   .unk_282 = 0xffffffff,  \
   .unk_283 = 0xffffffff,  \
   .unk_284 = 0xffffffff,  \
   .partial_reload_pipeline_unk = 4,  \
   .partial_store_pipeline_unk = 4,  \
   .unk_352 = 0x1,  \
   .unk_360 = 0x1c,  \
   .unk_365 = 0x1ffffffff,  \
   .sample_count = 1,  \
   .sample_0_x = 8,  \
   .sample_0_y = 8,  \
   .sample_1_x = 0,  \
   .sample_1_y = 0,  \
   .sample_2_x = 0,  \
   .sample_2_y = 0,  \
   .sample_3_x = 0,  \
   .sample_3_y = 0,  \
   .unk_490 = 8,  \
   .tile_width = 32,  \
   .tile_height = 32,  \
   .framebuffer_layers = 1,  \
   .unk_560 = 0,  \
   .unk_700 = 1

static inline void
AGX_IOGPU_GRAPHICS_pack(uint32_t * restrict cl,
                        const struct AGX_IOGPU_GRAPHICS * restrict values)
{
   assert((values->clear_pipeline & 0xf) == 0);
   assert((values->store_pipeline & 0xf) == 0);
   assert(values->depth_width >= 1);
   assert(values->depth_height >= 1);
   assert((values->depth_buffer_1 & 0x7f) == 0);
   assert((values->depth_acceleration_buffer_1 & 0x7f) == 0);
   assert((values->depth_buffer_2 & 0x7f) == 0);
   assert((values->depth_acceleration_buffer_2 & 0x7f) == 0);
   assert((values->stencil_buffer_1 & 0x7f) == 0);
   assert((values->stencil_acceleration_buffer_1 & 0x7f) == 0);
   assert((values->stencil_buffer_2 & 0x7f) == 0);
   assert((values->stencil_acceleration_buffer_2 & 0x7f) == 0);
   assert((values->partial_reload_pipeline & 0xf) == 0);
   assert((values->partial_store_pipeline & 0xf) == 0);
   cl[ 0] = 0;
   cl[ 1] = 0;
   cl[ 2] = 0;
   cl[ 3] = 0;
   cl[ 4] = 0;
   cl[ 5] = 0;
   cl[ 6] = 0;
   cl[ 7] = 0;
   cl[ 8] = 0;
   cl[ 9] = 0;
   cl[10] = 0;
   cl[11] = 0;
   cl[12] = 0;
   cl[13] = 0;
   cl[14] = 0;
   cl[15] = 0;
   cl[16] = 0;
   cl[17] = 0;
   cl[18] = 0;
   cl[19] = 0;
   cl[20] = 0;
   cl[21] = 0;
   cl[22] = util_bitpack_uint(values->deflake_1, 0, 63);
   cl[23] = util_bitpack_uint(values->deflake_1, 0, 63) >> 32;
   cl[24] = util_bitpack_uint(values->deflake_2, 0, 63);
   cl[25] = util_bitpack_uint(values->deflake_2, 0, 63) >> 32;
   cl[26] = 0;
   cl[27] = 0;
   cl[28] = 0;
   cl[29] = 0;
   cl[30] = 0;
   cl[31] = 0;
   cl[32] = 0;
   cl[33] = 0;
   cl[34] = 0;
   cl[35] = 0;
   cl[36] = 0;
   cl[37] = 0;
   cl[38] = util_bitpack_uint(values->unk_54, 0, 31);
   cl[39] = util_bitpack_uint(values->unk_55, 0, 31);
   cl[40] = util_bitpack_uint(values->unk_56, 0, 31);
   cl[41] = 0;
   cl[42] = 0;
   cl[43] = 0;
   cl[44] = 0;
   cl[45] = 0;
   cl[46] = 0;
   cl[47] = 0;
   cl[48] = 0;
   cl[49] = 0;
   cl[50] = 0;
   cl[51] = 0;
   cl[52] = 0;
   cl[53] = 0;
   cl[54] = 0;
   cl[55] = 0;
   cl[56] = 0;
   cl[57] = 0;
   cl[58] = 0;
   cl[59] = 0;
   cl[60] = 0;
   cl[61] = 0;
   cl[62] = 0;
   cl[63] = 0;
   cl[64] = 0;
   cl[65] = 0;
   cl[66] = 0;
   cl[67] = 0;
   cl[68] = 0;
   cl[69] = 0;
   cl[70] = 0;
   cl[71] = 0;
   cl[72] = 0;
   cl[73] = 0;
   cl[74] = 0;
   cl[75] = 0;
   cl[76] = 0;
   cl[77] = 0;
   cl[78] = 0;
   cl[79] = 0;
   cl[80] = 0;
   cl[81] = 0;
   cl[82] = 0;
   cl[83] = 0;
   cl[84] = 0;
   cl[85] = 0;
   cl[86] = 0;
   cl[87] = 0;
   cl[88] = 0;
   cl[89] = 0;
   cl[90] = 0;
   cl[91] = 0;
   cl[92] = 0;
   cl[93] = 0;
   cl[94] = util_bitpack_uint(values->deflake_3, 0, 63);
   cl[95] = util_bitpack_uint(values->deflake_3, 0, 63) >> 32;
   cl[96] = util_bitpack_uint(values->unk_112, 0, 31);
   cl[97] = 0;
   cl[98] = util_bitpack_uint(values->unk_114, 0, 31);
   cl[99] = 0;
   cl[100] = util_bitpack_uint(values->memoryless_render_targets_used, 0, 0) |
            util_bitpack_uint(values->opengl_depth_clipping, 24, 24);
   cl[101] = 0;
   cl[102] = util_bitpack_uint(values->unk_118, 0, 31);
   cl[103] = util_bitpack_uint(values->unk_119, 0, 31);
   cl[104] = util_bitpack_uint(values->unk_120, 0, 31);
   cl[105] = 0;
   cl[106] = 0;
   cl[107] = 0;
   cl[108] = 0;
   cl[109] = 0;
   cl[110] = 0;
   cl[111] = 0;
   cl[112] = 0;
   cl[113] = 0;
   cl[114] = 0;
   cl[115] = 0;
   cl[116] = 0;
   cl[117] = 0;
   cl[118] = 0;
   cl[119] = 0;
   cl[120] = 0;
   cl[121] = 0;
   cl[122] = 0;
   cl[123] = 0;
   cl[124] = 0;
   cl[125] = 0;
   cl[126] = 0;
   cl[127] = 0;
   cl[128] = 0;
   cl[129] = 0;
   cl[130] = 0;
   cl[131] = 0;
   cl[132] = 0;
   cl[133] = 0;
   cl[134] = 0;
   cl[135] = 0;
   cl[136] = 0;
   cl[137] = 0;
   cl[138] = 0;
   cl[139] = 0;
   cl[140] = 0;
   cl[141] = 0;
   cl[142] = 0;
   cl[143] = 0;
   cl[144] = 0;
   cl[145] = 0;
   cl[146] = util_bitpack_uint(values->clear_pipeline_bind, 0, 31);
   cl[147] = 0;
   cl[148] = util_bitpack_uint(values->clear_pipeline_unk, 0, 3) |
            util_bitpack_uint(values->clear_pipeline >> 4, 4, 31);
   cl[149] = 0;
   cl[150] = 0;
   cl[151] = 0;
   cl[152] = 0;
   cl[153] = 0;
   cl[154] = util_bitpack_uint(values->store_pipeline_bind, 0, 31);
   cl[155] = 0;
   cl[156] = util_bitpack_uint(values->store_pipeline_unk, 0, 3) |
            util_bitpack_uint(values->store_pipeline >> 4, 4, 31);
   cl[157] = 0;
   cl[158] = util_bitpack_uint(values->scissor_array, 0, 63);
   cl[159] = util_bitpack_uint(values->scissor_array, 0, 63) >> 32;
   cl[160] = util_bitpack_uint(values->depth_bias_array, 0, 63);
   cl[161] = util_bitpack_uint(values->depth_bias_array, 0, 63) >> 32;
   cl[162] = 0;
   cl[163] = 0;
   cl[164] = util_bitpack_uint(values->zls_control.unknown_0, 0, 0) |
            util_bitpack_uint(values->zls_control.unknown_1, 1, 1) |
            util_bitpack_uint(values->zls_control.z_compress_1, 2, 2) |
            util_bitpack_uint(values->zls_control.unknown_3, 3, 3) |
            util_bitpack_uint(values->zls_control.s_compress_1, 4, 4) |
            util_bitpack_uint(values->zls_control.unknown_5, 5, 5) |
            util_bitpack_uint(values->zls_control.z_compress_2, 6, 6) |
            util_bitpack_uint(values->zls_control.unknown_7, 7, 7) |
            util_bitpack_uint(values->zls_control.s_compress_2, 8, 8) |
            util_bitpack_uint(values->zls_control.s_load_enable, 14, 14) |
            util_bitpack_uint(values->zls_control.z_load_enable, 15, 15) |
            util_bitpack_uint(values->zls_control.s_store_enable, 18, 18) |
            util_bitpack_uint(values->zls_control.z_store_enable, 19, 19) |
            util_bitpack_uint(values->zls_control.z_format, 25, 26);
   cl[165] = util_bitpack_uint(values->zls_control.z_resolve, 24, 24) |
            util_bitpack_uint(values->zls_control.s_resolve, 26, 26);
   cl[166] = 0;
   cl[167] = 0;
   cl[168] = 0;
   cl[169] = 0;
   cl[170] = util_bitpack_uint(values->depth_width - 1, 0, 14) |
            util_bitpack_uint(values->depth_height - 1, 15, 29);
   cl[171] = 0;
   cl[172] = util_bitpack_uint(values->depth_buffer_1 >> 7, 7, 39);
   cl[173] = util_bitpack_uint(values->depth_buffer_1 >> 7, 7, 39) >> 32;
   cl[174] = 0;
   cl[175] = 0;
   cl[176] = util_bitpack_uint(values->depth_unknown_1, 0, 31);
   cl[177] = 0;
   cl[178] = util_bitpack_uint(values->depth_acceleration_buffer_1 >> 7, 7, 39);
   cl[179] = util_bitpack_uint(values->depth_acceleration_buffer_1 >> 7, 7, 39) >> 32;
   cl[180] = 0;
   cl[181] = 0;
   cl[182] = util_bitpack_uint(values->depth_buffer_2 >> 7, 7, 39);
   cl[183] = util_bitpack_uint(values->depth_buffer_2 >> 7, 7, 39) >> 32;
   cl[184] = 0;
   cl[185] = 0;
   cl[186] = util_bitpack_uint(values->depth_unknown_2, 0, 31);
   cl[187] = 0;
   cl[188] = util_bitpack_uint(values->depth_acceleration_buffer_2 >> 7, 7, 39);
   cl[189] = util_bitpack_uint(values->depth_acceleration_buffer_2 >> 7, 7, 39) >> 32;
   cl[190] = 0;
   cl[191] = 0;
   cl[192] = util_bitpack_uint(values->stencil_buffer_1 >> 7, 7, 39);
   cl[193] = util_bitpack_uint(values->stencil_buffer_1 >> 7, 7, 39) >> 32;
   cl[194] = 0;
   cl[195] = 0;
   cl[196] = util_bitpack_uint(values->stencil_unknown_1, 0, 31);
   cl[197] = 0;
   cl[198] = util_bitpack_uint(values->stencil_acceleration_buffer_1 >> 7, 7, 39);
   cl[199] = util_bitpack_uint(values->stencil_acceleration_buffer_1 >> 7, 7, 39) >> 32;
   cl[200] = 0;
   cl[201] = 0;
   cl[202] = util_bitpack_uint(values->stencil_buffer_2 >> 7, 7, 39);
   cl[203] = util_bitpack_uint(values->stencil_buffer_2 >> 7, 7, 39) >> 32;
   cl[204] = 0;
   cl[205] = 0;
   cl[206] = util_bitpack_uint(values->stencil_unknown_2, 0, 31);
   cl[207] = 0;
   cl[208] = util_bitpack_uint(values->stencil_acceleration_buffer_2 >> 7, 7, 39);
   cl[209] = util_bitpack_uint(values->stencil_acceleration_buffer_2 >> 7, 7, 39) >> 32;
   cl[210] = 0;
   cl[211] = 0;
   cl[212] = util_bitpack_uint(values->unk_212, 0, 31);
   cl[213] = 0;
   cl[214] = util_bitpack_uint(values->unk_214, 0, 15) |
            util_bitpack_uint(values->z16_unorm_attachment_1, 18, 18);
   cl[215] = 0;
   cl[216] = util_bitpack_uint(values->width_1, 0, 31);
   cl[217] = util_bitpack_uint(values->height_1, 0, 31);
   cl[218] = util_bitpack_uint(values->pointer, 0, 63);
   cl[219] = util_bitpack_uint(values->pointer, 0, 63) >> 32;
   cl[220] = 0;
   cl[221] = 0;
   cl[222] = 0;
   cl[223] = 0;
   cl[224] = 0;
   cl[225] = 0;
   cl[226] = util_bitpack_uint(values->spilling_unk_1, 0, 31);
   cl[227] = 0;
   cl[228] = 0;
   cl[229] = 0;
   cl[230] = 0;
   cl[231] = 0;
   cl[232] = 0;
   cl[233] = 0;
   cl[234] = 0;
   cl[235] = 0;
   cl[236] = 0;
   cl[237] = 0;
   cl[238] = util_bitpack_uint(values->spill_buffer_histogram.bin_0, 0, 3);
   cl[239] = util_bitpack_uint(values->spill_buffer_histogram.bin_1, 0, 3);
   cl[240] = util_bitpack_uint(values->spill_buffer_histogram.bin_2, 0, 3);
   cl[241] = util_bitpack_uint(values->spill_buffer_histogram.bin_3, 0, 3);
   cl[242] = util_bitpack_uint(values->spill_buffer_histogram.bin_4, 0, 3);
   cl[243] = util_bitpack_uint(values->spill_buffer_histogram.bin_5, 0, 3);
   cl[244] = util_bitpack_uint(values->spill_buffer_histogram.bin_6, 0, 3);
   cl[245] = util_bitpack_uint(values->spill_buffer_histogram.bin_7, 0, 3);
   cl[246] = util_bitpack_uint(values->spill_buffer_histogram.bin_8, 0, 3);
   cl[247] = util_bitpack_uint(values->spill_buffer_histogram.bin_9, 0, 3);
   cl[248] = util_bitpack_uint(values->spill_buffer_histogram.bin_10, 0, 3);
   cl[249] = util_bitpack_uint(values->spill_buffer_histogram.bin_11, 0, 3);
   cl[250] = util_bitpack_uint(values->spill_buffer_histogram.bin_12, 0, 3);
   cl[251] = util_bitpack_uint(values->spill_buffer_histogram.bin_13, 0, 3);
   cl[252] = util_bitpack_uint(values->spill_buffer_histogram.bin_14, 0, 3);
   cl[253] = 0;
   cl[254] = 0;
   cl[255] = 0;
   cl[256] = 0;
   cl[257] = 0;
   cl[258] = 0;
   cl[259] = 0;
   cl[260] = 0;
   cl[261] = 0;
   cl[262] = 0;
   cl[263] = 0;
   cl[264] = 0;
   cl[265] = 0;
   cl[266] = 0;
   cl[267] = 0;
   cl[268] = 0;
   cl[269] = 0;
   cl[270] = 0;
   cl[271] = 0;
   cl[272] = 0;
   cl[273] = 0;
   cl[274] = 0;
   cl[275] = 0;
   cl[276] = util_bitpack_uint(values->depth_clear_value, 0, 31);
   cl[277] = util_bitpack_uint(values->stencil_clear_value, 0, 7) |
            util_bitpack_uint(values->unk_277, 8, 15);
   cl[278] = 0;
   cl[279] = util_bitpack_uint(values->set_when_reloading_z_or_s_1, 8, 8) |
            util_bitpack_uint(values->set_when_frag_shader_spills, 24, 24);
   cl[280] = util_bitpack_uint(values->set_when_reloading_z_or_s_2, 24, 24);
   cl[281] = util_bitpack_uint(values->z16_unorm_attachment_2, 8, 8);
   cl[282] = util_bitpack_uint(values->unk_282, 0, 31);
   cl[283] = util_bitpack_uint(values->unk_283, 0, 31);
   cl[284] = util_bitpack_uint(values->unk_284, 0, 31);
   cl[285] = 0;
   cl[286] = util_bitpack_uint(values->visibility_result_buffer, 0, 63);
   cl[287] = util_bitpack_uint(values->visibility_result_buffer, 0, 63) >> 32;
   cl[288] = 0;
   cl[289] = 0;
   cl[290] = 0;
   cl[291] = 0;
   cl[292] = 0;
   cl[293] = 0;
   cl[294] = 0;
   cl[295] = 0;
   cl[296] = util_bitpack_uint(values->partial_reload_pipeline_bind, 0, 31);
   cl[297] = 0;
   cl[298] = util_bitpack_uint(values->partial_reload_pipeline_unk, 0, 3) |
            util_bitpack_uint(values->partial_reload_pipeline >> 4, 4, 31);
   cl[299] = 0;
   cl[300] = 0;
   cl[301] = 0;
   cl[302] = 0;
   cl[303] = 0;
   cl[304] = util_bitpack_uint(values->partial_store_pipeline_bind, 0, 31);
   cl[305] = 0;
   cl[306] = util_bitpack_uint(values->partial_store_pipeline_unk, 0, 3) |
            util_bitpack_uint(values->partial_store_pipeline >> 4, 4, 31);
   cl[307] = 0;
   cl[308] = 0;
   cl[309] = 0;
   cl[310] = 0;
   cl[311] = 0;
   cl[312] = 0;
   cl[313] = 0;
   cl[314] = 0;
   cl[315] = 0;
   cl[316] = 0;
   cl[317] = 0;
   cl[318] = 0;
   cl[319] = 0;
   cl[320] = 0;
   cl[321] = 0;
   cl[322] = 0;
   cl[323] = 0;
   cl[324] = 0;
   cl[325] = 0;
   cl[326] = 0;
   cl[327] = 0;
   cl[328] = 0;
   cl[329] = 0;
   cl[330] = 0;
   cl[331] = 0;
   cl[332] = 0;
   cl[333] = 0;
   cl[334] = 0;
   cl[335] = 0;
   cl[336] = 0;
   cl[337] = 0;
   cl[338] = 0;
   cl[339] = 0;
   cl[340] = util_bitpack_uint(values->depth_buffer_3, 0, 63);
   cl[341] = util_bitpack_uint(values->depth_buffer_3, 0, 63) >> 32;
   cl[342] = util_bitpack_uint(values->depth_acceleration_buffer_3, 0, 63);
   cl[343] = util_bitpack_uint(values->depth_acceleration_buffer_3, 0, 63) >> 32;
   cl[344] = util_bitpack_uint(values->stencil_buffer_3, 0, 63);
   cl[345] = util_bitpack_uint(values->stencil_buffer_3, 0, 63) >> 32;
   cl[346] = util_bitpack_uint(values->stencil_acceleration_buffer_3, 0, 63);
   cl[347] = util_bitpack_uint(values->stencil_acceleration_buffer_3, 0, 63) >> 32;
   cl[348] = 0;
   cl[349] = 0;
   cl[350] = 0;
   cl[351] = 0;
   cl[352] = util_bitpack_uint(values->unk_352, 0, 31);
   cl[353] = 0;
   cl[354] = 0;
   cl[355] = 0;
   cl[356] = 0;
   cl[357] = 0;
   cl[358] = 0;
   cl[359] = 0;
   cl[360] = util_bitpack_uint(values->unk_360, 0, 31);
   cl[361] = 0;
   cl[362] = util_bitpack_uint(values->encoder_id, 0, 31);
   cl[363] = 0;
   cl[364] = 0;
   cl[365] = util_bitpack_uint(values->unk_365, 0, 63);
   cl[366] = util_bitpack_uint(values->unk_365, 0, 63) >> 32;
   cl[367] = 0;
   cl[368] = 0;
   cl[369] = 0;
   cl[370] = util_bitpack_uint(values->unknown_buffer, 0, 63);
   cl[371] = util_bitpack_uint(values->unknown_buffer, 0, 63) >> 32;
   cl[372] = 0;
   cl[373] = 0;
   cl[374] = 0;
   cl[375] = 0;
   cl[376] = 0;
   cl[377] = 0;
   cl[378] = 0;
   cl[379] = 0;
   cl[380] = 0;
   cl[381] = 0;
   cl[382] = util_bitpack_uint(values->width_2, 0, 31);
   cl[383] = util_bitpack_uint(values->height_2, 0, 31);
   cl[384] = util_bitpack_uint(values->sample_count, 0, 31);
   cl[385] = util_bitpack_uint(values->sample_0_x, 0, 4);
   cl[386] = util_bitpack_uint(values->sample_0_y, 0, 4);
   cl[387] = util_bitpack_uint(values->sample_1_x, 0, 4);
   cl[388] = util_bitpack_uint(values->sample_1_y, 0, 4);
   cl[389] = util_bitpack_uint(values->sample_2_x, 0, 4);
   cl[390] = util_bitpack_uint(values->sample_2_y, 0, 4);
   cl[391] = util_bitpack_uint(values->sample_3_x, 0, 4);
   cl[392] = util_bitpack_uint(values->sample_3_y, 0, 4);
   cl[393] = 0;
   cl[394] = 0;
   cl[395] = 0;
   cl[396] = 0;
   cl[397] = 0;
   cl[398] = 0;
   cl[399] = 0;
   cl[400] = 0;
   cl[401] = util_bitpack_uint(values->unk_490, 0, 31);
   cl[402] = util_bitpack_uint(values->tile_width, 0, 31);
   cl[403] = util_bitpack_uint(values->tile_height, 0, 31);
   cl[404] = util_bitpack_uint(values->framebuffer_layers, 0, 31);
   cl[405] = 0;
   cl[406] = 0;
   cl[407] = 0;
   cl[408] = util_bitpack_uint(values->unk_560, 0, 31);
   cl[409] = 0;
   cl[410] = util_bitpack_uint(values->unk_700, 0, 31);
   cl[411] = 0;
   cl[412] = 0;
   cl[413] = 0;
   cl[414] = 0;
   cl[415] = 0;
   cl[416] = 0;
   cl[417] = 0;
   cl[418] = 0;
   cl[419] = 0;
   cl[420] = 0;
   cl[421] = 0;
   cl[422] = 0;
   cl[423] = 0;
   cl[424] = 0;
   cl[425] = 0;
   cl[426] = 0;
   cl[427] = 0;
   cl[428] = 0;
   cl[429] = 0;
   cl[430] = 0;
   cl[431] = 0;
   cl[432] = 0;
   cl[433] = 0;
   cl[434] = 0;
   cl[435] = 0;
   cl[436] = 0;
   cl[437] = 0;
   cl[438] = 0;
   cl[439] = 0;
   cl[440] = 0;
   cl[441] = 0;
   cl[442] = 0;
   cl[443] = 0;
   cl[444] = 0;
   cl[445] = 0;
   cl[446] = 0;
   cl[447] = 0;
   cl[448] = 0;
   cl[449] = 0;
   cl[450] = 0;
   cl[451] = 0;
   cl[452] = 0;
   cl[453] = 0;
   cl[454] = 0;
   cl[455] = 0;
   cl[456] = 0;
   cl[457] = 0;
   cl[458] = 0;
   cl[459] = 0;
   cl[460] = 0;
   cl[461] = 0;
   cl[462] = 0;
   cl[463] = 0;
   cl[464] = 0;
   cl[465] = 0;
   cl[466] = 0;
   cl[467] = 0;
   cl[468] = 0;
   cl[469] = 0;
   cl[470] = 0;
   cl[471] = 0;
   cl[472] = 0;
   cl[473] = 0;
   cl[474] = 0;
   cl[475] = 0;
   cl[476] = 0;
   cl[477] = 0;
   cl[478] = 0;
   cl[479] = 0;
}


#define AGX_IOGPU_GRAPHICS_LENGTH 1920
struct agx_iogpu_graphics_packed { uint32_t opaque[480]; };
static inline void
AGX_IOGPU_GRAPHICS_unpack(FILE *fp, const uint8_t * restrict cl,
                          struct AGX_IOGPU_GRAPHICS * restrict values)
{
   if (((const uint32_t *) cl)[0] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 0: got %X, bad mask %X\n", ((const uint32_t *) cl)[0], ((const uint32_t *) cl)[0] & 0xffffffff);
   if (((const uint32_t *) cl)[1] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 1: got %X, bad mask %X\n", ((const uint32_t *) cl)[1], ((const uint32_t *) cl)[1] & 0xffffffff);
   if (((const uint32_t *) cl)[2] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 2: got %X, bad mask %X\n", ((const uint32_t *) cl)[2], ((const uint32_t *) cl)[2] & 0xffffffff);
   if (((const uint32_t *) cl)[3] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 3: got %X, bad mask %X\n", ((const uint32_t *) cl)[3], ((const uint32_t *) cl)[3] & 0xffffffff);
   if (((const uint32_t *) cl)[4] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 4: got %X, bad mask %X\n", ((const uint32_t *) cl)[4], ((const uint32_t *) cl)[4] & 0xffffffff);
   if (((const uint32_t *) cl)[5] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 5: got %X, bad mask %X\n", ((const uint32_t *) cl)[5], ((const uint32_t *) cl)[5] & 0xffffffff);
   if (((const uint32_t *) cl)[6] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 6: got %X, bad mask %X\n", ((const uint32_t *) cl)[6], ((const uint32_t *) cl)[6] & 0xffffffff);
   if (((const uint32_t *) cl)[7] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 7: got %X, bad mask %X\n", ((const uint32_t *) cl)[7], ((const uint32_t *) cl)[7] & 0xffffffff);
   if (((const uint32_t *) cl)[8] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 8: got %X, bad mask %X\n", ((const uint32_t *) cl)[8], ((const uint32_t *) cl)[8] & 0xffffffff);
   if (((const uint32_t *) cl)[9] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 9: got %X, bad mask %X\n", ((const uint32_t *) cl)[9], ((const uint32_t *) cl)[9] & 0xffffffff);
   if (((const uint32_t *) cl)[10] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 10: got %X, bad mask %X\n", ((const uint32_t *) cl)[10], ((const uint32_t *) cl)[10] & 0xffffffff);
   if (((const uint32_t *) cl)[11] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 11: got %X, bad mask %X\n", ((const uint32_t *) cl)[11], ((const uint32_t *) cl)[11] & 0xffffffff);
   if (((const uint32_t *) cl)[12] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 12: got %X, bad mask %X\n", ((const uint32_t *) cl)[12], ((const uint32_t *) cl)[12] & 0xffffffff);
   if (((const uint32_t *) cl)[13] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 13: got %X, bad mask %X\n", ((const uint32_t *) cl)[13], ((const uint32_t *) cl)[13] & 0xffffffff);
   if (((const uint32_t *) cl)[14] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 14: got %X, bad mask %X\n", ((const uint32_t *) cl)[14], ((const uint32_t *) cl)[14] & 0xffffffff);
   if (((const uint32_t *) cl)[15] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 15: got %X, bad mask %X\n", ((const uint32_t *) cl)[15], ((const uint32_t *) cl)[15] & 0xffffffff);
   if (((const uint32_t *) cl)[16] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 16: got %X, bad mask %X\n", ((const uint32_t *) cl)[16], ((const uint32_t *) cl)[16] & 0xffffffff);
   if (((const uint32_t *) cl)[17] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 17: got %X, bad mask %X\n", ((const uint32_t *) cl)[17], ((const uint32_t *) cl)[17] & 0xffffffff);
   if (((const uint32_t *) cl)[18] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 18: got %X, bad mask %X\n", ((const uint32_t *) cl)[18], ((const uint32_t *) cl)[18] & 0xffffffff);
   if (((const uint32_t *) cl)[19] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 19: got %X, bad mask %X\n", ((const uint32_t *) cl)[19], ((const uint32_t *) cl)[19] & 0xffffffff);
   if (((const uint32_t *) cl)[20] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 20: got %X, bad mask %X\n", ((const uint32_t *) cl)[20], ((const uint32_t *) cl)[20] & 0xffffffff);
   if (((const uint32_t *) cl)[21] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 21: got %X, bad mask %X\n", ((const uint32_t *) cl)[21], ((const uint32_t *) cl)[21] & 0xffffffff);
   if (((const uint32_t *) cl)[26] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 26: got %X, bad mask %X\n", ((const uint32_t *) cl)[26], ((const uint32_t *) cl)[26] & 0xffffffff);
   if (((const uint32_t *) cl)[27] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 27: got %X, bad mask %X\n", ((const uint32_t *) cl)[27], ((const uint32_t *) cl)[27] & 0xffffffff);
   if (((const uint32_t *) cl)[28] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 28: got %X, bad mask %X\n", ((const uint32_t *) cl)[28], ((const uint32_t *) cl)[28] & 0xffffffff);
   if (((const uint32_t *) cl)[29] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 29: got %X, bad mask %X\n", ((const uint32_t *) cl)[29], ((const uint32_t *) cl)[29] & 0xffffffff);
   if (((const uint32_t *) cl)[30] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 30: got %X, bad mask %X\n", ((const uint32_t *) cl)[30], ((const uint32_t *) cl)[30] & 0xffffffff);
   if (((const uint32_t *) cl)[31] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 31: got %X, bad mask %X\n", ((const uint32_t *) cl)[31], ((const uint32_t *) cl)[31] & 0xffffffff);
   if (((const uint32_t *) cl)[32] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 32: got %X, bad mask %X\n", ((const uint32_t *) cl)[32], ((const uint32_t *) cl)[32] & 0xffffffff);
   if (((const uint32_t *) cl)[33] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 33: got %X, bad mask %X\n", ((const uint32_t *) cl)[33], ((const uint32_t *) cl)[33] & 0xffffffff);
   if (((const uint32_t *) cl)[34] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 34: got %X, bad mask %X\n", ((const uint32_t *) cl)[34], ((const uint32_t *) cl)[34] & 0xffffffff);
   if (((const uint32_t *) cl)[35] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 35: got %X, bad mask %X\n", ((const uint32_t *) cl)[35], ((const uint32_t *) cl)[35] & 0xffffffff);
   if (((const uint32_t *) cl)[36] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 36: got %X, bad mask %X\n", ((const uint32_t *) cl)[36], ((const uint32_t *) cl)[36] & 0xffffffff);
   if (((const uint32_t *) cl)[37] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 37: got %X, bad mask %X\n", ((const uint32_t *) cl)[37], ((const uint32_t *) cl)[37] & 0xffffffff);
   if (((const uint32_t *) cl)[41] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 41: got %X, bad mask %X\n", ((const uint32_t *) cl)[41], ((const uint32_t *) cl)[41] & 0xffffffff);
   if (((const uint32_t *) cl)[42] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 42: got %X, bad mask %X\n", ((const uint32_t *) cl)[42], ((const uint32_t *) cl)[42] & 0xffffffff);
   if (((const uint32_t *) cl)[43] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 43: got %X, bad mask %X\n", ((const uint32_t *) cl)[43], ((const uint32_t *) cl)[43] & 0xffffffff);
   if (((const uint32_t *) cl)[44] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 44: got %X, bad mask %X\n", ((const uint32_t *) cl)[44], ((const uint32_t *) cl)[44] & 0xffffffff);
   if (((const uint32_t *) cl)[45] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 45: got %X, bad mask %X\n", ((const uint32_t *) cl)[45], ((const uint32_t *) cl)[45] & 0xffffffff);
   if (((const uint32_t *) cl)[46] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 46: got %X, bad mask %X\n", ((const uint32_t *) cl)[46], ((const uint32_t *) cl)[46] & 0xffffffff);
   if (((const uint32_t *) cl)[47] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 47: got %X, bad mask %X\n", ((const uint32_t *) cl)[47], ((const uint32_t *) cl)[47] & 0xffffffff);
   if (((const uint32_t *) cl)[48] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 48: got %X, bad mask %X\n", ((const uint32_t *) cl)[48], ((const uint32_t *) cl)[48] & 0xffffffff);
   if (((const uint32_t *) cl)[49] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 49: got %X, bad mask %X\n", ((const uint32_t *) cl)[49], ((const uint32_t *) cl)[49] & 0xffffffff);
   if (((const uint32_t *) cl)[50] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 50: got %X, bad mask %X\n", ((const uint32_t *) cl)[50], ((const uint32_t *) cl)[50] & 0xffffffff);
   if (((const uint32_t *) cl)[51] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 51: got %X, bad mask %X\n", ((const uint32_t *) cl)[51], ((const uint32_t *) cl)[51] & 0xffffffff);
   if (((const uint32_t *) cl)[52] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 52: got %X, bad mask %X\n", ((const uint32_t *) cl)[52], ((const uint32_t *) cl)[52] & 0xffffffff);
   if (((const uint32_t *) cl)[53] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 53: got %X, bad mask %X\n", ((const uint32_t *) cl)[53], ((const uint32_t *) cl)[53] & 0xffffffff);
   if (((const uint32_t *) cl)[54] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 54: got %X, bad mask %X\n", ((const uint32_t *) cl)[54], ((const uint32_t *) cl)[54] & 0xffffffff);
   if (((const uint32_t *) cl)[55] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 55: got %X, bad mask %X\n", ((const uint32_t *) cl)[55], ((const uint32_t *) cl)[55] & 0xffffffff);
   if (((const uint32_t *) cl)[56] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 56: got %X, bad mask %X\n", ((const uint32_t *) cl)[56], ((const uint32_t *) cl)[56] & 0xffffffff);
   if (((const uint32_t *) cl)[57] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 57: got %X, bad mask %X\n", ((const uint32_t *) cl)[57], ((const uint32_t *) cl)[57] & 0xffffffff);
   if (((const uint32_t *) cl)[58] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 58: got %X, bad mask %X\n", ((const uint32_t *) cl)[58], ((const uint32_t *) cl)[58] & 0xffffffff);
   if (((const uint32_t *) cl)[59] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 59: got %X, bad mask %X\n", ((const uint32_t *) cl)[59], ((const uint32_t *) cl)[59] & 0xffffffff);
   if (((const uint32_t *) cl)[60] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 60: got %X, bad mask %X\n", ((const uint32_t *) cl)[60], ((const uint32_t *) cl)[60] & 0xffffffff);
   if (((const uint32_t *) cl)[61] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 61: got %X, bad mask %X\n", ((const uint32_t *) cl)[61], ((const uint32_t *) cl)[61] & 0xffffffff);
   if (((const uint32_t *) cl)[62] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 62: got %X, bad mask %X\n", ((const uint32_t *) cl)[62], ((const uint32_t *) cl)[62] & 0xffffffff);
   if (((const uint32_t *) cl)[63] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 63: got %X, bad mask %X\n", ((const uint32_t *) cl)[63], ((const uint32_t *) cl)[63] & 0xffffffff);
   if (((const uint32_t *) cl)[64] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 64: got %X, bad mask %X\n", ((const uint32_t *) cl)[64], ((const uint32_t *) cl)[64] & 0xffffffff);
   if (((const uint32_t *) cl)[65] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 65: got %X, bad mask %X\n", ((const uint32_t *) cl)[65], ((const uint32_t *) cl)[65] & 0xffffffff);
   if (((const uint32_t *) cl)[66] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 66: got %X, bad mask %X\n", ((const uint32_t *) cl)[66], ((const uint32_t *) cl)[66] & 0xffffffff);
   if (((const uint32_t *) cl)[67] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 67: got %X, bad mask %X\n", ((const uint32_t *) cl)[67], ((const uint32_t *) cl)[67] & 0xffffffff);
   if (((const uint32_t *) cl)[68] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 68: got %X, bad mask %X\n", ((const uint32_t *) cl)[68], ((const uint32_t *) cl)[68] & 0xffffffff);
   if (((const uint32_t *) cl)[69] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 69: got %X, bad mask %X\n", ((const uint32_t *) cl)[69], ((const uint32_t *) cl)[69] & 0xffffffff);
   if (((const uint32_t *) cl)[70] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 70: got %X, bad mask %X\n", ((const uint32_t *) cl)[70], ((const uint32_t *) cl)[70] & 0xffffffff);
   if (((const uint32_t *) cl)[71] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 71: got %X, bad mask %X\n", ((const uint32_t *) cl)[71], ((const uint32_t *) cl)[71] & 0xffffffff);
   if (((const uint32_t *) cl)[72] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 72: got %X, bad mask %X\n", ((const uint32_t *) cl)[72], ((const uint32_t *) cl)[72] & 0xffffffff);
   if (((const uint32_t *) cl)[73] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 73: got %X, bad mask %X\n", ((const uint32_t *) cl)[73], ((const uint32_t *) cl)[73] & 0xffffffff);
   if (((const uint32_t *) cl)[74] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 74: got %X, bad mask %X\n", ((const uint32_t *) cl)[74], ((const uint32_t *) cl)[74] & 0xffffffff);
   if (((const uint32_t *) cl)[75] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 75: got %X, bad mask %X\n", ((const uint32_t *) cl)[75], ((const uint32_t *) cl)[75] & 0xffffffff);
   if (((const uint32_t *) cl)[76] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 76: got %X, bad mask %X\n", ((const uint32_t *) cl)[76], ((const uint32_t *) cl)[76] & 0xffffffff);
   if (((const uint32_t *) cl)[77] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 77: got %X, bad mask %X\n", ((const uint32_t *) cl)[77], ((const uint32_t *) cl)[77] & 0xffffffff);
   if (((const uint32_t *) cl)[78] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 78: got %X, bad mask %X\n", ((const uint32_t *) cl)[78], ((const uint32_t *) cl)[78] & 0xffffffff);
   if (((const uint32_t *) cl)[79] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 79: got %X, bad mask %X\n", ((const uint32_t *) cl)[79], ((const uint32_t *) cl)[79] & 0xffffffff);
   if (((const uint32_t *) cl)[80] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 80: got %X, bad mask %X\n", ((const uint32_t *) cl)[80], ((const uint32_t *) cl)[80] & 0xffffffff);
   if (((const uint32_t *) cl)[81] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 81: got %X, bad mask %X\n", ((const uint32_t *) cl)[81], ((const uint32_t *) cl)[81] & 0xffffffff);
   if (((const uint32_t *) cl)[82] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 82: got %X, bad mask %X\n", ((const uint32_t *) cl)[82], ((const uint32_t *) cl)[82] & 0xffffffff);
   if (((const uint32_t *) cl)[83] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 83: got %X, bad mask %X\n", ((const uint32_t *) cl)[83], ((const uint32_t *) cl)[83] & 0xffffffff);
   if (((const uint32_t *) cl)[84] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 84: got %X, bad mask %X\n", ((const uint32_t *) cl)[84], ((const uint32_t *) cl)[84] & 0xffffffff);
   if (((const uint32_t *) cl)[85] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 85: got %X, bad mask %X\n", ((const uint32_t *) cl)[85], ((const uint32_t *) cl)[85] & 0xffffffff);
   if (((const uint32_t *) cl)[86] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 86: got %X, bad mask %X\n", ((const uint32_t *) cl)[86], ((const uint32_t *) cl)[86] & 0xffffffff);
   if (((const uint32_t *) cl)[87] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 87: got %X, bad mask %X\n", ((const uint32_t *) cl)[87], ((const uint32_t *) cl)[87] & 0xffffffff);
   if (((const uint32_t *) cl)[88] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 88: got %X, bad mask %X\n", ((const uint32_t *) cl)[88], ((const uint32_t *) cl)[88] & 0xffffffff);
   if (((const uint32_t *) cl)[89] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 89: got %X, bad mask %X\n", ((const uint32_t *) cl)[89], ((const uint32_t *) cl)[89] & 0xffffffff);
   if (((const uint32_t *) cl)[90] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 90: got %X, bad mask %X\n", ((const uint32_t *) cl)[90], ((const uint32_t *) cl)[90] & 0xffffffff);
   if (((const uint32_t *) cl)[91] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 91: got %X, bad mask %X\n", ((const uint32_t *) cl)[91], ((const uint32_t *) cl)[91] & 0xffffffff);
   if (((const uint32_t *) cl)[92] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 92: got %X, bad mask %X\n", ((const uint32_t *) cl)[92], ((const uint32_t *) cl)[92] & 0xffffffff);
   if (((const uint32_t *) cl)[93] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 93: got %X, bad mask %X\n", ((const uint32_t *) cl)[93], ((const uint32_t *) cl)[93] & 0xffffffff);
   if (((const uint32_t *) cl)[97] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 97: got %X, bad mask %X\n", ((const uint32_t *) cl)[97], ((const uint32_t *) cl)[97] & 0xffffffff);
   if (((const uint32_t *) cl)[99] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 99: got %X, bad mask %X\n", ((const uint32_t *) cl)[99], ((const uint32_t *) cl)[99] & 0xffffffff);
   if (((const uint32_t *) cl)[100] & 0xfefffffe) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 100: got %X, bad mask %X\n", ((const uint32_t *) cl)[100], ((const uint32_t *) cl)[100] & 0xfefffffe);
   if (((const uint32_t *) cl)[101] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 101: got %X, bad mask %X\n", ((const uint32_t *) cl)[101], ((const uint32_t *) cl)[101] & 0xffffffff);
   if (((const uint32_t *) cl)[105] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 105: got %X, bad mask %X\n", ((const uint32_t *) cl)[105], ((const uint32_t *) cl)[105] & 0xffffffff);
   if (((const uint32_t *) cl)[106] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 106: got %X, bad mask %X\n", ((const uint32_t *) cl)[106], ((const uint32_t *) cl)[106] & 0xffffffff);
   if (((const uint32_t *) cl)[107] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 107: got %X, bad mask %X\n", ((const uint32_t *) cl)[107], ((const uint32_t *) cl)[107] & 0xffffffff);
   if (((const uint32_t *) cl)[108] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 108: got %X, bad mask %X\n", ((const uint32_t *) cl)[108], ((const uint32_t *) cl)[108] & 0xffffffff);
   if (((const uint32_t *) cl)[109] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 109: got %X, bad mask %X\n", ((const uint32_t *) cl)[109], ((const uint32_t *) cl)[109] & 0xffffffff);
   if (((const uint32_t *) cl)[110] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 110: got %X, bad mask %X\n", ((const uint32_t *) cl)[110], ((const uint32_t *) cl)[110] & 0xffffffff);
   if (((const uint32_t *) cl)[111] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 111: got %X, bad mask %X\n", ((const uint32_t *) cl)[111], ((const uint32_t *) cl)[111] & 0xffffffff);
   if (((const uint32_t *) cl)[112] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 112: got %X, bad mask %X\n", ((const uint32_t *) cl)[112], ((const uint32_t *) cl)[112] & 0xffffffff);
   if (((const uint32_t *) cl)[113] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 113: got %X, bad mask %X\n", ((const uint32_t *) cl)[113], ((const uint32_t *) cl)[113] & 0xffffffff);
   if (((const uint32_t *) cl)[114] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 114: got %X, bad mask %X\n", ((const uint32_t *) cl)[114], ((const uint32_t *) cl)[114] & 0xffffffff);
   if (((const uint32_t *) cl)[115] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 115: got %X, bad mask %X\n", ((const uint32_t *) cl)[115], ((const uint32_t *) cl)[115] & 0xffffffff);
   if (((const uint32_t *) cl)[116] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 116: got %X, bad mask %X\n", ((const uint32_t *) cl)[116], ((const uint32_t *) cl)[116] & 0xffffffff);
   if (((const uint32_t *) cl)[117] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 117: got %X, bad mask %X\n", ((const uint32_t *) cl)[117], ((const uint32_t *) cl)[117] & 0xffffffff);
   if (((const uint32_t *) cl)[118] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 118: got %X, bad mask %X\n", ((const uint32_t *) cl)[118], ((const uint32_t *) cl)[118] & 0xffffffff);
   if (((const uint32_t *) cl)[119] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 119: got %X, bad mask %X\n", ((const uint32_t *) cl)[119], ((const uint32_t *) cl)[119] & 0xffffffff);
   if (((const uint32_t *) cl)[120] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 120: got %X, bad mask %X\n", ((const uint32_t *) cl)[120], ((const uint32_t *) cl)[120] & 0xffffffff);
   if (((const uint32_t *) cl)[121] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 121: got %X, bad mask %X\n", ((const uint32_t *) cl)[121], ((const uint32_t *) cl)[121] & 0xffffffff);
   if (((const uint32_t *) cl)[122] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 122: got %X, bad mask %X\n", ((const uint32_t *) cl)[122], ((const uint32_t *) cl)[122] & 0xffffffff);
   if (((const uint32_t *) cl)[123] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 123: got %X, bad mask %X\n", ((const uint32_t *) cl)[123], ((const uint32_t *) cl)[123] & 0xffffffff);
   if (((const uint32_t *) cl)[124] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 124: got %X, bad mask %X\n", ((const uint32_t *) cl)[124], ((const uint32_t *) cl)[124] & 0xffffffff);
   if (((const uint32_t *) cl)[125] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 125: got %X, bad mask %X\n", ((const uint32_t *) cl)[125], ((const uint32_t *) cl)[125] & 0xffffffff);
   if (((const uint32_t *) cl)[126] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 126: got %X, bad mask %X\n", ((const uint32_t *) cl)[126], ((const uint32_t *) cl)[126] & 0xffffffff);
   if (((const uint32_t *) cl)[127] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 127: got %X, bad mask %X\n", ((const uint32_t *) cl)[127], ((const uint32_t *) cl)[127] & 0xffffffff);
   if (((const uint32_t *) cl)[128] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 128: got %X, bad mask %X\n", ((const uint32_t *) cl)[128], ((const uint32_t *) cl)[128] & 0xffffffff);
   if (((const uint32_t *) cl)[129] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 129: got %X, bad mask %X\n", ((const uint32_t *) cl)[129], ((const uint32_t *) cl)[129] & 0xffffffff);
   if (((const uint32_t *) cl)[130] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 130: got %X, bad mask %X\n", ((const uint32_t *) cl)[130], ((const uint32_t *) cl)[130] & 0xffffffff);
   if (((const uint32_t *) cl)[131] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 131: got %X, bad mask %X\n", ((const uint32_t *) cl)[131], ((const uint32_t *) cl)[131] & 0xffffffff);
   if (((const uint32_t *) cl)[132] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 132: got %X, bad mask %X\n", ((const uint32_t *) cl)[132], ((const uint32_t *) cl)[132] & 0xffffffff);
   if (((const uint32_t *) cl)[133] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 133: got %X, bad mask %X\n", ((const uint32_t *) cl)[133], ((const uint32_t *) cl)[133] & 0xffffffff);
   if (((const uint32_t *) cl)[134] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 134: got %X, bad mask %X\n", ((const uint32_t *) cl)[134], ((const uint32_t *) cl)[134] & 0xffffffff);
   if (((const uint32_t *) cl)[135] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 135: got %X, bad mask %X\n", ((const uint32_t *) cl)[135], ((const uint32_t *) cl)[135] & 0xffffffff);
   if (((const uint32_t *) cl)[136] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 136: got %X, bad mask %X\n", ((const uint32_t *) cl)[136], ((const uint32_t *) cl)[136] & 0xffffffff);
   if (((const uint32_t *) cl)[137] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 137: got %X, bad mask %X\n", ((const uint32_t *) cl)[137], ((const uint32_t *) cl)[137] & 0xffffffff);
   if (((const uint32_t *) cl)[138] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 138: got %X, bad mask %X\n", ((const uint32_t *) cl)[138], ((const uint32_t *) cl)[138] & 0xffffffff);
   if (((const uint32_t *) cl)[139] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 139: got %X, bad mask %X\n", ((const uint32_t *) cl)[139], ((const uint32_t *) cl)[139] & 0xffffffff);
   if (((const uint32_t *) cl)[140] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 140: got %X, bad mask %X\n", ((const uint32_t *) cl)[140], ((const uint32_t *) cl)[140] & 0xffffffff);
   if (((const uint32_t *) cl)[141] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 141: got %X, bad mask %X\n", ((const uint32_t *) cl)[141], ((const uint32_t *) cl)[141] & 0xffffffff);
   if (((const uint32_t *) cl)[142] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 142: got %X, bad mask %X\n", ((const uint32_t *) cl)[142], ((const uint32_t *) cl)[142] & 0xffffffff);
   if (((const uint32_t *) cl)[143] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 143: got %X, bad mask %X\n", ((const uint32_t *) cl)[143], ((const uint32_t *) cl)[143] & 0xffffffff);
   if (((const uint32_t *) cl)[144] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 144: got %X, bad mask %X\n", ((const uint32_t *) cl)[144], ((const uint32_t *) cl)[144] & 0xffffffff);
   if (((const uint32_t *) cl)[145] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 145: got %X, bad mask %X\n", ((const uint32_t *) cl)[145], ((const uint32_t *) cl)[145] & 0xffffffff);
   if (((const uint32_t *) cl)[147] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 147: got %X, bad mask %X\n", ((const uint32_t *) cl)[147], ((const uint32_t *) cl)[147] & 0xffffffff);
   if (((const uint32_t *) cl)[149] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 149: got %X, bad mask %X\n", ((const uint32_t *) cl)[149], ((const uint32_t *) cl)[149] & 0xffffffff);
   if (((const uint32_t *) cl)[150] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 150: got %X, bad mask %X\n", ((const uint32_t *) cl)[150], ((const uint32_t *) cl)[150] & 0xffffffff);
   if (((const uint32_t *) cl)[151] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 151: got %X, bad mask %X\n", ((const uint32_t *) cl)[151], ((const uint32_t *) cl)[151] & 0xffffffff);
   if (((const uint32_t *) cl)[152] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 152: got %X, bad mask %X\n", ((const uint32_t *) cl)[152], ((const uint32_t *) cl)[152] & 0xffffffff);
   if (((const uint32_t *) cl)[153] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 153: got %X, bad mask %X\n", ((const uint32_t *) cl)[153], ((const uint32_t *) cl)[153] & 0xffffffff);
   if (((const uint32_t *) cl)[155] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 155: got %X, bad mask %X\n", ((const uint32_t *) cl)[155], ((const uint32_t *) cl)[155] & 0xffffffff);
   if (((const uint32_t *) cl)[157] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 157: got %X, bad mask %X\n", ((const uint32_t *) cl)[157], ((const uint32_t *) cl)[157] & 0xffffffff);
   if (((const uint32_t *) cl)[162] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 162: got %X, bad mask %X\n", ((const uint32_t *) cl)[162], ((const uint32_t *) cl)[162] & 0xffffffff);
   if (((const uint32_t *) cl)[163] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 163: got %X, bad mask %X\n", ((const uint32_t *) cl)[163], ((const uint32_t *) cl)[163] & 0xffffffff);
   if (((const uint32_t *) cl)[164] & 0xf9f33e00) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 164: got %X, bad mask %X\n", ((const uint32_t *) cl)[164], ((const uint32_t *) cl)[164] & 0xf9f33e00);
   if (((const uint32_t *) cl)[165] & 0xfaffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 165: got %X, bad mask %X\n", ((const uint32_t *) cl)[165], ((const uint32_t *) cl)[165] & 0xfaffffff);
   if (((const uint32_t *) cl)[166] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 166: got %X, bad mask %X\n", ((const uint32_t *) cl)[166], ((const uint32_t *) cl)[166] & 0xffffffff);
   if (((const uint32_t *) cl)[167] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 167: got %X, bad mask %X\n", ((const uint32_t *) cl)[167], ((const uint32_t *) cl)[167] & 0xffffffff);
   if (((const uint32_t *) cl)[168] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 168: got %X, bad mask %X\n", ((const uint32_t *) cl)[168], ((const uint32_t *) cl)[168] & 0xffffffff);
   if (((const uint32_t *) cl)[169] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 169: got %X, bad mask %X\n", ((const uint32_t *) cl)[169], ((const uint32_t *) cl)[169] & 0xffffffff);
   if (((const uint32_t *) cl)[170] & 0xc0000000) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 170: got %X, bad mask %X\n", ((const uint32_t *) cl)[170], ((const uint32_t *) cl)[170] & 0xc0000000);
   if (((const uint32_t *) cl)[171] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 171: got %X, bad mask %X\n", ((const uint32_t *) cl)[171], ((const uint32_t *) cl)[171] & 0xffffffff);
   if (((const uint32_t *) cl)[172] & 0x7f) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 172: got %X, bad mask %X\n", ((const uint32_t *) cl)[172], ((const uint32_t *) cl)[172] & 0x7f);
   if (((const uint32_t *) cl)[173] & 0xffffff00) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 173: got %X, bad mask %X\n", ((const uint32_t *) cl)[173], ((const uint32_t *) cl)[173] & 0xffffff00);
   if (((const uint32_t *) cl)[174] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 174: got %X, bad mask %X\n", ((const uint32_t *) cl)[174], ((const uint32_t *) cl)[174] & 0xffffffff);
   if (((const uint32_t *) cl)[175] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 175: got %X, bad mask %X\n", ((const uint32_t *) cl)[175], ((const uint32_t *) cl)[175] & 0xffffffff);
   if (((const uint32_t *) cl)[177] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 177: got %X, bad mask %X\n", ((const uint32_t *) cl)[177], ((const uint32_t *) cl)[177] & 0xffffffff);
   if (((const uint32_t *) cl)[178] & 0x7f) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 178: got %X, bad mask %X\n", ((const uint32_t *) cl)[178], ((const uint32_t *) cl)[178] & 0x7f);
   if (((const uint32_t *) cl)[179] & 0xffffff00) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 179: got %X, bad mask %X\n", ((const uint32_t *) cl)[179], ((const uint32_t *) cl)[179] & 0xffffff00);
   if (((const uint32_t *) cl)[180] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 180: got %X, bad mask %X\n", ((const uint32_t *) cl)[180], ((const uint32_t *) cl)[180] & 0xffffffff);
   if (((const uint32_t *) cl)[181] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 181: got %X, bad mask %X\n", ((const uint32_t *) cl)[181], ((const uint32_t *) cl)[181] & 0xffffffff);
   if (((const uint32_t *) cl)[182] & 0x7f) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 182: got %X, bad mask %X\n", ((const uint32_t *) cl)[182], ((const uint32_t *) cl)[182] & 0x7f);
   if (((const uint32_t *) cl)[183] & 0xffffff00) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 183: got %X, bad mask %X\n", ((const uint32_t *) cl)[183], ((const uint32_t *) cl)[183] & 0xffffff00);
   if (((const uint32_t *) cl)[184] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 184: got %X, bad mask %X\n", ((const uint32_t *) cl)[184], ((const uint32_t *) cl)[184] & 0xffffffff);
   if (((const uint32_t *) cl)[185] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 185: got %X, bad mask %X\n", ((const uint32_t *) cl)[185], ((const uint32_t *) cl)[185] & 0xffffffff);
   if (((const uint32_t *) cl)[187] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 187: got %X, bad mask %X\n", ((const uint32_t *) cl)[187], ((const uint32_t *) cl)[187] & 0xffffffff);
   if (((const uint32_t *) cl)[188] & 0x7f) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 188: got %X, bad mask %X\n", ((const uint32_t *) cl)[188], ((const uint32_t *) cl)[188] & 0x7f);
   if (((const uint32_t *) cl)[189] & 0xffffff00) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 189: got %X, bad mask %X\n", ((const uint32_t *) cl)[189], ((const uint32_t *) cl)[189] & 0xffffff00);
   if (((const uint32_t *) cl)[190] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 190: got %X, bad mask %X\n", ((const uint32_t *) cl)[190], ((const uint32_t *) cl)[190] & 0xffffffff);
   if (((const uint32_t *) cl)[191] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 191: got %X, bad mask %X\n", ((const uint32_t *) cl)[191], ((const uint32_t *) cl)[191] & 0xffffffff);
   if (((const uint32_t *) cl)[192] & 0x7f) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 192: got %X, bad mask %X\n", ((const uint32_t *) cl)[192], ((const uint32_t *) cl)[192] & 0x7f);
   if (((const uint32_t *) cl)[193] & 0xffffff00) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 193: got %X, bad mask %X\n", ((const uint32_t *) cl)[193], ((const uint32_t *) cl)[193] & 0xffffff00);
   if (((const uint32_t *) cl)[194] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 194: got %X, bad mask %X\n", ((const uint32_t *) cl)[194], ((const uint32_t *) cl)[194] & 0xffffffff);
   if (((const uint32_t *) cl)[195] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 195: got %X, bad mask %X\n", ((const uint32_t *) cl)[195], ((const uint32_t *) cl)[195] & 0xffffffff);
   if (((const uint32_t *) cl)[197] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 197: got %X, bad mask %X\n", ((const uint32_t *) cl)[197], ((const uint32_t *) cl)[197] & 0xffffffff);
   if (((const uint32_t *) cl)[198] & 0x7f) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 198: got %X, bad mask %X\n", ((const uint32_t *) cl)[198], ((const uint32_t *) cl)[198] & 0x7f);
   if (((const uint32_t *) cl)[199] & 0xffffff00) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 199: got %X, bad mask %X\n", ((const uint32_t *) cl)[199], ((const uint32_t *) cl)[199] & 0xffffff00);
   if (((const uint32_t *) cl)[200] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 200: got %X, bad mask %X\n", ((const uint32_t *) cl)[200], ((const uint32_t *) cl)[200] & 0xffffffff);
   if (((const uint32_t *) cl)[201] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 201: got %X, bad mask %X\n", ((const uint32_t *) cl)[201], ((const uint32_t *) cl)[201] & 0xffffffff);
   if (((const uint32_t *) cl)[202] & 0x7f) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 202: got %X, bad mask %X\n", ((const uint32_t *) cl)[202], ((const uint32_t *) cl)[202] & 0x7f);
   if (((const uint32_t *) cl)[203] & 0xffffff00) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 203: got %X, bad mask %X\n", ((const uint32_t *) cl)[203], ((const uint32_t *) cl)[203] & 0xffffff00);
   if (((const uint32_t *) cl)[204] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 204: got %X, bad mask %X\n", ((const uint32_t *) cl)[204], ((const uint32_t *) cl)[204] & 0xffffffff);
   if (((const uint32_t *) cl)[205] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 205: got %X, bad mask %X\n", ((const uint32_t *) cl)[205], ((const uint32_t *) cl)[205] & 0xffffffff);
   if (((const uint32_t *) cl)[207] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 207: got %X, bad mask %X\n", ((const uint32_t *) cl)[207], ((const uint32_t *) cl)[207] & 0xffffffff);
   if (((const uint32_t *) cl)[208] & 0x7f) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 208: got %X, bad mask %X\n", ((const uint32_t *) cl)[208], ((const uint32_t *) cl)[208] & 0x7f);
   if (((const uint32_t *) cl)[209] & 0xffffff00) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 209: got %X, bad mask %X\n", ((const uint32_t *) cl)[209], ((const uint32_t *) cl)[209] & 0xffffff00);
   if (((const uint32_t *) cl)[210] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 210: got %X, bad mask %X\n", ((const uint32_t *) cl)[210], ((const uint32_t *) cl)[210] & 0xffffffff);
   if (((const uint32_t *) cl)[211] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 211: got %X, bad mask %X\n", ((const uint32_t *) cl)[211], ((const uint32_t *) cl)[211] & 0xffffffff);
   if (((const uint32_t *) cl)[213] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 213: got %X, bad mask %X\n", ((const uint32_t *) cl)[213], ((const uint32_t *) cl)[213] & 0xffffffff);
   if (((const uint32_t *) cl)[214] & 0xfffb0000) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 214: got %X, bad mask %X\n", ((const uint32_t *) cl)[214], ((const uint32_t *) cl)[214] & 0xfffb0000);
   if (((const uint32_t *) cl)[215] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 215: got %X, bad mask %X\n", ((const uint32_t *) cl)[215], ((const uint32_t *) cl)[215] & 0xffffffff);
   if (((const uint32_t *) cl)[220] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 220: got %X, bad mask %X\n", ((const uint32_t *) cl)[220], ((const uint32_t *) cl)[220] & 0xffffffff);
   if (((const uint32_t *) cl)[221] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 221: got %X, bad mask %X\n", ((const uint32_t *) cl)[221], ((const uint32_t *) cl)[221] & 0xffffffff);
   if (((const uint32_t *) cl)[222] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 222: got %X, bad mask %X\n", ((const uint32_t *) cl)[222], ((const uint32_t *) cl)[222] & 0xffffffff);
   if (((const uint32_t *) cl)[223] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 223: got %X, bad mask %X\n", ((const uint32_t *) cl)[223], ((const uint32_t *) cl)[223] & 0xffffffff);
   if (((const uint32_t *) cl)[224] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 224: got %X, bad mask %X\n", ((const uint32_t *) cl)[224], ((const uint32_t *) cl)[224] & 0xffffffff);
   if (((const uint32_t *) cl)[225] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 225: got %X, bad mask %X\n", ((const uint32_t *) cl)[225], ((const uint32_t *) cl)[225] & 0xffffffff);
   if (((const uint32_t *) cl)[227] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 227: got %X, bad mask %X\n", ((const uint32_t *) cl)[227], ((const uint32_t *) cl)[227] & 0xffffffff);
   if (((const uint32_t *) cl)[228] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 228: got %X, bad mask %X\n", ((const uint32_t *) cl)[228], ((const uint32_t *) cl)[228] & 0xffffffff);
   if (((const uint32_t *) cl)[229] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 229: got %X, bad mask %X\n", ((const uint32_t *) cl)[229], ((const uint32_t *) cl)[229] & 0xffffffff);
   if (((const uint32_t *) cl)[230] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 230: got %X, bad mask %X\n", ((const uint32_t *) cl)[230], ((const uint32_t *) cl)[230] & 0xffffffff);
   if (((const uint32_t *) cl)[231] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 231: got %X, bad mask %X\n", ((const uint32_t *) cl)[231], ((const uint32_t *) cl)[231] & 0xffffffff);
   if (((const uint32_t *) cl)[232] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 232: got %X, bad mask %X\n", ((const uint32_t *) cl)[232], ((const uint32_t *) cl)[232] & 0xffffffff);
   if (((const uint32_t *) cl)[233] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 233: got %X, bad mask %X\n", ((const uint32_t *) cl)[233], ((const uint32_t *) cl)[233] & 0xffffffff);
   if (((const uint32_t *) cl)[234] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 234: got %X, bad mask %X\n", ((const uint32_t *) cl)[234], ((const uint32_t *) cl)[234] & 0xffffffff);
   if (((const uint32_t *) cl)[235] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 235: got %X, bad mask %X\n", ((const uint32_t *) cl)[235], ((const uint32_t *) cl)[235] & 0xffffffff);
   if (((const uint32_t *) cl)[236] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 236: got %X, bad mask %X\n", ((const uint32_t *) cl)[236], ((const uint32_t *) cl)[236] & 0xffffffff);
   if (((const uint32_t *) cl)[237] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 237: got %X, bad mask %X\n", ((const uint32_t *) cl)[237], ((const uint32_t *) cl)[237] & 0xffffffff);
   if (((const uint32_t *) cl)[238] & 0xfffffff0) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 238: got %X, bad mask %X\n", ((const uint32_t *) cl)[238], ((const uint32_t *) cl)[238] & 0xfffffff0);
   if (((const uint32_t *) cl)[239] & 0xfffffff0) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 239: got %X, bad mask %X\n", ((const uint32_t *) cl)[239], ((const uint32_t *) cl)[239] & 0xfffffff0);
   if (((const uint32_t *) cl)[240] & 0xfffffff0) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 240: got %X, bad mask %X\n", ((const uint32_t *) cl)[240], ((const uint32_t *) cl)[240] & 0xfffffff0);
   if (((const uint32_t *) cl)[241] & 0xfffffff0) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 241: got %X, bad mask %X\n", ((const uint32_t *) cl)[241], ((const uint32_t *) cl)[241] & 0xfffffff0);
   if (((const uint32_t *) cl)[242] & 0xfffffff0) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 242: got %X, bad mask %X\n", ((const uint32_t *) cl)[242], ((const uint32_t *) cl)[242] & 0xfffffff0);
   if (((const uint32_t *) cl)[243] & 0xfffffff0) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 243: got %X, bad mask %X\n", ((const uint32_t *) cl)[243], ((const uint32_t *) cl)[243] & 0xfffffff0);
   if (((const uint32_t *) cl)[244] & 0xfffffff0) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 244: got %X, bad mask %X\n", ((const uint32_t *) cl)[244], ((const uint32_t *) cl)[244] & 0xfffffff0);
   if (((const uint32_t *) cl)[245] & 0xfffffff0) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 245: got %X, bad mask %X\n", ((const uint32_t *) cl)[245], ((const uint32_t *) cl)[245] & 0xfffffff0);
   if (((const uint32_t *) cl)[246] & 0xfffffff0) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 246: got %X, bad mask %X\n", ((const uint32_t *) cl)[246], ((const uint32_t *) cl)[246] & 0xfffffff0);
   if (((const uint32_t *) cl)[247] & 0xfffffff0) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 247: got %X, bad mask %X\n", ((const uint32_t *) cl)[247], ((const uint32_t *) cl)[247] & 0xfffffff0);
   if (((const uint32_t *) cl)[248] & 0xfffffff0) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 248: got %X, bad mask %X\n", ((const uint32_t *) cl)[248], ((const uint32_t *) cl)[248] & 0xfffffff0);
   if (((const uint32_t *) cl)[249] & 0xfffffff0) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 249: got %X, bad mask %X\n", ((const uint32_t *) cl)[249], ((const uint32_t *) cl)[249] & 0xfffffff0);
   if (((const uint32_t *) cl)[250] & 0xfffffff0) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 250: got %X, bad mask %X\n", ((const uint32_t *) cl)[250], ((const uint32_t *) cl)[250] & 0xfffffff0);
   if (((const uint32_t *) cl)[251] & 0xfffffff0) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 251: got %X, bad mask %X\n", ((const uint32_t *) cl)[251], ((const uint32_t *) cl)[251] & 0xfffffff0);
   if (((const uint32_t *) cl)[252] & 0xfffffff0) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 252: got %X, bad mask %X\n", ((const uint32_t *) cl)[252], ((const uint32_t *) cl)[252] & 0xfffffff0);
   if (((const uint32_t *) cl)[253] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 253: got %X, bad mask %X\n", ((const uint32_t *) cl)[253], ((const uint32_t *) cl)[253] & 0xffffffff);
   if (((const uint32_t *) cl)[254] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 254: got %X, bad mask %X\n", ((const uint32_t *) cl)[254], ((const uint32_t *) cl)[254] & 0xffffffff);
   if (((const uint32_t *) cl)[255] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 255: got %X, bad mask %X\n", ((const uint32_t *) cl)[255], ((const uint32_t *) cl)[255] & 0xffffffff);
   if (((const uint32_t *) cl)[256] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 256: got %X, bad mask %X\n", ((const uint32_t *) cl)[256], ((const uint32_t *) cl)[256] & 0xffffffff);
   if (((const uint32_t *) cl)[257] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 257: got %X, bad mask %X\n", ((const uint32_t *) cl)[257], ((const uint32_t *) cl)[257] & 0xffffffff);
   if (((const uint32_t *) cl)[258] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 258: got %X, bad mask %X\n", ((const uint32_t *) cl)[258], ((const uint32_t *) cl)[258] & 0xffffffff);
   if (((const uint32_t *) cl)[259] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 259: got %X, bad mask %X\n", ((const uint32_t *) cl)[259], ((const uint32_t *) cl)[259] & 0xffffffff);
   if (((const uint32_t *) cl)[260] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 260: got %X, bad mask %X\n", ((const uint32_t *) cl)[260], ((const uint32_t *) cl)[260] & 0xffffffff);
   if (((const uint32_t *) cl)[261] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 261: got %X, bad mask %X\n", ((const uint32_t *) cl)[261], ((const uint32_t *) cl)[261] & 0xffffffff);
   if (((const uint32_t *) cl)[262] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 262: got %X, bad mask %X\n", ((const uint32_t *) cl)[262], ((const uint32_t *) cl)[262] & 0xffffffff);
   if (((const uint32_t *) cl)[263] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 263: got %X, bad mask %X\n", ((const uint32_t *) cl)[263], ((const uint32_t *) cl)[263] & 0xffffffff);
   if (((const uint32_t *) cl)[264] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 264: got %X, bad mask %X\n", ((const uint32_t *) cl)[264], ((const uint32_t *) cl)[264] & 0xffffffff);
   if (((const uint32_t *) cl)[265] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 265: got %X, bad mask %X\n", ((const uint32_t *) cl)[265], ((const uint32_t *) cl)[265] & 0xffffffff);
   if (((const uint32_t *) cl)[266] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 266: got %X, bad mask %X\n", ((const uint32_t *) cl)[266], ((const uint32_t *) cl)[266] & 0xffffffff);
   if (((const uint32_t *) cl)[267] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 267: got %X, bad mask %X\n", ((const uint32_t *) cl)[267], ((const uint32_t *) cl)[267] & 0xffffffff);
   if (((const uint32_t *) cl)[268] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 268: got %X, bad mask %X\n", ((const uint32_t *) cl)[268], ((const uint32_t *) cl)[268] & 0xffffffff);
   if (((const uint32_t *) cl)[269] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 269: got %X, bad mask %X\n", ((const uint32_t *) cl)[269], ((const uint32_t *) cl)[269] & 0xffffffff);
   if (((const uint32_t *) cl)[270] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 270: got %X, bad mask %X\n", ((const uint32_t *) cl)[270], ((const uint32_t *) cl)[270] & 0xffffffff);
   if (((const uint32_t *) cl)[271] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 271: got %X, bad mask %X\n", ((const uint32_t *) cl)[271], ((const uint32_t *) cl)[271] & 0xffffffff);
   if (((const uint32_t *) cl)[272] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 272: got %X, bad mask %X\n", ((const uint32_t *) cl)[272], ((const uint32_t *) cl)[272] & 0xffffffff);
   if (((const uint32_t *) cl)[273] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 273: got %X, bad mask %X\n", ((const uint32_t *) cl)[273], ((const uint32_t *) cl)[273] & 0xffffffff);
   if (((const uint32_t *) cl)[274] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 274: got %X, bad mask %X\n", ((const uint32_t *) cl)[274], ((const uint32_t *) cl)[274] & 0xffffffff);
   if (((const uint32_t *) cl)[275] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 275: got %X, bad mask %X\n", ((const uint32_t *) cl)[275], ((const uint32_t *) cl)[275] & 0xffffffff);
   if (((const uint32_t *) cl)[277] & 0xffff0000) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 277: got %X, bad mask %X\n", ((const uint32_t *) cl)[277], ((const uint32_t *) cl)[277] & 0xffff0000);
   if (((const uint32_t *) cl)[278] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 278: got %X, bad mask %X\n", ((const uint32_t *) cl)[278], ((const uint32_t *) cl)[278] & 0xffffffff);
   if (((const uint32_t *) cl)[279] & 0xfefffeff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 279: got %X, bad mask %X\n", ((const uint32_t *) cl)[279], ((const uint32_t *) cl)[279] & 0xfefffeff);
   if (((const uint32_t *) cl)[280] & 0xfeffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 280: got %X, bad mask %X\n", ((const uint32_t *) cl)[280], ((const uint32_t *) cl)[280] & 0xfeffffff);
   if (((const uint32_t *) cl)[281] & 0xfffffeff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 281: got %X, bad mask %X\n", ((const uint32_t *) cl)[281], ((const uint32_t *) cl)[281] & 0xfffffeff);
   if (((const uint32_t *) cl)[285] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 285: got %X, bad mask %X\n", ((const uint32_t *) cl)[285], ((const uint32_t *) cl)[285] & 0xffffffff);
   if (((const uint32_t *) cl)[288] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 288: got %X, bad mask %X\n", ((const uint32_t *) cl)[288], ((const uint32_t *) cl)[288] & 0xffffffff);
   if (((const uint32_t *) cl)[289] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 289: got %X, bad mask %X\n", ((const uint32_t *) cl)[289], ((const uint32_t *) cl)[289] & 0xffffffff);
   if (((const uint32_t *) cl)[290] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 290: got %X, bad mask %X\n", ((const uint32_t *) cl)[290], ((const uint32_t *) cl)[290] & 0xffffffff);
   if (((const uint32_t *) cl)[291] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 291: got %X, bad mask %X\n", ((const uint32_t *) cl)[291], ((const uint32_t *) cl)[291] & 0xffffffff);
   if (((const uint32_t *) cl)[292] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 292: got %X, bad mask %X\n", ((const uint32_t *) cl)[292], ((const uint32_t *) cl)[292] & 0xffffffff);
   if (((const uint32_t *) cl)[293] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 293: got %X, bad mask %X\n", ((const uint32_t *) cl)[293], ((const uint32_t *) cl)[293] & 0xffffffff);
   if (((const uint32_t *) cl)[294] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 294: got %X, bad mask %X\n", ((const uint32_t *) cl)[294], ((const uint32_t *) cl)[294] & 0xffffffff);
   if (((const uint32_t *) cl)[295] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 295: got %X, bad mask %X\n", ((const uint32_t *) cl)[295], ((const uint32_t *) cl)[295] & 0xffffffff);
   if (((const uint32_t *) cl)[297] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 297: got %X, bad mask %X\n", ((const uint32_t *) cl)[297], ((const uint32_t *) cl)[297] & 0xffffffff);
   if (((const uint32_t *) cl)[299] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 299: got %X, bad mask %X\n", ((const uint32_t *) cl)[299], ((const uint32_t *) cl)[299] & 0xffffffff);
   if (((const uint32_t *) cl)[300] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 300: got %X, bad mask %X\n", ((const uint32_t *) cl)[300], ((const uint32_t *) cl)[300] & 0xffffffff);
   if (((const uint32_t *) cl)[301] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 301: got %X, bad mask %X\n", ((const uint32_t *) cl)[301], ((const uint32_t *) cl)[301] & 0xffffffff);
   if (((const uint32_t *) cl)[302] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 302: got %X, bad mask %X\n", ((const uint32_t *) cl)[302], ((const uint32_t *) cl)[302] & 0xffffffff);
   if (((const uint32_t *) cl)[303] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 303: got %X, bad mask %X\n", ((const uint32_t *) cl)[303], ((const uint32_t *) cl)[303] & 0xffffffff);
   if (((const uint32_t *) cl)[305] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 305: got %X, bad mask %X\n", ((const uint32_t *) cl)[305], ((const uint32_t *) cl)[305] & 0xffffffff);
   if (((const uint32_t *) cl)[307] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 307: got %X, bad mask %X\n", ((const uint32_t *) cl)[307], ((const uint32_t *) cl)[307] & 0xffffffff);
   if (((const uint32_t *) cl)[308] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 308: got %X, bad mask %X\n", ((const uint32_t *) cl)[308], ((const uint32_t *) cl)[308] & 0xffffffff);
   if (((const uint32_t *) cl)[309] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 309: got %X, bad mask %X\n", ((const uint32_t *) cl)[309], ((const uint32_t *) cl)[309] & 0xffffffff);
   if (((const uint32_t *) cl)[310] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 310: got %X, bad mask %X\n", ((const uint32_t *) cl)[310], ((const uint32_t *) cl)[310] & 0xffffffff);
   if (((const uint32_t *) cl)[311] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 311: got %X, bad mask %X\n", ((const uint32_t *) cl)[311], ((const uint32_t *) cl)[311] & 0xffffffff);
   if (((const uint32_t *) cl)[312] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 312: got %X, bad mask %X\n", ((const uint32_t *) cl)[312], ((const uint32_t *) cl)[312] & 0xffffffff);
   if (((const uint32_t *) cl)[313] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 313: got %X, bad mask %X\n", ((const uint32_t *) cl)[313], ((const uint32_t *) cl)[313] & 0xffffffff);
   if (((const uint32_t *) cl)[314] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 314: got %X, bad mask %X\n", ((const uint32_t *) cl)[314], ((const uint32_t *) cl)[314] & 0xffffffff);
   if (((const uint32_t *) cl)[315] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 315: got %X, bad mask %X\n", ((const uint32_t *) cl)[315], ((const uint32_t *) cl)[315] & 0xffffffff);
   if (((const uint32_t *) cl)[316] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 316: got %X, bad mask %X\n", ((const uint32_t *) cl)[316], ((const uint32_t *) cl)[316] & 0xffffffff);
   if (((const uint32_t *) cl)[317] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 317: got %X, bad mask %X\n", ((const uint32_t *) cl)[317], ((const uint32_t *) cl)[317] & 0xffffffff);
   if (((const uint32_t *) cl)[318] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 318: got %X, bad mask %X\n", ((const uint32_t *) cl)[318], ((const uint32_t *) cl)[318] & 0xffffffff);
   if (((const uint32_t *) cl)[319] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 319: got %X, bad mask %X\n", ((const uint32_t *) cl)[319], ((const uint32_t *) cl)[319] & 0xffffffff);
   if (((const uint32_t *) cl)[320] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 320: got %X, bad mask %X\n", ((const uint32_t *) cl)[320], ((const uint32_t *) cl)[320] & 0xffffffff);
   if (((const uint32_t *) cl)[321] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 321: got %X, bad mask %X\n", ((const uint32_t *) cl)[321], ((const uint32_t *) cl)[321] & 0xffffffff);
   if (((const uint32_t *) cl)[322] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 322: got %X, bad mask %X\n", ((const uint32_t *) cl)[322], ((const uint32_t *) cl)[322] & 0xffffffff);
   if (((const uint32_t *) cl)[323] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 323: got %X, bad mask %X\n", ((const uint32_t *) cl)[323], ((const uint32_t *) cl)[323] & 0xffffffff);
   if (((const uint32_t *) cl)[324] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 324: got %X, bad mask %X\n", ((const uint32_t *) cl)[324], ((const uint32_t *) cl)[324] & 0xffffffff);
   if (((const uint32_t *) cl)[325] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 325: got %X, bad mask %X\n", ((const uint32_t *) cl)[325], ((const uint32_t *) cl)[325] & 0xffffffff);
   if (((const uint32_t *) cl)[326] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 326: got %X, bad mask %X\n", ((const uint32_t *) cl)[326], ((const uint32_t *) cl)[326] & 0xffffffff);
   if (((const uint32_t *) cl)[327] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 327: got %X, bad mask %X\n", ((const uint32_t *) cl)[327], ((const uint32_t *) cl)[327] & 0xffffffff);
   if (((const uint32_t *) cl)[328] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 328: got %X, bad mask %X\n", ((const uint32_t *) cl)[328], ((const uint32_t *) cl)[328] & 0xffffffff);
   if (((const uint32_t *) cl)[329] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 329: got %X, bad mask %X\n", ((const uint32_t *) cl)[329], ((const uint32_t *) cl)[329] & 0xffffffff);
   if (((const uint32_t *) cl)[330] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 330: got %X, bad mask %X\n", ((const uint32_t *) cl)[330], ((const uint32_t *) cl)[330] & 0xffffffff);
   if (((const uint32_t *) cl)[331] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 331: got %X, bad mask %X\n", ((const uint32_t *) cl)[331], ((const uint32_t *) cl)[331] & 0xffffffff);
   if (((const uint32_t *) cl)[332] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 332: got %X, bad mask %X\n", ((const uint32_t *) cl)[332], ((const uint32_t *) cl)[332] & 0xffffffff);
   if (((const uint32_t *) cl)[333] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 333: got %X, bad mask %X\n", ((const uint32_t *) cl)[333], ((const uint32_t *) cl)[333] & 0xffffffff);
   if (((const uint32_t *) cl)[334] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 334: got %X, bad mask %X\n", ((const uint32_t *) cl)[334], ((const uint32_t *) cl)[334] & 0xffffffff);
   if (((const uint32_t *) cl)[335] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 335: got %X, bad mask %X\n", ((const uint32_t *) cl)[335], ((const uint32_t *) cl)[335] & 0xffffffff);
   if (((const uint32_t *) cl)[336] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 336: got %X, bad mask %X\n", ((const uint32_t *) cl)[336], ((const uint32_t *) cl)[336] & 0xffffffff);
   if (((const uint32_t *) cl)[337] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 337: got %X, bad mask %X\n", ((const uint32_t *) cl)[337], ((const uint32_t *) cl)[337] & 0xffffffff);
   if (((const uint32_t *) cl)[338] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 338: got %X, bad mask %X\n", ((const uint32_t *) cl)[338], ((const uint32_t *) cl)[338] & 0xffffffff);
   if (((const uint32_t *) cl)[339] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 339: got %X, bad mask %X\n", ((const uint32_t *) cl)[339], ((const uint32_t *) cl)[339] & 0xffffffff);
   if (((const uint32_t *) cl)[348] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 348: got %X, bad mask %X\n", ((const uint32_t *) cl)[348], ((const uint32_t *) cl)[348] & 0xffffffff);
   if (((const uint32_t *) cl)[349] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 349: got %X, bad mask %X\n", ((const uint32_t *) cl)[349], ((const uint32_t *) cl)[349] & 0xffffffff);
   if (((const uint32_t *) cl)[350] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 350: got %X, bad mask %X\n", ((const uint32_t *) cl)[350], ((const uint32_t *) cl)[350] & 0xffffffff);
   if (((const uint32_t *) cl)[351] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 351: got %X, bad mask %X\n", ((const uint32_t *) cl)[351], ((const uint32_t *) cl)[351] & 0xffffffff);
   if (((const uint32_t *) cl)[353] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 353: got %X, bad mask %X\n", ((const uint32_t *) cl)[353], ((const uint32_t *) cl)[353] & 0xffffffff);
   if (((const uint32_t *) cl)[354] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 354: got %X, bad mask %X\n", ((const uint32_t *) cl)[354], ((const uint32_t *) cl)[354] & 0xffffffff);
   if (((const uint32_t *) cl)[355] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 355: got %X, bad mask %X\n", ((const uint32_t *) cl)[355], ((const uint32_t *) cl)[355] & 0xffffffff);
   if (((const uint32_t *) cl)[356] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 356: got %X, bad mask %X\n", ((const uint32_t *) cl)[356], ((const uint32_t *) cl)[356] & 0xffffffff);
   if (((const uint32_t *) cl)[357] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 357: got %X, bad mask %X\n", ((const uint32_t *) cl)[357], ((const uint32_t *) cl)[357] & 0xffffffff);
   if (((const uint32_t *) cl)[358] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 358: got %X, bad mask %X\n", ((const uint32_t *) cl)[358], ((const uint32_t *) cl)[358] & 0xffffffff);
   if (((const uint32_t *) cl)[359] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 359: got %X, bad mask %X\n", ((const uint32_t *) cl)[359], ((const uint32_t *) cl)[359] & 0xffffffff);
   if (((const uint32_t *) cl)[361] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 361: got %X, bad mask %X\n", ((const uint32_t *) cl)[361], ((const uint32_t *) cl)[361] & 0xffffffff);
   if (((const uint32_t *) cl)[363] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 363: got %X, bad mask %X\n", ((const uint32_t *) cl)[363], ((const uint32_t *) cl)[363] & 0xffffffff);
   if (((const uint32_t *) cl)[364] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 364: got %X, bad mask %X\n", ((const uint32_t *) cl)[364], ((const uint32_t *) cl)[364] & 0xffffffff);
   if (((const uint32_t *) cl)[367] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 367: got %X, bad mask %X\n", ((const uint32_t *) cl)[367], ((const uint32_t *) cl)[367] & 0xffffffff);
   if (((const uint32_t *) cl)[368] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 368: got %X, bad mask %X\n", ((const uint32_t *) cl)[368], ((const uint32_t *) cl)[368] & 0xffffffff);
   if (((const uint32_t *) cl)[369] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 369: got %X, bad mask %X\n", ((const uint32_t *) cl)[369], ((const uint32_t *) cl)[369] & 0xffffffff);
   if (((const uint32_t *) cl)[372] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 372: got %X, bad mask %X\n", ((const uint32_t *) cl)[372], ((const uint32_t *) cl)[372] & 0xffffffff);
   if (((const uint32_t *) cl)[373] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 373: got %X, bad mask %X\n", ((const uint32_t *) cl)[373], ((const uint32_t *) cl)[373] & 0xffffffff);
   if (((const uint32_t *) cl)[374] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 374: got %X, bad mask %X\n", ((const uint32_t *) cl)[374], ((const uint32_t *) cl)[374] & 0xffffffff);
   if (((const uint32_t *) cl)[375] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 375: got %X, bad mask %X\n", ((const uint32_t *) cl)[375], ((const uint32_t *) cl)[375] & 0xffffffff);
   if (((const uint32_t *) cl)[376] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 376: got %X, bad mask %X\n", ((const uint32_t *) cl)[376], ((const uint32_t *) cl)[376] & 0xffffffff);
   if (((const uint32_t *) cl)[377] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 377: got %X, bad mask %X\n", ((const uint32_t *) cl)[377], ((const uint32_t *) cl)[377] & 0xffffffff);
   if (((const uint32_t *) cl)[378] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 378: got %X, bad mask %X\n", ((const uint32_t *) cl)[378], ((const uint32_t *) cl)[378] & 0xffffffff);
   if (((const uint32_t *) cl)[379] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 379: got %X, bad mask %X\n", ((const uint32_t *) cl)[379], ((const uint32_t *) cl)[379] & 0xffffffff);
   if (((const uint32_t *) cl)[380] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 380: got %X, bad mask %X\n", ((const uint32_t *) cl)[380], ((const uint32_t *) cl)[380] & 0xffffffff);
   if (((const uint32_t *) cl)[381] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 381: got %X, bad mask %X\n", ((const uint32_t *) cl)[381], ((const uint32_t *) cl)[381] & 0xffffffff);
   if (((const uint32_t *) cl)[385] & 0xffffffe0) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 385: got %X, bad mask %X\n", ((const uint32_t *) cl)[385], ((const uint32_t *) cl)[385] & 0xffffffe0);
   if (((const uint32_t *) cl)[386] & 0xffffffe0) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 386: got %X, bad mask %X\n", ((const uint32_t *) cl)[386], ((const uint32_t *) cl)[386] & 0xffffffe0);
   if (((const uint32_t *) cl)[387] & 0xffffffe0) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 387: got %X, bad mask %X\n", ((const uint32_t *) cl)[387], ((const uint32_t *) cl)[387] & 0xffffffe0);
   if (((const uint32_t *) cl)[388] & 0xffffffe0) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 388: got %X, bad mask %X\n", ((const uint32_t *) cl)[388], ((const uint32_t *) cl)[388] & 0xffffffe0);
   if (((const uint32_t *) cl)[389] & 0xffffffe0) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 389: got %X, bad mask %X\n", ((const uint32_t *) cl)[389], ((const uint32_t *) cl)[389] & 0xffffffe0);
   if (((const uint32_t *) cl)[390] & 0xffffffe0) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 390: got %X, bad mask %X\n", ((const uint32_t *) cl)[390], ((const uint32_t *) cl)[390] & 0xffffffe0);
   if (((const uint32_t *) cl)[391] & 0xffffffe0) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 391: got %X, bad mask %X\n", ((const uint32_t *) cl)[391], ((const uint32_t *) cl)[391] & 0xffffffe0);
   if (((const uint32_t *) cl)[392] & 0xffffffe0) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 392: got %X, bad mask %X\n", ((const uint32_t *) cl)[392], ((const uint32_t *) cl)[392] & 0xffffffe0);
   if (((const uint32_t *) cl)[393] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 393: got %X, bad mask %X\n", ((const uint32_t *) cl)[393], ((const uint32_t *) cl)[393] & 0xffffffff);
   if (((const uint32_t *) cl)[394] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 394: got %X, bad mask %X\n", ((const uint32_t *) cl)[394], ((const uint32_t *) cl)[394] & 0xffffffff);
   if (((const uint32_t *) cl)[395] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 395: got %X, bad mask %X\n", ((const uint32_t *) cl)[395], ((const uint32_t *) cl)[395] & 0xffffffff);
   if (((const uint32_t *) cl)[396] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 396: got %X, bad mask %X\n", ((const uint32_t *) cl)[396], ((const uint32_t *) cl)[396] & 0xffffffff);
   if (((const uint32_t *) cl)[397] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 397: got %X, bad mask %X\n", ((const uint32_t *) cl)[397], ((const uint32_t *) cl)[397] & 0xffffffff);
   if (((const uint32_t *) cl)[398] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 398: got %X, bad mask %X\n", ((const uint32_t *) cl)[398], ((const uint32_t *) cl)[398] & 0xffffffff);
   if (((const uint32_t *) cl)[399] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 399: got %X, bad mask %X\n", ((const uint32_t *) cl)[399], ((const uint32_t *) cl)[399] & 0xffffffff);
   if (((const uint32_t *) cl)[400] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 400: got %X, bad mask %X\n", ((const uint32_t *) cl)[400], ((const uint32_t *) cl)[400] & 0xffffffff);
   if (((const uint32_t *) cl)[405] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 405: got %X, bad mask %X\n", ((const uint32_t *) cl)[405], ((const uint32_t *) cl)[405] & 0xffffffff);
   if (((const uint32_t *) cl)[406] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 406: got %X, bad mask %X\n", ((const uint32_t *) cl)[406], ((const uint32_t *) cl)[406] & 0xffffffff);
   if (((const uint32_t *) cl)[407] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 407: got %X, bad mask %X\n", ((const uint32_t *) cl)[407], ((const uint32_t *) cl)[407] & 0xffffffff);
   if (((const uint32_t *) cl)[409] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 409: got %X, bad mask %X\n", ((const uint32_t *) cl)[409], ((const uint32_t *) cl)[409] & 0xffffffff);
   if (((const uint32_t *) cl)[411] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 411: got %X, bad mask %X\n", ((const uint32_t *) cl)[411], ((const uint32_t *) cl)[411] & 0xffffffff);
   if (((const uint32_t *) cl)[412] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 412: got %X, bad mask %X\n", ((const uint32_t *) cl)[412], ((const uint32_t *) cl)[412] & 0xffffffff);
   if (((const uint32_t *) cl)[413] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 413: got %X, bad mask %X\n", ((const uint32_t *) cl)[413], ((const uint32_t *) cl)[413] & 0xffffffff);
   if (((const uint32_t *) cl)[414] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 414: got %X, bad mask %X\n", ((const uint32_t *) cl)[414], ((const uint32_t *) cl)[414] & 0xffffffff);
   if (((const uint32_t *) cl)[415] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 415: got %X, bad mask %X\n", ((const uint32_t *) cl)[415], ((const uint32_t *) cl)[415] & 0xffffffff);
   if (((const uint32_t *) cl)[416] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 416: got %X, bad mask %X\n", ((const uint32_t *) cl)[416], ((const uint32_t *) cl)[416] & 0xffffffff);
   if (((const uint32_t *) cl)[417] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 417: got %X, bad mask %X\n", ((const uint32_t *) cl)[417], ((const uint32_t *) cl)[417] & 0xffffffff);
   if (((const uint32_t *) cl)[418] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 418: got %X, bad mask %X\n", ((const uint32_t *) cl)[418], ((const uint32_t *) cl)[418] & 0xffffffff);
   if (((const uint32_t *) cl)[419] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 419: got %X, bad mask %X\n", ((const uint32_t *) cl)[419], ((const uint32_t *) cl)[419] & 0xffffffff);
   if (((const uint32_t *) cl)[420] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 420: got %X, bad mask %X\n", ((const uint32_t *) cl)[420], ((const uint32_t *) cl)[420] & 0xffffffff);
   if (((const uint32_t *) cl)[421] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 421: got %X, bad mask %X\n", ((const uint32_t *) cl)[421], ((const uint32_t *) cl)[421] & 0xffffffff);
   if (((const uint32_t *) cl)[422] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 422: got %X, bad mask %X\n", ((const uint32_t *) cl)[422], ((const uint32_t *) cl)[422] & 0xffffffff);
   if (((const uint32_t *) cl)[423] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 423: got %X, bad mask %X\n", ((const uint32_t *) cl)[423], ((const uint32_t *) cl)[423] & 0xffffffff);
   if (((const uint32_t *) cl)[424] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 424: got %X, bad mask %X\n", ((const uint32_t *) cl)[424], ((const uint32_t *) cl)[424] & 0xffffffff);
   if (((const uint32_t *) cl)[425] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 425: got %X, bad mask %X\n", ((const uint32_t *) cl)[425], ((const uint32_t *) cl)[425] & 0xffffffff);
   if (((const uint32_t *) cl)[426] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 426: got %X, bad mask %X\n", ((const uint32_t *) cl)[426], ((const uint32_t *) cl)[426] & 0xffffffff);
   if (((const uint32_t *) cl)[427] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 427: got %X, bad mask %X\n", ((const uint32_t *) cl)[427], ((const uint32_t *) cl)[427] & 0xffffffff);
   if (((const uint32_t *) cl)[428] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 428: got %X, bad mask %X\n", ((const uint32_t *) cl)[428], ((const uint32_t *) cl)[428] & 0xffffffff);
   if (((const uint32_t *) cl)[429] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 429: got %X, bad mask %X\n", ((const uint32_t *) cl)[429], ((const uint32_t *) cl)[429] & 0xffffffff);
   if (((const uint32_t *) cl)[430] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 430: got %X, bad mask %X\n", ((const uint32_t *) cl)[430], ((const uint32_t *) cl)[430] & 0xffffffff);
   if (((const uint32_t *) cl)[431] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 431: got %X, bad mask %X\n", ((const uint32_t *) cl)[431], ((const uint32_t *) cl)[431] & 0xffffffff);
   if (((const uint32_t *) cl)[432] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 432: got %X, bad mask %X\n", ((const uint32_t *) cl)[432], ((const uint32_t *) cl)[432] & 0xffffffff);
   if (((const uint32_t *) cl)[433] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 433: got %X, bad mask %X\n", ((const uint32_t *) cl)[433], ((const uint32_t *) cl)[433] & 0xffffffff);
   if (((const uint32_t *) cl)[434] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 434: got %X, bad mask %X\n", ((const uint32_t *) cl)[434], ((const uint32_t *) cl)[434] & 0xffffffff);
   if (((const uint32_t *) cl)[435] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 435: got %X, bad mask %X\n", ((const uint32_t *) cl)[435], ((const uint32_t *) cl)[435] & 0xffffffff);
   if (((const uint32_t *) cl)[436] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 436: got %X, bad mask %X\n", ((const uint32_t *) cl)[436], ((const uint32_t *) cl)[436] & 0xffffffff);
   if (((const uint32_t *) cl)[437] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 437: got %X, bad mask %X\n", ((const uint32_t *) cl)[437], ((const uint32_t *) cl)[437] & 0xffffffff);
   if (((const uint32_t *) cl)[438] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 438: got %X, bad mask %X\n", ((const uint32_t *) cl)[438], ((const uint32_t *) cl)[438] & 0xffffffff);
   if (((const uint32_t *) cl)[439] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 439: got %X, bad mask %X\n", ((const uint32_t *) cl)[439], ((const uint32_t *) cl)[439] & 0xffffffff);
   if (((const uint32_t *) cl)[440] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 440: got %X, bad mask %X\n", ((const uint32_t *) cl)[440], ((const uint32_t *) cl)[440] & 0xffffffff);
   if (((const uint32_t *) cl)[441] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 441: got %X, bad mask %X\n", ((const uint32_t *) cl)[441], ((const uint32_t *) cl)[441] & 0xffffffff);
   if (((const uint32_t *) cl)[442] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 442: got %X, bad mask %X\n", ((const uint32_t *) cl)[442], ((const uint32_t *) cl)[442] & 0xffffffff);
   if (((const uint32_t *) cl)[443] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 443: got %X, bad mask %X\n", ((const uint32_t *) cl)[443], ((const uint32_t *) cl)[443] & 0xffffffff);
   if (((const uint32_t *) cl)[444] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 444: got %X, bad mask %X\n", ((const uint32_t *) cl)[444], ((const uint32_t *) cl)[444] & 0xffffffff);
   if (((const uint32_t *) cl)[445] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 445: got %X, bad mask %X\n", ((const uint32_t *) cl)[445], ((const uint32_t *) cl)[445] & 0xffffffff);
   if (((const uint32_t *) cl)[446] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 446: got %X, bad mask %X\n", ((const uint32_t *) cl)[446], ((const uint32_t *) cl)[446] & 0xffffffff);
   if (((const uint32_t *) cl)[447] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 447: got %X, bad mask %X\n", ((const uint32_t *) cl)[447], ((const uint32_t *) cl)[447] & 0xffffffff);
   if (((const uint32_t *) cl)[448] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 448: got %X, bad mask %X\n", ((const uint32_t *) cl)[448], ((const uint32_t *) cl)[448] & 0xffffffff);
   if (((const uint32_t *) cl)[449] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 449: got %X, bad mask %X\n", ((const uint32_t *) cl)[449], ((const uint32_t *) cl)[449] & 0xffffffff);
   if (((const uint32_t *) cl)[450] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 450: got %X, bad mask %X\n", ((const uint32_t *) cl)[450], ((const uint32_t *) cl)[450] & 0xffffffff);
   if (((const uint32_t *) cl)[451] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 451: got %X, bad mask %X\n", ((const uint32_t *) cl)[451], ((const uint32_t *) cl)[451] & 0xffffffff);
   if (((const uint32_t *) cl)[452] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 452: got %X, bad mask %X\n", ((const uint32_t *) cl)[452], ((const uint32_t *) cl)[452] & 0xffffffff);
   if (((const uint32_t *) cl)[453] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 453: got %X, bad mask %X\n", ((const uint32_t *) cl)[453], ((const uint32_t *) cl)[453] & 0xffffffff);
   if (((const uint32_t *) cl)[454] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 454: got %X, bad mask %X\n", ((const uint32_t *) cl)[454], ((const uint32_t *) cl)[454] & 0xffffffff);
   if (((const uint32_t *) cl)[455] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 455: got %X, bad mask %X\n", ((const uint32_t *) cl)[455], ((const uint32_t *) cl)[455] & 0xffffffff);
   if (((const uint32_t *) cl)[456] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 456: got %X, bad mask %X\n", ((const uint32_t *) cl)[456], ((const uint32_t *) cl)[456] & 0xffffffff);
   if (((const uint32_t *) cl)[457] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 457: got %X, bad mask %X\n", ((const uint32_t *) cl)[457], ((const uint32_t *) cl)[457] & 0xffffffff);
   if (((const uint32_t *) cl)[458] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 458: got %X, bad mask %X\n", ((const uint32_t *) cl)[458], ((const uint32_t *) cl)[458] & 0xffffffff);
   if (((const uint32_t *) cl)[459] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 459: got %X, bad mask %X\n", ((const uint32_t *) cl)[459], ((const uint32_t *) cl)[459] & 0xffffffff);
   if (((const uint32_t *) cl)[460] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 460: got %X, bad mask %X\n", ((const uint32_t *) cl)[460], ((const uint32_t *) cl)[460] & 0xffffffff);
   if (((const uint32_t *) cl)[461] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 461: got %X, bad mask %X\n", ((const uint32_t *) cl)[461], ((const uint32_t *) cl)[461] & 0xffffffff);
   if (((const uint32_t *) cl)[462] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 462: got %X, bad mask %X\n", ((const uint32_t *) cl)[462], ((const uint32_t *) cl)[462] & 0xffffffff);
   if (((const uint32_t *) cl)[463] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 463: got %X, bad mask %X\n", ((const uint32_t *) cl)[463], ((const uint32_t *) cl)[463] & 0xffffffff);
   if (((const uint32_t *) cl)[464] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 464: got %X, bad mask %X\n", ((const uint32_t *) cl)[464], ((const uint32_t *) cl)[464] & 0xffffffff);
   if (((const uint32_t *) cl)[465] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 465: got %X, bad mask %X\n", ((const uint32_t *) cl)[465], ((const uint32_t *) cl)[465] & 0xffffffff);
   if (((const uint32_t *) cl)[466] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 466: got %X, bad mask %X\n", ((const uint32_t *) cl)[466], ((const uint32_t *) cl)[466] & 0xffffffff);
   if (((const uint32_t *) cl)[467] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 467: got %X, bad mask %X\n", ((const uint32_t *) cl)[467], ((const uint32_t *) cl)[467] & 0xffffffff);
   if (((const uint32_t *) cl)[468] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 468: got %X, bad mask %X\n", ((const uint32_t *) cl)[468], ((const uint32_t *) cl)[468] & 0xffffffff);
   if (((const uint32_t *) cl)[469] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 469: got %X, bad mask %X\n", ((const uint32_t *) cl)[469], ((const uint32_t *) cl)[469] & 0xffffffff);
   if (((const uint32_t *) cl)[470] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 470: got %X, bad mask %X\n", ((const uint32_t *) cl)[470], ((const uint32_t *) cl)[470] & 0xffffffff);
   if (((const uint32_t *) cl)[471] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 471: got %X, bad mask %X\n", ((const uint32_t *) cl)[471], ((const uint32_t *) cl)[471] & 0xffffffff);
   if (((const uint32_t *) cl)[472] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 472: got %X, bad mask %X\n", ((const uint32_t *) cl)[472], ((const uint32_t *) cl)[472] & 0xffffffff);
   if (((const uint32_t *) cl)[473] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 473: got %X, bad mask %X\n", ((const uint32_t *) cl)[473], ((const uint32_t *) cl)[473] & 0xffffffff);
   if (((const uint32_t *) cl)[474] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 474: got %X, bad mask %X\n", ((const uint32_t *) cl)[474], ((const uint32_t *) cl)[474] & 0xffffffff);
   if (((const uint32_t *) cl)[475] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 475: got %X, bad mask %X\n", ((const uint32_t *) cl)[475], ((const uint32_t *) cl)[475] & 0xffffffff);
   if (((const uint32_t *) cl)[476] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 476: got %X, bad mask %X\n", ((const uint32_t *) cl)[476], ((const uint32_t *) cl)[476] & 0xffffffff);
   if (((const uint32_t *) cl)[477] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 477: got %X, bad mask %X\n", ((const uint32_t *) cl)[477], ((const uint32_t *) cl)[477] & 0xffffffff);
   if (((const uint32_t *) cl)[478] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 478: got %X, bad mask %X\n", ((const uint32_t *) cl)[478], ((const uint32_t *) cl)[478] & 0xffffffff);
   if (((const uint32_t *) cl)[479] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Graphics unpacked at word 479: got %X, bad mask %X\n", ((const uint32_t *) cl)[479], ((const uint32_t *) cl)[479] & 0xffffffff);
   values->deflake_1 = __gen_unpack_uint(cl, 704, 767);
   values->deflake_2 = __gen_unpack_uint(cl, 768, 831);
   values->unk_54 = __gen_unpack_uint(cl, 1216, 1247);
   values->unk_55 = __gen_unpack_uint(cl, 1248, 1279);
   values->unk_56 = __gen_unpack_uint(cl, 1280, 1311);
   values->deflake_3 = __gen_unpack_uint(cl, 3008, 3071);
   values->unk_112 = __gen_unpack_uint(cl, 3072, 3103);
   values->unk_114 = __gen_unpack_uint(cl, 3136, 3167);
   values->memoryless_render_targets_used = __gen_unpack_uint(cl, 3200, 3200);
   values->opengl_depth_clipping = __gen_unpack_uint(cl, 3224, 3224);
   values->unk_118 = __gen_unpack_uint(cl, 3264, 3295);
   values->unk_119 = __gen_unpack_uint(cl, 3296, 3327);
   values->unk_120 = __gen_unpack_uint(cl, 3328, 3359);
   values->clear_pipeline_bind = __gen_unpack_uint(cl, 4672, 4703);
   values->clear_pipeline_unk = __gen_unpack_uint(cl, 4736, 4739);
   values->clear_pipeline = __gen_unpack_uint(cl, 4740, 4767) << 4;
   values->store_pipeline_bind = __gen_unpack_uint(cl, 4928, 4959);
   values->store_pipeline_unk = __gen_unpack_uint(cl, 4992, 4995);
   values->store_pipeline = __gen_unpack_uint(cl, 4996, 5023) << 4;
   values->scissor_array = __gen_unpack_uint(cl, 5056, 5119);
   values->depth_bias_array = __gen_unpack_uint(cl, 5120, 5183);
   values->zls_control.unknown_0 = __gen_unpack_uint(cl, 5248, 5248);
   values->zls_control.unknown_1 = __gen_unpack_uint(cl, 5249, 5249);
   values->zls_control.z_compress_1 = __gen_unpack_uint(cl, 5250, 5250);
   values->zls_control.unknown_3 = __gen_unpack_uint(cl, 5251, 5251);
   values->zls_control.s_compress_1 = __gen_unpack_uint(cl, 5252, 5252);
   values->zls_control.unknown_5 = __gen_unpack_uint(cl, 5253, 5253);
   values->zls_control.z_compress_2 = __gen_unpack_uint(cl, 5254, 5254);
   values->zls_control.unknown_7 = __gen_unpack_uint(cl, 5255, 5255);
   values->zls_control.s_compress_2 = __gen_unpack_uint(cl, 5256, 5256);
   values->zls_control.s_load_enable = __gen_unpack_uint(cl, 5262, 5262);
   values->zls_control.z_load_enable = __gen_unpack_uint(cl, 5263, 5263);
   values->zls_control.s_store_enable = __gen_unpack_uint(cl, 5266, 5266);
   values->zls_control.z_store_enable = __gen_unpack_uint(cl, 5267, 5267);
   values->zls_control.z_format = (enum agx_zls_format) __gen_unpack_uint(cl, 5273, 5274);
   values->zls_control.z_resolve = __gen_unpack_uint(cl, 5304, 5304);
   values->zls_control.s_resolve = __gen_unpack_uint(cl, 5306, 5306);
   values->depth_width = __gen_unpack_uint(cl, 5440, 5454) + 1;
   values->depth_height = __gen_unpack_uint(cl, 5455, 5469) + 1;
   values->depth_buffer_1 = __gen_unpack_uint(cl, 5511, 5543) << 7;
   values->depth_unknown_1 = __gen_unpack_uint(cl, 5632, 5663);
   values->depth_acceleration_buffer_1 = __gen_unpack_uint(cl, 5703, 5735) << 7;
   values->depth_buffer_2 = __gen_unpack_uint(cl, 5831, 5863) << 7;
   values->depth_acceleration_buffer_2 = __gen_unpack_uint(cl, 6023, 6055) << 7;
   values->depth_unknown_2 = __gen_unpack_uint(cl, 5952, 5983);
   values->stencil_buffer_1 = __gen_unpack_uint(cl, 6151, 6183) << 7;
   values->stencil_unknown_1 = __gen_unpack_uint(cl, 6272, 6303);
   values->stencil_acceleration_buffer_1 = __gen_unpack_uint(cl, 6343, 6375) << 7;
   values->stencil_buffer_2 = __gen_unpack_uint(cl, 6471, 6503) << 7;
   values->stencil_unknown_2 = __gen_unpack_uint(cl, 6592, 6623);
   values->stencil_acceleration_buffer_2 = __gen_unpack_uint(cl, 6663, 6695) << 7;
   values->unk_212 = __gen_unpack_uint(cl, 6784, 6815);
   values->unk_214 = __gen_unpack_uint(cl, 6848, 6863);
   values->z16_unorm_attachment_1 = __gen_unpack_uint(cl, 6866, 6866);
   values->width_1 = __gen_unpack_uint(cl, 6912, 6943);
   values->height_1 = __gen_unpack_uint(cl, 6944, 6975);
   values->pointer = __gen_unpack_uint(cl, 6976, 7039);
   values->spilling_unk_1 = __gen_unpack_uint(cl, 7232, 7263);
   values->spill_buffer_histogram.bin_0 = __gen_unpack_uint(cl, 7616, 7619);
   values->spill_buffer_histogram.bin_1 = __gen_unpack_uint(cl, 7648, 7651);
   values->spill_buffer_histogram.bin_2 = __gen_unpack_uint(cl, 7680, 7683);
   values->spill_buffer_histogram.bin_3 = __gen_unpack_uint(cl, 7712, 7715);
   values->spill_buffer_histogram.bin_4 = __gen_unpack_uint(cl, 7744, 7747);
   values->spill_buffer_histogram.bin_5 = __gen_unpack_uint(cl, 7776, 7779);
   values->spill_buffer_histogram.bin_6 = __gen_unpack_uint(cl, 7808, 7811);
   values->spill_buffer_histogram.bin_7 = __gen_unpack_uint(cl, 7840, 7843);
   values->spill_buffer_histogram.bin_8 = __gen_unpack_uint(cl, 7872, 7875);
   values->spill_buffer_histogram.bin_9 = __gen_unpack_uint(cl, 7904, 7907);
   values->spill_buffer_histogram.bin_10 = __gen_unpack_uint(cl, 7936, 7939);
   values->spill_buffer_histogram.bin_11 = __gen_unpack_uint(cl, 7968, 7971);
   values->spill_buffer_histogram.bin_12 = __gen_unpack_uint(cl, 8000, 8003);
   values->spill_buffer_histogram.bin_13 = __gen_unpack_uint(cl, 8032, 8035);
   values->spill_buffer_histogram.bin_14 = __gen_unpack_uint(cl, 8064, 8067);
   values->depth_clear_value = __gen_unpack_uint(cl, 8832, 8863);
   values->stencil_clear_value = __gen_unpack_uint(cl, 8864, 8871);
   values->unk_277 = __gen_unpack_uint(cl, 8872, 8879);
   values->set_when_reloading_z_or_s_1 = __gen_unpack_uint(cl, 8936, 8936);
   values->set_when_frag_shader_spills = __gen_unpack_uint(cl, 8952, 8952);
   values->set_when_reloading_z_or_s_2 = __gen_unpack_uint(cl, 8984, 8984);
   values->z16_unorm_attachment_2 = __gen_unpack_uint(cl, 9000, 9000);
   values->unk_282 = __gen_unpack_uint(cl, 9024, 9055);
   values->unk_283 = __gen_unpack_uint(cl, 9056, 9087);
   values->unk_284 = __gen_unpack_uint(cl, 9088, 9119);
   values->visibility_result_buffer = __gen_unpack_uint(cl, 9152, 9215);
   values->partial_reload_pipeline_bind = __gen_unpack_uint(cl, 9472, 9503);
   values->partial_reload_pipeline_unk = __gen_unpack_uint(cl, 9536, 9539);
   values->partial_reload_pipeline = __gen_unpack_uint(cl, 9540, 9567) << 4;
   values->partial_store_pipeline_bind = __gen_unpack_uint(cl, 9728, 9759);
   values->partial_store_pipeline_unk = __gen_unpack_uint(cl, 9792, 9795);
   values->partial_store_pipeline = __gen_unpack_uint(cl, 9796, 9823) << 4;
   values->depth_buffer_3 = __gen_unpack_uint(cl, 10880, 10943);
   values->depth_acceleration_buffer_3 = __gen_unpack_uint(cl, 10944, 11007);
   values->stencil_buffer_3 = __gen_unpack_uint(cl, 11008, 11071);
   values->stencil_acceleration_buffer_3 = __gen_unpack_uint(cl, 11072, 11135);
   values->unk_352 = __gen_unpack_uint(cl, 11264, 11295);
   values->unk_360 = __gen_unpack_uint(cl, 11520, 11551);
   values->encoder_id = __gen_unpack_uint(cl, 11584, 11615);
   values->unk_365 = __gen_unpack_uint(cl, 11680, 11743);
   values->unknown_buffer = __gen_unpack_uint(cl, 11840, 11903);
   values->width_2 = __gen_unpack_uint(cl, 12224, 12255);
   values->height_2 = __gen_unpack_uint(cl, 12256, 12287);
   values->sample_count = __gen_unpack_uint(cl, 12288, 12319);
   values->sample_0_x = __gen_unpack_uint(cl, 12320, 12324);
   values->sample_0_y = __gen_unpack_uint(cl, 12352, 12356);
   values->sample_1_x = __gen_unpack_uint(cl, 12384, 12388);
   values->sample_1_y = __gen_unpack_uint(cl, 12416, 12420);
   values->sample_2_x = __gen_unpack_uint(cl, 12448, 12452);
   values->sample_2_y = __gen_unpack_uint(cl, 12480, 12484);
   values->sample_3_x = __gen_unpack_uint(cl, 12512, 12516);
   values->sample_3_y = __gen_unpack_uint(cl, 12544, 12548);
   values->unk_490 = __gen_unpack_uint(cl, 12832, 12863);
   values->tile_width = __gen_unpack_uint(cl, 12864, 12895);
   values->tile_height = __gen_unpack_uint(cl, 12896, 12927);
   values->framebuffer_layers = __gen_unpack_uint(cl, 12928, 12959);
   values->unk_560 = __gen_unpack_uint(cl, 13056, 13087);
   values->unk_700 = __gen_unpack_uint(cl, 13120, 13151);
}

static inline void
AGX_IOGPU_GRAPHICS_print(FILE *fp, const struct AGX_IOGPU_GRAPHICS * values, unsigned indent)
{
   fprintf(fp, "%*sDeflake 1: 0x%" PRIx64 "\n", indent, "", values->deflake_1);
   fprintf(fp, "%*sDeflake 2: 0x%" PRIx64 "\n", indent, "", values->deflake_2);
   fprintf(fp, "%*sUnk 54: 0x%" PRIx32 "\n", indent, "", values->unk_54);
   fprintf(fp, "%*sUnk 55: 0x%" PRIx32 "\n", indent, "", values->unk_55);
   fprintf(fp, "%*sUnk 56: 0x%" PRIx32 "\n", indent, "", values->unk_56);
   fprintf(fp, "%*sDeflake 3: 0x%" PRIx64 "\n", indent, "", values->deflake_3);
   fprintf(fp, "%*sUnk 112: 0x%" PRIx32 "\n", indent, "", values->unk_112);
   fprintf(fp, "%*sUnk 114: 0x%" PRIx32 "\n", indent, "", values->unk_114);
   fprintf(fp, "%*sMemoryless render targets used: %s\n", indent, "", values->memoryless_render_targets_used ? "true" : "false");
   fprintf(fp, "%*sOpenGL depth clipping: %s\n", indent, "", values->opengl_depth_clipping ? "true" : "false");
   fprintf(fp, "%*sUnk 118: 0x%" PRIx32 "\n", indent, "", values->unk_118);
   fprintf(fp, "%*sUnk 119: 0x%" PRIx32 "\n", indent, "", values->unk_119);
   fprintf(fp, "%*sUnk 120: 0x%" PRIx32 "\n", indent, "", values->unk_120);
   fprintf(fp, "%*sClear pipeline bind: 0x%" PRIx32 "\n", indent, "", values->clear_pipeline_bind);
   fprintf(fp, "%*sClear pipeline unk: 0x%" PRIx32 "\n", indent, "", values->clear_pipeline_unk);
   fprintf(fp, "%*sClear pipeline: 0x%" PRIx64 "\n", indent, "", values->clear_pipeline);
   fprintf(fp, "%*sStore pipeline bind: 0x%" PRIx32 "\n", indent, "", values->store_pipeline_bind);
   fprintf(fp, "%*sStore pipeline unk: 0x%" PRIx32 "\n", indent, "", values->store_pipeline_unk);
   fprintf(fp, "%*sStore pipeline: 0x%" PRIx64 "\n", indent, "", values->store_pipeline);
   fprintf(fp, "%*sScissor array: 0x%" PRIx64 "\n", indent, "", values->scissor_array);
   fprintf(fp, "%*sDepth bias array: 0x%" PRIx64 "\n", indent, "", values->depth_bias_array);
   fprintf(fp, "%*sZLS control:\n", indent, "");
   AGX_ZLS_CONTROL_print(fp, &values->zls_control, indent + 2);
   fprintf(fp, "%*sDepth width: %u\n", indent, "", values->depth_width);
   fprintf(fp, "%*sDepth height: %u\n", indent, "", values->depth_height);
   fprintf(fp, "%*sDepth buffer 1: 0x%" PRIx64 "\n", indent, "", values->depth_buffer_1);
   fprintf(fp, "%*sDepth unknown 1: 0x%" PRIx32 "\n", indent, "", values->depth_unknown_1);
   fprintf(fp, "%*sDepth acceleration buffer 1: 0x%" PRIx64 "\n", indent, "", values->depth_acceleration_buffer_1);
   fprintf(fp, "%*sDepth buffer 2: 0x%" PRIx64 "\n", indent, "", values->depth_buffer_2);
   fprintf(fp, "%*sDepth acceleration buffer 2: 0x%" PRIx64 "\n", indent, "", values->depth_acceleration_buffer_2);
   fprintf(fp, "%*sDepth unknown 2: 0x%" PRIx32 "\n", indent, "", values->depth_unknown_2);
   fprintf(fp, "%*sStencil buffer 1: 0x%" PRIx64 "\n", indent, "", values->stencil_buffer_1);
   fprintf(fp, "%*sStencil unknown 1: 0x%" PRIx32 "\n", indent, "", values->stencil_unknown_1);
   fprintf(fp, "%*sStencil acceleration buffer 1: 0x%" PRIx64 "\n", indent, "", values->stencil_acceleration_buffer_1);
   fprintf(fp, "%*sStencil buffer 2: 0x%" PRIx64 "\n", indent, "", values->stencil_buffer_2);
   fprintf(fp, "%*sStencil unknown 2: 0x%" PRIx32 "\n", indent, "", values->stencil_unknown_2);
   fprintf(fp, "%*sStencil acceleration buffer 2: 0x%" PRIx64 "\n", indent, "", values->stencil_acceleration_buffer_2);
   fprintf(fp, "%*sUnk 212: 0x%" PRIx32 "\n", indent, "", values->unk_212);
   fprintf(fp, "%*sUnk 214: 0x%" PRIx32 "\n", indent, "", values->unk_214);
   fprintf(fp, "%*sZ16 Unorm attachment 1: %s\n", indent, "", values->z16_unorm_attachment_1 ? "true" : "false");
   fprintf(fp, "%*sWidth 1: %u\n", indent, "", values->width_1);
   fprintf(fp, "%*sHeight 1: %u\n", indent, "", values->height_1);
   fprintf(fp, "%*sPointer: 0x%" PRIx64 "\n", indent, "", values->pointer);
   fprintf(fp, "%*sSpilling unk 1: 0x%" PRIx32 "\n", indent, "", values->spilling_unk_1);
   fprintf(fp, "%*sSpill Buffer Histogram:\n", indent, "");
   AGX_SPILL_BUFFER_HISTOGRAM_print(fp, &values->spill_buffer_histogram, indent + 2);
   fprintf(fp, "%*sDepth clear value: 0x%" PRIx32 "\n", indent, "", values->depth_clear_value);
   fprintf(fp, "%*sStencil clear value: %u\n", indent, "", values->stencil_clear_value);
   fprintf(fp, "%*sUnk 277: 0x%" PRIx32 "\n", indent, "", values->unk_277);
   fprintf(fp, "%*sSet when reloading Z or S 1: %s\n", indent, "", values->set_when_reloading_z_or_s_1 ? "true" : "false");
   fprintf(fp, "%*sSet when frag shader spills: %s\n", indent, "", values->set_when_frag_shader_spills ? "true" : "false");
   fprintf(fp, "%*sSet when reloading Z or S 2: %s\n", indent, "", values->set_when_reloading_z_or_s_2 ? "true" : "false");
   fprintf(fp, "%*sZ16 Unorm attachment 2: %s\n", indent, "", values->z16_unorm_attachment_2 ? "true" : "false");
   fprintf(fp, "%*sUnk 282: 0x%" PRIx32 "\n", indent, "", values->unk_282);
   fprintf(fp, "%*sUnk 283: 0x%" PRIx32 "\n", indent, "", values->unk_283);
   fprintf(fp, "%*sUnk 284: 0x%" PRIx32 "\n", indent, "", values->unk_284);
   fprintf(fp, "%*sVisibility result buffer: 0x%" PRIx64 "\n", indent, "", values->visibility_result_buffer);
   fprintf(fp, "%*sPartial reload pipeline bind: 0x%" PRIx32 "\n", indent, "", values->partial_reload_pipeline_bind);
   fprintf(fp, "%*sPartial reload pipeline unk: 0x%" PRIx32 "\n", indent, "", values->partial_reload_pipeline_unk);
   fprintf(fp, "%*sPartial reload pipeline: 0x%" PRIx64 "\n", indent, "", values->partial_reload_pipeline);
   fprintf(fp, "%*sPartial store pipeline bind: 0x%" PRIx32 "\n", indent, "", values->partial_store_pipeline_bind);
   fprintf(fp, "%*sPartial store pipeline unk: 0x%" PRIx32 "\n", indent, "", values->partial_store_pipeline_unk);
   fprintf(fp, "%*sPartial store pipeline: 0x%" PRIx64 "\n", indent, "", values->partial_store_pipeline);
   fprintf(fp, "%*sDepth buffer 3: 0x%" PRIx64 "\n", indent, "", values->depth_buffer_3);
   fprintf(fp, "%*sDepth acceleration buffer 3: 0x%" PRIx64 "\n", indent, "", values->depth_acceleration_buffer_3);
   fprintf(fp, "%*sStencil buffer 3: 0x%" PRIx64 "\n", indent, "", values->stencil_buffer_3);
   fprintf(fp, "%*sStencil acceleration buffer 3: 0x%" PRIx64 "\n", indent, "", values->stencil_acceleration_buffer_3);
   fprintf(fp, "%*sUnk 352: 0x%" PRIx32 "\n", indent, "", values->unk_352);
   fprintf(fp, "%*sUnk 360: 0x%" PRIx32 "\n", indent, "", values->unk_360);
   fprintf(fp, "%*sEncoder ID: 0x%" PRIx32 "\n", indent, "", values->encoder_id);
   fprintf(fp, "%*sUnk 365: 0x%" PRIx64 "\n", indent, "", values->unk_365);
   fprintf(fp, "%*sUnknown buffer: 0x%" PRIx64 "\n", indent, "", values->unknown_buffer);
   fprintf(fp, "%*sWidth 2: %u\n", indent, "", values->width_2);
   fprintf(fp, "%*sHeight 2: %u\n", indent, "", values->height_2);
   fprintf(fp, "%*sSample count: %u\n", indent, "", values->sample_count);
   fprintf(fp, "%*sSample 0 X: %u\n", indent, "", values->sample_0_x);
   fprintf(fp, "%*sSample 0 Y: %u\n", indent, "", values->sample_0_y);
   fprintf(fp, "%*sSample 1 X: %u\n", indent, "", values->sample_1_x);
   fprintf(fp, "%*sSample 1 Y: %u\n", indent, "", values->sample_1_y);
   fprintf(fp, "%*sSample 2 X: %u\n", indent, "", values->sample_2_x);
   fprintf(fp, "%*sSample 2 Y: %u\n", indent, "", values->sample_2_y);
   fprintf(fp, "%*sSample 3 X: %u\n", indent, "", values->sample_3_x);
   fprintf(fp, "%*sSample 3 Y: %u\n", indent, "", values->sample_3_y);
   fprintf(fp, "%*sUnk 49:0: %u\n", indent, "", values->unk_490);
   fprintf(fp, "%*sTile width: %u\n", indent, "", values->tile_width);
   fprintf(fp, "%*sTile height: %u\n", indent, "", values->tile_height);
   fprintf(fp, "%*sFramebuffer layers: %u\n", indent, "", values->framebuffer_layers);
   fprintf(fp, "%*sUnk 56:0: %u\n", indent, "", values->unk_560);
   fprintf(fp, "%*sUnk 70:0: %u\n", indent, "", values->unk_700);
}

struct AGX_IOGPU_ATTACHMENT_COUNT {
   uint32_t                             count;
};

#define AGX_IOGPU_ATTACHMENT_COUNT_header       \
   0

static inline void
AGX_IOGPU_ATTACHMENT_COUNT_pack(uint32_t * restrict cl,
                                const struct AGX_IOGPU_ATTACHMENT_COUNT * restrict values)
{
   cl[ 0] = 0;
   cl[ 1] = 0;
   cl[ 2] = 0;
   cl[ 3] = util_bitpack_uint(values->count, 0, 31);
}


#define AGX_IOGPU_ATTACHMENT_COUNT_LENGTH 16
struct agx_iogpu_attachment_count_packed { uint32_t opaque[4]; };
static inline void
AGX_IOGPU_ATTACHMENT_COUNT_unpack(FILE *fp, const uint8_t * restrict cl,
                                  struct AGX_IOGPU_ATTACHMENT_COUNT * restrict values)
{
   if (((const uint32_t *) cl)[0] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Attachment Count unpacked at word 0: got %X, bad mask %X\n", ((const uint32_t *) cl)[0], ((const uint32_t *) cl)[0] & 0xffffffff);
   if (((const uint32_t *) cl)[1] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Attachment Count unpacked at word 1: got %X, bad mask %X\n", ((const uint32_t *) cl)[1], ((const uint32_t *) cl)[1] & 0xffffffff);
   if (((const uint32_t *) cl)[2] & 0xffffffff) fprintf(fp, "XXX: Unknown field of IOGPU Attachment Count unpacked at word 2: got %X, bad mask %X\n", ((const uint32_t *) cl)[2], ((const uint32_t *) cl)[2] & 0xffffffff);
   values->count = __gen_unpack_uint(cl, 96, 127);
}

static inline void
AGX_IOGPU_ATTACHMENT_COUNT_print(FILE *fp, const struct AGX_IOGPU_ATTACHMENT_COUNT * values, unsigned indent)
{
   fprintf(fp, "%*sCount: %u\n", indent, "", values->count);
}

#endif
