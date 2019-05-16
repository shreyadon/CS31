// rats.cpp

// Portions you are to complete are marked with a TODO: comment.
// We've provided some incorrect return statements (so indicated) just
// to allow this skeleton program to compile and run, albeit incorrectly.
// The first thing you probably want to do is implement the utterly trivial
// functions (marked TRIVIAL).  Then get Arena::display going.  That gives
// you more flexibility in the order you tackle the rest of the functionality.
// As you finish implementing each TODO: item, remove its TODO: comment.

#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <cstdlib>
#include <cctype>
using namespace std;

///////////////////////////////////////////////////////////////////////////
// Manifest constants
///////////////////////////////////////////////////////////////////////////

const int MAXROWS = 20;            // max number of rows in the arena
const int MAXCOLS = 20;            // max number of columns in the arena
const int MAXRATS = 100;           // max number of rats allowed

const int NORTH = 0;
const int EAST  = 1;
const int SOUTH = 2;
const int WEST  = 3;
const int NUMDIRS = 4;

const int EMPTY      = 0;
const int HAS_POISON = 1;

///////////////////////////////////////////////////////////////////////////
// Type definitions
///////////////////////////////////////////////////////////////////////////

class Arena;  // This is needed to let the compiler know that Arena is a
// type name, since it's mentioned in the Rat declaration.

class Rat
{
public:
    // Constructor
    Rat(Arena* ap, int r, int c);
    
    // Accessors
    int  row() const;
    int  col() const;
    bool isDead() const;
    
    // Mutators
    void move();
    
private:
    Arena* m_arena;
    int    m_row;
    int    m_col;
    
    // Addtional data members for implementing the conditions when a rat is poisoned by a pellet
    bool m_lastmoveidle;
    bool m_onepellet;
    bool m_twopellet;
};

class Player
{
public:
    // Constructor
    Player(Arena* ap, int r, int c);
    
    // Accessors
    int  row() const;
    int  col() const;
    bool isDead() const;
    
    // Mutators
    string dropPoisonPellet();
    string move(int dir);
    void   setDead();
    
private:
    Arena* m_arena;
    int    m_row;
    int    m_col;
    bool   m_dead;
};

class Arena
{
public:
    // Constructor/destructor
    Arena(int nRows, int nCols);
    ~Arena();
    
    // Accessors
    int     rows() const;
    int     cols() const;
    Player* player() const;
    int     ratCount() const;
    int     getCellStatus(int r, int c) const;
    int     numberOfRatsAt(int r, int c) const;
    void    display(string msg) const;
    
    // Mutators
    void setCellStatus(int r, int c, int status);
    bool addRat(int r, int c);
    bool addPlayer(int r, int c);
    void moveRats();
    
private:
    int     m_grid[MAXROWS][MAXCOLS];
    int     m_rows;
    int     m_cols;
    Player* m_player;
    Rat*    m_rats[MAXRATS];
    int     m_nRats;
    int     m_turns;
    
    // Helper functions
    void checkPos(int r, int c) const;
    bool isPosInBounds(int r, int c) const;
};

class Game
{
public:
    // Constructor/destructor
    Game(int rows, int cols, int nRats);
    ~Game();
    
    // Mutators
    void play();
    
private:
    Arena* m_arena;
    
    // Helper functions
    string takePlayerTurn();
};

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function declarations
///////////////////////////////////////////////////////////////////////////

int randInt(int min, int max);
bool decodeDirection(char ch, int& dir);
bool attemptMove(const Arena& a, int dir, int& r, int& c);
bool recommendMove(const Arena& a, int r, int c, int& bestDir);
bool checkSurroundings(const Arena& a, int r, int c);
void clearScreen();

///////////////////////////////////////////////////////////////////////////
//  Rat implementation
///////////////////////////////////////////////////////////////////////////

Rat::Rat(Arena* ap, int r, int c)
{
    if (ap == nullptr)
    {
        cout << "***** A rat must be created in some Arena!" << endl;
        exit(1);
    }
    if (r < 1  ||  r > ap->rows()  ||  c < 1  ||  c > ap->cols())
    {
        cout << "***** Rat created with invalid coordinates (" << r << ","
        << c << ")!" << endl;
        exit(1);
    }
    m_arena = ap;
    m_row = r;
    m_col = c;
    m_lastmoveidle = false;
    m_onepellet = false;
    m_twopellet = false;
}

