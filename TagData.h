#ifndef INCLUDE_TAGDATA_H
#define INCLUDE_TAGDATA_H

#include <string>
#include <nlohmann/json.hpp>

namespace MusicBox {

struct PlayState {
    int index = 0;
    long position = 0;
    unsigned seed = 0;
};

struct TagData {
   std::string tag;
   std::string name;
   std::string action;
   std::string contentRef;
   bool savePlayState = false;
   PlayState playState;
   bool randomize = false;
   int playCount = 0;
   time_t lastSeen = 0; 
};


inline void to_json(nlohmann::json& j, const MusicBox::PlayState& p) {
  j = nlohmann::json{ {"index", p.index}, {"position", p.position}, {"seed", p.seed} };
}

inline void from_json(const nlohmann::json& j, MusicBox::PlayState& p) {
  MusicBox::PlayState defaultObj;
  p.index = j.value("index", defaultObj.index);
  p.position = j.value("position", defaultObj.position);
  p.seed = j.value("seed", defaultObj.seed);
}

inline void to_json(nlohmann::json& j, const MusicBox::TagData& p) {
  j = nlohmann::json{ 
    {"tag", p.tag}, 
    {"name", p.name}, 
    {"action", p.action} ,
    {"contentRef", p.contentRef},
    {"savePlayState", p.savePlayState},
    {"playState", p.playState},
    {"randomize", p.randomize},
    {"playCount", p.playCount},
    {"lastSeen", p.lastSeen},
    };
}

inline void from_json(const nlohmann::json& j, MusicBox::TagData& p) {
  MusicBox::TagData defaultObj;
  p.tag = j.value("tag", defaultObj.tag);
  p.name = j.value("name", defaultObj.name);
  p.action = j.value("action", defaultObj.action);
  p.contentRef = j.value("contentRef", defaultObj.contentRef);
  p.savePlayState = j.value("savePlayState", defaultObj.savePlayState);
  p.playState = j.value("playState", defaultObj.playState);
  p.randomize = j.value("randomize", defaultObj.randomize);
  p.playCount = j.value("playCount", defaultObj.playCount);
  p.lastSeen = j.value("lastSeen", defaultObj.lastSeen);
}

}
#endif  //  INCLUDE_TAGDATA_H