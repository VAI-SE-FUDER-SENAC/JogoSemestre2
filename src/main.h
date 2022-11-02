//
// Created by Arthur Andrade - home on 02/11/22.
//

#ifndef JOGOSEMESTRE2_MAIN_H
#define JOGOSEMESTRE2_MAIN_H

#include <allegro5/events.h>

int const WINDOW_WIDTH;
int const WINDOW_HEIGHT;
bool LIMIT_WALLS;

void drawStars();

void drawMenu();

void drawConfig();

void eventHandler(ALLEGRO_EVENT ev);

void render(ALLEGRO_EVENT ev);

void killNine();

#endif //JOGOSEMESTRE2_MAIN_H