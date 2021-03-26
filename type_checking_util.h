#ifndef TYPE_CHECKING_UTIL_H
#define TYPE_CHECKING_UTIL_H

template<typename Base, typename T>
inline bool instanceof(const T*) {
    return std::is_base_of<Base, T>::value;
}

#endif