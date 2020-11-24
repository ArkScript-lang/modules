#ifndef MD5_HPP
#define MD5_HPP

#define _CRT_SECURE_NO_WARNINGS

#include <string>
#include <cstring>

namespace md5
{
    std::string md5(const std::string& dat);
    std::string md5(const void* dat, size_t len);
}

#endif // end of MD5_HPP