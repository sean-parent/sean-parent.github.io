void PanelBar::RepositionExpandedPanels(Panel* fixed_panel) {
    CHECK(fixed_panel);

    // First, find the index of the fixed panel.
    int fixed_index = GetPanelIndex(expanded_panels_, *fixed_panel);
    CHECK_LT(fixed_index, expanded_panels_.size());

    // Next, check if the panel has moved to the other side of another panel.
    const int center_x = fixed_panel->cur_panel_center();
    for (size_t i = 0; i < expanded_panels_.size(); ++i) {
        Panel* panel = expanded_panels_[i].get();
        if (center_x <= panel->cur_panel_center() || i == expanded_panels_.size() - 1) {
            if (panel != fixed_panel) {
                // If it has, then we reorder the panels.
                ref_ptr<Panel> ref = expanded_panels_[fixed_index];
                expanded_panels_.erase(expanded_panels_.begin() + fixed_index);
                if (i < expanded_panels_.size()) {
                    expanded_panels_.insert(expanded_panels_.begin() + i, ref);
                } else {
                    expanded_panels_.push_back(ref);
                }
            }
            break;
        }
    }

    // Find the total width of the panels to the left of the fixed panel.
    int total_width = 0;
    fixed_index = -1;
    for (int i = 0; i < static_cast<int>(expanded_panels_.size()); ++i) {
        Panel* panel = expanded_panels_[i].get();
        if (panel == fixed_panel) {
            fixed_index = i;
            break;
        }
        total_width += panel->panel_width();
    }
    CHECK_NE(fixed_index, -1);
    int new_fixed_index = fixed_index;

    // Move panels over to the right of the fixed panel until all of the ones
    // on the left will fit.
    int avail_width = max(fixed_panel->cur_panel_left() - kBarPadding, 0);
    while (total_width > avail_width) {
        new_fixed_index--;
        CHECK_GE(new_fixed_index, 0);
        total_width -= expanded_panels_[new_fixed_index]->panel_width();
    }

    // Reorder the fixed panel if its index changed.
    if (new_fixed_index != fixed_index) {
        Panels::iterator it = expanded_panels_.begin() + fixed_index;
        ref_ptr<Panel> ref = *it;
        expanded_panels_.erase(it);
        expanded_panels_.insert(expanded_panels_.begin() + new_fixed_index, ref);
        fixed_index = new_fixed_index;
    }

    // Now find the width of the panels to the right, and move them to the
    // left as needed.
    total_width = 0;
    for (Panels::iterator it = expanded_panels_.begin() + fixed_index + 1;
         it != expanded_panels_.end(); ++it) {
        total_width += (*it)->panel_width();
    }

    avail_width = max(wm_->width() - (fixed_panel->cur_right() + kBarPadding), 0);
    while (total_width > avail_width) {
        new_fixed_index++;
        CHECK_LT(new_fixed_index, expanded_panels_.size());
        total_width -= expanded_panels_[new_fixed_index]->panel_width();
    }

    // Do the reordering again.
    if (new_fixed_index != fixed_index) {
        Panels::iterator it = expanded_panels_.begin() + fixed_index;
        ref_ptr<Panel> ref = *it;
        expanded_panels_.erase(it);
        expanded_panels_.insert(expanded_panels_.begin() + new_fixed_index, ref);
        fixed_index = new_fixed_index;
    }

    // Finally, push panels to the left and the right so they don't overlap.
    int boundary = expanded_panels_[fixed_index]->cur_panel_left() - kBarPadding;
    for (Panels::reverse_iterator it =
             // Start at the panel to the left of 'new_fixed_index'.
         expanded_panels_.rbegin() + (expanded_panels_.size() - new_fixed_index);
         it != expanded_panels_.rend(); ++it) {
        Panel* panel = it->get();
        if (panel->cur_right() > boundary) {
            panel->Move(boundary, kAnimMs);
        } else if (panel->cur_panel_left() < 0) {
            panel->Move(min(boundary, panel->panel_width() + kBarPadding), kAnimMs);
        }
        boundary = panel->cur_panel_left() - kBarPadding;
    }

    boundary = expanded_panels_[fixed_index]->cur_right() + kBarPadding;
    for (Panels::iterator it = expanded_panels_.begin() + new_fixed_index + 1;
         it != expanded_panels_.end(); ++it) {
        Panel* panel = it->get();
        if (panel->cur_panel_left() < boundary) {
            panel->Move(boundary + panel->panel_width(), kAnimMs);
        } else if (panel->cur_right() > wm_->width()) {
            panel->Move(max(boundary + panel->panel_width(), wm_->width() - kBarPadding),
                        kAnimMs);
        }
        boundary = panel->cur_right() + kBarPadding;
    }
}
