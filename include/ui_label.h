#pragma once

#include "ui_types.h"
#include "ui_element.h"



class Label: public Element
{
public:
    Label();
    const char* text = nullptr;
    bool        wrap = false;
    h_alignment_t align = h_alignment_t::left;
    text_size_t size = text_size_t::medium;
    virtual void calculateBounds(const Box& within, const UIStyle& style, U8G2* device) override;
};