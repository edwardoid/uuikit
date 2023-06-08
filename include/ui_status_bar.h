#pragma once

#include "ui_element.h"
#include <array>

class StatusBar: public Element
{
public:
    using ElementsContainer = std::array<Element*, 5>;
    StatusBar();
    virtual ~StatusBar() = default;

    inline void add(Element* e) { m_children[m_childrenCount++] = e; }
    inline uint8_t childrenCount() const { return m_childrenCount; }
    inline const ElementsContainer& children() const { return m_children; }
    virtual bool handle(const user_input_t key, const UIStyle& style) { return false; }

private:
    ElementsContainer m_children;
    uint8_t m_childrenCount = 0;
};