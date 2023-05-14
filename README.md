# create-cpp-project

Utility program for quickly creating a git repo for a C++ project with libraries and structure that I commonly use.

Sets up .clang_format formatting, project structure, CMakeLists.txt, and installs useful external libraries as git submodules.

## Installation

```
./INSTALL.sh
```

and run `UNINSTALL.sh` to uninstall the program:

```
./UNINSTALL.sh
```

## Usage

Run `create-cpp-project` to start the program which will then create a directory in the current folder containing project files.

You can pass the name and description to the command like so,

```
create-cpp-project "My C++ Project" "Some text describing my project"
```

Run `create-cpp-project -h` to see possible flags and misc information.