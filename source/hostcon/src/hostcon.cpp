#include <windows.h>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

// Utility to trim trailing whitespace from a string
static inline void trim_right(std::string& s) {
    auto pos = s.find_last_not_of(" \t\r\n");
    if (pos != std::string::npos)
        s.erase(pos + 1);
    else
        s.clear();
}

void log_console_live(HANDLE hConsole, const std::string& logFile) {
    std::ofstream log(logFile);
    if (!log) {
        std::cerr << "Failed to open log file!\n";
        return;
    }

    // Get initial console buffer info
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);

    // Start logging from the current bottom of buffer
    SHORT lastLoggedRow = csbi.dwCursorPosition.Y;
    const SHORT bufferHeight = csbi.dwSize.Y;
    const SHORT bufferWidth = csbi.dwSize.X;
    DWORD charsRead = 0;
    CHAR* buffer = new CHAR[bufferWidth + 1];

    while (true) {
        // Refresh buffer info to see cursor movement
        if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) break;
        SHORT currentRow = csbi.dwCursorPosition.Y;

        // Handle buffer wrap-around
        while (lastLoggedRow != currentRow) {
            COORD readCoord = { 0, lastLoggedRow };
            DWORD charsRead = 0;
            // Read the entire width of the console buffer row
            if (ReadConsoleOutputCharacterA(hConsole, buffer, bufferWidth, readCoord, &charsRead) && charsRead > 0) {
                buffer[charsRead] = '\0';
                std::string line(buffer);
                trim_right(line);
                if (!line.empty()) {
                    log << line << std::endl;
                    log.flush();
                }
            }
            // Advance and wrap around if needed
            lastLoggedRow = (lastLoggedRow + 1) % bufferHeight;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        // Optional: break early with ESC
        if (GetAsyncKeyState(VK_ESCAPE)) break;
    }
    delete[] buffer;
}

int main() {
    AllocConsole();
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    STARTUPINFOA si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    std::string cmd = R"(d:\gamez\steamapps\common\Factorio\bin\x64\factorio.exe --benchmark benchmarks\0001-iron-smelter\iron_smelter_enable_clocked.zip --benchmark-ticks 5000 --disable-audio)";

    BOOL ok = CreateProcessA(
        NULL, &cmd[0], NULL, NULL, FALSE,
        CREATE_NEW_CONSOLE,
        NULL, NULL, &si, &pi);

    if (!ok) {
        std::cerr << "Failed to start Factorio\n";
        return 1;
    }

    // Start clean logger in background thread
    std::thread logger([&]() { log_console_live(hConsole, "hostcon.log"); });

    WaitForSingleObject(pi.hProcess, INFINITE);

    // Optionally signal logger thread to exit (add flag, etc.)
    logger.detach();
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return 0;
}
