#include "ui_engine.h"
#include "ui_push_button.h"
#include <esp_log.h>

#include <u8g2.h>
#include <U8g2lib.h>
#include <string.h>

#define TAG "EGN"

#define max(a, b) (((a) > (b)) ? (a) : (b))

#define UNCHECKED_SYMBOL 9744
#define CHECKED_SYMBOL   9745

Engine::Engine(U8G2* device)
    : m_dev(device)
    , m_st {
        .menuFontSize = 14,
        .menuBoxHeight = 18,
        .minimalOffset = 4,
        .toggleSize = 10,
        .regular = u8g2_font_frigidaire_mr,
        .small = u8g2_font_6x10_tf,
        .symbols = u8g2_font_unifont_t_symbols
    }
{
    m_x = 1;
    m_y = 20;
    m_dev->setFont(m_st.regular);
    m_st.regularHeight = m_dev->getMaxCharHeight();

    m_dev->setFont(m_st.symbols);
    m_st.symbolsHeight = m_dev->getMaxCharHeight();

    m_dev->setFont(m_st.small);
    m_st.smallHeight = m_dev->getMaxCharHeight();
}

bool Engine::render()
{
    ESP_LOGI(TAG, "Rendering screen %d", m_current);
    if (m_current < 0)
        return false;
    
    m_dev->firstPage();
    bool ret = false;
    do {
        ret = renderScreen(* m_screens[m_current]);
    } while( m_dev->nextPage() );
    return ret;
}

bool Engine::renderScreen(const Screen& screen)
{
    for(int i = 0; i < screen.childrenCount(); ++i)
    {
        Element* e = screen.children()[i];
        if ((e->flags && flags_t::visible) == 0)
            continue;
        renderUnknown(e);
    }
    return true;
}

bool Engine::renderUnknown(const Element* element)
{
    switch(element->type)
    {
        case element_type_t::button:
            return renderButton(* (PushButton*)(element));
        case element_type_t::toggle:
            return renderToggle(* (Toggle*)(element));
        case element_type_t::label:
            return renderLabel(* (Label*)(element));
        case element_type_t::menu:
            return renderMenu(* (Menu*)(element));
        default:
            ESP_LOGW(TAG, "Unknown element type");
    }
    return false;
}

bool Engine::renderButton(const PushButton& src)
{
    m_dev->setFont(u8g2_font_ncenB14_tr);
    uint8_t textW = src.text == nullptr ? 0 : m_dev->getStrWidth(src.text);
    uint8_t h = max(m_dev->getMaxCharHeight(), src.icon == nullptr ? 0 : src.icon->height)  + 2 * m_st.minimalOffset;
    if (src.text != nullptr)
    {
        m_dev->setFont(u8g2_font_ncenB14_tr);
        m_dev->drawStr(m_st.minimalOffset, m_y + m_st.menuFontSize + m_st.minimalOffset, src.text);
    }

    uint8_t w = textW + (src.icon == nullptr ? m_st.minimalOffset : (m_st.minimalOffset + src.icon->height)) + 2 * m_st.minimalOffset;
    
    m_dev->drawRFrame(0, m_y, w, h, m_st.minimalOffset);
    return true;
}

bool Engine::renderToggle(const Toggle& src)
{
    m_dev->setFont(m_st.regular);
    m_dev->setFontPosCenter();
    m_dev->drawUTF8(src.bounds.x + m_st.minimalOffset, src.bounds.vcenter(), src.text);

    m_dev->setFont(m_st.symbols);
    m_dev->setFontPosCenter();
    dim_t symbolWidth = 15;
    if (src.value)
        m_dev->drawGlyph(src.bounds.x + src.bounds.w - m_st.minimalOffset - symbolWidth, src.bounds.vcenter(), CHECKED_SYMBOL);
    else
        m_dev->drawGlyph(src.bounds.x + src.bounds.w - m_st.minimalOffset - symbolWidth, src.bounds.vcenter(), UNCHECKED_SYMBOL);

    m_dev->drawRFrame(src.bounds.x, src.bounds.y, src.bounds.w, src.bounds.h, m_st.minimalOffset);

    return true;
}

bool Engine::renderLabel(const Label& src)
{
    uint16_t x = src.bounds.x;
    uint16_t y = src.bounds.y;

    const char* p = src.text;
    m_dev->setFont(m_st.small);

    while(*p != '\0')
    {
        y += m_st.smallHeight;

        uint16_t len = m_dev->getUTF8Width(p);

        x = (m_dev->getWidth() - len) / 2;

        m_dev->drawUTF8(x, y, p);
        
        while(*p != '\n' && *p != '\r' && *p != '\0') ++p;
        if (*p != '\0')
            ++p;
    } 
    return true;
}

bool Engine::renderMenu(const Menu& src)
{
    return true;
}