#include <assert.h>
#include <fcntl.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// note PGM_H / PGM_W ~ 16/10, resolution of wacom tablet
#define PGM_H 300     // PGM image height
#define PGM_W 480     // PGM image width
#define PGM_DEPTH 255 // PGM maximum gray value
#define GRADIENT 25   // PGM gradient depth

#define STR_EXPAND(tok) #tok
#define STR(tok) STR_EXPAND(tok)

unsigned udiff(unsigned a, unsigned b) {
  unsigned axb = a ^ b;
  if (a < b)
    axb = 0;
  return (axb ^ b) - (axb ^ a); // a-b, or b-a
}

int better_read(const int fd, void *buf, const int amount) {
  int amt_read = 0;
  while (amt_read < amount) {
    const int rv = read(fd, (char *)buf + amt_read, amount - amt_read);
    amt_read += rv;
  }
  return amt_read;
}

int main(int ac, char *av[]) {
  assert(ac == 3 && "usage: signal-ppm $SIGNAL $PPM");

  int sptr = open(av[1], O_RDONLY);
  int pptr = open(av[2], O_WRONLY);

  while (1) {
      const char *pgm_init = "P5 " STR(PGM_W) " " STR(PGM_H) " " STR(PGM_DEPTH) " ";
      write(pptr, pgm_init, strlen(pgm_init));

      uint8_t sbuf[PGM_W] = {0};
      uint8_t pgm_buf[PGM_W][PGM_H] = {0};

      const int rv = better_read(sptr, sbuf, sizeof(sbuf));
      (void) rv; // FIXME

      printf("read signal\n");

      for (unsigned sx = 0; sx < PGM_W; sx++) {
          pgm_buf[sx][sbuf[sx]] = PGM_DEPTH;
      }

      for (unsigned sx = 0; sx < PGM_W; sx++) {
          const unsigned sy = sbuf[sx];
          // printf("%d %d\n", sx, sy);

          // fill in gradient in square around sampled point
          for (signed dx = -GRADIENT; dx <= GRADIENT; dx++) {
              for (signed dy = -GRADIENT; dy <= GRADIENT; dy++) {
                  // {sx + dx, sy + dy}
                  if (0 <= (signed) sx + dx && (signed) sx + dx < PGM_W &&
                      0 <= (signed) sy + dy && (signed) sy + dy < PGM_H) {
                      uint8_t *p = &pgm_buf[sx + dx][sy + dy];
                      const uint8_t new = PGM_DEPTH - (PGM_DEPTH / GRADIENT / sqrt(2)) *
                                                      sqrt(pow(dx, 2) + pow(dy, 2));
                      if (*p == 0)
                          *p = new;
                      else
                          *p = *p > new ? *p : new;
                  }
              }
          }
      }

      // - write(pptr, pgm_buf, PGM_H * PGM_W)
      // - write(pptr, pgm_buf[ii], PGM_H)
      // - write(pptr, &pgm_buf[ii][jj], 1)   <-- use this one (byte-order reasons)
      for (unsigned jj = 0; jj < PGM_H; jj++) {
          for (unsigned ii = 0; ii < PGM_W; ii++) {
              write(pptr, &pgm_buf[ii][jj], 1);
          }
      }
      printf("wrote %ui to pgm\n", sizeof(pgm_buf));
  }
  close(pptr);
  close(sptr);
}
