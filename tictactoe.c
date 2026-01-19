#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 900
#define GRID_OFFSET 150

const SDL_Color COLOR_BG = {25, 25, 35, 255};      
const SDL_Color COLOR_GRID = {60, 63, 90, 255};    
const SDL_Color COLOR_X = {0, 255, 200, 255};      
const SDL_Color COLOR_O = {255, 50, 150, 255};     
const SDL_Color COLOR_TEXT = {240, 240, 240, 255}; 
const SDL_Color COLOR_ACCENT = {255, 215, 0, 255}; 

typedef enum { EMPTY = 0, PLAYER_X = 1, PLAYER_O = 2 } Cell;
typedef enum { MENU, GAME, GAME_OVER } State;

typedef struct {
    int board_size;
    Cell board[5][5];
    State state;
    Cell winner;
    bool player_turn;
    int mouse_x, mouse_y;
} Game;

bool init(SDL_Window** window, SDL_Renderer** renderer, TTF_Font** font, TTF_Font** small_font) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) return false;
    if (TTF_Init() == -1) return false;
    
    *window = SDL_CreateWindow("Kółko i Krzyżyk Pro", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!*window) return false;
    
    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!*renderer) return false;

    // Zmniejszone rozmiary czcionek
    *font = TTF_OpenFont("font.ttf", 60); 
    *small_font = TTF_OpenFont("font.ttf", 28);
    if (!*font || !*small_font) {
        printf("Błąd: Nie można załadować font.ttf!\n");
        return false;
    }
    return true;
}

Cell check_winner(Game* g) {
    int n = g->board_size;
    for (int i = 0; i < n; i++) {
        bool row_win = true, col_win = true;
        for (int j = 1; j < n; j++) {
            if (g->board[i][j] == EMPTY || g->board[i][j] != g->board[i][0]) row_win = false;
            if (g->board[j][i] == EMPTY || g->board[j][i] != g->board[0][i]) col_win = false;
        }
        if (row_win && g->board[i][0] != EMPTY) return g->board[i][0];
        if (col_win && g->board[0][i] != EMPTY) return g->board[0][i];
    }
    bool d1 = true, d2 = true;
    for (int i = 1; i < n; i++) {
        if (g->board[i][i] == EMPTY || g->board[i][i] != g->board[0][0]) d1 = false;
        if (g->board[i][n-1-i] == EMPTY || g->board[i][n-1-i] != g->board[0][n-1]) d2 = false;
    }
    if (d1 && g->board[0][0] != EMPTY) return g->board[0][0];
    if (d2 && g->board[0][n-1] != EMPTY) return g->board[0][n-1];
    for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) if (g->board[i][j] == EMPTY) return EMPTY;
    return 3;
}


int minimax(Game* g, int depth, bool is_max, int alpha, int beta) {
    Cell res = check_winner(g);
    if (res == PLAYER_O) return 100 - depth;
    if (res == PLAYER_X) return depth - 100;
    if (res == 3) return 0;
    if (g->board_size > 3 && depth > 5) return 0;

    if (is_max) {
        int best = INT_MIN;
        for (int i = 0; i < g->board_size; i++) {
            for (int j = 0; j < g->board_size; j++) {
                if (g->board[i][j] == EMPTY) {
                    g->board[i][j] = PLAYER_O;
                    int score = minimax(g, depth + 1, false, alpha, beta);
                    g->board[i][j] = EMPTY;
                    if (score > best) best = score;
                    if (best > alpha) alpha = best;
                    if (beta <= alpha) break;
                }
            }
        }
        return best;
    } else {
        int best = INT_MAX;
        for (int i = 0; i < g->board_size; i++) {
            for (int j = 0; j < g->board_size; j++) {
                if (g->board[i][j] == EMPTY) {
                    g->board[i][j] = PLAYER_X;
                    int score = minimax(g, depth + 1, true, alpha, beta);
                    g->board[i][j] = EMPTY;
                    if (score < best) best = score;
                    if (best < beta) beta = best;
                    if (beta <= alpha) break;
                }
            }
        }
        return best;
    }
}

void ai_move(Game* g) {
    int best = INT_MIN, r = -1, c = -1;
    for (int i = 0; i < g->board_size; i++) {
        for (int j = 0; j < g->board_size; j++) {
            if (g->board[i][j] == EMPTY) {
                g->board[i][j] = PLAYER_O;
                int score = minimax(g, 0, false, INT_MIN, INT_MAX);
                g->board[i][j] = EMPTY;
                if (score > best) { best = score; r = i; c = j; }
            }
        }
    }
    if (r != -1) { g->board[r][c] = PLAYER_O; g->player_turn = true; }
}


void draw_text(SDL_Renderer* res, TTF_Font* font, const char* text, int x, int y, SDL_Color color, bool center) {
    SDL_Surface* surf = TTF_RenderUTF8_Blended(font, text, color);
    if (!surf) return;
    SDL_Texture* tex = SDL_CreateTextureFromSurface(res, surf);
    SDL_Rect rect = { center ? x - surf->w/2 : x, y, surf->w, surf->h };
    SDL_RenderCopy(res, tex, NULL, &rect);
    SDL_FreeSurface(surf);
    SDL_DestroyTexture(tex);
}

void draw_button(SDL_Renderer* res, TTF_Font* font, const char* text, int y, bool hovered) {
    SDL_Rect btn = { SCREEN_WIDTH/2 - 150, y, 300, 60 };
    SDL_SetRenderDrawColor(res, hovered ? 70 : 45, hovered ? 75 : 50, hovered ? 110 : 80, 255);
    SDL_RenderFillRect(res, &btn);
    SDL_SetRenderDrawColor(res, hovered ? 255 : 100, hovered ? 215 : 100, hovered ? 0 : 100, 255);
    SDL_RenderDrawRect(res, &btn);
    draw_text(res, font, text, SCREEN_WIDTH/2, y + 15, COLOR_TEXT, true);
}

