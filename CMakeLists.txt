cmake_minimum_required(VERSION 3.27)

project(hyprglot
  LANGUAGES CXX
  DESCRIPTION "A plugin that facilitates managing multiple keyboard languages in hyprland"
  VERSION 0.1
)

#Export compile_commands.json for proper clangd lsp completions
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#Fallback build type
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

#Match the new hyprland c++26 std update
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED true)

#Required flag for compiling shared libraries as plugins for hyprland
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "-fno-gnu-unique")
endif()

#Use pkg-config to check for dependencies instead of 
#the cmake built-in find_package
find_package(PkgConfig REQUIRED)

#pkg_check_modules is a function that PkgConfig provides
#to check for dependencies
pkg_check_modules(deps REQUIRED IMPORTED_TARGET
    hyprland
    libdrm
    libinput
    libudev
    pangocairo
    pixman-1
    wayland-server
    xkbcommon
)

add_library(hyprglot SHARED src/main.cpp src/globals.hpp)

target_link_libraries(hyprglot PRIVATE PkgConfig::deps)

set_target_properties(hyprglot PROPERTIES PREFIX "")

