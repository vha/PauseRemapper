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
        AddConfigValueToggle(t, getMainConfig().aButton);
        AddConfigValueToggle(t, getMainConfig().bButton);
        AddConfigValueToggle(t, getMainConfig().xButton);
        AddConfigValueToggle(t, getMainConfig().yButton);

        // Thumbsticks
        AddConfigValueToggle(t, getMainConfig().leftThumbstick);
        AddConfigValueToggle(t, getMainConfig().rightThumbstick);

        // Triggers and Grips
        AddConfigValueToggle(t, getMainConfig().leftTrigger);
        AddConfigValueToggle(t, getMainConfig().rightTrigger);
        AddConfigValueToggle(t, getMainConfig().leftGrip);
        AddConfigValueToggle(t, getMainConfig().rightGrip);
    }
}
