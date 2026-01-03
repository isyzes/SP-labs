#include <windows.h>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <vector>

using namespace std;

// Структура для передачи данных в поток
struct ThreadData {
    int launchNumber;
    string fileName;
};

// Функция потока для управления Notepad
DWORD WINAPI ManageNotepadThread(LPVOID lpParam) {
    ThreadData* data = (ThreadData*)lpParam;

    // Формируем текст для отображения
    string content = "Запуск № " + to_string(data->launchNumber);

    // Записываем текст во временный файл
    HANDLE hFile = CreateFileA(
        data->fileName.c_str(),
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hFile != INVALID_HANDLE_VALUE) {
        DWORD bytesWritten;
        WriteFile(hFile, content.c_str(), content.length(), &bytesWritten, NULL);
        CloseHandle(hFile);
    }

    // Запускаем Notepad с нашим файлом
    STARTUPINFOA si = { sizeof(si) };
    PROCESS_INFORMATION pi;

    string command = "notepad.exe " + data->fileName;

    if (CreateProcessA(
        NULL,
        (LPSTR)command.c_str(),
        NULL,
        NULL,
        FALSE,
        0,
        NULL,
        NULL,
        &si,
        &pi)) {

        // Ждем 10 секунд
        this_thread::sleep_for(chrono::seconds(10));

        // Закрываем процесс Notepad
        TerminateProcess(pi.hProcess, 0);

        // Закрываем дескрипторы
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        // Удаляем временный файл
        DeleteFileA(data->fileName.c_str());
    }

    delete data;
    return 0;
}

int main() {
    SetConsoleOutputCP(1251); // Для поддержки русского языка в консоли

    cout << "Программа запущена. Ожидание 15 секунд...\n";

    for (int i = 1; i <= 10; i++) {
        // Ждем 15 секунд
        this_thread::sleep_for(chrono::seconds(15));

        cout << "Запуск Notepad № " << i << "\n";

        // Создаем данные для потока
        ThreadData* data = new ThreadData;
        data->launchNumber = i;
        data->fileName = "temp_note_" + to_string(i) + ".txt";

        // Создаем поток для управления Notepad
        HANDLE hThread = CreateThread(
            NULL,
            0,
            ManageNotepadThread,
            data,
            0,
            NULL
        );

        if (hThread != NULL) {
            CloseHandle(hThread); // Мы не ждем завершения потока
        }
    }

    cout << "\nВсе 10 запусков завершены. Программа завершается.\n";
    system("pause");
    return 0;
}