

LDFLAGS=-L../build/ -lnng -lpthread
CFLAGS=-I../include

ALL=bus pair producer consumer nngpubsub lean

all: $(ALL)

$(ALL): %: %.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f $(ALL)
