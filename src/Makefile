CC = gcc
SRC = main.c editor.c parse.c
IDIR = ../headers
CFLAGS = -I$(IDIR)
TARGET = winapi_test
TARG_DIR = ../$(TARGET)

# $@ stands for exec or target name
# $^ stands for prerequisites - so all the .c or .o files after target name

$(TARGET): $(SRC)
	$(CC) -o $(TARG_DIR) $^ $(CFLAGS)

run:
	./$(TARG_DIR)