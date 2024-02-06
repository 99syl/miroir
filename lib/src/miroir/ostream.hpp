// `std::ostream` extensions for miroir.
// Include `miroir/type.hpp` before.

#ifndef MIROIR_OSTREAM_HPP
    #define MIROIR_OSTREAM_HPP

#include <iosfwd> //> std::ostream.

// Outputs a `const_string` object to an `std::ostream`.
template <unsigned int N>
inline std::ostream& operator<<(std::ostream& os, const miroir::const_string<N>& cs)
{
    return os << cs.value;
}

// Outputs every field of an object that belongs to a reflected type.
template <miroir::reflected_type T>
inline std::ostream& operator<<(std::ostream& os, const T& obj)
{
    auto i{0};
    os << miroir::get_type_name<T>() << ": { ";
    miroir::for_each_fields<T>([&os, &i, &obj]<miroir::field_info_interface FieldInfo>()
                               {
                                   os << FieldInfo::name << ": " << FieldInfo::get(obj);
                                   if (i + 1 < miroir::fields_size<T>())
                                   {
                                       os << ", ";
                                   }
                                   i++;
                               });
    return os << " }";
}

#endif // MIROIR_OSTREAM_HPP
