#pragma once

#include "ui_push_button.h"

struct Toggle: public PushButton
{
    Toggle();
    virtual void calculateBounds(const Box& within, const UIStyle& style, U8G2* device) override;
    bool value = false;
};