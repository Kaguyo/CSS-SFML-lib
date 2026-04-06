#pragma once
#include "StringUtils.hpp"
#include <SFML/System/Vector2.hpp>
#include <string>
#include <array>
#include <stdexcept>
#include <cstring>

namespace utilities {

// ─────────────────────────────────────────────────────────────────────────────
//  LengthResolver
//
//  Converts CSS length strings into float pixel values.
//
//  Supported units:
//    px   — pixels (passthrough)
//    %    — percentage of `reference` value
//    vw   — percentage of viewport width
//    vh   — percentage of viewport height
//    em   — treated as px (no font context available)
//    rem  — treated as px
//    pt   — treated as px
//    auto — returns 0.f (caller handles "auto" semantics)
//
//  Reference semantics:
//    resolve(val, ref, windowSize)
//      ref        = the containing block dimension (parent width for horizontal,
//                   parent height for vertical)
//      windowSize = for vw/vh resolution
// ─────────────────────────────────────────────────────────────────────────────

struct LengthResolver {

    // Full resolve: handles %, vw, vh, absolute units
    static float resolve(
        const std::string& val,
        float              reference,
        sf::Vector2f       windowSize = {0.f, 0.f}
    ) {
        std::string s = StringUtils::trim(val);
        if (s.empty() || s == "auto") return 0.f;

        // Percentage
        if (!s.empty() && s.back() == '%') {
            float pct = parseNumber(s.substr(0, s.size() - 1));
            return reference * pct / 100.f;
        }

        // Viewport units (need window size)
        if (s.size() >= 2) {
            std::string suffix = StringUtils::toLower(s.substr(s.size() - 2));
            if (suffix == "vw") return windowSize.x * parseNumber(s.substr(0, s.size()-2)) / 100.f;
            if (suffix == "vh") return windowSize.y * parseNumber(s.substr(0, s.size()-2)) / 100.f;
        }

        return parseAbsolute(s);
    }

    // Shorthand: only handle absolute units (no context needed)
    static float parseAbsolute(const std::string& val) {
        std::string s = StringUtils::trim(val);
        // Strip known unit suffixes
        for (const char* unit : {"px","em","rem","pt","dp"}) {
            std::string low = StringUtils::toLower(s);
            size_t pos = low.rfind(unit);
            if (pos != std::string::npos && pos == s.size() - std::strlen(unit)) {
                s = s.substr(0, pos);
                break;
            }
        }
        return parseNumber(s);
    }

    // Parse [top, right, bottom, left] from a CSS shorthand value string.
    // "10px"               → {10, 10, 10, 10}
    // "10px 20px"          → {10, 20, 10, 20}
    // "10px 20px 5px"      → {10, 20,  5, 20}
    // "10px 20px 5px 15px" → {10, 20,  5, 15}
    static std::array<float, 4> parseFourSides(
        const std::string& val,
        float              reference,
        sf::Vector2f       windowSize = {0.f, 0.f}
    ) {
        auto parts = StringUtils::tokenize(val);
        auto r = [&](const std::string& v){ return resolve(v, reference, windowSize); };

        std::array<float, 4> s{0,0,0,0};
        switch (parts.size()) {
            case 0: break;
            case 1: { float v = r(parts[0]); s = {v,v,v,v}; break; }
            case 2: { float v = r(parts[0]), h = r(parts[1]); s = {v,h,v,h}; break; }
            case 3: s = {r(parts[0]), r(parts[1]), r(parts[2]), r(parts[1])}; break;
            default:s = {r(parts[0]), r(parts[1]), r(parts[2]), r(parts[3])}; break;
        }
        return s;
    }

private:
    static float parseNumber(const std::string& s) {
        try {
            return std::stof(StringUtils::trim(s));
        } catch (...) {
            return 0.f;
        }
    }
};

} // namespace utilities
