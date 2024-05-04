#include "ContentHandler.h"

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

void ContentHandler::readContentDirectory(const std::filesystem::path& contentDir) {
    for (const auto& entry : std::filesystem::directory_iterator(contentDir)) {
        if (entry.is_regular_file() && entry.path().filename() == "content.json") {

            const std::filesystem::path path = entry.path();

            std::ifstream fileStream(path);
            if (!fileStream.is_open()) {
                std::cerr << "Failed to open file: " << path << std::endl;
                continue;
            }

            try {
                nlohmann::json jsonObject = nlohmann::json::parse(fileStream);
                MusicBox::ContentItem obj = jsonObject.template get<MusicBox::ContentItem>();
                obj.id = path.parent_path().filename();
                library.push_back(obj);
            } catch (const nlohmann::json::parse_error& e) {
                std::cerr << "Error parsing JSON file: " << path << ". Error: " << e.what() << std::endl;
            } catch (const nlohmann::json::exception& e) {
                std::cerr << "Unexpected JSON error: " << e.what() << std::endl;
            }

        } else if (entry.is_directory()) {
            readContentDirectory(entry.path());
        }
    }
}

std::vector<MusicBox::ContentItem> ContentHandler::getContentLibrary() {
    return library;
}

std::optional<MusicBox::ContentItem> ContentHandler::getContentItem(const std::string contentId) {
    for(const MusicBox::ContentItem& item : library) {
        if(item.id == contentId) {
            return item;
        }
    }
    return std::nullopt;
}