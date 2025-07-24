#include <windows.h>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

// Utility: trim trailing whitespace from a string (for clean logs)
static inline void trim_right(std::string& s) {
    auto pos = s.find_last_not_of(" \t\r\n");
    if (pos != std::string::npos)
        s.erase(pos + 1);
    else
        s.clear();
}

// This function logs every new line printed to the Windows console buffer, in real-time, to a log file.
void log_console_live(HANDLE hConsole, const std::string& logFile) {
    std::ofstream log(logFile);
    if (!log) {
        std::cerr << "Failed to open log file for writing!\n";
        return;
    }

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);

    SHORT lastLoggedRow = csbi.dwCursorPosition.Y;
    const SHORT bufferHeight = csbi.dwSize.Y;
    const SHORT bufferWidth = csbi.dwSize.X;
    CHAR* buffer = new CHAR[bufferWidth + 1];

    while (true) {
        if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) break;
        SHORT currentRow = csbi.dwCursorPosition.Y;

        // Log all new rows since last check
        while (lastLoggedRow != currentRow) {
            COORD readCoord = { 0, lastLoggedRow };
            DWORD charsRead = 0;
            if (ReadConsoleOutputCharacterA(hConsole, buffer, bufferWidth, readCoord, &charsRead) && charsRead > 0) {
                buffer[charsRead] = '\0';
                std::string line(buffer);
                trim_right(line);
                if (!line.empty()) {
                    log << line << std::endl;
                    log.flush();
                }
            }
            lastLoggedRow = (lastLoggedRow + 1) % bufferHeight;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        // This loop will run until the parent process closes the console or exits.
    }
    delete[] buffer;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: hostcon.exe <logfile> <factorio_path> [args...]\n";
        return 1;
    }
    AllocConsole();
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // First argument: log file path
    std::string log_file = argv[1];

    // Remaining arguments: Factorio command and its arguments
    std::string cmd;
    for (int i = 2; i < argc; ++i) {
        if (i > 2) cmd += " ";
        cmd += argv[i];
    }

    STARTUPINFOA si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    char* cmdline = &cmd[0];

    BOOL ok = CreateProcessA(
        NULL, cmdline, NULL, NULL, FALSE,
        CREATE_NEW_CONSOLE,
        NULL, NULL, &si, &pi);

    if (!ok) {
        std::cerr << "Failed to start Factorio (or target process)\n";
        return 1;
    }

    // Start logging in a background thread
    std::thread logger([&]() { log_console_live(hConsole, log_file); });

    // Wait until Factorio (or your target process) exits
    WaitForSingleObject(pi.hProcess, INFINITE);

    logger.detach(); // Don't wait for logger (console will close soon anyway)
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return 0;
}