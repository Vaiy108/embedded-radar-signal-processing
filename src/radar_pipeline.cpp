#include "radar_pipeline.h"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>

RDMap create_empty_map(int range_bins, int doppler_bins) {
    return RDMap(range_bins, std::vector<double>(doppler_bins, 0.0));
}

static void add_gaussian_blob(RDMap& map, const Target& t, double sigma_r = 1.2, double sigma_d = 1.0) {
    const int range_bins = static_cast<int>(map.size());
    const int doppler_bins = static_cast<int>(map[0].size());

    for (int r = 0; r < range_bins; ++r) {
        for (int d = 0; d < doppler_bins; ++d) {
            const double dr = static_cast<double>(r - t.range_bin);
            const double dd = static_cast<double>(d - t.doppler_bin);
            const double exponent = -0.5 * ((dr * dr) / (sigma_r * sigma_r) +
                                            (dd * dd) / (sigma_d * sigma_d));
            map[r][d] += t.amplitude * std::exp(exponent);
        }
    }
}

static void add_noise(RDMap& map, double mean = 0.0, double stddev = 0.8) {
    std::mt19937 rng(42);
    std::normal_distribution<double> dist(mean, stddev);

    for (auto& row : map) {
        for (auto& cell : row) {
            cell += dist(rng);
            if (cell < 0.0) {
                cell = 0.0;
            }
        }
    }
}

RDMap generate_synthetic_rd_map(int range_bins, int doppler_bins, const std::vector<Target>& targets) {
    RDMap map = create_empty_map(range_bins, doppler_bins);

    for (const auto& target : targets) {
        add_gaussian_blob(map, target);
    }

    add_noise(map);
    return map;
}

static double compute_local_noise_estimate(const RDMap& map, int r, int d, int guard = 1, int train = 3) {
    const int range_bins = static_cast<int>(map.size());
    const int doppler_bins = static_cast<int>(map[0].size());

    double sum = 0.0;
    int count = 0;

    for (int rr = r - train; rr <= r + train; ++rr) {
        for (int dd = d - train; dd <= d + train; ++dd) {
            if (rr < 0 || rr >= range_bins || dd < 0 || dd >= doppler_bins) {
                continue;
            }

            const bool in_guard_r = std::abs(rr - r) <= guard;
            const bool in_guard_d = std::abs(dd - d) <= guard;
            if (in_guard_r && in_guard_d) {
                continue;
            }

            sum += map[rr][dd];
            ++count;
        }
    }

    return (count > 0) ? (sum / static_cast<double>(count)) : 0.0;
}

std::vector<Detection> detect_peaks(const RDMap& map, double threshold_scale) {
    const int range_bins = static_cast<int>(map.size());
    const int doppler_bins = static_cast<int>(map[0].size());

    std::vector<Detection> detections;

    for (int r = 3; r < range_bins - 3; ++r) {
        for (int d = 3; d < doppler_bins - 3; ++d) {
            const double noise = compute_local_noise_estimate(map, r, d);
            const double threshold = noise * threshold_scale;
            const double value = map[r][d];

            if (value < threshold || value < 1.0) {
                continue;
            }

            bool is_local_max = true;
            for (int rr = r - 1; rr <= r + 1 && is_local_max; ++rr) {
                for (int dd = d - 1; dd <= d + 1; ++dd) {
                    if (rr == r && dd == d) {
                        continue;
                    }
                    if (map[rr][dd] >= value) {
                        is_local_max = false;
                        break;
                    }
                }
            }

            if (is_local_max) {
                detections.push_back({r, d, value});
            }
        }
    }

    return detections;
}

void print_map_ascii(const RDMap& map) {
    double max_val = 0.0;
    for (const auto& row : map) {
        for (double v : row) {
            max_val = std::max(max_val, v);
        }
    }

    const std::string levels = " .:-=+*#%@";

    for (const auto& row : map) {
        for (double v : row) {
            const double norm = (max_val > 0.0) ? v / max_val : 0.0;
            const std::size_t idx = std::min(
                static_cast<std::size_t>(norm * (levels.size() - 1)),
                levels.size() - 1
            );
            std::cout << levels[idx];
        }
        std::cout << '\n';
    }
}

void print_detections(const std::vector<Detection>& detections) {
    std::cout << "\nDetections:\n";
    std::cout << "----------------------------------\n";
    std::cout << "RangeBin  DopplerBin  Magnitude\n";
    std::cout << "----------------------------------\n";

    for (const auto& det : detections) {
        std::cout << std::setw(8) << det.range_bin << "  "
                  << std::setw(10) << det.doppler_bin << "  "
                  << std::setw(9) << std::fixed << std::setprecision(2)
                  << det.value << '\n';
    }
}

bool export_rd_map_to_csv(const RDMap& map, const char* filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    for (const auto& row : map) {
        for (std::size_t i = 0; i < row.size(); ++i) {
            file << row[i];
            if (i + 1 < row.size()) {
                file << ",";
            }
        }
        file << "\n";
    }

    return true;
}