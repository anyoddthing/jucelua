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

        REQUIRE(lua["i"].get<MoveableType<int>&>().value == 42);
        REQUIRE(lua["i"].get<MoveableType<int>&>().copies == 0);
        REQUIRE(lua["i"].get<MoveableType<int>&>().moves == 1);

        lua.safe_script("val[1] = i");

        REQUIRE(ints.getReference(0).copies == 1);
        REQUIRE(ints.getReference(0).moves == 1);
    }
    
    SECTION("can loop over array via for loop")
    {
        auto lua = initLua();
        Array<std::string> strings = {"0","2","4"};
        
        lua["strings"] = strings;
        
        lua.safe_script(R"(
            result = {}
            for k = 1, #strings do
                result[k] = strings[k]
            end
        )");
        
        auto result = lua.get<sol::table>("result");
        
        REQUIRE(result.size() == 3);
        REQUIRE(result.get<std::string>(3) == "4");
    }

    SECTION("can loop over array via ipairs")
    {
        auto lua = initLua();
        Array<std::string> strings = {"0","2","4"};
        
        lua["strings"] = strings;
        
        lua.safe_script(R"(
            result = {}
            for k,v in ipairs(strings) do
                result[k] = v
            end
        )");
        
        auto result = lua.get<sol::table>("result");
        
        REQUIRE(result.size() == 3);
        REQUIRE(result.get<std::string>(3) == "4");
    }
    
    SECTION("can loop over array via pairs")
    {
        auto lua = initLua();
        Array<std::string> strings = {"0","2","4"};
        
        lua["strings"] = strings;
        
        lua.safe_script(R"(
            result = {}
            for k,v in pairs(strings) do
                result[k] = v
            end
        )");
        
        auto result = lua.get<sol::table>("result");
        
        REQUIRE(result.size() == 3);
        REQUIRE(result.get<std::string>(3) == "4");
    }

}
