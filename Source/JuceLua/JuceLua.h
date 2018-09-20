#pragma once

#include <juce_core/juce_core.h>
#include <juce_graphics/juce_graphics.h>
#include <juce_gui_basics/juce_gui_basics.h>

/*******************************************************************************
 The block below describes the properties of this module, and is read by
 the Projucer to automatically generate project code that uses it.
 For details about the syntax and how to create or use a module, see the
 JUCE Module Format.txt file.


 BEGIN_JUCE_MODULE_DECLARATION

  ID:               JuceLua
  vendor:           daniel@anyoddthing.com
  version:          0.0.1
  name:             JuceLua
  description:      Wrapper for sol2 / lua libs for easy drop-in integration in JUCE projects
  website:          http://www.anyoddthing.com
  license:          BSD
  searchpaths:      ../../Libs/lua ../../Libs/sol2/single/sol

 END_JUCE_MODULE_DECLARATION

*******************************************************************************/

/* -------------------------------------------------------- */

#define SOL_USING_CXX_LUA 1

#include "sol.hpp"
