#include <utils.hpp>

#include <array>

static void increment(std::string& string)
{
    for (int i = string.size() - 1; i >= 0; --i) {
        if (string[i] != 'z') {
            ++string[i];
            return;
        }
        string[i] = 'a';
    }
}

static bool password_valid(const std::string& password)
{
    auto has_3_increasing = [](const std::string& p) -> bool {
        for (size_t i = 2; i < p.size(); ++i) {
            if (p[i - 1] == p[i - 2] + 1 && p[i] == p[i - 1] + 1) {
                return true;
            }
        }
        return false;
    };

    auto has_no_forbidden_chars = [](const std::string& p) -> bool {
        constexpr std::array forbidden { 'i', 'o', 'l' };
        return std::ranges::none_of(p, [&](const char c) { return std::ranges::contains(forbidden, c); });
    };

    auto has_two_pairs = [](const std::string& p) -> bool {
        auto find_pair = [](const std::string_view view) -> std::optional<size_t> {
            for (size_t i = 1; i < view.size(); ++i) {
                if (view[i - 1] == view[i]) {
                    return i - 1;
                }
            }
            return std::nullopt;
        };
        const std::optional<size_t> first_index = find_pair(p);
        if (!first_index.has_value()) {
            return false;
        }
        return find_pair(std::string_view { p.begin() + *first_index + 2, p.end() }).has_value();
    };

    return has_two_pairs(password) && has_no_forbidden_chars(password) && has_3_increasing(password);
}

static std::string solve(const std::string& data)
{
    std::string password;
    for (int pos = 0; pos < data.size() && data[pos] != '\n'; ++pos) {
        password.push_back(data[pos]);
    }

    do {
        increment(password);
    } while (!password_valid(password));

    do {
        increment(password);
    } while (!password_valid(password));

    return password;
}

int main()
{
    const std::string data = read_file("./day11-part2/input.txt");
#ifdef BENCHMARK
    benchmark([&] { return solve(data); }, 1000);
#else
    std::println("{}", solve(data));
#endif
}