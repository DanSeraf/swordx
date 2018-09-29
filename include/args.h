#ifndef ARGS_H
#define ARGS_H

typedef struct args {
    char *explude;  
    char *ignore;
    char *log; // log file name
    char *output;
    unsigned int min;
} args;

void initializeArgs(args *options);
void destroyArgs(args *o);

#endif