int Rat::row() const
{
    return m_row;
}

int Rat::col() const
{
    return m_col;
}

bool Rat::isDead() const
{
    if (m_twopellet) // if rat ate two pellets mark as dead.
    {
        return true;
    }
    return false;
}

void Rat::move()
{
    int tempdir;
    if (m_onepellet) // check if rat has already eaten one pellet
    {
        if(m_lastmoveidle) // check if the rat was idle in the last move
        {
            tempdir = randInt(NORTH, WEST); //generate random direction for a value between 0 to 3
            attemptMove(*m_arena, tempdir, m_row, m_col);
            {
                // check for pellet in the new location of the rat
                if (m_arena->getCellStatus(m_row, m_col) == HAS_POISON)
                {
                    m_twopellet = true; // if pellet exists, change the status of the rat.
                    m_arena->setCellStatus(m_row, m_col, EMPTY); // make rat eat the pellet.
                }
                m_lastmoveidle = false; // change the status of the rat so it doesn't move the next turn.
            
            }
            
        }
        else
            m_lastmoveidle = true; // change the status of the rat so it can move the next turn.
    }
    else
    {
        tempdir = randInt(NORTH, WEST); //generate random direction for a value between 0 to 3
        attemptMove(*m_arena, tempdir, m_row, m_col);
        {
            if (m_arena->getCellStatus(m_row, m_col) == HAS_POISON)
            {
                m_onepellet = true; // if pellet exists, change the status of the rat.
                m_lastmoveidle = false; // change the status of the rat it doesn't move the next turn.
                m_arena->setCellStatus(m_row, m_col, EMPTY); // make rat eat the pellet.
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////
//  Player implementation
///////////////////////////////////////////////////////////////////////////

Player::Player(Arena* ap, int r, int c)
{
    if (ap == nullptr)
    {
        cout << "***** The player must be created in some Arena!" << endl;
        exit(1);
    }
    if (r < 1  ||  r > ap->rows()  ||  c < 1  ||  c > ap->cols())
    {
        cout << "**** Player created with invalid coordinates (" << r
        << "," << c << ")!" << endl;
        exit(1);
    }
    m_arena = ap;
    m_row = r;
    m_col = c;
    m_dead = false;
}

int Player::row() const
{
    return m_row;
}

int Player::col() const
{
    return m_col;
}

string Player::dropPoisonPellet()
{
    if (m_arena->getCellStatus(m_row, m_col) == HAS_POISON)
        return "There's already a poison pellet at this spot.";
    m_arena->setCellStatus(m_row, m_col, HAS_POISON);
    return "A poison pellet has been dropped.";
}

string Player::move(int dir)
{
    string temp;
    if (attemptMove(*m_arena, dir, m_row, m_col)) // Check if the new position is in bounds.3
    {
        if (m_arena->numberOfRatsAt(m_row, m_col) > 0) // Check if the new position has any rats.
        {
            
            temp = "Player walked into a rat and died.";
            setDead();
        }
        else
        {
            switch (dir)
            {
                case NORTH:
                    temp = "Player moved north.";
                    break;
                case SOUTH:
                    temp = "Player moved south.";
                    break;
                case EAST:
                    temp = "Player moved east.";
                    break;
                case WEST:
                    temp = "Player moved west.";
                    break;
                default: // should never get to this point
                    exit(1);
            }
        }
    }
    else // If the new position is not in bounds.
    {
        temp = "Player couldn't move; player stands.";
    }

    return temp;
}

bool Player::isDead() const
{
    if (m_dead == true)
        return true;
    return false;
}

void Player::setDead()
{
    m_dead = true;
}

///////////////////////////////////////////////////////////////////////////
//  Arena implementation
///////////////////////////////////////////////////////////////////////////

Arena::Arena(int nRows, int nCols)
{
    if (nRows <= 0  ||  nCols <= 0  ||  nRows > MAXROWS  ||  nCols > MAXCOLS)
    {
        cout << "***** Arena created with invalid size " << nRows << " by "
        << nCols << "!" << endl;
        exit(1);
    }
    m_rows = nRows;
    m_cols = nCols;
    m_player = nullptr;
    m_nRats = 0;
    m_turns = 0;
    for ( int i = 0; i < MAXRATS; i++)
        m_rats[i] = nullptr;
    for (int r = 1; r <= m_rows; r++)
        for (int c = 1; c <= m_cols; c++)
            setCellStatus(r, c, EMPTY);
}

Arena::~Arena()
{
    //cerr << "Entering Arena destructor" << endl;
    delete m_player;
    m_player = nullptr;
    
    for(int i = 0; i < MAXRATS; i++)
    {
        // deleting all remaining dynamically allocated rats
        delete m_rats[i];
        m_rats[i] = nullptr;
        m_nRats--; // decrease the rat count every time a rat is deleted
    }
    //cerr << "Leaving Arena destructor" << endl;
}

int Arena::rows() const
{
    return m_rows;
}

int Arena::cols() const
{
    return m_cols;
}

Player* Arena::player() const
{
    return m_player;
}

int Arena::ratCount() const
{
    return m_nRats;
}

int Arena::getCellStatus(int r, int c) const
{
    checkPos(r, c);
    return m_grid[r-1][c-1];
}

int Arena::numberOfRatsAt(int r, int c) const
{
    int count = 0;
    for(int i = 0; i < MAXRATS; i++)
    {
        if (m_rats[i] != nullptr)
        {
            if(m_rats[i]->row() == r && m_rats[i]->col() == c)
                count++;
        }
    }
    return count;
}

void Arena::display(string msg) const
{
    char displayGrid[MAXROWS][MAXCOLS];
    int r, c;
    
    // Fill displayGrid with dots (empty) and stars (poison pellets)
    for (r = 1; r <= rows(); r++)
        for (c = 1; c <= cols(); c++)
            displayGrid[r-1][c-1] = (getCellStatus(r,c) == EMPTY ? '.' : '*');
    
    
    // Indicate each rat's position
    for (r = 1; r <= rows(); r++)
    {
        for (c = 1; c <= cols(); c++)
        {
            switch(numberOfRatsAt(r, c))
            {
                case 0:
                    displayGrid[r-1][c-1] = (getCellStatus(r,c) == EMPTY ? '.' : '*');
                    break;
                case 1:
                    displayGrid[r-1][c-1] = 'R';
                    break;
                case 2:
                    displayGrid[r-1][c-1] = '2';
                    break;
                case 3:
                    displayGrid[r-1][c-1] = '3';
                    break;
                case 4:
                    displayGrid[r-1][c-1] = '4';
                    break;
                case 5:
                    displayGrid[r-1][c-1] = '5';
                    break;
                case 6:
                    displayGrid[r-1][c-1] = '6';
                    break;
                case 7:
                    displayGrid[r-1][c-1] = '7';
                    break;
                case 8:
                    displayGrid[r-1][c-1] = '8';
                    break;
                default:
                    displayGrid[r-1][c-1] = '9';
                    break;
            }
        }
    }
    
    // Indicate player's position
    if (m_player != nullptr)
        displayGrid[m_player->row()-1][m_player->col()-1] = (m_player->isDead() ? 'X' : '@');
    
    // Draw the grid
    clearScreen();
    for (r = 1; r <= rows(); r++)
    {
        for (c = 1; c <= cols(); c++)
            cout << displayGrid[r-1][c-1];
        cout << endl;
    }
    cout << endl;
    
    // Write message, rat, and player info
    if (msg != "")
        cout << msg << endl;
    cout << "There are " << ratCount() << " rats remaining." << endl;
    if (m_player == nullptr)
        cout << "There is no player!" << endl;
    else if (m_player->isDead())
        cout << "The player is dead." << endl;
    cout << m_turns << " turns have been taken." << endl;
}

void Arena::setCellStatus(int r, int c, int status)
{
    checkPos(r, c);
    m_grid[r-1][c-1] = status;
}

bool Arena::addRat(int r, int c)
{
    if (! isPosInBounds(r, c))
        return false;
    
    // Don't add a rat on a spot with a poison pellet
    if (getCellStatus(r, c) != EMPTY)
        return false;
    
    // Don't add a rat on a spot with a player
    if (m_player != nullptr  &&  m_player->row() == r  &&  m_player->col() == c)
        return false;
    
    // If there are MAXRATS existing rats, return false.
    if (m_nRats >= MAXRATS)
        return false;
    // Otherwise, dynamically allocate a new rat at coordinates (r,c).  Save the
    // pointer to the newly allocated rat and return true.
    m_rats[m_nRats] = new Rat(this, r, c);
    m_nRats++; // Change the total number of rats.
    return true;
}

bool Arena::addPlayer(int r, int c)
{
    if (! isPosInBounds(r, c))
        return false;
    
    // Don't add a player if one already exists
    if (m_player != nullptr)
        return false;
    
    // Don't add a player on a spot with a rat
    if (numberOfRatsAt(r, c) > 0)
        return false;
    
    m_player = new Player(this, r, c);
    return true;
}

void Arena::moveRats()
{
    // Move all rats
    for(int i = 0; i < MAXRATS; i++)
    {
        if (m_rats[i] != nullptr) // check if rat does not exist
        {
            m_rats[i]->move(); // if it exists then move the rat
            // if a rat lands on the player, the player dies.
            if(m_rats[i]->row() == m_player->row() && m_rats[i]->col() == m_player->col())
                m_player->setDead();
            if (m_rats[i]->isDead() == true)
            {
                delete m_rats[i];
                m_rats[i] = nullptr;
                m_nRats--; // Deallocate any dead dynamically allocated rat.
            }
        }
    }
    
    // Another turn has been taken
    m_turns++;
}

bool Arena::isPosInBounds(int r, int c) const
{
    return (r >= 1  &&  r <= m_rows  &&  c >= 1  &&  c <= m_cols);
}

void Arena::checkPos(int r, int c) const
{
    if (r < 1  ||  r > m_rows  ||  c < 1  ||  c > m_cols)
    {
        cout << "***** " << "Invalid arena position (" << r << ","
        << c << ")" << endl;
        exit(1);
    }
}

///////////////////////////////////////////////////////////////////////////
//  Game implementation
///////////////////////////////////////////////////////////////////////////

Game::Game(int rows, int cols, int nRats)
{
    if (nRats < 0)
    {
        cout << "***** Cannot create Game with negative number of rats!" << endl;
        exit(1);
    }
    if (nRats > MAXRATS)
    {
        cout << "***** Trying to create Game with " << nRats
        << " rats; only " << MAXRATS << " are allowed!" << endl;
        exit(1);
    }
    if (rows == 1  &&  cols == 1  &&  nRats > 0)
    {
        cout << "***** Cannot create Game with nowhere to place the rats!" << endl;
        exit(1);
    }
    
    // Create arena
    m_arena = new Arena(rows, cols);
    
    // Add player
    int rPlayer;
    int cPlayer;
    do
    {
        rPlayer = randInt(1, rows);
        cPlayer = randInt(1, cols);
    } while (m_arena->getCellStatus(rPlayer, cPlayer) != EMPTY);
    m_arena->addPlayer(rPlayer, cPlayer);
    
    // Populate with rats
    while (nRats > 0)
    {
        int r = randInt(1, rows);
        int c = randInt(1, cols);
        if (r == rPlayer && c == cPlayer)
            continue;
        m_arena->addRat(r, c); // check for the return value of addRat
        nRats--;
    }
}

Game::~Game()
{
    delete m_arena;
}

string Game::takePlayerTurn()
{
    for (;;)
    {
        cout << "Your move (n/e/s/w/x or nothing): ";
        string playerMove;
        getline(cin, playerMove);
        
        Player* player = m_arena->player();
        int dir;
        
        if (playerMove.size() == 0)
        {
            if (recommendMove(*m_arena, player->row(), player->col(), dir))
            {
                return player->move(dir);}
            else
                return player->dropPoisonPellet();
        }
        else if (playerMove.size() == 1)
        {
            if (tolower(playerMove[0]) == 'x')
                return player->dropPoisonPellet();
            else if (decodeDirection(playerMove[0], dir))
            {
                return player->move(dir);
            }
        }
        cout << "Player move must be nothing, or 1 character n/e/s/w/x." << endl;
    }
}

void Game::play()
{
    m_arena->display("");
    while ( ! m_arena->player()->isDead()  &&  m_arena->ratCount() > 0)
    {
        string msg = takePlayerTurn();
        Player* player = m_arena->player();
        if (player->isDead())
        {
            cout << msg << endl;
            break;
        }
        m_arena->moveRats();
        m_arena->display(msg);
    }
    if (m_arena->player()->isDead())
        cout << "You lose." << endl;
    else
        cout << "You win." << endl;
}

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function implementation
///////////////////////////////////////////////////////////////////////////

// Return a uniformly distributed random int from min to max, inclusive
int randInt(int min, int max)
{
    if (max < min)
        swap(max, min);
    static random_device rd;
    static mt19937 generator(rd());
    uniform_int_distribution<> distro(min, max);
    return distro(generator);
}

bool decodeDirection(char ch, int& dir)
{
    switch (tolower(ch))
    {
        default:  return false;
        case 'n': dir = NORTH; break;
        case 'e': dir = EAST;  break;
        case 's': dir = SOUTH; break;
        case 'w': dir = WEST;  break;
    }
    return true;
}

// Return false without changing anything if moving one step from (r,c)
// in the indicated direction would run off the edge of the arena.
// Otherwise, update r and c to the position resulting from the move and
// return true.
bool attemptMove(const Arena& a, int dir, int& r, int& c)
{
    
    int tempRow = 0, tempCol = 0;
    bool temp = false;
    switch (dir)
    {
        case NORTH: // changing the temp coordinates to North
            tempRow = r - 1;
            tempCol = c;
            break;
        case SOUTH: // changing the temp coordinates to South
            tempRow = r + 1;
            tempCol = c;
            break;
        case EAST: // changing the temp coordinates to East
            tempCol = c + 1;
            tempRow = r;
            break;
        case WEST: // changing the temp coordinates to West
            tempCol = c - 1;
            tempRow = r;
            break;
        default: // should never get to this point
            exit(1);
    }
    
    // checking if the temp coordinates are in bounds
    if (tempRow >= 1  &&  tempRow <= a.rows()  &&  tempCol >= 1  &&  tempCol <= a.cols())
    {
        // if true then will change the coordinates to the temp coordinates and return true
        c = tempCol;
        r = tempRow;
        temp = true;
    }
    
    return temp;
}

// Recommend a move for a player at (r,c):  A false return means the
// recommendation is that the player should drop a poison pellet and not
// move; otherwise, this function sets bestDir to the recommended
// direction to move and returns true.
bool recommendMove(const Arena& a, int r, int c, int& bestDir)
{
    // TODO:
    int tempr = r, tempc = c;
    // if all spots around the player don't have rats and player hasn't already dropped a pellet.
    if (a.numberOfRatsAt(tempr, tempc-1) == 0 && a.numberOfRatsAt(tempr+1, tempc) == 0 && a.numberOfRatsAt(tempr, tempc+1) == 0 && a.numberOfRatsAt(tempr-1, tempc) == 0 && a.getCellStatus(tempr, tempc) == EMPTY)
        return false; // drop a poison pellet
    
    int emptySpots[NUMDIRS + 1] = {0, 0, 0, 0, 0};
    
    // North
    if (attemptMove(a, NORTH, tempr, tempc)) // check if position is in bounds.
    {
        tempr = r; tempc = c;
        if (a.numberOfRatsAt(tempr-1, tempc) == 0) // check if there are no rats north of current pos
        {
            // if the surrounding spots of north are empty increment the number of emptySpots
            if (a.numberOfRatsAt(tempr-1 , tempc) == 0)
                emptySpots[NORTH]++;
            if (a.numberOfRatsAt(tempr, tempc-1) == 0)
                emptySpots[NORTH]++;
            if (a.numberOfRatsAt(tempr, tempc+1) == 0)
                emptySpots[NORTH]++;
        }
    }
    // East
    tempr = r; tempc = c;
    if (attemptMove(a, EAST, tempr, tempc)) // check if position is in bounds.
    {
        tempr = r; tempc = c;
        if (a.numberOfRatsAt(tempr, tempc+1) == 0) // check if there are no rats east of current pos
        {
            // if the surrounding spots of east are empty increment the number of emptySpots
            if (a.numberOfRatsAt(tempr-1 , tempc) == 0)
                emptySpots[EAST]++;
            if (a.numberOfRatsAt(tempr+1, tempc) == 0)
                emptySpots[EAST]++;
            if (a.numberOfRatsAt(tempr, tempc+1) == 0)
                emptySpots[EAST]++;
        }
    }
    // South
    tempr = r; tempc = c;
    if (attemptMove(a, SOUTH, tempr, tempc)) // check if position is in bounds.
    {
        tempr = r; tempc = c;
        if (a.numberOfRatsAt(tempr+1, tempc) == 0) // check if there are no rats south of current pos
        {
            // if the surrounding spots of south are empty increment the number of emptySpots
            if (a.numberOfRatsAt(tempr+1 , tempc) == 0)
                emptySpots[SOUTH]++;
            if (a.numberOfRatsAt(tempr, tempc-1) == 0)
                emptySpots[SOUTH]++;
            if (a.numberOfRatsAt(tempr, tempc+1) == 0)
                emptySpots[SOUTH]++;
        }
    }
    // West
    tempr = r; tempc = c;
    if (attemptMove(a, WEST, tempr, tempc)) // check if there are no rats west of current pos
    {
        tempr = r; tempc = c;
        if (a.numberOfRatsAt(tempr, tempc-1) == 0)
        {
            // if the surrounding spots of west are empty increment the number of emptySpots
            if (a.numberOfRatsAt(tempr-1 , tempc) == 0)
                emptySpots[WEST]++;
            if (a.numberOfRatsAt(tempr+1, tempc) == 0)
                emptySpots[WEST]++;
            if (a.numberOfRatsAt(tempr, tempc-1) == 0)
                emptySpots[WEST]++;
        }
    }
    int maxEmptySpots = emptySpots[NORTH];
    bestDir = NORTH;
    for (int i = NORTH; i < NUMDIRS; i++)
    {
        if (emptySpots[i] > maxEmptySpots)
            bestDir = i;
    }
    
    return true;
}

///////////////////////////////////////////////////////////////////////////
// main()
///////////////////////////////////////////////////////////////////////////

int main()
{
    // Create a game
    // Use this instead to create a mini-game:
    Game g(3, 5, 2);
    
    //Game g(10, 12, 40);
    
    // Play the game
    g.play();
}

///////////////////////////////////////////////////////////////////////////
//  clearScreen implementation
///////////////////////////////////////////////////////////////////////////

// DO NOT MODIFY OR REMOVE ANY CODE BETWEEN HERE AND THE END OF THE FILE!!!
// THE CODE IS SUITABLE FOR VISUAL C++, XCODE, AND g++ UNDER LINUX.

// Note to Xcode users:  clearScreen() will just write a newline instead
// of clearing the window if you launch your program from within Xcode.
// That's acceptable.  (The Xcode output window doesn't have the capability
// of being cleared.)

#ifdef _MSC_VER  //  Microsoft Visual C++

#pragma warning(disable : 4005)
#include <windows.h>

void clearScreen()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    DWORD dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
    COORD upperLeft = { 0, 0 };
    DWORD dwCharsWritten;
    FillConsoleOutputCharacter(hConsole, TCHAR(' '), dwConSize, upperLeft,
                               &dwCharsWritten);
    SetConsoleCursorPosition(hConsole, upperLeft);
}

#else  // not Microsoft Visual C++, so assume UNIX interface

#include <iostream>
#include <cstring>
#include <cstdlib>

void clearScreen()  // will just write a newline in an Xcode output window
{
    static const char* term = getenv("TERM");
    if (term == nullptr  ||  strcmp(term, "dumb") == 0)
        cout << endl;
    else
    {
        static const char* ESC_SEQ = "\x1B[";  // ANSI Terminal esc seq:  ESC [
        cout << ESC_SEQ << "2J" << ESC_SEQ << "H" << flush;
    }
}

#endif
