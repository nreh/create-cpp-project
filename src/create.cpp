// functions for creating the project and files in the project

#include <filesystem>
#include <fstream>
#include <iostream>
#include <pstream.h>
#include <set>
#include <string>
#include <termcolor.hpp>

#include "info.cpp"
#include "utils.h"

using namespace std;

/**
 * @brief Used to store stdout/stderr output from programs
 *
 */
struct out {
    /**
     * @brief stdout
     */
    string o;

    /**
     * @brief stderr
     */
    string e;

    int return_code;
};

out run_command(string command) {
    redi::ipstream proc(command, redi::pstreams::pstdout | redi::pstreams::pstderr);
    out r;

    string temp;

    while (!proc.out().eof()) {
        getline(proc.out(), temp);

        if (trim_copy(temp) == "")
            continue; // no need to print empty lines

        r.o += temp + '\n';
        cout << termcolor::grey << temp << endl << termcolor::reset << flush;
    }

    if (proc.eof() && proc.fail()) {
        proc.clear();
    }

    while (!proc.err().eof()) {
        getline(proc.err(), temp);

        if (trim_copy(temp) == "")
            continue; // no need to print empty lines

        r.e += temp + '\n';
        cout << termcolor::red << temp << endl << termcolor::reset << flush;
    }

    proc.close();

    if (proc.rdbuf()->exited()) {
        r.return_code = proc.rdbuf()->status();
    }

    return r;
}

void new_directory(filesystem::path dir) {

    string name = dir.string();
    cout << termcolor::bold << "Creating directory '" << name << "'... ";

    if (filesystem::exists(dir)) {
        cout << termcolor::red << "✕" << endl;
        cout << "File or directory with name '" << name << "' already exists" << endl;
        throw string("Error creating " + name + " directory");
    }

    try {
        if (filesystem::create_directory(dir)) {
            cout << termcolor::green << "✔" << endl << termcolor::reset;
        } else {
            cout << termcolor::red << "✕" << endl;
            throw string("Error creating " + name + " directory");
        }
    } catch (filesystem::filesystem_error e) {
        cout << termcolor::red << "✕" << endl;
        cout << termcolor::red << e.what() << endl;
        throw string("Error creating " + name + " directory");
    }
}

void new_file(filesystem::path file, const string& contents) {

    string name = file.string();
    cout << termcolor::bold << "Creating file '" << name << "'... ";

    if (filesystem::exists(file)) {
        cout << termcolor::red << "✕" << endl;
        cout << "File or directory with name '" << name << "' already exists" << endl;
        throw string("Error creating " + name);
    }

    try {

        ofstream writer(file.string(), ios::trunc);
        writer.write(contents.c_str(), contents.size());

        cout << termcolor::green << "✔" << endl << termcolor::reset;
    } catch (exception e) {
        cout << termcolor::green << "✕" << endl << termcolor::reset;
        cout << termcolor::red << e.what() << endl << termcolor::reset;
        throw string("Error creating " + name);
    }
}

bool create_project(Info& info) {
    try {
        filesystem::path project_directory("./" + info.name);

        new_directory(project_directory);

        run_command("cd '" + project_directory.string() + "' && git init");

        new_directory(project_directory / "src");
        new_directory(project_directory / "external");

        // download all dependencies as git submodules
        for (auto& e : info.external_tools) {
            cout << "Adding submodule from " << e->github_url << endl;
            auto o = run_command(
                "cd '" + (project_directory / "external").string() + "' && git submodule add " + e->github_url + ".git 2>&1"
            );

            if (o.return_code != 0) {
                throw string(
                    "Unable to download dependency '" + e->name + "' from " + e->github_url + " (return code " +
                    to_string(o.return_code) + ")"
                );
            }
        }

        cout << termcolor::bold << "External dependencies added " << termcolor::green << "✔" << termcolor::reset << endl;

        // create README file for /external

        string contents = "# External Libraries and Tools\n\n";
        for (auto& e : info.external_tools) {
            contents += "- [" + e->name + "](" + e->github_url + ") - " + e->description + "\n";
        }

        new_file(project_directory / "external/README.md", contents);

        // create project README

        contents = "# " + info.name + "\n\n" + info.description + "\n\n";

        contents += R"(
## Quick Start

Before building project, initialize and update submodules,
```
git submodule init
git submodule update
```

To build project, in root project folder,
```
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```

and then to run,
```
./runme --help
```
)";

        new_file(project_directory / "README.md", contents);

        // create main.cpp
        // combine imports making sure there are no collisions
        set<string> default_imports; // no default imports as of yet, but keeping this here for now...

        contents = "";

        for (auto& e : info.external_tools) {
            for (auto i : e->imports) {
                if (default_imports.count(i) == 0) {
                    // hasn't been imported yet
                    contents += "#include " + i + "\n";
                }
            }
            contents += '\n';
        }

        contents += R"(
using namespace std;

int main() {
)";

        for (auto& e : info.external_tools) {
            contents += e->prefix + "\n";
        }

        contents += R"(
    /**
     * Your code here...
     */
    )";

        contents += '\n';

        for (auto& e : info.external_tools) {
            contents += e->suffix;
        }

        contents += "\n}";

        new_file(project_directory / "src/main.cpp", contents);

        // create any others files

        for (auto& e : info.external_tools) {
            for (const auto& [key, value] : e->files) {
                new_file(project_directory / "src" / key, value);
            }
        }

        // create CMakeLists.txt

        contents = "cmake_minimum_required(VERSION 3.16)\n\n";

        contents += "project(\n";
        contents += "   " + info.name + "\n";
        contents += "   VERSION 1.0\n";
        contents += "   DESCRIPTION \"" + info.description + "\"\n";
        contents += "   LANGUAGES CXX\n";
        contents += ")\n";

        contents += R"(
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

)";

        for (auto& e : info.external_tools) {
            contents += e->cmake_prefix + "\n";
        }

        contents += "\nadd_executable(runme ./src/main.cpp)\n\n";

        for (auto& e : info.external_tools) {
            contents += e->cmake_suffix + "\n";
        }

        new_file(project_directory / "CMakeLists.txt", contents);

        new_file(project_directory / ".gitignore", ".vscode/\nbuild/");

        contents = R"(IndentWidth: 4
ColumnLimit: 125
IndentPPDirectives: BeforeHash
PointerAlignment: Left
AlignAfterOpenBracket: BlockIndent
BinPackArguments: false
BinPackParameters: false
)";

        new_file(project_directory / ".clang-format", contents);

    } catch (string e) {
        cout << termcolor::red << termcolor::bold << e << endl << termcolor::reset;
        return false;
    }

    return true;
}