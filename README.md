# The Last Train
CS-1002: Programming Fundamentals (AI-C), Semester Project
Mohammad Waqas (I26-0143), Muhammad Omer Zaheer (I26-0155)
Release tag: v1.0.0155

## Folder Structure
| Folder      | Contents                                                  |
|-------------|-----------------------------------------------------------|
| `include/`  | Header files                                              |
| `src/core/` | Source files, including `main_core.cpp`                   |
| `data/`     | `building.txt`, `initial-state.txt`, test input files     |
| `out/`      | `savegame.txt`, `game_summary.txt` (sample run committed) |
| `docs/`     | `Project_Diagram.jpg`, `Flowchart.png`, `ConsoleUI.png`   |

## Build
```
g++ -std=c++17 -Wall -Wextra -Iinclude src/core/*.cpp -o last_train
```

## Run
Run from the project root so the relative paths `data/` and `out/` resolve.
```
./last_train          (Git Bash / Linux)
.\last_train.exe      (Windows PowerShell)
```
Choose `N` for a new game, `R` to resume from `out/savegame.txt`, or `Q` to quit.

## In-game Commands
| Key | Action                             |
|-----|------------------------------------|
| M   | Move (enter floor and room)        |
| I   | Inspect the room                   |
| C   | Collect an item (enter its symbol) |
| U   | Use an item                        |
| X   | Enter the cabinet code             |
| S   | Save the game                      |
| H   | Help                               |
| Q   | Quit                               |

## Verifying Deterministic Output
1. Build the program (see above).
2. Run it with the scripted input file, which plays through to a win or loss:
   - Git Bash / Linux: `./last_train < data/test_win.txt`
   - PowerShell: `Get-Content data\test_win.txt | .\last_train.exe`
3. Copy `out/game_summary.txt` to `run1.txt`.
4. Repeat step 2 and copy the new `out/game_summary.txt` to `run2.txt`.
5. Compare the two files:
   - Git Bash / Linux: `diff run1.txt run2.txt`
   - PowerShell: `fc run1.txt run2.txt`

No differences means identical inputs produce identical resources, score and objectives.