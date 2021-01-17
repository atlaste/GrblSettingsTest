#include "LegacySettingRegistry.h"

#include "LegacySettingHandler.h"

namespace Configuration 
{
    bool LegacySettingRegistry::isLegacySetting(const char* str)
    {
        return str[0] == '$' && (str[1] >= '0' && str[1] <= '9');
    }

    void LegacySettingRegistry::registerHandler(LegacySettingHandler* handler)
    {
        instance().handlers_.push_back(handler);
    }

    bool LegacySettingRegistry::tryHandleLegacy(const char* str) {
        if (isLegacySetting(str)) {
            auto start = str;

            int value = 0;
            ++str;

            while (*str && *str >= '0' && *str <= '9')
            {
                value = value * 10 + (*str - '0');
                ++str;
            }

            if (*str == '=') {
                ++str;

                handleLegacy(value, str);
            }
            else {
                warn("Incorrect setting '%d': cannot find '='.", start);
            }
            return true;
        }
        else {
            return false;
        }
    }

    void LegacySettingRegistry::handleLegacy(int index, const char* value) {
        for (auto it : instance().handlers_) {
            if (it->index() == index)
            {
                return it->handle(value);
            }
        }

        warn("Cannot find handler for $%d", index);
    }
}