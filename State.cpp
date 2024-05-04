#include "State.h"

MusicBox::StateData MusicBox::State::GetState() {
    std::lock_guard<std::mutex> lock(mutex);
    return state;
}

void MusicBox::State::UpdateTag(TagData tagdata) {
    std::lock_guard<std::mutex> lock(mutex);
    state.activeTag = tagdata;
}

void MusicBox::State::UpdateContentRef(std::string contentId) {
    std::lock_guard<std::mutex> lock(mutex);
    state.contentRef = contentId;
}

void MusicBox::State::UpdateActiveTrack(std::string track) {
    std::lock_guard<std::mutex> lock(mutex);
    state.activeTrack = track;
}

void MusicBox::State::UpdatePlayState(bool playState) {
    std::lock_guard<std::mutex> lock(mutex);
    state.playing = playState;
}