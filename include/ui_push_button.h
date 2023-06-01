#pragma once

#include "ui_button_base.h"
#include <functional>

class PushButton: public ButtonBase
{
public:
    PushButton();
    virtual bool handle(const user_input_t key, const UIStyle& style) override;
    std::function<void(void)> on_click;
};