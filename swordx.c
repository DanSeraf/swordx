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
#include "linkedlist.h"

#define recursive_flag  (1<<0)
#define follow_flag     (1<<1)
#define alpha_flag      (1<<2)
#define sbo_flag        (1<<3)

#define WORDCLASS "a-zA-Z0-9"

// return value are nonzero if flag is set
#define ISFLAGSET(bitopt, flag) (flag & bitopt)

// struct to store optionss
typedef struct args {
    char *explude;
    char *ignore;
    char *log;
    char *output;
    unsigned int min;
} args;

void run(args *options, unsigned int flag);
FILE *getFile(char *path); 
FILE *getoutFile(char *filename);
char *cleanWord(char *word);
void scanFile(FILE *f, t_node **root, args *options, unsigned int flag);
void scanDir(const char *name, t_node **root, args *options, unsigned int flag);
bool isWordAlpha(char *word);
void printUsage(void);
void printHelp(void);

// get file from a specific path
FILE *getFile(char *path) {
    FILE *f = fopen(path, "r");
    if (f == NULL)
        perror("Error reading file");
    return f;
}

FILE *getoutFile(char *fname) {
    FILE *f = fopen(fname, "ab+");
    if (f == NULL)
        perror("Error reading file");
    return f;
}

void run(args *options, unsigned int flag) {
    FILE *outfile;
    t_node **root = createTree();
    scanDir("./test", root, options, flag);
    outfile = getoutFile(options->output != NULL ? options->output : "swordx.out");
    if (ISFLAGSET(sbo_flag, flag) == 0)
        writeTree(*root, outfile);
    else {
        l_list **head = createList();
        addToList(*root, head);
        sortByOccurrences(head);
        writeList(*head, outfile);
        //printList(*head);
        destroyList(*head);
        free(head);
    }
    destroyTree(*root);
    free(root);
}

// check if word contains only alphabetic characters
bool isWordAlpha(char *w) {
    do {
        if (isalpha(*w) == 0)
            return false;
    } while (*++w != '\0');
    return true;
}

// Scan file to add word inside the binary tree 
void scanFile(FILE *f, t_node **root, args *opt, unsigned int flag) {
    char *word;
    int n;
    errno = 0;
    do {
        n = fscanf(f, "%m["WORDCLASS"]", &word);
        if (n == 1) {
            printf("Word: %s\n", word);
            if ((ISFLAGSET(alpha_flag, flag) && !isWordAlpha(word)) || strlen(word) < opt->min) {
                free(word);
                n = fscanf(f, "%*[^"WORDCLASS"]");
                continue;
            }
            addToTree(root, word);
            free(word);
            n = fscanf(f, "%*[^"WORDCLASS"]");
        } else if (errno != 0)
            perror("Error in scanf");
    } while (n != EOF);
    fclose(f);
}

// check every file in directory, if recursive flag is set then
// check the dir recursively
void scanDir(const char *name, t_node **root, args *opt, unsigned int flag) {
    FILE *f;
    DIR *dir;
    struct dirent *entry;

    if (!(dir = opendir(name)))
        perror("Problem opening dir");
    
    while ((entry = readdir(dir)) != NULL) {
                char path[1024];
                if (entry->d_type == DT_DIR && ISFLAGSET(recursive_flag, flag) != 0) {
                    if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                        sprintf(path, "%s/%s", name, entry->d_name);
                        printf("dir path: %s\n", path);
                        scanDir(path, root, opt, flag);
                    }
                } else if (entry->d_type == DT_REG) {
                    sprintf(path, "%s/%s", name, entry->d_name);
                    printf("file path: %s\n",  entry->d_name);
                    f = getFile(path);
                    scanFile(f, root, opt, flag);
                }
        }
    closedir(dir);
}


int main (int argc, char **argv) {
    mtrace();
    int opt = 0;
    int long_index = 0;
    struct args *options = (struct args *) malloc(sizeof(struct args));
    //struct stack *inputs = (stack *) malloc(sizeof(stack));

    unsigned int flag = 0; /* byte flag */
    
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
            case 'e': options->explude = strdup(optarg);
                break;
            case 'm': options->min = atoi(optarg);
                break; 
            case 'i': options->ignore = strdup(optarg);
                break; 
            case 'l': options->log = strdup(optarg);
                break;
            case 'o': options->output = strdup(optarg);
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
            //TODO put elements inside the struct
    }
    run(options, flag);
    free(options);
    exit(EXIT_SUCCESS);
}

void printUsage () {
	printf("USAGE: ./swordx [OPTIONS] [INPUTS]");
}

void printHelp () {
    printf("help\n");
}
