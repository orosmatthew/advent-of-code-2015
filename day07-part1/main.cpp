#include <utils.hpp>

#include <cassert>
#include <unordered_map>
#include <variant>

enum class PartType { signal, and_, or_, lshift, rshift, not_ };

using PartArg = std::variant<std::string, uint16_t>;

struct Part {
    PartType type;
    PartArg arg1;
    std::optional<PartArg> arg2;
    std::string output_wire;
};

static std::string parse_output_wire(const std::string& data, int& pos)
{
    std::string wire;
    while (data[pos] != '\n') {
        wire.push_back(data[pos]);
        ++pos;
    }
    return wire;
}

static PartArg parse_part_arg(const std::string& data, int& pos)
{
    if (is_digit(data[pos])) {
        return parse_uint<uint16_t>(data, pos);
    }
    std::string wire;
    while (data[pos] != ' ') {
        wire.push_back(data[pos]);
        ++pos;
    }
    return wire;
}

static Part parse_part(const std::string& data, int& pos)
{
    if (const std::string_view view { data.data() + pos, data.size() - pos }; view.starts_with("NOT")) {
        pos += 4; // "NOT "
        PartArg arg1 = parse_part_arg(data, pos);
        pos += 4; // " -> "
        std::string output_wire = parse_output_wire(data, pos);
        return Part {
            .type = PartType::not_, .arg1 = std::move(arg1), .arg2 = std::nullopt, .output_wire = std::move(output_wire)
        };
    }
    PartArg arg1 = parse_part_arg(data, pos);
    ++pos; // " "
    const std::string_view view { data.data() + pos, data.size() - pos };
    if (view.starts_with("-> ")) {
        pos += 3; // "-> "
        std::string output_wire = parse_output_wire(data, pos);
        return Part { .type = PartType::signal,
                      .arg1 = std::move(arg1),
                      .arg2 = std::nullopt,
                      .output_wire = std::move(output_wire) };
    }
    auto type = PartType::and_;
    if (view.starts_with("AND")) {
        type = PartType::and_;
        pos += 4; // "AND "
    } else if (view.starts_with("OR")) {
        type = PartType::or_;
        pos += 3; // "OR "
    } else if (view.starts_with("LSHIFT")) {
        type = PartType::lshift;
        pos += 7; // "LSHIFT "
    } else if (view.starts_with("RSHIFT")) {
        type = PartType::rshift;
        pos += 7; // "RSHIFT "
    } else {
        assert(false);
    }
    PartArg arg2 = parse_part_arg(data, pos);
    pos += 4; // " -> "
    std::string output_wire = parse_output_wire(data, pos);
    return Part {
        .type = type, .arg1 = std::move(arg1), .arg2 = std::move(arg2), .output_wire = std::move(output_wire)
    };
}

static int solve(const std::string& data)
{
    std::vector<Part> parts;
    for (int pos = 0; pos < data.size(); ++pos) {
        parts.emplace_back(parse_part(data, pos));
    }
    return 1;
}

int main()
{
    const std::string data = read_file("./day07-part1/sample.txt");
#ifdef BENCHMARK
    benchmark([&] { return solve(data); }, 1000);
#else
    std::println("{}", solve(data));
#endif
}