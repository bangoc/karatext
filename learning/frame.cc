#include "textline.h"

int main() {
  nlohmann::json j = nlohmann::json::parse(R"(
    {
      "text": "Từng hạt mưa rơi tí tách bên hiên nhà nàng",
      "begin": 1.2,
      "end": 6,
      "notes": [1, 1, 3, 1, 1, 1, 1, 1, 1, 2]
    }
  )");
  TextLine *line = new TextLine(j, 60, BLRgba32(0xFF, 0xFF, 0xFF, 0xFF),
                       BLRgba32(0x00, 0xFF, 0x00, 0xFF));
  double t = j["begin"].get<double>();
  double end = j["end"].get<double>();
  double step = 1.0/27;
  for (; t <= end; t += step) {
    line->Render(t);
  }
  if (t > end) {
    line->Render(t);
  }
  delete line;
  return 0;
}