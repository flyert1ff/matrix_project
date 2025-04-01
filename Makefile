CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11
INCLUDES = -Iinclude -Imatrix -Ioutput
LIBS = -lcunit
DOXYGEN = doxygen

SRC_DIR = src
BUILD_DIR = build
DOCS_DIR = docs

# Основные цели
all:	build tests doc

# Сборка библиотеки и основного приложения
build:	$(BUILD_DIR)/main

$(BUILD_DIR)/matrix.o:	$(SRC_DIR)/matrix/matrix.c	include/config.h	matrix/matrix.h
	$(CC)	$(CFLAGS)	$(INCLUDES)	-c	$<	-o	$@

$(BUILD_DIR)/output.o:	$(SRC_DIR)/output/output.c	output/output.h
	$(CC)	$(CFLAGS)	$(INCLUDES)	-c	$<	-o	$@

$(BUILD_DIR)/main.o:	$(SRC_DIR)/main.c matrix/matrix.h	output/output.h
	$(CC)	$(CFLAGS)	$(INCLUDES)	-c	$<	-o	$@

$(BUILD_DIR)/main:	$(BUILD_DIR)/main.o	$(BUILD_DIR)/matrix.o	$(BUILD_DIR)/output.o
	$(CC)	$^	-o	$@

# Сборка тестов
tests:	$(BUILD_DIR)/test_runner

$(BUILD_DIR)/tests_matrix.o: tests/tests_matrix.c matrix/matrix.h
	$(CC)	$(CFLAGS)	$(INCLUDES)	-c	$<	-o	$@

$(BUILD_DIR)/tests_output.o: tests/tests_output.c output/output.h
	$(CC)	$(CFLAGS)	$(INCLUDES)	-c	$<	-o	$@

$(BUILD_DIR)/test_runner:	$(BUILD_DIR)/tests_matrix.o	$(BUILD_DIR)/tests_output.o	$(BUILD_DIR)/matrix.o	$(BUILD_DIR)/output.o
	$(CC)	$^	-o	$@	$(LIBS)

# Запуск тестов
test: tests
	./$(BUILD_DIR)/test_runner

# Генерация документации
doc:
	$(DOXYGEN) Doxyfile

# Форматирование кода
format:
	clang-format	-i	$(SRC_DIR)/*/*.c	include/*.h	tests/*.c

# Очистка
clean:
	rm	-rf	$(BUILD_DIR)/*	$(DOCS_DIR)/html	$(DOCS_DIR)/latex

.PHONY: all build tests test doc format clean
