#ifndef INCLUDE_SETTINGS_H
#define INCLUDE_SETTINGS_H

#include <string>
#include <nlohmann/json.hpp>

namespace MusicBox {

struct Settings {
   std::string speaker;
};


inline void to_json(nlohmann::json& j, const MusicBox::Settings& p) {
  j = nlohmann::json{ {"speaker", p.speaker} };
}

inline void from_json(const nlohmann::json& j, MusicBox::Settings& p) {
  MusicBox::Settings defaultObj;
  p.speaker = j.value("speaker", defaultObj.speaker);
}

inline MusicBox::Settings LoadSettings()
{
    MusicBox::Settings defaultSettings;
    std::ifstream fileStream("settings.json");
    if (!fileStream.is_open()) {
        std::cerr << "Failed to open settings file: settings.json" << std::endl;
        return defaultSettings;
    }
    try {
        nlohmann::json jsonObject = nlohmann::json::parse(fileStream);
        MusicBox::Settings obj = jsonObject.template get<MusicBox::Settings>();
        return obj;
    } catch (const nlohmann::json::parse_error &e) {
        std::cerr << "Error parsing settings file. Error: " << e.what() << std::endl;
    } catch (const nlohmann::json::exception &e) {
        std::cerr << "Unexpected JSON error: " << e.what() << std::endl;
    }
    return defaultSettings;
}

}
#endif  //  INCLUDE_SETTINGS_H