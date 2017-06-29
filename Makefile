NAME := bmc_daemon 

BIT:=64
PLATFORM:=arm
ifeq ($(PLATFORM), arm)
    PRE:= arm-linux-
    BIT:=
endif

CC:=$(PRE)gcc
CXX:=$(PRE)g++
AR:=$(PRE)ar
LINK:=$(PRE)gcc
LINKFLAGS := -fpic -shared
CFLAGS := -fpic -g
Q_ := 
.PHONY:	clean

INCDIR += -I ./inc/\
		  -I ./bmc_share/xml/\
		  -I ./bmc_share/xml/libxml/\
          
#SRC = $(wildcard ./src/*.c ./src/*.cpp)
SRC = $(wildcard ./src/*.c)
SRC +=$(wildcard ./bmc_share/xml/*.c)

DIR = $(notdir$(SRC))
OBJ += $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SRC)))

CFLAGS += $(INCDIR)
 
DYN_LINKS_WITH +=  -lpthread -ldl 
DYN_LINKS_WITH +=  $(wildcard ./lib/*.so.*)

STATIC_LINKS_WITH += 

ifeq ($(PLATFORM), arm)
    STATIC_LINKS_WITH += $(wildcard ./lib/*.a)
endif


ifeq ($(PLATFORM), linux)
    STATIC_LINKS_WITH += 
endif

%.o: %.c
	echo '<$(CC)>[$(DBG)] Compiling object file "$@" ...'
	$(Q_)${CC} $(CFLAGS) -c $< -o $@

%.o: %.cpp
	echo '<$(CXX)>[$(DBG)] Compiling object file "$@" ...'
	$(Q_)${CXX} $(CFLAGS) -c $< -o $@

all: $(NAME)
$(NAME):$(OBJ)
	echo 1$(OBJ)
	echo 2'<$(AR)>creating binary "$(NAME)"'
	$(Q_)$(LINK)  -o $(NAME) $(OBJ) $(DYN_LINKS_WITH) '-Wl,--start-group' $(STATIC_LINKS_WITH) $(DYN_LINKS_WITH) '-Wl,--end-group'
install:

clean:
	rm $(NAME) -f
	rm $(OBJ)  -rf
	rm $(OUTPUT) -f
