#include "WebHandler.h"
#include <iostream>
#include <future>
#include <functional>
#include <nlohmann/json.hpp>

bool isValidTag(const std::string& str) {
    if (str.length() > 100) {
        return false;
    }
    for (char c : str) {
        if (!isalnum(c) && c != ':') {
            return false;
        }
    }
    return true;
}

bool isValidContentId(const std::string& str) {
    if (str.length() > 100) {
        return false;
    }
    for (char c : str) {
        if (!isalnum(c) && c != '_' && c != '-') {
            return false;
        }
    }
    return true;
}


void WebHandler::startWebinterface() {
    std::cout << "Web Interface started ...." << std::endl;


    CROW_ROUTE(app, "/api/bluetooth/devices").methods("POST"_method)([&](const crow::request& req) {
        std::cout << "get devices" << std::endl;
        devicelist_t objectsInterfacesAndProperties = bluetoothHandler->getDevices();

        std::vector<std::string> device_names;
        for (auto const& [key, val] : objectsInterfacesAndProperties) {
            std::cout << "entry: " << key << std::endl;
            for (auto const& [key2, val2] : val) {            
                std::cout << " -- " << key2 << ":"<< std::endl;
                if(key2 == "org.bluez.Device1") {
                    device_names.push_back(key);
                }
                for (auto const& [key3, val3] : val2) {
                    std::cout << " ---- " << key3 << " : " << val3.peekValueType() << std::endl;
                }
            }
        
        }
        crow::json::wvalue response;
        response = device_names;
        return crow::response(response);
    });

    CROW_ROUTE(app, "/api/content/list").methods("GET"_method)([&](const crow::request& req) {
        std::vector<MusicBox::ContentItem> library = contentHandler->getContentLibrary();
        nlohmann::json j = library;
        auto response = crow::response(j.dump());
        response.add_header("Content-Type", "application/json");
        return response;
    });

    CROW_ROUTE(app, "/api/content/<string>").methods("GET"_method)([&](const crow::request& req, std::string contentId) {
        if(!isValidContentId(contentId)) {
            return crow::response(crow::status::BAD_REQUEST, "Invalid content id format");
        }
        std::optional<MusicBox::ContentItem> item = contentHandler->getContentItem(contentId);
        if(!item.has_value()) {
            return crow::response(crow::status::NOT_FOUND);
        }
        nlohmann::json j = item.value();
        auto response = crow::response(j.dump());
        response.add_header("Content-Type", "application/json");
        return response;
    });

    CROW_ROUTE(app, "/api/content/<string>/cover").methods("GET"_method)([&](const crow::request& req, crow::response& res, std::string contentId) {
        if(!isValidContentId(contentId)) {
            res.code = crow::status::BAD_REQUEST;
            res.end();
            return;
        }
        std::optional<MusicBox::ContentItem> item = contentHandler->getContentItem(contentId);
        if(!item.has_value()) {
            res.code = crow::status::NOT_FOUND;
            res.end();
            return;
        } else {}
        res.set_static_file_info("content/"+contentId+"/"+item->image);
        res.end();
        return;
    });    

    CROW_ROUTE(app, "/api/tags").methods("GET"_method)([&](const crow::request& req) {       
        std::vector<MusicBox::TagData> tagData = tagHandler->GetAllTags();        
        nlohmann::json j = tagData;
        auto response = crow::response(j.dump());
        response.add_header("Content-Type", "application/json");
        return response;
    });

    CROW_ROUTE(app, "/api/tags/<string>").methods("GET"_method)([&](const crow::request& req, std::string tag) {
        if(!isValidTag(tag)) {
            return crow::response(crow::status::BAD_REQUEST, "Invalid tag format");
        }
        std::optional<MusicBox::TagData> tagData = tagHandler->GetTagData(tag);
        if(!tagData.has_value()) {
            return crow::response(crow::status::NOT_FOUND);
        }
        nlohmann::json j = tagData.value();
        auto response = crow::response(j.dump());
        response.add_header("Content-Type", "application/json");
        return response;
    });
    
    CROW_ROUTE(app, "/api/tags/<string>").methods("POST"_method)([&](const crow::request& req, std::string tag) {
        if(!isValidTag(tag)) {
            return crow::response(crow::status::BAD_REQUEST, "Invalid tag format");
        }
        try {
            nlohmann::json jsonObject = nlohmann::json::parse(req.body);
            MusicBox::TagData obj = jsonObject.template get<MusicBox::TagData>();
            if(obj.tag.empty() || obj.name.empty() || obj.action.empty()) {
                return crow::response(crow::status::BAD_REQUEST, "Minimal fields are missing");
            }
            std::cout << obj.name << std::endl;
            tagHandler->SetTagData(obj);

            MusicBox::StateData stateData = applicationState->GetState();
            if(stateData.activeTag.has_value()) {
                if(stateData.activeTag->tag == tag) {
                    applicationState->UpdateTag(obj);
                }
            }
        } catch(const nlohmann::json::parse_error& e) {
            return crow::response(crow::status::BAD_REQUEST, "Invalid JSON format: " + std::string(e.what()));
        }
        
        return crow::response(crow::status::OK);
    });

    CROW_ROUTE(app, "/api/tags/<string>/reset").methods("POST"_method)([&](const crow::request& req, std::string tag) {
        if(!isValidTag(tag)) {
            return crow::response(crow::status::BAD_REQUEST, "Invalid tag format");
        }
        std::optional<MusicBox::TagData> tagData = tagHandler->GetTagData(tag);
        if(!tagData.has_value()) {
            return crow::response(crow::status::NOT_FOUND);
        }
        tagData->playState.index = 0;
        tagData->playState.seed = 0;
        tagData->playState.position = 0;
        
        tagHandler->SetTagData(tagData.value());

        MusicBox::StateData stateData = applicationState->GetState();
        if(stateData.activeTag.has_value()) {
            if(stateData.activeTag->tag == tag) {
                applicationState->UpdateTag(tagData.value());
            }
        }

        nlohmann::json j = tagData.value();
        auto response = crow::response(j.dump());
        response.add_header("Content-Type", "application/json");
        return response;
    });

    CROW_ROUTE(app, "/api/content/upload")
      .methods(crow::HTTPMethod::Post)(std::bind(&WebHandler::upload, this, std::placeholders::_1));

    
    CROW_ROUTE(app, "/api/live").websocket()
      .onopen([&](crow::websocket::connection& conn) {
        std::lock_guard<std::mutex> _(mutexConnectedClients);
        connectedClients.insert(&conn);
      })
      .onclose(
          [&](crow::websocket::connection& conn, const std::string& reason) {
        std::lock_guard<std::mutex> _(mutexConnectedClients);
        connectedClients.erase(&conn);
      })
      .onmessage(std::bind(&WebHandler::handleWebsocketData, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));


    CROW_ROUTE(app, "/")
    ([](const crow::request&, crow::response& res) {
        res.set_static_file_info("webroot/index.html");
        res.end();
    });

    CROW_ROUTE(app, "/<string>/<string>")
    ([&](crow::response& res, std::string part1, std::string part2) {
        std::cout << "request: " << part1 << " - " << part2 << std::endl;
        if (part1.find("..") != std::string::npos && part2.find("..") != std::string::npos) {
            res.code = crow::status::NOT_FOUND;
            res.end();
            return;
        }
        const auto filepath = "webroot/" + part1 + "/" + part2;
        std::cout << "request: " << filepath << std::endl;       
        res.set_static_file_info(filepath);
        res.end();
    });
    
    webserver = app.port(18080).multithreaded().run_async();
    app.signal_clear();
}

