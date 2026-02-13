#include "main.hpp"
#include "UI/UIManager.hpp"
#include "MainConfig.hpp"
#include "bsml/shared/BSML.hpp"

DEFINE_TYPE(PauseRemapper::UI, UIManager);

using namespace GlobalNamespace;
using namespace UnityEngine;

namespace PauseRemapper::UI
{
    void UIManager::ctor() {}

    void UIManager::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
    {
        if (!firstActivation) return;

        auto t = get_transform();

        // ABXY Buttons
        BSML::Lite::CreateToggle(t, "A Button", getMainConfig().aButton.GetValue(), 
            [](bool value) { getMainConfig().aButton.SetValue(value); });
        BSML::Lite::CreateToggle(t, "B Button", getMainConfig().bButton.GetValue(), 
            [](bool value) { getMainConfig().bButton.SetValue(value); });
        BSML::Lite::CreateToggle(t, "X Button", getMainConfig().xButton.GetValue(), 
            [](bool value) { getMainConfig().xButton.SetValue(value); });
        BSML::Lite::CreateToggle(t, "Y Button", getMainConfig().yButton.GetValue(), 
            [](bool value) { getMainConfig().yButton.SetValue(value); });

        // Thumbsticks
        BSML::Lite::CreateToggle(t, "Left Thumbstick", getMainConfig().leftThumbstick.GetValue(), 
            [](bool value) { getMainConfig().leftThumbstick.SetValue(value); });
        BSML::Lite::CreateToggle(t, "Right Thumbstick", getMainConfig().rightThumbstick.GetValue(), 
            [](bool value) { getMainConfig().rightThumbstick.SetValue(value); });

        // Triggers and Grips
        BSML::Lite::CreateToggle(t, "Left Trigger", getMainConfig().leftTrigger.GetValue(), 
            [](bool value) { getMainConfig().leftTrigger.SetValue(value); });
        BSML::Lite::CreateToggle(t, "Right Trigger", getMainConfig().rightTrigger.GetValue(), 
            [](bool value) { getMainConfig().rightTrigger.SetValue(value); });
        BSML::Lite::CreateToggle(t, "Left Grip", getMainConfig().leftGrip.GetValue(), 
            [](bool value) { getMainConfig().leftGrip.SetValue(value); });
        BSML::Lite::CreateToggle(t, "Right Grip", getMainConfig().rightGrip.GetValue(), 
            [](bool value) { getMainConfig().rightGrip.SetValue(value); });
    }
}
