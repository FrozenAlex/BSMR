#include "UI/PresetsView.hpp"
#include "assets.hpp"
#include "logging.hpp"


DEFINE_TYPE(BSMR::UI, PresetsView);

void BSMR::UI::PresetsView::DidActivate(bool firstActivation, bool addedToHeirarchy, bool screenSystemDisabling)
{
    if (!firstActivation)
        return;

    INFO("PresetsView activated");

   
    BSML::parse_and_construct(Assets::PresetsView_bsml, this->get_transform(), this);

    #ifdef HotReload
        fileWatcher->filePath = "/sdcard/bsml/BSMR/PresetsView.bsml";
    #endif
};