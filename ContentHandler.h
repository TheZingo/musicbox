#ifndef INCLUDE_CONTENTHANDLER_H_
#define INCLUDE_CONTENTHANDLER_H_

#include "ContentItem.h"
#include <filesystem>
#include <vector>
#include <optional>

class ContentHandler {

    private:
       std::vector<MusicBox::ContentItem> library;

    public:
       ContentHandler() = default;
       void readContentDirectory(const std::filesystem::path& contentDir);
       std::vector<MusicBox::ContentItem> getContentLibrary();
       std::optional<MusicBox::ContentItem> getContentItem(const std::string contentRef);
};

#endif