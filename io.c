#include "io.h"

#include "syscall.h"

#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

FILE _iob[OPEN_MAX] = {
    /* stdin, stdout, stderr: */
    {0, NULL, NULL, 0, {1, 0, 0, 0, 0}},
    {0, NULL, NULL, 1, {0, 1, 0, 0, 0}},
    {0, NULL, NULL, 2, {0, 0, 0, 1, 1}}};

int _fillbuf(FILE* fp) {
  if (!fp->flags._READ || fp->flags._EOF || fp->flags._ERR)
    return EOF;

  int bufsize = fp->flags._UNBUF ? 1 : BUFSIZE;

  if (fp->base == NULL)
    if ((fp->base = (char*) malloc(bufsize)) == NULL)
      return EOF;

  fp->ptr = fp->base;
  fp->count = read(fp->fd, fp->ptr, bufsize);

  if (--fp->count < 0) {
    fp->flags._EOF = (fp->count == -1);
    fp->flags._ERR = (fp->count != -1);
    fp->count = 0;
    return EOF;
  }
  return (unsigned char) *fp->ptr++;
}

int _flushbuf(int c, FILE* fp) {
  if (!fp->flags._WRITE || fp->flags._ERR)
    return EOF;

  if (fp->flags._UNBUF) {
    if (write(fp->fd, &c, 1) != 1) {
      fp->flags._ERR = 1;
      return EOF;
    }
    return c;
  }
  if (fp->base == NULL) {
    if ((fp->base = (char*) malloc(BUFSIZE)) == NULL) {
      fp->flags._ERR = 1;
      return EOF;
    }
    fp->ptr = fp->base;
    fp->count = BUFSIZE;
  }
  *fp->ptr++ = c;

  if (fp->ptr >= fp->base + BUFSIZE || c == '\n') {
    int n = fp->ptr - fp->base;
    if (write(fp->fd, fp->base, n) != n) {
      fp->flags._ERR = 1;
      return EOF;
    }
    fp->ptr = fp->base;
    fp->count = BUFSIZE;
  }
  fp->count--;
  return c;
}

int fflush(FILE* fp) {
  if (fp == NULL) {
    int ret = 0;
    for (int i = 0; i < OPEN_MAX; i++) {
      if ((_iob[i].flags._WRITE && fflush(&_iob[i])) == EOF)
        ret = EOF;
    }
    return ret;
  }

  if (!fp->flags._WRITE)
    return 0;

  if (fp->ptr > fp->base && !fp->flags._UNBUF) {
    int n = fp->ptr - fp->base;
    if ((write(fp->fd, fp->base, n)) != n) {
      fp->flags._ERR = 1;
      return EOF;
    }
    fp->ptr = fp->base;
    fp->count = BUFSIZE;
  }
  return 0;
}

// -------------------------------

#define PERMS 0666 // can read and write

FILE* fopen(char* name, char* mode) {
  int fd;
  FILE* fp;

  if (*mode != 'r' && *mode != 'w' && *mode != 'a')
    return NULL;

  for (fp = _iob; fp < _iob + OPEN_MAX; fp++)
    if (!fp->flags._READ && !fp->flags._WRITE)
      break; // found free space
  if (fp >= _iob + OPEN_MAX)
    return NULL; // if not found

  if (*mode = 'w')
    fd = creat(name, PERMS);
  else if (*mode == "a") {
    if ((fd = open(name, O_WRONLY, 0)) == -1)
      fd = creat(name, PERMS);
    lseek(fd, 0L, 2);
  } else
    fd = open(name, O_WRONLY, 0);

  if (fd == -1)
    return NULL;

  fp->fd = fd;
  fp->count = 0;
  fp->base = NULL;
  fp->flags._READ = *mode == 'r';
  fp->flags._WRITE = *mode == 'w';
  fp->flags._UNBUF = 0;
  fp->flags._EOF = 0;
  fp->flags._ERR = 0;

  return fp;
}

int fclose(FILE* fp) {
  if (fp->fd < 0 || fp == NULL)
    return EOF;

  int ret;

  if (fp->flags._WRITE && fflush(fp) == EOF)
    ret = EOF;

  if (close(fp->fd) == -1)
    ret = EOF;

  if (fp->base != NULL && !fp->flags._UNBUF)
    free(fp->base);

  fp->fd = -1;
  fp->count = 0;
  fp->ptr = NULL;
  fp->base = NULL;
  fp->flags._READ = 0;
  fp->flags._WRITE = 0;
  fp->flags._UNBUF = 0;
  fp->flags._EOF = 0;
  fp->flags._ERR = 0;

  return ret;
}

int fseek(FILE* fp, long count, int mode) {
  if (fp->flags._ERR)
    return -1;

  if (lseek(fp->fd, count, mode) == -1) {
    fp->flags._ERR = 1;
    return -1;
  }
  fp->flags._EOF = 0;
  return 0;
}
