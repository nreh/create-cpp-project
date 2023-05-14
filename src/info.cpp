#pragma once

#include <memory>
#include <string>
#include <vector>

#include "external.cpp"

using namespace std;

struct Info {
    string name;
    string description;
    vector<shared_ptr<External>> external_tools;

    /**
     * @brief Checks if the project has an external dependency of some type
     *
     * @param type Type of dependency to check for
     * @return true Project uses this dependency
     * @return false Project does not use this dependency
     */
    bool has_external(External::external_type type) {
        for (auto const& p : external_tools) {
            if (p->type == type) {
                return true;
            }
        }

        return false;
    }
};