

LDFLAGS=-L../build/ -lnng -lpthread
CFLAGS=-I../include

ALL=bus pair producer consumer
all: $(ALL)

$(ALL):
	$(CC) -c $(CFLAGS) $@.c -o $@ $(LDFLAGS)

clean:
	rm -f $(ALL)
