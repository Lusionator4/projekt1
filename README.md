AUTOR: Franciszek Gębik
TEMAT: Gra "Kółko i Krzyżyk" z wykorzystaniem biblioteki SDL2

SPECYFIKACJA TECHNICZNA:
* Środowisko kompilacji: GCC (zalecane MinGW-w64 dla systemu Windows).
* Wykorzystane biblioteki: 
    - SDL2 (warstwa graficzna i obsługa zdarzeń).
    - SDL2_ttf (renderowanie czcionek TrueType).

ZASOBY I KODOWANIE:
* Formatowanie: Wszystkie pliki źródłowe są zakodowane w systemie UTF-8, co zapewnia poprawne działanie symboli i znaków specjalnych.
* Pliki zewnętrzne: Program wymaga dostępu do pliku czcionki 'font.ttf' znajdującego się w głównym katalogu roboczym.
* Biblioteki dynamiczne: Należy upewnić się, że pliki .dll z folderu 'SDL_assets' są dostępne dla kompilatora i pliku wykonywalnego.

STRUKTURA PROJEKTU:
* tictactoe.c: Główny moduł aplikacji zawierający logikę gry, silnik graficzny oraz algorytm sztucznej inteligencji.
* Makefile: Skrypt do automatyzacji procesu budowania projektu (kompilacja i linkowanie).
* font.ttf: Plik czcionki niezbędny do poprawnego działania interfejsu UI.

INSTRUKCJA BUDOWANIA PROGRAMU:
Po rozpakowaniu plików proszę wpisać w terminalu:
cd projekt-franka-main

A. Automatyczna kompilacja (rekomendowana):
W terminalu (w folderze z projektem) należy wywołać komendę:
   > make run
Spowoduje to skompilowanie kodu do pliku 'tictactoe.exe' i jego natychmiastowe uruchomienie.

B. Kompilacja ręczna (Windows/MinGW):
W przypadku braku narzędzia make, należy użyć bezpośredniego wywołania GCC:
   gcc tictactoe.c -o tictactoe.exe -L../SDL_assets/x86_64-w64-mingw32/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf
   ./tictactoe.exe

CECHY I FUNKCJONALNOŚCI:
* Skalowalność rozgrywki: Możliwość wyboru rozmiaru pola gry: 3x3, 4x4 oraz 5x5 bezpośrednio z poziomu menu.
* Zaawansowane AI: Przeciwnik komputerowy podejmuje decyzje w oparciu o algorytm Minimax z optymalizacją Alpha-Beta Pruning.
* Graficzny interfejs użytkownika: W pełni interaktywne menu główne z obsługą zdarzeń myszy (podświetlanie przycisków).
* System stanów gry: Czytelne komunikaty o turze gracza, procesie myślowym AI oraz końcowym werdykcie (wygrana/przegrana/remis).

UWAGI KOŃCOWE:
Dla poprawnego linkowania bibliotek, folder 'SDL_assets' musi znajdować się w katalogu nadrzędnym względem plików źródłowych, zgodnie ze ścieżką zdefiniowaną w Makefile.

