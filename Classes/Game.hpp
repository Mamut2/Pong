#pragma once

#include <SFML\Graphics.hpp>
#include <iostream>
#include <cmath>
#include <time.h>
#include <stdlib.h> 

class Game
{
private:
    sf::RenderWindow* window;
    sf::VideoMode videoMode;
    sf::Event event;
    sf::RectangleShape player1, player2;
    sf::CircleShape ball;
    sf::Clock clock;
    sf::Text scoreText1;
    sf::Text scoreText2;
    sf::Font font;
    sf::Vector2i score;
    float deltaTime;
    sf::Vector2f initialBallVelocity;
    sf::Vector2f ballVelocity;

    const float playerSpeed = 400.f;
    const float textOffset = 70.f;
    const float ballSize = 10.f;
    const float initialBallSpeed = 600.f;
    const float ballSpeedGain = 100.f;
    const sf::Vector2f playerSize = sf::Vector2f(10.f, 100.f);
    float ballSpeed = 600.f;

    // Private Functions
    void InitWindow();
    void InitVar();
    void InitPlayers(sf::Vector2f, float, sf::Color, sf::Color, sf::Color, float);
    void MovePlayer(sf::RectangleShape&, sf::Vector2f);
    void ResetBall();
    void UpdateBall();
    void UpdateScore();
    void PoolEvents();
    sf::Vector2f Normalize(sf::Vector2f);
public:
    // Constructors / Deconstructors
    Game();

    // Accessors
    const bool IsRunning() const;

    // Functions
    void Update();
    void Render();
};
