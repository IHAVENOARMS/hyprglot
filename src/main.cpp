#include "./globals.hpp"
#include <hyprland/src/SharedDefs.hpp>
#include <hyprland/src/plugins/PluginAPI.hpp>

APICALL EXPORT PLUGIN_DESCRIPTION_INFO PLUGIN_INIT(HANDLE handle) {
    PHANDLE     = handle;
    globalState = {};

    const std::string HASH = __hyprland_api_get_hash();
    if (HASH != GIT_COMMIT_HASH) {
        HyprlandAPI::addNotificationV2(PHANDLE,
                                       {{"text", "[hyprglot] Failure in Initialization: Version mismatch (headers version is not equal to running hyprland version)"},
                                        {"time", 5000},
                                        {"color", CHyprColor{1.0, 0.2, 0.2, 1.0}},
                                        {"icon", ICON_ERROR}});
        throw std::runtime_error("[hyprglot] Version mismatch");
    }

    HyprlandAPI::addNotificationV2(PHANDLE, {{"text", "[hyprglot] Hello!"}, {"time", 5000}, {"color", CHyprColor{1.0, 0.2, 0.2, 1.0}}, {"icon", ICON_INFO}});
}
