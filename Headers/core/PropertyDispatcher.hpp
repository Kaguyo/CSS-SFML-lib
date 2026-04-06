#pragma once
#include "../contracts/Types.hpp"
#include "../utilities/ColorParser.hpp"
#include "../utilities/LengthResolver.hpp"
#include "../utilities/TransformParser.hpp"
#include <string>
#include <vector>

namespace core {

// ─────────────────────────────────────────────────────────────────────────────
//  PropertyDispatcher
//
//  Applies a list of parsed declarations to a StyleContext in two passes:
//
//  Pass 1 — Intrinsic properties (size, color, font, box model, layout intent)
//           These can be resolved purely from the declaration value and the
//           containing block — no knowledge of other declarations needed.
//
//  Pass 2 — Positional properties (left, right, top, bottom, position: center)
//           These depend on the element's final size (which pass 1 resolves),
//           so they must run after pass 1 is complete.
//
//  After both passes, deferred transform offsets (% translateX/Y) are flushed.
// ─────────────────────────────────────────────────────────────────────────────

struct PropertyDispatcher {

    static void apply(
        contracts::StyleContext&                      ctx,
        const std::vector<contracts::Declaration>&    decls
    ) {
        for (const auto& d : decls) pass1(ctx, d.property, d.value);
        for (const auto& d : decls) pass2(ctx, d.property, d.value);
        flushDeferredTransforms(ctx);
    }

private:
    using LR  = utilities::LengthResolver;
    using CP  = utilities::ColorParser;
    using SU  = utilities::StringUtils;

    // ── Helpers ───────────────────────────────────────────────────────────

    static float resolveH(const std::string& v, const contracts::StyleContext& ctx) {
        return LR::resolve(v, ctx.parentSize.x, ctx.windowSize);
    }
    static float resolveV(const std::string& v, const contracts::StyleContext& ctx) {
        return LR::resolve(v, ctx.parentSize.y, ctx.windowSize);
    }

    // ─────────────────────────────────────────────────────────────────────
    //  PASS 1 — intrinsic properties
    // ─────────────────────────────────────────────────────────────────────

