#include "../utils.hpp"

#include <unordered_set>

struct Vector2 {
    int x;
    int y;

    bool operator==(const Vector2& vector) const
    {
        return x == vector.x && y == vector.y;
    }

    struct Hash {
        size_t operator()(const Vector2& vector) const noexcept
        {
            // Based on Boost's hash_combine.
            size_t seed = std::hash<int>()(vector.x);
            seed ^= std::hash<int>()(vector.y) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            return seed;
        }
    };
};

static int solve(const std::string& data)
{
    std::unordered_set<Vector2, Vector2::Hash> visited;
    Vector2 current_pos { 0, 0 };
    visited.insert(current_pos);
    for (size_t pos = 0; pos < data.length(); ++pos) {
        if (data[pos] == '^') {
            ++current_pos.y;
        } else if (data[pos] == 'v') {
            --current_pos.y;
        } else if (data[pos] == '>') {
            ++current_pos.x;
        } else if (data[pos] == '<') {
            --current_pos.x;
        } else {
            continue;
        }
        visited.insert(current_pos);
    }
    return visited.size();
}

int main()
{
    const std::string data = read_file("./day03-part1/input.txt");
#ifdef BENCHMARK
    benchmark([&] { return solve(data); }, 1000);
#else
    std::println("{}", solve(data));
#endif
}