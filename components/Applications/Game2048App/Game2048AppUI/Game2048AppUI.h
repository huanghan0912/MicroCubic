#ifndef _GAME2048APPUI_H_
#define _GAME2048APPUI_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "lvgl.h"


void Game2048UIInit(void);



const lv_img_dsc_t* getN(int i);
void showBoard(int* map);
void showAnim(int* animMap, int direction);
void showNewBorn(int newborn, int *map);
void born(int i);

#ifdef __cplusplus
} /* extern "C" */
#endif


#ifdef __cplusplus
extern "C"
{
#endif

#include "lvgl.h"
    extern const lv_img_dsc_t app_game_2048;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif