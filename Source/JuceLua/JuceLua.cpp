/*
 * Wrapper for sol2 / lua libs for easy drop-in integration in JUCE projects
 *
 * Author: 	Daniel Doubleday
 * Version:	1.0
 * Date:	September 2018
 * 
 * This file is in the public domain and can be used without
 * any restriction.
 */

#include "JuceLua.h"

#include "../../Libs/lua/lapi.c"
#include "../../Libs/lua/lauxlib.c"
#include "../../Libs/lua/lbaselib.c"
#include "../../Libs/lua/lcode.c"
#include "../../Libs/lua/lcorolib.c"
#include "../../Libs/lua/lctype.c"
#include "../../Libs/lua/ldblib.c"
#include "../../Libs/lua/ldebug.c"
#include "../../Libs/lua/ldo.c"
#include "../../Libs/lua/ldump.c"
#include "../../Libs/lua/lfunc.c"
#include "../../Libs/lua/lgc.c"
#include "../../Libs/lua/linit.c"
#include "../../Libs/lua/liolib.c"
#include "../../Libs/lua/llex.c"
#include "../../Libs/lua/lmathlib.c"
#include "../../Libs/lua/lmem.c"
#include "../../Libs/lua/loadlib.c"
#include "../../Libs/lua/lobject.c"
#include "../../Libs/lua/lopcodes.c"
#include "../../Libs/lua/loslib.c"
#include "../../Libs/lua/lparser.c"
#include "../../Libs/lua/lstate.c"
#include "../../Libs/lua/lstring.c"
#include "../../Libs/lua/lstrlib.c"
#include "../../Libs/lua/ltable.c"
#include "../../Libs/lua/ltablib.c"
#include "../../Libs/lua/ltests.c"
#include "../../Libs/lua/ltm.c"
#include "../../Libs/lua/lundump.c"
#include "../../Libs/lua/lutf8lib.c"
#include "../../Libs/lua/lvm.c"
#include "../../Libs/lua/lzio.c"

namespace aot { namespace lua {

#include "JuceLuaJuceCore.cpp"

} }

