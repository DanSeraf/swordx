#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <ctype.h>
#include <stdbool.h>
#include <dirent.h>
#include <errno.h>
#include <assert.h>
#include <mcheck.h>
#include "bintree.h"

#define recursive_flag  (1<<0)
#define follow_flag     (1<<1)
#define alpha_flag      (1<<2)
#define sbo_flag        (1<<3)

/* "const" struct to store options */
typedef const struct args {
    char *explude;
    char *ignore;
    char *log;
    char *output;
    unsigned int min;
} args;

void run(args *option, int flag);
FILE *getFile(char *path); /* Get file from path */
void scanFile(FILE *f, t_node **root);
void scanDir(const char *name, t_node **root, int flag);
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

void run(args *option, int flag) {
    t_node **root = createTree();
    scanDir("./test", root, flag);
    treePrint(root);
    destroyTree(*root);
    free(root);
}

// Scan file to add word inside the binary tree 
void scanFile(FILE *f, t_node **root) {
    char *word;
    int n;
    errno = 0;
    do {
        n = fscanf(f, "%ms", &word);
        if (n == 1) {
            printf("Word: %s\n", word);
            addToTree(root, word);
            free(word);
        } else if (errno != 0)
            perror("Error in scanf");
    } while (n != EOF);
    fclose(f);
}

void scanDir(const char *name, t_node **root, int flag) {
    FILE *f;
    DIR *dir;
    struct dirent *entry;

    if (!(dir = opendir(name)))
        perror("Problem opening dir");
    
    while ((entry = readdir(dir)) != NULL) {
                char path[1024];
                if (entry->d_type == DT_DIR && isFlagSet(recursive_flag, flag) != 0) {
                    if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                        sprintf(path, "%s/%s", name, entry->d_name);
                        printf("dir path: %s\n", path);
                        scanDir(path, root, flag);
                    }
                } else if (entry->d_type == DT_REG) {
                    sprintf(path, "%s/%s", name, entry->d_name);
                    printf("file path: %s\n",  entry->d_name);
                    f = getFile(path);
                    scanFile(f, root);
                }
        }
    closedir(dir);
}


int main (int argc, char **argv) {
    mtrace();
    int opt = 0;
    int long_index = 0;
    struct args *option = (struct args *) malloc(sizeof(struct args));
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
            case 'e': option->explude = strdup(optarg);
                break;
            case 'm': option->min = atoi(optarg);
                break; 
            case 'i': option->ignore = strdup(optarg);
                break; 
            case 'l': option->log = strdup(optarg);
                break;
        case 'o': option->output = strdup(optarg);
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
    run(option, flag);
    free(option);
    muntrace();
    exit(EXIT_SUCCESS);
}

void printUsage () {
	printf("USAGE: ./swordx [OPTIONS] [INPUTS]");
}

void printHelp () {
    printf("help\n");
}
