#ifndef MIROIR_INVOCABLE_HPP
    #define MIROIR_INVOCABLE_HPP

namespace miroir
{
    // Checks if the given parameter `Func` is actually a function that accepts the given `Args` and returns an object of type `Ret`.
    template <typename Func, typename Ret, typename ...Args>
    concept invocable = requires(Func func, Args&& ...args)
    {
        static_cast<Ret(*)(Args...)>(func);
    };
}

#endif // MIROIR_INVOCABLE_HPP