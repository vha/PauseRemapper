#include "main.hpp"

#include "bsml/shared/BSML.hpp"
#include "MainConfig.hpp"

#include "lapiz/shared/zenject/Zenjector.hpp"
#include "Installers/PauseRemapperInstaller.hpp"

using namespace GlobalNamespace;
using namespace UnityEngine;

static modloader::ModInfo modInfo{MOD_ID, VERSION, 0};

MOD_EXTERN_FUNC void setup(CModInfo* info) noexcept
{
    *info = modInfo.to_c();
    PaperLogger.info("Completed setup!");
}

MOD_EXTERN_FUNC void late_load() noexcept
{
    il2cpp_functions::Init();
    custom_types::Register::AutoRegister();

    getMainConfig().Init(modInfo);

    BSML::Init();
    BSML::Register::RegisterGameplaySetupTab("Pause Remapper", [](UnityEngine::GameObject* go, bool firstActivation) {
        if (!firstActivation) return;
        auto container = BSML::Lite::CreateScrollableSettingsContainer(go->get_transform());
        auto t = container->get_transform();

        AddConfigValueToggle(t, getMainConfig().aButton);
        AddConfigValueToggle(t, getMainConfig().bButton);
        AddConfigValueToggle(t, getMainConfig().xButton);
        AddConfigValueToggle(t, getMainConfig().yButton);
        AddConfigValueToggle(t, getMainConfig().leftThumbstick);
        AddConfigValueToggle(t, getMainConfig().rightThumbstick);
        AddConfigValueToggle(t, getMainConfig().leftTrigger);
        AddConfigValueToggle(t, getMainConfig().rightTrigger);
        AddConfigValueToggle(t, getMainConfig().leftGrip);
        AddConfigValueToggle(t, getMainConfig().rightGrip);
    }, BSML::MenuType::All);

    auto zenjector = Lapiz::Zenject::Zenjector::Get();
    zenjector->Install<PauseRemapper::Installers::PauseRemapperInstaller*, GlobalNamespace::StandardGameplayInstaller*>();
}
