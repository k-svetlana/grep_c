#ifndef _GREP_H_
#define _GREP_H_

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define BUFFSIZE 512
#define OPTSTRING "e:ivcln"

typedef struct {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int files_count;
} Options;

#endif /* _GREP_H */
