# Thanks to Job Vranish
# (https://spin.atomicobject.com/2016/08/26/makefile-c-projects/)
TARGET_EXEC := unworthy-space-patrol

BUILD_DIR_BASE = ./build
SRC_DIRS := ./src

ifndef mode
	mode = release
endif

MAKEFLAGS += "j $(nproc)"

CFLAGS = -g -Wall -std=c89 -pedantic -Wextra -Wconversion -Wcast-align=strict \
		 -Wcast-qual -Wfloat-equal -Wundef -Wshadow -Wpointer-arith \
		 -Wstrict-prototypes -Wstrict-overflow=5 -Wwrite-strings \
		 -Waggregate-return -Wunreachable-code

# Tell the engine where the settings file is
CFLAGS += -D ENGINE_SETTINGS_FILE=\"../engine_settings.h\"

ifeq ($(mode), debug)
CFLAGS += -O0
BUILD_DIR := $(BUILD_DIR_BASE)/debug
else
CFLAGS += -O2
BUILD_DIR := $(BUILD_DIR_BASE)/release
endif

mode_defined = 1
ifneq ($(mode), release)
ifneq ($(mode), debug)
mode_defined = 0
endif
endif

LDFLAGS += -lncurses
# Find all the C files we want to compile
# Note the single quotes around the * expressions. Make will incorrectly expand
# these otherwise.
SRCS := $(shell find $(SRC_DIRS) -name '*.c')

# String substitution for every C file.
# As an example, hello.c turns into ./build/hello.c.o
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

# String substitution (suffix version without %).
# As an example, ./build/hello.c.o turns into ./build/hello.c.d
DEPS := $(OBJS:.o=.d)

# Every folder in ./src will need to be passed to GCC so that it can find header
# files
INC_DIRS := $(shell find $(SRC_DIRS) -type d)
# Add a prefix to INC_DIRS. So moduleA would become -ImoduleA. GCC understands
# this -I flag
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

# The -MMD and -MP flags together generate Makefiles for us!
# These files will have .d instead of .o as the output.
CPPFLAGS := $(INC_FLAGS) -MMD -MP

.PHONY:all
all: information $(BUILD_DIR)/$(TARGET_EXEC)

information:
ifeq ($(mode_defined), 0)
	$(error Invalid build mode. Please use 'make mode=release'(default) or \
		'make mode=debug')
else
	$(info  Building in "$(mode)" mode)
endif

# The final build step.
$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# Build step for C source
$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

# Include the .d makefiles. The - at the front suppresses the errors of missing
# Makefiles. Initially, all the .d files will be missing, and we don't want
# those errors to show up.
-include $(DEPS)
