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

Header-only. Copy the `css/` folder into your project and include:

```cpp
#include "css/CSS.hpp"
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
