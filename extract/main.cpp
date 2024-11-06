#include <string>
#include <iostream>
#include "POLYNOMIAL_4D.h"
#include <nlohmann/json.hpp>
#include <fstream>

nlohmann::json quats_to_json(const std::vector<QUATERNION>& quats) {
    std::vector<std::array<double, 4>> res;
    for (auto& quat : quats) {
        std::array<double, 4> arr = {quat.x(), quat.y(), quat.z(), quat.w()};
        res.push_back(arr);
    }
    return res;
}

nlohmann::json poly_to_json(const POLYNOMIAL_4D& poly) {
    nlohmann::json res;
    res["roots"] = quats_to_json(poly.roots());
    auto powers = poly.powers();
    for (auto& power : powers) {
        power *= poly.powerScalar();
    }
    res["powers"] = powers;
    return res;
}

nlohmann::json extract(std::string filename)
{
    nlohmann::json res;
    FILE *file;
    file = fopen(filename.c_str(), "rb");
    if (file == NULL)
    {
        std::cout << " Could not open file " << filename << std::endl;
        exit(0);
    }
    std::cout << " Reading file " << filename.c_str() << " ... " << std::flush;

    std::array<int, 3> resolution;
    for (size_t i = 0; i < 3; i++) {
        fread((void*)&resolution[i], sizeof(int), 1, file);
    }

    res["resolution"] = resolution;

    std::cout << " Read in res: " << resolution[0] << " " << resolution[1] << " " << resolution[2] << std::endl;

    std::array<double, 3> center;
    for (size_t i = 0; i < 3; i++) {
        fread((void*)&center[i], sizeof(double), 1, file);
    }

    res["center"] = center;

    std::array<double, 3> lengths;
    for (size_t i = 0; i < 3; i++) {
        fread((void*)&lengths[i], sizeof(double), 1, file);
    }

    res["lengths"] = lengths;

    POLYNOMIAL_4D top;
    top.read(file);
    res["top"] = poly_to_json(top);

    POLYNOMIAL_4D bottom;
    bottom.read(file);
    res["bottom"] = poly_to_json(bottom);

    return res;
}

int main(int argc, char** argv) {
    std::string input = argv[1];
    std::string output = argv[2];
    nlohmann::json contents = extract(input);
    std::ofstream outfile(output);
    outfile << contents;
    return 0;
}
