# Qt with CMake

The purpose of this project is to provide Qt6 build of the framework, without the need for `QtCreator`.
An independent `QtDesigner` can be used for modifying the `*.ui` files.

Environment required (untested = might not work, require additional setup):

- C/C++ compiler: GCC, Clang;
- An IDE: VSCode (potentially CLion, MS Visual Studio, etc);
- CMake: Used for building the project - generate project files or make files.

Depending on the IDE used knowledge of bash might be required.

Assuming Qt6 is installed and the current command-line directory is in the `src` directory:

1. `mkdir build`
    - creates a build directory;
2. `cd build`
    - change the current directory to the build;
3. `cmake ..`
    - if Qt6 is installed correctly and found in the default search directory. An error might occur otherwise.
4. `make`
    - compiles the project.
5. `OpenGL_0`
    - executes the compiled program.

Alternatively, `QtCreator` (and probably most other IDEs) allows you to directly compile and run the program.

# Versions

Qt5 is supported under Windows and Linux.

Qt6 is supported on Windows, macOS, and Linux using the installers found at <https://download.qt.io/official_releases/online_installers/>. Registration is required, but only an email address is required to register (no private data or payment is required; the phone number field can be left blank). ~5GiB of download is required. Additionally, Qt6 is also available in the Linux packages `qt6-base-dev` and `qt6-3d-dev`. 

Depending on your system, a Linux machine may require you to install graphics drivers in order to run OpenGL. The open-source Mesa drivers should be automatically offered in Ubuntu 18.04 and up. You can use the `glxinfo` command found in the `mesa-utils` package to check whether the drivers are installed and up to date:
```bash
glxinfo | grep Mesa
```

In case you encounter issues setting up Qt, please contact a TA via the Computer Graphics helpdesk or in a lab session.
