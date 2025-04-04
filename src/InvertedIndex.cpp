#include "InvertedIndex.h"
#include <sstream>
#include <thread>
#include <mutex>
#include <queue>

void InvertedIndex::UpdateDocumentBase(const std::vector<std::string>& input_docs) {
    docs = input_docs;
    freq_dictionary.clear();

    std::mutex mtx;
    std::mutex queue_mtx;
    std::queue<size_t> work_queue;
    
    // Заполняем очередь работы
    for (size_t i = 0; i < docs.size(); ++i) {
        work_queue.push(i);
    }

    // Определяем оптимальное количество потоков
    unsigned int num_threads = std::thread::hardware_concurrency();
    if (num_threads == 0) num_threads = 2; // Если не удалось определить, используем 2 потока
    num_threads = std::min(num_threads, static_cast<unsigned int>(docs.size()));

    std::vector<std::thread> threads;
    for (unsigned int i = 0; i < num_threads; ++i) {
        threads.emplace_back([this, &mtx, &queue_mtx, &work_queue]() {
            while (true) {
                // Получаем следующий документ для обработки
                size_t doc_id;
                {
                    std::lock_guard<std::mutex> lock(queue_mtx);
                    if (work_queue.empty()) {
                        break;
                    }
                    doc_id = work_queue.front();
                    work_queue.pop();
                }

                // Обрабатываем документ
                std::map<std::string, size_t> word_count;
                std::istringstream stream(docs[doc_id]);
                std::string word;
                while (stream >> word) {
                    ++word_count[word];
                }

                // Обновляем общий словарь с синхронизацией
                std::lock_guard<std::mutex> lock(mtx);
                for (const auto& [word, count] : word_count) {
                    freq_dictionary[word].push_back({doc_id, count});
                }
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }
}

std::vector<Entry> InvertedIndex::GetWordCount(const std::string& word) const {
    auto it = freq_dictionary.find(word);
    if (it != freq_dictionary.end()) {
        return it->second;
    }
    return std::vector<Entry>();
}

bool Entry::operator==(const Entry& other) const {
    return doc_id == other.doc_id && count == other.count;
}
