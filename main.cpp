#include <SFML/Graphics.hpp>
#include "CSS.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode({1280, 720}), "CSS SFML Library - Test");
    window.setFramerateLimit(60);

    CSS::init(window);

    // Card centered in window
    auto card = sf::RectangleShape();
    CSS::Style(card, {
        "width: 90%",
        "height: 90%",
        "background-color: #1e1e2e",
        "border-color: #89b4fa",
        "border-width: 2px",
        "position: center"
    });

    // Button — 80% of card width, positioned inside card
    auto btn = sf::RectangleShape();
    CSS::Style(btn, {
        "width: 48px",
        "height: 48px",
        "background-color: #ffffff"
    }, CSS::wrap(card));

    // Arrange card children via flex — no parent, with children
    CSS::Style(card, {
        "padding: 32px 24px",
        "display: flex",
        "flex-direction: column",
        "justify-content: center",
        "align-items: center"
    }, CSS::StyleableList{ CSS::wrap(btn) });

    // Small circle pinned to bottom-right (FAB)
    auto fab = sf::CircleShape();
    CSS::Style(fab, {
        "width: 52px",
        "background-color: #a6e3a1",
        "border-color: #40a02b",
        "border-width: 2px",
        "position: absolute",
        "right: 24px",
        "bottom: 24px"
    });

    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear(sf::Color(17, 17, 27));
        window.draw(card);
        window.draw(btn);
        window.draw(fab);
        window.display();
    }

    return 0;
}