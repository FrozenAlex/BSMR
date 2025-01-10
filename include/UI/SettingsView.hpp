    #pragma once

#include "bsml/shared/macros.hpp"
#include "bsml/shared/BSML.hpp"
#include "bsml/shared/BSML/Components/Settings/SliderSetting.hpp"
#include "bsml/shared/BSML/Components/Settings/DropdownListSetting.hpp"
#include "bsml/shared/BSML/Components/Settings/ListSetting.hpp"
#include "bsml/shared/BSML/Components/Settings/StringSetting.hpp"
#include "bsml/shared/BSML/ViewControllers/HotReloadViewController.hpp"
#include "bsml/shared/BSML/Components/ClickableText.hpp"
#include "UnityEngine/UI/HorizontalLayoutGroup.hpp"
#include "HMUI/ViewController.hpp"
#include "custom-types/shared/coroutine.hpp"
#include "custom-types/shared/macros.hpp"




#ifdef HotReload
DECLARE_CLASS_CUSTOM(BSMR::UI, SettingsView, BSML::HotReloadViewController,
#else
DECLARE_CLASS_CODEGEN(BSMR::UI, SettingsView, HMUI::ViewController,
#endif

    DECLARE_SIMPLE_DTOR();
    DECLARE_OVERRIDE_METHOD_MATCH(void, DidActivate, &HMUI::ViewController::DidActivate, bool firstActivation, bool addedToHeirarchy, bool screenSystemDisabling);

    DECLARE_INSTANCE_METHOD(void, PostParse);
    DECLARE_INSTANCE_METHOD(void, UpdateGraphicsSettings);

    // Instant settings
    DECLARE_BSML_PROPERTY(bool, PassthroughEnabled);
    DECLARE_BSML_PROPERTY(float, PassthroughOpacity);
)