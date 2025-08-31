#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char **argv) {
	float lat=61.7291869, lon=24.7863233, tmp;
	while(1) {
		printf("lat=%f lon=%f\n", lat, lon);
		fflush(stdout);
		tmp = (rand() % 64) - 32;
		tmp *= 10e-7;
		lat += tmp;
		tmp = rand() % 256;
		tmp *= 10e-7;
		lon += tmp;
		sleep(1);
	}
	return 0;
}
