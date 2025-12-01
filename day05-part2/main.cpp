#include <utils.hpp>

static bool is_nice(const std::string& string)
{
    bool found = false;
    for (size_t i = 2; i < string.length(); ++i) {
        if (string[i] == string[i - 2]) {
            found = true;
            break;
        }
    }
    if (!found) {
        return false;
    }
    for (size_t i = 1; i < string.length() - 2; ++i) {
        for (size_t j = i + 2; j < string.length(); ++j) {
            if (string[i - 1] == string[j - 1] && string[i] == string[j]) {
                return true;
            }
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
    const std::string data = read_file("./day05-part2/input.txt");
#ifdef BENCHMARK
    benchmark([&] { return solve(data); }, 100000);
#else
    std::println("{}", solve(data));
#endif
}