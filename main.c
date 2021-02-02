#include <stdio.h>
#include <stdlib.h>
#include "args_parser.h"

int main(int argc, char* argv[]) {
    char* ofileName = NULL;
    parsed_args_t* pat = parseArgs(argc, argv, &ofileName);
    if (pat->useOutputFile)
        printf("output file: %s\n", ofileName);
    return 0;
}