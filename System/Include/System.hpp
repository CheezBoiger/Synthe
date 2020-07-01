// No License, this is entirely open source!
// Software for learning purposes.
// Author: Mario Garcia
#pragma once

#include "Common/Types.hpp"
#include "Display/Window.hpp"

namespace Synthe {


//! Initialize the operating system for this application.
void InitializeSystem();

//! Create an app window.
Window* CreateAppWindow();

//! Destroy an app window.
void DestroyAppWindow(Window* PWindow);

//! Poll events that are registered to dispatch callbacks to the Operating system.
//! This should be called every iteration of a loop.
void PollEvents();
} // Synthe