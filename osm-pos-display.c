/*
 * osm-pos-display.c
 * Copyright (C) 2025 Benedikt Heinz <zn000h @ gmail.com>
 *
 * This is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License
 * as published by the Free Software Foundation; version 2.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */
#include <stdio.h>
#include <locale.h>
#include <glib.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include "osm-gps-map.h"

static GtkWidget *win1 = NULL;

static gboolean show_track  = FALSE;
static gboolean quit_on_eof = FALSE;
static gint zoom_level      = 16;
static GOptionEntry options[] = {
  { "show-track", 't', 0, G_OPTION_ARG_NONE, &show_track, "Show track (positions history)", NULL },
  { "quit-on-eof", 'q', 0, G_OPTION_ARG_NONE, &quit_on_eof, "Quit app when stdin read yields EOF", NULL },
  { "zoom", 'z', 0, G_OPTION_ARG_INT, &zoom_level, "Set zoom level", "N" },
  { NULL }
};

static void on_close (GtkWidget *widget, gpointer user_data) {
    gtk_widget_destroy(widget);
    gtk_main_quit();
}

static gboolean on_stdin_data(GIOChannel *source, GIOCondition condition, gpointer data) {
	OsmGpsMap *map = OSM_GPS_MAP(data);
    GError *error = NULL;
    gsize length;
    gchar *line;

    GIOStatus status = g_io_channel_read_line(source, &line, &length, NULL, &error);
    if (status == G_IO_STATUS_NORMAL) {
		//g_print(line);
		float lat, lon;
        int res = sscanf(line,"lat=%f lon=%f",&lat,&lon);
        if(res == 2) {
			osm_gps_map_set_center(map, lat, lon);
			if(show_track)
				osm_gps_map_gps_add(map, lat, lon, OSM_GPS_MAP_INVALID);
		}
		else
			g_printerr("Data format error - expected: lat=... lon=...\n");
        g_free(line);
    } else if (status == G_IO_STATUS_EOF) {
		if (quit_on_eof)
			on_close(win1, NULL);
        return FALSE;
	}
    else if (status == G_IO_STATUS_ERROR) {
        g_printerr("Error reading from stdin: %s\n", error->message);
        g_clear_error(&error);
        return FALSE;
    }

    return TRUE;
}

static int usage (GOptionContext *oc) {
    fputs(g_option_context_get_help(oc, TRUE, NULL), stdout);
    puts("Location data is read from stdin with one location per line - example:");
    puts("lat=61.7291869 lon=24.7863233");
    puts("lat=61.7291784 lon=24.7873683");
    puts("...");
	return 1;
}

int main (int argc, char **argv) {
	GError *error = NULL;

	gtk_init(&argc, &argv);

	GOptionContext *oc = g_option_context_new("- OSM position display");
    g_option_context_set_help_enabled(oc, FALSE);
    g_option_context_add_main_entries(oc, options, NULL);
    if (!g_option_context_parse (oc, &argc, &argv, &error))
        return usage(oc);

    OsmGpsMap *map = g_object_new(OSM_TYPE_GPS_MAP,
                        "map-source", OSM_GPS_MAP_SOURCE_OPENSTREETMAP,
                        "tile-cache", OSM_GPS_MAP_CACHE_AUTO,
                        "user-agent", "osm-pos-display.c", // Always set user-agent, for better tile-usage compliance
                        NULL);

    OsmGpsMapLayer *osd = g_object_new(OSM_TYPE_GPS_MAP_OSD,
                        "show-scale",FALSE,
                        "show-coordinates",TRUE,
                        "show-crosshair",TRUE,
                        "show-dpad",FALSE,
                        "show-zoom",TRUE,
                        "show-gps-in-dpad",FALSE,
                        "show-gps-in-zoom",FALSE,
                        "dpad-radius", 30,
                        NULL);
    osm_gps_map_layer_add(OSM_GPS_MAP(map), osd);
    g_object_unref(G_OBJECT(osd));	

	win1 = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size(GTK_WINDOW(win1), 400, 600);

	gtk_container_add(GTK_CONTAINER(win1), GTK_WIDGET(map));

    g_signal_connect(win1, "destroy", G_CALLBACK(on_close), NULL);
    gtk_widget_show_all(win1);

	osm_gps_map_set_zoom(map, zoom_level);

	setlocale(LC_NUMERIC, "C"); // nasty little sscanf looking for comma instead of decimal point

	/* watch STDIN for input */
	GIOChannel *io_channel = g_io_channel_unix_new(STDIN_FILENO);
    g_io_add_watch(io_channel, G_IO_IN, on_stdin_data, map);
    g_io_channel_unref(io_channel);

    gtk_main();

	return 0;
}
