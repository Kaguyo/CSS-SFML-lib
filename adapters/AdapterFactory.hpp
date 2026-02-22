#pragma once
#include "../contracts/Types.hpp"
#include "RectAdapter.hpp"
#include "CircleAdapter.hpp"
#include "ConvexAdapter.hpp"
#include "TextAdapter.hpp"
#include "SpriteAdapter.hpp"
#include <memory>
#include <type_traits>

namespace adapters {

// ─────────────────────────────────────────────────────────────────────────────
//  AdapterFactory
//
//  Single entry point to wrap any supported SFML type into a Styleable handle.
//  Compile-time error for unsupported types via static_assert.
//
//  Usage:
//    auto btn = sf::RectangleShape();
//    contracts::Styleable s = AdapterFactory::make(btn);
// ─────────────────────────────────────────────────────────────────────────────

struct AdapterFactory {
    template<typename T>
    static contracts::Styleable make(T& element) {
        using U = std::remove_cv_t<std::remove_reference_t<T>>;

        if constexpr (std::is_same_v<U, contracts::Styleable>)
            return element;

        else if constexpr (std::is_same_v<U, sf::RectangleShape>)
            return contracts::Styleable(std::make_shared<RectAdapter>(element));

        else if constexpr (std::is_same_v<U, sf::CircleShape>)
            return contracts::Styleable(std::make_shared<CircleAdapter>(element));

        else if constexpr (std::is_same_v<U, sf::ConvexShape>)
            return contracts::Styleable(std::make_shared<ConvexAdapter>(element));

        else if constexpr (std::is_same_v<U, sf::Text>)
            return contracts::Styleable(std::make_shared<TextAdapter>(element));

        else if constexpr (std::is_same_v<U, sf::Sprite>)
            return contracts::Styleable(std::make_shared<SpriteAdapter>(element));

        else {
            static_assert(sizeof(U) == 0, "CSS: unsupported SFML type.");
            __builtin_unreachable();
        }
    }
};

} // namespace adapters
