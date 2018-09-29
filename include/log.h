#ifndef LOGGER_H
#define LOGGER_H

typedef struct logger {
    char *filename;
    int cword;
    int iword;
    double time;
    struct logger *nextlog;
} logger;

logger **createLogger();
void pushLog(logger **log_head, const char *file_name, int counted_word, int ignored_word, double time);
void writeLog(logger *log_head, FILE *out_log_file);
#endif
