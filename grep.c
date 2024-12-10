#include "grep.h"
#define _GNU_SOURCE

void append_pattern(char *pattern, const char *new_pattern, int *e_count);
void grep_func(const char *filename, Options *flags, char *pattern, FILE *file);
void openfile(int argc, char **argv, char *pattern, Options *flags);

int main(int argc, char **argv) {
  char pattern[BUFFSIZE] = {0};  // Buffer storing the search pattern
  Options flags = {0};
  int parsed_opt;   // Variable to store options parsed by getopt
  int e_count = 0;  // Counter for multiple `-e` patterns

  while ((parsed_opt = getopt(argc, argv, OPTSTRING)) != -1) {
    switch (parsed_opt) {
      case 'e':
        flags.e = 1;
        append_pattern(pattern, optarg, &e_count);
        break;
      case 'i':
        flags.i = 1;
        break;
      case 'v':
        flags.v = 1;
        break;
      case 'c':
        flags.c = 1;
        break;
      case 'l':
        flags.l = 1;
        break;
      case 'n':
        flags.n = 1;
        break;
    }
  }
  // no `-e` pattern is specified
  if (!flags.e && optind < argc) {
    strcpy(pattern, argv[optind++]);
    // Copy and save the next argument in pattern
  }
  openfile(argc, argv, pattern, &flags);
  return 0;
}

void append_pattern(char *pattern, const char *new_pattern, int *e_count) {
  if (*e_count > 0) {
    strcat(pattern, "|");
  }
  strcat(pattern, new_pattern);
  *e_count += 1;  // the count of patterns in e flags
}

void openfile(int argc, char **argv, char *pattern, Options *flags) {
  flags->files_count = argc - optind;  // Count the remaining arguments as files

  // Loop through all specified files
  for (; optind < argc; optind++) {
    FILE *file;
    if ((file = fopen(argv[optind], "rt"))) {
      grep_func(argv[optind], flags, pattern, file);
      fclose(file);
    } else {
      printf("grep: %s: No such file or directory\n", argv[optind]);
    }
  }
}

// int regcomp(regex_t *preg, const char *regex, int cflags);
// int regexec(const regex_t *preg, const char *string, size_t nmatch,
// regmatch_t pmatch[], int eflags);
// cflags: REG_EXTENDED, REG_ICASE
// pmatch и nmatch - адреса внутристрочных совпадений; можно игнорить

void grep_func(const char *filename, Options *flags, char *pattern,
               FILE *file) {
  char line[BUFFSIZE] = {0};  // Buffer for lines from the file

  regex_t preg;  // pointer to a compiled regular expression
  int ignore_case = REG_EXTENDED;
  if (flags->i) ignore_case |= REG_ICASE;
  regcomp(&preg, pattern, ignore_case);

  int matched_lines = 0;  // Counter for matching lines
  int lines_counter = 1;  // Line number counter

  // Read each line of the file, where
  // sizeof(line): Maximum size of the buffer to prevent overflow.
  // file: Pointer to the file being read.
  while (fgets(line, sizeof(line), file) != NULL) {
    // And check if the line matches the regex
    // int regexec(const regex_t *preg, const char *string,
    // size_t nmatch, regmatch_t *pmatch, int eflags);
    int success = regexec(&preg, line, 0, NULL, 0);

    if (flags->v) {
      success = !success;
    }

    if (success == 0) {
      // It's a match, т.к. regexec возвращает ноль при совпадениях
      if (!flags->c && !flags->l) {
        if (flags->files_count > 1) {
          printf("%s:", filename);
        }
        if (flags->n) {
          printf("%d:", lines_counter);
        }
        printf("%s", line);
      }
      matched_lines++;
    }
    lines_counter++;
  }

  if (flags->c) {
    if (flags->files_count > 1) {
      printf("%s:", filename);
    }
    printf("%d\n", matched_lines);
  }

  if (flags->l && matched_lines) {
    printf("%s\n", filename);
  }

  regfree(&preg);
}
