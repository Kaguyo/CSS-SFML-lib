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

## Credits

Built on top of **SFML** by Laurent Gomila — [sfml-dev.org](https://www.sfml-dev.org/)

**CSS-SFML** is developed by [Kaguyo](https://github.com/kaguyo).  
Licensed under the [MIT License](./LICENSE).
