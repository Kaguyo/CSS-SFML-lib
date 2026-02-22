#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <unordered_map>

namespace utilities {

// ─────────────────────────────────────────────────────────────────────────────
//  StringUtils — pure, stateless string operations.
//  No SFML, no CSS logic — just string manipulation primitives.
// ─────────────────────────────────────────────────────────────────────────────

struct StringUtils {

    static std::string trim(const std::string& s) {
        const auto a = s.find_first_not_of(" \t\r\n");
        const auto b = s.find_last_not_of(" \t\r\n");
        return a == std::string::npos ? "" : s.substr(a, b - a + 1);
    }

    static std::string toLower(std::string s) {
        std::transform(s.begin(), s.end(), s.begin(), ::tolower);
        return s;
    }

    // Split by whitespace; strips trailing commas (handles "rgb(r, g, b)" tokens)
    static std::vector<std::string> tokenize(const std::string& s) {
        std::vector<std::string> tokens;
        std::istringstream iss(s);
        std::string tok;
        while (iss >> tok) {
            if (!tok.empty() && tok.back() == ',')
                tok.pop_back();
            tokens.push_back(std::move(tok));
        }
        return tokens;
    }

    // Extract all integer numeric values in order: "rgb(255, 128, 0)" → {255,128,0}
    static std::vector<int> extractIntegers(const std::string& s) {
        std::vector<int> nums;
        std::string cur;
        for (char c : s) {
            if (std::isdigit(static_cast<unsigned char>(c))) {
                cur += c;
            } else if (!cur.empty()) {
                nums.push_back(std::stoi(cur));
                cur.clear();
            }
        }
        if (!cur.empty())
            nums.push_back(std::stoi(cur));
        return nums;
    }

    // Normalise property names: accept both camelCase and kebab-case inputs
    // "backgroundColor" → "background-color"
    // "background-color" → "background-color" (passthrough)
    static std::string normaliseProperty(const std::string& prop) {
        // Lookup table for camel-case aliases
        static const std::unordered_map<std::string, std::string> aliases {
            // background / color
            {"backgroundcolor",     "background-color"},
            {"backgroundcolour",    "background-color"},
            {"fillcolor",           "fill-color"},
            {"bordercolor",         "border-color"},
            {"bordercolour",        "border-color"},
            {"outlinecolor",        "outline-color"},
            {"outlinecolour",       "outline-color"},
            {"borderwidth",         "border-width"},
            {"outlinethickness",    "border-width"},
            {"borderradius",        "border-radius"},
            // font / text
            {"fontsize",            "font-size"},
            {"fontfamily",          "font-family"},
            {"fontstyle",           "font-style"},
            {"textdecoration",      "text-decoration"},
            {"letterspacing",       "letter-spacing"},
            {"linespacing",         "line-spacing"},
            // layout
            {"marginleft",          "margin-left"},
            {"margintop",           "margin-top"},
            {"marginright",         "margin-right"},
            {"marginbottom",        "margin-bottom"},
            {"paddingleft",         "padding-left"},
            {"paddingtop",          "padding-top"},
            {"paddingright",        "padding-right"},
            {"paddingbottom",       "padding-bottom"},
            {"minwidth",            "min-width"},
            {"maxwidth",            "max-width"},
            {"minheight",           "min-height"},
            {"maxheight",           "max-height"},
            // flex
            {"flexdirection",       "flex-direction"},
            {"justifycontent",      "justify-content"},
            {"alignitems",          "align-items"},
            {"rowgap",              "row-gap"},
            {"columngap",           "column-gap"},
            // transform
            {"scalex",              "scale-x"},
            {"scaley",              "scale-y"},
            // position
            {"backgroundimage",     "background-image"},
            {"pointcount",          "point-count"},
        };

        std::string lower = toLower(trim(prop));
        // Strip all hyphens for lookup (so "background-color" also matches)
        std::string noHyphen;
        for (char c : lower)
            if (c != '-') noHyphen += c;

        auto it = aliases.find(noHyphen);
        if (it != aliases.end()) return it->second;

        // Return the lowercase original (already kebab if it had hyphens)
        return lower;
    }
};

} // namespace utilities
