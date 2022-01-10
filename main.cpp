#include <percy/percy.hpp>
#include <lorina/lorina.hpp>
#include <cassert>
#include <cstdio>
#include <fstream>
#include <iostream>

using namespace percy;
using kitty::dynamic_truth_table;

int main(void)
{
    {
        chain c;
        spec spec;
        dynamic_truth_table x(3), y(3), z(3);

        create_nth_var(x, 0);
        create_nth_var(y, 1);
        create_nth_var(z, 2);

        // The sum and carry functions represent the outputs of the
        // chain that we want to synthesize.
        const auto sum = x ^ y ^ z;
        const auto carry = ternary_majority(x, y, z);
        spec[0] = sum;
        spec[1] = carry;

        // Call the synthesizer with the specification we've constructed.
        auto result = synthesize(spec, c);

        // Ensure that synthesis was successful.
        assert(result == success);

        // Simulate the synthesized circuit and ensure that it
        // computes the correct functions.
        auto sim_fs = c.simulate();
        assert(sim_fs[0] == sum);
        assert(sim_fs[1] == carry);
    }

    std::cout << "Finalizado\n";
    return 0;
}
