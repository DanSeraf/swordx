#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <ctype.h>
#include <stdbool.h>
#include <dirent.h>
#include <errno.h>
#include "bintree.h"
#include "swordx.h"

void run(struct t_node *, struct args *params, int flag);
FILE *getFile(char *path); /* Get file from path */
void scanDir(const char *name, struct t_node *root);
int isFlagSet(int bitoption, int flag);
void printUsage();
void printHelp();

int isFlagSet(int bitopt, int flag) {
    return flag & bitopt;
}

FILE *getFile(char *path) {
    FILE *f = fopen(path, "r");
    if (f == NULL)
        perror("Error reading file");
    return f;
}

void run(struct t_node *root, struct args *params, int flag) {
    if ((isFlagSet(recursive_flag, flag)) != 0)
        scanDir("./test", root);
    return true;
}


void scanDir(const char *name, struct t_node *root) {
    FILE *f;
    DIR *dir;
    struct dirent *entry;

    if (!(dir = opendir(name)))
        printf("Problem opening dir");
    
    while ((entry = readdir(dir)) != NULL) {
            char path[1024];
            if (entry->d_type == DT_DIR) {
                if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                    continue;
                sprintf(path, "%s/%s", name, entry->d_name);
                scanDir(path, root);
            } else {
                sprintf(path, "%s/%s", name, entry->d_name);
                f = getFile(path);
                root = scanFile(f, root);
            }
    }
    closedir(dir);
}


int main (int argc, char **argv) {
    int opt = 0;
    int long_index = 0;
    struct t_node *root = NULL;
    struct args *params = NULL;
    params = (struct args *) malloc(sizeof(struct args));
    int flag = 0; /* byte flag */
    
    static struct option long_options[] =
	{
        {"help",                no_argument,        0,  'h'},
        {"recursive",           no_argument,        0,  'r'},
        {"follow",              no_argument,        0,  'f'},
        {"sortbyoccurrency",    no_argument,        0,  's'},
        {"alpha",               no_argument,        0,  'a'},
        {"explude",             required_argument,  0,  'e'},
        {"min",                 required_argument,  0,  'm'},
        {"ignore",              required_argument,  0,  'i'},
        {"log",                 required_argument,  0,  'l'},
        {"output",              required_argument,  0,  'o'},
        {0, 0, 0, 0}
	};
	
	while ((opt = getopt_long(argc, argv, "hrfsae:m:i:l:o:", long_options, &long_index)) != -1) {
		switch (opt) {
			case 'h': printHelp();
				break;
			case 'r': flag |= recursive_flag;
				break;
            case 'f': flag |= follow_flag;
                break;
            case 'a': flag |= alpha_flag;
                break;
            case 's': flag |= sbo_flag;
                break;
            case 'e': params->explude = strdup(optarg);
                break;
            case 'm': params->min = atoi(optarg);
                break; 
            case 'i': params->ignore = strdup(optarg);
                break; 
            case 'l': params->log = strdup(optarg);
                break;
            case 'o': params->output = strdup(optarg);
                break;
            default: printUsage();
				exit(EXIT_FAILURE);
			}
	}
    // Remaining arguments (inputs)
    if (optind < argc) {
        printf("INPUTS\n");
        while (optind < argc) 
            printf("%s\n", argv[optind++]);
    }
    run(root, params, flag);
    free(params);
    exit(EXIT_SUCCESS);
}

void printUsage () {
	printf("USAGE: ./swordx [OPTIONS] [INPUTS]");
}

void printHelp () {
    printf("help\n");
}
