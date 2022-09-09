MODULE := engines/raunes

MODULE_OBJS = \
	raunes.o \
	metaengine.o \
	graphics.o \
	datfile.o \
	font.o \
	sound.o \
	logic.o \
	logic_v3.o \
	logic_v2de.o \
	logic_v2en.o

# This module can be built as a plugin
ifeq ($(ENABLE_RAUNES), DYNAMIC_PLUGIN)
PLUGIN := 1
endif

# Include common rules
include $(srcdir)/rules.mk

# Detection objects
DETECT_OBJS += $(MODULE)/detection.o
