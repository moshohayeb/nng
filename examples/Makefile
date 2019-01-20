

LDFLAGS=-L../build/ -lnng -lpthread
CFLAGS=-I../include

ALL=bus pair producer consumer

all: $(ALL)

$(ALL): %: %.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f $(ALL)
