#ifndef BREAKOUT_UTILITIES_H
#define BREAKOUT_UTILITIES_H


#include <SFML/Graphics.hpp>
#include <iostream>


inline float length(const sf::Vector2f& v)
{
    return std::sqrtf(v.x * v.x + v.y * v.y);
}

template<typename T>
inline void centerOrigin(T& t) {
    auto bounds = t.getLocalBounds();
    t.setOrigin((bounds.width / 2.f) + bounds.left,
        (bounds.height / 2.f) + bounds.top);
}

template<typename T>
inline void centerOrigin(std::shared_ptr<T> t) {
    if (t) {
        auto bounds = t->getLocalBounds();
        t->setOrigin((bounds.width / 2.f) + bounds.left,
            (bounds.height / 2.f) + bounds.top);
    }
}

template <typename T>
inline std::ostream& operator<<(std::ostream& os, const sf::Vector2<T>& v) {
    os << "{" << v.x << ", " << v.y << "}";
}


template <typename T>
inline std::ostream& operator<<(std::ostream& os, const sf::Rect<T>& r) {
    os << "{{" << r.left << ", " << r.top << "}, {"<< r.width << ", " << r.height << "}";
    return os;
}


template <typename T>
inline void displayBounds(const T& t) {
    std::cout << "Local bounds : " << t.getLocalBounds() << "\n";
    std::cout << "Global bounds: " << t.getGlobalBounds() << "\n";
}






#endif //BREAKOUT_UTILITIES_H
