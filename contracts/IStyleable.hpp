#pragma once
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>
#include <string>

namespace contracts {

// ─────────────────────────────────────────────────────────────────────────────
//  IStyleable
//
//  Unified interface over all SFML drawable/transformable types.
//  Adapters (adapters) implement this for each concrete SFML type.
//
//  Design notes:
//    • Every method has a default no-op body so adapters only override
//      what their underlying type actually supports.
//    • isText() is the single type-discriminator needed — avoids dynamic_cast
//      chains in the property dispatcher.
//    • getSize() always returns the VISUAL bounding size (post-scale for sprites,
//      character size bounds for text, shape size for rectangles/circles).
// ─────────────────────────────────────────────────────────────────────────────

class IStyleable {
public:
    virtual ~IStyleable() = default;

    // ── Geometry queries ───────────────────────────────────────────────────
    [[nodiscard]] virtual sf::Vector2f  getPosition() const = 0;
    [[nodiscard]] virtual sf::Vector2f  getSize()     const = 0;
    [[nodiscard]] virtual sf::FloatRect getBounds()   const = 0;
    [[nodiscard]] virtual sf::Vector2f  getOrigin()   const = 0;
    [[nodiscard]] virtual sf::Vector2f  getScale()    const = 0;

    // ── Geometry mutations ─────────────────────────────────────────────────
    virtual void setPosition(sf::Vector2f pos)  = 0;
    virtual void move       (sf::Vector2f delta)= 0;
    virtual void setOrigin  (sf::Vector2f o)    = 0;
    virtual void setScale   (sf::Vector2f s)    = 0;
    virtual void setRotation(float degrees)     = 0;

    // Only meaningful for resizable shapes (RectangleShape).
    // Default: no-op — CircleShape resizes via setRadius, Text via charSize.
    virtual void setSize(sf::Vector2f /*size*/) {}

    // ── Color / visual mutations ───────────────────────────────────────────
    virtual void      setFillColor       (sf::Color c) = 0;
    virtual void      setOutlineColor    (sf::Color c) = 0;
    virtual void      setOutlineThickness(float t)     = 0;
    [[nodiscard]]
    virtual sf::Color getFillColor()               const = 0;

    // ── Text-only mutations (no-op on non-text adapters) ──────────────────
    virtual void setCharacterSize(unsigned /*size*/)         {}
    virtual void setLetterSpacing(float /*factor*/)          {}
    virtual void setLineSpacing  (float /*factor*/)          {}
    virtual void setTextStyle    (sf::Text::Style /*style*/) {}

    // ── Type discriminators ────────────────────────────────────────────────
    [[nodiscard]] virtual bool        isText()    const { return false; }
    [[nodiscard]] virtual bool        isSprite()  const { return false; }
    [[nodiscard]] virtual std::string typeName()  const = 0;
};

} // namespace contracts
