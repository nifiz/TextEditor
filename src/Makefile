CC = gcc
SRC = main.c editor.c parse.c char_operations.c text_structure.c display.c text_buffer.c util.c
IDIR = ../headers
CFLAGS = -I$(IDIR)
TARGET = winapi_test
TARG_DIR = ../$(TARGET)
DEBUG_TARGET_BUILD = /build/Debug/outDebug

# $@ stands for exec or target name
# $^ stands for prerequisites - so all the .c or .o files after target name

$(TARGET): $(SRC)
	$(CC) -o $(TARG_DIR) $^ $(CFLAGS)

run:
	./$(TARG_DIR)

run_debug:
	@echo "Hi!"
	./$(DEBUG_TARGET_BUILD)