#	$OpenBSD: Makefile,v 1.5 2024/04/02 10:42:13 jsg Exp $

LIB=	asahi_lib

NOPROFILE=

SRCS=	agx_bo.c \
		agx_border.c \
		agx_device.c \
		agx_formats.c \
		agx_meta.c \
		agx_nir_lower_alpha.c \
		agx_nir_lower_msaa.c \
		agx_nir_lower_sample_intrinsics.c \
		agx_nir_lower_tilebuffer.c \
		agx_nir_lower_vbo.c \
		agx_nir_predicate_layer_id.c \
		agx_tilebuffer.c \
		agx_ppp.h \
		agx_pack.h \
		pool.c \
		

# CFLAGS+= -mclflushopt

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS} ${NO_OVERRIDE_INIT_ARGS}
CPPFLAGS+=	-DGALLIUM_ASAHI \
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

.PATH: ${MESA_SRC}/src/asahi/lib
