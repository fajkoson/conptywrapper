#include <windows.h>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

void log_console_live(HANDLE hConsole, SHORT startRow, const std::string& logFile) {
    std::ofstream log(logFile);
    if (!log) {
        std::cerr << "Failed to open log file!\n";
        return;
    }

    CHAR buffer[256];
    DWORD charsRead;
    COORD coord = {0, startRow};
    SHORT currentRow = startRow;
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    while (true) {
        if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) break;

        while (currentRow < csbi.dwCursorPosition.Y) {
            COORD readCoord = {0, currentRow};
            if (ReadConsoleOutputCharacterA(hConsole, buffer, sizeof(buffer) - 1, readCoord, &charsRead) && charsRead > 0) {
                buffer[charsRead] = '\0';
                std::string line(buffer);
                std::cout << line << std::endl;
                log << line << std::endl;
                log.flush();
            }
            currentRow++;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        if (GetAsyncKeyState(VK_ESCAPE)) break;
    }
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

    // Log console output to hostcon.log
    std::thread logger([&](){ log_console_live(hConsole, 0, "hostcon.log"); });

    WaitForSingleObject(pi.hProcess, INFINITE);

    logger.detach(); 
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return 0;
}

