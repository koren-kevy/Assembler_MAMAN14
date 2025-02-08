CC = gcc

CFLAGS = -Wall -ansi -pedantic

SRC = assembler_main.c pre_Assembler.c error_handler.c Utility.c first_passage.c
HEADERS = assembler_definitions.h pre_and_passages.h error_handler.h Utility.h

TARGET = assembler

all: $(TARGET)

$(TARGET): $(SRC) $(HEADERS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)
