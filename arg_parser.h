#ifndef ARG_PARSER
#define ARG_PARSER
#include <vector>
#include <string>

enum CompilerMode: int {
    MODE_ERR,
    MODE_ZERO,
    MODE_ONE,
    MODE_TWO,
    MODE_THREE,
    MODE_FOUR,
    MODE_FIVE,
    MODE_SIX,
    MODE_SEVEN,
    MODE_EIGHT
};

class Arguments {
private:
    CompilerMode m_compiler_mode;
    bool m_use_output_file;
    std::vector<std::string> m_input_filenames;
    std::string m_output_filename;
    Arguments(CompilerMode compiler_mode, bool use_out_file, std::vector<std::string> in_files, std::string out_file);
public:
    static Arguments* parse_arguments(int argc, char* argv[]);

    CompilerMode compiler_mode() const;
    bool should_output_file() const;
    std::vector<std::string> input_filenames() const;
    std::string output_filename() const;
};

#endif