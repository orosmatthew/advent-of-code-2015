#include <utils.hpp>

int solve(const std::string& data)
{
    int count = 0;
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
        ++count;
        if (floor == -1) {
            break;
        }
    }
    return count;
}

int main()
{
    const std::string data = read_file("./day01-part2/input.txt");
#ifdef BENCHMARK
    benchmark([&] { return solve(data); }, 1000000);
#else
    std::println("{}", solve(data));
#endif
}