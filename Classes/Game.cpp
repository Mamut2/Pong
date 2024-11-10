#include "Game.hpp"

#define UP sf::Vector2f(0.f, -1.f)
#define DOWN sf::Vector2f(0.f, 1.f)

// Private Functions
void Game::InitWindow()
{
    this->videoMode = sf::VideoMode::getDesktopMode();
    this->window = new sf::RenderWindow(this->videoMode, "Pong", sf::Style::Fullscreen);
}

void Game::InitVar()
{
    std::srand(std::time(NULL));
    this->deltaTime = 0.f;

    // Font
    if(!this->font.loadFromFile("square-deal.ttf")) return;
    this->score = sf::Vector2i(0, 0);
    this->scoreText1.setFont(font);
    this->scoreText1.setCharacterSize(50);
    this->scoreText2.setFont(font);
    this->scoreText2.setCharacterSize(50);
    this->UpdateScore();
}

void Game::InitPlayers(sf::Vector2f _playerSize, float _ballRadius, sf::Color _color1, sf::Color _color2, sf::Color _ballColor, float _padding)
{
    this->player1.setSize(_playerSize);
    this->player2.setSize(_playerSize);
    this->player1.setOrigin(sf::Vector2f(_playerSize.x / 2.f, _playerSize.y / 2.f));
    this->player2.setOrigin(sf::Vector2f(_playerSize.x / 2.f, _playerSize.y / 2.f));
    this->player1.setFillColor(_color1);
    this->player2.setFillColor(_color2);
    this->player1.setPosition(sf::Vector2f(_padding + _playerSize.x / 2.f, this->videoMode.height / 2.f));
    this->player2.setPosition(sf::Vector2f(this->videoMode.width - (_padding + _playerSize.x / 2.f), this->videoMode.height / 2.f));

    this->ball.setRadius(_ballRadius);
    this->ball.setFillColor(_ballColor);
    this->ball.setOrigin(sf::Vector2f(_ballRadius, _ballRadius));
    this->ResetBall();
}

void Game::MovePlayer(sf::RectangleShape& _player, sf::Vector2f _direction)
{
    _player.move(_direction * this->playerSpeed * this->deltaTime);
}

void Game::ResetBall()
{
    this->ballSpeed = this->initialBallSpeed;
    this->ball.setPosition(sf::Vector2f(this->videoMode.width / 2.f, this->videoMode.height / 2.f));
    int coinFlip = rand() % 2;
    if(coinFlip == 0) this->initialBallVelocity = sf::Vector2f(-this->initialBallSpeed, 0.f);
    else this->initialBallVelocity = sf::Vector2f(this->initialBallSpeed, 0.f);
    this->ballVelocity = this->initialBallVelocity;
}

void Game::UpdateBall()
{
    sf::Vector2f ballPos = this->ball.getPosition();
    float ballRad = this->ballSize;
    if(ballPos.x + ballRad >= this->player2.getPosition().x - this->playerSize.x) 
    {
        if(ballPos.y + ballRad >= this->player2.getPosition().y - this->playerSize.y &&
            ballPos.y - ballRad <= this->player2.getPosition().y + this->playerSize.y)
        {
            sf::Vector2f dir = ballPos - this->player2.getPosition();
            this->ballVelocity.x *= -1.f;
            this->ballVelocity = this->Normalize(this->Normalize(this->ballVelocity) + dir) * this->ballSpeed;
            this->ballSpeed += this->ballSpeedGain;
        }    
        else
        {
            this->score.x++;
            this->UpdateScore();
            this->ResetBall();
        }
    }
    if(ballPos.x - ballRad <= this->player1.getPosition().x + this->playerSize.x) 
    {
        if(ballPos.y + ballRad >= this->player1.getPosition().y - this->playerSize.y &&
            ballPos.y - ballRad <= this->player1.getPosition().y + this->playerSize.y) 
        {
            sf::Vector2f dir = ballPos - this->player1.getPosition();
            this->ballVelocity.x *= -1.f;
            this->ballVelocity = this->Normalize(this->Normalize(this->ballVelocity) + dir) * this->ballSpeed;
            this->ballSpeed += this->ballSpeedGain;
        }
        else
        {
            this->score.y++;
            this->UpdateScore();
            this->ResetBall();
        }
    }

    // Keep in bounds
    if(ballPos.x + ballRad >= this->videoMode.width || ballPos.x - ballRad <= 0.f) this->ballVelocity.x *= -1.f;
    if(ballPos.y + ballRad >= this->videoMode.height || ballPos.y - ballRad <= 0.f) this->ballVelocity.y *= -1.f;

    this->ball.move(this->ballVelocity * this->deltaTime);
}

