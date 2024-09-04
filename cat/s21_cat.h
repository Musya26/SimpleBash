#include <errno.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct {
  bool b;  // nonblank
  bool e;
  bool n;  // numberAll
  bool s;  // squeeze
  bool t;
  bool v;
  bool T;
  bool E;
} Flags;

Flags parser(int argc, char *argv[]);
char v_output(char ch);
void outline(Flags *arg, char *line, int n);
void output(Flags *arg, char *argv[]);
