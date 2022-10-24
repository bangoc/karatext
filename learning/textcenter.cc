#include <blend2d.h>
#include <stdio.h>
#include <string.h>

#include "pixels.h"

int main(int argc, char* argv[]) {
  BLImage img(1920, 1080, BL_FORMAT_PRGB32);
  BLContext ctx(img);

  ctx.setCompOp(BL_COMP_OP_SRC_COPY);
  ctx.fillAll();
  ctx.setFillStyle(BLRgba32(0xFFFFFFFF));

  BLFontFace face;
  BLResult err = face.createFromFile("/usr/share/fonts/truetype/noto/NotoSansMono-Regular.ttf");
  if (err) {
    printf("Failed to load a font-face (err=%u)\n", err);
    return 1;
  }

  BLFont font;
  font.createFromFace(face, 60.0f);

  BLFontMetrics fm = font.metrics();
  BLTextMetrics tm;
  BLGlyphBuffer gb;

  BLPoint p(0, 0);
  const char* text = "Mưa trên cuộc tình";
  gb.setUtf8Text(text, SIZE_MAX);
  font.shape(gb);
  font.getTextMetrics(gb, tm);

  p.x = (1920.0 - (tm.boundingBox.x1 - tm.boundingBox.x0)) / 2.0;
  p.y = (1080 - (fm.ascent + fm.descent + fm.lineGap));
  ctx.fillGlyphRun(p, font, gb.glyphRun());
  BLRgba32 red(0xFF, 0, 0, 0xFF);
  for (long i = p.x; i < p.x + (tm.boundingBox.x1 - tm.boundingBox.x0) / 3 + 20; ++i) {
    for (long j = p.y - (fm.ascent + fm.descent + fm.lineGap); j < p.y; ++j) {
      BLRgba32 color = GetPixelColor(&img, i, j);
      if (color.r() != 0 && color.g() != 0 && color.b() != 0) {
        SetPixelColor(&img, i, j, &red);
      }
    }
  }
  ctx.end();
  img.writeToFile("mua-tren-cuoc-tinh-center.png");

  return 0;
}