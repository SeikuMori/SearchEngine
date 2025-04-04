#include "ConverterJSON.h"
#include "SearchServer.h"
#include <fstream>
#include <stdexcept>
#include <nlohmann/json.hpp>

std::vector<std::string> ConverterJSON::GetTextDocuments() {
    std::ifstream config_file("config.json");
    if (!config_file.is_open()) {
        throw std::runtime_error("Unable to open config.json");
    }
    
    nlohmann::json config;
    config_file >> config;
    
    std::vector<std::string> documents;
    for (const auto& file_path : config["files"]) {
        std::ifstream file(file_path);
        if (!file.is_open()) {
            throw std::runtime_error("Unable to open file: " + std::string(file_path));
        }
        std::string content((std::istreambuf_iterator<char>(file)),
                            std::istreambuf_iterator<char>());
        documents.push_back(content);
    }
    return documents;
}

std::vector<std::string> ConverterJSON::GetRequests() {
    std::ifstream request_file("requests.json");
    if (!request_file.is_open()) {
        throw std::runtime_error("Unable to open requests.json");
    }
    
    nlohmann::json requests_json;
    request_file >> requests_json;
    return requests_json["requests"].get<std::vector<std::string>>();
}

void ConverterJSON::putAnswers(const std::vector<std::vector<RelativeIndex>>& answers) {
    nlohmann::json answer_json;
    for (size_t i = 0; i < answers.size(); ++i) {
        nlohmann::json result;
        result["request" + std::to_string(i + 1)] = {{"result", answers[i].empty() ? "false" : "true"}};
        nlohmann::json relevance;
        for (const auto& entry : answers[i]) {
            relevance.push_back({{"docid", entry.doc_id}, {"rank", entry.rank}});
        }
        result["relevance"] = relevance;
        answer_json["answers"].push_back(result);
    }

    std::ofstream output("answers.json");
    if (!output.is_open()) {
        throw std::runtime_error("Unable to open answers.json for writing");
    }
    output << answer_json.dump(4); 
}
