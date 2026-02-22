#pragma once
#include "../contracts/Types.hpp"
#include "../utilities/StringUtils.hpp"
#include <vector>
#include <string>

namespace core {

// ─────────────────────────────────────────────────────────────────────────────
//  RuleParser
//
//  Converts a raw list of CSS-like rule strings into a normalised
//  Declaration list.
//
//  Input:  { "width: 190px", "background-color: #1e1e2e", "opacity: 0.9" }
//  Output: [ {property:"width",          value:"190px"},
//            {property:"background-color",value:"#1e1e2e"},
//            {property:"opacity",         value:"0.9"} ]
//
//  Normalisation applied:
//    • leading/trailing whitespace stripped from property and value
//    • property name lowercased and camelCase aliases resolved to kebab-case
//    • lines without ':' are silently skipped
// ─────────────────────────────────────────────────────────────────────────────

struct RuleParser {

    static std::vector<contracts::Declaration>
    parse(const std::vector<std::string>& rules) {
        std::vector<contracts::Declaration> out;
        out.reserve(rules.size());

        for (const auto& rule : rules) {
            auto colon = rule.find(':');
            if (colon == std::string::npos) continue;

            contracts::Declaration d;
            d.property = utilities::StringUtils::normaliseProperty(
                             rule.substr(0, colon));
            d.value    = utilities::StringUtils::trim(
                             rule.substr(colon + 1));

            if (!d.property.empty())
                out.push_back(std::move(d));
        }

        return out;
    }
};

} // namespace core
