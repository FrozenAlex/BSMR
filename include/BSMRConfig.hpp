#pragma once

#include "config-utils/shared/config-utils.hpp"

DECLARE_CONFIG(BSMRConfig,
    CONFIG_VALUE(PassthroughEnabled, bool, "PassthroughEnabled", false); 
    CONFIG_VALUE(PassthroughOpacity, float, "PassthroughOpacity", 0.1f);   
)