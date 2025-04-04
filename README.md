# Search Engine

A C++ implementation of a search engine that uses an inverted index for efficient text search across multiple documents.

## Features

- Inverted index for fast text search
- Configurable through JSON files
- Document relevance ranking
- Support for multi-word queries
- Unit tests using Google Test framework

## Project Structure
search_engine/ ├── src/ # Source files │ ├── InvertedIndex.cpp # Inverted index implementation │ ├── SearchServer.cpp # Search server implementation │ ├── ConverterJSON.cpp # JSON handling │ └── Main.cpp # Entry point ├── include/ # Header files ├── tests/ # Test files ├── resources/ # Text documents for searching └── config/ # Configuration files ├── config.json # Main configuration └── request.json # Search queries


## Dependencies

- C++17 or higher
- CMake 3.10 or higher
- nlohmann/json library (automatically fetched by CMake)
- Google Test framework (automatically fetched by CMake)

## Building the Project

```bash
# Create build directory
mkdir build
cd build

# Configure CMake
cmake ..

# Build the project
cmake --build .
