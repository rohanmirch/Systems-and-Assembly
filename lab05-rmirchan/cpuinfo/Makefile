CFLAGS=-Wall -Werror


OBJS=cpuid.o cpuid_ext.o cpuinfo.o

all: cpuinfo

cpuinfo: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o cpuinfo $(LDFLAGS)

clean:
	$(RM) $(OBJS) *~ cpuinfo

.PHONY: clean

