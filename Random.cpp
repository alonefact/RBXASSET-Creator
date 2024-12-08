#include "Random.h"

namespace Random {

    std::string New(size_t Length) {
        const char CharSet[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
        std::uniform_int_distribution<> Dist(0, sizeof(CharSet) / sizeof(CharSet[0]) - 2);
        std::mt19937 Rng{ std::random_device{}() };

        std::string RandomString;
        for (size_t i = 0; i < Length; ++i) {
            RandomString += CharSet[Dist(Rng)];
        }

        return RandomString;
    }

}
