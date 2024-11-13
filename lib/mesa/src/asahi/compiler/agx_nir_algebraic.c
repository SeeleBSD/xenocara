#include "agx_nir.h"

#include "nir.h"
#include "nir_builder.h"
#include "nir_search.h"
#include "nir_search_helpers.h"

/* What follows is NIR algebraic transform code for the following 30
 * transforms:
 *    ('ishl', 'a@8', 'b') => ('ishl', 'a', ('iand', 'b', 7))
 *    ('ishr', 'a@8', 'b') => ('ishr', 'a', ('iand', 'b', 7))
 *    ('ushr', 'a@8', 'b') => ('ushr', 'a', ('iand', 'b', 7))
 *    ('ishl', 'a@16', 'b') => ('ishl', 'a', ('iand', 'b', 15))
 *    ('ishr', 'a@16', 'b') => ('ishr', 'a', ('iand', 'b', 15))
 *    ('ushr', 'a@16', 'b') => ('ushr', 'a', ('iand', 'b', 15))
 *    ('ishl', 'a@32', 'b') => ('ishl', 'a', ('iand', 'b', 31))
 *    ('ishr', 'a@32', 'b') => ('ishr', 'a', ('iand', 'b', 31))
 *    ('ushr', 'a@32', 'b') => ('ushr', 'a', ('iand', 'b', 31))
 *    ('ishl', 'a@64', 'b') => ('ishl', 'a', ('iand', 'b', 63))
 *    ('ishr', 'a@64', 'b') => ('ishr', 'a', ('iand', 'b', 63))
 *    ('ushr', 'a@64', 'b') => ('ushr', 'a', ('iand', 'b', 63))
 *    ('pack_half_2x16_split', 'a', 'b') => ('pack_32_2x16_split', ('f2f16', 'a'), ('f2f16', 'b'))
 *    ('pack_32_4x8_split', 'a', 'b', 'c', 'd') => ('pack_32_2x16_split', ('ior', ('u2u16', 'a'), ('ishl', ('u2u16', 'b'), 8)), ('ior', ('u2u16', 'c'), ('ishl', ('u2u16', 'd'), 8)))
 *    ('unpack_half_2x16_split_x', 'a') => ('f2f32', ('unpack_32_2x16_split_x', 'a'))
 *    ('unpack_half_2x16_split_y', 'a') => ('f2f32', ('unpack_32_2x16_split_y', 'a'))
 *    ('extract_u16', 'a@32', 0) => ('u2u32', ('unpack_32_2x16_split_x', 'a'))
 *    ('extract_u16', 'a@32', 1) => ('u2u32', ('unpack_32_2x16_split_y', 'a'))
 *    ('extract_i16', 'a@32', 0) => ('i2i32', ('unpack_32_2x16_split_x', 'a'))
 *    ('extract_i16', 'a@32', 1) => ('i2i32', ('unpack_32_2x16_split_y', 'a'))
 *    ('u2f32', ('u2u32', 'a')) => ('u2f32', 'a')
 *    ('i2f32', ('i2i32', 'a')) => ('i2f32', 'a')
 *    ('f2u8', 'a') => ('u2u8', ('f2u16', 'a'))
 *    ('f2i8', 'a') => ('i2i8', ('f2i16', 'a'))
 *    ('f2f16_rtz', 'a@32') => ('bcsel', ('flt', ('fabs', 'a'), ('fabs', ('f2f32', ('f2f16_rtne', 'a')))), ('isub', ('f2f16_rtne', 'a'), 1), ('f2f16_rtne', 'a'))
 *    ('ibitfield_extract', 'value', 'offset', 'bits(is_not_const)') => ('bcsel', ('ieq', 0, 'bits'), 0, ('ishr', ('ishl', 'value', ('isub', ('isub', 32, 'bits'), 'offset')), ('isub', 32, 'bits')))
 *    ('ubitfield_extract', 'value', 'offset', 'bits(is_not_const)') => ('iand', ('ushr', 'value', 'offset'), ('bcsel', ('ieq', 'bits', 32), 4294967295, ('isub', ('ishl', 1, 'bits'), 1)))
 *    ('ubitfield_extract', 'value', 'offset', 0) => 0
 *    ('ibitfield_extract', 'value', 'offset', 0) => 0
 *    ('ibitfield_extract', 'a', 'b', '#bits') => ('ishr', ('ishl', ('ubitfield_extract', 'a', 'b', 'bits'), ('isub', 32, 'bits')), ('isub', 32, 'bits'))
 */


