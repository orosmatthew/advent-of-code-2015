#include <utils.hpp>

#include <unordered_map>
#include <unordered_set>

struct StringPairHash {
    size_t operator()(const std::pair<std::string, std::string>& pair) const noexcept
    {
        return std::hash<std::string>()(pair.first) ^ std::hash<std::string>()(pair.second);
    }
};

using LocationPair = std::pair<std::string, std::string>;
using LocationPairDistances = std::unordered_map<LocationPair, int, StringPairHash>;

struct LocationsDistanceInput {
    std::string from;
    std::string to;
    int distance;
};

static LocationsDistanceInput parse_locations_distance(const std::string& data, int& pos)
{
    std::string from;
    while (data[pos] != ' ') {
        from.push_back(data[pos]);
        ++pos;
    }
    pos += 4; // " to "
    std::string to;
    while (data[pos] != ' ') {
        to.push_back(data[pos]);
        ++pos;
    }
    pos += 3; // " = ";
    const int distance = parse_uint<int>(data, pos);
    return LocationsDistanceInput { .from = std::move(from), .to = std::move(to), .distance = distance };
}

std::optional<int> calc_distance(const LocationPairDistances& distances, const std::vector<std::string>& locations)
{
    int distance = 0;
    for (size_t i = 1; i < locations.size(); ++i) {
        const std::string& from = locations[i - 1];
        const std::string& to = locations[i];
        auto it = distances.find(std::make_pair(from, to));
        if (it == distances.end()) {
            return std::nullopt;
        }
        distance += it->second;
    }
    return distance;
}

static int solve(const std::string& data)
{
    LocationPairDistances distances;
    std::unordered_set<std::string> locations_set;
    for (int pos = 0; pos < data.size(); ++pos) {
        const auto [from, to, distance] = parse_locations_distance(data, pos);
        distances[std::make_pair(from, to)] = distance;
        distances[std::make_pair(to, from)] = distance;
        locations_set.insert(from);
        locations_set.insert(to);
    }
    std::vector<std::string> locations;
    locations.reserve(locations_set.size());
    for (const std::string& location : locations_set) {
        locations.emplace_back(location);
    }
    std::ranges::sort(locations, std::less {});

    int max_dist = std::numeric_limits<int>::lowest();
    do {
        if (std::optional<int> dist = calc_distance(distances, locations); dist.has_value()) {
            max_dist = std::max(max_dist, *dist);
        }
    } while (std::ranges::next_permutation(locations, std::less {}).found);

    return max_dist;
}

int main()
{
    const std::string data = read_file("./day09-part2/input.txt");
#ifdef BENCHMARK
    benchmark([&] { return solve(data); }, 1000);
#else
    std::println("{}", solve(data));
#endif
}