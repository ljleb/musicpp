#include "self_power.hpp"
#include <musicpp.hpp>

using namespace mpp;
using namespace mpp::literals;

int main(int const argc, char** const argv)
{
    Config const& config { argc, argv };

    auto&& sine
    {
        seq(
            make_basic<"sine">(SelfPower { Bezier { 0, 20. } }),
            bind_front<Volume>(.1),
            bind_front<Size>(config.sample_rate * 20)
        )
    };

    mpp::generate_and_write_samples(sine, config);
    return 0;
}
