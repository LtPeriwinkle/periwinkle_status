#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "disk_free.h"
#include "mem_free.h"

int main(int argc, char* argv[]) {
        time_t rawtime;
        struct tm *dt;
        char gpu_pct[3];
        char diskinfo[7];
        char meminfo[10];
        int cpu_temp;
        int gpu_temp;
        unsigned long int free_bytes;
        char datetime[20];
        FILE *f;
        int num = 0;
        while (1) {
                // only check the disk on the first time and afer num is reset
                if (num == 0) {
                        free_bytes = get_disk_free("/");
                        create_size_str(free_bytes, diskinfo);
                }
                f = fopen("/sys/class/drm/card0/device/gpu_busy_percent", "r");
                fscanf(f, "%s", gpu_pct);
                fclose(f);
                // hwmon0 is my gpu on my gentoo install
                // you will probably have to change the index for your specific computer
                f = fopen("/sys/class/hwmon/hwmon0/temp2_input", "r");
                fscanf(f, "%d", &gpu_temp);
                fclose(f);
                // cpu temperature at hwmon1 for me
                f = fopen("/sys/class/hwmon/hwmon1/temp2_input", "r");
                fscanf(f, "%d", &cpu_temp);
                fclose(f);
                // divide by 1000 since the values have extra zeroes for some reason
                cpu_temp /= 1000;
                gpu_temp /= 1000;
                // get the time and format it
                time(&rawtime);
                dt = localtime(&rawtime);

		// on my machine the values in /proc/meminfo are in kb, we want bytes here.
                free_bytes = get_mem_free() * 1024;
                create_size_str(free_bytes, meminfo);
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

