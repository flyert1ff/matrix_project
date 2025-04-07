CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11
INCLUDES = -I./include -I./matrix -I./output
LIBS = -lcunit
DOXYGEN = doxygen

SRC_DIR = src
BUILD_DIR = build
DOCS_DIR = docs

# Создание папки для сборки
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Основные цели
all: build tests doc

# Сборка библиотеки и основного приложения
build: $(BUILD_DIR)/main

$(BUILD_DIR)/matrix.o: | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $(SRC_DIR)/matrix/matrix.c -o $@

$(BUILD_DIR)/output.o: | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $(SRC_DIR)/output/output.c -o $@

$(BUILD_DIR)/main.o: | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $(SRC_DIR)/main.c -o $@

$(BUILD_DIR)/main: $(BUILD_DIR)/main.o $(BUILD_DIR)/matrix.o $(BUILD_DIR)/output.o
	$(CC) $^ -o $@

# Сборка тестов
tests: $(BUILD_DIR)/test_runner

$(BUILD_DIR)/tests_matrix.o: | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c tests/tests_matrix.c -o $@

$(BUILD_DIR)/tests_output.o: | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c tests/tests_output.c -o $@

$(BUILD_DIR)/test_runner: $(BUILD_DIR)/tests_matrix.o $(BUILD_DIR)/tests_output.o $(BUILD_DIR)/matrix.o $(BUILD_DIR)/output.o
	$(CC) $^ -o $@ $(LIBS)

# Запуск тестов
test: tests
	./$(BUILD_DIR)/test_runner

# Генерация документации
doc:
	$(DOXYGEN) Doxyfile

# Форматирование кода
format:
	clang-format -i $(SRC_DIR)/*/*.c include/*.h tests/*.c

# Проверка Valgrind
valgrind-check:
	@valgrind --leak-check=full --show-leak-kinds=all build/main

# Проверка CppCheck
cppcheck-test:
	@cppcheck --enable=all --inconclusive --quiet src/main.c src/matrix/matrix.c src/output/output.c

# Очистка
clean:
	rm -rf $(BUILD_DIR)/*	$(DOCS_DIR)/html	$(DOCS_DIR)/latex

.PHONY: all build tests test doc format clean
