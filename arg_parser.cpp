#include "arg_parser.h"
#include <iostream>
#include <unistd.h>

/* Strings to make formatting usage output easier */
const char *usage_str =
    "Usage:\n"
    "\tmycc -mode [options] infile\n\n"
    "Valid modes:\n"
    "\t-0: Version information\n"
    "\t-1: Part 1\n"
    "\t-2: Part 2\n"
    "\t-3: Part 3\n"
    "\t-4: Part 4\n"
    "\t-5: Part 5 (not implemented yet)\n"
    "\t-6: Output Syntax Tree (for debugging purposes)\n"
    "\t-7: Output Bound Tree (for debugging purposes)\n"
    "\t-8: General Purpose Debugging Output (subject to change behavior "
    "whenever)\n\n"
    "Valid options:\n"
    "\t-o outfile: write to outfile instead of standard output\n";

Arguments::Arguments(CompilerMode compiler_mode, bool use_out_file,
                     std::vector<std::string> in_files, std::string out_file)
    : m_compiler_mode(compiler_mode), m_use_output_file(use_out_file),
      m_input_filenames(in_files), m_output_filename(out_file) {}

Arguments *Arguments::parse_arguments(int argc, char *argv[]) {
  CompilerMode current_mode = CompilerMode::MODE_ERR;
  bool use_output = false;
  std::vector<std::string> files;
  std::string out_file;

  int option;
  while ((option = getopt(argc, argv, "012345678o:")) != -1) {
    switch (option) {
    case '0':
      current_mode = CompilerMode::MODE_ZERO;
      break;
    case '1':
      current_mode = CompilerMode::MODE_ONE;
      break;
    case '2':
      current_mode = CompilerMode::MODE_TWO;
      break;
    case '3':
      current_mode = CompilerMode::MODE_THREE;
      break;
    case '4':
      current_mode = CompilerMode::MODE_FOUR;
      break;
    case '5':
      current_mode = CompilerMode::MODE_FIVE;
      break;
    case '6':
      current_mode = CompilerMode::MODE_SIX;
      break;
    case '7':
      current_mode = CompilerMode::MODE_SEVEN;
      break;
    case '8':
      current_mode = CompilerMode::MODE_EIGHT;
      break;
    case 'o':
      use_output = true;
      out_file = std::string(optarg);
      break;
    default:
      std::cerr << usage_str;
      exit(EXIT_FAILURE);
    }
  }

  // GNU getopt permutes argv so that all non-option arguments (the input
  // files) end up after optind, regardless of where they appeared.
  for (int i = optind; i < argc; i++)
    files.push_back(std::string(argv[i]));

  if (current_mode == CompilerMode::MODE_ERR ||
      (current_mode != CompilerMode::MODE_ZERO && files.empty())) {
    std::cerr << usage_str;
    exit(EXIT_FAILURE);
  }

  return new Arguments(current_mode, use_output, files, out_file);
}

CompilerMode Arguments::compiler_mode() const { return m_compiler_mode; }

bool Arguments::should_output_file() const { return m_use_output_file; }

std::vector<std::string> Arguments::input_filenames() const {
  return m_input_filenames;
}

std::string Arguments::output_filename() const { return m_output_filename; }
