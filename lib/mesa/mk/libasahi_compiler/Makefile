#	$OpenBSD: Makefile,v 1.8 2024/04/02 10:42:13 jsg Exp $

LIB=	asahi_compiler

NOPROFILE=

SRCS=	agx_compile.c \
		agx_dce.c \
		agx_liveness.c \
		agx_insert_waits.c \
		agx_nir_fence_images.c \
		agx_nir_lower_address.c \
		agx_nir_lower_frag_sidefx.c \
		agx_nir_lower_sample_mask.c \
		agx_nir_lower_discard_zs_emit.c \
		agx_nir_lower_texture.c \
		agx_nir_lower_interpolation.c \
		agx_nir_lower_layer.c \
		agx_nir_lower_load_mask.c \
		agx_nir_lower_shared_bitsize.c \
		agx_nir_opt_preamble.c \
		agx_lower_64bit.c \
		agx_lower_parallel_copy.c \
		agx_lower_pseudo.c \
		agx_lower_uniform_sources.c \
		agx_pack.c \
		agx_performance.c \
		agx_print.c \
		agx_pressure_schedule.c \
		agx_ir.c \
		agx_opt_cse.c \
		agx_opt_break_if.c \
		agx_opt_empty_else.c \
		agx_opt_jmp_none.c \
		agx_optimizer.c \
		agx_register_allocate.c \
		agx_validate.c \
		agx_opcodes.c \
		agx_nir_algebraic.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS} ${NO_OVERRIDE_INIT_ARGS}
CXXFLAGS+=	${CXX_VIS_ARGS}
CPPFLAGS+=  -DGALLIUM_ASAHI \
		-I${MESA_SRC}/src/asahi \
		-I${MESA_SRC}/src/asahi/lib \
		-I${MESA_SRC}/src/asahi/layout \
		-I${MESA_SRC}/src/asahi/compiler \
		-I${MESA_SRC}/src/mesa \
		-I${MESA_SRC}/src/gallium/include \
		-I${MESA_SRC}/src/util \
		-I${MESA_SRC}/src/compiler \
		-I${MESA_SRC}/src/compiler/nir \
		-I${MESA_SRC}/generated/src \
		-I${MESA_SRC}/generated/src/compiler \
		-I${MESA_SRC}/generated/src/compiler/nir \
		-I${MESA_SRC}/generated/src/asahi

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/asahi/compiler
