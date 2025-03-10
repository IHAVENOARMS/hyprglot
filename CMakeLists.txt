cmake_minimum_required(VERSION 3.27)

project(hyprglot
  DESCRIPTION "A plugin that facilitates managing multiple keyboard languages in hyprland"
    VERSION 0.1
)

set(CMAKE_CXX_STANDARD 26)

file(GLOB_RECURSE SRC "*.cpp")

add_library(hyprglot SHARED ${SRC})

find_package(PkgConfig REQUIRED)
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
target_link_libraries(hyprglot PRIVATE rt PkgConfig::deps)

install(TARGETS hyprglot)
