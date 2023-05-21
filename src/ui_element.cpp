#include "ui_element.h"

void Element::calculateAligment(const Box& within)
{
    switch (align_h)
    {
        case h_alignment_t::left:
        {
            bounds.x = within.x;
            break;
        }

        case h_alignment_t::right:
        {
            bounds.x = within.x + (within.w - bounds.w);
            break;
        }

        case h_alignment_t::center:
        {
            bounds.x = within.hcenter() - bounds.hcenter() + within.x;
            break;
        }
    }

    switch (align_v)
    {
        case v_alignment_t::top:
        {
            bounds.y = within.y;
            break;
        }

        case v_alignment_t::bottom:
        {
            bounds.y = within.y + (within.h - bounds.h);
            break;
        }

        case v_alignment_t::center:
        {
            bounds.y = within.vcenter() - bounds.vcenter() + within.y;
            break;
        }
    }
}