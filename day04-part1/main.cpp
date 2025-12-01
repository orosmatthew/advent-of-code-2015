#include <utils.hpp>

#include <md5.h>

#include <format>

static std::string parse_key(const std::string& data)
{
    std::string key;
    for (size_t i = 0; i < data.length(); ++i) {
        if (data[i] == '\n') {
            break;
        }
        key.push_back(data[i]);
    }
    return key;
}

static bool starts_with_5_zeros(const std::string& string)
{
    for (size_t i = 0; i < 5; ++i) {
        if (string[i] != '0') {
            return false;
        }
    }
    return true;
}

static int solve(const std::string& data)
{
    const std::string key = parse_key(data);
    MD5 md5;
    int count = 0;
    while (true) {
        const std::string input = std::format("{}{}", key, count);
        if (const std::string hash = md5(input); starts_with_5_zeros(hash)) {
            break;
        }
        ++count;
    }
    return count;
}

int main()
{
    const std::string data = read_file("./day04-part1/input.txt");
#ifdef BENCHMARK
    benchmark([&] { return solve(data); }, 50);
#else
    std::println("{}", solve(data));
#endif
}