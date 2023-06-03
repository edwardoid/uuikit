#include "ui_engine.h"
#include "ui_push_button.h"
#include <esp_log.h>
#include <stdio.h>
#include <stdlib.h>
#include <u8g2.h>
#include <U8g2lib.h>
#include <string.h>

#define TAG "EGN"

#define max(a, b) (((a) > (b)) ? (a) : (b))

#define ALARM_SYMBOL           9200
#define TIMER_SYMBOL           9201
#define HOURGLASS_SYMBOL       9203
#define ARROW_LEFT_SYMBOL      9204
#define ARROW_RIGHT_SYMBOL     9205
#define ARROW_UP               9206
#define ARROW_DOWN             9207
#define PAUSE_SYMBOL           9208
#define STOP_SYMBOL            9209
#define REC_SYMBOL             9210
#define NEXT_SYMBOL            9654
#define UNCHECKED_SYMBOL       9744
#define CHECKED_SYMBOL         9745
#define CHECKED_CROSS_SYMBOL   9746

Engine::Engine(U8G2* device)
    : m_dev(device)
    , m_st {
        .menuFontSize = 14,
        .menuBoxHeight = 18,
        .minimalOffset = 4,
        .maxOffset = 5,
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

void Engine::addScreen(Screen* screen)
{
    m_screens[m_screensCount++] = screen;
    screen->calculateBounds({
        .x = 0, .y = 0,
        .w = (dim_t) m_dev->getWidth(), .h = (dim_t) m_dev->getHeight()
    }, m_st, m_dev);
};

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
    if (screen.type == element_type_t::dialog)
    {
        return renderDialog((const Dialog&) (screen));
    }
    for(int i = 0; i < screen.childrenCount(); ++i)
    {
        Element* e = screen.children()[i];
        if ((e->flags && flags_t::visible) == 0)
            continue;
        renderUnknown(e);
    }
    m_dev->setDrawColor(1);
    m_dev->drawRBox(0, 0, m_dev->getWidth(), 16, 2);
    return true;
}

