// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own.
// TODO: remove this comment header!

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "grid.h"
#include "gwindow.h"
#include "simpio.h"
#include "lifegui.h"
using namespace std;



Grid<int> InitializeStartingColony() {
    // Extract row and column from input file
    ifstream startingColony;
    string fileName = promptUserForFile(startingColony, "Grid input file name? ", "Try again ");
    string str1;
    getline(startingColony, str1);
    while(str1.at(0) == '#') {
        getline(startingColony, str1);
    }

    int rowSize = stringToInteger(str1);
    getline(startingColony, str1);
    int columnSize = stringToInteger(str1);

    // Extract grid pattern into fileGrid
    Grid<int> fileGrid(rowSize,columnSize);
    for (int i = 0; i < rowSize; i ++) {
            getline(startingColony, str1);
            for (int j = 0; j < columnSize; j ++) {
                if (str1.at(j) == '-') {
                    fileGrid.set(i,j,0);
                } else {
                    fileGrid.set(i, j, 1);
                }
            }
        }
    startingColony.close();

    cout << fileGrid.toString2D() << endl;

    return fileGrid;
}


int CountNeighbors(int row, int col, Grid<int> mainGrid) {

    int neighbors = 0;

    if (mainGrid.get(row, col) != 0) {
        neighbors --;
    }

    for (int i = row - 1; i <= row + 1; i++) {
        for (int j = col - 1; j <= col + 1; j++) {
            if (i >= 0 && i < mainGrid.numRows() && j >= 0 && j < mainGrid.numCols()) {
                if (mainGrid.get(i,j) != 0) {
                    neighbors ++;
                }
            }
        }
    }

    return neighbors;
}


void PopulateNextGeneration(Grid<int> mainGrid, Grid<int> &newGrid) {
    int numNeighbors = 0;
    int mainGridValue = 0;
    Grid<int> testGrid(mainGrid.numRows(), mainGrid.numCols());
    for (int i = 0; i < mainGrid.numRows(); i++) {
        for (int j = 0; j < mainGrid.numCols(); j++) {
            numNeighbors = CountNeighbors(i,j,mainGrid);
            mainGridValue = mainGrid.get(i,j);
            testGrid.set(i,j,numNeighbors);

            switch (numNeighbors) {
            case (2):
                newGrid.set(i,j, mainGridValue);
                break;
            case (3):
                if (mainGridValue == 0) {
                    newGrid.set(i,j, mainGridValue + 1);
                    break;
                } else {
                    newGrid.set(i,j, mainGridValue);
                    break;
                }
            default:
                newGrid.set(i,j, 0);
                break;
            }

        }
    }
    //cout << testGrid.toString2D() << endl;

}


void GenerateSimulation(Grid<int> mainGrid, Grid<int> newGrid) {

    while (true) {
        cout << "a)nimate, t)ick, q)uit? ";
        string response = getLine();

        if (response == "a" || response == "A") {
            int numFrames = 0;

            while (true) {
                cout << "How many frames? ";
                cin >> numFrames;
                if (!cin.fail()) {
                    break;
                }
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Illegal integer format. Try again." << endl;
            }
//            do {
//                cout << "How many frames? ";
//                if (!cin) {
//                    cout << "Illegal integer format. Try again." << endl;
//                    cin.clear();
//                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
//                }
//            } while (!(cin >> numFrames));

            int count = 0;

            while (count < numFrames) {
                clearConsole();

                PopulateNextGeneration(mainGrid, newGrid);
                cout << newGrid.toString2D() << endl;
                mainGrid = newGrid;

                pause(500);
                count ++;
            }
//                cout << "How many frames? ";
//                string inputLine;
//                getLine(cin, inputLine);
//                stringstream lineStream(inputLine);
//                if (lineStream >> numFrames) {
//                    //numFrames = stringToInteger(lineStream);
//                    cout << "Yay " << lineStream;
//                    break;
//                }
//                cout << "Illegal integer format. Try again." << endl;

            //}
//            if (stringToInteger(numberFrames) >= 0) {
//                cout << "Yay " << stringToInteger(numberFrames);
//                break;
//            } else {
//                cout << "Nay";
//                break;
//            }

        } else if (response == "t" || response == "T") {
            PopulateNextGeneration(mainGrid, newGrid);
            cout << newGrid.toString2D() << endl;
            mainGrid = newGrid;
        } else if (response == "q" || response == "Q") {
            cout << "Have a nice Life!";
            break;
        } else {
            cout << "Whoops!" << endl;
        }

    }

}


void Welcome() {
    cout << "Welcome to the CS 106B Game of Life, " << endl;
    cout << "a simulation of the lifecycle of a bacteria colony. " << endl;
    cout << "Cells (X) live and die by the following rules: " << endl;
    cout << "- A cell with 1 or fewer neighbors dies. " << endl;
    cout << "- Locations with 2 neighbors remain stable. " << endl;
    cout << "- Location with 3 neighbors will create life. " << endl;
    cout << "- A cell with 4 or more neighbors dies. " << endl;
    cout << endl;

}


int main() {
    // TODO: Finish the program!

    Welcome();

    Grid<int> mainGrid(1, 1);
    mainGrid = InitializeStartingColony();
    Grid<int> newGrid(mainGrid.numRows(), mainGrid.numCols());

    GenerateSimulation(mainGrid, newGrid);

    return 0;
}
