#include "utils.h"

int main() {
    Utils g_Utils;

    g_Utils.waitForLoadingModules();

    // get the path to the current executable
    char szPath[MAX_PATH];
    GetModuleFileNameA(NULL, szPath, sizeof(szPath) / sizeof(char));

    // get the filename without the path
    char* fileName = strrchr(szPath, '\\');
    if (fileName != NULL) {
        fileName++; // move past the last backslash
    }

    // get a handle to the current process
    HANDLE hProcess = GetCurrentProcess();

    // define an array to store module handles
    HMODULE hModules[1024];
    DWORD cbNeeded;

    // vector to store non-whitelisted modules
    std::vector<std::string> nonWhitelistedModules;

    // enumerate the loaded modules in the process
    if (EnumProcessModules(hProcess, hModules, sizeof(hModules), &cbNeeded)) {
        for (unsigned int i = 0; i < (cbNeeded / sizeof(HMODULE)); i++) {
            char szModName[MAX_PATH];

            // get the full path of the module
            if (GetModuleFileNameExA(hProcess, hModules[i], szModName, sizeof(szModName) / sizeof(char))) {
                char* modFileName = strrchr(szModName, '\\');
                if (modFileName != NULL) {
                    modFileName++; // move past the last backslash
                }

                if (!g_Utils.isModuleWhitelisted(modFileName)) {
                    nonWhitelistedModules.push_back(modFileName);
                }
            }
        }
    }

    // close the process handle
    CloseHandle(hProcess);

    // check if any non-whitelisted modules were found
    if (!nonWhitelistedModules.empty()) {
        std::cout << "[ > ] non-whitelisted modules loaded:" << std::endl;
        for (const std::string& moduleName : nonWhitelistedModules) {
            printf("module: %s --> at address: 0x%p\n", moduleName.c_str(), &moduleName);
        }

        g_Utils.outputWithColor(FOREGROUND_RED, "detected");
        
        // add code here to perform any necessary cleanup or logging
        // then exit the process
        ExitProcess(EXIT_FAILURE);
    }

    g_Utils.outputWithColor(FOREGROUND_GREEN, "all right");

    return EXIT_SUCCESS;
}