static const nir_search_value_union agx_nir_lower_algebraic_late_values[] = {
   /* ('ishl', 'a@8', 'b') => ('ishl', 'a', ('iand', 'b', 7)) */
   { .variable = {
      { nir_search_value_variable, 8 },
      0, /* a */
      false,
      nir_type_invalid,
      -1,
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
   } },
   { .variable = {
      { nir_search_value_variable, 32 },
      1, /* b */
      false,
      nir_type_invalid,
      -1,
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
   } },
   { .expression = {
      { nir_search_value_expression, 8 },
      false,
      false,
      false,
      nir_op_ishl,
      -1, 0,
      { 0, 1 },
      -1,
   } },

   /* replace0_0 -> 0 in the cache */
   /* replace0_1_0 -> 1 in the cache */
   { .constant = {
      { nir_search_value_constant, 32 },
      nir_type_int, { 0x7 /* 7 */ },
   } },
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_iand,
      0, 1,
      { 1, 3 },
      -1,
   } },
   { .expression = {
      { nir_search_value_expression, 8 },
      false,
      false,
      false,
      nir_op_ishl,
      -1, 1,
      { 0, 4 },
      -1,
   } },

   /* ('ishr', 'a@8', 'b') => ('ishr', 'a', ('iand', 'b', 7)) */
   /* search1_0 -> 0 in the cache */
   /* search1_1 -> 1 in the cache */
   { .expression = {
      { nir_search_value_expression, 8 },
      false,
      false,
      false,
      nir_op_ishr,
      -1, 0,
      { 0, 1 },
      -1,
   } },

   /* replace1_0 -> 0 in the cache */
   /* replace1_1_0 -> 1 in the cache */
   /* replace1_1_1 -> 3 in the cache */
   /* replace1_1 -> 4 in the cache */
   { .expression = {
      { nir_search_value_expression, 8 },
      false,
      false,
      false,
      nir_op_ishr,
      -1, 1,
      { 0, 4 },
      -1,
   } },

   /* ('ushr', 'a@8', 'b') => ('ushr', 'a', ('iand', 'b', 7)) */
   /* search2_0 -> 0 in the cache */
   /* search2_1 -> 1 in the cache */
   { .expression = {
      { nir_search_value_expression, 8 },
      false,
      false,
      false,
      nir_op_ushr,
      -1, 0,
      { 0, 1 },
      -1,
   } },

   /* replace2_0 -> 0 in the cache */
   /* replace2_1_0 -> 1 in the cache */
   /* replace2_1_1 -> 3 in the cache */
   /* replace2_1 -> 4 in the cache */
   { .expression = {
      { nir_search_value_expression, 8 },
      false,
      false,
      false,
      nir_op_ushr,
      -1, 1,
      { 0, 4 },
      -1,
   } },

   /* ('ishl', 'a@16', 'b') => ('ishl', 'a', ('iand', 'b', 15)) */
   { .variable = {
      { nir_search_value_variable, 16 },
      0, /* a */
      false,
      nir_type_invalid,
      -1,
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
   } },
   /* search3_1 -> 1 in the cache */
   { .expression = {
      { nir_search_value_expression, 16 },
      false,
      false,
      false,
      nir_op_ishl,
      -1, 0,
      { 10, 1 },
      -1,
   } },

   /* replace3_0 -> 10 in the cache */
   /* replace3_1_0 -> 1 in the cache */
   { .constant = {
      { nir_search_value_constant, 32 },
      nir_type_int, { 0xf /* 15 */ },
   } },
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_iand,
      0, 1,
      { 1, 12 },
      -1,
   } },
   { .expression = {
      { nir_search_value_expression, 16 },
      false,
      false,
      false,
      nir_op_ishl,
      -1, 1,
      { 10, 13 },
      -1,
   } },

   /* ('ishr', 'a@16', 'b') => ('ishr', 'a', ('iand', 'b', 15)) */
   /* search4_0 -> 10 in the cache */
   /* search4_1 -> 1 in the cache */
   { .expression = {
      { nir_search_value_expression, 16 },
      false,
      false,
      false,
      nir_op_ishr,
      -1, 0,
      { 10, 1 },
      -1,
   } },

   /* replace4_0 -> 10 in the cache */
   /* replace4_1_0 -> 1 in the cache */
   /* replace4_1_1 -> 12 in the cache */
   /* replace4_1 -> 13 in the cache */
   { .expression = {
      { nir_search_value_expression, 16 },
      false,
      false,
      false,
      nir_op_ishr,
      -1, 1,
      { 10, 13 },
      -1,
   } },

   /* ('ushr', 'a@16', 'b') => ('ushr', 'a', ('iand', 'b', 15)) */
   /* search5_0 -> 10 in the cache */
   /* search5_1 -> 1 in the cache */
   { .expression = {
      { nir_search_value_expression, 16 },
      false,
      false,
      false,
      nir_op_ushr,
      -1, 0,
      { 10, 1 },
      -1,
   } },

   /* replace5_0 -> 10 in the cache */
   /* replace5_1_0 -> 1 in the cache */
   /* replace5_1_1 -> 12 in the cache */
   /* replace5_1 -> 13 in the cache */
   { .expression = {
      { nir_search_value_expression, 16 },
      false,
      false,
      false,
      nir_op_ushr,
      -1, 1,
      { 10, 13 },
      -1,
   } },

   /* ('ishl', 'a@32', 'b') => ('ishl', 'a', ('iand', 'b', 31)) */
   { .variable = {
      { nir_search_value_variable, 32 },
      0, /* a */
      false,
      nir_type_invalid,
      -1,
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
   } },
   /* search6_1 -> 1 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_ishl,
      -1, 0,
      { 19, 1 },
      -1,
   } },

   /* replace6_0 -> 19 in the cache */
   /* replace6_1_0 -> 1 in the cache */
   { .constant = {
      { nir_search_value_constant, 32 },
      nir_type_int, { 0x1f /* 31 */ },
   } },
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_iand,
      0, 1,
      { 1, 21 },
      -1,
   } },
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_ishl,
      -1, 1,
      { 19, 22 },
      -1,
   } },

   /* ('ishr', 'a@32', 'b') => ('ishr', 'a', ('iand', 'b', 31)) */
   /* search7_0 -> 19 in the cache */
   /* search7_1 -> 1 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_ishr,
      -1, 0,
      { 19, 1 },
      -1,
   } },

   /* replace7_0 -> 19 in the cache */
   /* replace7_1_0 -> 1 in the cache */
   /* replace7_1_1 -> 21 in the cache */
   /* replace7_1 -> 22 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_ishr,
      -1, 1,
      { 19, 22 },
      -1,
   } },

   /* ('ushr', 'a@32', 'b') => ('ushr', 'a', ('iand', 'b', 31)) */
   /* search8_0 -> 19 in the cache */
   /* search8_1 -> 1 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_ushr,
      -1, 0,
      { 19, 1 },
      -1,
   } },

   /* replace8_0 -> 19 in the cache */
   /* replace8_1_0 -> 1 in the cache */
   /* replace8_1_1 -> 21 in the cache */
   /* replace8_1 -> 22 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_ushr,
      -1, 1,
      { 19, 22 },
      -1,
   } },

   /* ('ishl', 'a@64', 'b') => ('ishl', 'a', ('iand', 'b', 63)) */
   { .variable = {
      { nir_search_value_variable, 64 },
      0, /* a */
      false,
      nir_type_invalid,
      -1,
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
   } },
   /* search9_1 -> 1 in the cache */
   { .expression = {
      { nir_search_value_expression, 64 },
      false,
      false,
      false,
      nir_op_ishl,
      -1, 0,
      { 28, 1 },
      -1,
   } },

   /* replace9_0 -> 28 in the cache */
   /* replace9_1_0 -> 1 in the cache */
   { .constant = {
      { nir_search_value_constant, 32 },
      nir_type_int, { 0x3f /* 63 */ },
   } },
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_iand,
      0, 1,
      { 1, 30 },
      -1,
   } },
   { .expression = {
      { nir_search_value_expression, 64 },
      false,
      false,
      false,
      nir_op_ishl,
      -1, 1,
      { 28, 31 },
      -1,
   } },

   /* ('ishr', 'a@64', 'b') => ('ishr', 'a', ('iand', 'b', 63)) */
   /* search10_0 -> 28 in the cache */
   /* search10_1 -> 1 in the cache */
   { .expression = {
      { nir_search_value_expression, 64 },
      false,
      false,
      false,
      nir_op_ishr,
      -1, 0,
      { 28, 1 },
      -1,
   } },

   /* replace10_0 -> 28 in the cache */
   /* replace10_1_0 -> 1 in the cache */
   /* replace10_1_1 -> 30 in the cache */
   /* replace10_1 -> 31 in the cache */
   { .expression = {
      { nir_search_value_expression, 64 },
      false,
      false,
      false,
      nir_op_ishr,
      -1, 1,
      { 28, 31 },
      -1,
   } },

   /* ('ushr', 'a@64', 'b') => ('ushr', 'a', ('iand', 'b', 63)) */
   /* search11_0 -> 28 in the cache */
   /* search11_1 -> 1 in the cache */
   { .expression = {
      { nir_search_value_expression, 64 },
      false,
      false,
      false,
      nir_op_ushr,
      -1, 0,
      { 28, 1 },
      -1,
   } },

   /* replace11_0 -> 28 in the cache */
   /* replace11_1_0 -> 1 in the cache */
   /* replace11_1_1 -> 30 in the cache */
   /* replace11_1 -> 31 in the cache */
   { .expression = {
      { nir_search_value_expression, 64 },
      false,
      false,
      false,
      nir_op_ushr,
      -1, 1,
      { 28, 31 },
      -1,
   } },

   /* ('pack_half_2x16_split', 'a', 'b') => ('pack_32_2x16_split', ('f2f16', 'a'), ('f2f16', 'b')) */
   /* search12_0 -> 19 in the cache */
   /* search12_1 -> 1 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_pack_half_2x16_split,
      -1, 0,
      { 19, 1 },
      -1,
   } },

   /* replace12_0_0 -> 19 in the cache */
   { .expression = {
      { nir_search_value_expression, 16 },
      false,
      false,
      false,
      nir_op_f2f16,
      -1, 0,
      { 19 },
      -1,
   } },
   /* replace12_1_0 -> 1 in the cache */
   { .expression = {
      { nir_search_value_expression, 16 },
      false,
      false,
      false,
      nir_op_f2f16,
      -1, 0,
      { 1 },
      -1,
   } },
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_pack_32_2x16_split,
      -1, 0,
      { 38, 39 },
      -1,
   } },

   /* ('pack_32_4x8_split', 'a', 'b', 'c', 'd') => ('pack_32_2x16_split', ('ior', ('u2u16', 'a'), ('ishl', ('u2u16', 'b'), 8)), ('ior', ('u2u16', 'c'), ('ishl', ('u2u16', 'd'), 8))) */
   /* search13_0 -> 0 in the cache */
   { .variable = {
      { nir_search_value_variable, 8 },
      1, /* b */
      false,
      nir_type_invalid,
      -1,
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
   } },
   { .variable = {
      { nir_search_value_variable, 8 },
      2, /* c */
      false,
      nir_type_invalid,
      -1,
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
   } },
   { .variable = {
      { nir_search_value_variable, 8 },
      3, /* d */
      false,
      nir_type_invalid,
      -1,
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
   } },
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_pack_32_4x8_split,
      -1, 0,
      { 0, 41, 42, 43 },
      -1,
   } },

   /* replace13_0_0_0 -> 0 in the cache */
   { .expression = {
      { nir_search_value_expression, 16 },
      false,
      false,
      false,
      nir_op_u2u16,
      -1, 0,
      { 0 },
      -1,
   } },
   /* replace13_0_1_0_0 -> 41 in the cache */
   { .expression = {
      { nir_search_value_expression, 16 },
      false,
      false,
      false,
      nir_op_u2u16,
      -1, 0,
      { 41 },
      -1,
   } },
   { .constant = {
      { nir_search_value_constant, 32 },
      nir_type_int, { 0x8 /* 8 */ },
   } },
   { .expression = {
      { nir_search_value_expression, 16 },
      false,
      false,
      false,
      nir_op_ishl,
      -1, 0,
      { 46, 47 },
      -1,
   } },
   { .expression = {
      { nir_search_value_expression, 16 },
      false,
      false,
      false,
      nir_op_ior,
      0, 1,
      { 45, 48 },
      -1,
   } },
   /* replace13_1_0_0 -> 42 in the cache */
   { .expression = {
      { nir_search_value_expression, 16 },
      false,
      false,
      false,
      nir_op_u2u16,
      -1, 0,
      { 42 },
      -1,
   } },
   /* replace13_1_1_0_0 -> 43 in the cache */
   { .expression = {
      { nir_search_value_expression, 16 },
      false,
      false,
      false,
      nir_op_u2u16,
      -1, 0,
      { 43 },
      -1,
   } },
   /* replace13_1_1_1 -> 47 in the cache */
   { .expression = {
      { nir_search_value_expression, 16 },
      false,
      false,
      false,
      nir_op_ishl,
      -1, 0,
      { 51, 47 },
      -1,
   } },
   { .expression = {
      { nir_search_value_expression, 16 },
      false,
      false,
      false,
      nir_op_ior,
      1, 1,
      { 50, 52 },
      -1,
   } },
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_pack_32_2x16_split,
      -1, 2,
      { 49, 53 },
      -1,
   } },

   /* ('unpack_half_2x16_split_x', 'a') => ('f2f32', ('unpack_32_2x16_split_x', 'a')) */
   /* search14_0 -> 19 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_unpack_half_2x16_split_x,
      -1, 0,
      { 19 },
      -1,
   } },

   /* replace14_0_0 -> 19 in the cache */
   { .expression = {
      { nir_search_value_expression, 16 },
      false,
      false,
      false,
      nir_op_unpack_32_2x16_split_x,
      -1, 0,
      { 19 },
      -1,
   } },
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_f2f32,
      -1, 0,
      { 56 },
      -1,
   } },

   /* ('unpack_half_2x16_split_y', 'a') => ('f2f32', ('unpack_32_2x16_split_y', 'a')) */
   /* search15_0 -> 19 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_unpack_half_2x16_split_y,
      -1, 0,
      { 19 },
      -1,
   } },

   /* replace15_0_0 -> 19 in the cache */
   { .expression = {
      { nir_search_value_expression, 16 },
      false,
      false,
      false,
      nir_op_unpack_32_2x16_split_y,
      -1, 0,
      { 19 },
      -1,
   } },
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_f2f32,
      -1, 0,
      { 59 },
      -1,
   } },

   /* ('extract_u16', 'a@32', 0) => ('u2u32', ('unpack_32_2x16_split_x', 'a')) */
   /* search16_0 -> 19 in the cache */
   { .constant = {
      { nir_search_value_constant, 32 },
      nir_type_int, { 0x0 /* 0 */ },
   } },
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_extract_u16,
      -1, 0,
      { 19, 61 },
      -1,
   } },

   /* replace16_0_0 -> 19 in the cache */
   /* replace16_0 -> 56 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_u2u32,
      -1, 0,
      { 56 },
      -1,
   } },

   /* ('extract_u16', 'a@32', 1) => ('u2u32', ('unpack_32_2x16_split_y', 'a')) */
   /* search17_0 -> 19 in the cache */
   { .constant = {
      { nir_search_value_constant, 32 },
      nir_type_int, { 0x1 /* 1 */ },
   } },
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_extract_u16,
      -1, 0,
      { 19, 64 },
      -1,
   } },

   /* replace17_0_0 -> 19 in the cache */
   /* replace17_0 -> 59 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_u2u32,
      -1, 0,
      { 59 },
      -1,
   } },

   /* ('extract_i16', 'a@32', 0) => ('i2i32', ('unpack_32_2x16_split_x', 'a')) */
   /* search18_0 -> 19 in the cache */
   /* search18_1 -> 61 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_extract_i16,
      -1, 0,
      { 19, 61 },
      -1,
   } },

   /* replace18_0_0 -> 19 in the cache */
   /* replace18_0 -> 56 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_i2i32,
      -1, 0,
      { 56 },
      -1,
   } },

   /* ('extract_i16', 'a@32', 1) => ('i2i32', ('unpack_32_2x16_split_y', 'a')) */
   /* search19_0 -> 19 in the cache */
   /* search19_1 -> 64 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_extract_i16,
      -1, 0,
      { 19, 64 },
      -1,
   } },

   /* replace19_0_0 -> 19 in the cache */
   /* replace19_0 -> 59 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_i2i32,
      -1, 0,
      { 59 },
      -1,
   } },

   /* ('u2f32', ('u2u32', 'a')) => ('u2f32', 'a') */
   { .variable = {
      { nir_search_value_variable, -1 },
      0, /* a */
      false,
      nir_type_invalid,
      -1,
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
   } },
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_u2u32,
      -1, 0,
      { 71 },
      -1,
   } },
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_u2f32,
      -1, 0,
      { 72 },
      -1,
   } },

   /* replace20_0 -> 71 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_u2f32,
      -1, 0,
      { 71 },
      -1,
   } },

   /* ('i2f32', ('i2i32', 'a')) => ('i2f32', 'a') */
   /* search21_0_0 -> 71 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_i2i32,
      -1, 0,
      { 71 },
      -1,
   } },
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_i2f32,
      -1, 0,
      { 75 },
      -1,
   } },

   /* replace21_0 -> 71 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_i2f32,
      -1, 0,
      { 71 },
      -1,
   } },

   /* ('f2u8', 'a') => ('u2u8', ('f2u16', 'a')) */
   /* search22_0 -> 71 in the cache */
   { .expression = {
      { nir_search_value_expression, 8 },
      false,
      false,
      false,
      nir_op_f2u8,
      -1, 0,
      { 71 },
      -1,
   } },

   /* replace22_0_0 -> 71 in the cache */
   { .expression = {
      { nir_search_value_expression, 16 },
      false,
      false,
      false,
      nir_op_f2u16,
      -1, 0,
      { 71 },
      -1,
   } },
   { .expression = {
      { nir_search_value_expression, 8 },
      false,
      false,
      false,
      nir_op_u2u8,
      -1, 0,
      { 79 },
      -1,
   } },

   /* ('f2i8', 'a') => ('i2i8', ('f2i16', 'a')) */
   /* search23_0 -> 71 in the cache */
   { .expression = {
      { nir_search_value_expression, 8 },
      false,
      false,
      false,
      nir_op_f2i8,
      -1, 0,
      { 71 },
      -1,
   } },

   /* replace23_0_0 -> 71 in the cache */
   { .expression = {
      { nir_search_value_expression, 16 },
      false,
      false,
      false,
      nir_op_f2i16,
      -1, 0,
      { 71 },
      -1,
   } },
   { .expression = {
      { nir_search_value_expression, 8 },
      false,
      false,
      false,
      nir_op_i2i8,
      -1, 0,
      { 82 },
      -1,
   } },

   /* ('f2f16_rtz', 'a@32') => ('bcsel', ('flt', ('fabs', 'a'), ('fabs', ('f2f32', ('f2f16_rtne', 'a')))), ('isub', ('f2f16_rtne', 'a'), 1), ('f2f16_rtne', 'a')) */
   /* search24_0 -> 19 in the cache */
   { .expression = {
      { nir_search_value_expression, 16 },
      false,
      false,
      false,
      nir_op_f2f16_rtz,
      -1, 0,
      { 19 },
      -1,
   } },

   /* replace24_0_0_0 -> 19 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_fabs,
      -1, 0,
      { 19 },
      -1,
   } },
   /* replace24_0_1_0_0_0 -> 19 in the cache */
   { .expression = {
      { nir_search_value_expression, 16 },
      false,
      false,
      false,
      nir_op_f2f16_rtne,
      -1, 0,
      { 19 },
      -1,
   } },
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_f2f32,
      -1, 0,
      { 86 },
      -1,
   } },
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_fabs,
      -1, 0,
      { 87 },
      -1,
   } },
   { .expression = {
      { nir_search_value_expression, 1 },
      false,
      false,
      false,
      nir_op_flt,
      -1, 0,
      { 85, 88 },
      -1,
   } },
   /* replace24_1_0_0 -> 19 in the cache */
   /* replace24_1_0 -> 86 in the cache */
   { .constant = {
      { nir_search_value_constant, 16 },
      nir_type_int, { 0x1 /* 1 */ },
   } },
   { .expression = {
      { nir_search_value_expression, 16 },
      false,
      false,
      false,
      nir_op_isub,
      -1, 0,
      { 86, 90 },
      -1,
   } },
   /* replace24_2_0 -> 19 in the cache */
   /* replace24_2 -> 86 in the cache */
   { .expression = {
      { nir_search_value_expression, 16 },
      false,
      false,
      false,
      nir_op_bcsel,
      -1, 0,
      { 89, 91, 86 },
      -1,
   } },

   /* ('ibitfield_extract', 'value', 'offset', 'bits(is_not_const)') => ('bcsel', ('ieq', 0, 'bits'), 0, ('ishr', ('ishl', 'value', ('isub', ('isub', 32, 'bits'), 'offset')), ('isub', 32, 'bits'))) */
   { .variable = {
      { nir_search_value_variable, 32 },
      0, /* value */
      false,
      nir_type_invalid,
      -1,
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
   } },
   { .variable = {
      { nir_search_value_variable, 32 },
      1, /* offset */
      false,
      nir_type_invalid,
      -1,
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
   } },
   { .variable = {
      { nir_search_value_variable, 32 },
      2, /* bits */
      false,
      nir_type_invalid,
      0,
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
   } },
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_ibitfield_extract,
      -1, 0,
      { 93, 94, 95 },
      -1,
   } },

   /* replace25_0_0 -> 61 in the cache */
   { .variable = {
      { nir_search_value_variable, 32 },
      2, /* bits */
      false,
      nir_type_invalid,
      -1,
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
   } },
   { .expression = {
      { nir_search_value_expression, 1 },
      false,
      false,
      false,
      nir_op_ieq,
      0, 1,
      { 61, 97 },
      -1,
   } },
   /* replace25_1 -> 61 in the cache */
   /* replace25_2_0_0 -> 93 in the cache */
   { .constant = {
      { nir_search_value_constant, 32 },
      nir_type_int, { 0x20 /* 32 */ },
   } },
   /* replace25_2_0_1_0_1 -> 97 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_isub,
      -1, 0,
      { 99, 97 },
      -1,
   } },
   /* replace25_2_0_1_1 -> 94 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_isub,
      -1, 0,
      { 100, 94 },
      -1,
   } },
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_ishl,
      -1, 0,
      { 93, 101 },
      -1,
   } },
   /* replace25_2_1_0 -> 99 in the cache */
   /* replace25_2_1_1 -> 97 in the cache */
   /* replace25_2_1 -> 100 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_ishr,
      -1, 0,
      { 102, 100 },
      -1,
   } },
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_bcsel,
      -1, 1,
      { 98, 61, 103 },
      -1,
   } },

   /* ('ubitfield_extract', 'value', 'offset', 'bits(is_not_const)') => ('iand', ('ushr', 'value', 'offset'), ('bcsel', ('ieq', 'bits', 32), 4294967295, ('isub', ('ishl', 1, 'bits'), 1))) */
   /* search26_0 -> 93 in the cache */
   /* search26_1 -> 94 in the cache */
   /* search26_2 -> 95 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_ubitfield_extract,
      -1, 0,
      { 93, 94, 95 },
      -1,
   } },

   /* replace26_0_0 -> 93 in the cache */
   /* replace26_0_1 -> 94 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_ushr,
      -1, 0,
      { 93, 94 },
      -1,
   } },
   /* replace26_1_0_0 -> 97 in the cache */
   /* replace26_1_0_1 -> 99 in the cache */
   { .expression = {
      { nir_search_value_expression, 1 },
      false,
      false,
      false,
      nir_op_ieq,
      1, 1,
      { 97, 99 },
      -1,
   } },
   { .constant = {
      { nir_search_value_constant, 32 },
      nir_type_int, { 0xffffffff /* 4294967295 */ },
   } },
   /* replace26_1_2_0_0 -> 64 in the cache */
   /* replace26_1_2_0_1 -> 97 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_ishl,
      -1, 0,
      { 64, 97 },
      -1,
   } },
   /* replace26_1_2_1 -> 64 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_isub,
      -1, 0,
      { 109, 64 },
      -1,
   } },
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_bcsel,
      -1, 1,
      { 107, 108, 110 },
      -1,
   } },
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_iand,
      0, 2,
      { 106, 111 },
      -1,
   } },

   /* ('ubitfield_extract', 'value', 'offset', 0) => 0 */
   /* search27_0 -> 93 in the cache */
   /* search27_1 -> 94 in the cache */
   /* search27_2 -> 61 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_ubitfield_extract,
      -1, 0,
      { 93, 94, 61 },
      -1,
   } },

   /* replace27 -> 61 in the cache */

   /* ('ibitfield_extract', 'value', 'offset', 0) => 0 */
   /* search28_0 -> 93 in the cache */
   /* search28_1 -> 94 in the cache */
   /* search28_2 -> 61 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_ibitfield_extract,
      -1, 0,
      { 93, 94, 61 },
      -1,
   } },

   /* replace28 -> 61 in the cache */

   /* ('ibitfield_extract', 'a', 'b', '#bits') => ('ishr', ('ishl', ('ubitfield_extract', 'a', 'b', 'bits'), ('isub', 32, 'bits')), ('isub', 32, 'bits')) */
   /* search29_0 -> 19 in the cache */
   /* search29_1 -> 1 in the cache */
   { .variable = {
      { nir_search_value_variable, 32 },
      2, /* bits */
      true,
      nir_type_invalid,
      -1,
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
   } },
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_ibitfield_extract,
      -1, 0,
      { 19, 1, 115 },
      -1,
   } },

   /* replace29_0_0_0 -> 19 in the cache */
   /* replace29_0_0_1 -> 1 in the cache */
   /* replace29_0_0_2 -> 97 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_ubitfield_extract,
      -1, 0,
      { 19, 1, 97 },
      -1,
   } },
   /* replace29_0_1_0 -> 99 in the cache */
   /* replace29_0_1_1 -> 97 in the cache */
   /* replace29_0_1 -> 100 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_ishl,
      -1, 0,
      { 117, 100 },
      -1,
   } },
   /* replace29_1_0 -> 99 in the cache */
   /* replace29_1_1 -> 97 in the cache */
   /* replace29_1 -> 100 in the cache */
   { .expression = {
      { nir_search_value_expression, 32 },
      false,
      false,
      false,
      nir_op_ishr,
      -1, 0,
      { 118, 100 },
      -1,
   } },

};


