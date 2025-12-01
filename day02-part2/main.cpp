#include "../utils.hpp"

#include <algorithm>
#include <array>

static std::array<int, 3> parse_dimensions(const std::string& data, size_t& pos)
{
    const int length = parse_int(data, pos);
    ++pos; // "x"
    const int width = parse_int(data, pos);
    ++pos; // "x"
    const int height = parse_int(data, pos);
    return std::array { length, width, height };
}

static int solve(const std::string& data)
{
    int total = 0;
    for (size_t pos = 0; pos < data.length(); ++pos) {
        std::array<int, 3> dimensions = parse_dimensions(data, pos);
        std::ranges::sort(dimensions);
        const int ribbon_wrap = dimensions[0] * 2 + dimensions[1] * 2;
        const int ribbon_bow = dimensions[0] * dimensions[1] * dimensions[2];
        total += ribbon_wrap + ribbon_bow;
    }
    return total;
}

int main()
{
    const std::string data = read_file("./day02-part2/input.txt");
#ifdef BENCHMARK
    benchmark([&] { return solve(data); }, 100000);
#else
    std::println("{}", solve(data));
#endif
}