#ifndef SAFE_STRING_H
#define SAFE_STRING_H

#include <cstddef>
#include <type_traits>

#define TIME_HOUR ((__TIME__[0] - '0') * 10 + (__TIME__[1] - '0'))
#define TIME_MIN ((__TIME__[3] - '0') * 10 + (__TIME__[4] - '0'))
#define TIME_SEC ((__TIME__[6] - '0') * 10 + (__TIME__[7] - '0'))

#define TIME_KEY (TIME_HOUR * 3600 + TIME_MIN * 60 + TIME_SEC)

template <std::size_t N>
struct EncryptedString
{
    char data[N];

    constexpr EncryptedString(const char (&str)[N], int key) : data{}
    {
        encrypt<0>(str, key);
    }

    template <std::size_t I>
        constexpr typename std::enable_if < I<N, void>::type
                                            encrypt(const char (&str)[N], int key)
    {
        data[I] = str[I] ^ ((key >> ((I % 4) * 8)) & 0xFF);
        encrypt<I + 1>(str, key);
    }

    template <std::size_t I>
    constexpr typename std::enable_if<I >= N, void>::type
    encrypt(const char (&)[N], int)
    {
    }

    char *decrypt(int key)
    {
        for (std::size_t i = 0; i < N; ++i)
        {
            data[i] ^= ((key >> ((i % 4) * 8)) & 0xFF);
        }
        return data;
    }
};

#define SAFE_STRING(s) ([]() -> char * { \
    static EncryptedString<sizeof(s)> es(s, TIME_KEY); \
    return es.decrypt(TIME_KEY); }())

#endif // SAFE_STRING_H
