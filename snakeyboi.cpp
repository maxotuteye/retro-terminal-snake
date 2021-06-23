#include <iostream>
#include <random>
#include <cstdlib>
#include <conio.h>
#include <string>
#include <windows.h>
#include <time.h>
#include <ctype.h>
#include <fstream>

using namespace std;

//  f implies Fruit,    t => Tail,  n => number,    x and y are coords
const short width = 50;
const short height = 20;
short x, y, fx, fy, score, nt, tx[100], ty[100];
short diff = 100;
string names[100];
string scores[100];
enum Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};
Direction dir, ndir;
bool gameover = false;
bool close = false;

void MainMenu()
{
    system("cls");
    cout << "#Welcome to The Pyro's Classic Snake#"
         << "\n\nPress 'S' to Start"
         << "\nPress 'H' for Help/Instructions"
         << "\nPress 'L' to select Difficulty"
         << "\n\nPress 'Q' to Quit" << endl;
}
void Help()
{
    system("cls");
    cout << "\nPress 'W' to go UP."
         << "\nPress 'S' to go DOWN."
         << "\nPress 'A' to turn LEFT."
         << "\nPress 'D' to turn RIGHT."
         << "\nPress 'X' to EXIT during the game"
         << "\n\nYou will DIE if you hit a border."
         << "\nYou will DIE if you eat yourself....lol"
         << "\n\nPress 'M' to return to the Main Menu..";
    if (toupper(_getch()) == 'M')
        MainMenu();
    else
        Help();
}
void Difficulty()
{
    system("cls");
    cout << "PS: This game is pretty easy even when it's hard"
         << "\n1 - Easy"
         << "\n2 - Medium"
         << "\n3 - Hard"
         << "\n\nChoose your difficulty setting:\t";
    string in;
    cin >> in;
    if (in == "1")
        diff = 40;
    else if (in == "2")
        diff = 20;
    else if (in == "3")
        diff = 5;
    MainMenu();
}

void SpawnF(int seed)
{
    srand(seed);
    fx = rand() % width;
    fy = rand() % height;
}

void Setup()
{
    x = width / 2;
    y = height / 2;
    nt = 0;
    SpawnF(time(0));
    score = 0;
    dir = DOWN;
    gameover = false;
}

void DrawGame()
{
    /* draw TopBorder 
        loop through every row
        foreach row:
            draw border,
            check for x-coords of snake and fruit and draw */
    system("cls");
    for (int i = 0; i < width + 2; i++)
        cout << "=";
    cout << endl;
    for (int i = 0; i < height; i++)
    {
        for (int ii = 0; ii < width; ii++)
        {
            if (ii == 0)
                cout << "|";
            if (i == y && ii == x)
                cout << "0";
            else if (i == fy && ii == fx)
                cout << "F";
            else
            {
                bool space = true;
                for (int iii = 0; iii < nt; iii++)
                    if (tx[iii] == ii && ty[iii] == i)
                    {
                        cout << "o";
                        space = false;
                    }

                if (space)
                    cout << " ";
            }
            if (ii == width - 1)
                cout << "|";
        }
        cout << endl;
    }
    for (int i = 0; i < width + 2; i++)
        cout << "=";
    cout << endl;
    cout << "Score: " << score << endl;
    cout << "X: " << x << "\tY: " << y << endl;
}
void CheckInput()
{
    if (_kbhit())
        switch (toupper(_getch()))
        {
        case 'A':
            ndir = LEFT;
            break;
        case 'S':
            ndir = DOWN;
            break;
        case 'D':
            ndir = RIGHT;
            break;
        case 'W':
            ndir = UP;
            break;
        case 'X':
            gameover = true;
            break;
        }
}
void Update()
{
    // update coords for head
    // current x and y of head
    int curX = tx[0];
    int curY = ty[0];

    // to store previous values of tail elements
    int prevX, prevY;
    // set the new head coords
    tx[0] = x;
    ty[0] = y;
    // update tail coords
    for (int i = 1; i < nt; i++)
    {
        /* let prevs hold the current value, then
            assign them the new value (curs), then  
            assign curs the previous values (prevs) */
        /* That is, since every tail element will move
            to the location of the element before it
            store the coords of the element before it as current
            then give it the coords of the element before it
            while storing the current coords as the next new coords*/
        /* Like so:
            previous = current
            current = new coords
            new coords = previous */
        prevX = tx[i];
        prevY = ty[i];
        tx[i] = curX;
        ty[i] = curY;
        curX = prevX;
        curY = prevY;
    }
    // now to update head coords to new Direction
    if (ndir == LEFT && dir == RIGHT)
    {
        ndir = RIGHT;
    }
    else if (ndir == RIGHT && dir == LEFT)
    {
        ndir = LEFT;
    }

    switch (ndir)
    {
    case UP:
        y--;
        break;
    case DOWN:
        y++;
        break;
    case LEFT:
        x--;
        break;
    case RIGHT:
        x++;
        break;
    }

    if (x > width || x < 0 || y > height || y < 0)
    {
        gameover = true;
        MainMenu();
    }

    for (int i = 0; i < nt; i++)
    {
        if (tx[i] == x && ty[i] == y)
            gameover = true;
    }

    if (x == fx && y == fy)
    {
        score += 10;
        nt++;
        SpawnF(fx);
    }
    dir = ndir;
}

void Play()
{
    DrawGame();
    CheckInput();
    Update();
    Sleep(diff);
}

int main()
{
    do
    {
        MainMenu();
        switch (toupper(_getch()))
        {
        case 'S':
            Setup();
            while (!gameover)
                Play();
            break;
        case 'H':
            Help();
            break;
        case 'L':
            Difficulty();
            break;
        case 'Q':
            close = true;
            break;
        }
    } while (!close);

    cout << "\nThanks for playing!" << endl;
    return 0;
}
void SaveScore(string name)
{
    ofstream fil;
    string nam;
    fil.open("scores.dat", ios::out | ios::app);
    cout << "Please enter your name: ";
    cin >> nam;
    fil << nam << "|" << score << endl;
}
void GetScores()
{
    ifstream fil;
    string temp;
    string s;
    fil.open("scores.dat", ios::in);
    while (!fil.eof())
    {
        fil >> temp;
        for (int j = 0; j < temp.length(); j++)
        {
            if (temp[j] == '|')
            {
                s = temp.substr(j + 1, temp.length());
                temp = temp.substr(0, j - 1);
                break;
            }
        }
        for (int i = 0; i < 100; i++)
        {
            if (names[i].empty())
            {
                names[i] = temp;
                scores[i] = s;
            }
        }
    }
}
void DisplayScores()
{
    int arr[100];
    for (int i = 0; i < 100; i++)
    {
        if (stoi(scores[i]) > 0)
            arr[i] = stoi(scores[i]);
    }
    cout << "--HIGHSCORES--"
         << "\nPlayer" << setw(20) << "Score"
         << "\n------" << setw(20) << "-----" << endl;
    for(int i = 0; i < 100; i++)
        if(!names[i].empty()){
            cout << names[i] << setw(20) << scores[i] << endl;
        }
}