// No License, this is entirely open source!
// Software for learning purposes.
// Author: Mario Garcia
#pragma once

#include "Common/Types.hpp"
#include "Display/Window.hpp"

namespace Synthe {



void InitializeSystem();

Window* CreateAppWindow();
void DestroyAppWindow(Window* PWindow);

void PollEvents();
} // Synthe