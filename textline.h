#ifndef TEXT_LINE_H_
#define TEXT_LINE_H_

#include <iostream>
#include <json.hpp>
#include <vector>

#include "token.h"
#include "pixels.h"

class TextLine {
private:
  std::string _text;
  double _begin, _end;
  std::vector<double> _notes;
  double _sum_notes;
  std::vector<double> _widths;
  double _sum_widths;
  BLFont _font;
  double _size;
  BLImage _regular_view, _high_view;
  long _width, _height;
  BLPoint _pos;
public:
  TextLine(nlohmann::json j, double size,
        BLRgba32 regular_color, BLRgba32 high_color): _size(size) {
    _text = j["text"];
    _begin = j["begin"];
    _end = j["end"];
    _sum_notes = 0;

    std::cout << "begin: " << _begin << "end: " << _end << std::endl;

    for (auto n: j["notes"]) {
      _notes.push_back(n.get<double>());
      _sum_notes += n.get<double>();
    }

    std::cout << "notes.size() " << _notes.size() << " sum " << _sum_notes << std::endl;

    BLFontFace face;
    face.createFromFile("/usr/share/fonts/truetype/tlwg/Laksaman-Italic.ttf");
    _font.createFromFace(face, _size);
    BLFontMetrics fm = _font.metrics();
    std::string s = "";
    _sum_widths = 0;
    for (int i = 0; i < _text.length(); ++i) {
      if (_text[i] == ' ') {
        Token tk(s.c_str(), &_font);
        _widths.push_back(tk.width());
        _sum_widths += _widths[_widths.size() - 1];
        std::cout << s << " width: " << tk.width() << std::endl;
        s = "";
      } else {
        s += _text[i];
      }
    }
    if (s.length() > 0) {
      Token tk(s.c_str(), &_font);
      _widths.push_back(tk.width());
      _sum_widths += _widths[_widths.size() - 1];
      std::cout << s << " width: " << tk.width() << std::endl;
    }
    std::cout << "_widths.size() " << _widths.size() << " sum: " << _sum_widths << std::endl;
    BLGlyphBuffer gb;
    gb.setUtf8Text(_text.c_str(), SIZE_MAX);
    _font.shape(gb);
    BLTextMetrics tm;
    _font.getTextMetrics(gb, tm);
    _width = tm.boundingBox.x1 - tm.boundingBox.x0 + 10;
    _height = fm.ascent + fm.descent + fm.lineGap + 10;

    double bias = _width - _sum_widths;
    if (_widths.size() > 1) {
      for (long i = 0; i < _widths.size() - 1; ++i) {
        _widths[i] += (bias / (_widths.size() - 1));
      }
    }
    _sum_widths += bias;

    std::cout << "_width: " << _width << " _height " << _height << std::endl;
    if (_width % 2) {
      ++_width;
    }
    _pos.x = 5;
    _pos.y = _height - fm.descent - 5;
    blImageInitAs(&_regular_view, _width, _height, BL_FORMAT_PRGB32);
    blImageInitAs(&_high_view, _width, _height, BL_FORMAT_PRGB32);
    BLContext regular_context(_regular_view);
    regular_context.fillAll();
    regular_context.setFillStyle(regular_color);
    regular_context.fillUtf8Text(_pos, _font, _text.c_str());
    regular_context.end();

    BLContext high_context(_high_view);
    high_context.fillAll();
    high_context.setFillStyle(high_color);
    high_context.fillUtf8Text(_pos, _font, _text.c_str());
    high_context.end();
  }
  void Paint(BLContext *ctx) {

  }

  BLImage Render(double t) {
    static int cc = 1;
    BLImage img(_width, _height, BL_FORMAT_PRGB32);
    double light = 0;
    if (t > _begin && t <= _end) {
      double frag = (t - _begin) / (_end - _begin);
      for (long i = 0; i < _notes.size(); ++i) {
        if (frag >= _notes[i] / _sum_notes) {
          frag -= (_notes[i] / _sum_notes);
          light += _widths[i];
        } else {
          light += (_widths[i] * frag / (_notes[i] / _sum_notes));
          frag = 0;
          break;
        }
      }
    } else if (t > _end) {
      light = _sum_widths;
    }
    std::cout << "t: " << t << " light points: " << light << std::endl;
    for (long i = 0; i < _pos.x + light; ++i) {
      for (long j = 0; j < _height; ++j) {
        SetPixelColor(&img, i, j,
          GetPixelColor(&_high_view, i, j));
      }
    }
    for (long i = _pos.x + light; i < _width; ++i) {
      for (long j = 0; j < _height; ++j) {
        SetPixelColor(&img, i, j,
          GetPixelColor(&_regular_view, i, j));
      }
    }
    char buff[1000];
    sprintf(buff, "frame%08d.png", cc++);
    // ctx.end();
    img.writeToFile(buff);
    return img;
  }
};

#endif  // TEXT_LINE_H_