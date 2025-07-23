#include <pybind11/pybind11.h>
#include "example.hpp"



int double_number(int x) {
    return x * 2;
}

PYBIND11_MODULE(example, m) {
    m.doc() = "Example module that doubles a number";
    m.def("double_number", &double_number, "Doubles the input integer");
}
