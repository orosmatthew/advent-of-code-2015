#include "../utils.hpp"

int solve(const std::string& data)
{
    int floor = 0;
    for (size_t pos = 0; pos < data.length(); ++pos) {
        switch (data[pos]) {
        case '(':
            ++floor;
            break;
        case ')':
            --floor;
            break;
        default:
            break;
        }
    }
    return floor;
}

int main()
{
    const std::string data = read_file("./day01-part1/input.txt");
#ifdef BENCHMARK
    benchmark([&] { return solve(data); }, 100000);
#else
    std::println("{}", solve(data));
#endif
}