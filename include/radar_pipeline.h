#pragma once

#include <vector>

// Represents a simulated radar target
struct Target {
    int range_bin;
    int doppler_bin;
    double amplitude;
};

// Represents a detected target
struct Detection {
    int range_bin;
    int doppler_bin;
    double value;
};

// Range-Doppler map type
using RDMap = std::vector<std::vector<double>>;

// Create empty range-doppler map
RDMap create_empty_map(int range_bins, int doppler_bins);

// Generate synthetic radar data with targets
RDMap generate_synthetic_rd_map(
    int range_bins,
    int doppler_bins,
    const std::vector<Target>& targets
);

// Detect peaks in the map using thresholding
std::vector<Detection> detect_peaks(
    const RDMap& map,
    double threshold_scale = 4.0
);

// Print ASCII visualization of the map
void print_map_ascii(const RDMap& map);

// Print detected targets
void print_detections(const std::vector<Detection>& detections);

// Save range-doppler map to CSV
bool export_rd_map_to_csv(const RDMap& map, const char* filename);