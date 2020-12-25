#include <stdlib.h>
#include <stdio.h>
#include <sys/statvfs.h>
#include "disk_free.h"

// gets the free space in bytes using POSIX statvfs
unsigned long get_disk_free(const char *path) {
        struct statvfs stat;
        statvfs(path, &stat);
        // massive int to account for tebibytes of freespace
        // using bavail for space accessible by user
        unsigned long int free_bytes = stat.f_bsize * stat.f_bavail;
        return free_bytes;
}

/* reduce the bytes into readable form. depending on how many reductions are necessary,
 change the unit label at the end. then write to the given string */
void create_disk_str(unsigned long free_bytes, char *buf) {
        int reductions = 0;
        char unit;
        long double ret = free_bytes;
        while (ret > 1024) {
                reductions += 1;
                ret /= 1024;
        }
        switch (reductions) {
                case 0:
                        unit = 'B';
                        break;
                case 1:
                        unit = 'K';
                        break;
                case 2:
                        unit = 'M';
                        break;
                case 3:
                        unit = 'G';
                        break;
                case 4:
                        unit = 'T';
                        break;
                default:
                        unit = ' ';
                        break;
        }
        snprintf(buf, 7, "%.2Lf%c", ret, unit);
}
