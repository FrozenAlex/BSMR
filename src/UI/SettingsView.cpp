#include "UI/SettingsView.hpp"
#include "assets.hpp"
#include "logging.hpp"
#include "GlobalNamespace/OVRPlugin.hpp"
#include "bsml/shared/BSML.hpp"
#include "bsml/shared/BSML/Components/Backgroundable.hpp"
#include "UnityEngine/Resources.hpp"
#include "UnityEngine/Color.hpp"
#include "sombrero/shared/FastColor.hpp"
#include "GlobalNamespace/OVRPlugin.hpp"
#include "GlobalNamespace/ConditionalActivation.hpp"
#include "GlobalNamespace/ShockwaveEffect.hpp"
#include "GlobalNamespace/MenuShockwave.hpp"
#include "bsml/shared/BSML/MainThreadScheduler.hpp"
#include "GlobalNamespace/OVRPlugin.hpp"
#include "Unity/XR/Oculus/NativeMethods.hpp"
#include "BSMRConfig.hpp"
#include "main.hpp"
#include "TMPro/TextMeshProUGUI.hpp"
#include "GlobalNamespace/OVRManager.hpp"
#include "GlobalNamespace/OVRPassthroughLayer.hpp"
#include "GlobalNamespace/OVROverlay.hpp"
#include "GlobalNamespace/AlwaysVisibleQuad.hpp"
#include "UnityEngine/Camera.hpp"

DEFINE_TYPE(BSMR::UI, SettingsView);

using namespace UnityEngine;
using namespace GlobalNamespace;
using namespace BSMR;

const bool instantlySave = true;

UnityEngine::Sprite* GetBGSprite(std::string str)
{
    return UnityEngine::Resources::FindObjectsOfTypeAll<UnityEngine::Sprite*>()->First([str](UnityEngine::Sprite* x) {
        return x->get_name() == str;
    });
}

void BSMR::UI::SettingsView::DidActivate(bool firstActivation, bool addedToHeirarchy, bool screenSystemDisabling)
{
    if (!firstActivation)
        return;

    BSML::parse_and_construct(Assets::SettingsView_bsml, this->get_transform(), this);

    #ifdef HotReload
        fileWatcher->checkInterval = 0.5f;
        fileWatcher->filePath = "/sdcard/bsml/BSMR/SettingsView.bsml";
    #endif
};

void BSMR::UI::SettingsView::PostParse() {
    
}

void BSMR::UI::SettingsView::UpdateGraphicsSettings() {
    DEBUG("Updating graphics settings");
    auto ovrManager = UnityEngine::GameObject::Find("OVRManager");

    if (!ovrManager) {
        INFO("OVRManager not found");
        return;
    }

    auto ovrManagerComponent = ovrManager->GetComponent<GlobalNamespace::OVRManager*>();

    if (!ovrManagerComponent) {
        INFO("OVRManager component not found");
        return;
    }

    auto mainCamera = UnityEngine::Camera::get_main();
    auto mainCameraGO = mainCamera->get_gameObject();

    auto ovrPassthroughLayer = mainCameraGO->GetComponent<GlobalNamespace::OVRPassthroughLayer*>();
    ovrPassthroughLayer->___overlayType = GlobalNamespace::OVROverlay::OverlayType::Underlay;
    

    ovrPassthroughLayer->___hidden = !getBSMRConfig().PassthroughEnabled.GetValue();
    ovrPassthroughLayer->set_textureOpacity(getBSMRConfig().PassthroughOpacity.GetValue());

    if (getBSMRConfig().PassthroughEnabled.GetValue()) {
        auto alwaysVisibleQuads = UnityEngine::Resources::FindObjectsOfTypeAll<GlobalNamespace::AlwaysVisibleQuad*>();
        for (UnityW<GlobalNamespace::AlwaysVisibleQuad> quad : alwaysVisibleQuads) {
            if (quad) {
                quad->set_enabled(getBSMRConfig().PassthroughEnabled.GetValue());
            }
        }
    }
}


bool BSMR::UI::SettingsView::get_PassthroughEnabled() {
    return getBSMRConfig().PassthroughEnabled.GetValue();
}

void BSMR::UI::SettingsView::set_PassthroughEnabled(bool value) {
    getBSMRConfig().PassthroughEnabled.SetValue(value, instantlySave);
    this->UpdateGraphicsSettings();
}

float BSMR::UI::SettingsView::get_PassthroughOpacity() {
    return getBSMRConfig().PassthroughOpacity.GetValue();
}

void BSMR::UI::SettingsView::set_PassthroughOpacity(float value) {
    getBSMRConfig().PassthroughOpacity.SetValue(value, instantlySave);
    this->UpdateGraphicsSettings();
}
