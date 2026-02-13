#include "PauseRemapperController.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/Time.hpp"
#include "UnityEngine/Resources.hpp"
#include "UnityEngine/AudioSource.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/UI/Button.hpp"
#include "MainConfig.hpp"
#include "main.hpp"
#include "GlobalNamespace/OVRInput.hpp"
#include "System/Action.hpp"
#include "lapiz/shared/utilities/MainThreadScheduler.hpp"

DEFINE_TYPE(PauseRemapper, PauseRemapperController);

using namespace UnityEngine;
using namespace System::Threading::Tasks;
using namespace UnityEngine::UI;

namespace PauseRemapper
{
    void PauseRemapperController::ctor(PauseController *pauseController, PauseMenuManager *pauseMenuManager)
    {
        INVOKE_CTOR();
        _pauseController = pauseController;
        _pauseMenuManager = pauseMenuManager;
        PaperLogger.info("Constructed PauseRemapper Controller");
    }

    void PauseRemapperController::Initialize()
    {
        PaperLogger.info("Initialised PauseRemapper Controller");
    }

    void PauseRemapperController::Dispose()
    {
        PaperLogger.info("Disposed PauseRemapper Controller");
    }

    void PauseRemapperController::CachePauseMenuButtons()
    {
        // Find buttons by name instead of accessing private fields
        if (!_pauseMenuManager) return;
        
        auto transform = _pauseMenuManager->get_transform();
        if (!transform) return;

        // Try to find buttons by searching the hierarchy
        auto FindButton = [&](std::string name) -> Button* {
            auto child = transform->Find(name);
            if (child) {
                return child->GetComponent<Button*>();
            }
            return nullptr;
        };

        // Common button names in Beat Saber pause menu
        _backButton = FindButton("BackButton");
        _restartButton = FindButton("RestartButton");
        _continueButton = FindButton("ContinueButton");

        // Try alternative names if not found
        if (!_backButton) _backButton = FindButton("Back");
        if (!_restartButton) _restartButton = FindButton("Restart");
        if (!_continueButton) _continueButton = FindButton("Continue");
    }
    
    void PauseRemapperController::PauseTrigger()
    {
        PaperLogger.info("A Selected Pause Button Was Clicked/Pressed");
        timeHeld += UnityEngine::Time::get_deltaTime();
        
        // Use public methods instead of private field access
        if (_pauseController && _pauseController->___m_CachedPtr.m_value && _pauseController->_paused && timeHeld >= 1 && timeHeld < 1.1)
        {
            if (_pauseMenuManager) {
                _pauseMenuManager->ContinueButtonPressed();
            }
        }
        else if (_pauseController && _pauseController->___m_CachedPtr.m_value && _pauseController->get_canPause() && timeHeld < 0.6)
        {
            // Cache buttons before pausing
            CachePauseMenuButtons();
            
            _pauseController->Pause();
            PaperLogger.info("Successfully Paused!");
            
            // Disable buttons using cached references
            if (_backButton) _backButton->set_interactable(false);
            if (_restartButton) _restartButton->set_interactable(false);
            if (_continueButton) _continueButton->set_interactable(false);

            std::thread([=]()
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(300));
                Lapiz::Utilities::MainThreadScheduler::Schedule([=](){
                    if (_backButton) _backButton->set_interactable(true);
                    if (_restartButton) _restartButton->set_interactable(true);
                    if (_continueButton) _continueButton->set_interactable(true);
                });
            }).detach();
        }
        return;
    }

    void PauseRemapperController::Tick()
    {
        // Check each enabled button individually
        if (getMainConfig().aButton.GetValue() && GlobalNamespace::OVRInput::Get(GlobalNamespace::OVRInput::Button::One, GlobalNamespace::OVRInput::Controller::Touch))
            buttonPressed = true;
        else if (getMainConfig().bButton.GetValue() && GlobalNamespace::OVRInput::Get(GlobalNamespace::OVRInput::Button::Two, GlobalNamespace::OVRInput::Controller::Touch))
            buttonPressed = true;
        else if (getMainConfig().xButton.GetValue() && GlobalNamespace::OVRInput::Get(GlobalNamespace::OVRInput::Button::Three, GlobalNamespace::OVRInput::Controller::Touch))
            buttonPressed = true;
        else if (getMainConfig().yButton.GetValue() && GlobalNamespace::OVRInput::Get(GlobalNamespace::OVRInput::Button::Four, GlobalNamespace::OVRInput::Controller::Touch))
            buttonPressed = true;
        else if (getMainConfig().leftTrigger.GetValue() && GlobalNamespace::OVRInput::Get(GlobalNamespace::OVRInput::Button::PrimaryIndexTrigger, GlobalNamespace::OVRInput::Controller::Touch))
            buttonPressed = true;
        else if (getMainConfig().rightTrigger.GetValue() && GlobalNamespace::OVRInput::Get(GlobalNamespace::OVRInput::Button::SecondaryIndexTrigger, GlobalNamespace::OVRInput::Controller::Touch))
            buttonPressed = true;
        else if (getMainConfig().leftGrip.GetValue() && GlobalNamespace::OVRInput::Get(GlobalNamespace::OVRInput::Button::PrimaryHandTrigger, GlobalNamespace::OVRInput::Controller::Touch))
            buttonPressed = true;
        else if (getMainConfig().rightGrip.GetValue() && GlobalNamespace::OVRInput::Get(GlobalNamespace::OVRInput::Button::SecondaryHandTrigger, GlobalNamespace::OVRInput::Controller::Touch))
            buttonPressed = true;
        else if (getMainConfig().leftThumbstick.GetValue() && GlobalNamespace::OVRInput::Get(GlobalNamespace::OVRInput::Button::SecondaryThumbstick, GlobalNamespace::OVRInput::Controller::Touch))
            buttonPressed = true;
        else if (getMainConfig().rightThumbstick.GetValue() && GlobalNamespace::OVRInput::Get(GlobalNamespace::OVRInput::Button::PrimaryThumbstick, GlobalNamespace::OVRInput::Controller::Touch))
            buttonPressed = true;

        if (buttonPressed) PauseTrigger();
        else timeHeld = 0;
        buttonPressed = false;
    }
}
