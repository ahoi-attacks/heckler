all: libsevstep.so libsevstep.a

include environment.env
export 

ifndef KERNEL_HEADER_PATH
$(error Missing KERNEL_HEADER_PATH env var. Did you source environment.env ? Also check the comments in the file on how to set this variable)
endif

INCLUDES =   -I${KERNEL_HEADER_PATH}
LIBDIRS = -L./build/libs
CFLAGS = -Wall -Wextra -fstack-protector -Wshadow -Werror -fno-omit-frame-pointer -fPIC $(shell pkg-config --cflags --libs glib-2.0) -Wno-error -Wno-unused-command-line-argument #-g  -fsanitize=address

SUBDIR = sev-step-lib
OBJ_FILE_DIR = ./build/obj
OUTPUT_BINARY_DIR = ./build/binaries
OUTPUT_LIB_DIR = ./build/libs
OUTPUT_LOG_DIR = ./build/logs

INCS = $(wildcard *.h $(foreach fd, $(SUBDIR), $(fd)/*.h))
SRCS = sev-step-lib/sev_step_api.c
NODIR_SRC = $(notdir $(SRCS))
OBJS = $(addprefix $(OBJ_FILE_DIR)/, $(SRCS:c=o)) # obj/xxx.o obj/folder/xxx .o

#build all object files
$(OBJ_FILE_DIR)/%.o: $(SRCS) $(INCS)
	mkdir -p $(@D)
	clang $(INCLUDES) $(CFLAGS) -o $@ -c $<  $(INC_DIRS)

$(OBJ_FILE_DIR)/sev-step-lib/raw_spinlock.o:  sev-step-lib/raw_spinlock.asm
	mkdir -p $(@D)
	nasm -f elf64 -o $(OBJ_FILE_DIR)/sev-step-lib/raw_spinlock.o $^

libsevstep.a: $(OBJ_FILE_DIR)/sev-step-lib/sev_step_api.o $(OBJ_FILE_DIR)/sev-step-lib/raw_spinlock.o $(OBJ_FILE_DIR)/sev-step-lib/sev_step_routines.o $(OBJ_FILE_DIR)/sev-step-lib/sev_step_eviction_set_builders.o $(OBJ_FILE_DIR)/sev-step-lib/sev_step_pagemap_parser.o $(OBJ_FILE_DIR)/sev-step-lib/sev_step_cache_attack_log.o $(OBJ_FILE_DIR)/sev-step-lib/sev_step_http_client.o
	mkdir -p $(OUTPUT_LIB_DIR)
	ar rcs $(OUTPUT_LIB_DIR)/libsevstep.a $^


libsevstep.so: $(OBJ_FILE_DIR)/sev-step-lib/sev_step_api.o $(OBJ_FILE_DIR)/sev-step-lib/raw_spinlock.o 
	mkdir -p $(OUTPUT_LIB_DIR)
	clang -shared -fPIC -o $(OUTPUT_LIB_DIR)/libsevstep.so $^


clean-apps:
	rm -f libsevstep.a
	rm -rf build

clean: clean-apps
