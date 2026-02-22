#pragma once
#include "../contracts/Types.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <optional>

namespace core {

// ─────────────────────────────────────────────────────────────────────────────
//  ContextBuilder
//
//  Constructs a StyleContext that describes the layout environment for one
//  Style() call.
//
//  Containing-block resolution rules (mirrors CSS spec):
//    • If parent is provided → parentSize/parentPos come from parent
//    • If parent is absent   → window is the containing block
//      (analogous to an element whose nearest positioned ancestor is <body>)
// ─────────────────────────────────────────────────────────────────────────────

struct ContextBuilder {

    static contracts::StyleContext build(
        contracts::Styleable&                    self,
        std::optional<contracts::Styleable>&     parent,
        sf::RenderWindow&                        window
    ) {
        contracts::StyleContext ctx;
        ctx.self       = self;
        ctx.windowSize = sf::Vector2f(window.getSize());

        if (parent.has_value() && parent->valid()) {
            ctx.parentSize = (*parent)->getSize();
            ctx.parentPos  = (*parent)->getPosition();
        } else {
            ctx.parentSize = ctx.windowSize;
            ctx.parentPos  = { 0.f, 0.f };
        }

        return ctx;
    }
};

} // namespace core
