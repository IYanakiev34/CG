# How to setup?

1. `git clone --recurse-submodules git@github.com:IYanakiev34/CG.git`
2. You need to have CMake installed or use `sudo apt install cmake -y`
After this is done you should have the repository cloned and have access to 
openGL because of glfw and glew which is the wrangler library. In order to run
the project you need to naviagte to the build folder and type: `make`.

## Crash course on CMake
CMake is a bild system which is highly more advanced than plain old make. The
power of CMake is that it can generate different types of project files. For
example it can generate xCode solutions, VS solutions,make files etc. The most basic
way in which you work with it is the following. Assume you make a new class called
Shader and you have a Shader.h and Shader.cpp in the `src` folder. Then you only
need to add the following to the CMakeLists.txt file
```bash
 # find the line with add_executable and add your files
 # after modification it should look something like this
 add_executable(${PROJECT_NAME} src/main.cpp src/Shader.cpp)
```
It is important to understand that you only need to add the sources. The headers
no need. everything should be set up to work. If you make another folder lets say
include and you place the headers there and the implementations in a src/lib folder.
Then you would need to specify the include directory also. You can see in the 
CMakeLists.txt we have a line:
```bash
    target_include_directories()
```
This specifies which directories to search for header files.

1. target - an executable name or library name
2. set - sets a variable value
3. target_link_libraries - link some libraries to a target (executable)
Links could be PRIVATE PUBLIC or INTERFACE. PRIVATE means that only
your executable can use the libraries. PUBLIC means that if someone lets say
uses your executable in their they can also use the libraries you linked to.
INTERFACE is used only when you link against libraries which are header only.
(Usually never)
4. target_include_directories - specify the include directories for some target
Keep in mind a target can be an executable or even a library
Those are the basic features you need to use cmake.

## TODO
- [ ] Add basic shader class
- [ ] Create basic window to verify that everything is working
