EE_BIN = Ps2donut.elf
EE_OBJS = main.o

# Define PS2DEV path if not already set
ifndef PS2DEV
PS2DEV = $(HOME)/ps2dev
endif

# Include paths for PS2 libraries
IFLAGS = -I$(PS2DEV)/ps2sdk/ee/include -I$(PS2DEV)/gsKit/include
LDFLAGS = -L$(PS2DEV)/ps2sdk/ee/lib -L$(PS2DEV)/gsKit/lib

# Libraries to link against
LIBS = -lgs -laudsrv -lkernel -lpad -ldma

# EE GCC compiler
EE_CC = $(PS2DEV)/ee/bin/ee-gcc
EE_CFLAGS = -O2 -G0 -Wall -Werror $(IFLAGS)
EE_LDFLAGS = $(LDFLAGS) $(LIBS)

all: $(EE_BIN)

clean:
	rm -f $(EE_BIN) $(EE_OBJS)

main.o: main.c
	$(EE_CC) $(EE_CFLAGS) -c main.c -o main.o

$(EE_BIN): $(EE_OBJS)
	$(EE_CC) -T $(PS2DEV)/ps2sdk/ee/startup/linkfile -o $(EE_BIN) $(EE_OBJS) $(EE_LDFLAGS)
	$(PS2DEV)/bin/ps2-packer $(EE_BIN)
