CC = gcc
AR = ar
RM = rm -rf

#原始目录
SRC_PATH :=.
MODULES :=
#目标名
TARGET :=
#源文件
MODULES += $(wildcard $(SRC_PATH)/src/*.c)
SRCS += $(MODULES)
SRCS += $(wildcard $(SRC_PATH)/*.c)
#中间文件
OBJS := $(SRCS:.c=.o)

MODULE_OBJS := $(MODULES:.c=.o)
_OBJS=$(MODULE_OBJS) test/mp4_demux_test.o
_TARGET=mp4_demux_test


#动态库
LIBS := stdc++ pthread

#模块库文件
MOULE_LIBRARY_PATH = /usr/lib/ /usr/local/lib/

#头文件路径
INCLUDE_PATH :=.
#动态库路径
LIBRARY_PATH :=

INCLUDE_PATH += /usr/include
INCLUDE_PATH += ./include
LIBRARY_PATH += $(MOULE_LIBRARY_PATH)


RELEASE = 1
BITS =

#ifeq ( 1 , ${DBG_ENABLE} )
#	CFLAGS += -D_DEBUG -O0 -g -DDEBUG=1
#endif

CFLAGS = -Wall -std=c++11
LFLAGS =

#头文件
CFLAGS += $(foreach dir,$(INCLUDE_PATH),-I$(dir))

#库路径
LDFLAGS += $(foreach lib,$(LIBRARY_PATH),-L$(lib))

#库名
LDFLAGS += $(foreach lib,$(LIBS),-l$(lib))

#检查版本
ifeq ($(RELEASE),0)
	#debug
	CFLAGS += -g
else
	#release
	CFLAGS += -O3 -DNDEBUG
endif

#检查位宽
ifeq ($(BITS),32)
	CFLAGS += -m32
	LFLAGS += -m32
else
	ifeq ($(BITS),64)
		CFLAGS += -m64
		LFLAGS += -m64
	else
	endif
endif

$(warning $(OBJS))

#操作命令
all:clean build

$(OBJS):%.o:%.c
	$(CC) $(CFLAGS) -c $^ -o $@

build_target:$(_OBJS)
	$(CC) $(LFLAGS) -o $(_TARGET) $(_OBJS) $(LDFLAGS)

build:build_target
	$(RM) $(OBJS)

clean:
	echo $(SRCS)
	$(RM) $(OBJS) $(_TARGET)
