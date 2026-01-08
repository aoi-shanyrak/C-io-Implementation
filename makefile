CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
TARGET = tester
OBJS = io.o tester.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

io.o: io.c io.h
	$(CC) $(CFLAGS) -c io.c

tester.o: tester.c io.h
	$(CC) $(CFLAGS) -c tester.c

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET) *.txt

memcheck: $(TARGET)
	valgrind --leak-check=yes ./$(TARGET)

.PHONY: all clean run memcheck
