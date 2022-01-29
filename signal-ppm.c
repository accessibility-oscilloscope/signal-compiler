#include <assert.h>
#include <stdio.h>
#include <string.h>

#define PGM_W 10      // PGM image height
#define PGM_H 10      // PGM image width
#define PGM_DEPTH 255 // PGM maximum gray value

#define STR_EXPAND(tok) #tok
#define STR(tok) STR_EXPAND(tok)

int main(int ac, char *av[]) {
  assert(ac == 3 && "usage: signal-ppm $SIGNAL $PPM");

  FILE *sptr = fopen(av[1], "rb");
  FILE *pptr = fopen(av[2], "wb");

  const char *pgm_init = "P5 " STR(PGM_W) " " STR(PGM_H) " " STR(PGM_DEPTH) " ";
  fwrite(pgm_init, strlen(pgm_init), 1, pptr);

  int c;
  while ((c = fgetc(sptr)) != EOF) {
    unsigned char pgm_linebuf[PGM_W] = {0};
    pgm_linebuf[(c * PGM_W) / 256] = PGM_DEPTH;
    fwrite(pgm_linebuf, sizeof(pgm_linebuf), 1, pptr);
  }
}
