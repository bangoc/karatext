#ifndef TOKEN_H_
#define TOKEN_H_

#include <blend2d.h>
#include <string>

class Token {
private:
  std::string _text;
  double _width;
public:
  Token(const char *text, BLFont *font): _text(text) {
    BLGlyphBuffer gb;
    gb.setUtf8Text(text, SIZE_MAX);
    font->shape(gb);
    BLTextMetrics tm;
    font->getTextMetrics(gb, tm);
    _width = tm.boundingBox.x1 - tm.boundingBox.x0;
  }
  double width() {
    return _width;
  }
};

#endif  // TOKEN_H_