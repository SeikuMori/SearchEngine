#include "SearchServer.h"
#include <map>
#include <algorithm>
#include <sstream>
#include <cmath>

SearchServer::SearchServer(InvertedIndex& idx) : _index(idx) {}

std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string>& queries_input) {
    const size_t MAX_RESULTS = 5; // Максимальное количество результатов для каждого запроса
    
    std::vector<std::vector<RelativeIndex>> result;
    result.reserve(queries_input.size());

    for (const auto& query : queries_input) {
        std::map<size_t, float> doc_relevance;
        std::istringstream stream(query);
        std::string word;
        
        // Подсчитываем релевантность для каждого документа
        while (stream >> word) {
            auto word_count = _index.GetWordCount(word);
            for (const auto& entry : word_count) {
                doc_relevance[entry.doc_id] += entry.count;
            }
        }

        // Если нет результатов, добавляем пустой вектор
        if (doc_relevance.empty()) {
            result.push_back({});
            continue;
        }

        // Находим максимальную релевантность
        float max_relevance = 0.0f;
        for (const auto& [doc_id, relevance] : doc_relevance) {
            max_relevance = std::max(max_relevance, relevance);
        }

        // Создаем вектор результатов
        std::vector<RelativeIndex> ranked_docs;
        ranked_docs.reserve(doc_relevance.size());
        
        // Преобразуем map в вектор RelativeIndex
        for (const auto& [doc_id, relevance] : doc_relevance) {
            // Защита от деления на ноль
            float normalized_rank = (std::abs(max_relevance) < 1e-6) ? 0.0f : (relevance / max_relevance);
            ranked_docs.push_back({doc_id, normalized_rank});
        }

        // Частичная сортировка - только для TOP-K результатов
        if (ranked_docs.size() > MAX_RESULTS) {
            std::partial_sort(ranked_docs.begin(), 
                            ranked_docs.begin() + MAX_RESULTS,
                            ranked_docs.end(),
                            [](const RelativeIndex& a, const RelativeIndex& b) {
                                return (a.rank > b.rank) || 
                                       (a.rank == b.rank && a.doc_id < b.doc_id);
                            });
            ranked_docs.resize(MAX_RESULTS);
        } else {
            std::sort(ranked_docs.begin(), ranked_docs.end(),
                     [](const RelativeIndex& a, const RelativeIndex& b) {
                         return (a.rank > b.rank) || 
                                (a.rank == b.rank && a.doc_id < b.doc_id);
                     });
        }

        result.push_back(std::move(ranked_docs));
    }
    return result;
}

bool RelativeIndex::operator==(const RelativeIndex& other) const {
    return doc_id == other.doc_id && std::abs(rank - other.rank) < 1e-6;
}
