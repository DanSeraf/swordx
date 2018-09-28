#include <stdio.h>
#include <stdlib.h>
#include "args.h"

void initializeArgs(args *o) {
    o->explude = NULL;
    o->ignore = NULL;
    o->log = NULL;
    o->output = NULL;
    o->min = 0;
}

void destroyArgs(args *o) {
    if (o->explude != NULL) free(o->explude);
    else if (o->ignore != NULL) free(o->ignore);
    else if (o->log != NULL) free(o->log);
    else if (o->output != NULL)free(o->output);
}