void WebHandler::stopWebinterface() {
    app.stop();
}

WebHandler::~WebHandler() {
    stopWebinterface();
}

void WebHandler::setBluetoothHandler(BluetoothHandler* blHander) {
    bluetoothHandler = blHander;
}

void WebHandler::setContentHandler(ContentHandler* cHander) {
    contentHandler = cHander;
}

void WebHandler::setMusicHandler(MusicHandler* mHander) {
    musicHandler = mHander;
}

void WebHandler::setTagHandler(TagHandler* tHandler) {
    tagHandler = tHandler;
}

void WebHandler::setStateInstance(MusicBox::State* state) {
    applicationState = state;
}

void WebHandler::sendMessage(const std::string msg) {
  std::lock_guard<std::mutex> _(mutexConnectedClients);
  for (auto u : connectedClients) {
    u->send_text(msg);
  }
}

crow::response WebHandler::upload(const crow::request &request) {
    crow::multipart::message file_message(request);
    for (const auto& part : file_message.part_map) {
        const auto& part_name = part.first;
        const auto& part_value = part.second;
        std::cout << "Part: " << part_name << std::endl;
        if ("file" == part_name) {
            // Extract the file name
            auto headers_it = part_value.headers.find("Content-Disposition");
            if (headers_it == part_value.headers.end()) {
                CROW_LOG_ERROR << "No Content-Disposition found";
                return crow::response(400);
            }
            auto params_it = headers_it->second.params.find("filename");
            if (params_it == headers_it->second.params.end())
            {
                CROW_LOG_ERROR << "Part with name \"InputFile\" should have a file";
                return crow::response(400);
            }
            const std::string outfile_name = params_it->second;
            std::cout << "Filename: " << outfile_name << std::endl;
            for (const auto& part_header : part_value.headers) {
                const auto& part_header_name = part_header.first;
                const auto& part_header_val = part_header.second;
                CROW_LOG_DEBUG << "Header: " << part_header_name << '=' << part_header_val.value;
                for (const auto& param : part_header_val.params) {
                    const auto& param_key = param.first;
                    const auto& param_val = param.second;
                    CROW_LOG_DEBUG << " Param: " << param_key << ',' << param_val;
                }
            }

            // Create a new file with the extracted file name and write file contents to it
            std::ofstream out_file("content/" + sanitizeFileName(outfile_name));
            if (!out_file) {
                CROW_LOG_ERROR << " Write to file failed\n";
                continue;
            }
            out_file << part_value.body;
            out_file.close();
            CROW_LOG_INFO << " Contents written to " << outfile_name << '\n';
        }
    }
    return crow::response("done");
}