static const nir_search_variable_cond agx_nir_lower_algebraic_late_variable_cond[] = {
   (is_not_const),
};

static const struct transform agx_nir_lower_algebraic_late_transforms[] = {
   { ~0, ~0, ~0 }, /* Sentinel */

   { 2, 5, 0 },
   { 11, 14, 0 },
   { 20, 23, 0 },
   { 29, 32, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

   { 6, 7, 0 },
   { 15, 16, 0 },
   { 24, 25, 0 },
   { 33, 34, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

   { 8, 9, 0 },
   { 17, 18, 0 },
   { 26, 27, 0 },
   { 35, 36, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

   { 37, 40, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

   { 44, 54, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

   { 55, 57, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

   { 58, 60, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

   { 62, 63, 0 },
   { 65, 66, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

   { 67, 68, 0 },
   { 69, 70, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

   { 78, 80, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

   { 81, 83, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

   { 84, 92, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

   { 96, 104, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

   { 96, 104, 0 },
   { 114, 61, 0 },
   { 116, 119, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

   { 105, 112, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

   { 105, 112, 0 },
   { 113, 61, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

   { 73, 74, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

   { 76, 77, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

};

static const struct per_op_table agx_nir_lower_algebraic_late_pass_op_table[nir_num_search_ops] = {
   [nir_op_ishl] = {
      .filter = NULL,
      
      .num_filtered_states = 1,
      .table = (const uint16_t []) {
      
         2,
      },
   },
   [nir_op_ishr] = {
      .filter = NULL,
      
      .num_filtered_states = 1,
      .table = (const uint16_t []) {
      
         3,
      },
   },
   [nir_op_ushr] = {
      .filter = NULL,
      
      .num_filtered_states = 1,
      .table = (const uint16_t []) {
      
         4,
      },
   },
   [nir_op_pack_half_2x16_split] = {
      .filter = NULL,
      
      .num_filtered_states = 1,
      .table = (const uint16_t []) {
      
         5,
      },
   },
   [nir_op_pack_32_4x8_split] = {
      .filter = NULL,
      
      .num_filtered_states = 1,
      .table = (const uint16_t []) {
      
         6,
      },
   },
   [nir_op_unpack_half_2x16_split_x] = {
      .filter = NULL,
      
      .num_filtered_states = 1,
      .table = (const uint16_t []) {
      
         7,
      },
   },
   [nir_op_unpack_half_2x16_split_y] = {
      .filter = NULL,
      
      .num_filtered_states = 1,
      .table = (const uint16_t []) {
      
         8,
      },
   },
   [nir_op_extract_u16] = {
      .filter = (const uint16_t []) {
         0,
         1,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
      },
      
      .num_filtered_states = 2,
      .table = (const uint16_t []) {
      
         0,
         9,
         0,
         9,
      },
   },
   [nir_op_extract_i16] = {
      .filter = (const uint16_t []) {
         0,
         1,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
      },
      
      .num_filtered_states = 2,
      .table = (const uint16_t []) {
      
         0,
         10,
         0,
         10,
      },
   },
   [nir_search_op_u2f] = {
      .filter = (const uint16_t []) {
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         1,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
      },
      
      .num_filtered_states = 2,
      .table = (const uint16_t []) {
      
         0,
         20,
      },
   },
   [nir_search_op_u2u] = {
      .filter = NULL,
      
      .num_filtered_states = 1,
      .table = (const uint16_t []) {
      
         11,
      },
   },
   [nir_search_op_i2f] = {
      .filter = (const uint16_t []) {
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         1,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
      },
      
      .num_filtered_states = 2,
      .table = (const uint16_t []) {
      
         0,
         21,
      },
   },
   [nir_search_op_i2i] = {
      .filter = NULL,
      
      .num_filtered_states = 1,
      .table = (const uint16_t []) {
      
         12,
      },
   },
   [nir_search_op_f2u] = {
      .filter = NULL,
      
      .num_filtered_states = 1,
      .table = (const uint16_t []) {
      
         13,
      },
   },
   [nir_search_op_f2i] = {
      .filter = NULL,
      
      .num_filtered_states = 1,
      .table = (const uint16_t []) {
      
         14,
      },
   },
   [nir_op_f2f16_rtz] = {
      .filter = NULL,
      
      .num_filtered_states = 1,
      .table = (const uint16_t []) {
      
         15,
      },
   },
   [nir_op_ibitfield_extract] = {
      .filter = (const uint16_t []) {
         0,
         1,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
      },
      
      .num_filtered_states = 2,
      .table = (const uint16_t []) {
      
         16,
         17,
         16,
         17,
         16,
         17,
         16,
         17,
      },
   },
   [nir_op_ubitfield_extract] = {
      .filter = (const uint16_t []) {
         0,
         1,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
      },
      
      .num_filtered_states = 2,
      .table = (const uint16_t []) {
      
         18,
         19,
         18,
         19,
         18,
         19,
         18,
         19,
      },
   },
};

/* Mapping from state index to offset in transforms (0 being no transforms) */
static const uint16_t agx_nir_lower_algebraic_late_transform_offsets[] = {
   0,
   0,
   1,
   6,
   11,
   16,
   18,
   20,
   22,
   24,
   27,
   0,
   0,
   30,
   32,
   34,
   36,
   38,
   42,
   44,
   47,
   49,
};

static const nir_algebraic_table agx_nir_lower_algebraic_late_table = {
   .transforms = agx_nir_lower_algebraic_late_transforms,
   .transform_offsets = agx_nir_lower_algebraic_late_transform_offsets,
   .pass_op_table = agx_nir_lower_algebraic_late_pass_op_table,
   .values = agx_nir_lower_algebraic_late_values,
   .expression_cond = NULL,
   .variable_cond = agx_nir_lower_algebraic_late_variable_cond,
};

bool
agx_nir_lower_algebraic_late(nir_shader *shader)
{
   bool progress = false;
   bool condition_flags[1];
   const nir_shader_compiler_options *options = shader->options;
   const shader_info *info = &shader->info;
   (void) options;
   (void) info;

   STATIC_ASSERT(120 == ARRAY_SIZE(agx_nir_lower_algebraic_late_values));
   condition_flags[0] = true;

   nir_foreach_function_impl(impl, shader) {
     progress |= nir_algebraic_impl(impl, condition_flags, &agx_nir_lower_algebraic_late_table);
   }

   return progress;
}


#include "nir.h"
#include "nir_builder.h"
#include "nir_search.h"
#include "nir_search_helpers.h"

/* What follows is NIR algebraic transform code for the following 39
 * transforms:
 *    ('iadd', ('iadd(is_used_once)', ('imul(is_used_once)', 'a', 'b'), ('imul(is_used_once)', 'c', 'd')), 'e') => ('imadshl_agx', 'a', 'b', ('imadshl_agx', 'c', 'd', 'e', 0), 0)
 *    ('iadd', ('imul(is_used_once)', 'a', 'b'), 'c') => ('imadshl_agx', 'a', 'b', 'c', 0)
 *    ('isub', ('imul(is_used_once)', 'a', 'b'), 'c') => ('imsubshl_agx', 'a', 'b', 'c', 0)
 *    ('iadd', 'a', ('ishl(is_used_once)', 'b', 1)) => ('imadshl_agx', 'a', 1, 'b', 1)
 *    ('isub', 'a', ('ishl(is_used_once)', 'b', 1)) => ('imsubshl_agx', 'a', 1, 'b', 1)
 *    ('ineg', ('ishl(is_used_once)', 'b', 1)) => ('imsubshl_agx', 0, 1, 'b', 1)
 *    ('imadshl_agx', 'a', 'b', ('ishl(is_used_once)', 'c', 1), 0) => ('imadshl_agx', 'a', 'b', 'c', 1)
 *    ('imsubshl_agx', 'a', 'b', ('ishl(is_used_once)', 'c', 1), 0) => ('imsubshl_agx', 'a', 'b', 'c', 1)
 *    ('imul', 'a', 3) => ('imadshl_agx', 'a', 1, 'a', 1)
 *    ('imul', 'a', -1) => ('imsubshl_agx', 'a', 1, 'a', 1)
 *    ('ineg', ('imul(is_used_once)', 'a', 1)) => ('imsubshl_agx', 'a', 1, 'a', 1)
 *    ('ishl', 'a', 1) => ('imadshl_agx', 0, 1, 'a', 1)
 *    ('iadd', 'a', ('ishl(is_used_once)', 'b', 2)) => ('imadshl_agx', 'a', 1, 'b', 2)
 *    ('isub', 'a', ('ishl(is_used_once)', 'b', 2)) => ('imsubshl_agx', 'a', 1, 'b', 2)
 *    ('ineg', ('ishl(is_used_once)', 'b', 2)) => ('imsubshl_agx', 0, 1, 'b', 2)
 *    ('imadshl_agx', 'a', 'b', ('ishl(is_used_once)', 'c', 2), 0) => ('imadshl_agx', 'a', 'b', 'c', 2)
 *    ('imsubshl_agx', 'a', 'b', ('ishl(is_used_once)', 'c', 2), 0) => ('imsubshl_agx', 'a', 'b', 'c', 2)
 *    ('imul', 'a', 5) => ('imadshl_agx', 'a', 1, 'a', 2)
 *    ('imul', 'a', -3) => ('imsubshl_agx', 'a', 1, 'a', 2)
 *    ('ineg', ('imul(is_used_once)', 'a', 3)) => ('imsubshl_agx', 'a', 1, 'a', 2)
 *    ('ishl', 'a', 2) => ('imadshl_agx', 0, 1, 'a', 2)
 *    ('iadd', 'a', ('ishl(is_used_once)', 'b', 3)) => ('imadshl_agx', 'a', 1, 'b', 3)
 *    ('isub', 'a', ('ishl(is_used_once)', 'b', 3)) => ('imsubshl_agx', 'a', 1, 'b', 3)
 *    ('ineg', ('ishl(is_used_once)', 'b', 3)) => ('imsubshl_agx', 0, 1, 'b', 3)
 *    ('imadshl_agx', 'a', 'b', ('ishl(is_used_once)', 'c', 3), 0) => ('imadshl_agx', 'a', 'b', 'c', 3)
 *    ('imsubshl_agx', 'a', 'b', ('ishl(is_used_once)', 'c', 3), 0) => ('imsubshl_agx', 'a', 'b', 'c', 3)
 *    ('imul', 'a', 9) => ('imadshl_agx', 'a', 1, 'a', 3)
 *    ('imul', 'a', -7) => ('imsubshl_agx', 'a', 1, 'a', 3)
 *    ('ineg', ('imul(is_used_once)', 'a', 7)) => ('imsubshl_agx', 'a', 1, 'a', 3)
 *    ('ishl', 'a', 3) => ('imadshl_agx', 0, 1, 'a', 3)
 *    ('iadd', 'a', ('ishl(is_used_once)', 'b', 4)) => ('imadshl_agx', 'a', 1, 'b', 4)
 *    ('isub', 'a', ('ishl(is_used_once)', 'b', 4)) => ('imsubshl_agx', 'a', 1, 'b', 4)
 *    ('ineg', ('ishl(is_used_once)', 'b', 4)) => ('imsubshl_agx', 0, 1, 'b', 4)
 *    ('imadshl_agx', 'a', 'b', ('ishl(is_used_once)', 'c', 4), 0) => ('imadshl_agx', 'a', 'b', 'c', 4)
 *    ('imsubshl_agx', 'a', 'b', ('ishl(is_used_once)', 'c', 4), 0) => ('imsubshl_agx', 'a', 'b', 'c', 4)
 *    ('imul', 'a', 17) => ('imadshl_agx', 'a', 1, 'a', 4)
 *    ('imul', 'a', -15) => ('imsubshl_agx', 'a', 1, 'a', 4)
 *    ('ineg', ('imul(is_used_once)', 'a', 15)) => ('imsubshl_agx', 'a', 1, 'a', 4)
 *    ('ishl', 'a', 4) => ('imadshl_agx', 0, 1, 'a', 4)
 */


static const nir_search_value_union agx_nir_fuse_algebraic_late_values[] = {
   /* ('iadd', ('iadd(is_used_once)', ('imul(is_used_once)', 'a', 'b'), ('imul(is_used_once)', 'c', 'd')), 'e') => ('imadshl_agx', 'a', 'b', ('imadshl_agx', 'c', 'd', 'e', 0), 0) */
   { .variable = {
      { nir_search_value_variable, -5 },
      0, /* a */
      false,
      nir_type_invalid,
      -1,
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
   } },
   { .variable = {
      { nir_search_value_variable, -5 },
      1, /* b */
      false,
      nir_type_invalid,
      -1,
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
   } },
   { .expression = {
      { nir_search_value_expression, -5 },
      false,
      false,
      false,
      nir_op_imul,
      2, 1,
      { 0, 1 },
      0,
   } },
   { .variable = {
      { nir_search_value_variable, -5 },
      2, /* c */
      false,
      nir_type_invalid,
      -1,
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
   } },
   { .variable = {
      { nir_search_value_variable, -5 },
      3, /* d */
      false,
      nir_type_invalid,
      -1,
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
   } },
   { .expression = {
      { nir_search_value_expression, -5 },
      false,
      false,
      false,
      nir_op_imul,
      3, 1,
      { 3, 4 },
      0,
   } },
   { .expression = {
      { nir_search_value_expression, -5 },
      false,
      false,
      false,
      nir_op_iadd,
      1, 3,
      { 2, 5 },
      0,
   } },
   { .variable = {
      { nir_search_value_variable, -5 },
      4, /* e */
      false,
      nir_type_invalid,
      -1,
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
   } },
   { .expression = {
      { nir_search_value_expression, -5 },
      false,
      false,
      false,
      nir_op_iadd,
      0, 4,
      { 6, 7 },
      -1,
   } },

   /* replace30_0 -> 0 in the cache */
   /* replace30_1 -> 1 in the cache */
   /* replace30_2_0 -> 3 in the cache */
   /* replace30_2_1 -> 4 in the cache */
   /* replace30_2_2 -> 7 in the cache */
   { .constant = {
      { nir_search_value_constant, -5 },
      nir_type_int, { 0x0 /* 0 */ },
   } },
   { .expression = {
      { nir_search_value_expression, -5 },
      false,
      false,
      false,
      nir_op_imadshl_agx,
      -1, 0,
      { 3, 4, 7, 9 },
      -1,
   } },
   /* replace30_3 -> 9 in the cache */
   { .expression = {
      { nir_search_value_expression, -5 },
      false,
      false,
      false,
      nir_op_imadshl_agx,
      -1, 0,
      { 0, 1, 10, 9 },
      -1,
   } },

   /* ('iadd', ('imul(is_used_once)', 'a', 'b'), 'c') => ('imadshl_agx', 'a', 'b', 'c', 0) */
   { .variable = {
      { nir_search_value_variable, -3 },
      0, /* a */
      false,
      nir_type_invalid,
      -1,
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
   } },
   { .variable = {
      { nir_search_value_variable, -3 },
      1, /* b */
      false,
      nir_type_invalid,
      -1,
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
   } },
   { .expression = {
      { nir_search_value_expression, -3 },
      false,
      false,
      false,
      nir_op_imul,
      1, 1,
      { 12, 13 },
      0,
   } },
   { .variable = {
      { nir_search_value_variable, -3 },
      2, /* c */
      false,
      nir_type_invalid,
      -1,
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
   } },
   { .expression = {
      { nir_search_value_expression, -3 },
      false,
      false,
      false,
      nir_op_iadd,
      0, 2,
      { 14, 15 },
      -1,
   } },

   /* replace31_0 -> 12 in the cache */
   /* replace31_1 -> 13 in the cache */
   /* replace31_2 -> 15 in the cache */
   { .constant = {
      { nir_search_value_constant, -3 },
      nir_type_int, { 0x0 /* 0 */ },
   } },
   { .expression = {
      { nir_search_value_expression, -3 },
      false,
      false,
      false,
      nir_op_imadshl_agx,
      -1, 0,
      { 12, 13, 15, 17 },
      -1,
   } },

   /* ('isub', ('imul(is_used_once)', 'a', 'b'), 'c') => ('imsubshl_agx', 'a', 'b', 'c', 0) */
   /* search32_0_0 -> 12 in the cache */
   /* search32_0_1 -> 13 in the cache */
   { .expression = {
      { nir_search_value_expression, -3 },
      false,
      false,
      false,
      nir_op_imul,
      0, 1,
      { 12, 13 },
      0,
   } },
   /* search32_1 -> 15 in the cache */
   { .expression = {
      { nir_search_value_expression, -3 },
      false,
      false,
      false,
      nir_op_isub,
      -1, 1,
      { 19, 15 },
      -1,
   } },

   /* replace32_0 -> 12 in the cache */
   /* replace32_1 -> 13 in the cache */
   /* replace32_2 -> 15 in the cache */
   /* replace32_3 -> 17 in the cache */
   { .expression = {
      { nir_search_value_expression, -3 },
      false,
      false,
      false,
      nir_op_imsubshl_agx,
      -1, 0,
      { 12, 13, 15, 17 },
      -1,
   } },

   /* ('iadd', 'a', ('ishl(is_used_once)', 'b', 1)) => ('imadshl_agx', 'a', 1, 'b', 1) */
   { .variable = {
      { nir_search_value_variable, -2 },
      0, /* a */
      false,
      nir_type_invalid,
      -1,
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
   } },
   { .variable = {
      { nir_search_value_variable, -2 },
      1, /* b */
      false,
      nir_type_invalid,
      -1,
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
   } },
   { .constant = {
      { nir_search_value_constant, 32 },
      nir_type_int, { 0x1 /* 1 */ },
   } },
   { .expression = {
      { nir_search_value_expression, -2 },
      false,
      false,
      false,
      nir_op_ishl,
      -1, 0,
      { 23, 24 },
      0,
   } },
   { .expression = {
      { nir_search_value_expression, -2 },
      false,
      false,
      false,
      nir_op_iadd,
      0, 1,
      { 22, 25 },
      -1,
   } },

   /* replace33_0 -> 22 in the cache */
   { .constant = {
      { nir_search_value_constant, -2 },
      nir_type_int, { 0x1 /* 1 */ },
   } },
   /* replace33_2 -> 23 in the cache */
   /* replace33_3 -> 27 in the cache */
   { .expression = {
      { nir_search_value_expression, -2 },
      false,
      false,
      false,
      nir_op_imadshl_agx,
      -1, 0,
      { 22, 27, 23, 27 },
      -1,
   } },

   /* ('isub', 'a', ('ishl(is_used_once)', 'b', 1)) => ('imsubshl_agx', 'a', 1, 'b', 1) */
   /* search34_0 -> 22 in the cache */
   /* search34_1_0 -> 23 in the cache */
   /* search34_1_1 -> 24 in the cache */
   /* search34_1 -> 25 in the cache */
   { .expression = {
      { nir_search_value_expression, -2 },
      false,
      false,
      false,
      nir_op_isub,
      -1, 0,
      { 22, 25 },
      -1,
   } },

   /* replace34_0 -> 22 in the cache */
   /* replace34_1 -> 27 in the cache */
   /* replace34_2 -> 23 in the cache */
   /* replace34_3 -> 27 in the cache */
   { .expression = {
      { nir_search_value_expression, -2 },
      false,
      false,
      false,
      nir_op_imsubshl_agx,
      -1, 0,
      { 22, 27, 23, 27 },
      -1,
   } },

   /* ('ineg', ('ishl(is_used_once)', 'b', 1)) => ('imsubshl_agx', 0, 1, 'b', 1) */
   { .variable = {
      { nir_search_value_variable, -1 },
      0, /* b */
      false,
      nir_type_invalid,
      -1,
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
   } },
   /* search35_0_1 -> 24 in the cache */
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_ishl,
      -1, 0,
      { 31, 24 },
      0,
   } },
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_ineg,
      -1, 0,
      { 32 },
      -1,
   } },

   { .constant = {
      { nir_search_value_constant, -1 },
      nir_type_int, { 0x0 /* 0 */ },
   } },
   { .constant = {
      { nir_search_value_constant, -1 },
      nir_type_int, { 0x1 /* 1 */ },
   } },
   /* replace35_2 -> 31 in the cache */
   /* replace35_3 -> 35 in the cache */
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_imsubshl_agx,
      -1, 0,
      { 34, 35, 31, 35 },
      -1,
   } },

   /* ('imadshl_agx', 'a', 'b', ('ishl(is_used_once)', 'c', 1), 0) => ('imadshl_agx', 'a', 'b', 'c', 1) */
   /* search36_0 -> 12 in the cache */
   /* search36_1 -> 13 in the cache */
   /* search36_2_0 -> 15 in the cache */
   /* search36_2_1 -> 24 in the cache */
   { .expression = {
      { nir_search_value_expression, -3 },
      false,
      false,
      false,
      nir_op_ishl,
      -1, 0,
      { 15, 24 },
      0,
   } },
   /* search36_3 -> 17 in the cache */
   { .expression = {
      { nir_search_value_expression, -3 },
      false,
      false,
      false,
      nir_op_imadshl_agx,
      -1, 0,
      { 12, 13, 37, 17 },
      -1,
   } },

   /* replace36_0 -> 12 in the cache */
   /* replace36_1 -> 13 in the cache */
   /* replace36_2 -> 15 in the cache */
   { .constant = {
      { nir_search_value_constant, -3 },
      nir_type_int, { 0x1 /* 1 */ },
   } },
   { .expression = {
      { nir_search_value_expression, -3 },
      false,
      false,
      false,
      nir_op_imadshl_agx,
      -1, 0,
      { 12, 13, 15, 39 },
      -1,
   } },

   /* ('imsubshl_agx', 'a', 'b', ('ishl(is_used_once)', 'c', 1), 0) => ('imsubshl_agx', 'a', 'b', 'c', 1) */
   /* search37_0 -> 12 in the cache */
   /* search37_1 -> 13 in the cache */
   /* search37_2_0 -> 15 in the cache */
   /* search37_2_1 -> 24 in the cache */
   /* search37_2 -> 37 in the cache */
   /* search37_3 -> 17 in the cache */
   { .expression = {
      { nir_search_value_expression, -3 },
      false,
      false,
      false,
      nir_op_imsubshl_agx,
      -1, 0,
      { 12, 13, 37, 17 },
      -1,
   } },

   /* replace37_0 -> 12 in the cache */
   /* replace37_1 -> 13 in the cache */
   /* replace37_2 -> 15 in the cache */
   /* replace37_3 -> 39 in the cache */
   { .expression = {
      { nir_search_value_expression, -3 },
      false,
      false,
      false,
      nir_op_imsubshl_agx,
      -1, 0,
      { 12, 13, 15, 39 },
      -1,
   } },

   /* ('imul', 'a', 3) => ('imadshl_agx', 'a', 1, 'a', 1) */
   { .variable = {
      { nir_search_value_variable, -1 },
      0, /* a */
      false,
      nir_type_invalid,
      -1,
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
   } },
   { .constant = {
      { nir_search_value_constant, -1 },
      nir_type_int, { 0x3 /* 3 */ },
   } },
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_imul,
      0, 1,
      { 43, 44 },
      -1,
   } },

   /* replace38_0 -> 43 in the cache */
   /* replace38_1 -> 35 in the cache */
   /* replace38_2 -> 43 in the cache */
   /* replace38_3 -> 35 in the cache */
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_imadshl_agx,
      -1, 0,
      { 43, 35, 43, 35 },
      -1,
   } },

   /* ('imul', 'a', -1) => ('imsubshl_agx', 'a', 1, 'a', 1) */
   /* search39_0 -> 43 in the cache */
   { .constant = {
      { nir_search_value_constant, -1 },
      nir_type_int, { -0x1 /* -1 */ },
   } },
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_imul,
      0, 1,
      { 43, 47 },
      -1,
   } },

   /* replace39_0 -> 43 in the cache */
   /* replace39_1 -> 35 in the cache */
   /* replace39_2 -> 43 in the cache */
   /* replace39_3 -> 35 in the cache */
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_imsubshl_agx,
      -1, 0,
      { 43, 35, 43, 35 },
      -1,
   } },

   /* ('ineg', ('imul(is_used_once)', 'a', 1)) => ('imsubshl_agx', 'a', 1, 'a', 1) */
   /* search40_0_0 -> 43 in the cache */
   /* search40_0_1 -> 35 in the cache */
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_imul,
      0, 1,
      { 43, 35 },
      0,
   } },
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_ineg,
      -1, 1,
      { 50 },
      -1,
   } },

   /* replace40_0 -> 43 in the cache */
   /* replace40_1 -> 35 in the cache */
   /* replace40_2 -> 43 in the cache */
   /* replace40_3 -> 35 in the cache */
   /* replace40 -> 49 in the cache */

   /* ('ishl', 'a', 1) => ('imadshl_agx', 0, 1, 'a', 1) */
   /* search41_0 -> 43 in the cache */
   /* search41_1 -> 24 in the cache */
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_ishl,
      -1, 0,
      { 43, 24 },
      -1,
   } },

   /* replace41_0 -> 34 in the cache */
   /* replace41_1 -> 35 in the cache */
   /* replace41_2 -> 43 in the cache */
   /* replace41_3 -> 35 in the cache */
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_imadshl_agx,
      -1, 0,
      { 34, 35, 43, 35 },
      -1,
   } },

   /* ('iadd', 'a', ('ishl(is_used_once)', 'b', 2)) => ('imadshl_agx', 'a', 1, 'b', 2) */
   /* search42_0 -> 22 in the cache */
   /* search42_1_0 -> 23 in the cache */
   { .constant = {
      { nir_search_value_constant, 32 },
      nir_type_int, { 0x2 /* 2 */ },
   } },
   { .expression = {
      { nir_search_value_expression, -2 },
      false,
      false,
      false,
      nir_op_ishl,
      -1, 0,
      { 23, 54 },
      0,
   } },
   { .expression = {
      { nir_search_value_expression, -2 },
      false,
      false,
      false,
      nir_op_iadd,
      0, 1,
      { 22, 55 },
      -1,
   } },

   /* replace42_0 -> 22 in the cache */
   /* replace42_1 -> 27 in the cache */
   /* replace42_2 -> 23 in the cache */
   { .constant = {
      { nir_search_value_constant, -2 },
      nir_type_int, { 0x2 /* 2 */ },
   } },
   { .expression = {
      { nir_search_value_expression, -2 },
      false,
      false,
      false,
      nir_op_imadshl_agx,
      -1, 0,
      { 22, 27, 23, 57 },
      -1,
   } },

   /* ('isub', 'a', ('ishl(is_used_once)', 'b', 2)) => ('imsubshl_agx', 'a', 1, 'b', 2) */
   /* search43_0 -> 22 in the cache */
   /* search43_1_0 -> 23 in the cache */
   /* search43_1_1 -> 54 in the cache */
   /* search43_1 -> 55 in the cache */
   { .expression = {
      { nir_search_value_expression, -2 },
      false,
      false,
      false,
      nir_op_isub,
      -1, 0,
      { 22, 55 },
      -1,
   } },

   /* replace43_0 -> 22 in the cache */
   /* replace43_1 -> 27 in the cache */
   /* replace43_2 -> 23 in the cache */
   /* replace43_3 -> 57 in the cache */
   { .expression = {
      { nir_search_value_expression, -2 },
      false,
      false,
      false,
      nir_op_imsubshl_agx,
      -1, 0,
      { 22, 27, 23, 57 },
      -1,
   } },

   /* ('ineg', ('ishl(is_used_once)', 'b', 2)) => ('imsubshl_agx', 0, 1, 'b', 2) */
   /* search44_0_0 -> 31 in the cache */
   /* search44_0_1 -> 54 in the cache */
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_ishl,
      -1, 0,
      { 31, 54 },
      0,
   } },
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_ineg,
      -1, 0,
      { 61 },
      -1,
   } },

   /* replace44_0 -> 34 in the cache */
   /* replace44_1 -> 35 in the cache */
   /* replace44_2 -> 31 in the cache */
   { .constant = {
      { nir_search_value_constant, -1 },
      nir_type_int, { 0x2 /* 2 */ },
   } },
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_imsubshl_agx,
      -1, 0,
      { 34, 35, 31, 63 },
      -1,
   } },

   /* ('imadshl_agx', 'a', 'b', ('ishl(is_used_once)', 'c', 2), 0) => ('imadshl_agx', 'a', 'b', 'c', 2) */
   /* search45_0 -> 12 in the cache */
   /* search45_1 -> 13 in the cache */
   /* search45_2_0 -> 15 in the cache */
   /* search45_2_1 -> 54 in the cache */
   { .expression = {
      { nir_search_value_expression, -3 },
      false,
      false,
      false,
      nir_op_ishl,
      -1, 0,
      { 15, 54 },
      0,
   } },
   /* search45_3 -> 17 in the cache */
   { .expression = {
      { nir_search_value_expression, -3 },
      false,
      false,
      false,
      nir_op_imadshl_agx,
      -1, 0,
      { 12, 13, 65, 17 },
      -1,
   } },

   /* replace45_0 -> 12 in the cache */
   /* replace45_1 -> 13 in the cache */
   /* replace45_2 -> 15 in the cache */
   { .constant = {
      { nir_search_value_constant, -3 },
      nir_type_int, { 0x2 /* 2 */ },
   } },
   { .expression = {
      { nir_search_value_expression, -3 },
      false,
      false,
      false,
      nir_op_imadshl_agx,
      -1, 0,
      { 12, 13, 15, 67 },
      -1,
   } },

   /* ('imsubshl_agx', 'a', 'b', ('ishl(is_used_once)', 'c', 2), 0) => ('imsubshl_agx', 'a', 'b', 'c', 2) */
   /* search46_0 -> 12 in the cache */
   /* search46_1 -> 13 in the cache */
   /* search46_2_0 -> 15 in the cache */
   /* search46_2_1 -> 54 in the cache */
   /* search46_2 -> 65 in the cache */
   /* search46_3 -> 17 in the cache */
   { .expression = {
      { nir_search_value_expression, -3 },
      false,
      false,
      false,
      nir_op_imsubshl_agx,
      -1, 0,
      { 12, 13, 65, 17 },
      -1,
   } },

   /* replace46_0 -> 12 in the cache */
   /* replace46_1 -> 13 in the cache */
   /* replace46_2 -> 15 in the cache */
   /* replace46_3 -> 67 in the cache */
   { .expression = {
      { nir_search_value_expression, -3 },
      false,
      false,
      false,
      nir_op_imsubshl_agx,
      -1, 0,
      { 12, 13, 15, 67 },
      -1,
   } },

   /* ('imul', 'a', 5) => ('imadshl_agx', 'a', 1, 'a', 2) */
   /* search47_0 -> 43 in the cache */
   { .constant = {
      { nir_search_value_constant, -1 },
      nir_type_int, { 0x5 /* 5 */ },
   } },
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_imul,
      0, 1,
      { 43, 71 },
      -1,
   } },

   /* replace47_0 -> 43 in the cache */
   /* replace47_1 -> 35 in the cache */
   /* replace47_2 -> 43 in the cache */
   /* replace47_3 -> 63 in the cache */
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_imadshl_agx,
      -1, 0,
      { 43, 35, 43, 63 },
      -1,
   } },

   /* ('imul', 'a', -3) => ('imsubshl_agx', 'a', 1, 'a', 2) */
   /* search48_0 -> 43 in the cache */
   { .constant = {
      { nir_search_value_constant, -1 },
      nir_type_int, { -0x3 /* -3 */ },
   } },
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_imul,
      0, 1,
      { 43, 74 },
      -1,
   } },

   /* replace48_0 -> 43 in the cache */
   /* replace48_1 -> 35 in the cache */
   /* replace48_2 -> 43 in the cache */
   /* replace48_3 -> 63 in the cache */
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_imsubshl_agx,
      -1, 0,
      { 43, 35, 43, 63 },
      -1,
   } },

   /* ('ineg', ('imul(is_used_once)', 'a', 3)) => ('imsubshl_agx', 'a', 1, 'a', 2) */
   /* search49_0_0 -> 43 in the cache */
   /* search49_0_1 -> 44 in the cache */
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_imul,
      0, 1,
      { 43, 44 },
      0,
   } },
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_ineg,
      -1, 1,
      { 77 },
      -1,
   } },

   /* replace49_0 -> 43 in the cache */
   /* replace49_1 -> 35 in the cache */
   /* replace49_2 -> 43 in the cache */
   /* replace49_3 -> 63 in the cache */
   /* replace49 -> 76 in the cache */

   /* ('ishl', 'a', 2) => ('imadshl_agx', 0, 1, 'a', 2) */
   /* search50_0 -> 43 in the cache */
   /* search50_1 -> 54 in the cache */
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_ishl,
      -1, 0,
      { 43, 54 },
      -1,
   } },

   /* replace50_0 -> 34 in the cache */
   /* replace50_1 -> 35 in the cache */
   /* replace50_2 -> 43 in the cache */
   /* replace50_3 -> 63 in the cache */
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_imadshl_agx,
      -1, 0,
      { 34, 35, 43, 63 },
      -1,
   } },

   /* ('iadd', 'a', ('ishl(is_used_once)', 'b', 3)) => ('imadshl_agx', 'a', 1, 'b', 3) */
   /* search51_0 -> 22 in the cache */
   /* search51_1_0 -> 23 in the cache */
   { .constant = {
      { nir_search_value_constant, 32 },
      nir_type_int, { 0x3 /* 3 */ },
   } },
   { .expression = {
      { nir_search_value_expression, -2 },
      false,
      false,
      false,
      nir_op_ishl,
      -1, 0,
      { 23, 81 },
      0,
   } },
   { .expression = {
      { nir_search_value_expression, -2 },
      false,
      false,
      false,
      nir_op_iadd,
      0, 1,
      { 22, 82 },
      -1,
   } },

   /* replace51_0 -> 22 in the cache */
   /* replace51_1 -> 27 in the cache */
   /* replace51_2 -> 23 in the cache */
   { .constant = {
      { nir_search_value_constant, -2 },
      nir_type_int, { 0x3 /* 3 */ },
   } },
   { .expression = {
      { nir_search_value_expression, -2 },
      false,
      false,
      false,
      nir_op_imadshl_agx,
      -1, 0,
      { 22, 27, 23, 84 },
      -1,
   } },

   /* ('isub', 'a', ('ishl(is_used_once)', 'b', 3)) => ('imsubshl_agx', 'a', 1, 'b', 3) */
   /* search52_0 -> 22 in the cache */
   /* search52_1_0 -> 23 in the cache */
   /* search52_1_1 -> 81 in the cache */
   /* search52_1 -> 82 in the cache */
   { .expression = {
      { nir_search_value_expression, -2 },
      false,
      false,
      false,
      nir_op_isub,
      -1, 0,
      { 22, 82 },
      -1,
   } },

   /* replace52_0 -> 22 in the cache */
   /* replace52_1 -> 27 in the cache */
   /* replace52_2 -> 23 in the cache */
   /* replace52_3 -> 84 in the cache */
   { .expression = {
      { nir_search_value_expression, -2 },
      false,
      false,
      false,
      nir_op_imsubshl_agx,
      -1, 0,
      { 22, 27, 23, 84 },
      -1,
   } },

   /* ('ineg', ('ishl(is_used_once)', 'b', 3)) => ('imsubshl_agx', 0, 1, 'b', 3) */
   /* search53_0_0 -> 31 in the cache */
   /* search53_0_1 -> 81 in the cache */
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_ishl,
      -1, 0,
      { 31, 81 },
      0,
   } },
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_ineg,
      -1, 0,
      { 88 },
      -1,
   } },

   /* replace53_0 -> 34 in the cache */
   /* replace53_1 -> 35 in the cache */
   /* replace53_2 -> 31 in the cache */
   /* replace53_3 -> 44 in the cache */
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_imsubshl_agx,
      -1, 0,
      { 34, 35, 31, 44 },
      -1,
   } },

   /* ('imadshl_agx', 'a', 'b', ('ishl(is_used_once)', 'c', 3), 0) => ('imadshl_agx', 'a', 'b', 'c', 3) */
   /* search54_0 -> 12 in the cache */
   /* search54_1 -> 13 in the cache */
   /* search54_2_0 -> 15 in the cache */
   /* search54_2_1 -> 81 in the cache */
   { .expression = {
      { nir_search_value_expression, -3 },
      false,
      false,
      false,
      nir_op_ishl,
      -1, 0,
      { 15, 81 },
      0,
   } },
   /* search54_3 -> 17 in the cache */
   { .expression = {
      { nir_search_value_expression, -3 },
      false,
      false,
      false,
      nir_op_imadshl_agx,
      -1, 0,
      { 12, 13, 91, 17 },
      -1,
   } },

   /* replace54_0 -> 12 in the cache */
   /* replace54_1 -> 13 in the cache */
   /* replace54_2 -> 15 in the cache */
   { .constant = {
      { nir_search_value_constant, -3 },
      nir_type_int, { 0x3 /* 3 */ },
   } },
   { .expression = {
      { nir_search_value_expression, -3 },
      false,
      false,
      false,
      nir_op_imadshl_agx,
      -1, 0,
      { 12, 13, 15, 93 },
      -1,
   } },

   /* ('imsubshl_agx', 'a', 'b', ('ishl(is_used_once)', 'c', 3), 0) => ('imsubshl_agx', 'a', 'b', 'c', 3) */
   /* search55_0 -> 12 in the cache */
   /* search55_1 -> 13 in the cache */
   /* search55_2_0 -> 15 in the cache */
   /* search55_2_1 -> 81 in the cache */
   /* search55_2 -> 91 in the cache */
   /* search55_3 -> 17 in the cache */
   { .expression = {
      { nir_search_value_expression, -3 },
      false,
      false,
      false,
      nir_op_imsubshl_agx,
      -1, 0,
      { 12, 13, 91, 17 },
      -1,
   } },

   /* replace55_0 -> 12 in the cache */
   /* replace55_1 -> 13 in the cache */
   /* replace55_2 -> 15 in the cache */
   /* replace55_3 -> 93 in the cache */
   { .expression = {
      { nir_search_value_expression, -3 },
      false,
      false,
      false,
      nir_op_imsubshl_agx,
      -1, 0,
      { 12, 13, 15, 93 },
      -1,
   } },

   /* ('imul', 'a', 9) => ('imadshl_agx', 'a', 1, 'a', 3) */
   /* search56_0 -> 43 in the cache */
   { .constant = {
      { nir_search_value_constant, -1 },
      nir_type_int, { 0x9 /* 9 */ },
   } },
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_imul,
      0, 1,
      { 43, 97 },
      -1,
   } },

   /* replace56_0 -> 43 in the cache */
   /* replace56_1 -> 35 in the cache */
   /* replace56_2 -> 43 in the cache */
   /* replace56_3 -> 44 in the cache */
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_imadshl_agx,
      -1, 0,
      { 43, 35, 43, 44 },
      -1,
   } },

   /* ('imul', 'a', -7) => ('imsubshl_agx', 'a', 1, 'a', 3) */
   /* search57_0 -> 43 in the cache */
   { .constant = {
      { nir_search_value_constant, -1 },
      nir_type_int, { -0x7 /* -7 */ },
   } },
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_imul,
      0, 1,
      { 43, 100 },
      -1,
   } },

   /* replace57_0 -> 43 in the cache */
   /* replace57_1 -> 35 in the cache */
   /* replace57_2 -> 43 in the cache */
   /* replace57_3 -> 44 in the cache */
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_imsubshl_agx,
      -1, 0,
      { 43, 35, 43, 44 },
      -1,
   } },

   /* ('ineg', ('imul(is_used_once)', 'a', 7)) => ('imsubshl_agx', 'a', 1, 'a', 3) */
   /* search58_0_0 -> 43 in the cache */
   { .constant = {
      { nir_search_value_constant, -1 },
      nir_type_int, { 0x7 /* 7 */ },
   } },
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_imul,
      0, 1,
      { 43, 103 },
      0,
   } },
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_ineg,
      -1, 1,
      { 104 },
      -1,
   } },

   /* replace58_0 -> 43 in the cache */
   /* replace58_1 -> 35 in the cache */
   /* replace58_2 -> 43 in the cache */
   /* replace58_3 -> 44 in the cache */
   /* replace58 -> 102 in the cache */

   /* ('ishl', 'a', 3) => ('imadshl_agx', 0, 1, 'a', 3) */
   /* search59_0 -> 43 in the cache */
   /* search59_1 -> 81 in the cache */
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_ishl,
      -1, 0,
      { 43, 81 },
      -1,
   } },

   /* replace59_0 -> 34 in the cache */
   /* replace59_1 -> 35 in the cache */
   /* replace59_2 -> 43 in the cache */
   /* replace59_3 -> 44 in the cache */
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_imadshl_agx,
      -1, 0,
      { 34, 35, 43, 44 },
      -1,
   } },

   /* ('iadd', 'a', ('ishl(is_used_once)', 'b', 4)) => ('imadshl_agx', 'a', 1, 'b', 4) */
   /* search60_0 -> 22 in the cache */
   /* search60_1_0 -> 23 in the cache */
   { .constant = {
      { nir_search_value_constant, 32 },
      nir_type_int, { 0x4 /* 4 */ },
   } },
   { .expression = {
      { nir_search_value_expression, -2 },
      false,
      false,
      false,
      nir_op_ishl,
      -1, 0,
      { 23, 108 },
      0,
   } },
   { .expression = {
      { nir_search_value_expression, -2 },
      false,
      false,
      false,
      nir_op_iadd,
      0, 1,
      { 22, 109 },
      -1,
   } },

   /* replace60_0 -> 22 in the cache */
   /* replace60_1 -> 27 in the cache */
   /* replace60_2 -> 23 in the cache */
   { .constant = {
      { nir_search_value_constant, -2 },
      nir_type_int, { 0x4 /* 4 */ },
   } },
   { .expression = {
      { nir_search_value_expression, -2 },
      false,
      false,
      false,
      nir_op_imadshl_agx,
      -1, 0,
      { 22, 27, 23, 111 },
      -1,
   } },

   /* ('isub', 'a', ('ishl(is_used_once)', 'b', 4)) => ('imsubshl_agx', 'a', 1, 'b', 4) */
   /* search61_0 -> 22 in the cache */
   /* search61_1_0 -> 23 in the cache */
   /* search61_1_1 -> 108 in the cache */
   /* search61_1 -> 109 in the cache */
   { .expression = {
      { nir_search_value_expression, -2 },
      false,
      false,
      false,
      nir_op_isub,
      -1, 0,
      { 22, 109 },
      -1,
   } },

   /* replace61_0 -> 22 in the cache */
   /* replace61_1 -> 27 in the cache */
   /* replace61_2 -> 23 in the cache */
   /* replace61_3 -> 111 in the cache */
   { .expression = {
      { nir_search_value_expression, -2 },
      false,
      false,
      false,
      nir_op_imsubshl_agx,
      -1, 0,
      { 22, 27, 23, 111 },
      -1,
   } },

   /* ('ineg', ('ishl(is_used_once)', 'b', 4)) => ('imsubshl_agx', 0, 1, 'b', 4) */
   /* search62_0_0 -> 31 in the cache */
   /* search62_0_1 -> 108 in the cache */
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_ishl,
      -1, 0,
      { 31, 108 },
      0,
   } },
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_ineg,
      -1, 0,
      { 115 },
      -1,
   } },

   /* replace62_0 -> 34 in the cache */
   /* replace62_1 -> 35 in the cache */
   /* replace62_2 -> 31 in the cache */
   { .constant = {
      { nir_search_value_constant, -1 },
      nir_type_int, { 0x4 /* 4 */ },
   } },
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_imsubshl_agx,
      -1, 0,
      { 34, 35, 31, 117 },
      -1,
   } },

   /* ('imadshl_agx', 'a', 'b', ('ishl(is_used_once)', 'c', 4), 0) => ('imadshl_agx', 'a', 'b', 'c', 4) */
   /* search63_0 -> 12 in the cache */
   /* search63_1 -> 13 in the cache */
   /* search63_2_0 -> 15 in the cache */
   /* search63_2_1 -> 108 in the cache */
   { .expression = {
      { nir_search_value_expression, -3 },
      false,
      false,
      false,
      nir_op_ishl,
      -1, 0,
      { 15, 108 },
      0,
   } },
   /* search63_3 -> 17 in the cache */
   { .expression = {
      { nir_search_value_expression, -3 },
      false,
      false,
      false,
      nir_op_imadshl_agx,
      -1, 0,
      { 12, 13, 119, 17 },
      -1,
   } },

   /* replace63_0 -> 12 in the cache */
   /* replace63_1 -> 13 in the cache */
   /* replace63_2 -> 15 in the cache */
   { .constant = {
      { nir_search_value_constant, -3 },
      nir_type_int, { 0x4 /* 4 */ },
   } },
   { .expression = {
      { nir_search_value_expression, -3 },
      false,
      false,
      false,
      nir_op_imadshl_agx,
      -1, 0,
      { 12, 13, 15, 121 },
      -1,
   } },

   /* ('imsubshl_agx', 'a', 'b', ('ishl(is_used_once)', 'c', 4), 0) => ('imsubshl_agx', 'a', 'b', 'c', 4) */
   /* search64_0 -> 12 in the cache */
   /* search64_1 -> 13 in the cache */
   /* search64_2_0 -> 15 in the cache */
   /* search64_2_1 -> 108 in the cache */
   /* search64_2 -> 119 in the cache */
   /* search64_3 -> 17 in the cache */
   { .expression = {
      { nir_search_value_expression, -3 },
      false,
      false,
      false,
      nir_op_imsubshl_agx,
      -1, 0,
      { 12, 13, 119, 17 },
      -1,
   } },

   /* replace64_0 -> 12 in the cache */
   /* replace64_1 -> 13 in the cache */
   /* replace64_2 -> 15 in the cache */
   /* replace64_3 -> 121 in the cache */
   { .expression = {
      { nir_search_value_expression, -3 },
      false,
      false,
      false,
      nir_op_imsubshl_agx,
      -1, 0,
      { 12, 13, 15, 121 },
      -1,
   } },

   /* ('imul', 'a', 17) => ('imadshl_agx', 'a', 1, 'a', 4) */
   /* search65_0 -> 43 in the cache */
   { .constant = {
      { nir_search_value_constant, -1 },
      nir_type_int, { 0x11 /* 17 */ },
   } },
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_imul,
      0, 1,
      { 43, 125 },
      -1,
   } },

   /* replace65_0 -> 43 in the cache */
   /* replace65_1 -> 35 in the cache */
   /* replace65_2 -> 43 in the cache */
   /* replace65_3 -> 117 in the cache */
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_imadshl_agx,
      -1, 0,
      { 43, 35, 43, 117 },
      -1,
   } },

   /* ('imul', 'a', -15) => ('imsubshl_agx', 'a', 1, 'a', 4) */
   /* search66_0 -> 43 in the cache */
   { .constant = {
      { nir_search_value_constant, -1 },
      nir_type_int, { -0xf /* -15 */ },
   } },
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_imul,
      0, 1,
      { 43, 128 },
      -1,
   } },

   /* replace66_0 -> 43 in the cache */
   /* replace66_1 -> 35 in the cache */
   /* replace66_2 -> 43 in the cache */
   /* replace66_3 -> 117 in the cache */
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_imsubshl_agx,
      -1, 0,
      { 43, 35, 43, 117 },
      -1,
   } },

   /* ('ineg', ('imul(is_used_once)', 'a', 15)) => ('imsubshl_agx', 'a', 1, 'a', 4) */
   /* search67_0_0 -> 43 in the cache */
   { .constant = {
      { nir_search_value_constant, -1 },
      nir_type_int, { 0xf /* 15 */ },
   } },
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_imul,
      0, 1,
      { 43, 131 },
      0,
   } },
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_ineg,
      -1, 1,
      { 132 },
      -1,
   } },

   /* replace67_0 -> 43 in the cache */
   /* replace67_1 -> 35 in the cache */
   /* replace67_2 -> 43 in the cache */
   /* replace67_3 -> 117 in the cache */
   /* replace67 -> 130 in the cache */

   /* ('ishl', 'a', 4) => ('imadshl_agx', 0, 1, 'a', 4) */
   /* search68_0 -> 43 in the cache */
   /* search68_1 -> 108 in the cache */
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_ishl,
      -1, 0,
      { 43, 108 },
      -1,
   } },

   /* replace68_0 -> 34 in the cache */
   /* replace68_1 -> 35 in the cache */
   /* replace68_2 -> 43 in the cache */
   /* replace68_3 -> 117 in the cache */
   { .expression = {
      { nir_search_value_expression, -1 },
      false,
      false,
      false,
      nir_op_imadshl_agx,
      -1, 0,
      { 34, 35, 43, 117 },
      -1,
   } },

};

