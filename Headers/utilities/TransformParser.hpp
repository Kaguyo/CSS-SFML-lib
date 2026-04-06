#pragma once
#include "StringUtils.hpp"
#include "LengthResolver.hpp"
#include "../contracts/Types.hpp"
#include <string>

namespace utilities {

// ─────────────────────────────────────────────────────────────────────────────
//  TransformParser
//
//  Parses the value of the CSS `transform` property and applies each
//  transform function to a StyleContext.
//
//  Supported functions:
//    translateX(px | %)    → immediate move or deferred % offset
//    translateY(px | %)    → immediate move or deferred % offset
//    translate(x, y)       → combination of the above
//    rotate(deg)           → setRotation
//    scale(sx [, sy])      → setScale
//    scaleX(sx)
//    scaleY(sy)
//
//  Percentage translates are deferred (stored in StyleContext) because
//  the element's own size may not be final when transform is parsed.
// ─────────────────────────────────────────────────────────────────────────────

struct TransformParser {

    static void apply(contracts::StyleContext& ctx, const std::string& val) {
        std::string s = val;
        size_t pos = 0;

        while (pos < s.size()) {
            size_t open  = s.find('(', pos);
            if (open == std::string::npos) break;
            size_t close = s.find(')', open);
            if (close == std::string::npos) break;

            std::string fn  = StringUtils::toLower(StringUtils::trim(s.substr(pos, open - pos)));
            std::string arg = StringUtils::trim(s.substr(open + 1, close - open - 1));

            applyFunction(ctx, fn, arg);

            pos = close + 1;
        }
    }

private:
    static void applyFunction(
        contracts::StyleContext& ctx,
        const std::string&       fn,
        const std::string&       arg
    ) {
        auto& el = ctx.self;

        if (fn == "translatex") {
            if (!arg.empty() && arg.back() == '%')
                ctx.deferredTranslateXPct = std::stof(arg);
            else
                el->move({ LengthResolver::parseAbsolute(arg), 0.f });
        }
        else if (fn == "translatey") {
            if (!arg.empty() && arg.back() == '%')
                ctx.deferredTranslateYPct = std::stof(arg);
            else
                el->move({ 0.f, LengthResolver::parseAbsolute(arg) });
        }
        else if (fn == "translate") {
            auto parts = StringUtils::tokenize(arg);
            if (parts.size() >= 2) {
                float dx = parts[0].back()=='%'
                    ? el->getSize().x * std::stof(parts[0]) / 100.f
                    : LengthResolver::parseAbsolute(parts[0]);
                float dy = parts[1].back()=='%'
                    ? el->getSize().y * std::stof(parts[1]) / 100.f
                    : LengthResolver::parseAbsolute(parts[1]);
                el->move({ dx, dy });
            }
        }
        else if (fn == "rotate") {
            el->setRotation(std::stof(arg));
        }
        else if (fn == "scale") {
            auto parts = StringUtils::tokenize(arg);
            float sx = std::stof(parts[0]);
            float sy = parts.size() >= 2 ? std::stof(parts[1]) : sx;
            el->setScale({ sx, sy });
        }
        else if (fn == "scalex") {
            el->setScale({ std::stof(arg), el->getScale().y });
        }
        else if (fn == "scaley") {
            el->setScale({ el->getScale().x, std::stof(arg) });
        }
    }
};

} // namespace utilities