std::string WebHandler::sanitizeFileName(const std::string& filename) const {
    std::string sanitizedFilename;

    for (char c : filename) {
        if (c == '/' || c == '\\' || c == '.' || c == '~' || c == ':') {
            continue;
        }
        else if (c == ' ') {
            sanitizedFilename.push_back('_');
        }
        else {
            sanitizedFilename.push_back(c);
        }
    }

    return sanitizedFilename;
}

void WebHandler::handleWebsocketData (crow::websocket::connection& , const std::string& data, bool is_binary) {
    std::cout << "socker msg: " << data << std::endl;    
    if("startPlay" == data) {
        musicHandler->startPlay();
        sendMessage(nlohmann::json{{"event", "play"}}.dump());
    }
    if("stopPlay" == data) {
        musicHandler->stopPlay();
        sendMessage(nlohmann::json{{"event", "stop"}}.dump());
    }
    if("forward" == data) {
        musicHandler->forward();
    }
    if("backward" == data) {
        musicHandler->backward();
    }
    if("seekForward" == data) {
        musicHandler->seek(15);
    }
    if("seekBackward" == data) {
        musicHandler->seek(-15);
    }
    if("startScanning" == data) {
        bluetoothHandler->startScanning();
    }
    if("stopScanning" == data) {
        bluetoothHandler->stopScanning();
    }
    if("uePowerOn" == data) {
        bluetoothHandler->powerOnUEBoom();
    }
}