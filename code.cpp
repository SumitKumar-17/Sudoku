#include <bits/stdc++.h>
using namespace std;

class SudokuSolver
{
private:
    vector<vector<int>> grid;
    vector<vector<int>> originalGrid;
    int steps;
    bool visualMode;
    int delay;

public:
    SudokuSolver() : grid(9, vector<int>(9, 0)),
                     originalGrid(9, vector<int>(9, 0)),
                     steps(0), visualMode(true), delay(100) {}

    void clearScreen()
    {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }

    void displayGrid(int highlightRow = -1, int highlightCol = -1, int value = 0)
    {
        clearScreen();

        cout << "\n";
        cout << "╔═══════════════════════╗\n";
        cout << "║     SUDOKU SOLVER     ║\n";
        cout << "║     Step: " << setw(4) << steps << "        ║\n";
        cout << "╠═══════════════════════╣\n";

        for (int i = 0; i < 9; i++)
        {
            if (i % 3 == 0 && i != 0)
            {
                cout << "║───────┼───────┼───────║\n";
            }

            cout << "║ ";
            for (int j = 0; j < 9; j++)
            {
                if (j % 3 == 0 && j != 0)
                {
                    cout << "│ ";
                }

                if (i == highlightRow && j == highlightCol)
                {
                    cout << "\033[32m";
                    if (value > 0)
                    {
                        cout << value;
                    }
                    else
                    {
                        cout << (grid[i][j] == 0 ? "." : to_string(grid[i][j]));
                    }
                    cout << "\033[0m ";
                }
                else if (originalGrid[i][j] != 0)
                {
                    cout << "\033[34m" << grid[i][j] << "\033[0m ";
                }
                else if (grid[i][j] != 0)
                {
                    cout << "\033[31m" << grid[i][j] << "\033[0m ";
                }
                else
                {
                    cout << ". ";
                }
            }
            cout << "║\n";
        }

        cout << "╚═══════════════════════╝\n";

        if (highlightRow != -1 && highlightCol != -1)
        {
            cout << "Trying: [" << highlightRow + 1 << "," << highlightCol + 1 << "] = " << value << "\n";
        }

        cout << "\nLegend: \033[34mOriginal\033[0m | \033[31mSolved\033[0m | \033[32mCurrent\033[0m\n";

        if (visualMode)
        {
            this_thread::sleep_for(chrono::milliseconds(delay));
        }
    }

    bool isValid(int row, int col, int num)
    {
        // Check row
        for (int j = 0; j < 9; j++)
        {
            if (grid[row][j] == num)
                return false;
        }

        // Check column
        for (int i = 0; i < 9; i++)
        {
            if (grid[i][col] == num)
                return false;
        }

        // Check 3x3 box
        int boxRow = (row / 3) * 3;
        int boxCol = (col / 3) * 3;
        for (int i = boxRow; i < boxRow + 3; i++)
        {
            for (int j = boxCol; j < boxCol + 3; j++)
            {
                if (grid[i][j] == num)
                    return false;
            }
        }

        return true;
    }

    bool solve()
    {
        steps++;

        for (int row = 0; row < 9; row++)
        {
            for (int col = 0; col < 9; col++)
            {
                if (grid[row][col] == 0)
                {
                    for (int num = 1; num <= 9; num++)
                    {
                        if (visualMode)
                        {
                            displayGrid(row, col, num);
                        }

                        if (isValid(row, col, num))
                        {
                            grid[row][col] = num;

                            if (visualMode)
                            {
                                displayGrid(row, col);
                            }

                            if (solve())
                            {
                                return true;
                            }

                            // Backtrack
                            grid[row][col] = 0;
                            if (visualMode)
                            {
                                displayGrid(row, col, 0);
                            }
                        }
                    }
                    return false;
                }
            }
        }
        return true;
    }

    void loadPuzzle(const vector<vector<int>> &puzzle)
    {
        grid = puzzle;
        originalGrid = puzzle;
        steps = 0;
    }