    static void pass1(
        contracts::StyleContext& ctx,
        const std::string&       prop,
        const std::string&       val
    ) {
        auto& el = ctx.self;

        // ── Sizing ────────────────────────────────────────────────────────
        if (prop == "width") {
            float w = resolveH(val, ctx);
            el->setSize({ w, el->getSize().y });
        }
        else if (prop == "height") {
            float h = resolveV(val, ctx);
            el->setSize({ el->getSize().x, h });
        }
        else if (prop == "size") {
            auto parts = SU::tokenize(val);
            if (parts.size() >= 2)
                el->setSize({ resolveH(parts[0], ctx), resolveV(parts[1], ctx) });
            else if (parts.size() == 1) {
                float v = LR::resolve(parts[0],
                              std::min(ctx.parentSize.x, ctx.parentSize.y),
                              ctx.windowSize);
                el->setSize({ v, v });
            }
        }
        else if (prop == "min-width") {
            float w = resolveH(val, ctx);
            if (el->getSize().x < w) el->setSize({ w, el->getSize().y });
        }
        else if (prop == "max-width") {
            float w = resolveH(val, ctx);
            if (el->getSize().x > w) el->setSize({ w, el->getSize().y });
        }
        else if (prop == "min-height") {
            float h = resolveV(val, ctx);
            if (el->getSize().y < h) el->setSize({ el->getSize().x, h });
        }
        else if (prop == "max-height") {
            float h = resolveV(val, ctx);
            if (el->getSize().y > h) el->setSize({ el->getSize().x, h });
        }
        else if (prop == "radius") {
            float r = LR::resolve(val,
                          std::min(ctx.parentSize.x, ctx.parentSize.y),
                          ctx.windowSize);
            el->setSize({ r * 2.f, r * 2.f });
        }

        // ── Colors ────────────────────────────────────────────────────────
        else if (prop == "background-color" || prop == "fill" || prop == "fill-color") {
            el->setFillColor(CP::parse(val));
        }
        else if (prop == "color") {
            // Text → fill color; shapes → outline color
            if (el->isText()) el->setFillColor(CP::parse(val));
            else              el->setOutlineColor(CP::parse(val));
        }
        else if (prop == "border-color" || prop == "outline-color") {
            el->setOutlineColor(CP::parse(val));
        }
        else if (prop == "border-width") {
            el->setOutlineThickness(LR::parseAbsolute(val));
        }
        else if (prop == "opacity") {
            float alpha = std::stof(val);
            if (alpha <= 1.f) alpha *= 255.f;
            auto c = el->getFillColor();
            c.a = static_cast<uint8_t>(std::clamp(static_cast<int>(alpha), 0, 255));
            el->setFillColor(c);
        }
        else if (prop == "tint") {
            // Sprite tinting alias
            el->setFillColor(CP::parse(val));
        }

        // ── Text properties ───────────────────────────────────────────────
        else if (prop == "font-size") {
            unsigned sz = static_cast<unsigned>(
                LR::resolve(val, ctx.parentSize.y, ctx.windowSize));
            el->setCharacterSize(sz);
        }
        else if (prop == "letter-spacing") {
            el->setLetterSpacing(std::stof(val));
        }
        else if (prop == "line-spacing") {
            el->setLineSpacing(std::stof(val));
        }
        else if (prop == "font-style" || prop == "text-decoration") {
            el->setTextStyle(parseTextStyle(val));
        }

        // ── Transform ─────────────────────────────────────────────────────
        else if (prop == "transform") {
            utilities::TransformParser::apply(ctx, val);
        }
        else if (prop == "rotation") {
            el->setRotation(std::stof(val));
        }
        else if (prop == "scale") {
            auto parts = SU::tokenize(val);
            if (parts.size() >= 2)
                el->setScale({ std::stof(parts[0]), std::stof(parts[1]) });
            else
                el->setScale({ std::stof(val), std::stof(val) });
        }
        else if (prop == "scale-x") {
            el->setScale({ std::stof(val), el->getScale().y });
        }
        else if (prop == "scale-y") {
            el->setScale({ el->getScale().x, std::stof(val) });
        }
        else if (prop == "origin") {
            auto parts = SU::tokenize(val);
            if (parts.size() >= 2)
                el->setOrigin({ LR::parseAbsolute(parts[0]),
                                 LR::parseAbsolute(parts[1]) });
        }

        // ── Box model ─────────────────────────────────────────────────────
        else if (prop == "padding") {
            auto s = LR::parseFourSides(val, ctx.parentSize.x, ctx.windowSize);
            ctx.box = { s[0], s[1], s[2], s[3],
                        ctx.box.marginTop, ctx.box.marginRight,
                        ctx.box.marginBottom, ctx.box.marginLeft };
        }
        else if (prop == "padding-top")    ctx.box.paddingTop    = resolveV(val,ctx);
        else if (prop == "padding-right")  ctx.box.paddingRight  = resolveH(val,ctx);
        else if (prop == "padding-bottom") ctx.box.paddingBottom = resolveV(val,ctx);
        else if (prop == "padding-left")   ctx.box.paddingLeft   = resolveH(val,ctx);
        else if (prop == "margin") {
            auto s = LR::parseFourSides(val, ctx.parentSize.x, ctx.windowSize);
            ctx.box.marginTop    = s[0];
            ctx.box.marginRight  = s[1];
            ctx.box.marginBottom = s[2];
            ctx.box.marginLeft   = s[3];
        }
        else if (prop == "margin-top")    ctx.box.marginTop    = resolveV(val,ctx);
        else if (prop == "margin-right")  ctx.box.marginRight  = resolveH(val,ctx);
        else if (prop == "margin-bottom") ctx.box.marginBottom = resolveV(val,ctx);
        else if (prop == "margin-left")   ctx.box.marginLeft   = resolveH(val,ctx);

        // ── Flex / layout intent ──────────────────────────────────────────
        else if (prop == "display") {
            ctx.flex.enabled = (val == "flex" || val == "grid");
        }
        else if (prop == "flex-direction") {
            ctx.flex.column = (val == "column" || val == "column-reverse");
        }
        else if (prop == "gap" || prop == "row-gap" || prop == "column-gap") {
            ctx.flex.gap = resolveH(val, ctx);
        }
        else if (prop == "justify-content") {
            ctx.flex.justify = parseJustify(val);
        }
        else if (prop == "align-items") {
            ctx.flex.align = parseAlign(val);
        }

        // ── Position mode ─────────────────────────────────────────────────
        else if (prop == "position") {
            if      (val == "absolute") ctx.positionMode = contracts::PositionMode::Absolute;
            else if (val == "relative") ctx.positionMode = contracts::PositionMode::Relative;
            else if (val == "center")   ctx.positionMode = contracts::PositionMode::Center;
            else                        ctx.positionMode = contracts::PositionMode::Default;
        }
    }

