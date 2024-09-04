#include "s21_grep.h"

// BuffSize 4096

int main(int argc, char *argv[]) {
  struct s_flags flag = {0};
  char patternE[BUFSIZ] = {0};

  if (argc > 1) {
    ParsFlag(argc, argv, &flag, patternE);
    if (!flag.e && !flag.f) {
      if (!*argv[optind]) argv[optind] = ".";
      strcat(patternE, argv[optind]);  // Объединение строк.Cкопировать строку
                                       // "argv[optind]" в patternE
      optind += 1;
    }
    OpenFile(argc, argv, flag, patternE);
  }
}

void ParsFlag(int argc, char *argv[], struct s_flags *flag, char *patternE) {
  int opt = 0;
  int e_count = 0;
  char *optstring = "e:ivclnhsf:o";

  while ((opt = getopt_long(argc, argv, optstring, NULL, NULL)) != -1) {
    switch (opt) {
      case 'e':
        flag->e = 1;
        e_flag(optarg, &e_count, patternE, flag);
        break;
      case 'i':
        flag->i = 1;
        break;
      case 'v':
        flag->v = 1;
        break;
      case 'c':
        flag->c = 1;
        break;
      case 'l':
        flag->l = 1;
        break;
      case 'n':
        flag->n = 1;
        break;
      case 'h':
        flag->h = 1;
        break;
      case 's':
        flag->s = 1;
        break;
      case 'f':
        flag->f = 1;
        f_flag(optarg, &e_count, patternE, flag);
        break;
      case 'o':
        flag->o = 1;
        break;
      default:
        exit(1);
    }
  }
  if (flag->empty_lines) flag->o = 0;
}

void e_flag(char *optarg, int *e_count, char *patternE, struct s_flags *flag) {
  if (*e_count) {
    char *ch = "|";
    strcat(patternE, ch);
  }
  if (!*optarg) {
    optarg = ".";
    flag->empty_lines++;
  }
  strcat(patternE, optarg);
  *e_count += 1;
}

void f_flag(char *optarg, int *e_count, char *patternE, struct s_flags *flag) {
  FILE *file = NULL;
  int empty_counter = flag->empty_lines;

  if ((file = fopen(optarg, "r")) == NULL) {
    printf("grep: %s: No such file or directory\n", optarg);
    exit(2);
  } else {
    while (!feof(file)) {
      if (*e_count) {
        char *ch = "|";
        strcat(patternE, ch);
      }

      size_t len = strlen(patternE);
      size_t new_len;

      fgets(patternE + len, BuffSize, file);
      char *nch = strrchr(patternE, 10);
      if (nch) *nch = '\0';
      new_len = strlen(patternE);
      if (new_len == len) {
        flag->empty_lines++;
        char *all = ".";
        strcat(patternE, all);
      }
      *e_count += 1;
    }
    fclose(file);
  }
  if (patternE[strlen(patternE) - 1] == '.') {
    if ((flag->empty_lines - empty_counter) == 1)
      flag->empty_lines = empty_counter;
    patternE[strlen(patternE) - 1] = '\0';
    patternE[strlen(patternE) - 1] = '\0';
  }
}

void OpenFile(int argc, char *argv[], struct s_flags flag, char *patternE) {
  // optind количество «съеденных» аргументов
  int files_count = argc - optind;  // уменьшает количество аргументов на optind

  while (optind < argc) {
    FILE *file;
    if ((file = fopen(argv[optind], "r")) != NULL) {
      RegexComp(argv, patternE, file, flag, files_count);
      fclose(file);
    } else if (!flag.s) {
      printf("grep: %s: No such file or directory\n", argv[optind]);
    }
    ++optind;
  }
}

void RegexComp(char *argv[], char *patternE, FILE *file, struct s_flags flag,
               int files_count) {
  regex_t regex;  // сохронение скомпилированноого регулярного выражения.
  char buff_str[BuffSize];       // Массив строк в файле
  regmatch_t regmatch[1] = {0};  // Поиск строки
  size_t nmatch =
      1;  // это количество подстрок в строке , которые regexec()функция должна
          // попытаться сопоставить с подвыражениями в preg
  size_t file_line = 1;
  int status;

  int lines_count = 0;
  int i_options = 0;

  if (flag.i) i_options = REG_ICASE;
  status = regcomp(&regex, patternE, REG_EXTENDED | i_options);

  while (!feof(file)) {
    if (fgets(buff_str, BuffSize, file)) {
      int new_line_o_counter = 1;
      status = regexec(&regex, buff_str, nmatch, regmatch, 0);
      if (flag.v) status = status ? 0 : 1;

      if (!status) {
        if (!flag.c && !flag.l) {
          if (files_count > 1 && !flag.h) {
            printf("%s:", argv[optind]);
          }
          if (flag.n) {
            printf("%lu:", file_line);
          }
          if (flag.o && !flag.v) {
            new_line_o_counter = 0;
            char *pattern = buff_str;
            while (!status) {
              if (regmatch[0].rm_eo == regmatch[0].rm_so) break;
              printf("%.*s\n", (int)(regmatch[0].rm_eo - regmatch[0].rm_so),
                     pattern + regmatch[0].rm_so);
              pattern += regmatch[0].rm_eo;
              status = regexec(&regex, pattern, nmatch, regmatch, REG_NOTBOL);
            }
          }
          if (!flag.o || flag.v) printf("%s", buff_str);
          if (buff_str[strlen(buff_str) - 1] != '\n' && new_line_o_counter) {
            printf("\n");
          }
        }
        lines_count++;
      }
      file_line++;
    }
  }
  if (flag.c) {
    if (files_count > 1 && !flag.h) printf("%s:", argv[optind]);
    if (flag.l && lines_count)
      printf("1\n");
    else
      printf("%d\n", lines_count);
  }
  if (flag.l && lines_count) {
    printf("%s\n", argv[optind]);
  }
  regfree(&regex);
}
