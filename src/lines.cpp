#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using std::string;
using std::size_t;
using std::filesystem::path;
using std::filesystem::directory_iterator;
using std::getline;
using std::ifstream;
using std::filesystem::current_path;
using std::vector;

size_t lines(path dir, vector<string> exts, bool verbose) {
  size_t total_lines = 0;
  for (const auto &entry : directory_iterator(dir)) {
    if (entry.is_directory()) {
      total_lines += lines(entry.path(), exts, verbose);
    } else {
      if (!exts.empty() && std::find(exts.begin(), exts.end(), entry.path().extension()) == exts.end()) {
        continue;
      } else if (entry.is_block_file() || entry.is_character_file() || entry.is_fifo() || entry.is_socket() || entry.path().extension() == ".exe") {
        continue;
      }

      ifstream file(entry.path());

      size_t file_lines = 0;
      string line;
      
      while (getline(file, line)) {
        file_lines++;
      }

      if (verbose)
        printf("%s: %zu\n", entry.path().c_str(), file_lines);
      total_lines += file_lines;

      file.close();
    }
  }
  return total_lines;
}

int main(int argc, char *argv[]) {
  vector<string> exts = {};
  bool verbose = false;
  if (argc > 1) {
    for (int i = 1; i < argc; i++) {
      if (strcmp(argv[i], "-v") == 0) {
        verbose = true;
      }

      if (argv[i][0] == '.') {
        exts.push_back(argv[i]);
      } else {
        std::cout << "bad argument: " << argv[i] << std::endl;
        exit(EXIT_FAILURE);
      }
    } 
  }

  path cwd = current_path();
  printf("Total lines: %zu\n", lines(cwd, exts, verbose));
  return EXIT_SUCCESS;
} // main
