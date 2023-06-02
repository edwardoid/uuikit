#pragma once

#include "ui_types.h"
#include "ui_element.h"

class Slider: public Element
{
public:
    Slider();
    
    virtual void calculateBounds(const Box& within, const UIStyle& style, U8G2* device) override;
    inline void setRange(uint16_t min, uint16_t max) { m_minValue = min; m_maxValue = max; }
    inline uint16_t minValue() const { return m_minValue; }
    inline uint16_t maxValue() const { return m_maxValue; }
    
    inline void setValue(uint16_t value) { m_value = value; }
    inline uint16_t value() const { return m_value; }
private:
    uint16_t m_minValue = 0;
    uint16_t m_maxValue = 100;
    uint16_t m_value = 0;
};