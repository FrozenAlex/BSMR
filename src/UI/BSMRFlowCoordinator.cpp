#include "UI/BSMRFlowCoordinator.hpp"

#include "bsml/shared/Helpers/creation.hpp"
#include "bsml/shared/Helpers/getters.hpp"
#include "logging.hpp"

#include "GlobalNamespace/MenuTransitionsHelper.hpp"
#include "UnityEngine/Resources.hpp"
#include "bsml/shared/BSML/ViewControllers/HotReloadViewController.hpp"
#include "BSMRConfig.hpp"
#include "bsml/shared/BSML/MainThreadScheduler.hpp"
DEFINE_TYPE(BSMR::UI, BSMRFlowCoordinator);

void BSMR::UI::BSMRFlowCoordinator::Awake() {
    fcInstance = this;
    if (!settingsView) {
        settingsView = BSML::Helpers::CreateViewController<BSMR::UI::SettingsView*>();
    }
    if (!advancedSettingsView) {
        advancedSettingsView = BSML::Helpers::CreateViewController<BSMR::UI::AdvancedSettingsView*>();
    }
    if (!presetsView) {
        presetsView = BSML::Helpers::CreateViewController<BSMR::UI::PresetsView*>();
    }
}

void BSMR::UI::BSMRFlowCoordinator::DidActivate(bool firstActivation, bool addedToHeirarchy, bool screenSystemEnabling) {
    if (!firstActivation) return;

    SetTitle("BSMR", HMUI::ViewController::AnimationType::In);
    this->____showBackButton = true;
    ProvideInitialViewControllers(settingsView, presetsView,  advancedSettingsView, nullptr, nullptr);
}

void BSMR::UI::BSMRFlowCoordinator::BackButtonWasPressed(HMUI::ViewController* topViewController) {
    this->Close();
}

void BSMR::UI::BSMRFlowCoordinator::Close(bool immediately){
    // Do nothing if there's no parent flow coordinator (in multiplayer if you never called it it crashed)
    
    BSML::MainThreadScheduler::ScheduleAfterTime(1.0f,[this]() {
        INFO("Saving BSMR config...");
        getBSMRConfig().Save();
    });

    if (fcInstance && fcInstance->get_isActiveAndEnabled() && fcInstance->get_isActivated()) {
        this->____parentFlowCoordinator->DismissFlowCoordinator(this, HMUI::ViewController::AnimationDirection::Horizontal, nullptr, immediately);
    }
};