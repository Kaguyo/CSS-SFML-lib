#pragma once
#include "StringUtils.hpp"
#include <SFML/Graphics/Color.hpp>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <cstdint>

namespace utilities {

// ─────────────────────────────────────────────────────────────────────────────
//  ColorParser
//
//  Parses CSS color strings into sf::Color.
//  Supported formats:
//    • #rgb           shorthand hex (expands to #rrggbb)
//    • #rrggbb        hex without alpha (alpha = 255)
//    • #rrggbbaa      hex with alpha
//    • rgb(r, g, b)   integer channels 0–255
//    • rgba(r,g,b,a)  integer channels 0–255
//    • <named>        see namedColors() below
// ─────────────────────────────────────────────────────────────────────────────

struct ColorParser {

    static sf::Color parse(const std::string& raw) {
        std::string s = StringUtils::trim(raw);

        if (!s.empty() && s[0] == '#')
            return fromHex(s.substr(1));

        std::string lower = StringUtils::toLower(s);

        if (lower.rfind("rgba", 0) == 0 || lower.rfind("rgb", 0) == 0)
            return fromRGB(s);

        return fromNamed(lower);
    }

private:
    static uint8_t clamp8(int v) {
        return static_cast<uint8_t>(std::clamp(v, 0, 255));
    }

    // ── Hex ───────────────────────────────────────────────────────────────
    static sf::Color fromHex(std::string hex) {
        // Shorthand: #rgb → #rrggbb
        if (hex.size() == 3) {
            hex = { hex[0],hex[0], hex[1],hex[1], hex[2],hex[2] };
        }
        // Shorthand: #rgba → #rrggbbaa
        if (hex.size() == 4) {
            hex = { hex[0],hex[0], hex[1],hex[1], hex[2],hex[2], hex[3],hex[3] };
        }
        if (hex.size() == 6) hex += "ff";
        if (hex.size() == 8) {
            try {
                unsigned long val = std::stoul(hex, nullptr, 16);
                return sf::Color(
                    static_cast<uint8_t>((val >> 24) & 0xFF),
                    static_cast<uint8_t>((val >> 16) & 0xFF),
                    static_cast<uint8_t>((val >>  8) & 0xFF),
                    static_cast<uint8_t>( val        & 0xFF)
                );
            } catch (...) {}
        }
        return sf::Color::White; // fallback
    }

    // ── rgb() / rgba() ────────────────────────────────────────────────────
    static sf::Color fromRGB(const std::string& s) {
        auto nums = StringUtils::extractIntegers(s);
        return sf::Color(
            nums.size() > 0 ? clamp8(nums[0]) : 0,
            nums.size() > 1 ? clamp8(nums[1]) : 0,
            nums.size() > 2 ? clamp8(nums[2]) : 0,
            nums.size() > 3 ? clamp8(nums[3]) : 255
        );
    }

    // ── Named colors ──────────────────────────────────────────────────────
    static sf::Color fromNamed(const std::string& name) {
        static const std::unordered_map<std::string, sf::Color> table {
            // CSS standard
            {"transparent",  sf::Color::Transparent},
            {"black",        sf::Color::Black},
            {"white",        sf::Color::White},
            {"red",          sf::Color::Red},
            {"green",        sf::Color::Green},
            {"blue",         sf::Color::Blue},
            {"yellow",       sf::Color::Yellow},
            {"magenta",      sf::Color::Magenta},
            {"cyan",         sf::Color::Cyan},
            // Extended
            {"gray",         {128, 128, 128}},
            {"grey",         {128, 128, 128}},
            {"darkgray",     { 64,  64,  64}},
            {"lightgray",    {211, 211, 211}},
            {"orange",       {255, 165,   0}},
            {"darkorange",   {255, 140,   0}},
            {"pink",         {255, 192, 203}},
            {"hotpink",      {255, 105, 180}},
            {"purple",       {128,   0, 128}},
            {"violet",       {238, 130, 238}},
            {"indigo",       { 75,   0, 130}},
            {"brown",        {165,  42,  42}},
            {"lime",         { 50, 205,  50}},
            {"navy",         {  0,   0, 128}},
            {"teal",         {  0, 128, 128}},
            {"silver",       {192, 192, 192}},
            {"gold",         {255, 215,   0}},
            {"coral",        {255, 127,  80}},
            {"salmon",       {250, 128, 114}},
            {"crimson",      {220,  20,  60}},
            {"turquoise",    { 64, 224, 208}},
            {"skyblue",      {135, 206, 235}},
            {"steelblue",    { 70, 130, 180}},
            {"chocolate",    {210, 105,  30}},
            {"tomato",       {255,  99,  71}},
            {"orchid",       {218, 112, 214}},
            {"plum",         {221, 160, 221}},
            {"khaki",        {240, 230, 140}},
            {"beige",        {245, 245, 220}},
            {"ivory",        {255, 255, 240}},
            {"lavender",     {230, 230, 250}},
            {"linen",        {250, 240, 230}},
            {"mintcream",    {245, 255, 250}},
            {"snow",         {255, 250, 250}},
            {"wheat",        {245, 222, 179}},
        };

        auto it = table.find(name);
        return (it != table.end()) ? it->second : sf::Color::White;
    }
};

} // namespace utilities
