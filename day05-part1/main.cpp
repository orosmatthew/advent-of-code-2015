#include <utils.hpp>

#include <array>

static bool is_nice(const std::string& string)
{
    for (constexpr std::array forbidden { "ab", "cd", "pq", "xy" }; const std::string& word : forbidden) {
        if (string.contains(word)) {
            return false;
        }
    }
    int vowel_count = 0;
    for (const char c : string) {
        if (constexpr std::array vowels { 'a', 'e', 'i', 'o', 'u' }; std::ranges::contains(vowels, c)) {
            ++vowel_count;
        }
        if (vowel_count >= 3) {
            break;
        }
    }
    if (vowel_count < 3) {
        return false;
    }
    for (size_t i = 1; i < string.length(); ++i) {
        if (string[i] == string[i - 1]) {
            return true;
        }
    }
    return false;
}

static int solve(const std::string& data)
{
    int nice_count = 0;
    std::string string;
    for (size_t pos = 0; pos < data.length(); ++pos) {
        string.clear();
        while (data[pos] != '\n') {
            string.push_back(data[pos]);
            ++pos;
        }
        if (is_nice(string)) {
            ++nice_count;
        }
    }
    return nice_count;
}

int main()
{
    const std::string data = read_file("./day05-part1/input.txt");
#ifdef BENCHMARK
    benchmark([&] { return solve(data); }, 10000);
#else
    std::println("{}", solve(data));
#endif
}