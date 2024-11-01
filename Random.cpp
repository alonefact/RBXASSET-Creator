#include "Random.h"
#include <random>

namespace Random {

    std::wstring New(size_t Length) {
        const wchar_t CharSet[] = L"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
        std::uniform_int_distribution<> Dist(0, sizeof(CharSet) / sizeof(CharSet[0]) - 2);
        std::mt19937 Rng{ std::random_device{}() };

        std::wstring RandomString;
        for (size_t i = 0; i < Length; ++i) {
            RandomString += CharSet[Dist(Rng)];
        }

        return RandomString;
    }

}