#include "StandardTest.hpp"

namespace
{
sol::state initLua()
{
    sol::state lua;
    lua.open_libraries(sol::lib::base);
    aot::lua::openCoreLibrary(lua);
    
    lua.safe_script("function p(a) return a end");
    lua.safe_script("function g() return val end");
    return lua;
}
}

TEST_CASE("Array Tests")
{
    SECTION("can read array value")
    {
        auto lua = initLua();
        Array<int> ints = {0,2,4};
        
        lua["val"] = ints;
        lua.safe_script("result = val[1]");
        
        REQUIRE(lua["result"] == 0);
    }
    
    SECTION("can set simple array value")
    {
        auto lua = initLua();
        Array<int> ints = {0,2,4};
        
        lua["val"] = &ints;
        lua.safe_script("val[1] = 42");
        
        REQUIRE(ints[0] == 42);
    }
    
    SECTION("can set string array value")
    {
        auto lua = initLua();
        Array<std::string> strings = {"0","2","4"};
        
        lua["val"] = &strings;
        lua.safe_script("val[1] = \"42\"");
        
        REQUIRE(strings[0] == "42");
    }
    
    SECTION("will move value in array")
    {
        auto lua = initLua();
        Array<MoveableType<>> ints = {0,2,4};
        
        lua["val"] = &ints;
        lua["i"] = MoveableType<> {42};
        lua.safe_script("val[1] = i");
        
        REQUIRE(ints.getReference(0).value == 42);
    }

}