bool Engine::renderDialog(const Dialog& dialog)
{
    m_dev->setDrawColor(0);

    dim_t w = dialog.bounds.w - 2 * m_st.maxOffset;
    dim_t h = dialog.bounds.h - 2 * m_st.maxOffset;

    m_dev->drawRBox(dialog.bounds.x + m_st.maxOffset, dialog.bounds.y + m_st.maxOffset,
                    w, h, m_st.maxOffset / 2);

    m_dev->setDrawColor(1);

    m_dev->drawRFrame(dialog.bounds.x + m_st.maxOffset, dialog.bounds.y + m_st.maxOffset,
                      w, h, m_st.maxOffset / 2);

    uint8_t buttonsCount = 0;

    uint8_t buttonsInfo = dialog.buttons();

    while(buttonsInfo)
    {
        if (buttonsInfo & 0x1)
            ++buttonsCount;
        buttonsInfo = buttonsInfo >> 1;
    }

    if (buttonsCount == 0)
        return true;

    dim_t spaceForButton = w / buttonsCount;

    buttonsInfo = (uint8_t) dialog.buttons();

    m_dev->setFont(m_st.small);
    m_dev->setDrawColor(2);
    m_dev->setFontPosCenter();

    uint8_t button = 0;

    Box bounds = {
        .x = (pos_t)(dialog.bounds.x + m_st.maxOffset - spaceForButton),
        .y = (pos_t)(dialog.bounds.y + dialog.bounds.h - (pos_t)( 2 * m_st.maxOffset)),
        .w = (dim_t)(spaceForButton - m_st.minimalOffset),
        .h = (dim_t)(m_st.smallHeight + 2 * m_st.minimalOffset)
    };

    bounds.y -= bounds.h;

    for(int8_t i = 0; i < buttonsCount; ++i)
    {
        for (int8_t j = button = 0; button == 0 && j < 9; ++j)
        {
            button = (1 << j) & buttonsInfo;
        }

        buttonsInfo = buttonsInfo & (~button);

        pos_t textPosY = bounds.vcenter() + 1;

        const char* txt = ((dialog_buttons_t)button == dialog_buttons_t::ok) ? "OK" : 
                          (((dialog_buttons_t)button == dialog_buttons_t::cancel) ? "Cancel" :
                           ((dialog_buttons_t)button == dialog_buttons_t::abort) ? "Abort" :
                           ((dialog_buttons_t)button == dialog_buttons_t::yes) ? "Yes" :
                           ((dialog_buttons_t)button == dialog_buttons_t::no) ? "No" :
                           "None");

        bounds.x += spaceForButton;

        m_dev->drawFrame(bounds.x + m_st.minimalOffset, bounds.y, bounds.w - m_st.minimalOffset, bounds.h);

        if (txt != nullptr)
        {
            dim_t txtW = m_dev->getUTF8Width(txt);
            m_dev->drawUTF8(bounds.hcenter() - txtW / 2 + m_st.minimalOffset / 2, textPosY, txt);
        }
    }

    if (dialog.text() != nullptr)
    {
        Label l;
        l.text = dialog.text();
        l.align_h = h_alignment_t::center;
        l.align_v = v_alignment_t::top;
        l.wrap = true;
        l.calculateAligment(Box {
            .x = (pos_t)(dialog.bounds.x + m_st.maxOffset), .y = (pos_t)(dialog.bounds.y + 2 * m_st.maxOffset),
            .w = (dim_t)(w - 2 * m_st.minimalOffset), .h = (dim_t)(h - 2 * m_st.minimalOffset - bounds.h)
        });

        l.size = text_size_t::small;

        renderLabel(l);
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
        case element_type_t::progress:
            return renderProgress(* (ProgressBar*)(element));
        case element_type_t::slider:
            return renderSlider(* (Slider*)(element));
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

    m_dev->setDrawColor(1);
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

bool Engine::renderProgress(const ProgressBar& src)
{
    m_dev->setDrawColor(1);

    if (src.flags & flags_t::selected)
    {
        m_dev->drawBox(src.bounds.x, src.bounds.y, src.bounds.w, src.bounds.h);
    }
    
    m_dev->drawFrame(src.bounds.x, src.bounds.y,
                     src.bounds.w, src.bounds.h);

    if (src.flags & flags_t::selected)
    {
        m_dev->setDrawColor(0);
    }

                        
    float range = src.maxValue() - src.minValue();
    float progress = (src.maxValue() == src.minValue())
                            ? 0.
                            : ((float)(src.value() - src.minValue()) / range);

    if (progress > 0.)
    {
        m_dev->drawBox(src.bounds.x + 1, src.bounds.y + 1,
                       src.bounds.w * progress - 1, src.bounds.h - 2);
    }

    if (src.options != progress_flags_t::bar_only)
    {
        char tmp[32];
        m_dev->setFont(m_st.small);
        m_dev->setFontMode(1);
        m_dev->setDrawColor(2);

        progress_flags_t options = src.options;
        if (src.text == nullptr && options == progress_flags_t::text_only)
            options = progress_flags_t::percentage;

        switch(options)
        {
            case progress_flags_t::percentage:
            {
                sprintf(tmp, "%d%%", (int)(progress * 100));
                break;
            }
            case progress_flags_t::text_only:
            {
                strcpy(tmp, src.text);
                break;
            }
            case progress_flags_t::values:
            {
                sprintf(tmp, "%d/%d", (int)(src.value()),
                                     (int)(src.maxValue()));
                break;
            }
            default:
                return true;
        }

        uint8_t textWidth = m_dev->getUTF8Width(tmp);
        m_dev->setFontPosCenter();
        m_dev->drawUTF8(src.bounds.hcenter() - textWidth / 2,
                        src.bounds.vcenter() + 1,
                        tmp);
    }
    return true;
}

bool Engine::renderSlider(const Slider& slider)
{
    if (slider.flags & flags_t::selected)
    {
        m_dev->setDrawColor(1);
        m_dev->drawBox(slider.bounds.x, slider.bounds.y, slider.bounds.w, slider.bounds.h);
        m_dev->setDrawColor(0);
    }

    const dim_t padding = 1;

    m_dev->drawBox(slider.bounds.x + padding , slider.bounds.y + padding, m_st.minimalOffset, slider.bounds.h - 2 * padding);
    m_dev->drawBox(slider.bounds.x + slider.bounds.w - padding - m_st.minimalOffset, slider.bounds.y + padding, m_st.minimalOffset, slider.bounds.h - 2 * padding);
    m_dev->drawHLine(slider.bounds.x + padding, slider.bounds.vcenter(), slider.bounds.w - 2 * padding);
    dim_t area = slider.bounds.w - 2 * m_st.minimalOffset - 2 * padding;
    float progress = ((float) (slider.value() - slider.minValue())) / (float)(slider.maxValue() - slider.minValue());
    dim_t offset = area * progress + m_st.minimalOffset / 2;
    m_dev->drawBox(slider.bounds.x + offset + padding,
                   slider.bounds.vcenter() - (m_st.minimalOffset / 2),
                   m_st.minimalOffset, m_st.minimalOffset);
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

    dim_t contentHeight = src.childrenCount() == 0 ? 0 : 
                          (src.children()[src.childrenCount() - 1 ]->bounds.y + src.children()[src.childrenCount() - 1 ]->bounds.h);
    contentHeight -= src.bounds.y;


    if (contentHeight > src.bounds.h)
    {
        // Rendering scrollbar
        const pos_t x = src.bounds.x + src.bounds.w - (m_st.minimalOffset + 1);
        const pos_t bottom = src.bounds.y + src.bounds.h - m_st.minimalOffset / 2;
        m_dev->drawBox(x,
                    src.bounds.y,
                    m_st.minimalOffset + 1, m_st.minimalOffset / 2);

        m_dev->drawBox(x,
                    bottom,
                    m_st.minimalOffset + 1, m_st.minimalOffset / 2);
        const pos_t y = src.bounds.y + m_st.minimalOffset / 2;
        const dim_t H0 = src.bounds.h - m_st.minimalOffset; // Visible scroll line height

        m_dev->drawVLine(x + m_st.minimalOffset / 2, y, H0);

        const dim_t H2 = H0 * H0 / contentHeight;
        const dim_t scrollHeight = ui_max(1, H2);
        const dim_t yOffset = src.renderData.yOffset * (H0 - scrollHeight) / contentHeight;
        m_dev->drawBox(x + 1, y - yOffset, ui_max(m_st.minimalOffset - 2, 3), scrollHeight);
    }

    return true;
}