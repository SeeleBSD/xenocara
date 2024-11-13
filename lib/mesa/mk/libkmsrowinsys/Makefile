#	$OpenBSD: Makefile,v 1.3 2021/07/22 11:10:09 jsg Exp $

LIB=	kmsrowinsys

NOPROFILE=

SRCS=	kmsro_drm_winsys.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS}
CPPFLAGS+=	-DGALLIUM_KMSRO \
		-I${MESA_SRC}/src/gallium/include \
		-I${MESA_SRC}/src/gallium/auxiliary \
		-I${MESA_SRC}/src/gallium/winsys \
		-I${MESA_SRC}/src/gallium/winsys/kmsro/drm \
		-I${MESA_SRC}/src/gallium/drivers \
		-I${MESA_SRC}/src/loader

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/gallium/winsys/kmsro/drm
.PATH: ${MESA_SRC}/src/gallium/drivers
