#pragma once
#include <complex>
#include <vector>
#include <utility>

// Function to check if a complex number is in the Mandelbrot set
bool isInMandelbrotSet(std::complex<double> c, int maxIterations = 100) {
    std::complex<double> z = 0;
    for (int i = 0; i < maxIterations; ++i) {
        z = z * z + c;
        if (std::abs(z) > 2.0)
            return false; // Escapes
    }
    return true; // Bounded, assume in set
}

void StorePoints(std::vector<std::pair<float, float>>& p) {
    const int width = 600;
    const int height = 600;

    // Range of real and imaginary parts (typical for Mandelbrot set)
    const double realMin = -1.0;
    const double realMax = 1.0;
    const double imagMin = -1.0;
    const double imagMax = 1.0;

    for (int y = 0; y < height; ++y) {
        double imag = imagMax - (imagMax - imagMin) * y / (height - 1);
        for (int x = 0; x < width; ++x) {
            double real = realMin + (realMax - realMin) * x / (width - 1);
            std::complex<double> c(real, imag);
            if (isInMandelbrotSet(c))
            {
                p.push_back({ real, imag });
            }
        }
    }

}