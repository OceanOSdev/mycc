#ifndef ARGS_PARSER
#define ARGS_PARSER

typedef enum cmode {
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
} parsed_args_t;

parsed_args_t* parseArgs(int argc, char* argv[], char** outputFileName);

#endif