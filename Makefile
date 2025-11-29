# Makefile for PC Calculator (Linux/Unix)
# ANSI C compliant

CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c89
LDFLAGS =
TARGET = calc

# Source files (all in root directory)
SRCS = bignum.c bignum_ops.c bignum_math.c \
       converter.c formatter.c \
       parser.c evaluator.c \
       calculator.c utils.c \
       main.c

# Object files
OBJS = $(SRCS:.c=.o)

# Default target
all: $(TARGET)

# Link object files to create executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $(TARGET)

# Compile source files to object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets
.PHONY: all clean

# Dependencies (header files)
bignum.o: bignum.h utils.h
bignum_ops.o: bignum_ops.h bignum.h
bignum_math.o: bignum_math.h bignum.h bignum_ops.h
converter.o: converter.h bignum.h utils.h
formatter.o: formatter.h bignum.h utils.h
parser.o: parser.h utils.h
evaluator.o: evaluator.h parser.h bignum.h bignum_ops.h bignum_math.h converter.h
calculator.o: calculator.h utils.h parser.h evaluator.h formatter.h
utils.o: utils.h
main.o: calculator.h
