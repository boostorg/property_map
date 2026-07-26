// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// Exercises property_map_iterator.hpp: make_property_map_iterator over both
// an lvalue property map (pointer) and a readable property map (identity),
// covering both branches of property_map_iterator_generator.

#include <boost/property_map/property_map_iterator.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/core/lightweight_test.hpp>
#include <cstddef>
#include <vector>

int main()
{
    using namespace boost;

    // Readable property map: identity_property_map -> readable_pmap_iter branch.
    {
        using key_iterator = std::vector<std::size_t>::iterator;
        using pmap_iterator =
            property_map_iterator_generator<identity_property_map, key_iterator>::type;

        std::vector<std::size_t> keys;
        for (std::size_t i = 0; i < 5; ++i) keys.push_back(i);

        identity_property_map pmap;
        std::size_t idx = 0;
        pmap_iterator it = make_property_map_iterator(pmap, keys.begin());
        pmap_iterator end = make_property_map_iterator(pmap, keys.end());
        for (; it != end; ++it, ++idx)
            BOOST_TEST(*it == keys[idx]);
        BOOST_TEST(idx == keys.size());
    }

    // Lvalue property map: pointer -> lvalue_pmap_iter branch.
    {
        using key_iterator = std::vector<std::ptrdiff_t>::iterator;
        using pmap_iterator =
            property_map_iterator_generator<double*, key_iterator>::type;

        double values[5] = {10., 11., 12., 13., 14.};
        double* pmap = values;

        std::vector<std::ptrdiff_t> keys;
        for (std::ptrdiff_t i = 0; i < 5; ++i) keys.push_back(i);

        std::ptrdiff_t idx = 0;
        pmap_iterator it = make_property_map_iterator(pmap, keys.begin());
        pmap_iterator end = make_property_map_iterator(pmap, keys.end());
        for (; it != end; ++it, ++idx)
        {
            BOOST_TEST(*it == values[idx]);
            // lvalue iterator yields a mutable reference.
            *it += 1.0;
            BOOST_TEST(values[idx] == 11.0 + static_cast<double>(idx));
        }
        BOOST_TEST(idx == 5);
    }

    return boost::report_errors();
}
