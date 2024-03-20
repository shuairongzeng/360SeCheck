#include <iostream>
#include <windows.h>
#include <fstream>

bool fileExists(const std::wstring& path) {
    DWORD attributes = GetFileAttributesW(path.c_str());
    return (attributes != INVALID_FILE_ATTRIBUTES &&
        !(attributes & FILE_ATTRIBUTE_DIRECTORY));
}

void createFile(const std::wstring& path) {
    std::wofstream file(path);
    if (file.is_open()) {
        std::wcout << L"File created: " << path << std::endl;
        file << L"Hello, this is a test file.\n";
        file.close();
    }
    else {
        std::wcout << L"Failed to create file: " << path << std::endl;
    }
}

void executeAExe(const std::wstring& path) {
    STARTUPINFOW si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    if (!CreateProcessW(path.c_str(), NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        std::wcerr << L"CreateProcess failed (" << GetLastError() << L")." << std::endl;
    }
    else {
        std::wcout << L"Started " << path << L" successfully." << std::endl;
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
}

std::wstring getCurrentDirectory() {
    wchar_t path[MAX_PATH];
    DWORD length = GetModuleFileNameW(NULL, path, MAX_PATH);
    if (length == 0 || length == MAX_PATH) {
        std::wcout << L"Failed to get the module file name." << std::endl;
        return L"";
    }

    std::wstring pathStr(path);
    size_t pos = pathStr.find_last_of(L"\\/");
    return pos != std::wstring::npos ? pathStr.substr(0, pos + 1) : L"";
}

int main() {
    std::wstring currentDir = getCurrentDirectory();
    if (currentDir.empty()) {
        return -1;
    }

    std::wstring iniPath = currentDir + L"check.ini";
    std::wcout << L"Path to check.ini: " << iniPath << std::endl;

    if (fileExists(iniPath)) {
        std::wcout << L"check.ini exists at the path." << std::endl;
    }
    else {
        std::wcout << L"check.ini does not exist at the path, creating..." << std::endl;
        createFile(iniPath);

        std::wstring exePath = currentDir + L"a.exe";
        executeAExe(exePath);
    }

    return 0;
}
