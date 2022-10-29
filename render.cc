#include <iostream>
#include <fstream>
#include <json.hpp>
#include <glog/logging.h>
#include <map>
#include <vector>

#include "textline.h"

double toSecond(std::string time) {
  double h, m, s;
  sscanf(time.c_str(), "%lf:%lf:%lf", &h, &m, &s);
  return h * 3600 + m * 60 + s;
}

int main(int argc, char *argv[]) {
  google::InitGoogleLogging(argv[0]);
  if (argc < 2) {
    std::cout << "Usage: ./render song.t" << std::endl;
    return 1;
  }
  std::ifstream conf(argv[1]);
  nlohmann::json j = nlohmann::json::parse(conf);
  LOG(INFO) << "width: " << j["width"];
  LOG(INFO) << "height: " << j["height"];
  LOG(INFO) << "fps: " << j["fps"];
  long width = j["width"].get<long>(),
       height = j["height"].get<long>();
  long fps = j["fps"].get<long>();
  double duration = toSecond(j["duration"].get<std::string>());
  j["duration"] = duration;
  std::vector<TextLine*> lines;
  for (auto &line: j["lyric"]) {
    std::string b = line["begin"].get<std::string>();
    line["begin"] = toSecond(b);
    std::string e = line["end"].get<std::string>();
    line["end"] = toSecond(e);
    LOG(INFO) << line.dump(2) << std::endl;
    TextLine *tmp = new TextLine(line, 100,
        BLRgba32(0xFF, 0xFF, 0xFF, 0xFF), BLRgba32(0x00, 0xFF, 0x00, 0xFF));
    lines.push_back(tmp);
  }
  BLImage img(width, height, BL_FORMAT_PRGB32);
  img.readFromFile(j["background"].get<std::string>().c_str());

  double step = 1.0 / fps;
  long idx = 0, idx1 = -1, idx2 = -1;
  long sz = j["lyric"].size();
  std::vector<int> positions(sz, 0);
  for (double t = 0; t <= duration; t += step) {
    BLImage frame;
    frame.assignDeep(img);
    if (idx1 < 0 && idx2 < 0) {
      if (idx < sz) {
        if (t < lines[idx]->end() && t + 5 >= lines[idx]->begin()) {
          idx1 = idx;
          if (idx < sz - 1 && lines[idx]->end() + 5 >= lines[idx + 1]->begin()) {
            idx2 = idx + 1;
          }
        }
      }
    } else {
      long h = 0;
      if (idx1 >= 0) {
        h = lines[idx1]->height();
        lines[idx1]->Paint(&frame, t, BLPoint(10, height - 2 * lines[idx1]->height() - 10));
        if (t >= lines[idx1]->begin() && t <= lines[idx1]->end()) {
          if (idx2 >= 0 && idx2 < idx1) {
            if (idx1 < sz - 1 && lines[idx1]->end() + 5 >= lines[idx1 + 1]->begin()) {
              idx2 = idx1 + 1;
            }  else {
              idx2 = -1;
              if (idx1 < sz - 1) {
                idx = idx1 + 1;
              }
            }
          }
        } else if (t > lines[idx1]->end() && idx2 < idx1) {
          idx1 = -1;
          idx2 = -1;
        }
      }
      if (idx2 >= 0) {
        h = lines[idx2]->height();
        lines[idx2]->Paint(&frame, t, BLPoint(width - lines[idx2]->width() - 10, height - 1 * lines[idx2]->height() - 10));
        if (t >= lines[idx2]->begin() && t <= lines[idx2]->end()) {
          if (idx1 >= 0 && idx1 < idx2) {
            if (idx2 < sz - 1 && lines[idx2]->end() + 5 >= lines[idx2 + 1]->begin()) {
              idx1 = idx2 + 1;
            }  else {
              idx1 = -1;
              if (idx2 < sz - 1) {
                idx = idx2 + 1;
              }
            }
          }
        } else if (t > lines[idx2]->end() && idx1 < idx2) {
          idx1 = -1;
          idx2 = -1;
        }
      }
      long min = 0;
      if (idx1 >= 0) {
        min = idx1;
      }
      if (idx2 >= 0 && idx2 < min) {
        min = idx2;
      }
      if (t < lines[min]->begin()) {
        long ws = ceil(lines[min]->begin() - t);
        if (ws > 0) {
          std::string s;
          for (long i = 0; i < ws; ++i) {
            // s += "🎤";
            s += "*";
          }
          std::cout << s << std::endl;
          BLContext tmp(frame);
          tmp.setFillStyle(BLRgba32(0x00, 0xFF, 0x00, 0xFF));
          tmp.fillUtf8Text(BLPoint(10, height - 2 * h - 10), lines[min]->font(), s.c_str());
          tmp.end();
        }
      }
    }
    static int cc = 0;
    char buff[1000];
    sprintf(buff, "frame%08d.png", cc++);
    frame.writeToFile(buff);
  }
  return 0;
}