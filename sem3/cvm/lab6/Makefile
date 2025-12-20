CC := clang
OBJDUMP ?= objdump
CFLAGS := -g -O0 --target=i386-apple-darwin -masm=intel
ASFLAGS := $(CFLAGS)

SRCS := 00.c 01.c
PROGS := lab_00 lab_01
ASMS := 00.s 01.s
OBJS := 00.o 01.o
OBJDUMPS := 00.objdump 01.objdump

.PHONY: all build asm obj objdump clean

all: build

build: $(PROGS)

lab_00: 00.o
	$(CC) $(CFLAGS) 00.o -o $@

lab_01: 01.o
	$(CC) $(CFLAGS) 01.o -o $@

# produce asm (.s) from .c
%.s: %.c
	$(CC) $(ASFLAGS) -S $< -o $@

# produce object (.o) from .c
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# create objdump with source interleaved
%.objdump: %.o
	$(OBJDUMP) -d -M intel -S $< > $@

asm: $(ASMS)
obj: $(OBJS)
objdump: $(OBJDUMPS)

clean:
	rm -f $(PROGS) $(ASMS) $(OBJS) $(OBJDUMPS)