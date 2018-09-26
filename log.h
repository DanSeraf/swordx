#ifndef LOGGER_H
#define LOGGER_H

typedef struct logger {
    char *filename;
    int cword;
    int iword;
    float time;
    struct logger *nextlog;
} logger;

#endif
