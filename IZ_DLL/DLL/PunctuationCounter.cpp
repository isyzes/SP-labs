#define PUNCTUATIONCOUNTER_EXPORTS

#include "PunctuationCounter.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <cctype>
#include <windows.h>
#include <cstring> 

static std::map<char, int> punctuationCount;
static std::string resultsString;
static char* fileBuffer = nullptr;
static size_t bufferSize = 0;

static const std::string PUNCTUATION_CHARS = ".,;:!?-\"'()[]{}";

extern "C" {

    PUNCTUATIONCOUNTER_API char* readFileToBuffer(const char* filename, size_t* fileSize) {
        std::ifstream file(filename, std::ios::binary | std::ios::ate);
        if (!file.is_open()) {
            *fileSize = 0;
            return nullptr;
        }

        *fileSize = static_cast<size_t>(file.tellg());
        file.seekg(0, std::ios::beg);

        bufferSize = *fileSize + 1024; // Запас 1KB для результатов
        fileBuffer = new char[bufferSize];

        memset(fileBuffer, 0, bufferSize);

        file.read(fileBuffer, *fileSize);

        file.close();
        return fileBuffer;
    }

    PUNCTUATIONCOUNTER_API int findFirstWordChar(const char* buffer, int startPos, int bufferSize) {
        if (!buffer || startPos < 0 || startPos >= bufferSize) {
            return -1;
        }

        for (int i = startPos; i < bufferSize && buffer[i] != '\0'; i++) {
            if (std::isalpha(static_cast<unsigned char>(buffer[i])) ||
                std::isdigit(static_cast<unsigned char>(buffer[i]))) {
                return i;
            }
        }

        return -1;
    }

    PUNCTUATIONCOUNTER_API bool countPunctuationInFile(const char* filename) {
        punctuationCount.clear();
        resultsString.clear();

        for (char c : PUNCTUATION_CHARS) {
            punctuationCount[c] = 0;
        }

        size_t originalFileSize;
        char* buffer = readFileToBuffer(filename, &originalFileSize);

        if (!buffer) {
            std::cerr << "Error: Failed to open file " << filename << std::endl;
            return false;
        }

        for (size_t i = 0; i < originalFileSize && buffer[i] != '\0'; i++) {
            char currentChar = buffer[i];
            if (PUNCTUATION_CHARS.find(currentChar) != std::string::npos) {
                punctuationCount[currentChar]++;
            }
        }

        std::stringstream results;
        results << "\n\n=== PUNCTUATION COUNT RESULTS ===\n";

        int totalPunctuation = 0;
        for (const auto& pair : punctuationCount) {
            if (pair.second > 0) {
                std::string charName;
                switch (pair.first) {
                case '.': charName = "Period"; break;
                case ',': charName = "Comma"; break;
                case ';': charName = "Semicolon"; break;
                case ':': charName = "Colon"; break;
                case '!': charName = "Exclamation mark"; break;
                case '?': charName = "Question mark"; break;
                case '-': charName = "Hyphen/dash"; break;
                case '\"': charName = "Quotation marks"; break;
                case '\'': charName = "Apostrophe"; break;
                case '(': charName = "Opening parenthesis"; break;
                case ')': charName = "Closing parenthesis"; break;
                case '[': charName = "Opening square bracket"; break;
                case ']': charName = "Closing square bracket"; break;
                case '{': charName = "Opening curly brace"; break;
                case '}': charName = "Closing curly brace"; break;
                default: charName = "Unknown character"; break;
                }

                results << charName << " ('" << pair.first << "'): " << pair.second << " time(s)\n";
                totalPunctuation += pair.second;
            }
        }

        results << "===========================================\n";
        results << "Total punctuation marks: " << totalPunctuation << "\n";
        results << "===========================================\n";

        resultsString = results.str();

        size_t resultsLength = resultsString.length();
        if (originalFileSize + resultsLength < bufferSize) {
            strcpy_s(buffer + originalFileSize, bufferSize - originalFileSize, resultsString.c_str());

            std::ofstream outFile(filename, std::ios::binary | std::ios::trunc);
            if (outFile.is_open()) {
                outFile.write(buffer, originalFileSize + resultsLength);
                outFile.close();
                return true;
            }
        }

        return false;
    }

    PUNCTUATIONCOUNTER_API const char* getResultsString() {
        return resultsString.c_str();
    }

    PUNCTUATIONCOUNTER_API void cleanup() {
        if (fileBuffer) {
            delete[] fileBuffer;
            fileBuffer = nullptr;
        }
        punctuationCount.clear();
        resultsString.clear();
        bufferSize = 0;
    }

}