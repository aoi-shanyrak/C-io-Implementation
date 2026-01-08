#include "io.h"

void print_char(char c) {
  putchar(c);
}

void print_str(const char* s) {
  while (*s) {
    putchar(*s++);
  }
}

void print_int(int n) {
  if (n < 0) {
    putchar('-');
    n = -n;
  }
  if (n / 10) {
    print_int(n / 10);
  }
  putchar((n % 10) + '0');
}

void print_test(const char* test_name) {
  print_str("\n=== ");
  print_str(test_name);
  print_str(" ===\n");
}

int main() {
  FILE* fp;
  int c;

  print_str("start of the test io\n");

  // no.1

  print_test("test 1: read & write");

  fp = fopen("test.txt", "w");
  if (fp == NULL) {
    print_str("error: cant open file for writing\n");
    return 1;
  }
  putc('H', fp);
  putc('e', fp);
  putc('l', fp);
  putc('l', fp);
  putc('o', fp);
  putc('!', fp);
  putc('\n', fp);

  fclose(fp);

  fp = fopen("test.txt", "r");
  if (fp == NULL) {
    print_str("error: cant open file for reading\n");
    return 1;
  }

  print_str("reading: ");
  while ((c = getc(fp)) != EOF) {
    putchar(c);
  }
  putchar('\n');

  fclose(fp);
  print_str("test 1 is done\n");

  // no.2

  print_test("test 2: append");

  fp = fopen("test.txt", "a");
  if (fp) {
    putc('A', fp);
    putc('p', fp);
    putc('p', fp);
    putc('e', fp);
    putc('n', fp);
    putc('d', fp);
    putc('!', fp);
    putc('\n', fp);
    fclose(fp);
    print_str("added 'Append!'\n");
  }
  fp = fopen("test.txt", "r");
  print_str("file now: ");
  while ((c = getc(fp)) != EOF) {
    putchar(c);
  }
  fclose(fp);
  print_str("test 2 is done\n");

  // no.3

  print_test("test 3: fseek");

  fp = fopen("test.txt", "r");
  if (fp) {
    if (fseek(fp, 2, SEEK_SET) == 0) {
      print_str("fseek to pos 2: ");
      c = getc(fp);
      putchar(c);
      putchar('\n');

      fseek(fp, -3, SEEK_END);
      print_str("last 3 chars: ");
      while ((c = getc(fp)) != EOF) {
        putchar(c);
      }
    }
    fclose(fp);
  }
  print_str("test 3 is done\n");

  // no.4

  print_test("test 4: fflush");

  fp = fopen("fflush_test.txt", "w");
  if (fp) {
    putc('F', fp);
    putc('L', fp);
    putc('U', fp);
    putc('S', fp);
    putc('H', fp);

    if (fflush(fp) != EOF) {
      print_str("fflush done\n");
    }
    putc('!', fp);
    fclose(fp);

    fp = fopen("fflush_test.txt", "r");
    print_str("inside: ");
    while ((c = getc(fp)) != EOF) {
      putchar(c);
    }
    putchar('\n');
    fclose(fp);
  }
  print_str("test 4 is done\n");

  // no.5

  print_test("test 5: multiply files");

  FILE* fp1 = fopen("file1.txt", "w");
  FILE* fp2 = fopen("file2.txt", "w");

  if (fp1 && fp2) {
    putc('1', fp1);
    putc('a', fp2);
    putc('2', fp1);
    putc('b', fp2);

    fclose(fp1);
    fclose(fp2);

    print_str("created 2 files\n");
  }
  print_str("test 5 is done\n");

  // no.6

  print_test("test 6: stderr (no buffering)");

  putc('E', stderr);
  putc('R', stderr);
  putc('R', stderr);
  putc('O', stderr);
  putc('R', stderr);
  putc(':', stderr);
  putc(' ', stderr);
  putc('T', stderr);
  putc('e', stderr);
  putc('s', stderr);
  putc('t', stderr);
  putc(' ', stderr);
  putc('m', stderr);
  putc('e', stderr);
  putc('s', stderr);
  putc('s', stderr);
  putc('a', stderr);
  putc('g', stderr);
  putc('e', stderr);
  putc('\n', stderr);

  print_str("test 6 is done\n");

  // no.7

  print_test("test 7: feof & ferror");

  fp = fopen("test.txt", "r");
  if (fp) {
    while (getc(fp) != EOF)
      ;
    if (feof(fp))
      print_str("feof is set\n");
    if (!ferror(fp))
      print_str("ferror is not set (good)\n");

    fclose(fp);
  }
  print_str("test 7 is done\n");

  print_str("\n=== all is good! ===\n");

  fflush(stdout);

  return 0;
}