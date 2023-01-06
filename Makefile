# The C Compiler
MPICC=mpicc

# Packages used and Compiler flags
PACKAGES=gsl

CFLAGS=$(shell pkgconf --cflags-only-other $(PACKAGES)) -fopenmp -march=native -mavx -std=gnu99 -O3 
LDFLAGS=$(shell pkgconf --libs $(PACKAGES))

# The various directories used in the project
BUILD_DIR	?= ./build
BIN_DIR		?= ./bin
SRC_DIR 	?= ./src
TESTS_DIR	?= ./tests
TOOLS_DIR ?= ./tools

# Source files for the project
SRCS := $(notdir $(shell find $(SRC_DIR) -name *.c))

# Test files for the project
TESTS := $(basename $(notdir $(shell find $(TESTS_DIR) -name *.c)))

# The object files and dependency files
OBJS := $(patsubst %.c, $(BUILD_DIR)/%.o, $(SRCS))
TEST_OBJS := $(patsubst %, $(BUILD_DIR)/%.o, $(TESTS))

DEPS := $(patsubst %.o, %.d, $(OBJS) $(TEST_OBJS))

# Include directories for the compiler
INC_DIRS := $(shell find $(SRC_DIR) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS)) $(shell pkgconf --cflags-only-I $(PACKAGES))

# Setting VPATH to include the source file directories
SPACE= 
VPATH=$(TESTS_DIR):$(subst $(SPACE),:,$(INC_DIRS))

# C preprocessor flags
CPPFLAGS=$(INC_FLAGS) -MMD -MP

.PHONY: default test tests clean

default: tests

test: tests
	@$(TOOLS_DIR)/runtests.sh

tests: $(addprefix $(BIN_DIR)/,$(TESTS))

$(BIN_DIR)/%: $(BUILD_DIR)/%.o $(OBJS) | $(BIN_DIR)
	$(MPICC) $(CPPFLAGS) $(CFLAGS) $^ -o $@ $(LDFLAGS)

.PRECIOUS: $(BUILD_DIR)/%.o
$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(MPICC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# Creating the directories
$(BIN_DIR) $(BUILD_DIR):
	@mkdir -p $@

# Remove all the produced files from the directory
clean: 
	rm -rf $(BIN_DIR) $(BUILD_DIR)

-include $(DEPS)
