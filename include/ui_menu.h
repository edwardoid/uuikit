#pragma once

#include "ui_element.h"

#include <array>

class Menu: public Element
{
public:
    using EntriesContainer = std::array<Element*, 10>;
    Menu();
    ~Menu() = default;

    void add(Element* e);
    inline uint8_t childrenCount() const { return m_childrenCount; }
    inline const EntriesContainer& children() const { return m_children; }
    inline uint8_t selected() const { return m_selectedEntry; }
    inline void select(uint8_t entry) { m_selectedEntry = entry; m_children[entry]->select(true); }
    virtual bool handle(const user_input_t key);
    virtual void calculateBounds(const Box& within, const UIStyle& style, struct U8G2* device) override;
private:
    EntriesContainer m_children;
    uint8_t m_childrenCount = 0;
    uint8_t m_selectedEntry = 0;
};