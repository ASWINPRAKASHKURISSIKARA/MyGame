
#include <iostream>
#include "GameEngine.h"
#include "Scene_Title.h"

int main()
{
    GameEngine game("../config.txt");
    game.changeScene("TITLE", std::make_shared<Scene_Title>(&game));
    game.run();
    return 0;
}

