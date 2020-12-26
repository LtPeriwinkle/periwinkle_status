#include <stdio.h>
#include <string.h>
#include <stdlib.h>

unsigned long get_mem_free() {
    	// /proc/meminfo contains free memory among other things
	FILE* f = fopen("/proc/meminfo", "r");
	char line[40];
	char *tok;
	// get total memory
	fgets(line, 40, f);
	// first token is MemTotal:
	strtok(line, " ");
	tok = strtok(NULL, " ");
	int total = atoi(tok);

	// get free memory
	fgets(line, 40, f);
	// first token is just "MemFree:"
	strtok(line, " ");
	// second token is our number
	tok = strtok(NULL, " ");
	int free = atoi(tok);
	return total - free;
}
