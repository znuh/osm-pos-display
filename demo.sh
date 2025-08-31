#!/bin/sh
make all && (./demo | ./osm-pos-display --show-track)
