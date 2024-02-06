namespace lib
{
    struct point3d
    {
        int x, y, z;
    };
}

#include "../miroir.hpp"
#include "ostream.hpp"

template <> struct miroir::type_info_provider<lib::point3d>
{
    using data = miroir::type_info_builder<lib::point3d>
        ::set_name<"point3d">
        ::add_field<&lib::point3d::x, "x">
        ::add_field<&lib::point3d::y, "y">
        ::add_field<&lib::point3d::z, "z">
        ::result;
};

int main()
{
    std::cout << lib::point3d{3, 0, 10} << std::endl;
    return 0;
}