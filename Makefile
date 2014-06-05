SOURCES = map.c tree.c
OBJECTS = $(patsubst %.c,%.o,$(SOURCES))

CFLAGS = -std=c99 -pedantic

all: $(OBJECTS)

%.o: %.c
	gcc $(CFLAGS) -c -o $@ $<

clean:
	@echo cleaning...
	@rm -f *.o *~ core.*
