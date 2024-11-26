# 定义变量
CC = gcc
CFLAGS_DEBUG = -Wall -g -O0 -Werror
CFLAGS_RELEASE = -O2 -Wall -Werror
CFLAGS_CHECK = -O2 -Wall -Werror -fsanitize=address

# 默认目标
all: release

# Release 目标
release: CFLAGS = $(CFLAGS_RELEASE)
release: camera_test

# Debug 目标
debug: CFLAGS = $(CFLAGS_DEBUG)
debug: camera_test

# Check
check: CFLAGS = $(CFLAGS_CHECK)
check: camera_test

# 链接目标
camera_test: test/camera/main.c camera/camera.h
	$(CC) $(CFLAGS) -I. $< -o $@

# 清理目标
clean:
	rm -f camera_test

# 伪目标
.PHONY: all release debug clean