    // ─────────────────────────────────────────────────────────────────────
    //  PASS 2 — positional properties
    // ─────────────────────────────────────────────────────────────────────

    static void pass2(
        contracts::StyleContext& ctx,
        const std::string&       prop,
        const std::string&       val
    ) {
        auto& el = ctx.self;

        // Resolve the reference frame
        const bool isAbsolute  = (ctx.positionMode == contracts::PositionMode::Absolute);
        sf::Vector2f refOrigin = isAbsolute ? sf::Vector2f{0.f,0.f} : ctx.parentPos;
        sf::Vector2f refSize   = isAbsolute ? ctx.windowSize         : ctx.parentSize;

        auto pos    = el->getPosition();
        auto elSize = el->getSize();

        if (prop == "left" || prop == "x") {
            float x = LR::resolve(val, refSize.x, ctx.windowSize);
            el->setPosition({ refOrigin.x + x + ctx.box.marginLeft, pos.y });
        }
        else if (prop == "right") {
            float x = LR::resolve(val, refSize.x, ctx.windowSize);
            el->setPosition({
                refOrigin.x + refSize.x - x - elSize.x - ctx.box.marginRight,
                pos.y
            });
        }
        else if (prop == "top" || prop == "y") {
            float y = LR::resolve(val, refSize.y, ctx.windowSize);
            el->setPosition({ pos.x, refOrigin.y + y + ctx.box.marginTop });
        }
        else if (prop == "bottom") {
            float y = LR::resolve(val, refSize.y, ctx.windowSize);
            el->setPosition({
                pos.x,
                refOrigin.y + refSize.y - y - elSize.y - ctx.box.marginBottom
            });
        }
        else if (prop == "position") {
            if (val == "center") {
                el->setPosition({
                    refOrigin.x + (refSize.x - elSize.x) / 2.f,
                    refOrigin.y + (refSize.y - elSize.y) / 2.f
                });
            }
        }
    }

    // ─────────────────────────────────────────────────────────────────────
    //  Deferred transforms
    // ─────────────────────────────────────────────────────────────────────

    static void flushDeferredTransforms(contracts::StyleContext& ctx) {
        if (!ctx.deferredTranslateXPct.has_value() &&
            !ctx.deferredTranslateYPct.has_value()) return;

        sf::Vector2f size = ctx.self->getSize();
        sf::Vector2f pos  = ctx.self->getPosition();

        if (ctx.deferredTranslateXPct.has_value())
            pos.x += size.x * (*ctx.deferredTranslateXPct / 100.f);
        if (ctx.deferredTranslateYPct.has_value())
            pos.y += size.y * (*ctx.deferredTranslateYPct / 100.f);

        ctx.self->setPosition(pos);
    }

    // ─────────────────────────────────────────────────────────────────────
    //  Small enum parsers
    // ─────────────────────────────────────────────────────────────────────

    static contracts::FlexLayout::Justify parseJustify(const std::string& v) {
        using J = contracts::FlexLayout::Justify;
        if (v=="flex-end"   ||v=="end")            return J::End;
        if (v=="center")                           return J::Center;
        if (v=="space-between")                    return J::SpaceBetween;
        if (v=="space-around")                     return J::SpaceAround;
        if (v=="space-evenly")                     return J::SpaceEvenly;
        return J::Start;
    }

    static contracts::FlexLayout::Align parseAlign(const std::string& v) {
        using A = contracts::FlexLayout::Align;
        if (v=="flex-end" || v=="end")   return A::End;
        if (v=="center")                 return A::Center;
        if (v=="stretch")                return A::Stretch;
        return A::Start;
    }

    static sf::Text::Style parseTextStyle(const std::string& val) {
        sf::Text::Style style = sf::Text::Style::Regular;
        if (val.find("bold")      != std::string::npos)
            style = static_cast<sf::Text::Style>(style | sf::Text::Style::Bold);
        if (val.find("italic")    != std::string::npos)
            style = static_cast<sf::Text::Style>(style | sf::Text::Style::Italic);
        if (val.find("underline") != std::string::npos)
            style = static_cast<sf::Text::Style>(style | sf::Text::Style::Underlined);
        if (val.find("strike")    != std::string::npos)
            style = static_cast<sf::Text::Style>(style | sf::Text::Style::StrikeThrough);
        return style;
    }
};

} // namespace core
