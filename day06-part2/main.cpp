#include <utils.hpp>

#include <algorithm>
#include <cassert>

struct Vector2 {
    int x;
    int y;
};

enum class InstructionType { on, off, toggle };

struct Instruction {
    InstructionType type;
    Vector2 start;
    Vector2 end;
};

static Instruction parse_instruction(const std::string& data, int& pos)
{
    const std::string_view view { data.data() + pos, data.size() - pos };
    auto type = InstructionType::on;
    if (view.starts_with("turn on")) {
        type = InstructionType::on;
        pos += 8;
    } else if (view.starts_with("turn off")) {
        type = InstructionType::off;
        pos += 9;
    } else if (view.starts_with("toggle")) {
        type = InstructionType::toggle;
        pos += 7;
    } else {
        assert(false);
    }
    Vector2 start {};
    start.x = parse_uint<int>(data, pos);
    ++pos; // ","
    start.y = parse_uint<int>(data, pos);
    pos += 9; // " through "
    Vector2 end {};
    end.x = parse_uint<int>(data, pos);
    ++pos; // ","
    end.y = parse_uint<int>(data, pos);
    assert(start.x <= end.x);
    assert(start.y <= end.y);
    return { .type = type, .start = start, .end = end };
}

constexpr size_t grid_size = 1000;

enum class GridState : uint8_t { off, on };

static size_t grid_index(const Vector2& pos)
{
    return pos.x + pos.y * grid_size;
}

static void execute_instruction(std::vector<uint16_t>& grid, const Instruction& instruction)
{
    for (int y = instruction.start.y; y <= instruction.end.y; ++y) {
        for (int x = instruction.start.x; x <= instruction.end.x; ++x) {
            const size_t index = grid_index({ x, y });
            switch (instruction.type) {
            case InstructionType::on:
                ++grid[index];
                break;
            case InstructionType::off:
                if (grid[index] > 0) {
                    --grid[index];
                }
                break;
            case InstructionType::toggle:
                grid[index] += 2;
                break;
            }
        }
    }
}

static uint32_t solve(const std::string& data)
{
    std::vector<uint16_t> grid;
    grid.resize(grid_size * grid_size, 0);
    for (int pos = 0; pos < data.length(); ++pos) {
        Instruction instruction = parse_instruction(data, pos);
        execute_instruction(grid, instruction);
    }
    const uint32_t brightness
        = std::ranges::fold_left(grid, 0, [&](const uint32_t count, const uint16_t state) { return count + state; });
    return brightness;
}

int main()
{
    const std::string data = read_file("./day06-part2/input.txt");
#ifdef BENCHMARK
    benchmark([&] { return solve(data); }, 1000);
#else
    std::println("{}", solve(data));
#endif
}