#ifndef ARGS_PARSER
#define ARGS_PARSER

typedef enum cmode {
    MODE_ERR,
    MODE_ZERO,
    MODE_ONE,
    MODE_TWO,
    MODE_THREE,
    MODE_FOUR,
    MODE_FIVE
} mode_e;

typedef struct parsed_args {
    mode_e mode;
    int useOutputFile; // 0 if false, 1 if true
    char** inputFiles;
    int numFiles;
} parsed_args_t;

/*
 * Parses the command line arguments passed in to mycc.
 * 
 * argc: Number of arguments
 * argv: Array of arguments
 * outputFileName: If '-o' gets passed with a file, this will store the file name
 */
parsed_args_t* parseArgs(int argc, char* argv[], char** outputFileName);

#endif