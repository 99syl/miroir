#include <iostream>

#include <miroir/type.hpp>
#include <miroir/ostream.hpp>

#include "lib.hpp"

template <> struct miroir::type_info_provider<lib::point3d>
{
    using data = miroir::type_info<lib::point3d>
        ::with_name<"point3d">

        ::with_field<&lib::point3d::x, "x">
        ::with_field<&lib::point3d::y, "y">
        ::with_field<&lib::point3d::z, "z">;
};

int main()
{
    std::cout << lib::point3d{3, 0, 10} << std::endl;
    return 0;
}
