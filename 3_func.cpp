#include <chrono>
#include <cmath>
#include "3_const.cpp"

// Функция, используемая для установки скорости смены кадров
void pause(float dur) {
    auto start = std::chrono::high_resolution_clock::now();
    auto current = std::chrono::high_resolution_clock::now();
    for (;(current-start).count() < (long long)(dur*std::pow(10,9));current = std::chrono::high_resolution_clock::now()) {}
}

// Зависимость положения точки подвеса от времени
float H(float global_time) {
    return -H_0*400/l * std::cos(80*global_time);
}

// Перевод градусов в радианы
float to_rad(float theta) {
    return theta*(M_PI/180);
}

// Фунцкция для расчёта углового ускорения
float accel(float theta, float global_time) {
    return ((H_0*80*80*std::cos(80*global_time))/(l*l) - g/l)*std::sin(theta);
}
