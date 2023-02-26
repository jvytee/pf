#include <png.h>
#include <stdio.h>

int main(void) {
  FILE *fp = fopen("sea-watch_logo_140.png", "rb");
  if (!fp) {
    return 1;
  }

  unsigned char sig[8];
  const size_t items_read = fread(sig, 1, 8, fp);
  if (items_read < 8 || !png_check_sig(sig, 8)) {
    return 1;
  }

  if (fclose(fp) != 0) {
    return 1;
  }

  printf("Hello, world!\n");
  return 0;
}
