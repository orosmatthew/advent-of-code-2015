#include <cassert>
#include <utils.hpp>

static void look_say_step(const std::string& input, std::string& output)
{
    output.clear();
    std::optional<char> prev;
    int repeat = 1;
    auto push_output = [&] {
        assert(prev.has_value());
        assert(repeat <= 9);
        output.push_back(repeat + '0');
        output.push_back(*prev);
    };
    for (size_t i = 0; i < input.size(); ++i) {
        const char c = input[i];
        if (prev.has_value()) {
            if (c == *prev) {
                ++repeat;
            } else {
                push_output();
                repeat = 1;
            }
        }
        prev = c;
    }
    push_output();
}

static size_t solve(const std::string& data)
{
    std::string input;
    input.clear();
    for (int pos = 0; pos < data.size() && data[pos] != '\n'; ++pos) {
        input.push_back(data[pos]);
    }
    std::string result = std::move(input);
    std::string next;
    for (int i = 0; i < 40; ++i) {
        look_say_step(result, next);
        std::swap(result, next);
    }
    return result.size();
}

int main()
{
    const std::string data = read_file("./day10-part1/input.txt");
#ifdef BENCHMARK
    benchmark([&] { return solve(data); }, 1000);
#else
    std::println("{}", solve(data));
#endif
}