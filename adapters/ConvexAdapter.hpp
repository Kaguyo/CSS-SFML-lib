#pragma once
#include "ShapeAdapterBase.hpp"
#include <SFML/Graphics/ConvexShape.hpp>

namespace adapters {

// ConvexShape has no intrinsic "size" — getBounds() returns the tight AABB.
// setSize scales the shape uniformly to fit the requested bounding box.
class ConvexAdapter final : public ShapeAdapterBase<sf::ConvexShape> {
public:
    explicit ConvexAdapter(sf::ConvexShape& s)
        : ShapeAdapterBase(s) {}

    void setSize(sf::Vector2f target) override {
        auto current = getSize();
        if (current.x > 0.f && current.y > 0.f)
            shape_->setScale({ target.x / current.x, target.y / current.y });
    }

    std::string typeName() const override { return "ConvexShape"; }
};

} // namespace adapters
