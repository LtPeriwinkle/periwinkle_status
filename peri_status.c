#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/sysinfo.h>
#include "disk_free.h"

int main(int argc, char* argv[]) {
        struct sysinfo si;
        struct tm *dt;
        time_t rawtime;
        char gpu_pct[3];
        char diskinfo[7];
        char meminfo[10];
        int cpu_temp;
        int gpu_temp;
        unsigned long int free_bytes;
        char datetime[20];
        int num = 0;
        // gpu temperature file
        FILE *gput = fopen("/sys/class/hwmon/hwmon0/temp2_input", "r");
        // cpu temp file
        FILE *cput = fopen("/sys/class/hwmon/hwmon1/temp2_input", "r");
        // gpu percent file
        FILE *gpup = fopen("/sys/class/drm/card0/device/gpu_busy_percent", "r");
        while (1) {
                // only check the disk on the first time and afer num is reset
                if (num == 0) {
                        free_bytes = get_disk_free("/");
                        create_size_str(free_bytes, diskinfo);
                }
                // get the string from each file then refresh it and rewind the cursor to the start of the file
                fscanf(gput, "%d", &gpu_temp);
                rewind(gput);
                fflush(gput);
                fscanf(gpup, "%s", gpu_pct);
                rewind(gpup);
                fflush(gpup);
                fscanf(cput, "%d", &cpu_temp);
                rewind(cput);
                fflush(cput);
                // divide by 1000 since the values have extra zeroes for some reason
                cpu_temp /= 1000;
                gpu_temp /= 1000;

		// get memory info using sysinfo. will eventually get and format system uptime here
		sysinfo(&si);
		free_bytes = (si.totalram * si.mem_unit) - (si.freeram * si.mem_unit);
		create_size_str(free_bytes, meminfo);

		// hopefully fix time stuff
                rawtime = time(NULL);
                dt = localtime(&rawtime);
                strftime(datetime, 20, "%m/%d/%Y %H:%M:%S", dt);
                
                printf("mem %s | disk %s | cpu %d°C | gpu %s%% %d°C | %s\n", meminfo, diskinfo, cpu_temp, gpu_pct, gpu_temp, datetime);
                fflush(stdout);
                sleep(1);
                num += 1;
                // reset num after 600 cycles to check disk every 10 minutes
                if (num == 600) {
                        num = 0;
                }
        }
}

