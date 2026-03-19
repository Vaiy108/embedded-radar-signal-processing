# Embedded Radar Signal Processing

A compact C++ demo of a simplified radar processing pipeline for automotive and embedded sensing applications.

## Features
- Synthetic range-Doppler map generation
- CFAR-like adaptive peak detection
- Target list extraction
- CSV export for visualization
- Python heatmap plotting
- CMake-based build setup

## Project Structure
- `include/` : radar pipeline interface
- `src/` : implementation and executable entry point
- `python/` : visualization script

## Build
```bash
mkdir build
cd build
cmake ..
make
./radar_demo
```
## Output

The program generates:

- console-based ASCII range-Doppler visualization

- detected target list

- rd_map.csv for plotting


## Example Detection Pipeline

Synthetic targets → Range-Doppler map → CFAR-like detection → Target list


## Example Visualization

![Synthetic Range-Doppler Heatmap](docs/radar_heatmap.png)


## Background

This project demonstrates radar-processing fundamentals relevant to embedded and automotive sensing systems. It was created as a compact C++ implementation to complement earlier MATLAB-based radar target generation and detection work.