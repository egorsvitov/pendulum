#include <iostream>
#include <cmath>
#include <vector>
#include <chrono>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <string>
#include "3_func.hpp"


int main() {

    // Создание массивов моментов времени и соответствующих им угловых скоростьей и углов
    std::vector<float> t;
    for (float i = 0; i < duration; i+=time_step) {
        t.push_back(i);
    }
    auto n = t.size();
    std::vector<float> theta(n, 0.0f);
    std::vector<float> omega(n, 0.0f);
    theta[0] = to_rad(initial_angle);

    // Начальная угловая скорость маятника
    omega[0] = 0; // -(H_0*nu)*std::sin(to_rad(initial_angle))/l;

    // Создание объектов стержня маятника и груза
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "pendulum");
    
    sf::RectangleShape rectangle(sf::Vector2f(10, 400));

    rectangle.setFillColor(sf::Color::White);
    rectangle.setOrigin(5,10);
    rectangle.rotate(initial_angle);
    rectangle.setPosition(500, 500);

    sf::CircleShape bob(20);
    bob.setFillColor(sf::Color::White);
    bob.setOrigin(20, -380);
    bob.rotate(initial_angle);
    bob.setPosition(500, 500);

    // Текущее время в симуляции
    float global_time = 0;

    // Цикл, в каждой итерации которого пересчитываются и отрисовываются новые состояния системы
    for (int i = 0; (i < n-1) && window.isOpen(); i++, global_time+=time_step) {
        auto start1 = std::chrono::high_resolution_clock::now();
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        // Численное интегрирование угла и угловой скорости с помощью метода Рунге-Кутты
        float k1_theta = time_step*omega[i];
        float k1_w = time_step*accel(theta[i], global_time);

        float k2_theta = time_step*(omega[i]+0.5*k1_w);
        float k2_w = time_step*accel(theta[i]+0.5*k1_theta, global_time+time_step/2);

        float k3_theta = time_step*(omega[i]+0.5*k2_w);
        float k3_w = time_step*accel(theta[i]+0.5*k2_theta, global_time+time_step/2);

        float k4_theta = time_step*(omega[i]+k3_w);
        float k4_w = time_step*accel(theta[i]+k3_theta, global_time);

        theta[i+1] = theta[i] + (k1_theta + 2 * k2_theta + 2 * k3_theta + k4_theta) / 6.0f; 
        omega[i+1] = omega[i] + (k1_w + 2 * k2_w + 2 * k3_w + k4_w) / 6.0f;

        // Смена кадров, отрисовка новых положений объектов
        rectangle.rotate(theta[i+1]*180/M_PI-theta[i]*180/M_PI);
        bob.rotate(theta[i+1]*180/M_PI-theta[i]*180/M_PI);
        rectangle.setPosition(500, 500 + H(global_time));
        bob.setPosition(500, 500 + H(global_time));
        window.clear();
        window.draw(rectangle);
        window.draw(bob);
        window.display();

        auto end1 = std::chrono::high_resolution_clock::now();
        // Остановка программы для совпадения с реальным временем
        pause(slowness*(time_step-(end1-start1).count()/std::pow(10,9)));
    }

    // Запись данных об изменении угла и угловой скорости в файл
    std::ofstream create;
    create.open("kapitzas_thetas.txt");
    create << '\n';
    create.close();
    std::ofstream out;
    out.open("kapitzas_thetas.txt", std::ios::app);
    for (int i = 0; i < n; i++) {
        out << std::to_string(time_step*i) << " " << std::to_string(theta[i]) << " " << std::to_string(omega[i]) << std::endl;
    }
}