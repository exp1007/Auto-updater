#include <iostream>
#include <string>
#include <Windows.h>
#include <fstream>

#include "Networking.hpp"

using namespace std;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

// Allocate console if debugging
#if _DEBUG
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
#endif

    int argc;
    LPWSTR* argv = CommandLineToArgvW(GetCommandLine(), &argc);

    // If there are no arguments just exit
    if (argc < 2)
        return 1;

    size_t bufferSize = 254;
    char* auxArg = new char[bufferSize + 1];

// When debugging there's an additional first argument with the startup directory
#if _DEBUG
    if (argv[1] == NULL) {
    cout << "[Updater] Auto-update failed " << "(Null argument 0)";
    Sleep(5000);
    return 1;
    }

    wcstombs(auxArg, argv[1], bufferSize);
    string path = auxArg;

    if (argv[2] == NULL) {
        cout << "[Updater] Auto-update failed " << "(Null argument 1)";
        Sleep(5000);
        return 1;
    }

    wcstombs(auxArg, argv[2], bufferSize);
    string sessionKey = auxArg;

    if (argv[3] == NULL) {
        cout << "[Updater] Auto-update failed " << "(Null argument 2)";
        Sleep(5000);
        return 1;
    }

    wcstombs(auxArg, argv[3], bufferSize);
    bool beta = stoi(auxArg);
#else
    if (argv[0] == NULL) {
        cout << "[Updater] Auto-update failed " << "(Null argument 0)";
        Sleep(5000);
        return 1;
}

    wcstombs(auxArg, argv[0], bufferSize);
    string path = auxArg;

    if (argv[1] == NULL) {
        cout << "[Updater] Auto-update failed " << "(Null argument 1)";
        Sleep(5000);
        return 1;
    }

    wcstombs(auxArg, argv[1], bufferSize);
    string sessionKey = auxArg;

    if (argv[2] == NULL) {
        cout << "[Updater] Auto-update failed " << "(Null argument 2)";
        Sleep(5000);
        return 1;
    }

    wcstombs(auxArg, argv[2], bufferSize);
    bool beta = stoi(auxArg);
#endif

    delete[] auxArg;

    // Here we build the url
    string url = "https://site.com/download.php";
    url = url + "sessionID=" + sessionKey + "&buid=" + to_string(beta);

#if _DEBUG
    cout << "[Updater] Final URL ->" << url << endl;
    cout << "[Updater] Received params ->" << " Path:" << path << " SessionID:" << sessionKey << " Build:" << beta << endl;
    Sleep(3000);
#endif

    // Find the targeted binary that we want to replace
    Sleep(100);
    while (FindWindowA(NULL, "Software_name")) {
        Sleep(100);
    }

    DeleteFileA(path.c_str());

    std::string replyDownload = downloadFile(url.c_str(), path.c_str());
    if (replyDownload != "Success")
        cout << replyDownload; // Print the error

    // additional information
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;

    // set the size of the structures
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // Start the program up
    CreateProcessA(
        path.c_str(),   // the path
        NULL,           // Command line
        NULL,           // Process handle not inheritable
        NULL,           // Thread handle not inheritable
        FALSE,          // Set handle inheritance to FALSE
        0,              // No creation flags
        NULL,           // Use parent's environment block
        NULL,           // Use parent's starting directory 
        &si,            // Pointer to STARTUPINFO structure
        &pi             // Pointer to PROCESS_INFORMATION structure (removed extra parentheses)
    );

    // Close process and thread handles. 
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);


    return 0;
}