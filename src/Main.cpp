// main.cpp
#include <iostream>
#include "ConverterJSON.h"
#include "InvertedIndex.h"
#include "SearchServer.h"

int main() {
    try {
        ConverterJSON converter;
        InvertedIndex index;

        // Получаем список документов из config.json
        auto docs = converter.GetTextDocuments();
        index.UpdateDocumentBase(docs);

        // Получаем поисковые запросы из requests.json
        auto requests = converter.GetRequests();
        SearchServer searchServer(index);
        auto results = searchServer.search(requests);

        // Записываем результаты поиска в answers.json
        converter.putAnswers(results);
        std::cout << "Search results saved to answers.json" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
