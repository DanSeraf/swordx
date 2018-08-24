#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <ctype.h>
#include <stdbool.h>
#include <errno.h>
#include <dirent.h>
#include "bintree.h"
#include "swordx.h"

bool run(struct t_node *, struct args *params, int flag);
bool readWord(FILE *file, struct t_node *);
FILE *getFile(char *path); /* Get file from path */
void getFileFromDir(const char *name, int indent);
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

bool readWord(FILE *f, struct t_node *root) {
    char *word;
    int n;
    errno = 0;

    do {
        n = fscanf(f, "%ms", &word);
        if (n == 1) {
            printf("Word: %s\n", word);
            root = addtree(root, word);
            free(word);
        } else if (errno != 0)
            perror("Error in scanf");
    } while (n != EOF);
    treePrint(root);
    return true;
}

bool run(struct t_node *root, struct args *params, int flag) {
    FILE *f;
    // Check if recursive is on
    if ((isFlagSet(recursive_flag, flag)) != 0)
        getFileFromDir(".", 0);
    f = getFile("./test/test.txt");
    if ((readWord(f, root)) == true)
        fclose(f);
    return true;
}

void getFileFromDir(const char *name, int indent)
{
    DIR *dir;
    struct dirent *entry;

    if (!(dir = opendir(name)))
        return;

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
            char path[1024];
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            snprintf(path, sizeof(path), "%s/%s", name, entry->d_name);
            printf("%*s[%s]\n", indent, "", entry->d_name);
            getFileFromDir(path, indent + 2);
        } else {
            printf("%*s- %s\n", indent, "", entry->d_name);
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
    printf("flag opt: %d\n", flag);
    if (optind < argc) {
        printf("OPTIND variable: %d\n", optind);
        printf("other arguments, probably dir:\n");
        while (optind < argc) 
            printf("%s\n", argv[optind++]);
    }
    if ((run(root, params, flag)) == true) 
        free(root);
    free(params);
}

void printUsage () {
	printf("USAGE: ./swordx [OPTIONS] [INPUTS]");
}

void printHelp () {
    printf("help\n");
}
