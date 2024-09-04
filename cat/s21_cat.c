#include "s21_cat.h"

Flags parser(int argc, char *argv[]) {
  struct option long_option[] = {// GNU версии флагов
                                 {"number-nonblank", 0, NULL, 'b'},
                                 {"squeeze-blank", 0, NULL, 's'},
                                 {"number", 0, NULL, 'n'},
                                 {NULL, 0, NULL, 0}};
  int currentFlag = getopt_long(argc, argv, "+benstvTE", long_option,
                                NULL);  //результат парсинга
  Flags flags = {false, false, false, false, false, false, false, false};
  for (; currentFlag != -1;
       currentFlag = getopt_long(
           argc, argv, "+benstvTE", long_option,
           NULL)) {  //-1 выводит,когда все прошел и не дал результатов
    switch (currentFlag) {
      case 'b':
        flags.b = true;
        break;
      case 'n':
        flags.n = true;
        break;
      case 's':
        flags.s = true;
        break;
      case 'v':
        flags.v = true;
        break;
      case 'T':
        flags.T = true;
        break;
      case 'E':
        flags.E = true;
        break;
      case 'e':
        flags.e = true;
        flags.v = true;
        break;
      case 't':
        flags.t = true;
        flags.v = true;
        break;
      case '?':
        perror("error");
        break;
        // default:
        // fprintf(stderr,"usage"benstv);
    }
  }
  return flags;
}

/*int count_flags(int argc, char *argv[]){
  int k = 1;
    for (int i = 0; i < argc; i++){
    if((argv[i][0] == '-')){
      k++;
    }
  }
  return k;
}*/

/*void pars (int argc, char *argv[], char **mass_flags, int count)
{
  int i = count - 1;
  mass_flags[0] = argv[0];
  for (int j = 0; j<argc; j++){
    if((argv[j][0] == '-')){
      mass_flags[count - i] = argv[j];
      i--;
    }
  }
}*/

char v_output(char ch) {  //вывод непечатаемых символов
  if (ch != '\n' && ch != '\t') {
    if (ch < 0) {
      printf("M-");
      ch = ch & 0x7F;
    } else if (ch <= 31) {
      putchar('^');
      ch += 64;
    } else if (ch == 127) {
      putchar('^');
      ch = '?';
    }
  }
  return ch;
}

void outline(Flags *arg, char *line, int n) {  //функция вывода строки на экран
  // printf("%d %d", arg->v, arg->E);
  for (int i = 0; i < n; i++) {
    if (arg->v || arg->t || arg->e) line[i] = v_output(line[i]);
    if ((arg->T || arg->t) && line[i] == '\t')
      printf("^I");
    else {
      if ((arg->E || arg->e) && line[i] == '\n') putchar('$');
      putchar(line[i]);  //вывод строки
    }
  }
}

/*int count_files(int argc, char *argv[])
{
  int k =0;//количество файлов
  for (int i =0; i<argc; i++){
    if((argv[i][0]!='-')&&(argv[i]!="number-noblank")&&(argv[i]!="ssqueeze-blank")&&(argv[i]!="number")){
      k++;
    }
  }
  return k;
}*/

/*void file (int argc, char *argv[], char **res, int k)
{
  int i = k;
  for (int j = 1; j<argc; j++){
    if((argv[j][0]!='-')&&(argv[j]!="--number-noblank")&&(argv[j]!="--ssqueeze-blank")&&(argv[j]!="--number")){
      res[k - i] = argv[j];
      i--;
    }
  }
}*/

void output(Flags *arg, char *argv[]) {
  // int line_count = 1;
  // int k = count_files(argc, argv);
  // char *mass[k];
  // file(argc, argv, mass, k);
  // for (int i =0; i<k; i++){
  FILE *f;  //?optind - глобальная переменная пути до файла
  f = fopen(argv[optind], "r");
  if (f) {
    //  if (!first_file && !arg->e)
    // printf("\n");
    char *line = NULL;   // хранение строки
    size_t memline = 0;  //хранение размера памяти выделенного под строку
    int read = 0;  // количество считываемых символов
    int line_count = 1;  //для нумерации строк
    int empty_count = 0;
    read = getline(&line, &memline,
                   f);  //считываем строку, будет -1 если дойдет до конца
    while (read > 0) {
      if (line[0] == '\n')
        empty_count++;  // flag s ???????
      else
        empty_count = 0;
      if (arg->s && empty_count > 1) {
      } else {
        if (arg->n || arg->b) {
          if (arg->b) {
            if (line[0] != '\n') {
              printf("%6d\t", line_count);
              line_count++;
            }
          } else if (arg->n) {
            printf("%6d\t", line_count);
            line_count++;
          }
        }
        outline(arg, line, read);
      }
      read = getline(&line, &memline, f);  // считываем следущую строку
    }
    free(line);
    fclose(f);
    optind++;
  }
}

int main(int argc, char *argv[]) {
  // Flags parser;
  if (argc > 1) {
    // int flags_count = count_flags(argc, argv);
    // char* mass_flags[flags_count];
    // pars(argc, argv, mass_flags, flags_count);
    //  printf("%d\n", flags_count);
    //  for (int i = 0; i < flags_count; i++)
    //  {
    //    printf("%s\n", mass_flags[i]);
    //  }
    Flags arg = parser(argc, argv);  //флаги
    int k = argc - optind;           // количество файлов
    // int n = optind;  //позицияб с которой начинаются файлы
    // int line_count = 1;
    // bool first_file = true;
    // mass(argc, argv);
    while (k) {
      output(&arg, argv);
      k--;
      // n++;
      // first_file = false;
    }
  }
  return 0;
}
