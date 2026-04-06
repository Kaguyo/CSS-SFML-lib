#pragma once
#include "../contracts/IStyleable.hpp"
#include <SFML/Graphics/Shape.hpp>

namespace adapters {

// ─────────────────────────────────────────────────────────────────────────────
//  ShapeAdapterBase<ShapeT>
//
//  CRTP base that implements all IStyleable methods common to every sf::Shape
//  subclass (RectangleShape, CircleShape, ConvexShape, etc.).
//
//  Concrete adapters inherit from this and only override what differs
//  (e.g. setSize maps to different calls for Rect vs Circle).
// ─────────────────────────────────────────────────────────────────────────────

template<typename ShapeT>
class ShapeAdapterBase : public contracts::IStyleable {
public:
    explicit ShapeAdapterBase(ShapeT& shape) : shape_(&shape) {}

    // ── Geometry queries ───────────────────────────────────────────────────
    sf::Vector2f getPosition() const override { return shape_->getPosition(); }
    sf::Vector2f getOrigin()   const override { return shape_->getOrigin(); }
    sf::Vector2f getScale()    const override { return shape_->getScale(); }

    sf::FloatRect getBounds() const override {
        return shape_->getLocalBounds();
    }
    sf::Vector2f getSize() const override {
        auto b = shape_->getLocalBounds();
        return { b.size.x, b.size.y };
    }

    // ── Geometry mutations ─────────────────────────────────────────────────
    void setPosition(sf::Vector2f p) override { shape_->setPosition(p); }
    void move       (sf::Vector2f d) override { shape_->move(d); }
    void setOrigin  (sf::Vector2f o) override { shape_->setOrigin(o); }
    void setScale   (sf::Vector2f s) override { shape_->setScale(s); }
    void setRotation(float deg)      override { shape_->setRotation(sf::degrees(deg)); }

    // ── Color / visual mutations ───────────────────────────────────────────
    void setFillColor       (sf::Color c) override { shape_->setFillColor(c); }
    void setOutlineColor    (sf::Color c) override { shape_->setOutlineColor(c); }
    void setOutlineThickness(float t)     override { shape_->setOutlineThickness(t); }
    sf::Color getFillColor() const        override { return shape_->getFillColor(); }

    std::string typeName() const override { return "Shape"; }

protected:
    ShapeT* shape_;
};

} // namespace adapters
