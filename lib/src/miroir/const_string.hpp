#ifndef MIROIR_CONST_STRING_HPP
    #define MIROIR_CONST_STRING_HPP

namespace miroir
{
    // A compile-time string type that can be used as a constant expression in template non-type parameters.
    template <unsigned int N>
    struct const_string
    {
        // Stored character array.
        char value[N];

        // Constructible only at compile-time !
        // Constructs a `const_string` object from a char array.
        consteval const_string(const char (&in)[N])
        {
            for (unsigned int i = 0; i < N; i++)
            {
                value[i] = in[i];
            }
        }

        // Returns the character at the given `index`.
        [[nodiscard]] consteval char operator[](unsigned int index)
        {
            return value[index];
        }
    };

    // Compares two `const_string` objects of the same size.
    template <unsigned int N>
    consteval bool operator==(const_string<N> left, const_string<N> right)
    {
        for (unsigned int i = 0; i < N; i++)
        {
            if (left.value[i] != right.value[i])
            {
                return false;
            }
        }
        return true;
    }

    // Compares two `const_string` objects of different sizes...
    // Obviously it returns false without any needed computation.
    template <unsigned int N, unsigned int N2>
    consteval bool operator==(const_string<N> left, const_string<N2> right)
    {
        return false;
    }

    // Compares a `const_string` object with another `StringType` object.
    template <unsigned int N, typename StringType>
    consteval bool operator==(const_string<N> left, StringType right)
    {
        for (unsigned int i = 0; i < N; i++)
        {
            if (left.value[i] != right[i])
            {
                return false;
            }
        }
        return true;
    }
}

#endif // MIROIR_CONST_STRING_HPP