void Game::UpdateScore()
{
    this->scoreText1.setString(std::to_string(this->score.x));
    this->scoreText2.setString(std::to_string(this->score.y));
    this->scoreText1.setOrigin(sf::Vector2f(this->scoreText1.getLocalBounds().width / 2.f, this->scoreText1.getLocalBounds().height / 2.f));
    this->scoreText1.setPosition(this->videoMode.width / 2.f - this->textOffset, 20.f);
    this->scoreText2.setOrigin(sf::Vector2f(this->scoreText2.getLocalBounds().width / 2.f, this->scoreText2.getLocalBounds().height / 2.f));
    this->scoreText2.setPosition(this->videoMode.width / 2.f + this->textOffset, 20.f);
}

void Game::PoolEvents()
{
    while (this->window->pollEvent(this->event))
    {
        switch (this->event.type)
        {
        case sf::Event::Closed:
            this->window->close();
            break;
        case sf::Event::KeyPressed:
            if(this->event.key.code == sf::Keyboard::Escape)
                this->window->close();
        default:
            break;
        }
    }

    float p1 = this->player1.getPosition().y;
    float p2 = this->player2.getPosition().y;

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::W) && p1 - this->player1.getSize().y / 2.f >= 0.f) MovePlayer(this->player1, UP);
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S) && p1 + this->player1.getSize().y / 2.f <= this->videoMode.height) MovePlayer(this->player1, DOWN);

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && p2 - this->player2.getSize().y / 2.f >= 0.f) MovePlayer(this->player2, UP);
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && p2 + this->player2.getSize().y / 2.f <= this->videoMode.height) MovePlayer(this->player2, DOWN);
}

sf::Vector2f Game::Normalize(sf::Vector2f _vec)
{
    float mag = sqrt(_vec.x * _vec.x + _vec.y * _vec.y);
    return sf::Vector2f(_vec.x / mag, _vec.y / mag);
}

// Constructors / Deconstructors
Game::Game()
{
    this->InitWindow();
    this->InitVar();
    this->InitPlayers(this->playerSize, this->ballSize, sf::Color::Blue, sf::Color::Red, sf::Color::White, 20.f);
}

// Accessors
const bool Game::IsRunning() const
{
    return this->window->isOpen();
}

// Functions
void Game::Update()
{
    this->PoolEvents();
    this->UpdateBall();
    this->deltaTime = this->clock.getElapsedTime().asSeconds();
    this->clock.restart();
}

void Game::Render()
{
    this->window->clear();

    sf::RectangleShape midLine(sf::Vector2f(10.f, this->videoMode.height));
    midLine.setOrigin(sf::Vector2f(midLine.getSize().x / 2.f, midLine.getSize().y / 2.f));
    midLine.setPosition(sf::Vector2f(this->videoMode.width / 2.f, this->videoMode.height / 2.f));
    midLine.setFillColor(sf::Color(255, 255, 255, 100));
    this->window->draw(midLine);

    this->window->draw(this->scoreText1);
    this->window->draw(this->scoreText2);

    this->window->draw(this->ball);
    this->window->draw(this->player1);
    this->window->draw(this->player2);

    this->window->display();
}