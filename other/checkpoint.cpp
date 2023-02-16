#include <iostream>
#include <cmath>
#include <vector>
#include <chrono>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <string>

// g++ 3.cpp -o play -lsfml-graphics -lsfml-system -lsfml-window

float m = 1.0, g = 9.8, r = 1.0, I = 1.0, h = 0.001, l = 1.0;
float initial_angle = 175;
float nu = 44.5, H_0 = 0.1;
float duration = 60.0;

void pause(float dur) {
    auto start = std::chrono::high_resolution_clock::now();
    auto current = std::chrono::high_resolution_clock::now();
    for (;(current-start).count() < (long long)(dur*std::pow(10,9));current = std::chrono::high_resolution_clock::now()) {}
}

float f(float global_time) {
    return m * H_0 * nu * nu * std::cos(nu*global_time);
}

float H(float global_time) {
    return H_0*400/l * std::sin(nu*global_time);
}

float to_rad(float theta) {
    return theta*(M_PI/180);
}

float accel(float theta, float global_time) {
    return -(f(global_time)+m*g)*r*sin(theta)/I;
}

int main() {

    // I = (1.0f/3.0f) * m * l*l;
    std::vector<float> t;
    for (float i = 0; i < duration; i+=h) {
        t.push_back(i);
    }
    auto n = t.size();
    
    std::vector<float> theta(n, 0.0f);
    std::vector<float> omega(n, 0.0f);
    theta[0] = to_rad(initial_angle);
    omega[0] = 0.0f;

    sf::RenderWindow window(sf::VideoMode(1000, 1000), "pendulum");
    sf::RectangleShape rectangle(sf::Vector2f(10, 400));
    sf::Font font;
    font.loadFromFile("Roboto-Black.ttf");
    sf::Text text_omega("Omega", font, 30);
    text_omega.setFillColor(sf::Color::White);
    // window.setFramerateLimit(60);
    rectangle.setFillColor(sf::Color::White);
    rectangle.setOrigin(5,10);
    rectangle.rotate(initial_angle);
    rectangle.setPosition(500, 500);

    sf::CircleShape bob(20);
    bob.setFillColor(sf::Color::White);
    bob.setOrigin(20, -380);
    bob.rotate(initial_angle);
    bob.setPosition(500, 500);

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

        float k1_theta = h*omega[i];
        float k1_w = h*accel(theta[i], global_time);

        float k2_theta = h*(omega[i]+0.5*k1_w);
        float k2_w = h*accel(theta[i]+0.5*k1_theta, global_time+h/2);

        float k3_theta = h*(omega[i]+0.5*k2_w);
        float k3_w = h*accel(theta[i]+0.5*k2_theta, global_time+h/2);

        float k4_theta = h*(omega[i]+k3_w);
        float k4_w = h*accel(theta[i]+k3_theta, global_time);

        theta[i+1] = theta[i] + (k1_theta + 2 * k2_theta + 2 * k3_theta + k4_theta) / 6.0f; 
        omega[i+1] = omega[i] + (k1_w + 2 * k2_w + 2 * k3_w + k4_w) / 6.0f;

        // std::cout << perfect_theta(global_time) << ' ' << global_time << std::endl; 
        rectangle.rotate(theta[i+1]*180/M_PI-theta[i]*180/M_PI);
        bob.rotate(theta[i+1]*180/M_PI-theta[i]*180/M_PI);
        // rectangle.setPosition(500, 500 + H(global_time));
        // bob.setPosition(500, 500 + H(global_time));
        window.clear();
        window.draw(rectangle);
        window.draw(bob);
        text_omega.setString(std::to_string(omega[i])+" "+std::to_string(global_time)+" "+std::to_string(theta[i]));
        window.draw(text_omega);
        window.display();

        auto end1 = std::chrono::high_resolution_clock::now();
        pause(h-(end1-start1).count()/std::pow(10,9));
        // std::cout << y[i]*180/M_PI << std::endl;
    }

    std::ofstream create;
    create.open("kapitzas_thetas.txt");
    create << '\n';
    create.close();
    std::ofstream out;
    out.open("kapitzas_thetas.txt", std::ios::app);
    for (int i = 0; i < n; i++) {
        out << std::to_string(h*i) << " " << std::to_string(theta[i]) << " " << std::to_string(omega[i]) << std::endl;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::cout << (end-start).count()/std::pow(10,9) << std::endl;
}