OBJS=repl.o global.o grammar.l.o grammar.y.o eval.o alloc.o ast.o

CFLAGS=-Wall -Wextra -pedantic -Werror -g -O0
LDFLAGS=-lm

ifdef NREADLINE
	CFLAGS += -DNREADLINE
else
	LDFLAGS += -lreadline
endif

all: subpython

subpython: $(OBJS)
	$(CC) $(CFLAGS) $^ -o subpython $(LDFLAGS)

clean:
	rm -f *.o subpython

.PHONY: all clean

alloc.o: alloc.c alloc.h types.h global.h eval.h grammar.h grammar.y.h \
 ast.h grammar.l.h
ast.o: ast.c ast.h types.h
eval.o: eval.c eval.h grammar.h grammar.y.h ast.h types.h global.h \
 grammar.l.h alloc.h
global.o: global.c global.h
grammar.l.o: grammar.l.c grammar.y.h ast.h types.h global.h
grammar.y.o: grammar.y.c ast.h types.h global.h grammar.l.h
repl.o: repl.c alloc.h types.h eval.h grammar.h grammar.y.h ast.h \
 global.h grammar.l.h
