#include "game_winapi.h"
#include "piece_movement.h"

constexpr int TILE_SIZE = 80;

Game::Game() {
    instance = GetModuleHandle(nullptr);
    font = CreateFont(
        TILE_SIZE, 0, 0, 0,
        FW_NORMAL,
        FALSE, FALSE, FALSE,
        DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_SWISS,
        L"Arial"
    );
}

void Game::run() {
    WNDCLASS wc{};
    wc.lpfnWndProc = Game::wnd_proc;
    wc.hInstance = instance;
    wc.lpszClassName = L"ChessWindow";
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

    RegisterClass(&wc);

    RECT window_rect = {0, 0, TILE_SIZE * board.get_width(), TILE_SIZE * board.get_height()};
    DWORD window_style = WS_OVERLAPPEDWINDOW;
    AdjustWindowRectEx(&window_rect, window_style, FALSE, 0);

    hwnd = CreateWindow(
        wc.lpszClassName,
        L"Chess",
        window_style,
        CW_USEDEFAULT, CW_USEDEFAULT,
        window_rect.right - window_rect.left,
        window_rect.bottom - window_rect.top,
        nullptr, nullptr, instance, this
    );

    ShowWindow(hwnd, SW_SHOW);

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

LRESULT CALLBACK Game::wnd_proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    Game* game;

    if (msg == WM_NCCREATE) {
        CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
        game = static_cast<Game*>(cs->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(game));
        return TRUE;
    }

    game = reinterpret_cast<Game*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    if (!game) {
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    switch (msg) {
        case WM_LBUTTONDOWN:
            game->handle_click(LOWORD(lParam), HIWORD(lParam));
            InvalidateRect(hwnd, nullptr, TRUE);
            break;

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            game->draw_board(hdc);
            EndPaint(hwnd, &ps);
            break;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            break;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

void Game::draw_board(HDC hdc) {
    SelectObject(hdc, font);

    for (uint32_t row = 0; row < board.get_height(); row++) {
        for (uint32_t column = 0; column < board.get_width(); column++) {
            uint32_t square_index = row * board.get_width() + column;

            HBRUSH brush;
            if (square_index == selected_square) {
                brush = CreateSolidBrush(RGB(150, 240, 150));
            } else {
                bool white_square = (row + column) % 2;
                brush = CreateSolidBrush(
                    white_square ? RGB(240, 217, 181) : RGB(181, 136, 99)
                );
            }

            RECT square_rect {
                column * TILE_SIZE,
                row * TILE_SIZE,
                (column + 1) * TILE_SIZE,
                (row + 1) * TILE_SIZE
            };

            FillRect(hdc, &square_rect, brush);
            DeleteObject(brush);

            Piece::Type piece = board.get_square(square_index);

            if (piece != Piece::Type::None) {
                wchar_t piece_symbol = L' ';

                if (Piece::is_pawn(piece)) {
                    piece_symbol = L'♙';
                } else if (Piece::is_rook(piece)) {
                    piece_symbol = L'♖';
                } else if (Piece::is_knight(piece)) {
                    piece_symbol = L'♘';
                } else if (Piece::is_bishop(piece)) {
                    piece_symbol = L'♗';
                } else if (Piece::is_queen(piece)) {
                    piece_symbol = L'♕';
                } else if (Piece::is_king(piece)) {
                    piece_symbol = L'♔';
                }


                SetBkMode(hdc, TRANSPARENT);
                SetTextColor(hdc, Piece::is_white(piece) ? RGB(255,255,255) : RGB(0,0,0));

                TextOut(
                    hdc,
                    column * TILE_SIZE + 8,
                    row * TILE_SIZE,
                    &piece_symbol, 1
                );
            }
        }
    }
}

void Game::handle_click(int x, int y) {
    uint32_t column = x / TILE_SIZE;
    uint32_t row = y / TILE_SIZE;

    if (column < 0 || column >= board.get_width() || row < 0 || row >= board.get_height()) {
        return;
    }

    uint32_t square_index = row * board.get_width() + column;
    bool is_white_turn = board.state.is_white_turn();

    if (selected_square == -1) {
        Piece::Type piece = board.get_square(square_index);
        if (piece != Piece::Type::None && Piece::is_white(piece) == is_white_turn) {
            selected_square = square_index;
        }
        return;
    }

    PieceMovement move(board, selected_square);
    if (move.is_move_possible(square_index)) {
        board.make_move(selected_square, square_index);
    }

    selected_square = -1;
}
