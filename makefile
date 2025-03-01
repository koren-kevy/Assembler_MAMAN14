CC = gcc

CFLAGS = -Wall -ansi -pedantic

SRC = assembler_main.c pre_Assembler.c first_passage.c second_passage.c error_handler.c Utility.c
HEADERS = assembler_definitions.h pre_and_passages.h error_handler.h Utility.h

TARGET = assembler

all: $(TARGET)

$(TARGET): $(SRC) $(HEADERS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)
