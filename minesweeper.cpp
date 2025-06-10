#include <iostream>
#include <ctime>
#include <cstdlib>   // C standard utilities library, for random number generation
#include <fstream>
#include <string>
#include<iomanip>

using namespace std;

const int board_size = 10;

int gameboard[board_size][board_size];  // The board is a 10x10 board
bool revealed[board_size][board_size];


bool gameexists(string& filename)
{
    ifstream infile(filename);

    return infile.good();
}

void savegame(const string& filename) {
    ofstream outfile(filename);
    if (!outfile) {
        cerr << "Error opening file for writing." << endl;
        return;
    }

    for (int i = 0; i < board_size; i++) {
        for (int j = 0; j < board_size; j++) {
            outfile << gameboard[i][j] << " ";
        }
        outfile << endl;
    }

    for (int i = 0; i < board_size; i++) {
        for (int j = 0; j < board_size; j++) {
            outfile << revealed[i][j] << " ";
        }
        outfile << endl;
    }

    outfile.close();
    cout << "Game saved successfully." << endl;
}

void loadgame(const string& filename) {
    ifstream infile(filename);
    if (!infile) {
        cerr << "Error opening file for reading." << endl;
        return;
    }

    for (int i = 0; i < board_size; i++) {
        for (int j = 0; j < board_size; j++) {
            infile >> gameboard[i][j];
        }
    }

    for (int i = 0; i < board_size; i++) {
        for (int j = 0; j < board_size; j++) {
            infile >> revealed[i][j];
        }
    }

    infile.close();
    cout << endl;
    cout << "Game state loaded successfully." << endl;
    cout << endl;
    cout << "--------------------------------------------------------------------------------------------------------------------------" << endl;

}

void boardstartup() {
    for (int i = 0; i < board_size; i++) {
        for (int j = 0; j < board_size; j++) {
            gameboard[i][j] = 0;    // Initialize each coordinate with the value zero
            revealed[i][j] = false; // Initialize all cells as not revealed
        }
    }
}

void placingmines() {
    srand(time(0));
    for (int i = 0; i < 6; i++) {          // Places 6 mines on the board
        int x, y;
        while (true) {          // Find a cell with 0
            x = rand() % board_size;    // Generate random coordinates
            y = rand() % board_size;
            if (gameboard[x][y] != -1) {        // Check if mine is already placed
                break;
            }
        }
        gameboard[x][y] = -1;                   // Place the mine
    }
}

void countmines() {
    for (int i = 0; i < board_size; i++) {
        for (int j = 0; j < board_size; j++) {
            if (gameboard[i][j] != -1) { // Check if mine is not present
                for (int x = i - 1; x <= i + 1; x++) {
                    for (int y = j - 1; y <= j + 1; y++) {
                        if (x >= 0 && x < board_size && y >= 0 && y < board_size && gameboard[x][y] == -1) {
                            gameboard[i][j]++;
                        }
                    }
                }
            }
        }
    }
}

void userinputmanipulation(int x, int y) {
    if (x < 0 || x >= board_size || y < 0 || y >= board_size) { // If the coordinates are out of bounds
        return;
    }
    if (revealed[x][y]) {                      // If the coordinates have already been revealed
        return;
    }
    revealed[x][y] = true;                     // Mark the coordinate as revealed
    if (gameboard[x][y] == -1) {               // Check if mine is present
        cout << setw(70) << "!!! GAME OVER !!!" << endl;
        cout << setw(68) << "A mine was hit" << endl;
        cout << endl;
        return;
    }
    if (gameboard[x][y] == 0) {                // Reveal all surrounding cells with zero
        for (int i = x - 1; i <= x + 1; i++) {
            for (int j = y - 1; j <= y + 1; j++) {
                if (i >= 0 && i < board_size && j >= 0 && j < board_size) {
                    userinputmanipulation(i, j);
                }
            }
        }
    }
}

void displaygameboard() {
    cout << "Current State:" << endl;
    cout << endl;
    cout << "  ";
    for (int i = 0; i < board_size; i++) {
        cout << i << " ";
    }
    cout << endl;
    for (int i = 0; i < board_size; i++) {
        cout << i << " ";
        for (int j = 0; j < board_size; j++) {
            if (revealed[i][j]) {
                if (gameboard[i][j] == -1) {
                    cout << "* ";
                }
                else {
                    cout << gameboard[i][j] << " ";
                }
            }
            else {
                cout << "^ ";
            }
        }
        cout << endl;
    }
}

void displayboardfinal() {
    cout << "Final Board State:" << endl;
    cout << "  ";
    for (int i = 0; i < board_size; i++) {
        cout << i << " ";
    }
    cout << endl;
    for (int i = 0; i < board_size; i++) {
        cout << i << " ";
        for (int j = 0; j < board_size; j++) {
            if (gameboard[i][j] == -1) {
                cout << "* ";
            }
            else if (revealed[i][j]) {
                cout << gameboard[i][j] << " ";
            }
            else {
                cout << "^ ";
            }
        }
        cout << endl;
    }
}

int main() {
        system("color 0E");

        string filename;
        char restart;

        do {
            cout << setw(75) << " WELCOME TO MINESWEEPER GAME" << endl;
            cout << endl;
            cout << "---------------------------------------------------------LOGIN PAGE----------------------------------------------------------";
            cout << endl;
            cout << endl;
            cout << "==> Please enter your username: ";
            cin >> filename;
            cout << endl;
            filename += ".txt";
            if (gameexists(filename)) {
                cout << endl;
                cout << "Game loading......" << endl;
                loadgame(filename);
                displaygameboard();
            }
            else {
                cout << "oops! game not found." << endl << "Loading New game." << endl;
                cout << endl;
                cout << "--------------------------------------------------------------------------------------------------------------------------" << endl;
                cout << endl;
                cout << endl;
                boardstartup();
                placingmines();
                countmines();
                displaygameboard();
            }

            int newgame = 0;
            while (newgame == 0) {
                int x, y;
                cout << endl;
                cout << "Enter the coordinates (x y) or enter (-1, -1) to save game state: "; // Get user input
                cin >> x >> y;
                cout << endl;
                cout << "---------------------------------------------------------------------------------------------------------------------------" << endl;

                if (x == -1 && y == -1) {
                    savegame(filename);
                    continue;
                }
                if (x == -2 && y == -2) {
                    loadgame(filename);
                    displaygameboard();
                    continue;
                }

                userinputmanipulation(x, y);
                if (gameboard[x][y] == -1) { 
                    newgame = 0;
                    break;
                }
                displaygameboard();
                bool winningcondition = true; 
                for (int i = 0; i < board_size; i++) {
                    for (int j = 0; j < board_size; j++) {
                        if (gameboard[i][j] != -1 && !revealed[i][j]) {
                            winningcondition = false;
                            break;
                        }
                    }
                    if (!winningcondition) {
                        break;
                    }
                }
                if (winningcondition) {
                    cout << "--------------------------------------------------------------------------------------------------------------------------" << endl;
                    cout << endl;
                    cout << setw(75) << "!! YOU WON :)  !!" << endl;
                    newgame = 1;
                    break;
                }
            }
            displayboardfinal();

            cout << "Press Y to start another game/ Press N to exit: ";
            cin >> restart;
        } while (restart == 'y' || restart == 'Y');

        return 0;
    } 

