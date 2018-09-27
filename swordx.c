#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <ctype.h>
#include <stdbool.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>
#include <mcheck.h>
#include "trie.h"
#include "bintree.h"
#include "linkedstack.h"

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

void run(args *options, trie *root, unsigned int flag);
FILE *getFile(const char *path); 
FILE *getoutFile(char *filename);
static inline char *rmNewline(char *name);
static inline void consumeChar(int n, char *word, FILE *f);
static inline void toLow(char *word);
static inline void closeFile(FILE *f);
char *cleanWord(char *word);
void scanFile(const char *file_name, trie *root, args *options, unsigned int flag);
void scanDir(const char *name, trie *root, stack *explude_head, unsigned int flag, args *options);
void scanGlobal(const char *name, trie *root, stack *explude_head, unsigned int flag, args *options);
bool isWordAlpha(char *word);
void pushToLinkedStack(stack **explude_head, FILE *ef);
void printUsage(void);
void printHelp(void);

// get file from a specific path
FILE *getFile(const char *path) {
    FILE *f = fopen(path, "r");
    if (f == NULL)
        perror("Error reading file\n");
    return f;
}

FILE *getoutFile(char *fname) {
    FILE *f = fopen(fname, "ab+");
    if (f == NULL)
        perror("Error reading file\n");
    return f;
}

static inline char *rmNewline(char *name) {
    char *tmp;
    tmp = strchr(name, '\n');
    *tmp = '\0';
    return name;
}

static inline void consumeChar(int n, char *word, FILE *f) {
    free(word);
    n = fscanf(f, "%*[^"WORDCLASS"]");
}

static inline void toLow(char *w) {
    for ( ; *w; ++w) *w = tolower(*w);
}

static inline void closeFile(FILE *f) {
    errno = 0;
    if (fclose(f) == EOF){
        printf("Error closing file: %s\n", strerror(errno));
    }    
}

void run(args *options, trie *root, unsigned int flag) {
    FILE *outfile;
    char word[512];
    outfile = getoutFile(options->output != NULL ? options->output : "swordx.out");
    if (ISFLAGSET(sbo_flag, flag)){
        t_node **tree_root = createTree();
        sboTrie(root, tree_root, word, 0);
        treePrint(*tree_root);
        destroyTree(*tree_root);
        free(tree_root);
    } else writeTrie(root, word, 0, outfile);
    destroyTrie(root);
    fclose(outfile);
}

// check if word contains only alphabetic characters
bool isWordAlpha(char *w) {
    do {
        if (isalpha(*w) == 0)
            return false;
    } while (*++w != '\0');

    return true;
}

// scan file to add word inside the binary tree 
void scanFile(const char *fn, trie *root, args *opt, unsigned int flag) {
    FILE *f;
    char *word;
    int n;
    errno = 0;
    f = getFile(fn);
    do {
        n = fscanf(f, "%*[^"WORDCLASS"]");
        n = fscanf(f, "%m["WORDCLASS"]", &word);
        if (n == 1) {
            toLow(word); 
            printf("Word: %s\n", word);
            if ((ISFLAGSET(alpha_flag, flag) && !isWordAlpha(word)) || (opt->min != 0 && strlen(word) < opt->min)) {
                consumeChar(n, word, f);
                continue;
            }
            addToTrie(root, word);
            consumeChar(n, word, f);
        } else if (errno != 0)
            printf("scanf error: %s", strerror(errno));
    } while (n != EOF);
    closeFile(f);
}

// check every file in directory, if recursive flag is set then
// check the dir recursively
void scanDir(const char *name, trie *root, stack *ex_head, unsigned int flag, args *opt) {
    DIR *dir;
    struct dirent *entry;
    
    if (!(dir = opendir(name)))
        perror("Problem opening dir");
    
    while ((entry = readdir(dir)) != NULL) {
        char *path = (char *) malloc(strlen(name)+strlen(entry->d_name) + 2);
        if (entry->d_type == DT_DIR) { 
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                strcpy(path, name);
                strcat(path, entry->d_name);
                strcat(path, "/");
                printf("dir path: %s\n", path);
                scanDir(path, root, ex_head, flag, opt);
                free(path);
            }
        } 
        else if (entry->d_type == DT_REG && searchStack(ex_head, entry->d_name) == false) {
            strcpy(path, name);
            strcat(path, entry->d_name);
            printf("file path: %s\n",  path);
            scanFile(path, root, opt, flag);
            free(path);
        } 
        else if (entry->d_type == DT_LNK && ISFLAGSET(follow_flag, flag)) {
            char *abs_path;
            strcpy(path, name);
            strcat(path, entry->d_name);
            abs_path = realpath(path, NULL);
            printf("abs path %s\n", abs_path);
            free(path);

            if(abs_path!=NULL) {
                struct stat fi;

                if (stat(abs_path, &fi) == -1) {
                    printf("stat error: %s", strerror(errno));
                }

                switch (fi.st_mode & S_IFMT) {
                    case S_IFDIR:
                        scanDir(abs_path, root, ex_head, flag, opt);
                        free(abs_path);
                        break;
                    case S_IFREG:
                        scanFile(abs_path, root, opt, flag);
                        free(abs_path);
                        break;
                    default:
                        fprintf(stderr, "Unknown file type of %s\n", name);
                }
            } else printf("symbolic link error: %s", strerror(errno)); 
        }
    }
    closedir(dir);
}

void scanGlobal(const char *name, trie *root, stack *ex_head, unsigned int flag, args *opt) {
    struct stat fi;
    char *actual_path;

    if (lstat(name, &fi) == -1) {
        printf("stat error: %s", strerror(errno));
    }

    switch (fi.st_mode & S_IFMT) {
        case S_IFDIR:
            if (ISFLAGSET(recursive_flag, flag))
                scanDir(name, root, ex_head, flag, opt);
            break;
        case S_IFREG:
            scanFile(name, root, opt, flag);
            break;
        case S_IFLNK:
            if (ISFLAGSET(follow_flag, flag)) {
                errno = 0;
                actual_path = realpath(name,NULL);
                if(actual_path!=NULL){
                    scanGlobal(actual_path, root, ex_head, flag, opt);
                    free(actual_path);
                } else printf("symbolic link error: %s", strerror(errno)); 
            }
            break;
        default:
            fprintf(stderr, "Unknown file type of %s\n", name);
        }
    }

void pushToLinkedStack(stack **ex_head, FILE *ef) {
    char *explude_file_name = NULL;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&explude_file_name, &len, ef)) != -1) {
        explude_file_name = rmNewline(explude_file_name);
        push(ex_head, explude_file_name);
    }
    
    if (explude_file_name)
        free(explude_file_name);
    
    fclose(ef);
}

int main (int argc, char **argv) {
    mtrace();
    FILE *ef;
    int opt = 0, long_index = 0;
    unsigned int flag = 0; /* byte flag */
    args *options = (struct args *) malloc(sizeof(struct args));
    stack **ex_head = createStack();
    trie *root = getNode();
    //TODO Initialize args struct (valgrind problem)
    options->min = 0;

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
                ef = getFile(options->explude);
                pushToLinkedStack(ex_head, ef);
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
    
    if (optind < argc) {
        while (optind < argc) {
            char *input = strdup(argv[optind++]);
            scanGlobal(input, root, *ex_head, flag, options);
            free(input);
        }
    }
    run(options, root, flag);
    free(options);
    exit(EXIT_SUCCESS);
}

void printUsage () {
	printf("USAGE: ./swordx [OPTIONS] [INPUTS]");
}

void printHelp () {
    printf("help\n");
}
