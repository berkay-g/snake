#pragma once

#include <utility>

struct Vec2i
{
    int x, y;

    Vec2i operator+(Vec2i const& other)
    {
        return Vec2i{x + other.x, y + other.y};
    }

    bool operator==(Vec2i const& other)
    {
        return (x == other.x) && (y == other.y);
    }

    bool operator!=(Vec2i const& other)
    {
        return (x != other.x) && (y != other.y);
    }
};

constexpr int get1DIndex(int i, int j, int num_columns)
{
    return i * num_columns + j;
}

constexpr std::pair<int, int> get2DIndex(int index, int num_columns)
{
    int i = index / num_columns;
    int j = index % num_columns;
    return {i, j};
}