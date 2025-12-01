#include "../utils.hpp"

#include <array>
#include <algorithm>

static std::array<int, 3> parse_dimensions(const std::string& data, size_t& pos)
{
    const int length = parse_int(data, pos);
    ++pos; // "x"
    const int width = parse_int(data, pos);
    ++pos; // "x"
    const int height = parse_int(data, pos);
    return std::array<int, 3> { length, width, height };
}

static int calc_surface_area(const std::array<int, 3>& dimensions)
{
    const std::array<int, 3>& d = dimensions;
    return 2 * d[0] * d[1] + 2 * d[1] * d[2] + 2 * d[2] * d[0];
}

static int min_side_area(std::array<int, 3>& dimensions)
{
    std::ranges::sort(dimensions);
    return dimensions[0] * dimensions[1];
}

static int solve(const std::string& data)
{
    int total = 0;
    for (size_t pos = 0; pos < data.length(); ++pos) {
        std::array<int, 3> dimensions = parse_dimensions(data, pos);
        const int surface_area = calc_surface_area(dimensions);
        const int extra = min_side_area(dimensions);
        total += surface_area + extra;
    }
    return total;
}

int main()
{
    const std::string data = read_file("./day02-part1/input.txt");
#ifdef BENCHMARK
    benchmark([&] { return solve(data); }, 100000);
#else
    std::println("{}", solve(data));
#endif
}