void render(SDL_Renderer* res, TTF_Font* font, TTF_Font* small_font, Game* g) {
    SDL_SetRenderDrawColor(res, COLOR_BG.r, COLOR_BG.g, COLOR_BG.b, 255);
    SDL_RenderClear(res);

    if (g->state == MENU) {
        draw_text(res, font, "KÓŁKO I KRZYŻYK", SCREEN_WIDTH/2, 100, COLOR_TEXT, true);
        
        draw_button(res, small_font, "Plansza 3x3", 350, (g->mouse_x > 250 && g->mouse_x < 550 && g->mouse_y > 350 && g->mouse_y < 410));
        draw_button(res, small_font, "Plansza 4x4", 430, (g->mouse_x > 250 && g->mouse_x < 550 && g->mouse_y > 430 && g->mouse_y < 490));
        draw_button(res, small_font, "Plansza 5x5", 510, (g->mouse_x > 250 && g->mouse_x < 550 && g->mouse_y > 510 && g->mouse_y < 570));
    } else {
        int cell_size = SCREEN_WIDTH / g->board_size;
        int grid_h = SCREEN_HEIGHT - GRID_OFFSET;

        SDL_SetRenderDrawColor(res, COLOR_GRID.r, COLOR_GRID.g, COLOR_GRID.b, 255);
        for (int i = 1; i < g->board_size; i++) {
            SDL_Rect v_line = { i * cell_size - 2, GRID_OFFSET, 4, grid_h };
            SDL_Rect h_line = { 0, GRID_OFFSET + i * grid_h / g->board_size - 2, SCREEN_WIDTH, 4 };
            SDL_RenderFillRect(res, &v_line);
            SDL_RenderFillRect(res, &h_line);
        }

        for (int i = 0; i < g->board_size; i++) {
            for (int j = 0; j < g->board_size; j++) {
                int cx = j * cell_size + cell_size / 2;
                int cy = GRID_OFFSET + i * grid_h / g->board_size + grid_h / (2 * g->board_size);
                if (g->board[i][j] == PLAYER_X) draw_text(res, font, "X", cx, cy - 40, COLOR_X, true);
                else if (g->board[i][j] == PLAYER_O) draw_text(res, font, "O", cx, cy - 40, COLOR_O, true);
            }
        }

        if (g->state == GAME_OVER) {
            const char* msg;
            SDL_Color msg_color;
            if (g->winner == 3) { msg = "REMIS"; msg_color = COLOR_TEXT; }
            else if (g->winner == PLAYER_X) { msg = "WYGRANA"; msg_color = COLOR_X; }
            else { msg = "PRZEGRANA"; msg_color = COLOR_O; }
            
            draw_text(res, small_font, msg, SCREEN_WIDTH/2, 30, msg_color, true);
            draw_text(res, small_font, "Kliknij pole, aby wrócić", SCREEN_WIDTH/2, 80, COLOR_TEXT, true);
        } else {
            draw_text(res, small_font, g->player_turn ? "TWÓJ RUCH" : "AI MYŚLI...", SCREEN_WIDTH/2, 55, COLOR_TEXT, true);
        }
    }
    SDL_RenderPresent(res);
}

int main(int argc, char* argv[]) {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    TTF_Font *font = NULL, *small_font = NULL;

    if (!init(&window, &renderer, &font, &small_font)) return 1;

    Game game = { .state = MENU, .board_size = 3 };
    bool running = true;
    SDL_Event e;

    while (running) {
        SDL_GetMouseState(&game.mouse_x, &game.mouse_y);
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = false;
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                if (game.state == MENU) {
                    if (game.mouse_x > 250 && game.mouse_x < 550) {
                        if (game.mouse_y > 350 && game.mouse_y < 410) { game.board_size = 3; game.state = GAME; }
                        else if (game.mouse_y > 430 && game.mouse_y < 490) { game.board_size = 4; game.state = GAME; }
                        else if (game.mouse_y > 510 && game.mouse_y < 570) { game.board_size = 5; game.state = GAME; }
                        if (game.state == GAME) {
                            for(int i=0; i<5; i++) for(int j=0; j<5; j++) game.board[i][j] = EMPTY;
                            game.player_turn = true;
                        }
                    }
                } else if (game.state == GAME && game.player_turn) {
                    int cs = SCREEN_WIDTH / game.board_size;
                    int gh = SCREEN_HEIGHT - GRID_OFFSET;
                    int row = (game.mouse_y - GRID_OFFSET) / (gh / game.board_size);
                    int col = game.mouse_x / cs;
                    if (row >= 0 && row < game.board_size && col >= 0 && col < game.board_size && game.board[row][col] == EMPTY) {
                        game.board[row][col] = PLAYER_X;
                        game.player_turn = false;
                        Cell win = check_winner(&game);
                        if (win != EMPTY) { game.state = GAME_OVER; game.winner = win; }
                    }
                } else if (game.state == GAME_OVER) {
                    game.state = MENU;
                }
            }
        }
        if (game.state == GAME && !game.player_turn) {
            render(renderer, font, small_font, &game);
            ai_move(&game);
            Cell win = check_winner(&game);
            if (win != EMPTY) { game.state = GAME_OVER; game.winner = win; }
        }
        render(renderer, font, small_font, &game);
    }

    TTF_CloseFont(font); TTF_CloseFont(small_font);
    SDL_DestroyRenderer(renderer); SDL_DestroyWindow(window);
    TTF_Quit(); SDL_Quit();
    return 0;
}