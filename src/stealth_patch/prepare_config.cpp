#include <memory>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <cstring>

char** split_string(const char* input, int* count) {
    std::vector<std::unique_ptr<char[]>> temp_result;
    std::istringstream iss(input);
    std::string word;

    while (iss >> word) {
        auto word_ptr = std::make_unique<char[]>(word.length() + 1);
        strcpy(word_ptr.get(), word.c_str());
        temp_result.push_back(std::move(word_ptr));
    }

    *count = temp_result.size();

    char** result = new char*[*count];
    for (int i = 0; i < *count; ++i) {
        result[i] = temp_result[i].release();
    }

    return result;
}

void free_split_string(char** arr, int count) {
    for (int i = 0; i < count; ++i) {
        delete[] arr[i];
    }
    delete[] arr;
}
