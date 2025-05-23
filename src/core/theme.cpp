#include "core/theme.h"
Theme& Theme::instance() {
    static Theme theme;
    return theme;
}