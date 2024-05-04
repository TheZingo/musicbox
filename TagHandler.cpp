#include "TagHandler.h"
#include <fstream>
#include <filesystem>
#include <iostream>
#include <nlohmann/json.hpp>




std::optional<MusicBox::TagData> ReadFromFile(std::filesystem::path fileName) {
  std::ifstream fileStream(fileName);
  if (!fileStream.is_open()) {
      std::cerr << "Failed to open file: " << fileName << std::endl;
      return std::nullopt;
  }
  try {
      nlohmann::json jsonObject = nlohmann::json::parse(fileStream);
      std::cout << jsonObject << std::endl;
      MusicBox::TagData obj = jsonObject.template get<MusicBox::TagData>();
      return std::optional<MusicBox::TagData> { obj };
  } catch (const nlohmann::json::parse_error& e) {
      std::cerr << "Error parsing JSON file: " << fileName << ". Error: " << e.what() << std::endl;
  } catch (const nlohmann::json::exception& e) {
      std::cerr << "Unexpected JSON error: " << e.what() << std::endl;
  }
  return std::nullopt;
}

std::optional<MusicBox::TagData> TagHandler::GetTagData(std::string tag) {

  const std::filesystem::path baseDir ("tags");
  const std::filesystem::path fileName (tag + ".json");
  std::filesystem::path fullPath = baseDir / fileName;

  std::cout << fullPath << std::endl;
  if(std::filesystem::exists(fullPath)) {
    return ReadFromFile(fullPath);
  }
  return std::nullopt;
}



void TagHandler::SetTagData(MusicBox::TagData tagData) {
  const std::filesystem::path baseDir ("tags");
  const std::filesystem::path fileName (tagData.tag + ".json");
  std::filesystem::path fullPath = baseDir / fileName;

  nlohmann::json j = tagData;

  std::ofstream file(fullPath);
  std::cout << j.dump(4) << std::endl;
  file << j.dump(4);
  file.close();
}



std::vector<MusicBox::TagData> TagHandler::GetAllTags() {
  std::vector<MusicBox::TagData> tags;
  const std::filesystem::path baseDir ("tags");
  for (const auto& entry : std::filesystem::directory_iterator(baseDir)) {
        if (entry.is_regular_file() && entry.path().extension() == ".json") {

            const std::filesystem::path path = entry.path();
            std::optional<MusicBox::TagData> tagData = ReadFromFile(path);
            if(tagData.has_value()) {
              tags.push_back(tagData.value());
            }
        }
  }
  return tags;
}
