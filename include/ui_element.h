#pragma once

#include "ui_types.h"

#define ui_max(a, b) ( (a) > (b) ? (a) : (b) )
#define ui_min(a, b) ( (a) < (b) ? (b) : (a) )

enum element_type_t: uint8_t
{
    unknown = 0,
    label,
    pixmap,
    button,
    toggle,
    progress,
    slider,
    menu,
    dialog,
    scrollbar
};

enum flags_t: uint8_t
{
    visible = 0x01,
    enabled = 0x02,
    selected = 0x04,
    editing = 0x08
};

enum class h_alignment_t: uint8_t
{
    left = 0,
    right,
    center
};

enum class v_alignment_t: uint8_t
{
    top = 0,
    bottom,
    center
};

struct Box
{
    pos_t x = 0, y = 0;
    dim_t w = 0, h = 0;
    pos_t hcenter() const { return x + w/2; }
    pos_t vcenter() const { return y + h/2; }
    void map(dim_t dx, dim_t dy) { x += dx; y += dy; }
};

struct UIStyle
{
    dim_t menuFontSize;
    dim_t menuBoxHeight;
    dim_t minimalOffset;
    dim_t maxOffset;
    int8_t toggleSize;
    const uint8_t  *regular;
    dim_t regularHeight = 0;
    const uint8_t  *small;
    dim_t smallHeight = 0;
    const uint8_t  *symbols;
    dim_t symbolsHeight = 0;
};

class Element
{
public:
    Element() = default;
    virtual ~Element() = default;
    h_alignment_t align_h = h_alignment_t::left;
    v_alignment_t align_v = v_alignment_t::center;

    element_type_t type;
    uint8_t flags = flags_t::visible | flags_t::enabled;
    Box bounds;
    inline void select(bool select)
    {
        flags = select
                    ? ( flags | flags_t::selected )
                    : ( flags & ~(flags_t::selected));
    }
    virtual bool handle(const user_input_t key, const UIStyle& style) { return false; }
    virtual void calculateBounds(const Box& within, const UIStyle& style, struct U8G2* device) { bounds = within; }
    virtual void calculateAligment(const Box& within);
};