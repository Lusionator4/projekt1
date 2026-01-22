AUTOR: Franciszek Gębik
TEMAT: Gra "Kółko i Krzyżyk" (SDL2)

1. WYMAGANIA I BIBLIOTEKI
    Kompilator: GCC (zalecane MinGW-w64).
    Biblioteki: SDL2 oraz SDL2_ttf.
    Ważne uwagi konfiguracyjne:
     Plik `font.ttf` musi znajdować się w tym samym katalogu co plik wykonywalny (`.exe`).
     Biblioteki `.dll` (z folderu `SDL_assets`) muszą być widoczne dla systemu.
     Folder `SDL_assets` powinien znajdować się katalog wyżej niż pliki źródłowe (jest to wymagane do poprawnego linkowania w Makefile).

2. STRUKTURA PROJEKTU
 `tictactoe.c` – Główny kod źródłowy gry (logika, AI, obsługa grafiki).
 `Makefile` – Skrypt do automatycznej kompilacji projektu.
 `font.ttf` – Plik czcionki wymagany do działania interfejsu.

 3. INSTRUKCJA URUCHOMIENIA
Należy wejść w terminalu do katalogu `projekt1-main`.

Opcja A: Automatyczna (Zalecana)
Wpisz w terminalu:
make run

Opcja B: Ręczna kompilacja (MinGW)
Jeżeli polecenie `make` nie jest dostępne, użyj poniższej komendy:
gcc tictactoe.c -o tictactoe.exe -L../SDL_assets/x86_64-w64-mingw32/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf

A następnie uruchom:
./tictactoe.exe

4. FUNKCJONALNOŚĆ
Rozmiar planszy:** Możliwość wyboru trybu 3x3, 4x4 lub 5x5 w menu głównym.
Sztuczna Inteligencja:** Przeciwnik komputerowy oparty na algorytmie Minimax z optymalizacją Alpha-Beta Pruning.
Interfejs:** Graficzne menu z obsługą myszki, komunikaty o stanie gry (tura gracza, wygrana, remis).
