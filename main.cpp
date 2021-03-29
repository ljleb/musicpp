#include <fstream>
#include <iostream>
#include <cmath>
#include <cstdint>
#include <array>

float process_sample(const uint64_t& index)
{
    return std::sin(index * 0.01f);
}

template <uint64_t samplesSize>
void process(std::array<float, samplesSize>& samples)
{
    for(uint64_t index = 0; index < samples.size(); ++index)
    {
        samples[index] = process_sample(index);
    }
}

int main()
{
    std::array<float, 44100*2> samples;

    std::ofstream file("audio.raw", std::ios::out | std::ios::binary);

    if (!file.good()) {
        std::cerr << "cannot open audio.raw";
        exit(1);
    }

    process(samples);

    file.seekp(0);
    file.write(reinterpret_cast<char*>(samples.data()), sizeof(float) * samples.size());

    return 0;
}
