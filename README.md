# About
Auto-updater created in C++ using cURL Lib to download a file to a specified path using command line arguments.

# Implementation
This can not be directly added into the program that has to be updated since it runs as a spearate process. The updater will wait untill the main program is closed and then it will get replaced.
The intended way to use this is by checking in your main program if it's out of date and then launch (CreateProcessA) the auto-updater using command line arguments.

```c++
std::string cmdArgs = mainProgramPath + " " + sessionID + " " + std::to_string(build);

std::string localPath = getExePath(); // Use GetModuleFileName

std::string updaterPath = "C:\\Users\\" + user + "\\AppData\\Roaming" + "\\Auto-updater.exe";
```
```c++
    // Additional information
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;

    // Set the size of the structures
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // start the program up
    CreateProcessA(
        updaterPath.c_str(),   // the path
        const_cast<char*>(args.c_str()),        // Command line
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
exit(0);
```


# Example cmd arguments

```
C:\Users\example\AppData\Roaming\Auto-updater.exe sessionID_12345 1.8.9
```


