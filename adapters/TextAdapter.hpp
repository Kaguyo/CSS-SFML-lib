#pragma once
#include "../contracts/IStyleable.hpp"
#include <SFML/Graphics/Text.hpp>

namespace adapters {

class TextAdapter final : public contracts::IStyleable {
public:
    explicit TextAdapter(sf::Text& t) : text_(&t) {}

    // ── Geometry queries ───────────────────────────────────────────────────
    sf::Vector2f getPosition() const override { return text_->getPosition(); }
    sf::Vector2f getOrigin()   const override { return text_->getOrigin(); }
    sf::Vector2f getScale()    const override { return text_->getScale(); }

    sf::FloatRect getBounds() const override {
        return text_->getLocalBounds();
    }
    sf::Vector2f getSize() const override {
        auto b = text_->getLocalBounds();
        return { b.size.x, b.size.y };
    }

    // ── Geometry mutations ─────────────────────────────────────────────────
    void setPosition(sf::Vector2f p) override { text_->setPosition(p); }
    void move       (sf::Vector2f d) override { text_->move(d); }
    void setOrigin  (sf::Vector2f o) override { text_->setOrigin(o); }
    void setScale   (sf::Vector2f s) override { text_->setScale(s); }
    void setRotation(float deg)      override { text_->setRotation(sf::degrees(deg)); }
    // Text has no setSize — font size is controlled via setCharacterSize
    void setSize(sf::Vector2f)       override {}

    // ── Color / visual mutations ───────────────────────────────────────────
    void setFillColor       (sf::Color c) override { text_->setFillColor(c); }
    void setOutlineColor    (sf::Color c) override { text_->setOutlineColor(c); }
    void setOutlineThickness(float t)     override { text_->setOutlineThickness(t); }
    sf::Color getFillColor() const        override { return text_->getFillColor(); }

    // ── Text-only mutations ────────────────────────────────────────────────
    void setCharacterSize(unsigned sz)      override { text_->setCharacterSize(sz); }
    void setLetterSpacing(float f)          override { text_->setLetterSpacing(f); }
    void setLineSpacing  (float f)          override { text_->setLineSpacing(f); }
    void setTextStyle    (sf::Text::Style s)override { text_->setStyle(s); }

    bool        isText()   const override { return true; }
    std::string typeName() const override { return "Text"; }

private:
    sf::Text* text_;
};

} // namespace adapters
