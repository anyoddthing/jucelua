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

template <typename T>
T passThrough(T t)
{
    return t;
}

}

TEST_CASE("juce::String shim")
{
    SECTION("std::string can be converted to all strings")
    {
        auto lua = initLua();
        lua["val"] = std::string {"val"};
        
        sol::function get = lua["g"];
        
        REQUIRE(get().get<std::string>() == std::string {"val"});
        REQUIRE(get().get<String>() == "val");
    }
    
    SECTION("juce String can be converted to all strings")
    {
        auto lua = initLua();
        lua["val"] = String {"val"};
        
        sol::function get = lua["g"];
        
        REQUIRE(get().get<std::string>() == std::string {"val"});
        REQUIRE(get().get<String>() == "val");
    }
    
    SECTION("juce String is a lua string")
    {
        auto lua = initLua();
        lua["val"] = String {"val"};
        
        REQUIRE(lua["val"].get_type() == sol::type::string);
        
        sol::function get = lua["g"];
        REQUIRE(get().get_type() == sol::type::string);
        
        sol::function p = lua["p"];
        REQUIRE(p(String {"test"}).get_type() == sol::type::string);
    }

    SECTION("can call functions with std::string")
    {
        auto lua = initLua();
        lua["juce_string"] = String {"val"};
        lua["std_string"] = std::string {"val"};
        
        lua["cp"] = passThrough<std::string>;
        lua.safe_script("juce_result = cp(juce_string)");
        lua.safe_script("std_result = cp(std_string)");
        
        REQUIRE(lua["juce_result"] == String {"val"} );
        REQUIRE(lua["std_result"] == std::string {"val"} );
    }
    
    SECTION("can call functions with juce::String")
    {
        auto lua = initLua();
        lua["juce_string"] = String {"val"};
        lua["std_string"] = std::string {"val"};
        
        lua["cp"] = passThrough<String>;
        lua.safe_script("juce_result = cp(juce_string)");
        lua.safe_script("std_result = cp(std_string)");
        
        REQUIRE(lua["juce_result"] == String {"val"} );
        REQUIRE(lua["std_result"] == std::string {"val"} );
    }

}
