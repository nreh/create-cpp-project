#include <vector>

#include "../external.cpp"
#include "../info.cpp"
#include "../utils.h"

using namespace std;

class E_cli11 : public External {
  public:
    E_cli11(Info project_info) {
        name = "CLI11";
        description = "Command line parser for C++11 and beyond that provides a rich feature set with a simple and "
                      "intuitive interface.";

        category = category_type::cli;

        github_url = "https://github.com/CLIUtils/CLI11";

        cmake_prefix = "add_subdirectory(external/CLI11)";
        cmake_suffix = "target_link_libraries(runme CLI11::CLI11)";

        imports.emplace("<CLI/App.hpp> // CLI library is used for getting command line arguments");
        imports.emplace("<CLI/Config.hpp>");
        imports.emplace("<CLI/Formatter.hpp>");

        type = cli11;

        prefix = R"(
    /**
     * Set up CLI app and parse arguments into variables
     */)";

        prefix += "\n    CLI::App app(\"" + escape_string(project_info.description) + "\");\n";

        suffix = "    CLI11_PARSE(app);\n";
    }
};