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
  BLTextMetrics tm1, tm2;

  const char* line1 = "Từng hạt mưa rơi tí tách bên hiên nhà nàng";
  const char* line2 = "Lòng thầm mong một ánh mắt thấp thoáng mong chờ";

  BLGlyphBuffer gb1, gb2;
  gb1.setUtf8Text(line1, SIZE_MAX);
  gb2.setUtf8Text(line2, SIZE_MAX);
  font.shape(gb1);
  font.getTextMetrics(gb1, tm1);
  BLPoint p1(100,
            (1080 - 2 * (fm.ascent + fm.descent + fm.lineGap)));
  ctx.fillGlyphRun(p1, font, gb1.glyphRun());

  font.shape(gb2);
  font.getTextMetrics(gb2, tm2);
  BLPoint p2((1920.0 - (tm2.boundingBox.x1 - tm2.boundingBox.x0)) - 100,
            (1080 - (fm.ascent + fm.descent + fm.lineGap)));
  ctx.fillGlyphRun(p2, font, gb2.glyphRun());


  BLRgba32 red(0xFF, 0, 0, 0xFF);
  for (long i = p1.x; i < p1.x + (tm1.boundingBox.x1 - tm1.boundingBox.x0) / 3; ++i) {
    for (long j = p1.y - (fm.ascent + fm.lineGap); j < p1.y + fm.descent; ++j) {
      BLRgba32 color = GetPixelColor(&img, i, j);
      if (color.r() != 0 && color.g() != 0 && color.b() != 0) {
        SetPixelColor(&img, i, j, &red);
      }
    }
  }
  ctx.end();
  img.writeToFile("hai-cau.png");

  return 0;
}