    void generateRandomPuzzle()
    {
        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                grid[i][j] = 0;
            }
        }

        fillDiagonal();

        bool oldVisualMode = visualMode;
        visualMode = false;
        solve();
        visualMode = oldVisualMode;

        removeCells(40);

        originalGrid = grid;
        steps = 0;
    }

    void fillDiagonal()
    {
        for (int i = 0; i < 9; i += 3)
        {
            fillBox(i, i);
        }
    }

    void fillBox(int row, int col)
    {
        vector<int> nums = {1, 2, 3, 4, 5, 6, 7, 8, 9};
        random_device rd;
        mt19937 g(rd());
        shuffle(nums.begin(), nums.end(), g);

        int idx = 0;
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                grid[row + i][col + j] = nums[idx++];
            }
        }
    }

    void removeCells(int count)
    {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(0, 8);

        while (count > 0)
        {
            int row = dis(gen);
            int col = dis(gen);

            if (grid[row][col] != 0)
            {
                grid[row][col] = 0;
                count--;
            }
        }
    }

    void setVisualMode(bool mode)
    {
        visualMode = mode;
    }

    void setDelay(int ms)
    {
        delay = ms;
    }

    bool isSolved()
    {
        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                if (grid[i][j] == 0)
                    return false;
            }
        }
        return true;
    }

    void showStats()
    {
        cout << "\n";
        cout << "╔═══════════════════════╗\n";
        cout << "║   SOLVING COMPLETE!   ║\n";
        cout << "║                       ║\n";
        cout << "║  Total Steps: " << setw(8) << steps << " ║\n";
        cout << "║                       ║\n";
        cout << "╚═══════════════════════╝\n";
    }
};

class SudokuGame
{
private:
    SudokuSolver solver;

    // Predefined puzzles with different difficulty levels
    vector<vector<vector<int>>> puzzles = {
        // Easy puzzle
        {{5, 3, 0, 0, 7, 0, 0, 0, 0},
         {6, 0, 0, 1, 9, 5, 0, 0, 0},
         {0, 9, 8, 0, 0, 0, 0, 6, 0},
         {8, 0, 0, 0, 6, 0, 0, 0, 3},
         {4, 0, 0, 8, 0, 3, 0, 0, 1},
         {7, 0, 0, 0, 2, 0, 0, 0, 6},
         {0, 6, 0, 0, 0, 0, 2, 8, 0},
         {0, 0, 0, 4, 1, 9, 0, 0, 5},
         {0, 0, 0, 0, 8, 0, 0, 7, 9}},

        // Medium puzzle
        {{0, 0, 0, 6, 0, 0, 4, 0, 0},
         {7, 0, 0, 0, 0, 3, 6, 0, 0},
         {0, 0, 0, 0, 9, 1, 0, 8, 0},
         {0, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 5, 0, 1, 8, 0, 0, 0, 3},
         {0, 0, 0, 3, 0, 6, 0, 4, 5},
         {0, 4, 0, 2, 0, 0, 0, 6, 0},
         {9, 0, 3, 0, 0, 0, 0, 0, 0},
         {0, 2, 0, 0, 0, 0, 1, 0, 0}},

        // Hard puzzle
        {{0, 0, 0, 0, 0, 0, 0, 1, 0},
         {4, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 0, 6, 0, 2},
         {0, 0, 0, 0, 3, 0, 0, 7, 0},
         {5, 0, 0, 0, 0, 0, 0, 0, 0},
         {0, 0, 0, 0, 0, 8, 0, 0, 0},
         {0, 6, 0, 1, 0, 0, 0, 0, 0},
         {0, 0, 2, 0, 0, 0, 0, 0, 3},
         {0, 0, 0, 0, 0, 0, 0, 0, 0}}};

public:
    void showMenu()
    {
        cout << "\n╔═══════════════════════════════════╗\n";
        cout << "║          SUDOKU SOLVER            ║\n";
        cout << "║        WITH SIMULATION            ║\n";
        cout << "╠═══════════════════════════════════╣\n";
        cout << "║  1. Solve Easy Puzzle             ║\n";
        cout << "║  2. Solve Medium Puzzle           ║\n";
        cout << "║  3. Solve Hard Puzzle             ║\n";
        cout << "║  4. Generate Random Puzzle        ║\n";
        cout << "║  5. Settings                      ║\n";
        cout << "║  6. Exit                          ║\n";
        cout << "╚═══════════════════════════════════╝\n";
        cout << "Enter your choice: ";
    }

