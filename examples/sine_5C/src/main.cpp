#include <musicpp.hpp>

using namespace mpp;
using namespace mpp::literals;

int main(int const argc, char** const argv)
{
    Config const& config { argc, argv };

    auto&& sine
    {
        seq(
            make_basic<"sine">(5_C),
            bind_front<Volume>(0.8),
            bind_front<Size>(config.sample_rate * 20)
        )
    };

    mpp::generate_and_write_samples(sine, config);
    return 0;
}
