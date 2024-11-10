#include "Classes\Game.hpp"

int main(){
    Game game;
    while (game.IsRunning())
    {
        game.Update();
        game.Render();
    }
}