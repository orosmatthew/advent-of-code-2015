#include <utils.hpp>

// ReSharper disable CppTooWideScopeInitStatement

static void parse_line(const std::string& data, int& pos, std::string& line)
{
    line.clear();
    while (data[pos] != '\n') {
        line.push_back(data[pos]);
        ++pos;
    }
}

static int count_encoded_chars(const std::string& string)
{
    int count = 2;
    for (int i = 0; i < string.size(); ++i) {
        const char c = string[i];
        if (c == '\"' || c == '\\') {
            ++count;
        }
        ++count;
    }
    return count;
}

static int solve(const std::string& data)
{
    static std::string line;
    int total_source_count = 0;
    int total_encoded_count = 0;
    for (int pos = 0; pos < data.size(); ++pos) {
        parse_line(data, pos, line);
        total_source_count += line.size();
        total_encoded_count += count_encoded_chars(line);
    }
    return total_encoded_count - total_source_count;
}

int main()
{
    const std::string data = read_file("./day08-part2/input.txt");
#ifdef BENCHMARK
    benchmark([&] { return solve(data); }, 100000);
#else
    std::println("{}", solve(data));
#endif
}