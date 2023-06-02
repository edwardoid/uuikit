#include "ui_progress_bar.h"

#include <u8g2.h>
#include <U8g2lib.h>

ProgressBar::ProgressBar()
{
    type = element_type_t::progress;
}

void ProgressBar::calculateBounds(const Box& within, const UIStyle& style, U8G2* device)
{
    bounds.x = within.x;
    bounds.y = within.y;
    bounds.w = style.minimalOffset;
    bounds.h = style.minimalOffset;

    if (options == progress_flags_t::bar_only)
    {
        bounds.h = 4;
        bounds.w = within.w - 2;
    } else
    {
        bounds.h = style.smallHeight + 2;
        bounds.w = within.w - 2;
    }
}