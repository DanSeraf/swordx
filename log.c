#include <stdio.h>
#include <stdlib.h>
#include <string.h>

logger **createLogger() {
    logger **l = (logger *) malloc(sizeof(logger));
    *l = NULL;
    return l;
}

void push(logger **l, char *fn, int cw, int iw, float time) {
    logger *nextlog = (logger *) malloc(sizeof(logger));
    nextlog->filename = fn;
    nextlog->cword = cw;
    nextlog->iword = iw;
    nextlof->time = time;
    *l = nextlog; 
}

void writeLog(logger *l) {

    if (l->nextlog != NULL) {
    }
}
