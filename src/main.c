
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <src/main.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include "innerIncludes/headers/nossaLivraria.h"
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include "innerIncludes/headers/Cores.h"
#include "innerIncludes/headers/gameCore.h"
#include "outIncludes/headers/algif.h"


// ========== Window VARS ===========================================
int const WINDOW_WIDTH          = 1280;
int const WINDOW_HEIGHT         = 720;
bool LIMIT_WALLS                = false;
// =================================================================

// ========== Frame VARS ===========================================
bool GAME_FREQUENCY_POLARITY    = false;
float const GAME_FREQUENCY      = 60; // Quantos ciclos de atualizacao acontecem no jogo
float const MPS                 = GAME_FREQUENCY;  // Maximo de vezes o jogo é renderizado
float FPS                       = 0;   // Guarda quantas de vezes o jogo está sendo renderizado
bool allow                      = true;
bool SHOW_FPS                   = false;
long global_counter;
// ==================================================================


ALLEGRO_BITMAP *astro, *tittleWorbit, *tittleWelcome, *lifeHeart;
ALLEGRO_EVENT_QUEUE *event_queue, *timer_queue;
ALLEGRO_FONT *font25 , *font90;
ALLEGRO_DISPLAY *display;
ALGIF_ANIMATION *tuto;
ALLEGRO_TIMER* timer;
ALLEGRO_FONT *font25 , *font90, *font45;

GAMEMODE GAMESTATE;
bool orderRedraw = true;

int main() {

    // Inicia allegro
    al_init();

    // Inicia paleta de cores
    init_colors();

    // Carrega as imagens do jogo
    if (al_init_image_addon()) {
        astro = al_load_bitmap("../src/assets/astronauta.png");
        tittleWorbit = al_load_bitmap("../src/assets/worbit.png");
        tittleWelcome = al_load_bitmap("../src/assets/welcome.png");
        lifeHeart = al_load_bitmap("../src/assets/vida.png");
    }

    const char *gif = "../src/assets/tutorial/giphy.gif";
    tuto = algif_load_animation(gif);

    // Inicia biblioteca de primitives
    al_init_primitives_addon();

    // Carrega as fontes do jogo
    if (al_init_font_addon() && al_init_ttf_addon()) {
        font25 = al_load_ttf_font("../src/assets/fonts/Bungee-Regular.ttf",25,0 );
        font45 = al_load_ttf_font("../src/assets/fonts/Bungee-Regular.ttf",45,0 );
    }

    // Inicia constante de newton
    initGame();

    // Inicia pilha de eventos do allegro
    event_queue = al_create_event_queue();
    timer_queue = al_create_event_queue();

    // Inicia event listener do mouse
    al_install_mouse();
    al_register_event_source(event_queue, al_get_mouse_event_source());

    // Inicia event listener do teclado
    al_install_keyboard();
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    // Inicia display
    display = al_create_display(WINDOW_WIDTH, WINDOW_HEIGHT);
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_clear_to_color(BLACK);

    // Inicia loops por GAME_FREQUENCY
    timer = al_create_timer(1.0 / GAME_FREQUENCY);
    al_register_event_source(timer_queue, al_get_timer_event_source(timer));
    al_start_timer(timer);

    while (1) {
        ALLEGRO_EVENT ev;

        do {
            al_wait_for_event(event_queue, &ev); // ESPERANDO POR EVENTOS
            if (ev.type != ALLEGRO_EVENT_MOUSE_AXES) {
                eventHandler(ev);
                render(ev);
            }
        } while (!al_is_event_queue_empty(event_queue));

        while(al_is_event_queue_empty(event_queue)) {
            al_wait_for_event(timer_queue, &ev); // ESPERANDO POR EVENTOS
            render(ev);
        }
    }
}

