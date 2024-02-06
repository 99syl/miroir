# miroir

miroir is a C++20 compile-time reflection library. It demonstrates how one can achieve simple reflection with some template tricks.

### Important Notes !!!

This library has never been tested on real-world big projects so I have absolutely no idea how it performs in those situations. If you have some metrics, feel free to open an issue to submit it.

### Usage

#### Generating type information:

```cpp
#include <miroir.hpp>

struct point2d
{
    float x;
    float y;

    void add_x(float x);

    void add_y(float y);
    
    using type_info = miroir::type_info_builder<point2d>
        ::set_name<"point2d">
        
        ::add_field<&point2d::x, "x">
        ::add_field<&point2d::y, "y">

        ::add_method<&point2d::add_x, "add_x">
        ::add_method<&point2d::add_y, "add_y">
        
        ::result;
};
```

#### Fetching a field/method from a type:

```cpp
#include <miroir.hpp>

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

#### Iterating over every field/method from a type:
```cpp
miroir::for_each_field<T>([]<typename FieldInfo>
{
    ...
});
```

The [examples](examples) folder is also available to show some use-cases.