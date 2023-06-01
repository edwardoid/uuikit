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
        .regular = u8g2_font_prospero_nbp_tr, // u8g2_font_prospero_nbp_tr
        .small = u8g2_font_6x10_tf,
        .symbols = u8g2_font_unifont_t_symbols
    }
{
    m_dev->setFont(m_st.regular);
    m_st.regularHeight = m_dev->getMaxCharHeight();

    m_dev->setFont(m_st.symbols);
    m_st.symbolsHeight = m_dev->getMaxCharHeight();

    m_dev->setFont(m_st.small);
    m_st.smallHeight = m_dev->getMaxCharHeight();

    m_screens.fill(nullptr);
}

void Engine::input(user_input_t key)
{
    if (m_current < m_screensCount)
        if(m_screens[m_current]->handle(key, m_st))
            render();
}

bool Engine::render()
{
    ESP_LOGI(TAG, "Rendering screen %d", m_current);
    if (m_screensCount < m_current)
        return false;
    bool ret = true;
    
    m_dev->firstPage();
    do {
        renderScreen(* m_screens[m_current]);
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

bool Engine::renderButton(const ButtonBase& src)
{
    m_dev->setDrawColor(1);
    if (src.flags & flags_t::selected)
        m_dev->drawRBox(src.bounds.x, src.bounds.y, src.bounds.w, src.bounds.h, m_st.minimalOffset);
    else
        m_dev->drawRFrame(src.bounds.x, src.bounds.y, src.bounds.w, src.bounds.h, m_st.minimalOffset);

    m_dev->setFont(m_st.regular);
    m_dev->setFontPosCenter();

    if (src.flags & flags_t::selected)
        m_dev->setDrawColor(0);

    uint8_t textOffset = 0;

    if (src.icon != nullptr)
    {
        textOffset = m_st.minimalOffset + src.icon->sz;

        m_dev->drawBitmap(src.bounds.x + m_st.minimalOffset,
                          src.bounds.y + (src.bounds.h - src.icon->sz) / 2,
                          src.icon->sz / 8,
                          src.icon->sz, src.icon->img);
    }

    m_dev->drawUTF8(src.bounds.x + m_st.minimalOffset + textOffset, src.bounds.vcenter(), src.text);

    return true;
}

bool Engine::renderToggle(const Toggle& src)
{
    renderButton(src);

    m_dev->setFont(m_st.symbols);
    //m_dev->setFontPosCenter();
    dim_t symbolWidth = 16;
    if (src.value)
        m_dev->drawGlyph(src.bounds.x + src.bounds.w - m_st.minimalOffset - symbolWidth, src.bounds.vcenter() + 2, CHECKED_SYMBOL);
    else
        m_dev->drawGlyph(src.bounds.x + src.bounds.w - m_st.minimalOffset - symbolWidth, src.bounds.vcenter() + 2, UNCHECKED_SYMBOL);


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
    for(uint8_t i = src.renderData.currentItem; i < src.childrenCount(); ++i)
    {
        src.children()[i]->bounds.y += src.renderData.yOffset;
        renderUnknown(src.children()[i]);
        src.children()[i]->bounds.y -= src.renderData.yOffset;
    }
    return true;
}