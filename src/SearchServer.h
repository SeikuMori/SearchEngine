#ifndef SEARCH_SERVER_H
#define SEARCH_SERVER_H

#include <vector>
#include <string>
#include "InvertedIndex.h"

struct RelativeIndex {
    size_t doc_id;
    float rank;
    bool operator==(const RelativeIndex& other) const;
};

class SearchServer {
public:
    SearchServer(InvertedIndex& idx);
    std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string>& queries_input);

private:
    InvertedIndex& _index;
};

#endif
