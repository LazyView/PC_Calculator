# Makefile for PC Calculator (Linux/Unix)
# ANSI C compliant

CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c89 -Iinclude
LDFLAGS =
TARGET = calc

# Directories
SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include

# Source files by module
CORE_SRCS = $(SRC_DIR)/core/bignum.c \
            $(SRC_DIR)/core/bignum_ops.c \
            $(SRC_DIR)/core/bignum_math.c

CONVERSION_SRCS = $(SRC_DIR)/conversion/converter.c \
                  $(SRC_DIR)/conversion/formatter.c

EXPRESSION_SRCS = $(SRC_DIR)/expression/parser.c \
                  $(SRC_DIR)/expression/evaluator.c

APP_SRCS = $(SRC_DIR)/app/calculator.c \
           $(SRC_DIR)/app/utils.c

MAIN_SRC = $(SRC_DIR)/main.c

# All source files
SRCS = $(CORE_SRCS) $(CONVERSION_SRCS) $(EXPRESSION_SRCS) $(APP_SRCS) $(MAIN_SRC)

# Object files (place in build directory)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

# Default target
all: $(TARGET)

# Create build directory structure
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)/core
	mkdir -p $(BUILD_DIR)/conversion
	mkdir -p $(BUILD_DIR)/expression
	mkdir -p $(BUILD_DIR)/app

# Link object files to create executable
$(TARGET): $(BUILD_DIR) $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $(TARGET)

# Compile source files to object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

# Phony targets
.PHONY: all clean

# Dependencies (header files)
$(BUILD_DIR)/core/bignum.o: $(INCLUDE_DIR)/core/bignum.h $(INCLUDE_DIR)/app/utils.h
$(BUILD_DIR)/core/bignum_ops.o: $(INCLUDE_DIR)/core/bignum_ops.h $(INCLUDE_DIR)/core/bignum.h
$(BUILD_DIR)/core/bignum_math.o: $(INCLUDE_DIR)/core/bignum_math.h $(INCLUDE_DIR)/core/bignum.h $(INCLUDE_DIR)/core/bignum_ops.h
$(BUILD_DIR)/conversion/converter.o: $(INCLUDE_DIR)/conversion/converter.h $(INCLUDE_DIR)/core/bignum.h $(INCLUDE_DIR)/app/utils.h
$(BUILD_DIR)/conversion/formatter.o: $(INCLUDE_DIR)/conversion/formatter.h $(INCLUDE_DIR)/core/bignum.h $(INCLUDE_DIR)/app/utils.h
$(BUILD_DIR)/expression/parser.o: $(INCLUDE_DIR)/expression/parser.h $(INCLUDE_DIR)/app/utils.h
$(BUILD_DIR)/expression/evaluator.o: $(INCLUDE_DIR)/expression/evaluator.h $(INCLUDE_DIR)/expression/parser.h $(INCLUDE_DIR)/core/bignum.h $(INCLUDE_DIR)/core/bignum_ops.h $(INCLUDE_DIR)/core/bignum_math.h $(INCLUDE_DIR)/conversion/converter.h
$(BUILD_DIR)/app/calculator.o: $(INCLUDE_DIR)/app/calculator.h $(INCLUDE_DIR)/app/utils.h $(INCLUDE_DIR)/expression/parser.h $(INCLUDE_DIR)/expression/evaluator.h $(INCLUDE_DIR)/conversion/formatter.h
$(BUILD_DIR)/app/utils.o: $(INCLUDE_DIR)/app/utils.h
$(BUILD_DIR)/main.o: $(INCLUDE_DIR)/app/calculator.h
