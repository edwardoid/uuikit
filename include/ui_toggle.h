#pragma once

#include "ui_button_base.h"

class Toggle: public ButtonBase
{
public:
    Toggle();
    virtual ~Toggle() {}
    virtual void calculateBounds(const Box& within, const UIStyle& style, U8G2* device) override;
    virtual bool handle(const user_input_t key, const UIStyle& style) override;
    bool value;
};  