#include "UI/AdvancedSettingsView.hpp"
#include "assets.hpp"
#include "logging.hpp"

DEFINE_TYPE(BSMR::UI, AdvancedSettingsView);

void BSMR::UI::AdvancedSettingsView::DidActivate(bool firstActivation, bool addedToHeirarchy, bool screenSystemDisabling)
{
    if (!firstActivation)
        return;

    INFO("AdvancedSettingsView activated");

    BSML::parse_and_construct(Assets::AdvancedSettingsView_bsml, this->get_transform(), this);
    #ifdef HotReload
        fileWatcher->filePath = "/sdcard/bsml/BSMR/AdvancedSettingsView.bsml";
    #endif
};