#pragma once

#include <string>
#include <cstdint>

bool match(uint8_t* addr, uint8_t *pattern, const std::string& mask) {
    for (uint32_t i = 0; mask[i]; i++) {
        if (mask[i] == 'x' && addr[i] != pattern[i])
            return false;
    }
    return true;
}
