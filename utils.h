#include <iostream>
#include <windows.h>
#include <psapi.h>
#include <vector>
#include <string>

// define a list of whitelisted module names or paths
// you need to debug and check modules which loaded into process
// and put it to this list
const std::vector<std::string> whitelist = {
    "ntdll.dll",
    "KERNEL32.DLL",
    "KERNELBASE.dll",
};

class Utils {

public:
    void outputWithColor(WORD attributes, std::string output);
    bool isModuleWhitelisted(const std::string& moduleName);
    void waitForLoadingModules();
};