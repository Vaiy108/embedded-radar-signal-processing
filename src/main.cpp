#include "radar_pipeline.h"

#include <iostream>
#include <vector>

int main() {
    constexpr int range_bins = 32;
    constexpr int doppler_bins = 32;

    const std::vector<Target> targets = {
        {8, 10, 12.0},
        {18, 22, 10.0},
        {25, 7, 8.5}
    };

    RDMap rd_map = generate_synthetic_rd_map(range_bins, doppler_bins, targets);
    std::vector<Detection> detections = detect_peaks(rd_map, 4.0);

    std::cout << "Synthetic Range-Doppler Map (ASCII visualization)\n";
    std::cout << "=================================================\n";
    print_map_ascii(rd_map);

    print_detections(detections);

    if (export_rd_map_to_csv(rd_map, "rd_map.csv")) {
        std::cout << "\nSaved range-Doppler map to rd_map.csv\n";
    } else {
        std::cout << "\nFailed to save range-Doppler map to CSV\n";
    }

    return 0;
}