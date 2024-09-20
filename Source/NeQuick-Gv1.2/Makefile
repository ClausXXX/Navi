#environment
SHELL = /bin/bash
RM = rm -f

#compiler
CC = gcc
CFLAGS = -Wall -Wtrigraphs -Wcomment -Wmissing-prototypes \
  -Wunused-variable -Wmissing-braces -Wfloat-equal -Wreturn-type -ggdb \
  -Wunused-parameter

#linker
LINK = gcc
LFLAGS =

#files
ALLPROGRS = NeQuick_command
SOURCEFILES = NeqCalcModipAz.c NeqCalcBottomsideNe.c NeqCalcTopsideNe.c NeqUtils.c NeqCalcEpstParams.c NeqGetNeOnVertRay.c NeqGetNeOnSlantRay.c NeqGetRayProperties.c NeQuick.c NeqIntegrate.c
#SOURCEFILES = $(wildcard *.c)
HEADERS = $(wildcard *.h)
OBJS = $(SOURCEFILES:%.c=%.o)
LIBS = -lm

all: $(ALLPROGRS)

NeQuick_command: $(OBJS) NeQuick_command.c
	$(LINK) $(LFLAGS) $(LIBS) -o $@ $^
	
$(OBJS) : $(SOURCEFILES) $(HEADERS)
	$(CC) $(CFLAGS) -c $(@:.o=.c) -o $@

clean :
	$(RM) $(OBJS) $(ALLPROGRS)

