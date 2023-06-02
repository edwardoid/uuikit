#pragma once

#include "ui_types.h"
#include "ui_element.h"

class Slider: public Element
{
public:
    Slider();
    
    virtual void calculateBounds(const Box& within, const UIStyle& style, U8G2* device) override;
    inline void setRange(int16_t min, int16_t max) { m_minValue = min; m_maxValue = max; }
    inline int16_t minValue() const { return m_minValue; }
    inline int16_t maxValue() const { return m_maxValue; }
    virtual bool handle(const user_input_t key, const UIStyle& style);
    inline void setValue(int16_t value) { m_value = value; }
    inline void setStep(int16_t step) { m_step = step; }
    inline int16_t value() const { return m_value; }
private:
    int16_t m_minValue = 0;
    int16_t m_maxValue = 100;
    int16_t m_value = 0;
    int16_t m_step = 0;
};