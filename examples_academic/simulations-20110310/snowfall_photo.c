#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "fbeggx.h"
#include "snowfall.h"

#define L 300
#define MAXSNOW  400

int main(int argc, char **argv)
{
  char bgcolor[] = "#000000", message[80];
  int win, key = 0;
  int i, n, b = 0;
  double snowx[MAXSNOW], snowy[MAXSNOW];
  unsigned char *snowp[6] = {
    Xpm_image_snow0, Xpm_image_snow1, Xpm_image_snow2,
    Xpm_image_snow3, Xpm_image_snow4, Xpm_image_snow5
  };

  srand48(time(NULL));
  for (i = 0; i < MAXSNOW; i++) {
    snowx[i] = L * drand48();
    snowy[i] = L * (1 + drand48());
  }

  gsetinitialbgcolor(bgcolor);
  win = gopen(L, L);
  winname(win, "Snow falls on a tree");
  layer(win, 0, 1);
  newcolor(win, "snow");
  gsetbgcolor(win, "SteelBlue4");
  sprintf(message, "bgcolor: %s\n", bgcolor);
  gsetnonblock(ENABLE);
  while (key != 'q') {
    if (key == 'c') {
      sprintf(bgcolor, "#%02X%02X%02X", (int) (drand48() * 128),
	      (int) (drand48() * 128), (int) (drand48() * 128));
      gsetbgcolor(win, bgcolor);
      sprintf(message, "bgcolor: %s", bgcolor);
    }
    gclr(win);
    gputimage(win, 75, 10,Xpm_image_tree, XPM_WIDTH_TREE, XPM_HEIGHT_TREE,
	      1);
    b++;
    b %= 300;
    if (b == 0)
      sprintf(message, "hit key 'c'");
    drawstr(win, L - 140, 10, 18, 0, message);
    for (i = 0; i < MAXSNOW; i++) {
      n = (int) (6 * drand48());
      snowx[i] += 3 * (drand48() - 0.5);
      snowy[i] -= (1 + 2 * drand48());
      gputimage(win, snowx[i], snowy[i],(unsigned char *) snowp[n], 3, 3,0);
      if (snowy[i] < 0)
	snowy[i] = L;
    }
    copylayer(win, 1, 0);
    msleep(40);
    key = ggetch();
  }
  gclose(win);

  return 0;
}
