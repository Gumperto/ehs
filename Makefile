EXEC := ehs

BUILD_DIR := ./build
SRC_DIR := ./src

SRCS := $(shell find $(SRC_DIR) -name '*.c')
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

# String substitution (suffix version without %).
# As an example, ./build/hello.cpp.o turns into ./build/hello.cpp.d
DEPS := $(OBJS:.o=.d)

# Every folder in ./src will need to be passed to GCC so that it can find header files
INC_DIRS := $(shell find $(SRC_DIR) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS := $(INC_FLAGS) -MMD -MP -Werror -Wextra

# Make executable ./build/ehs from objects
$(BUILD_DIR)/$(EXEC): $(OBJS)
	gcc $(OBJS) -o $@ $(LDFLAGS) -lm

# Find all files that end in .c in ./build and make .o's out of them
# and then dump them into a subdirectory named after the one they came from
$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	gcc $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# Nukes ./build
clean:
	rm -rf $(BUILD_DIR) results_*.csv .env

-include $(DEPS)
