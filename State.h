#ifndef INCLUDE_STATE_H
#define INCLUDE_STATE_H

#include <string>
#include <optional>
#include <mutex>
#include "TagData.h"
#include <nlohmann/json.hpp>

namespace MusicBox {

struct StateData {
    std::optional<TagData> activeTag;
    bool playing;
    std::string activeTrack;
    std::string contentRef;
};

class State {
    private:
        StateData state;
        std::mutex mutex;

    public:
        void UpdateTag(TagData tagdata);
        void UpdateContentRef(std::string contentId);
        void UpdateActiveTrack(std::string track);
        void UpdatePlayState(bool playState);
        StateData GetState();
};

inline void to_json(nlohmann::json& j, const MusicBox::StateData& p) {
    TagData defaultTag;
   j = nlohmann::json{ {"activeTag", p.activeTag.value_or(defaultTag)}, {"playing", p.playing}, {"activeTrack", p.activeTrack}, {"contentRef", p.contentRef} };
}

}
#endif  //  INCLUDE_STATE_H