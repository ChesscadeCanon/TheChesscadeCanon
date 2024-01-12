Chesscade is an open source, deterministic, perfect-information, falling block puzzle game with
chess pieces. For complete rules, see "rules.h". The Chesscade Canon seeks to be a universal,
canonical implementation of Chesscade, in the form of a lightweight, thread-safe, optimized
dynamic library that can be used as a backend to graphical interfaces written in any language,
as well as AI engines for experimentation. Included here is an extremely basic frontend
written in Python using the PyGame engine.

# To play on Replit (THE EASY WAY):

1. In a web browser, navigate to https://www.replit.com and create an account.
2. Navigate to https://replit.com/@chesscadecanon/TheChesscadeCanon
2. Click the "Fork" button in the upper right hand corner.
3. In the popup that appears, click the "Fork Repl" button.
4. The game will appear in a tab within Replit called "Output".
5. Click the game and it will begin responding to keyboard input.

# To play on Windows using Visual Studio:

1. Install Visual Studio, including the Desktop Development with C++ workload and the Python Development workload.
2. Install Pip and PyGame.
3. Open Visual Studio.
4. Under "Get Started" click "Clone a Repository".
5. For the repository location, enter https://github.com/ChesscadeCanon/TheChesscadeCanon
6. Choose a path to download into.
7. Click "Clone". VS will open the "pygame_frontend" project, because that's the part that has an .sln file.
8. Go to "File->Open->Folder..."
9. Navigate to the ChesscadeBox directory.
10. Click "Open".
11. In the Solution Explorer, select CMakeLists.txt.
12. Go to "Build->Rebuild All".
13. If the build is successful, go to "File->Open->Project/Solution...".
14. Open "ChesscadeBox\pygame_frontend\pygame_frontend.sln".
15. Make sure there is a "ChesscadeBox\pygame_frontend\bin" folder containing a file called "ChesscadeLib.dll".
16. If not, make such a folder and copy ChesscadeLib.dll into it from ChesscadeBox\out\build\x64-Debug.
17. The green play button at the top of the screen should now launch the game.

# To play on Unix:

1. Install Git, CMake, Pip, and PyGame.
2. In a terminal:
```
$ git clone https://github.com/ChesscadeCanon/TheChesscadeCanon
$ cd ChesscadeBox
$ cmake -S . -B out/build
$ cmake --build out/build
$ cp out/build/libChesscadeLib.so pygame_frontend/bin/libChesscadeLib.so
$ python pygame_frontend/pygame_frontend.py
```