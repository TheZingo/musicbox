#ifndef INCLUDE_CONTENTITEM_H
#define INCLUDE_CONTENTITEM_H

#include <string>
#include <vector>
#include <nlohmann/json.hpp>

namespace MusicBox {

struct ContentItem {
   std::string id;
   std::string title;   
   std::string type;
   std::string by;
   long length = 0;
   std::string image;
   std::string description;
   std::vector<std::string> files;
};


inline void to_json(nlohmann::json& j, const MusicBox::ContentItem& p) {
  j = nlohmann::json { 
      {"id", p.id}, 
      {"title", p.title}, 
      {"type", p.type}, 
      {"by", p.by}, 
      {"length", p.length}, 
      {"image", p.image}, 
      {"description", p.description},
      {"files", p.files} 
   };
}

inline void from_json(const nlohmann::json& j, MusicBox::ContentItem& p) {
   ContentItem defaultObj;
   p.id = j.value("id", defaultObj.id);
   p.title = j.value("title", defaultObj.title);
   p.type = j.value("type", defaultObj.type);
   p.by = j.value("by", defaultObj.by);
   p.length = j.value("length", defaultObj.length);
   p.image = j.value("image", defaultObj.image);   
   p.description = j.value("description", defaultObj.description);
   p.files = j.value("files", defaultObj.files);
}

}
#endif  //  IINCLUDE_CONTENTITEM_H