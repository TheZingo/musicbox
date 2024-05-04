#ifndef INCLUDE_TAGHANDLER_H_
#define INCLUDE_TAGHANDLER_H_

#include "TagData.h"
#include <optional>

class TagHandler {
    public:
        TagHandler() = default;
        std::optional<MusicBox::TagData> GetTagData(std::string tag);
        void SetTagData(MusicBox::TagData tagData);
        std::vector<MusicBox::TagData> GetAllTags();
};

#endif