static const nir_search_expression_cond agx_nir_fuse_algebraic_late_expression_cond[] = {
   (is_used_once),
};


static const struct transform agx_nir_fuse_algebraic_late_transforms[] = {
   { ~0, ~0, ~0 }, /* Sentinel */

   { 45, 46, 0 },
   { 48, 49, 0 },
   { 72, 73, 0 },
   { 75, 76, 0 },
   { 98, 99, 0 },
   { 101, 102, 0 },
   { 126, 127, 0 },
   { 129, 130, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

   { 52, 53, 0 },
   { 79, 80, 0 },
   { 106, 107, 0 },
   { 134, 135, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

   { 16, 18, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

   { 26, 28, 0 },
   { 56, 58, 0 },
   { 83, 85, 0 },
   { 110, 112, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

   { 16, 18, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

   { 16, 18, 0 },
   { 26, 28, 0 },
   { 56, 58, 0 },
   { 83, 85, 0 },
   { 110, 112, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

   { 29, 30, 0 },
   { 59, 60, 0 },
   { 86, 87, 0 },
   { 113, 114, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

   { 20, 21, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

   { 20, 21, 0 },
   { 29, 30, 0 },
   { 59, 60, 0 },
   { 86, 87, 0 },
   { 113, 114, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

   { 51, 49, 0 },
   { 78, 76, 0 },
   { 105, 102, 0 },
   { 133, 130, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

   { 33, 36, 0 },
   { 62, 64, 0 },
   { 89, 90, 0 },
   { 116, 118, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

   { 38, 40, 0 },
   { 66, 68, 0 },
   { 92, 94, 0 },
   { 120, 122, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

   { 41, 42, 0 },
   { 69, 70, 0 },
   { 95, 96, 0 },
   { 123, 124, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

   { 8, 11, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

   { 8, 11, 0 },
   { 16, 18, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

   { 8, 11, 0 },
   { 26, 28, 0 },
   { 56, 58, 0 },
   { 83, 85, 0 },
   { 110, 112, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

};

static const struct per_op_table agx_nir_fuse_algebraic_late_pass_op_table[nir_num_search_ops] = {
   [nir_op_iadd] = {
      .filter = (const uint16_t []) {
         0,
         0,
         1,
         1,
         2,
         0,
         0,
         3,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
      },
      
      .num_filtered_states = 4,
      .table = (const uint16_t []) {
      
         0,
         5,
         6,
         16,
         5,
         7,
         8,
         17,
         6,
         8,
         6,
         18,
         16,
         17,
         18,
         16,
      },
   },
   [nir_op_imul] = {
      .filter = (const uint16_t []) {
         0,
         1,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
      },
      
      .num_filtered_states = 2,
      .table = (const uint16_t []) {
      
         2,
         3,
         3,
         3,
      },
   },
   [nir_op_isub] = {
      .filter = (const uint16_t []) {
         0,
         0,
         1,
         1,
         2,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
      },
      
      .num_filtered_states = 3,
      .table = (const uint16_t []) {
      
         0,
         0,
         9,
         10,
         10,
         11,
         0,
         0,
         9,
      },
   },
   [nir_op_ishl] = {
      .filter = (const uint16_t []) {
         0,
         1,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
      },
      
      .num_filtered_states = 2,
      .table = (const uint16_t []) {
      
         0,
         4,
         0,
         4,
      },
   },
   [nir_op_ineg] = {
      .filter = (const uint16_t []) {
         0,
         0,
         0,
         1,
         2,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
      },
      
      .num_filtered_states = 3,
      .table = (const uint16_t []) {
      
         0,
         12,
         13,
      },
   },
   [nir_op_imadshl_agx] = {
      .filter = (const uint16_t []) {
         0,
         1,
         0,
         0,
         2,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
      },
      
      .num_filtered_states = 3,
      .table = (const uint16_t []) {
      
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         14,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         14,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         14,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         14,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         14,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         14,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         14,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         14,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         14,
         0,
      },
   },
   [nir_op_imsubshl_agx] = {
      .filter = (const uint16_t []) {
         0,
         1,
         0,
         0,
         2,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
      },
      
      .num_filtered_states = 3,
      .table = (const uint16_t []) {
      
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         15,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         15,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         15,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         15,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         15,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         15,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         15,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         15,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         0,
         15,
         0,
      },
   },
};

/* Mapping from state index to offset in transforms (0 being no transforms) */
static const uint16_t agx_nir_fuse_algebraic_late_transform_offsets[] = {
   0,
   0,
   0,
   1,
   10,
   15,
   17,
   22,
   24,
   30,
   35,
   37,
   43,
   48,
   53,
   58,
   63,
   65,
   68,
};

static const nir_algebraic_table agx_nir_fuse_algebraic_late_table = {
   .transforms = agx_nir_fuse_algebraic_late_transforms,
   .transform_offsets = agx_nir_fuse_algebraic_late_transform_offsets,
   .pass_op_table = agx_nir_fuse_algebraic_late_pass_op_table,
   .values = agx_nir_fuse_algebraic_late_values,
   .expression_cond = agx_nir_fuse_algebraic_late_expression_cond,
   .variable_cond = NULL,
};

bool
agx_nir_fuse_algebraic_late(nir_shader *shader)
{
   bool progress = false;
   bool condition_flags[1];
   const nir_shader_compiler_options *options = shader->options;
   const shader_info *info = &shader->info;
   (void) options;
   (void) info;

   STATIC_ASSERT(136 == ARRAY_SIZE(agx_nir_fuse_algebraic_late_values));
   condition_flags[0] = true;

   nir_foreach_function_impl(impl, shader) {
     progress |= nir_algebraic_impl(impl, condition_flags, &agx_nir_fuse_algebraic_late_table);
   }

   return progress;
}


#include "nir.h"
#include "nir_builder.h"
#include "nir_search.h"
#include "nir_search_helpers.h"

/* What follows is NIR algebraic transform code for the following 1
 * transforms:
 *    ('ixor', ('bcsel', 'a', '#b', '#c'), '#d') => ('bcsel', 'a', ('ixor', 'b', 'd'), ('ixor', 'c', 'd'))
 */


static const nir_search_value_union agx_nir_opt_ixor_bcsel_values[] = {
   /* ('ixor', ('bcsel', 'a', '#b', '#c'), '#d') => ('bcsel', 'a', ('ixor', 'b', 'd'), ('ixor', 'c', 'd')) */
   { .variable = {
      { nir_search_value_variable, 1 },
      0, /* a */
      false,
      nir_type_invalid,
      -1,
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
   } },
   { .variable = {
      { nir_search_value_variable, -4 },
      1, /* b */
      true,
      nir_type_invalid,
      -1,
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
   } },
   { .variable = {
      { nir_search_value_variable, -4 },
      2, /* c */
      true,
      nir_type_invalid,
      -1,
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
   } },
   { .expression = {
      { nir_search_value_expression, -4 },
      false,
      false,
      false,
      nir_op_bcsel,
      -1, 0,
      { 0, 1, 2 },
      -1,
   } },
   { .variable = {
      { nir_search_value_variable, -4 },
      3, /* d */
      true,
      nir_type_invalid,
      -1,
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
   } },
   { .expression = {
      { nir_search_value_expression, -4 },
      false,
      false,
      false,
      nir_op_ixor,
      0, 1,
      { 3, 4 },
      -1,
   } },

   /* replace69_0 -> 0 in the cache */
   { .variable = {
      { nir_search_value_variable, -4 },
      1, /* b */
      false,
      nir_type_invalid,
      -1,
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
   } },
   { .variable = {
      { nir_search_value_variable, -4 },
      3, /* d */
      false,
      nir_type_invalid,
      -1,
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
   } },
   { .expression = {
      { nir_search_value_expression, -4 },
      false,
      false,
      false,
      nir_op_ixor,
      0, 1,
      { 6, 7 },
      -1,
   } },
   { .variable = {
      { nir_search_value_variable, -4 },
      2, /* c */
      false,
      nir_type_invalid,
      -1,
      {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
   } },
   /* replace69_2_1 -> 7 in the cache */
   { .expression = {
      { nir_search_value_expression, -4 },
      false,
      false,
      false,
      nir_op_ixor,
      1, 1,
      { 9, 7 },
      -1,
   } },
   { .expression = {
      { nir_search_value_expression, -4 },
      false,
      false,
      false,
      nir_op_bcsel,
      -1, 2,
      { 0, 8, 10 },
      -1,
   } },

};



static const struct transform agx_nir_opt_ixor_bcsel_transforms[] = {
   { ~0, ~0, ~0 }, /* Sentinel */

   { 5, 11, 0 },
   { ~0, ~0, ~0 }, /* Sentinel */

};

static const struct per_op_table agx_nir_opt_ixor_bcsel_pass_op_table[nir_num_search_ops] = {
   [nir_op_ixor] = {
      .filter = (const uint16_t []) {
         0,
         1,
         2,
         0,
      },
      
      .num_filtered_states = 3,
      .table = (const uint16_t []) {
      
         0,
         0,
         0,
         0,
         0,
         3,
         0,
         3,
         0,
      },
   },
   [nir_op_bcsel] = {
      .filter = (const uint16_t []) {
         0,
         1,
         0,
         0,
      },
      
      .num_filtered_states = 2,
      .table = (const uint16_t []) {
      
         0,
         0,
         0,
         2,
         0,
         0,
         0,
         2,
      },
   },
};

/* Mapping from state index to offset in transforms (0 being no transforms) */
static const uint16_t agx_nir_opt_ixor_bcsel_transform_offsets[] = {
   0,
   0,
   0,
   1,
};

static const nir_algebraic_table agx_nir_opt_ixor_bcsel_table = {
   .transforms = agx_nir_opt_ixor_bcsel_transforms,
   .transform_offsets = agx_nir_opt_ixor_bcsel_transform_offsets,
   .pass_op_table = agx_nir_opt_ixor_bcsel_pass_op_table,
   .values = agx_nir_opt_ixor_bcsel_values,
   .expression_cond = NULL,
   .variable_cond = NULL,
};

bool
agx_nir_opt_ixor_bcsel(nir_shader *shader)
{
   bool progress = false;
   bool condition_flags[1];
   const nir_shader_compiler_options *options = shader->options;
   const shader_info *info = &shader->info;
   (void) options;
   (void) info;

   STATIC_ASSERT(12 == ARRAY_SIZE(agx_nir_opt_ixor_bcsel_values));
   condition_flags[0] = true;

   nir_foreach_function_impl(impl, shader) {
     progress |= nir_algebraic_impl(impl, condition_flags, &agx_nir_opt_ixor_bcsel_table);
   }

   return progress;
}

