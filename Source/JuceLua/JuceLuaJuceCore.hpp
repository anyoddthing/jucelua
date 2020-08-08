#pragma once

namespace sol {

    //==============================================================================
    // String
    //==============================================================================
    template <>
    struct lua_type_of<juce::String>
        : std::integral_constant<sol::type, sol::type::string> {};
        
    template <typename Handler>
    bool sol_lua_check(sol::types<juce::String>, lua_State* L, int index, Handler&& handler, sol::stack::record& tracking)
    {
        if (!sol::stack::check_usertype<juce::String>(L, index)
            && !sol::stack::check<sol::string_view>(L, index, handler))
        {
            handler(L, index, sol::type_of(L, index), sol::type::userdata, "expected String or string_view");
            return false;
        }
        tracking.use(1);
        return true;
    }

    static inline juce::String sol_lua_get(sol::types<juce::String>, lua_State* L, int index, sol::stack::record& tracking)
    {
        if (sol::stack::check_usertype<juce::String>(L, index))
        {
            return sol::stack::get_usertype<juce::String>(L, index, tracking);
        }
        else if (sol::stack::check<sol::string_view>(L, index))
        {
            auto sv = sol::stack::get<sol::string_view>(L, index, tracking);
            return juce::String(sv.data(), sv.size());
        }
        
        tracking.use(1);
        return {};
    }

    static inline int sol_lua_push(sol::types<juce::String>, lua_State* L, const juce::String& str)
    {
        lua_pushlstring(L, str.begin(), str.end() - str.begin());
        return 1;
    }
    
    
    //==============================================================================
    // Array<T>
    //==============================================================================
    template <typename T>
    struct is_container<juce::Array<T>> : std::true_type { };

    template <typename T>
    struct usertype_container<juce::Array<T>> {
        // Hooks Lua's syntax for #c
        static int size(lua_State* L) {
            juce::Array<T>& v = sol::stack::get<juce::Array<T>&>(L, 1);
            return stack::push(L, v.size());
        }
        
        // Used by default implementation
        static auto begin(lua_State*, juce::Array<T>& self) {
            return self.begin();
        }
        
        static auto end(lua_State*, juce::Array<T>& self) {
            return self.end();
        }
        
        static int get(lua_State* L)
        {
            auto& self = sol::stack::get<juce::Array<T>&>(L, 1);
            auto idx = stack::unqualified_get<std::ptrdiff_t>(L, 2);
            idx += index_adjustment(L, self);
            return stack::push_reference(L, detail::deref_move_only(*(self.begin() + idx)));
        }

        static int index_get(lua_State* L)
        {
            return get(L);
        }
        
        static int set(lua_State* L)
        {
            auto& self = sol::stack::get<juce::Array<T>&>(L, 1);
            auto idx = stack::unqualified_get<std::ptrdiff_t>(L, 2);
            idx += index_adjustment(L, self);
            
            auto size = self.size();
            if (idx >= size)
            {
                return luaL_error(L, "sol: index out of bounds (too big) for set on Array<T>");
            }
            
            if (idx < 0)
            {
                return luaL_error(L, "sol: index out of bounds (too small) for set on Array<T>");
            }
            
            if (self.size() - 1 == idx && sol::type_of(L, 3) == type::lua_nil)
            {
                return erase(L);
            }
            
            self.set(static_cast<int>(idx), stack::unqualified_get<T>(L, 3));
            return 0;
        }

        static int index_set(lua_State* L)
        {
            return set(L);
        }
            
        static int erase(lua_State* L)
        {
            auto& self = sol::stack::get<juce::Array<T>&>(L, 1);
            auto idx = stack::unqualified_get<std::ptrdiff_t>(L, 2);
            idx += index_adjustment(L, self);
            
            auto size = self.size();
            if (idx >= size)
            {
                return luaL_error(L, "sol: index out of bounds (too big) for erase on Array<T>");
            }
            
            if (idx < 0)
            {
                return luaL_error(L, "sol: index out of bounds (too small) for erase on Array<T>");
            }
            
            self.remove(static_cast<int>(idx));
            return 0;
        }
        
        static std::ptrdiff_t index_adjustment(lua_State*, juce::Array<T>&)
        {
            return -1;
        }
    };
} // namespace sol

namespace aot { namespace lua {

    static inline void openCoreLibrary(sol::state& lua)
    {
        auto string = lua.new_usertype<juce::String>("String",
            sol::constructors<juce::String(const char *), juce::String(std::string&)>()
        );        
    }
    
} }
