CC:=clang

CFLAGS:=-Wall -Wextra -Wpedantic -Werror -std=c23

EXEC:=libtorin.a
BUILD_DIR:=build
TEST_DIR:=tests
OBJ:=arena.o pool.o ring.o
TESTS:=test_arena test_pool test_ring

OBJ_PATHS=$(addprefix $(BUILD_DIR)/, $(OBJ))

define build_test
	$(CC) $(CFLAGS) -I include -o $(TEST_DIR)/$(1) $(1).c $(BUILD_DIR)/$(EXEC)

endef

define run_test
	@./$(TEST_DIR)/$(1)

endef

.PHONY: all clean run compiledb test help

all: $(BUILD_DIR)/$(EXEC)

help:
	@echo "Available commands:"
	@echo "all 		- build executable"
	@echo "clean 		- remove build and tests directories"
	@echo "run 		- run the executable"
	@echo "compiledb 	- generate compilation database (requires bear)"
	@echo "test		- build and run tests"

run: all
	./$(BUILD_DIR)/$(EXEC)

test: $(BUILD_DIR)/$(EXEC) $(TEST_DIR)
	$(foreach item, $(TESTS), $(call build_test,$(item)))
	$(foreach item, $(TESTS), $(call run_test,$(item)))

compiledb: clean
	bear -- make -B

$(BUILD_DIR)/$(EXEC): $(OBJ_PATHS)
	ar rcs $(BUILD_DIR)/$(EXEC) $(OBJ_PATHS)

$(BUILD_DIR)/%.o: %.c $(BUILD_DIR)
	$(CC) $(CFLAGS) -I include -Og -c -o $@ $< 

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

$(TEST_DIR):
	mkdir $(TEST_DIR)

clean:
	rm -r $(BUILD_DIR)
	rm -r $(TEST_DIR)
