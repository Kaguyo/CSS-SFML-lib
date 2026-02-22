#pragma once
#include "../contracts/IStyleable.hpp"
#include <SFML/Graphics/Sprite.hpp>

namespace adapters {

class SpriteAdapter final : public contracts::IStyleable {
public:
    explicit SpriteAdapter(sf::Sprite& s) : sprite_(&s) {}

    // ── Geometry queries ───────────────────────────────────────────────────
    sf::Vector2f getPosition() const override { return sprite_->getPosition(); }
    sf::Vector2f getOrigin()   const override { return sprite_->getOrigin(); }
    sf::Vector2f getScale()    const override { return sprite_->getScale(); }

    sf::FloatRect getBounds() const override {
        return sprite_->getLocalBounds();
    }
    sf::Vector2f getSize() const override {
        // For sprites, report the scaled visual size
        auto b = sprite_->getLocalBounds();
        auto s = sprite_->getScale();
        return { b.size.x * s.x, b.size.y * s.y };
    }

    // ── Geometry mutations ─────────────────────────────────────────────────
    void setPosition(sf::Vector2f p) override { sprite_->setPosition(p); }
    void move       (sf::Vector2f d) override { sprite_->move(d); }
    void setOrigin  (sf::Vector2f o) override { sprite_->setOrigin(o); }
    void setRotation(float deg)      override { sprite_->setRotation(sf::degrees(deg)); }

    // Sprites scale to achieve a target size
    void setSize(sf::Vector2f target) override {
        auto b = sprite_->getLocalBounds();
        if (b.size.x > 0.f && b.size.y > 0.f)
            sprite_->setScale({ target.x / b.size.x, target.y / b.size.y });
    }
    void setScale(sf::Vector2f s) override { sprite_->setScale(s); }

    // ── Color / visual mutations ───────────────────────────────────────────
    // sf::Sprite uses setColor (tint), not setFillColor
    void setFillColor       (sf::Color c) override { sprite_->setColor(c); }
    void setOutlineColor    (sf::Color)   override {} // not supported
    void setOutlineThickness(float)       override {} // not supported
    sf::Color getFillColor() const        override { return sprite_->getColor(); }

    bool        isSprite() const override { return true; }
    std::string typeName() const override { return "Sprite"; }

private:
    sf::Sprite* sprite_;
};

} // namespace adapters
