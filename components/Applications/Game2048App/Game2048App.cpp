#include "Game2048App.h"
#include "esp_random.h"
#include <iostream>
#include "Game2048Controller.h"
#include "Game2048AppUI.h"
#define Game2048_APP_NAME "2048"

GAME2048 game;
static void Game2048Init(void)
{
    Game2048UIInit();
    game.init();

    int new1 = game.addRandom();
    int new2 = game.addRandom();
}


APPOBJ Game2048App = {Game2048_APP_NAME, &app_game_2048, "",
                        //  Game2048Init,Game2048Process,Game2048Background,
                        //  Game2048WeatherExit
                        };
