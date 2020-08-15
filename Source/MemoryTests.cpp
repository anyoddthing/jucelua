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

    std::vector<uintptr_t> lifeObjects;
    std::vector<uintptr_t> releasedObjects;
    
    struct Bookkeeper
    {
        Bookkeeper()
        {
            std::cout << "Bookkeeper ctor: " << getID() << std::endl;
            lifeObjects.push_back(getID());
        }
        
        Bookkeeper(const Bookkeeper&)
        {
            std::cout << "Bookkeeper cctor: " << getID() << std::endl;
            lifeObjects.push_back(getID());
        }

        Bookkeeper(Bookkeeper&&) noexcept
        {
            std::cout << "Bookkeeper mctor: " << getID() << std::endl;
            lifeObjects.push_back(getID());
        }
        
        virtual ~Bookkeeper()
        {
            std::cout << "Bookkeeper dtor: " << getID() << std::endl;
            auto iter = std::find(lifeObjects.begin(), lifeObjects.end(), getID());
            if (iter != lifeObjects.end()) lifeObjects.erase(iter);
            
            releasedObjects.push_back(getID());
        }
        
        uintptr_t getID() const
        {
            return reinterpret_cast<uintptr_t>(this);
        }
    };
}

TEST_CASE("Memory")
{
    SECTION("Object gets released")
    {
//        static_assert(std::is_trivially_move_constructible_v<Bookkeeper>, "");
        
        auto lua = initLua();
        
        auto bookkeeper = lua.new_usertype<Bookkeeper>("Bookkeeper",
            sol::call_constructor, sol::constructors<Bookkeeper()>());

        bookkeeper[sol::meta_function::to_string] = [](const Bookkeeper& it) {
            return "Bookkeeper: " + std::to_string(it.getID());
        };
        bookkeeper[sol::meta_function::garbage_collect] = [] (const Bookkeeper& it) {
            std::cout <<  "Collecting Bookkeeper: " + std::to_string(it.getID()) << std::endl;
            it.~Bookkeeper();
        };
        
//        REQUIRE(lua["bookkeeper"] == nullptr);
        
//        lua.safe_script("bookkeeper = Bookkeeper()");
//        lua["bookkeeper"] = std::make_unique<Bookkeeper>();
        lua["bookkeeper"] = Bookkeeper{};
        
        REQUIRE(lifeObjects.size() == 1);
        REQUIRE(lua["bookkeeper"] != nullptr);
        
        lua.safe_script("bookkeeper = nil");
        std::cout << "Start GC \n";
        lua.collect_garbage();
        std::cout << "End GC \n";
        REQUIRE(lifeObjects.size() == 0);
    }
}

