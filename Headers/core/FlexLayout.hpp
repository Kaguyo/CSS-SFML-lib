#pragma once
#include "../contracts/Types.hpp"
#include <algorithm>
#include <cmath>

namespace core {

// ─────────────────────────────────────────────────────────────────────────────
//  FlexLayout
//
//  Implements a subset of CSS Flexible Box Layout for SFML elements.
//
//  Supported features:
//    flex-direction   row | column
//    justify-content  flex-start | flex-end | center |
//                     space-between | space-around | space-evenly
//    align-items      flex-start | flex-end | center | stretch
//    gap              uniform spacing between items
//    padding          inner offset from container edges
//
//  Limitations vs. CSS spec (intentional, SFML has no reflow):
//    • No flex-wrap (single-axis only)
//    • No flex-grow / flex-shrink / flex-basis on individual children
//    • No nested flex contexts (children are positioned, not reflowed)
//
//  Usage:
//    FlexLayout::apply(ctx, children);
//    — called after PropertyDispatcher::apply() so ctx.box and ctx.flex
//      are already populated.
// ─────────────────────────────────────────────────────────────────────────────

struct FlexLayout {

    static void apply(
        const contracts::StyleContext& ctx,
        contracts::StyleableList&      children
    ) {
        if (children.empty()) return;

        if (ctx.flex.enabled)
            applyFlex(ctx, children);
        else
            applyPaddingOffset(ctx, children);
    }

private:
    // ── Flex distribution ─────────────────────────────────────────────────

    static void applyFlex(const contracts::StyleContext& ctx, contracts::StyleableList& children)
    {
        sf::Vector2f containerPos  = ctx.self->getPosition();
        sf::Vector2f containerSize = ctx.self->getSize();

        // Inner area after padding
        float innerX = containerPos.x + ctx.box.paddingLeft;
        float innerY = containerPos.y + ctx.box.paddingTop;
        float innerW = containerSize.x - ctx.box.paddingLeft - ctx.box.paddingRight;
        float innerH = containerSize.y - ctx.box.paddingTop  - ctx.box.paddingBottom;

        const bool isColumn = ctx.flex.column;
        const float gap     = ctx.flex.gap;

        // Main-axis: total children size
        float totalMain = 0.f;
        for (auto& child : children) {
            auto sz = child->getSize();
            totalMain += isColumn ? sz.y : sz.x;
        }

        float mainAvail  = isColumn ? innerH : innerW;
        float gapTotal   = gap * static_cast<float>(std::max(0, static_cast<int>(children.size()) - 1));
        float remaining  = mainAvail - totalMain - gapTotal;

        // justify-content → starting offset and per-item extra spacing
        float offset  = 0.f;
        float between = 0.f;
        using J = contracts::FlexLayout::Justify;
        const size_t n = children.size();

        switch (ctx.flex.justify) {
            case J::Start:
                offset  = 0.f;
                between = 0.f;
                break;
            case J::End:
                offset  = remaining;
                between = 0.f;
                break;
            case J::Center:
                offset  = remaining / 2.f;
                between = 0.f;
                break;
            case J::SpaceBetween:
                offset  = 0.f;
                between = n > 1 ? remaining / static_cast<float>(n - 1) : 0.f;
                break;
            case J::SpaceAround:
                between = n > 0 ? remaining / static_cast<float>(n) : 0.f;
                offset  = between / 2.f;
                break;
            case J::SpaceEvenly:
                between = n > 0 ? remaining / static_cast<float>(n + 1) : 0.f;
                offset  = between;
                break;
        }

        // Iterate children and position each one
        float cursor = (isColumn ? innerY : innerX) + offset;

        for (auto& child : children) {
            sf::Vector2f sz = child->getSize();

            // Cross-axis alignment
            using A = contracts::FlexLayout::Align;
            float crossAvail = isColumn ? innerW : innerH;
            float crossStart = isColumn ? innerX : innerY;
            float crossEl    = isColumn ? sz.x   : sz.y;
            float crossPos;

            switch (ctx.flex.align) {
                case A::Start:
                    crossPos = crossStart;
                    break;
                case A::End:
                    crossPos = crossStart + crossAvail - crossEl;
                    break;
                case A::Center:
                    crossPos = crossStart + (crossAvail - crossEl) / 2.f;
                    break;
                case A::Stretch:
                    crossPos = crossStart;
                    if (isColumn) child->setSize({ innerW, sz.y });
                    else          child->setSize({ sz.x, innerH });
                    // Refresh sz after resize
                    sz = child->getSize();
                    break;
                default:
                    crossPos = crossStart;
                    break;
            }

            // Place child
            if (isColumn)
                child->setPosition({ crossPos, cursor });
            else
                child->setPosition({ cursor, crossPos });

            // Advance cursor: main size + gap + justify extra spacing
            float mainEl = isColumn ? sz.y : sz.x;
            cursor += mainEl + gap + between;
        }
    }

    // ── No flex — just offset children by padding ─────────────────────────

    static void applyPaddingOffset(
        const contracts::StyleContext& ctx,
        contracts::StyleableList&      children
    ) {
        sf::Vector2f origin = {
            ctx.self->getPosition().x + ctx.box.paddingLeft,
            ctx.self->getPosition().y + ctx.box.paddingTop
        };

        for (auto& child : children) {
            sf::Vector2f p = child->getPosition();
            child->setPosition({ origin.x + p.x, origin.y + p.y });
        }
    }
};

} // namespace core