void eventHandler(ALLEGRO_EVENT ev) {

    switch (ev.type) {
        case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN: {

            // EVENTO CLICK
            switch (GAMESTATE) {
                case MENU: {
                    // BOTÕES DO MENU
                    if (ev.mouse.x >= (WINDOW_WIDTH/2)-200 && ev.mouse.x <= (WINDOW_WIDTH/2)+200) {
                        if (ev.mouse.y >= 440 && ev.mouse.y <= 490) {     // play
                            orderRedraw = true;
                            GAMESTATE = TUTORIAL;
                        } else if (ev.mouse.y >= 520 && ev.mouse.y <= 570) {     // config
                            orderRedraw = true;
                            GAMESTATE = CONFIG;
                        } else if(ev.mouse.y >= 600 && ev.mouse.y <= 670) {             // quit
                            killNine();
                        }
                    }
                    break;
                }
                case PLAY: {
                    // BOTÕES DA TELA PLAY
                    if (!b.active) {
                        setBulletTo(ev.mouse.x, ev.mouse.y);
                    }

                    break;
                }
                case TUTORIAL: {
                    if (ev.mouse.x >= 30 && ev.mouse.x <= 230 && ev.mouse.y >= 30 && ev.mouse.y <= 80) {
                        orderRedraw = true;
                        GAMESTATE = PLAY; // RETORNA A TELA DE MENU
                    }
                    break;
                }
                case CONFIG: {
                    // BOTÕES DA TELA CONFIG

                    if (ev.mouse.x >= 30 && ev.mouse.x <= 230 && ev.mouse.y >= 30 && ev.mouse.y <= 80) {
                        orderRedraw = true;
                        GAMESTATE = MENU; // RETORNA A TELA DE MENU
                    }
                    break;
                }
                default: {
                    break;
                }
            }
            break;
        }
        case ALLEGRO_EVENT_KEY_DOWN: {
            if (ev.keyboard.keycode == 59) {
                orderRedraw = true;
                GAMESTATE = MENU; // RETORNA A TELA DE MENU
            }
            break;
        }
        case ALLEGRO_EVENT_DISPLAY_CLOSE: {
            killNine();
            break;
        }
    }
}

void render(ALLEGRO_EVENT ev) {
    if (ev.type == ALLEGRO_EVENT_TIMER) {
        GAME_FREQUENCY_POLARITY = !GAME_FREQUENCY_POLARITY; // POLARIDADE DO GAME_FREQUENCY
        switch (GAMESTATE) {
            case MENU: {
                if (orderRedraw) {
                    drawMenu();
                    orderRedraw = false;
                }
                break;
            }
            case PLAY: {
                drawGame();
                break;
            }
            case TUTORIAL: {
                if (GAME_FREQUENCY_POLARITY) {
                    drawTutorial();
                    if (orderRedraw) {
                        printf(" - Drawing Tutorial....[%s]\n", getNow());
                        orderRedraw = false;
                    }
                }
                break;
            }
            case CONFIG: {
                // TELA CONFIG
                if (orderRedraw) {
                    drawConfig();
                    orderRedraw = false;
                }
                break;
            }
            default: {
                break;
            }
        }
    }
}

void drawStars(){

    for (int i = 0; i < 90; ++i) {
        int randomX = getRandomInt(WINDOW_WIDTH, 0);
        int randomY = getRandomInt(WINDOW_HEIGHT, 0);
        int randomR = getRandomInt(4, 0);
        al_draw_filled_circle((float) randomX, (float) randomY, (float) randomR, WHITE);
    }
}

void drawMenu() {
    // TELA DO MENU
    al_clear_to_color(BLACK);

    //DESENHA ESTRELAS
        drawStars();

    //SOMBRA OPÇÕES MENU
    insertFilledSquare(50, 400, (WINDOW_WIDTH/2)-190, 450, DARK_PURPLE, display);
    insertFilledSquare(50, 400, (WINDOW_WIDTH/2)-190, 530, DARK_PURPLE, display);
    insertFilledSquare(50, 400, (WINDOW_WIDTH/2)-190, 610, DARK_PURPLE, display);

    //OPÇÕES MENU
    insertFilledSquare(50, 400, (WINDOW_WIDTH/2)-200, 440, LIGHT_PURPLE, display);
    insertFilledSquare(50, 400, (WINDOW_WIDTH/2)-200, 520, LIGHT_PURPLE, display);
    insertFilledSquare(50, 400, (WINDOW_WIDTH/2)-200, 600, LIGHT_PURPLE, display);

    //FONTE MENU
    al_draw_text( font25, WHITE, (float) (WINDOW_WIDTH/2.0)-30, 455, 0, "Play");
    al_draw_text( font25, WHITE, (float) (WINDOW_WIDTH/2.0)-42, 535, 0, "Config");
    al_draw_text( font25, WHITE, (float) (WINDOW_WIDTH/2.0)-30, 615, 0, "Quit");

    //IMAGENS MENU
    al_draw_bitmap(astro, 870, 150, 0);
    al_draw_bitmap(tittleWorbit, (float) (WINDOW_WIDTH/2.0)-250, 80, 0);
    insertFilledSquare(17, 150, (WINDOW_WIDTH/2)-75, 30, BLACK, display);
    al_draw_bitmap(tittleWelcome, (float) (WINDOW_WIDTH/2.0)-75, 30, 0);

    printf(" - Drawing MENU....[%s]\n", getNow());
    al_flip_display();
}

