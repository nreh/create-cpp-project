#include <iostream>
#include <string>

#include <CLI/App.hpp> // CLI library is used for getting command line arguments
#include <CLI/Config.hpp>
#include <CLI/Formatter.hpp>

#include <array>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

using namespace std;

int main() {
    /**
     * Set up CLI app and parse arguments into variables
     */
    CLI::App app("Utility program for quickly creating C++ projects");

    CLI11_PARSE(app);

    cout << exec("git statuss") << endl;
}