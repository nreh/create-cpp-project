#pragma once

#include <algorithm>
#include <cctype>
#include <functional>
#include <iostream>
#include <locale>
#include <string>
#include <termcolor.hpp>

#pragma region String Trimming
// trim from start (in place)
static inline void ltrim(std::string& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) { return !std::isspace(ch); }));
}

// trim from end (in place)
static inline void rtrim(std::string& s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string& s) {
    rtrim(s);
    ltrim(s);
}

// trim from start (copying)
static inline std::string ltrim_copy(std::string s) {
    ltrim(s);
    return s;
}

// trim from end (copying)
static inline std::string rtrim_copy(std::string s) {
    rtrim(s);
    return s;
}

// trim from both ends (copying)
static inline std::string trim_copy(std::string s) {
    trim(s);
    return s;
}
#pragma endregion

std::string replace_all(const std::string& str, const std::string& from, const std::string& to) {
    std::string temp = str;

    if (from.empty())
        return temp;
    size_t start_pos = 0;
    while ((start_pos = temp.find(from, start_pos)) != std::string::npos) {
        temp.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }

    return temp;
}

/**
 * @brief Escape string so that it can be safely placed as a string in C++ code
 *
 * @param str String to escape
 * @return std::string
 */
std::string escape_string(std::string str) { return replace_all(str, "\"", "\\\""); }

void boolean_prompt(std::string prompt, function<void()> on_yes, function<void()> on_no) {
    while (true) {
        std::cout << termcolor::bold << prompt << " " << termcolor::reset << termcolor::grey << "(y/n) " << termcolor::reset;

        cin.clear();
        cin.sync();

        std::string input;
        getline(cin, input);

        trim(input);

        if (input == "") {
            // user just gave whitespace
            std::cout << termcolor::red << "This field is required" << termcolor::reset << std::endl << std::endl;
            continue;
        }

        char selection = tolower(input[0]);
        if (selection == 'y') {
            on_yes();
            break;
        } else if (selection == 'n') {
            on_no();
            break;
        } else {
            std::cout << termcolor::red << "Invalid input '" << termcolor::bold << input << termcolor::reset
                      << termcolor::red << "'" << termcolor::reset << std::endl
                      << std::endl;
        }
    }
}
void boolean_prompt(std::string prompt, function<void()> on_yes, function<void()> on_no, bool default_value) {
    while (true) {
        std::cout << termcolor::bold << prompt << " " << termcolor::reset << termcolor::grey;
        if (default_value) {
            std::cout << "(" << termcolor::bold << termcolor::white << "y" << termcolor::reset << termcolor::grey << "/n) "
                      << termcolor::reset;
        } else {
            std::cout << "(y/" << termcolor::bold << termcolor::white << "n" << termcolor::reset << termcolor::grey << ") "
                      << termcolor::reset;
        }

        cin.clear();
        cin.sync();

        std::string input;
        getline(cin, input);

        trim(input);

        if (input == "") {
            // user just gave whitespace
            char o = 'n';
            if (default_value)
                o = 'y';

            std::cout << "\e[A\r"; // move up one line
            std::cout << termcolor::bold << prompt << " " << termcolor::reset << termcolor::grey;
            if (default_value) {
                std::cout << "(" << termcolor::bold << termcolor::white << "y" << termcolor::reset << termcolor::grey
                          << "/n) " << termcolor::reset;
            } else {
                std::cout << "(y/" << termcolor::bold << termcolor::white << "n" << termcolor::reset << termcolor::grey
                          << ") " << termcolor::reset;
            }
            std::cout << termcolor::grey << termcolor::italic << o << termcolor::reset << endl;

            if (default_value) {
                on_yes();
            } else {
                on_no();
            }

            break;
        }

        char selection = tolower(input[0]);
        if (selection == 'y') {
            on_yes();
            break;
        } else if (selection == 'n') {
            on_no();
            break;
        } else {
            std::cout << termcolor::red << "Invalid input '" << termcolor::bold << input << termcolor::reset
                      << termcolor::red << "'" << termcolor::reset << std::endl
                      << std::endl;
        }
    }
}

void string_prompt(std::string prompt, function<void(string)> on_submit) {
    std::string temp;

    while (true) {
        std::cout << termcolor::bold << prompt << ": " << termcolor::reset;
        getline(std::cin, temp);

        trim(temp);

        if (temp == "") {
            // user just gave whitespace
            std::cout << termcolor::red << "This field is required" << termcolor::reset << std::endl << std::endl;
        } else {
            on_submit(temp);
            break;
        }
    }
}
void string_prompt(std::string prompt, function<void(string)> on_submit, std::string default_value) {
    std::string temp;

    while (true) {
        std::cout << termcolor::bold << prompt << termcolor::reset << termcolor::grey << " (Default is '" << default_value
                  << "')" << termcolor::reset << termcolor::bold << ": " << termcolor::reset;
        getline(std::cin, temp);

        trim(temp);

        if (temp == "") {
            // user just gave whitespace
            std::cout << "\e[A\r"; // move up one line
            std::cout << termcolor::bold << prompt << termcolor::reset << termcolor::grey << " (Default is '"
                      << default_value << "')" << termcolor::reset << termcolor::bold << ": " << termcolor::reset;
            std::cout << termcolor::grey << termcolor::italic << default_value << termcolor::reset << endl;
            on_submit(default_value);
            break;
        } else {
            on_submit(temp);
            break;
        }
    }
}

void grey_text(std::string str) { cout << termcolor::grey << str << termcolor::reset << endl; }