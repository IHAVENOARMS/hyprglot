#include "./globals.hpp"
#include <hyprland/src/desktop/Window.hpp>
#include <hyprland/src/SharedDefs.hpp>
#include <hyprland/src/plugins/PluginAPI.hpp>
#include <hyprland/src/devices/Keyboard.hpp>
#include <optional>
#include <hyprland/src/Compositor.hpp>
#include <hyprland/src/managers/input/InputManager.hpp>

const KeyboardLayout getKeyboardLayoutCount(const SP<IKeyboard> PKEYBOARD) {
    const auto LAYOUTCOUNT = xkb_keymap_num_layouts(PKEYBOARD->xkbKeymap);
    return LAYOUTCOUNT;
}

const KeyboardLayout getActiveKeyboardLayout(const SP<IKeyboard> PKEYBOARD) {
    const auto     LAYOUTCOUNT  = getKeyboardLayoutCount(PKEYBOARD);
    KeyboardLayout activeLayout = 0;
    while (activeLayout < LAYOUTCOUNT) {
        if (xkb_state_layout_index_is_active(PKEYBOARD->xkbState, activeLayout, XKB_STATE_LAYOUT_EFFECTIVE) == 1)
            break;

        activeLayout++;
    }

    return activeLayout;
}

std::optional<PHLWINDOW> getActiveWindow() {
    if (!g_pCompositor->m_pLastWindow)
        return std::nullopt;
    return g_pCompositor->m_pLastWindow.lock();
}

const std::optional<SP<IKeyboard>> getActiveKeyboard() {
    for (auto const& PKEYBOARD : g_pInputManager->m_vKeyboards) {
        if (!PKEYBOARD->active)
            continue;

        return PKEYBOARD;
    }
    return std::nullopt;
}

void switchKeyboardLayout(const SP<IKeyboard> PKEYBOARD, KeyboardLayout layout) {
    const auto LAYOUTCOUNT = getKeyboardLayoutCount(PKEYBOARD);

    PKEYBOARD->updateModifiers(PKEYBOARD->modifiersState.depressed, PKEYBOARD->modifiersState.latched, PKEYBOARD->modifiersState.locked,
                               layout > LAYOUTCOUNT || layout < 0 ? 0 : layout);
}

void switchActiveKeyboardLayout(KeyboardLayout layout) {
    const auto ACTIVEKEYBOARD = getActiveKeyboard();
    if (const auto PACTIVEKEYBOARD = ACTIVEKEYBOARD.value()) {
        switchKeyboardLayout(PACTIVEKEYBOARD, layout);
    }
}

void onActiveWindow(PHLWINDOW pWindow) {
    if (globalState.contains(pWindow->m_szTitle))
        switchActiveKeyboardLayout(globalState[pWindow->m_szTitle]);
}

void onActiveLayout(std::vector<std::any> args) {
    const SP<IKeyboard> PACTIVEKEYBOARD = std::any_cast<SP<IKeyboard>>(args[0]);
    const auto          ACTIVEWINDOW    = getActiveWindow();
    if (!ACTIVEWINDOW.has_value()) {
        return;
    }
    const auto PACTIVEWINDOW              = ACTIVEWINDOW.value();
    const auto ACTIVELAYOUT               = getActiveKeyboardLayout(PACTIVEKEYBOARD);
    globalState[PACTIVEWINDOW->m_szTitle] = ACTIVELAYOUT;
}

static SDispatchResult switchLayoutDispatcher(std::string args) {
    const auto ACTIVEKEYBOARD = getActiveKeyboard();
    if (!ACTIVEKEYBOARD.has_value())
        return SDispatchResult{.success = false, .error = "Failed to find active keyboard"};

    const auto PACTIVEKEYBOARD = ACTIVEKEYBOARD.value();

    const auto ACTIVELAYOUT = getActiveKeyboardLayout(PACTIVEKEYBOARD);
    const auto LAYOUTCOUNT  = getKeyboardLayoutCount(PACTIVEKEYBOARD);
    const auto NEWLAYOUT    = ACTIVELAYOUT >= LAYOUTCOUNT - 1 ? 0 : ACTIVELAYOUT + 1;
    switchKeyboardLayout(PACTIVEKEYBOARD, NEWLAYOUT);
    return SDispatchResult{};
}

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

    static auto p =
        HyprlandAPI::registerCallbackDynamic(PHANDLE, "activeWindow", [&](void* self, SCallbackInfo& info, std::any data) { onActiveWindow(std::any_cast<PHLWINDOW>(data)); });
    static auto p2 = HyprlandAPI::registerCallbackDynamic(PHANDLE, "activeLayout",
                                                          [&](void* self, SCallbackInfo& info, std::any data) { onActiveLayout(std::any_cast<std::vector<std::any>>(data)); });

    auto        success = HyprlandAPI::addDispatcherV2(PHANDLE, "plugin:hyprglot:switchxkblayout", switchLayoutDispatcher);

    HyprlandAPI::addNotification(PHANDLE, "[hyprglot] Initialized successfully!", CHyprColor{0.2, 1.0, 0.2, 1.0}, 5000);

    return {"hyprglot", "A plugin that facilitates managing multiple keyboard layouts in hyprland", "Jimmy (Yousef)", "1.0"};
}
