#pragma once

#define CATCH_CONFIG_ENABLE_BENCHMARKING 1

#include "../JuceLibraryCode/JuceHeader.h"
#include <catch2/catch.hpp>

#define DEBUG_MOVEABLETYPE 0


template <typename T = int>
struct MoveableType
{
    T value;
    int moves = 0;
    int copies = 0;
    
    MoveableType(T v = {})
    {
        value = v;

#if DEBUG_MOVEABLETYPE
        std::cout << "MoveableType ctor: " << typeid(T).name()
            << " value: " << value
            << " moves: " << moves
            << " copies: " << copies
            << "\n";
#endif
    }
    
    ~MoveableType() noexcept
    {
#if DEBUG_MOVEABLETYPE
        std::cout << "MoveableType dtor: " << typeid(T).name()
            << " value: " << value
            << " moves: " << moves
            << " copies: " << copies
            << "\n";
#endif
    }
    
    MoveableType(const MoveableType& other)
    {
        value = other.value;
        copies += other.copies;
        moves += other.moves;

#if DEBUG_MOVEABLETYPE
        std::cout << "MoveableType copy ctor: " << typeid(T).name()
            << " value: " << value
            << " moves: " << moves
            << " copies: " << copies
            << "\n";
#endif
    }
    
    MoveableType(MoveableType&& other) noexcept
    {
        moves += other.moves + 1;
        copies += other.copies;
        value = other.value;

#if DEBUG_MOVEABLETYPE
        std::cout << "MoveableType move ctor: " << typeid(T).name()
            << " value: " << value
            << " moves: " << moves
            << " copies: " << copies
            << "\n";
#endif
    }
    
    MoveableType& operator=(const MoveableType& other)
    {
        value = other.value;
        moves += other.moves;
        copies += other.copies + 1;

#if DEBUG_MOVEABLETYPE
        std::cout << "MoveableType copy assign: " << typeid(T).name()
            << " value: " << value
            << " moves: " << moves
            << " copies: " << copies
            << "\n";
#endif
        return *this;
    }
    
    MoveableType& operator=(MoveableType&& other)
    {
        moves += other.moves + 1;
        copies += other.copies;
        std::swap(value, other.value);

#if DEBUG_MOVEABLETYPE
        std::cout << "MoveableType move assign: " << typeid(T).name()
            << " value: " << value
            << " moves: " << moves
            << " copies: " << copies
            << "\n";
#endif
        return *this;
    }
};
