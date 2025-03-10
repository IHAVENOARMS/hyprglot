#include "./globals.hpp"
#include <hyprland/src/desktop/Window.hpp>
#include <hyprland/src/SharedDefs.hpp>
#include <hyprland/src/plugins/PluginAPI.hpp>

APICALL EXPORT std::string PLUGIN_API_VERSION() {
    return HYPRLAND_API_VERSION;
}

APICALL EXPORT PLUGIN_DESCRIPTION_INFO PLUGIN_INIT(HANDLE handle) {
    PHANDLE     = handle;
    globalState = {};

    const std::string HASH = __hyprland_api_get_hash();
    if (HASH != GIT_COMMIT_HASH) {
        HyprlandAPI::addNotification(PHANDLE, "[hyprglot] Failure in Initialization: Version mismatch (headers version is not equal to running hyprland version)",
                                     CHyprColor{1.0, 0.2, 0.2, 1.0}, 5000);

        throw std::runtime_error("[hyprglot] Version mismatch");
    }

    HyprlandAPI::addNotification(PHANDLE, "[hyprglot] Initialized successfully!", CHyprColor{0.2, 1.0, 0.2, 1.0}, 5000);

    return {"hyprglot", "A plugin that facilitates managing multiple keyboard languages in hyprland", "Jimmy (Yousef)", "1.0"};
}
