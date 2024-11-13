#	$OpenBSD: Makefile,v 1.3 2021/07/22 11:10:09 jsg Exp $

LIB=	asahiwinsys

NOPROFILE=

SRCS=	asahi_drm_winsys.c

.include "../Makefile.inc"

CFLAGS+=	${C_VIS_ARGS}
CPPFLAGS+=	-DGALLIUM_ASAHI \
		-I${MESA_SRC}/src/gallium/include \
		-I${MESA_SRC}/src/gallium/auxiliary \
		-I${MESA_SRC}/src/gallium/winsys/asahi/drm \
		-I${MESA_SRC}/src/asahi/lib \
		-I${MESA_SRC}/src/asahi/layout \
		-I${MESA_SRC}/src/asahi/compiler \
		-I${MESA_SRC}/src/gallium/drivers \
		-I${MESA_SRC}/generated/src/asahi

install:

obj: _xenocara_obj

.include <bsd.lib.mk>
.include <bsd.xorg.mk>

.PATH: ${MESA_SRC}/src/gallium/winsys/asahi/drm
.PATH: ${MESA_SRC}/src/gallium/drivers
