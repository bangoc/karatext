#include <blend2d.h>
#include <stdio.h>

int main() {
  BLImage img(1920, 1080, BL_FORMAT_PRGB32);
  BLContext ctx(img);

  ctx.setCompOp(BL_COMP_OP_SRC_COPY);
  ctx.fillAll();

  BLFontFace face;
  BLResult err = face.createFromFile("/usr/share/fonts/truetype/noto/NotoSansMono-Regular.ttf");

  if (err) {
    printf("Failed to load a font-face (err=%u)\n", err);
    return 1;
  }

  BLFont font;
  font.createFromFace(face, 50.0f);

  ctx.setFillStyle(BLRgba32(0xFFFFFFFF));
  ctx.fillUtf8Text(BLPoint(60, 80), font, "Mưa trên cuộc tình");

  ctx.end();
  img.writeToFile("mua-tren-cuoc-tinh.png");
  return 0;
}