# miroir

miroir is a C++20 compile-time reflection library. It is still for now in experimental stage.

### Build status

[![ubuntu-clang12-cmake](https://github.com/SylEze/miroir/actions/workflows/ubuntu-clang12-cmake.yml/badge.svg)](https://github.com/SylEze/miroir/actions/workflows/ubuntu-clang12-cmake.yml) [![ubuntu-g++10-cmake](https://github.com/SylEze/miroir/actions/workflows/ubuntu-g++10-cmake.yml/badge.svg)](https://github.com/SylEze/miroir/actions/workflows/ubuntu-g++10-cmake.yml) [![windows-msvc142-cmake](https://github.com/SylEze/miroir/actions/workflows/windows-msvc142-cmake.yml/badge.svg)](https://github.com/SylEze/miroir/actions/workflows/windows-msvc142-cmake.yml)

### Usage

#### Generating type information

```cpp
#include <miroir/type.hpp>

struct point2d
{
    float x;
    float y;

    void add_x(float x);

    void add_y(float y);
    
    using type_info = miroir::type_info<point2d>
        ::with_name<"point2d">
        
        ::with_field<&point2d::x, "x">
        ::with_field<&point2d::y, "y">

        ::with_method<&point2d::add_x, "add_x">
        ::with_method<&point2d::add_y, "add_y">;
};
```

#### Fetching a field/method from a type

```cpp
#include <miroir/type.hpp>

int main()
{
    // Fetching field information.
    using x_field_info = miroir::get_field_info<point2d, "x">; 
    using y_field_info = miroir::get_field_info<point2d, "y">;

    // Fetching method information.
    using add_x_info = miroir::get_method_info<point2d, "add_x">;
    
    point2d p{10, 2};
    
    // Sets `x` member of `p` to 5.
    x_field_info::set(p, 5);
    
    // Sets `y` member of `p` to `x`.
    y_field_info::set(p, x_field_info::get(p));

    add_x_info::invoke(p, 2.5);
}
```

### Examples

The [examples](examples) folder is also available for more documentation and some use-case.