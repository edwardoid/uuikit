#pragma once

#include "ui_pixmap.h"
#include "ui_element.h"
#include "ui_label.h"
#include <string>

struct PushButton: public Element
{
    PushButton();
    virtual void calculateBounds(const Box& within, const UIStyle& style, U8G2* device) override;
    const char* text = nullptr;
    Pixmap* icon = nullptr;
    Label*  label = nullptr;
};