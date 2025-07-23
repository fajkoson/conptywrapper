# conptywrapper

A lightweight Windows-only C++ library for working with ConPTY (Windows Pseudo Console API), with Python bindings via pybind11.
It is just proof of concept and no more is expected from this (for now).

## Requirements

- Windows 10 or later
- CMake 3.20+
- Python 3.12.6
- MSVC(2022)

## Project Structure

```text

conptywrapper/
├── CMakeLists.txt           # Top-level CMake build file
├── CMakePresets.json        # CMake presets for convenient builds
├── LICENSE                  # Project license
├── README.md                # (This file)
│
├──source/
│  ├── pseudoconsole/        # C++ module compiled as a Python wheel
│  │   ├── include/
│  │   ├── src/
│  │   └── CMakeLists.txt
│  └── example/              # C++ module compiled as a Python wheel
│      ├── include/
│      ├── src/
│      └── CMakeLists.txt
│ 
└── 3rd-party/
    └── include/pybind11/    # Embedded pybind11 headers
```
