MODULE := engines/orlando

MODULE_OBJS = \
	orlando.o \
	detection.o \
	debugger.o \
	pak_archive.o \
	graphics.o \
	resource.o \
	main_menu.o \
	font.o \
	mouse.o \
	sound.o \
	libgsm.o \
	flx_anim.o \
	avx_video.o \
	text_parser.o \
	scene.o \
	animation.o \
	element.o \
	area.o

# This module can be built as a plugin
ifeq ($(ENABLE_ORLANDO), DYNAMIC_PLUGIN)
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk
