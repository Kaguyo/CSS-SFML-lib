#pragma once
#include <string>
#include <vector>
#include <optional>
#include <memory>
#include <array>
#include "./IStyleable.hpp"
#include <SFML/System/Vector2.hpp>

namespace contracts {

// Forward declared here — defined in adapters layer


// ─────────────────────────────────────────────────────────────────────────────
//  Styleable — lightweight copyable handle over any IStyleable implementation.
//  Internally holds a shared_ptr so copies are cheap and the handle is nullable.
// ─────────────────────────────────────────────────────────────────────────────

class Styleable {
public:
    Styleable() = default;

    explicit Styleable(std::shared_ptr<IStyleable> impl)
        : impl_(std::move(impl)) {}

    IStyleable* operator->() const { return impl_.get(); }
    IStyleable& operator*()  const { return *impl_; }

    [[nodiscard]] bool valid() const { return impl_ != nullptr; }
    explicit operator bool()  const { return valid(); }

private:
    std::shared_ptr<IStyleable> impl_;
};

using StyleableList = std::vector<Styleable>;

// ─────────────────────────────────────────────────────────────────────────────
//  Declaration — one parsed CSS rule: "background-color" → "#1e1e2e"
// ─────────────────────────────────────────────────────────────────────────────

struct Declaration {
    std::string property;   // normalised to lowercase-kebab-case
    std::string value;      // trimmed raw value string
};

// ─────────────────────────────────────────────────────────────────────────────
//  BoxModel — resolved padding and margin values
// ─────────────────────────────────────────────────────────────────────────────

struct BoxModel {
    float paddingTop    = 0.f, paddingRight  = 0.f,
          paddingBottom = 0.f, paddingLeft   = 0.f;
    float marginTop     = 0.f, marginRight   = 0.f,
          marginBottom  = 0.f, marginLeft    = 0.f;

    [[nodiscard]] sf::Vector2f paddingOffset() const {
        return { paddingLeft, paddingTop };
    }
    [[nodiscard]] sf::Vector2f innerSize(sf::Vector2f outer) const {
        return {
            outer.x - paddingLeft - paddingRight,
            outer.y - paddingTop  - paddingBottom
        };
    }
};

// ─────────────────────────────────────────────────────────────────────────────
//  FlexLayout — layout intent parsed from display/flex-* properties
// ─────────────────────────────────────────────────────────────────────────────

struct FlexLayout {
    bool enabled = false;
    bool column  = false;   // true → flex-direction: column
    float gap    = 0.f;

    enum class Justify {
        Start, End, Center,
        SpaceBetween, SpaceAround, SpaceEvenly
    };
    enum class Align {
        Start, End, Center, Stretch
    };

    Justify justify = Justify::Start;
    Align   align   = Align::Start;
};

// ─────────────────────────────────────────────────────────────────────────────
//  PositionMode — how the element is positioned relative to its container
// ─────────────────────────────────────────────────────────────────────────────

enum class PositionMode {
    Default,    // flow — positioned by parent flex or explicit left/top
    Relative,   // offset from parent origin
    Absolute,   // offset from window origin
    Center,     // CSS extension: centered in containing block
};

// ─────────────────────────────────────────────────────────────────────────────
//  StyleContext — full resolved context for one Style() call.
//  Passed by reference through every layer of the pipeline.
// ─────────────────────────────────────────────────────────────────────────────

struct StyleContext {
    // The element being styled
    Styleable self;

    // Containing block geometry
    sf::Vector2f parentSize;    // resolve % against this
    sf::Vector2f parentPos;     // origin of the containing block
    sf::Vector2f windowSize;    // always the render window size

    // Parsed box model (filled during pass 1)
    BoxModel   box;

    // Flex layout intent (filled during pass 1)
    FlexLayout flex;

    // Positioning mode (filled during pass 1, consumed in pass 2)
    PositionMode positionMode = PositionMode::Default;

    // Deferred percentage transforms — resolved after size is known
    std::optional<float> deferredTranslateXPct;
    std::optional<float> deferredTranslateYPct;
};

} // namespace contracts
