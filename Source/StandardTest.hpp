#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <catch2/catch.hpp>

template <typename T = int>
struct MoveableType
{
    T value;
    int moves = 0;
    int copies = 0;
    
    MoveableType(T v = {})
    {
        value = v;
        
        std::cout << "MoveableType ctor: " << typeid(T).name()
            << " value: " << value
            << " moves: " << moves
            << " copies: " << copies
            << "\n";
    }
    
    ~MoveableType() noexcept
    {
        std::cout << "MoveableType dtor: " << typeid(T).name()
            << " value: " << value
            << " moves: " << moves
            << " copies: " << copies
            << "\n";
    }
    
    MoveableType(const MoveableType& other)
    {
        value = other.value;
        copies += other.copies;
        moves += other.moves;

        std::cout << "MoveableType copy ctor: " << typeid(T).name()
            << " value: " << value
            << " moves: " << moves
            << " copies: " << copies
            << "\n";
    }
    
    MoveableType(MoveableType&& other) noexcept
    {
        moves += other.moves + 1;
        copies += other.copies;
        value = other.value;
        
        std::cout << "MoveableType move ctor: " << typeid(T).name()
            << " value: " << value
            << " moves: " << moves
            << " copies: " << copies
            << "\n";
    }
    
    MoveableType& operator=(const MoveableType& other)
    {
        value = other.value;
        moves += other.moves;
        copies += other.copies + 1;
        
        std::cout << "MoveableType copy assign: " << typeid(T).name()
            << " value: " << value
            << " moves: " << moves
            << " copies: " << copies
            << "\n";

        return *this;
    }
    
    MoveableType& operator=(MoveableType&& other)
    {
        moves += other.moves + 1;
        copies += other.copies;
        std::swap(value, other.value);
        
        std::cout << "MoveableType move assign: " << typeid(T).name()
            << " value: " << value
            << " moves: " << moves
            << " copies: " << copies
            << "\n";

        return *this;
    }
};
