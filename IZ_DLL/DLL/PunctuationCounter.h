#ifndef PUNCTUATIONCOUNTER_H
#define PUNCTUATIONCOUNTER_H

#ifdef _WIN32
#ifdef PUNCTUATIONCOUNTER_EXPORTS
#define PUNCTUATIONCOUNTER_API __declspec(dllexport)
#else
#define PUNCTUATIONCOUNTER_API __declspec(dllimport)
#endif
#else
#define PUNCTUATIONCOUNTER_API
#endif

#include <string>
#include <vector>

// Объявление функций с C-линковкой
extern "C" {
    // Функция подсчета знаков препинания
    PUNCTUATIONCOUNTER_API bool countPunctuationInFile(const char* filename);

    // Функция получения результатов
    PUNCTUATIONCOUNTER_API const char* getResultsString();

    // Функция освобождения ресурсов
    PUNCTUATIONCOUNTER_API void cleanup();

    // Функция чтения файла в буфер
    PUNCTUATIONCOUNTER_API char* readFileToBuffer(const char* filename, size_t* fileSize);

    // Функция нахождения позиции первого символа слова
    PUNCTUATIONCOUNTER_API int findFirstWordChar(const char* buffer, int startPos, int bufferSize);
}

#endif