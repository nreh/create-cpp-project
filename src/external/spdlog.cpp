#include <map>
#include <vector>

#include "../external.cpp"
#include "../info.cpp"
#include "../utils.h"

using namespace std;

class E_spdlog : public External {
  public:
    E_spdlog(Info project_info) {
        name = "spdlog";
        description = "Very fast, header-only/compiled, C++ logging library.";

        category = category_type::cli;

        github_url = "https://github.com/gabime/spdlog";

        cmake_prefix = "add_subdirectory(external/spdlog)";
        cmake_suffix =
            "target_compile_definitions(spdlog PUBLIC SPDLOG_COMPILED_LIB)\ntarget_link_libraries(runme spdlog::spdlog)";

        type = spdlog;

        imports.emplace("\"logging.cpp\" // contains #import <spdlog/spdlog.h> as well as configuration defines");

        prefix = "    bool verbose = false;\n";

        if (project_info.has_external(external_type::cli11)) {
            // because we have the CLI11 dependency, we can add the verbose flag
            prefix += "    app.add_flag(\"-v,--verbose\", verbose, \"Enable verbose debugging\")->default_val(false);\n";
        }

        suffix = "    // initalize and configure spdlog\n";
        suffix += "    logging::initialize(verbose);\n";

        files["logging.h"] = "#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE\n"
                             "#include <spdlog/spdlog.h>\n";

        files["logging.cpp"] = R"(// Logging initialization and configuration

#include <iostream>

#include "logging.h"

namespace logging {

/**
 * @brief Initialize logging
 *
 * @param verbose
 */
void initialize(bool verbose) {
    if (verbose) {
        SPDLOG_INFO("Logging level set to VERBOSE");
        spdlog::set_level(spdlog::level::trace); // Set global log level to trace
    } else {
        spdlog::set_level(spdlog::level::info);
    }

    // change log pattern
    spdlog::set_pattern("%-20s@ %-25!!%^[%l]: %v%$");
}

}
)";
    }
};