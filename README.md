# CSS-SFML

A CSS-inspired styling library for [SFML 3.0](https://www.sfml-dev.org/).

```cpp
CSS::init(window);

auto card = sf::RectangleShape();
CSS::Style(card, {
    "width: 50%",
    "height: 200px",
    "background-color: #1e1e2e",
    "position: center"
});
```

If you know CSS, you already know how to use this.

---

## Requirements

- **C++17** or later — GCC 10+, Clang 11+, MSVC 19.29+
- **SFML 3.0.0** — [download at sfml-dev.org](https://www.sfml-dev.org/download/)

---

## Installation

Header-only. Copy the `include/` folder into your project, add it to your include path, and include:

```cpp
#include <CSS.hpp>
```

---

## Usage

When no parent is provided, the window acts as the containing block — the same way `<body>` works in a browser.

**No parent, no children** — `%` resolves against the window:
```cpp
CSS::Style(card, {
    "width: 40%",
    "height: 300px",
    "background-color: #1e1e2e",
    "position: center"
});
```

**With parent, no children** — `%` resolves against the parent:
```cpp
CSS::Style(btn, {
    "width: 80%",
    "height: 48px",
    "background-color: #89b4fa"
}, CSS::wrap(card));
```

**No parent, with children** — distributes children via flex:
```cpp
CSS::Style(card, {
    "padding: 24px",
    "display: flex",
    "flex-direction: column",
    "align-items: center",
    "justify-content: space-between",
    "gap: 12px"
}, CSS::StyleableList{ CSS::wrap(btn), CSS::wrap(label) });
```

**With parent and children**:
```cpp
CSS::Style(card, {
    "width: 60%",
    "padding: 24px",
    "display: flex",
    "align-items: center",
    "gap: 8px"
}, CSS::wrap(panel), CSS::StyleableList{ CSS::wrap(btn), CSS::wrap(label) });
```

---

## What it supports

`width` `height` `background-color` `color` `border-color` `border-width` `opacity`
`left` `right` `top` `bottom` `position` `margin` `padding`
`display: flex` `flex-direction` `justify-content` `align-items` `gap`
`transform` `rotation` `scale` `origin`
`font-size` `font-style` `letter-spacing` `line-spacing`

Units: `px` `%` `vw` `vh` — camelCase aliases accepted.

---

## Technical Notes for Developers
### Architectural organization:
```
💱 Adapters

- Acts as a *bridge between native SFML objects and the library core*.
- Uses *factory-like principles* to wrap or instantiate SFML types (e.g., shapes, text, sprites) into objects the CSS system can work with.
- Works as *middleware*, so the core doesn’t need to know SFML specifics — it only works with abstract styleable objects.
- Translates core styling operations back into actual SFML API calls.
________________________________________________________________________

✔️ Contracts

- Contains *interfaces and abstract definitions* that describe what a styleable object should support.
- Ensures consistent behavior across different SFML objects that can be styled (like width, height, colors, layout).
- Provides the rules that the core styling logic relies on to apply properties safely.
________________________________________________________________________

⚙️ Core

- This is the *CSS “engine”* of the library.
- Parses and *dispatches CSS-like properties* to objects through contract interfaces.
- Manages layout behaviors (position, size resolution, basic layout logic) and applies changes using operations defined by natives and adapters.
- Coordinates how styles get propagated and resolved across objects.
________________________________________________________________________

🛠 Utilities

- Contains *helper functions and logic* used by all other layers.
- Handles string parsing (e.g., `"50%"`, `"14px"`), unit conversions, generic operations, and other reusable logic.
- Pure computational support — no direct SFML or styling responsibilities.
________________________________________________________________________

📌 Overall Flow

1. *SFML native objects* are wrapped by *Adapters* into objects that the CSS system can style.  
2. The *Core* takes CSS rules and uses *Contracts* to understand and validate what properties apply to those objects.  
3. When applying styles, the core uses *Utilities* to parse and compute values (e.g., convert units).  
4. Results are translated back through *Adapters* to update the real SFML objects with the computed styles.
```
### Design patterns: 
`Adapter` `Factory` `Facade`

---

## Credits

Built on top of **SFML** by Laurent Gomila — [sfml-dev.org](https://www.sfml-dev.org/)

**CSS-SFML** is developed by [Kaguyo](https://github.com/kaguyo).  
Licensed under the [zlib License](./LICENSE).