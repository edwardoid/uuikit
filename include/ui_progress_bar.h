#pragma once

#include "ui_types.h"
#include "ui_element.h"

enum class progress_flags_t: uint8_t
{
    bar_only = 0x00,   /* Only progress bar, without any text*/
    percentage = 0x01, /* eg 56% */
    text_only = 0x02,  /* Bar and text */
    values = 0x04      /* Values 567/892 */
};

class ProgressBar: public Element
{
public:
    ProgressBar();
    progress_flags_t options = progress_flags_t::bar_only;
    const char* text = nullptr;
    
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