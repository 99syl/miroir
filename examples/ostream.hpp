#include <iostream>
#include "../miroir.hpp"

// Puts in the stream a const_string.
template <unsigned int N>
inline std::ostream& operator<<(std::ostream& os, const miroir::const_string<N>& cs)
{
    return os << cs.value;
}

// Puts in the stream every field of an object that belongs to a reflected type.
template <miroir::reflected_type T>
inline std::ostream& operator<<(std::ostream& os, const T& obj)
{
    auto i{ 0 };
    os << miroir::get_type_name<T>() << ": { ";
    miroir::for_each_fields<T>([&os, &i, &obj]<typename FieldInfo>
    {
        os << FieldInfo::name.value << ": " << FieldInfo::get_val(obj);
        if (i + 1 < miroir::field_count<T>())
        {
            os << ", ";
        }
        i++;
    });
    return os << " }";
}