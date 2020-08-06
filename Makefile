TARGET = vimon

# - Compiler
CC=gcc
CXX=g++
CFLAGS = -g -Wall -Wno-unused -Wno-unknown-pragmas

# - Linker
LIBS = -lwiringPi -lwiringPiDev -lpthread -lstdc++

OBJDIR = ./obj

.PHONY: default all celan

all: default

CSRCS += $(wildcard *.c)
CSRCS += $(wildcard aprs/*.c)
CPPSRCS += $(wildcard *.cpp)
CPPSRCS += $(wildcard aprs/*.cpp)

COBJS = $(patsubst %.c,$(OBJDIR)/%.o,$(CSRCS))
CPPOBJS = $(patsubst %.cpp,$(OBJDIR)/%.o,$(CPPSRCS))

SRCS = $(CSRCS) $(CPPSRCS)
OBJS = $(COBJS) $(CPPOBJS)

$(OBJDIR)/%.o: %.c
	@mkdir -p $(OBJDIR)
	@echo "CC $<"
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(OBJDIR)
	@echo "CXX $<"
	@$(CXX) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: %.h

$(OBJDIR)/vimon.o: vimon_cal.h

default: $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) $(LIBS) -o $(TARGET)

.PRECIOUS: $(TARGET) $(OBJ)

