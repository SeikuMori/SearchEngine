#ifndef CONVERTER_JSON_H
#define CONVERTER_JSON_H

#include <vector>
#include <string>
#include "SearchServer.h"

class ConverterJSON {
public:
    std::vector<std::string> GetTextDocuments();
    std::vector<std::string> GetRequests();
    void putAnswers(const std::vector<std::vector<RelativeIndex>>& answers);
};

#endif
