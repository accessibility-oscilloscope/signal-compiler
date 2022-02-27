#include <assert.h>
#include <stdio.h>
#include <string.h>

// note PGM_W / PGM_H ~ 16/10, resolution of wacom tablet
#define PGM_W 300     // PGM image height
#define PGM_H 480     // PGM image width
#define PGM_DEPTH 255 // PGM maximum gray value

#define STR_EXPAND(tok) #tok
#define STR(tok) STR_EXPAND(tok)

int main(int ac, char *av[]) {
  assert(ac == 3 && "usage: signal-ppm $SIGNAL $PPM");

  FILE *sptr = fopen(av[1], "rb");
  FILE *pptr = fopen(av[2], "wb");

  const char *pgm_init = "P5 " STR(PGM_W) " " STR(PGM_H) " " STR(PGM_DEPTH) " ";
  fwrite(pgm_init, strlen(pgm_init), 1, pptr);

  for (unsigned int ii = 0; ii < PGM_H; ii++) {
    unsigned char c = fgetc(sptr);

    unsigned char pgm_linebuf[PGM_W] = {0};
    pgm_linebuf[(c * PGM_W) / 256] = PGM_DEPTH;

    fwrite(pgm_linebuf, sizeof(pgm_linebuf), 1, pptr);
  }

  fclose(pptr);
  fclose(sptr);
}
