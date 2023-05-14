#include <iostream>
#include <cmath>
#include <unordered_map>
#include <random>

// Define a structure to represent complex numbers
struct Complex {
    double real;
    double imag;
};

std::unordered_map<int, Complex> generateSymbolMap() {
    std::unordered_map<int, Complex> symbolMap;
    double step = 10.0 / std::sqrt(256.0);

    // Generate the symbol map points
    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 16; ++j) {
            double real = -5.0 + step * j + step / 2.0;
            double imag = -5.0 + step * i + step / 2.0;
            int symbol = i * 16 + j;  // Generate unique symbol key
            symbolMap[symbol] = { real, imag };
        }
    }

    return symbolMap;
}

std::unordered_map<int, std::pair<double, double>> generateAmpPhaseMap() {
    const double M_PI = std::acos(-1.0) * 2.0;

    std::unordered_map<int, std::pair<double, double>> ampPhaseMap;
    std::unordered_map<int, Complex> symbolMap = generateSymbolMap();

    for (const auto& entry : symbolMap) {
        int symbol = entry.first;
        const Complex& point = entry.second;

        double amp = std::hypot(point.real, point.imag);
        double phase = std::atan2(point.imag, point.real) * 180.0 / M_PI;

        ampPhaseMap[symbol] = { amp, phase };
    }

    return ampPhaseMap;
}

int main() {
    // Generate white Gaussian noise for each point in the symbol map
    std::unordered_map<int, Complex> symbolMap = generateSymbolMap();
    std::default_random_engine generator;
    std::normal_distribution<double> distribution(0.0, 0.1);

    for (auto& entry : symbolMap) {
        double noise = distribution(generator);
        entry.second.real += noise;
        entry.second.imag += noise;
    }

    std::unordered_map<int, std::pair<double, double>> ampPhaseMap = generateAmpPhaseMap();

    std::cout << "Symbol Map with Amplitude and Phase:" << std::endl;
    for (const auto& entry : symbolMap) {
        int symbol = entry.first;
        const Complex& point = entry.second;
        const auto& ampPhase = ampPhaseMap[symbol];
        double amp = ampPhase.first;
        double phase = ampPhase.second;

        std::cout << "Symbol: " << symbol << ", Complex Value: " << point.real << " + " << point.imag << "i"
            << ", Amplitude: " << amp << ", Phase: " << phase << " degrees" << std::endl;
    }

    return 0;
}
