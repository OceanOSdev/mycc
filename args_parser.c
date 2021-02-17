#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "args_parser.h"

/* Strings to make formatting usage output easier */
const char* usage_str = 
"Usage:\n"
"\tmycc -mode [options] infile\n\n"
"Valid modes:\n"
"\t-0: Version information\n"
"\t-1: Part 1 (not implemented yet)\n"
"\t-2: Part 2 (not implemented yet)\n"
"\t-3: Part 3 (not implemented yet)\n"
"\t-4: Part 4 (not implemented yet)\n"
"\t-5: Part 5 (not implemented yet)\n\n"
"Valid options:\n"
"\t-o outfile: write to outfile instead of standard output\n";


parsed_args_t* parseArgs(int argc, char* argv[], char** outputFileName) {
    parsed_args_t* pat = malloc(sizeof(parsed_args_t));
    pat->mode = MODE_ERR;
    pat->useOutputFile = 0;
    int fileStartInd = 0;
    int fileCount = 0;
    pat->numFiles = fileCount;
    char** filenames;

    int option;
    while ((option = getopt(argc, argv, "01:2345o:")) != -1) {
        switch (option) {
            case '0': pat->mode = MODE_ZERO; break;
            case '1':
             pat->mode = MODE_ONE; 
             // super hacky, had to take a shower after this one
             fileStartInd = --optind;
             for( ;optind < argc && *argv[optind] != '-'; optind++) {
                 fileCount++;
             }
             filenames = malloc(fileCount * sizeof(char*));
             pat->numFiles = fileCount;
             fileCount = 0;
             optind = fileStartInd;
             for( ;optind < argc && *argv[optind] != '-'; optind++) {
                 filenames[fileCount] = malloc(strlen(argv[optind]) * sizeof(char));
                 filenames[fileCount++] = argv[optind];
             }

             break;
            case '2': pat->mode = MODE_TWO; break;
            case '3': pat->mode = MODE_THREE; break;
            case '4': pat->mode = MODE_FOUR; break;
            case '5': pat->mode = MODE_FIVE; break;
            case 'o': 
                pat->useOutputFile = 1; 
                *outputFileName = optarg;
                break;
            default:
                fprintf(stderr, usage_str);
                exit(EXIT_FAILURE);
        }
    }

    if (pat->mode == MODE_ERR) {
        fprintf(stderr, usage_str);
        exit(EXIT_FAILURE);
    }

    pat->inputFiles = filenames;

    return pat;
}