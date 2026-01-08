#ifndef IO_H
#define IO_H

#define NULL     ((void*) 0)
#define EOF      (-1)
#define BUFSIZE  1024
#define OPEN_MAX 20 // max opened files

typedef struct _iobuf {
  int count;  // count last symbols
  char* ptr;  // next pos
  char* base; // place of buf
  int fd;     // descryptor file

  struct {
    unsigned int _READ  : 1;
    unsigned int _WRITE : 1;
    unsigned int _UNBUF : 1;
    unsigned int _EOF   : 1;
    unsigned int _ERR   : 1;
  } flags;
} FILE;

extern FILE _iob[OPEN_MAX];

#define stdin     (&_iob[0])
#define stdout    (&_iob[1])
#define stderr    (&_iob[2])

#define SEEK_SET  0
#define SEEK_CUR  1
#define SEEK_END  2

#define feof(P)   ((P)->flags._EOF)
#define ferror(P) ((P)->flags._ERR)

int _fillbuf(FILE*), _flushbuf(int, FILE*);
int fflush(FILE*);

FILE* fopen(char*, char*);
int fclose(FILE*), fseek(FILE*, long, int);

#define getc(P)    (--(P)->count >= 0 ? (unsigned char) *(P)->ptr++ : _fillbuf(P))
#define putc(X, P) (--(P)->count >= 0 ? *(P)->ptr++ = (X) : _flushbuf((X), (P)))

#define getchar()  getc(stdin)
#define putchar(X) putc((X), stdout)

#endif
