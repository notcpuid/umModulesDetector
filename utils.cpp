#include "utils.h"

void Utils::outputWithColor(WORD attributes, std::string output) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), attributes);

    printf(output.c_str());

    // set to default
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

bool Utils::isModuleWhitelisted(const std::string& moduleName) {
    for (const std::string& whitelistedModule : whitelist) {
        if (moduleName == whitelistedModule) {
            return true; // module is whitelisted
        }
    }

    // do not check this module
    size_t dotPos = moduleName.find_last_of('.');
    if (dotPos != std::string::npos) {
        std::string extension = moduleName.substr(dotPos);
        if (extension == ".exe") {
            return true; // module has the ".exe" extension
        }
    }

    return false; // module is not whitelisted
}

// use it because something modules can't load on time
void Utils::waitForLoadingModules() {
    for (const std::string whitelistedModule : whitelist) {
        while (!GetModuleHandleA(whitelistedModule.c_str())) {
            Sleep(200);
        }
    }
}