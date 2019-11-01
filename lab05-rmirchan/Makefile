OBJS = lists.o arraylist.o linkedlist.o listperf.o smallobj.o

CFLAGS := -Wall -Werror -O2 $(CFLAGS)

all: info measure-perf measure-iter plot

info:
	$(MAKE) -C cpuinfo cpuinfo 
	mkdir -p data
	./cpuinfo/cpuinfo > data/cpu-info.txt 
	echo "" >> data/cpu-info.txt
	$(CC) -v 2>> data/cpu-info.txt
	echo "" >> data/cpu-info.txt
	uname -a >> data/cpu-info.txt

measure-perf:
	./measure.sh -p 50

measure-iter:
	./measure.sh -i 10

plot:
	./plot.sh

listperf: $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	$(RM) *.o listperf

.PHONY: all clean info measure-perf measure-iter plot
