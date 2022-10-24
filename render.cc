#include <iostream>
#include <fstream>
#include <json.hpp>
#include <glog/logging.h>

int main(int argc, char *argv[]) {
  google::InitGoogleLogging(argv[0]);
  if (argc < 3) {
    std::cout << "Usage: ./render config.json song.t" << std::endl;
    return 1;
  }
  std::ifstream conf(argv[1]);
  nlohmann::json j = nlohmann::json::parse(conf);
  LOG(INFO) << "width: " << j["width"];
  LOG(INFO) << "height: " << j["height"];
  LOG(INFO) << "fps: " << j["fps"];
  return 0;
}