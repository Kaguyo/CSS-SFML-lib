#pragma once
#include "ShapeAdapterBase.hpp"
#include <SFML/Graphics/RectangleShape.hpp>

namespace adapters {

class RectAdapter final : public ShapeAdapterBase<sf::RectangleShape> {
public:
    explicit RectAdapter(sf::RectangleShape& s)
        : ShapeAdapterBase(s) {}

    // RectangleShape has an explicit setSize / getSize pair
    void setSize(sf::Vector2f sz) override {
        shape_->setSize(sz);
    }
    sf::Vector2f getSize() const override {
        return shape_->getSize();
    }

    std::string typeName() const override { return "RectangleShape"; }
};

} // namespace adapters
