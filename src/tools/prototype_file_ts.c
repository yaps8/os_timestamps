#ifndef PROTOTYPE_FILE_TS_C
#define PROTOTYPE_FILE_TS_C

#include "prototype_file_ts.h"

int main (int argc, char **argv){
   
    if (argc < 2){
        printf("ERROR: need one argument (path)\n");
        return 1;
    }
  
    char* path = argv[1];
    int mode = 0;
    if (argc >= 3){
        mode = atoi(argv[2]);
    }

    if (mode == 1){
        printf("%s: (lstat + B)\n", path);
        print_path_timestamps_lstat_ns(path);
    }
    else if (mode == 2){
        #ifdef __linux__
        printf("%s: (statx AT_SYMLINK_NOFOLLOW)\n", argv[1]);
        print_path_timestamps_statx_ns(argv[1], 0);
        #endif
    }
    else {
        printf("%s: (stat + B)\n", path);
        print_path_timestamps_ns(path);
    }
}


#endif