void drawTutorial() {
// TELA DE TUROTIAL
    al_clear_to_color(BLACK);

    al_draw_bitmap(algif_get_bitmap(tuto, al_get_time()), WINDOW_WIDTH/2 - 110, WINDOW_HEIGHT/2 - 110, 0);

    insertFilledSquare(50, 200, 40, 40, DARK_PURPLE, display);
    insertFilledSquare(50, 200, 30, 30, LIGHT_PURPLE, display);

    al_draw_text(font25, WHITE, 90, 40, 0, "Skip");

    al_flip_display();
}

void drawConfig() {
    // TELA DE CONFIGURAÇÕES
    al_clear_to_color(BLACK);

    drawStars();

    insertFilledSquare(50, 200, 40, 40, DARK_PURPLE, display);
    insertFilledSquare(50, 200, 30, 30, LIGHT_PURPLE, display);

    al_draw_text(font25, WHITE, 90, 40, 0, "Back");

    printf(" - Drawing SETTINGS....[%s]\n", getNow());
    al_flip_display();
}

void drawGame(){
    //TELA DO JOGO
    al_clear_to_color(BLACK);
    moveBall();
    readCreatePlanetsBullets();
    drawLifeBar();

    if(player1.life != 0){
        al_draw_filled_circle((float)player1.coordX, (float)player1.coordY, (float)player1.radius, LIGHT_BLUE);
    }

    if(player2.life != 0){
        al_draw_filled_circle((float)player2.coordX, (float)player2.coordY, (float)player2.radius, RED);
    }

    if(gameRound){
        al_draw_text( font45, LIGHT_BLUE, 400, 25, 0, "- VEZ DO JOGADOR 1 -");
    }else{
        al_draw_text( font45, RED, 400, 25, 0, "- VEZ DO JOGADOR 2 -");
    }
    al_flip_display();


    //printf(" - Drawing Play Screen....[%s]\n", getNow());
}

//desenha os corações das vidas dos bonecos
void drawLifeBar(){
    int t = 30;
    for(int i = 0; i < player1.life; i++){
        al_draw_bitmap(lifeHeart, (float) t, 20, 0);
        t += 30;
    }

    int w = 1220;
    for(int i = 0; i < player2.life; i++){
        al_draw_bitmap(lifeHeart, (float) w, 20, 0);
        w -= 30;
    }
}

//void drawSprite(){
//    if(player1.coordX < WINDOW_WIDTH/2 || player2.coordX > WINDOW_WIDTH){
//        al_draw_bitmap(SpritRight, player1.coordX, player1.coordY, 0);
//    } else{
//        al_draw_bitmap(SpritLeft, player1.coordX, player1.coordY, 0);
//    }
//
//    if(player2.coordX < WINDOW_WIDTH){
//        al_draw_bitmap(SpritRight, player1.coordX, player1.coordY, 0);
//    } else{
//        al_draw_bitmap(SpritLeft, player1.coordX, player1.coordY, 0);
//    }
//}

void killNine() {
    printf(" - Killing APP....[%s]\n", getNow());
    al_destroy_bitmap(tittleWelcome);
    al_destroy_bitmap(tittleWorbit);
    al_destroy_bitmap(astro);
    al_destroy_bitmap(lifeHeart);
    algif_destroy_animation(tuto);
    al_destroy_event_queue(timer_queue);
    al_destroy_event_queue(event_queue);
    al_destroy_display(display);
    al_destroy_timer(timer);

    exit(0);
}