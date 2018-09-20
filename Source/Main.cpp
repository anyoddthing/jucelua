/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a JUCE application.

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
int main (int argc, char* argv[])
{
    sol::state lua;
    int x = 0;
    lua.open_libraries(sol::lib::base, sol::lib::package);
    lua.set_function("beep", [&x]{ ++x; });
    lua.script("beep()");
    lua.script("print('hello world :-)')");
    assert(x == 1);
    
    return 0;
}
