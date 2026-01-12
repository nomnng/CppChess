#pragma once

#include <windows.h>

#include "board.h"

class Game {
public:
    Game();
    void run();

private:
    Board board;
    bool is_game_running;

    HWND hwnd;
    HINSTANCE instance;
    HFONT font;

    int selected_square = -1;

    static LRESULT CALLBACK wnd_proc(HWND, UINT, WPARAM, LPARAM);

    void draw_board(HDC hdc);
    void handle_click(int x, int y);
};