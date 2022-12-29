#include <iostream>
#include <cmath>
#include <vector>
#include <chrono>
#include <SFML/Graphics.hpp>

float m = 3.0, g = 9.8, r = 2.0, I = 12.0, h = 0.0025, l = 2.0;
float initial_angle = 165;
float w_0 = 100, H_0 = 0.2;

void pause(float dur) {
    auto start = std::chrono::high_resolution_clock::now();
    auto current = std::chrono::high_resolution_clock::now();
    for (;(current-start).count() < (long long)(dur*std::pow(10,9));current = std::chrono::high_resolution_clock::now()) {}
}

float f(float global_time) {
    return m * H_0 * w_0 * w_0 * std::sin(w_0*global_time);
}

float H(float global_time) {
    return H_0*400/l * std::sin(w_0*global_time);
}

float to_rad(float theta) {
    return theta*(M_PI/180);
}

float accel(float theta, float global_time) {
    return (f(global_time)-m*g)*r*sin(theta)/I;
}

int main() {

    float cycle = 30.0;
    std::vector<float> t;
    for (float i = 0; i < cycle; i+=h) {
        t.push_back(i);
    }
    auto n = t.size();
    
    std::vector<float> y(n, 0.0f);
    std::vector<float> v(n, 0.0f);
    y[0] = to_rad(initial_angle);
    v[0] = to_rad(0.0f);

    sf::RenderWindow window(sf::VideoMode(1000, 1000), "simple pendulum");
    sf::RectangleShape rectangle(sf::Vector2f(20, 400));
    // window.setFramerateLimit(60);
    rectangle.setFillColor(sf::Color::White);
    rectangle.setOrigin(10,10);
    rectangle.rotate(initial_angle);
    rectangle.setPosition(500, 500);

    float global_time = 0;
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; (i < n-1) && window.isOpen(); i++, global_time+=h) {
        auto start1 = std::chrono::high_resolution_clock::now();
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        float k1_theta = h*v[i];
        float k1_w = h*accel(y[i], global_time);

        float k2_theta = h*(v[i]+0.5*k1_w);
        float k2_w = h*accel(y[i]+0.5*k1_theta, global_time);

        float k3_theta = h*(v[i]+0.5*k2_w);
        float k3_w = h*accel(y[i]+0.5*k2_theta, global_time);

        float k4_theta = h*(v[i]+k3_w);
        float k4_w = h*accel(y[i]+k3_theta, global_time);

        y[i+1] = y[i] + (k1_theta + 2 * k2_theta + 2 * k3_theta + k4_theta) / 6.0f; 
        v[i+1] = v[i] + (k1_w + 2 * k2_w + 2 * k3_w + k4_w) / 6.0f;

        // std::cout << perfect_theta(global_time) << ' ' << global_time << std::endl; 
        rectangle.rotate(y[i+1]*180/M_PI-y[i]*180/M_PI);
        // rectangle.setPosition(500, 500 + H(global_time));
        window.clear();
        window.draw(rectangle);
        window.display();

        auto end1 = std::chrono::high_resolution_clock::now();
        pause(h-(end1-start1).count()/std::pow(10,9));
        // std::cout << y[i]*180/M_PI << std::endl;
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << (end-start).count()/std::pow(10,9);
}