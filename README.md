# osm-pos-display
Simple Gtk based position display using [osm-gps-map](https://github.com/nzjrs/osm-gps-map). Position(s) are read from stdin.
<img width="404" height="638" alt="demo" src="https://github.com/user-attachments/assets/a7d8bc5b-7320-49df-a59c-8d67e3ef7855" />

## Requirements
[osm-gps-map](https://github.com/nzjrs/osm-gps-map) + Gtk, etc.<br>On Debian-based distros you can simply install **libosmgpsmap-1.0-dev**.<br>However, there are some issues with [center hopping](https://github.com/nzjrs/osm-gps-map/issues/101) in this version. You can use [this fork+branch](https://github.com/znuh/osm-gps-map/tree/center_hopping-fix) to fix these issues.

## Usage
* Try it out with **demo.sh**
* **./osm-pos-display -h** shows usage/help

## References
* [osm-gps-map API docs](https://nzjrs.github.io/osm-gps-map/docs/reference/html/index.html)
* [osm-gps-map examples](https://github.com/nzjrs/osm-gps-map/tree/master/examples)
