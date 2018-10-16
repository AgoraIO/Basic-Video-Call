ifeq (${CXX},)
CXX=g++
endif
LINK=${CXX}

TOPDIR=`pwd`
LIBPATH=./../../libs
LDFLAGS= -static-libgcc -std=c++11
CXXFLAGS  = -std=c++0x -fPIC -g -fno-omit-frame-pointer \
			-DNDEBUG=1 -Wconversion -O3 -Wall -W -fvisibility=hidden
 					
LIB	   = -pthread -lpthread -L$(LIBPATH) -Wl,-rpath=$(LIBPATH) -lagora_rtc_sdk -ldl -lrt -lasound -lpulse 
INCPATH =-I. -I./../../include

SRC =$(wildcard *.cpp View/*.cpp Controller/*.cpp EngineModel/*.cpp )
OBJ=$(addsuffix .o, main $(basename $(SRC)))
TARGET=openVideoCall


.PHONY: all clean
all: $(TARGET)

$(TARGET): $(OBJ)
	$(LINK) $(LDFLAGS) $(INCPATH) $^ -o "$@" $(LIB)

%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

clean:
	rm -f $(TARGET)
	rm -f ${OBJ}
