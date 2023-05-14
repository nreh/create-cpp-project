#include <iostream>
#include <memory>
#include <string>

#include <CLI/App.hpp> // CLI library is used for getting command line arguments
#include <CLI/Config.hpp>
#include <CLI/Formatter.hpp>

#include <termcolor.hpp>

#include "create.cpp"
#include "external.cpp"
#include "info.cpp"
#include "utils.h"

#include "external/cli_11.cpp"
#include "external/spdlog.cpp"

using namespace std;

int main() {
    /**
     * Set up CLI app and parse arguments into variables
     */
    CLI::App app("Utility program for quickly creating git repo for a C++ project");

    Info project_info;

    CLI11_PARSE(app);

    string_prompt("Project Name", [&](string name) { project_info.name = name; });

    string_prompt(
        "Project Description", [&](string description) { project_info.description = description; }, "My C++ project"
    );

    cout << endl;
    grey_text("CLI11 is a command line parser for C++11 and beyond that provides a rich feature set with a simple and "
              "intuitive interface.");

    boolean_prompt(
        "Use CLI11 library?",
        [&]() {
            auto pointer = make_shared<External>(E_cli11(project_info));
            project_info.external_tools.push_back(pointer);
        },
        [&]() {},
        true
    );

    cout << endl;
    grey_text("spdlog is a very fast, header-only/compiled, C++ logging library.");
    boolean_prompt(
        "Use spdlog library?",
        [&]() {
            auto pointer = make_shared<External>(E_spdlog(project_info));
            project_info.external_tools.push_back(pointer);
        },
        [&]() {},
        true
    );

    cout << endl;
    create_project(project_info);
}