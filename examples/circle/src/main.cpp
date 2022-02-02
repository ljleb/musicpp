#include "circle.hpp"
#include <musicpp.hpp>

using namespace mpp;
using namespace mpp::literals;

int main(int const argc, char** const argv)
{
    Config const& config { argc, argv };

    auto&& sine
    {
        seq(
            make_basic<"circle">(500.),
            bind_front<Volume>(0.60),
            bind_front<Size>(config.sample_rate * 5)
        )
    };

    mpp::generate_and_write_samples(sine, config);
    return 0;
}
