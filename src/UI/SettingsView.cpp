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
#include "bsml/shared/BSed/BSML-Lite/Creation/Text.hpp"
#include "TMPro/TextMeshProUGUI.hpp"


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

    
}


bool BSMR::UI::SettingsView::get_PassthroughEnabled() {
    return getBSMRConfig().PassthroughEnabled.GetValue();
}

void BSMR::UI::SettingsView::set_PassthroughEnabled(bool value) {
    getBSMRConfig().PassthroughEnabled.SetValue(value, instantlySave);
}
