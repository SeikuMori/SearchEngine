#ifndef INVERTED_INDEX_H
#define INVERTED_INDEX_H

#include <vector>
#include <string>
#include <map>

struct Entry {
    size_t doc_id, count;
    bool operator==(const Entry& other) const;
};

class InvertedIndex {
public:
    void UpdateDocumentBase(const std::vector<std::string>& docs);
    std::vector<Entry> GetWordCount(const std::string& word) const;

private:
    std::vector<std::string> docs;
    std::map<std::string, std::vector<Entry>> freq_dictionary;
};

#endif
