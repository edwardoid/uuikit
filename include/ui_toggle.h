#pragma once

#include "ui_push_button.h"

class Toggle: public PushButton
{
public:
    Toggle();
    virtual ~Toggle() {}
    virtual void calculateBounds(const Box& within, const UIStyle& style, U8G2* device) override;
    virtual bool handle(const user_input_t key) override;
    bool value;
};  