#include <blend2d.h>
#include <iostream>

#include "pixels.h"

int main(int argc, char *argv[]) {
  BLImage img(1920, 1080, BL_FORMAT_PRGB32);
  BLContext ctx(img);
  ctx.fillAll();
  BLFontFace face;
  BLResult err = face.createFromFile("/usr/share/fonts/truetype/tlwg/Laksaman-Italic.ttf");
  if (err) {
    std::cout << "Failed to load a font-face\n";
    return 1;
  }
  BLFont font;
  font.createFromFace(face, 60.0f);
  BLFontMetrics fm = font.metrics();
  const char *line = "Mưa trên cuộc tình";
  BLGlyphBuffer gb;
  gb.setUtf8Text(line, SIZE_MAX);
  font.shape(gb);
  BLTextMetrics tm;
  font.getTextMetrics(gb, tm);
  BLPoint p1(100, 100);
  ctx.setFillStyle(BLRgba32(0xFF, 0xFF, 0xFF, 0xFF));
  ctx.fillGlyphRun(p1, font, gb.glyphRun());

  BLFontFace face2;
  err = face2.createFromFile("/usr/share/fonts/truetype/tlwg/Laksaman-BoldItalic.ttf");
  BLFont font2;
  font2.createFromFace(face2, 60.0f);
  BLFontMetrics fm2 = font2.metrics();
  font2.shape(gb);
  BLTextMetrics tm2;
  font2.getTextMetrics(gb, tm2);
  BLPoint p2(100, p1.y + fm.descent + fm2.ascent + fm2.lineGap);
  ctx.setFillStyle(BLRgba32(0x00, 0XFF, 0x00, 0xFF));
  ctx.fillGlyphRun(p2, font2, gb.glyphRun());

  BLPath path;
  path.moveTo(p2.x, p2.y);
  path.lineTo(p2.x + 300, p2.y);
  ctx.setStrokeStyle(BLRgba32(0xFFFF8000));
  ctx.setStrokeWidth(3);
  ctx.setStrokeStartCap(BL_STROKE_CAP_BUTT);
  ctx.strokePath(path);

  std::cout << tm2.boundingBox.x0 << " " << tm2.boundingBox.x1 << std::endl
            << tm2.boundingBox.y0 << " " << tm2.boundingBox.y1 << std::endl;

  std::cout << tm.boundingBox.x0 << " " << tm.boundingBox.x1 << std::endl
            << tm.boundingBox.y0 << " " << tm.boundingBox.y1 << std::endl;

  std::cout << fm2.descent << " " << fm2.ascent << " " << fm2.lineGap << std::endl;

  BLPoint p3(100, p2.y + fm2.descent + fm2.ascent + fm2.lineGap * 2 + fm2.ascent);
  double p = 0.3;
  long i, j;
  for (i = tm2.boundingBox.x0; i <= tm2.boundingBox.x1 * p; ++i) {
    for (j = -(fm2.ascent + fm2.lineGap) + 1; j <= fm2.descent; ++j) {
      SetPixelColor(&img, p3.x + (i - tm2.boundingBox.x0), p3.y + j,
          GetPixelColor(&img, p2.x + i, p2.y + j));
    }
  }

  for (; i <= tm.boundingBox.x1; ++i) {
    for (j = -(fm.ascent + fm.lineGap); j <= fm.descent; ++j) {
      SetPixelColor(&img, p3.x + (i - tm.boundingBox.x0), p3.y + j,
          GetPixelColor(&img, p1.x + i, p1.y + j));
    }
  }

  ctx.end();
  img.writeToFile("twostyle.png");
  return 0;
}