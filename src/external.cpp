#pragma once

#include <map>
#include <set>
#include <string>

using namespace std;

/**
 * @brief Abstract class describing an external library or file(s) that can be included in a project.*
 */
class External {
  public:
    enum external_type { cli11, spdlog };
    external_type type;

  protected:
    External() {}

  public:
    string name;
    string description;

    enum category_type { logging, cli, util, etc };
    category_type category = External::etc;

    string github_url = "";
    string cmake_prefix = "";
    string cmake_suffix = "";

    set<string> imports;

    /**
     * @brief Code added to beginning of main function
     */
    string prefix;

    /**
     * @brief Code added to end of main function
     */
    string suffix;

    /**
     * @brief Other files that this dependency creates
     */
    map<string, string> files;
};