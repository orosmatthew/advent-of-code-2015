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

static int count_memory_chars(const std::string& string)
{
    const std::string_view view { string.data() + 1, string.size() - 2 };
    int count = 0;
    for (int i = 0; i < view.size(); ++i) {
        ++count;
        if (i >= view.size() - 1) {
            continue;
        }
        const char c1 = view[i];
        const char c2 = view[i + 1];
        if (c1 == '\\' && c2 == '\\') {
            ++i;
        } else if (c1 == '\\' && c2 == '\"') {
            ++i;
        } else if (c1 == '\\' && c2 == 'x') {
            i += 3;
        }
    }
    return count;
}

static int solve(const std::string& data)
{
    static std::string line;
    int total_source_count = 0;
    int total_mem_count = 0;
    for (int pos = 0; pos < data.size(); ++pos) {
        parse_line(data, pos, line);
        total_source_count += line.size();
        total_mem_count += count_memory_chars(line);
    }
    return total_source_count - total_mem_count;
}

int main()
{
    const std::string data = read_file("./day08-part1/input.txt");
#ifdef BENCHMARK
    benchmark([&] { return solve(data); }, 10000);
#else
    std::println("{}", solve(data));
#endif
}