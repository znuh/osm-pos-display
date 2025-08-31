CFLAGS = -Wall -O2

OSM_CFLAGS = $(shell pkg-config --cflags osmgpsmap-1.0)
OSM_LIBS   = $(shell pkg-config --libs osmgpsmap-1.0)

osm-pos-display: osm-pos-display.c
	$(CC) $(CFLAGS) $(OSM_CFLAGS) -o $@ $< $(OSM_LIBS)
	
demo: demo.c
	$(CC) $(CFLAGS) -o $@ $< $(DEMO_LIBS)

.PHONY: all clean

all: osm-pos-display demo

clean:
	rm -f osm-pos-display demo *.o
