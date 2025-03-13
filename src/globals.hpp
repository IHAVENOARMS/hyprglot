#pragma once
#include <hyprland/src/plugins/PluginAPI.hpp>
#include <xkbcommon/xkbcommon.h>
#include <unordered_map>

inline HANDLE                                              PHANDLE = nullptr;

inline std::unordered_map<std::string, xkb_layout_index_t> globalState;

typedef xkb_layout_index_t                                 KeyboardLayout;
