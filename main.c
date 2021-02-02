#include <stdio.h>
#include <stdlib.h>
#include "args_parser.h"

const char* versionInfo =
"My bare-bones C compiler (for COM 440)\n"
"\tWritten by Thomas Maloney (tmaloney@iastate.edu)\n"
"\tVersion 0.1\n"
"\t2 February, 2021\n";

void handleArgs(parsed_args_t* pat, char* oFileName) {
    FILE *fout = pat->useOutputFile 
                    ? fopen(oFileName, "w+")
                    : stdout; 
    
    switch (pat->mode) {
        case MODE_ERR: break; // <-- Should literally never happen
        case MODE_ZERO: fprintf(fout, versionInfo); break;
        case MODE_ONE: break;
        case MODE_TWO: break;
        case MODE_THREE: break;
        case MODE_FOUR: break;
        case MODE_FIVE: break;
    }

    // probably shouldn't close stdout
    if (pat->useOutputFile)
        fclose(fout);
}

int main(int argc, char* argv[]) {
    char* oFileName = NULL;
    parsed_args_t* pat = parseArgs(argc, argv, &oFileName);
    handleArgs(pat, oFileName);
    return 0;
}