    void showSettings()
    {
        int choice;
        cout << "\n╔═══════════════════════════════════╗\n";
        cout << "║            SETTINGS               ║\n";
        cout << "╠═══════════════════════════════════╣\n";
        cout << "║  1. Toggle Visual Mode            ║\n";
        cout << "║  2. Set Animation Speed           ║\n";
        cout << "║  3. Back to Main Menu             ║\n";
        cout << "╚═══════════════════════════════════╝\n";
        cout << "Enter your choice: ";

        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            static bool visualMode = true;
            visualMode = !visualMode;
            solver.setVisualMode(visualMode);
            cout << "Visual mode " << (visualMode ? "enabled" : "disabled") << "!\n";
            cout << "Press Enter to continue...";
            cin.ignore();
            cin.get();
            break;
        }
        case 2:
        {
            cout << "Enter delay in milliseconds (1-2000): ";
            int delay;
            cin >> delay;
            delay = max(1, min(2000, delay));
            solver.setDelay(delay);
            cout << "Animation speed set to " << delay << "ms!\n";
            cout << "Press Enter to continue...";
            cin.ignore();
            cin.get();
            break;
        }
        }
    }

    void solvePuzzle(int puzzleIndex)
    {
        if (puzzleIndex < 0 || puzzleIndex >= puzzles.size())
        {
            cout << "Invalid puzzle index!\n";
            return;
        }

        solver.loadPuzzle(puzzles[puzzleIndex]);

        cout << "\nStarting puzzle:\n";
        solver.displayGrid();

        cout << "Press Enter to start solving...";
        cin.ignore();
        cin.get();

        auto start = chrono::high_resolution_clock::now();

        if (solver.solve())
        {
            auto end = chrono::high_resolution_clock::now();
            auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);

            solver.displayGrid();
            solver.showStats();
            cout << "Time taken: " << duration.count() << " ms\n";
        }
        else
        {
            cout << "No solution exists for this puzzle!\n";
        }

        cout << "Press Enter to continue...";
        cin.get();
    }

    void run()
    {
        int choice;

        while (true)
        {
            solver.clearScreen();
            showMenu();
            cin >> choice;

            switch (choice)
            {
            case 1:
                solvePuzzle(0); // Easy
                break;
            case 2:
                solvePuzzle(1); // Medium
                break;
            case 3:
                solvePuzzle(2); // Hard
                break;
            case 4:
            {
                cout << "\nGenerating random puzzle...\n";
                solver.generateRandomPuzzle();

                cout << "Generated puzzle:\n";
                solver.displayGrid();

                cout << "Press Enter to start solving...";
                cin.ignore();
                cin.get();

                auto start = chrono::high_resolution_clock::now();

                if (solver.solve())
                {
                    auto end = chrono::high_resolution_clock::now();
                    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);

                    solver.displayGrid();
                    solver.showStats();
                    cout << "Time taken: " << duration.count() << " ms\n";
                }
                else
                {
                    cout << "No solution exists for this puzzle!\n";
                }

                cout << "Press Enter to continue...";
                cin.get();
                break;
            }
            case 5:
                showSettings();
                break;
            case 6:
                cout << "\nThank you for using Sudoku Solver!\n";
                return;
            default:
                cout << "Invalid choice! Press Enter to continue...";
                cin.ignore();
                cin.get();
            }
        }
    }
};

int main()
{
    SudokuGame game;
    game.run();
    return 0;
}
