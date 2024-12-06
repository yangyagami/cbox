SRC_DIRS := camera
SRCS := $(shell find $(SRC_DIRS) -name '*.c')
OBJS := $(SRCS:%.c=$(BUILD_DIR)/%.o)

TARGET = ff
INCLUDE_DIRS = -Icamera -Icamera/include

$(TARGET): $(OBJS)
	@echo "Why: $(OBJS)"
	$(CC) $(CFLAGS) -fPIC -shared -o libcamera.so $^

$(BUILD_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -fPIC $(INCLUDE_DIRS) -c $< -o $@
