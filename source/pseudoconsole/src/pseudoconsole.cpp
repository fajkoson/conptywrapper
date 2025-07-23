#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <windows.h>
#include <iostream>
#include <vector>

namespace py = pybind11;

class PseudoConsole {
public:
    PseudoConsole() : hPC(nullptr), hInputWrite(nullptr), hOutputRead(nullptr) {}

    bool spawn(const std::string& command) {
        COORD size = {80, 25};
        HANDLE hInputRead, hOutputWrite;

        // Create pipes for ConPTY
        if (!CreatePipe(&hInputRead, &hInputWrite, nullptr, 0)) return false;
        if (!CreatePipe(&hOutputRead, &hOutputWrite, nullptr, 0)) return false;

        // Create the Pseudo Console
        HRESULT hr = CreatePseudoConsole(size, hInputRead, hOutputWrite, 0, &hPC);
        if (FAILED(hr)) return false;

        CloseHandle(hInputRead);
        CloseHandle(hOutputWrite);

        // Setup attributes for CreateProcess
        STARTUPINFOEX si = { sizeof(STARTUPINFOEX) };
        SIZE_T attrListSize;
        InitializeProcThreadAttributeList(nullptr, 1, 0, &attrListSize);
        si.lpAttributeList = (LPPROC_THREAD_ATTRIBUTE_LIST)HeapAlloc(GetProcessHeap(), 0, attrListSize);
        InitializeProcThreadAttributeList(si.lpAttributeList, 1, 0, &attrListSize);
        UpdateProcThreadAttribute(
            si.lpAttributeList,
            0,
            PROC_THREAD_ATTRIBUTE_PSEUDOCONSOLE,
            hPC,
            sizeof(HPCON),
            nullptr,
            nullptr
        );

        PROCESS_INFORMATION pi{};
        std::wstring wcmd(command.begin(), command.end());

        BOOL result = CreateProcessW(
            nullptr,
            wcmd.data(),
            nullptr,
            nullptr,
            FALSE,
            EXTENDED_STARTUPINFO_PRESENT,
            nullptr,
            nullptr,
            &si.StartupInfo,
            &pi
        );

        DeleteProcThreadAttributeList(si.lpAttributeList);
        HeapFree(GetProcessHeap(), 0, si.lpAttributeList);

        if (!result) return false;

        CloseHandle(pi.hThread);
        hProcess = pi.hProcess;

        return true;
    }

    std::string read() {
        char buffer[1024];
        DWORD bytesRead;
        if (ReadFile(hOutputRead, buffer, sizeof(buffer) - 1, &bytesRead, nullptr) && bytesRead > 0) {
            buffer[bytesRead] = '\0';
            return std::string(buffer);
        }
        return "";
    }

    void write(const std::string& input) {
        DWORD bytesWritten;
        WriteFile(hInputWrite, input.c_str(), static_cast<DWORD>(input.size()), &bytesWritten, nullptr);
    }

    void close() {
        if (hPC) ClosePseudoConsole(hPC);
        if (hInputWrite) CloseHandle(hInputWrite);
        if (hOutputRead) CloseHandle(hOutputRead);
        if (hProcess) CloseHandle(hProcess);
    }

    ~PseudoConsole() {
        close();
    }

private:
    HPCON hPC;
    HANDLE hInputWrite;
    HANDLE hOutputRead;
    HANDLE hProcess = nullptr;
};

PYBIND11_MODULE(pseudoconsole, m) {
    m.doc() = "Minimal PseudoConsole wrapper using ConPTY and pybind11";

    py::class_<PseudoConsole>(m, "PseudoConsole")
        .def(py::init<>())
        .def("spawn", &PseudoConsole::spawn)
        .def("read", &PseudoConsole::read)
        .def("write", &PseudoConsole::write)
        .def("close", &PseudoConsole::close);
}
