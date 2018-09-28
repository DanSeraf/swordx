#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "log.h"

logger **createLogger() {
    logger **l = (logger **) malloc(sizeof(logger));
    *l = NULL;
    return l;
}

void pushLog(logger **l, const char *fn, int cw, int iw, double time) {
    logger *newlog = (logger *) malloc(sizeof(logger));
    newlog->filename = strdup(fn);
    newlog->cword = cw;
    newlog->iword = iw;
    newlog->time = time;
    newlog->nextlog = (*l);
    (*l) = newlog;
}

void writeLog(logger *l, FILE *log) {
    logger *curr = l;

    while (curr != NULL) {
        fprintf(log, "%s %d %d %f\n", curr->filename, curr->cword, curr->iword, curr->time);
        curr = curr->nextlog;
    }
}
