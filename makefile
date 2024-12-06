BUILD_DIR = build

# 定义变量
CC = gcc
CFLAGS = -Wall -Werror

ifeq ($(DEBUG), 1)
CFLAGS += -g -O0 -D_DEBUG
endif

ifeq ($(CHECK), 1)
CFLAGS += -D_DEBUG -O0 -fsanitize=address
endif

ifeq ($(RELEASE), 1)
CFLAGS += -O3 -DNDEBUG
endif

include camera/camera.mk

# 清理目标
clean:
	rm -f *.o *.d
