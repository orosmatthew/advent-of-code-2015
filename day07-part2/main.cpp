#include <utils.hpp>

#include <cassert>
#include <ranges>
#include <unordered_map>
#include <variant>

template <class... Ts>
struct overloads : Ts... {
    using Ts::operator()...;
};

using PartArg = std::variant<std::string, uint16_t>;

enum class PartOpUnaryType { signal, not_ };

struct PartOpUnary {
    PartOpUnaryType type;
    PartArg arg;
};

enum class PartOpBinaryType { and_, or_, lshift, rshift };

struct PartOpBinary {
    PartOpBinaryType type;
    PartArg arg1;
    PartArg arg2;
};

using PartOp = std::variant<PartOpUnary, PartOpBinary>;

struct Part {
    PartOp op;
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
        PartArg arg = parse_part_arg(data, pos);
        pos += 4; // " -> "
        std::string output_wire = parse_output_wire(data, pos);
        return Part { .op = PartOpUnary { .type = PartOpUnaryType::not_, .arg = std::move(arg) },
                      .output_wire = std::move(output_wire) };
    }
    PartArg arg1 = parse_part_arg(data, pos);
    ++pos; // " "
    const std::string_view view { data.data() + pos, data.size() - pos };
    if (view.starts_with("-> ")) {
        pos += 3; // "-> "
        std::string output_wire = parse_output_wire(data, pos);
        return Part { .op = PartOpUnary { .type = PartOpUnaryType::signal,
                                          .arg = std::move(arg1),
                                           }, .output_wire = std::move(output_wire) };
    }
    auto type = PartOpBinaryType::and_;
    if (view.starts_with("AND")) {
        type = PartOpBinaryType::and_;
        pos += 4; // "AND "
    } else if (view.starts_with("OR")) {
        type = PartOpBinaryType::or_;
        pos += 3; // "OR "
    } else if (view.starts_with("LSHIFT")) {
        type = PartOpBinaryType::lshift;
        pos += 7; // "LSHIFT "
    } else if (view.starts_with("RSHIFT")) {
        type = PartOpBinaryType::rshift;
        pos += 7; // "RSHIFT "
    } else {
        assert(false);
    }
    PartArg arg2 = parse_part_arg(data, pos);
    pos += 4; // " -> "
    std::string output_wire = parse_output_wire(data, pos);
    return Part { .op = PartOpBinary { .type = type, .arg1 = std::move(arg1), .arg2 = std::move(arg2) },
                  .output_wire = std::move(output_wire) };
}

using Wires = std::unordered_map<std::string, std::optional<uint16_t>>;

static std::optional<uint16_t> get_arg(const PartArg& arg, const Wires& wires)
{
    if (const uint16_t* value = std::get_if<uint16_t>(&arg); value != nullptr) {
        return *value;
    }
    if (const std::string* wire = std::get_if<std::string>(&arg); wire != nullptr) {
        assert(wires.contains(*wire));
        return wires.at(*wire);
    }
    assert(false);
    return std::nullopt;
}

static bool try_solve(const Part& part, Wires& wires)
{
    if (const PartOpUnary* op = std::get_if<PartOpUnary>(&part.op); op != nullptr) {
        const std::optional<uint16_t> arg_value = get_arg(op->arg, wires);
        if (!arg_value.has_value()) {
            return false;
        }
        switch (op->type) {
        case PartOpUnaryType::signal:
            wires[part.output_wire] = *arg_value;
            return true;
        case PartOpUnaryType::not_:
            wires[part.output_wire] = ~*arg_value;
            return true;
        }
    }
    if (const PartOpBinary* op = std::get_if<PartOpBinary>(&part.op); op != nullptr) {
        const std::optional<uint16_t> arg1_value = get_arg(op->arg1, wires);
        const std::optional<uint16_t> arg2_value = get_arg(op->arg2, wires);
        if (!arg1_value.has_value() || !arg2_value.has_value()) {
            return false;
        }
        switch (op->type) {
        case PartOpBinaryType::and_:
            wires[part.output_wire] = *arg1_value & *arg2_value;
            return true;
        case PartOpBinaryType::or_:
            wires[part.output_wire] = *arg1_value | *arg2_value;
            return true;
        case PartOpBinaryType::lshift:
            wires[part.output_wire] = *arg1_value << *arg2_value;
            return true;
        case PartOpBinaryType::rshift:
            wires[part.output_wire] = *arg1_value >> *arg2_value;
            return true;
        }
    }
    assert(false);
    return false;
}

static bool solve_step(const std::vector<Part>& parts, Wires& wires)
{
    bool changed = false;
    for (const Part& part : parts) {
        assert(wires.contains(part.output_wire));
        if (wires.at(part.output_wire).has_value()) {
            continue;
        }
        if (try_solve(part, wires)) {
            changed = true;
        }
    }
    return changed;
}

static int solve(const std::string& data)
{
    std::vector<Part> parts;
    for (int pos = 0; pos < data.size(); ++pos) {
        parts.emplace_back(parse_part(data, pos));
    }
    Wires wires;
    for (const auto& [op, output_wire] : parts) {
        wires[output_wire] = std::nullopt;
    }
    while (true) {
        if (!solve_step(parts, wires)) {
            break;
        }
    }
    assert(wires.contains("a") && wires.at("a").has_value());
    const uint16_t first_wire_a_value = wires.at("a").value();
    for (auto& value : wires | std::views::values) {
        value.reset();
    }
    wires["b"] = first_wire_a_value;
    while (true) {
        if (!solve_step(parts, wires)) {
            break;
        }
    }
    assert(wires.at("a").has_value());
    return wires.at("a").value();
}

int main()
{
    const std::string data = read_file("./day07-part2/input.txt");
#ifdef BENCHMARK
    benchmark([&] { return solve(data); }, 1000);
#else
    std::println("{}", solve(data));
#endif
}