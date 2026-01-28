#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>

using namespace std;

// Function declarations from DLL
typedef bool (*CountPunctuationFunc)(const char*);
typedef const char* (*GetResultsFunc)();
typedef void (*CleanupFunc)();
typedef char* (*ReadFileFunc)(const char*, size_t*);
typedef int (*FindWordCharFunc)(const char*, int, int);
typedef double (*AverageWordLengthFunc)(const char*);

// Function to create a test file
void createTestFile(const string& filename) {
    ofstream testFile(filename.c_str());
    if (testFile.is_open()) {
        testFile << "This is a test file for the punctuation counter program!\n";
        testFile << "It contains various punctuation marks: periods, commas, question and exclamation marks!\n";
        testFile << "Also quotes: \"Hello world!\", parentheses (round), [square brackets] and {curly braces}.\n";
        testFile << "Don't forget: semicolons; dashes - colons: and apostrophe's.\n";
        testFile << "How many punctuation marks are here? Let's count them all!\n";
        testFile << "Multiple sentences. Each with punctuation! Right?\n";
        testFile.close();
        cout << "Created test file: " << filename << endl;
    }
    else {
        cerr << "Error creating test file!" << endl;
    }
}

int main() {
    cout << "=== Punctuation Counter Client Application ===\n";
    cout << "============================================\n\n";

    // Load DLL
    HMODULE hDLL = LoadLibrary(L"D:\\project\\cpp\\SP\\IZ_DLL\\x64\\Debug\\DLL.dll");

    if (!hDLL) {
        cerr << "ERROR: Cannot load PunctuationCounter.dll\n";
        cerr << "Error code: " << GetLastError() << endl;
        cout << "Make sure PunctuationCounter.dll is in the same directory.\n";
        cout << "Press Enter to exit...";
        cin.get();
        return 1;
    }

    cout << "Library loaded successfully!\n";

    // Get function addresses
    CountPunctuationFunc countPunctuation =
        (CountPunctuationFunc)GetProcAddress(hDLL, "countPunctuationInFile");

    GetResultsFunc getResults =
        (GetResultsFunc)GetProcAddress(hDLL, "getResultsString");

    CleanupFunc cleanupFunc =
        (CleanupFunc)GetProcAddress(hDLL, "cleanup");

    ReadFileFunc readFileFunc =
        (ReadFileFunc)GetProcAddress(hDLL, "readFileToBuffer");

    FindWordCharFunc findWordCharFunc =
        (FindWordCharFunc)GetProcAddress(hDLL, "findFirstWordChar");



    // Check if all functions were found
    if (!countPunctuation || !getResults || !cleanupFunc ||
        !readFileFunc || !findWordCharFunc) {
        cerr << "ERROR: Cannot find all required functions in DLL\n";

        if (!countPunctuation) cerr << "  - countPunctuationInFile not found\n";
        if (!getResults) cerr << "  - getResultsString not found\n";
        if (!cleanupFunc) cerr << "  - cleanup not found\n";
        if (!readFileFunc) cerr << "  - readFileToBuffer not found\n";
        if (!findWordCharFunc) cerr << "  - findFirstWordChar not found\n";

        FreeLibrary(hDLL);
        cout << "\nPress Enter to exit...";
        cin.get();
        return 1;
    }

    cout << "All library functions found!\n\n";

    // Get filename from user
    string filename;
    cout << "Enter text file name (or press Enter for default 'sample.txt'): ";
    getline(cin, filename);

    if (filename.empty()) {
        filename = "sample.txt";
        createTestFile(filename);
    }

    cout << "\n=== Testing Library Functions ===\n\n";

    // Test 1: readFileToBuffer
    cout << "1. Testing readFileToBuffer() function...\n";
    size_t fileSize;
    char* buffer = readFileFunc(filename.c_str(), &fileSize);

    if (!buffer) {
        cerr << "ERROR: Cannot read file '" << filename << "'\n";
        cout << "Check if the file exists and is accessible.\n";
        FreeLibrary(hDLL);
        cout << "Press Enter to exit...";
        cin.get();
        return 1;
    }

    cout << "   Success! File size: " << fileSize << " bytes\n";

    // Test 2: findFirstWordChar
    cout << "\n2. Testing findFirstWordChar() function...\n";
    int wordStart = findWordCharFunc(buffer, 0, (int)fileSize);
    if (wordStart != -1) {
        cout << "   First word character found at position: " << wordStart << "\n";

        // Extract first word
        string firstWord;
        int i = wordStart;
        while (i < fileSize && buffer[i] != '\0' &&
            buffer[i] != ' ' && buffer[i] != '\n' &&
            buffer[i] != '\r' && buffer[i] != '\t') {
            firstWord += buffer[i];
            i++;
        }
        cout << "   First word: " << firstWord << "\n";
    }
    else {
        cout << "   No words found in the file\n";
    }

    // Test 3: countPunctuationInFile
    cout << "\n4. Testing countPunctuationInFile() function...\n";
    cout << "   Processing file '" << filename << "'...\n";

    if (countPunctuation(filename.c_str())) {
        cout << "   Success! Punctuation counted and results added to file.\n\n";

        // Display results
        cout << "=== COUNTING RESULTS ===\n";
        cout << getResults() << endl;

        // Open file in Notepad
        cout << "5. Opening processed file in Notepad...\n";
        string command = "notepad.exe \"" + filename + "\"";
        system(command.c_str());
    }
    else {
        cerr << "   ERROR: Failed to process file!\n";
    }

    // Clean up
    cleanupFunc();

    // Unload DLL
    FreeLibrary(hDLL);

    cout << "\n============================================\n";
    cout << "Program completed successfully.\n";
    cout << "Press Enter to exit...";
    cin.get();

    return 0;
}