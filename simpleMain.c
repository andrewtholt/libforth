#include "libforth.h"
#include "unit.h"
#include <assert.h>
#include <errno.h>
#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

extern int _fileno(FILE*);

#ifdef USE_BUILT_IN_CORE
extern unsigned char forth_core_data[];
extern forth_cell_t forth_core_size;
#endif

int main(int argc, char **argv) {
    FILE *core = fopen("forth.core", "rb");
    forth_t *o = NULL;
    int r = 0; 
    if (core) {
        o = forth_load_core_file(core);
        fclose(core);
    }    
    if (!o) 
        o = forth_init(DEFAULT_CORE_SIZE, stdin, stdout, NULL);
    if (!o) {
        fatal("failed to initialize forth: %s", forth_strerror());
        return -1;
    }    
    forth_set_args(o, argc, argv);
    if ((r = forth_run(o)) < 0) 
        return r;
    errno = 0; 
    if (!(core = fopen("forth.core", "wb"))) {
        fatal("failed to save core file: %s", forth_strerror());
        return -1;
    }    
    fclose(core);
    r = forth_save_core_file(o, core);
    forth_free(o);
    return r;
}

