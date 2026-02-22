#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <vector>
#include <optional>
#include <stdexcept>
#include <string>

#include "./contracts/IStyleable.hpp"
#include "./contracts/Types.hpp"
#include "./adapters/AdapterFactory.hpp"
#include "./utilities/StringUtils.hpp"
#include "./utilities/ColorParser.hpp"
#include "./utilities/LengthResolver.hpp"
#include "./utilities/TransformParser.hpp"
#include "./core/RuleParser.hpp"
#include "./core/ContextBuilder.hpp"
#include "./core/PropertyDispatcher.hpp"
#include "./core/FlexLayout.hpp"

class CSS {
public:
    using Styleable     = contracts::Styleable;
    using StyleableList = contracts::StyleableList;

    static void init(sf::RenderWindow& window) {
        s_window = &window;
    }

    // Overload 1: no parent, no children
    template<typename T>
    static void Style(
        T&                              element,
        const std::vector<std::string>& rules
    ) {
        assertInitialised();
        Styleable self = adapters::AdapterFactory::make(element);
        std::optional<Styleable> noParent;
        auto ctx   = core::ContextBuilder::build(self, noParent, *s_window);
        auto decls = core::RuleParser::parse(rules);
        core::PropertyDispatcher::apply(ctx, decls);
    }

    // Overload 2: with parent, no children
    template<typename T>
    static void Style(
        T&                              element,
        const std::vector<std::string>& rules,
        Styleable                       parent
    ) {
        assertInitialised();
        Styleable self = adapters::AdapterFactory::make(element);
        std::optional<Styleable> optParent = parent;
        auto ctx   = core::ContextBuilder::build(self, optParent, *s_window);
        auto decls = core::RuleParser::parse(rules);
        core::PropertyDispatcher::apply(ctx, decls);
    }

    // Overload 3: no parent, with children
    template<typename T>
    static void Style(
        T&                              element,
        const std::vector<std::string>& rules,
        StyleableList                   children
    ) {
        assertInitialised();
        Styleable self = adapters::AdapterFactory::make(element);
        std::optional<Styleable> noParent;
        auto ctx   = core::ContextBuilder::build(self, noParent, *s_window);
        auto decls = core::RuleParser::parse(rules);
        core::PropertyDispatcher::apply(ctx, decls);
        core::FlexLayout::apply(ctx, children);
    }

    // Overload 4: with parent and children
    template<typename T>
    static void Style(
        T&                              element,
        const std::vector<std::string>& rules,
        Styleable                       parent,
        StyleableList                   children
    ) {
        assertInitialised();
        Styleable self = adapters::AdapterFactory::make(element);
        std::optional<Styleable> optParent = parent;
        auto ctx   = core::ContextBuilder::build(self, optParent, *s_window);
        auto decls = core::RuleParser::parse(rules);
        core::PropertyDispatcher::apply(ctx, decls);
        core::FlexLayout::apply(ctx, children);
    }

    static sf::Color parseColor(const std::string& value) {
        return utilities::ColorParser::parse(value);
    }

    template<typename T>
    static Styleable wrap(T& element) {
        return adapters::AdapterFactory::make(element);
    }

private:
    inline static sf::RenderWindow* s_window = nullptr;

    static void assertInitialised() {
        if (!s_window)
            throw std::runtime_error(
                "[CSS] CSS::init(window) must be called before CSS::Style().");
    }
};