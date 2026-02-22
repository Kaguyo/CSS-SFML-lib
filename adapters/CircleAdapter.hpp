#pragma once
#include "ShapeAdapterBase.hpp"
#include <SFML/Graphics/CircleShape.hpp>

namespace adapters {

class CircleAdapter final : public ShapeAdapterBase<sf::CircleShape> {
public:
    explicit CircleAdapter(sf::CircleShape& s)
        : ShapeAdapterBase(s) {}

    // width/height on a circle → radius = min(w,h) / 2
    void setSize(sf::Vector2f sz) override {
        shape_->setRadius(std::min(sz.x, sz.y) / 2.f);
    }
    sf::Vector2f getSize() const override {
        float r = shape_->getRadius();
        return { r * 2.f, r * 2.f };
    }

    std::string typeName() const override { return "CircleShape"; }
};

} // namespace adapters
