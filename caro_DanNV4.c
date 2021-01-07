#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>
#include <ctype.h>
#define SIZE 10 // size of the board game
#define LEFT 1
#define TOP 1
#define START_GAME 1
#define BOT 2
#define REPLAY 3
#define PLAYER_INFOR 4
#define GUID 5
#define EXIT 6
#define CONTINUE 0
#define WIN1 -1 // player 1 win
#define WIN2 1  // player 2 win
#define DRAW 2  // two player draw
#define PLAYER1 -1
#define PLAYER2 1
#define NUMBER_WIN 4 //the number of pieces that are aligned or diagonal to win
#define HUMAN '1'
#define BOT_MAN '2'
typedef struct bd
{
    int left;
    int top;
    char point[SIZE][SIZE];
} Board;
typedef struct p
{
    int _x;    //
    int _y;    //
    int check; // result of cell
} Point;
typedef struct g
{
    int CountWin1;
    int CountWin2;
    int CountStep1;
    int CountStep2;
    int turn;
    int result;
} Game;
/*FUNCTIONS FOR WORK WITH CONSOLE WINDOW */
void txtColor(int x)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), x);
}
void SetColor(int backgound_color, int text_color)
{
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

    int color_code = backgound_color * 16 + text_color;
    SetConsoleTextAttribute(hStdout, color_code);
}
void GoTo(SHORT posX, SHORT posY)
{
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD Position;
    Position.X = posX;
    Position.Y = posY;

    SetConsoleCursorPosition(hStdout, Position);
}
void SetWindowSize(SHORT width, SHORT height)
{
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

    SMALL_RECT WindowSize;
    WindowSize.Top = 0;
    WindowSize.Left = 0;
    WindowSize.Right = width;
    WindowSize.Bottom = height;

    SetConsoleWindowInfo(hStdout, 1, &WindowSize);
}
void SetScreenBufferSize(SHORT width, SHORT height)
{
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

    COORD NewSize;
    NewSize.X = width;
    NewSize.Y = height;

    SetConsoleScreenBufferSize(hStdout, NewSize);
}
int menu_main()
{
    /*SetConsoleOutputCP(65001);// set vietnamese*/
    system("cls");
    printf("*--------MAIN MENU-------*\n");
    printf("%19s", "Press your choice:\n");
    printf("1.Play with Other Player\n");
    printf("2.Play with BOT\n");
    printf("3.Replay\n");
    printf("4.Player's Information\n");
    printf("5.Guild\n");
    printf("6.Exit\n");
    GoTo(20, 1); // go to line 1-20.
    int choice;
    fflush(stdin);
    scanf("%d", &choice);
    GoTo(0, 8);
    printf("Choice: %d\n", choice);
    return choice;
}
int menu_bot()
{
    system("cls");
    printf("*-----Play with BOT-----*\n");
    printf("Press number to choice: \n");
    printf("1.Easy Mode\n");
    printf("2.Normal Mode\n");
    printf("3.Hard Mode\n");
    printf("4.Back to MAIN MENU");
    GoTo(0, 25);
    int choice_bot = 0;
    scanf("%d", &choice_bot);
    return choice_bot;
}
void the_board(Game *_g)
{
    system("cls");
    // draws the board
    register int i = 0, j = 0;
    txtColor(14);
    for (i = 0; i <= SIZE; i++)
    {
        for (j = 0; j <= SIZE; j++)
        {
            GoTo(4 * i + LEFT, 2 * j + TOP);
            txtColor(10);
            printf("."); // printf '*'
        }
    }
    for (i = 0; i < SIZE; i++)
    {
        GoTo(4 * i + LEFT + 2, 0);
        printf("%d", i);
        GoTo(0, 2 * i + TOP + 1);
        printf("%d", i);
    }
    printf("\n\n");
    txtColor(14);
    GoTo(SIZE * 7 - 16, TOP + 1);
    txtColor(15);
    printf("PLAYER 1 VS PLAYER 2");
    GoTo(SIZE * 5, TOP + 5);
    printf("Step:");
    GoTo(SIZE * 6, TOP + 5);
    printf("%d", _g->CountStep1);
    GoTo(SIZE * 7, TOP + 5);
    printf("%d", _g->CountStep2);
    GoTo(SIZE * 5, TOP + 12);
    txtColor(15);
    printf("Turn");
    txtColor(11);
    GoTo(SIZE * 6 + 2, TOP + 10);
    printf("%c%c%s%c%c", (char)(219), (char)(219), "      ", (char)(219), (char)(219));
    GoTo(SIZE * 6 + 2, TOP + 11);
    printf("%s%c%c%s%c%c%s", "  ", (char)(219), (char)(219), "  ", (char)(219), (char)(219), "  ");
    GoTo(SIZE * 6 + 2, TOP + 12);
    printf("%s%c%c%s", "    ", (char)(219), (char)(219), "    ");
    GoTo(SIZE * 6 + 2, TOP + 13);
    printf("%s%c%c%s%c%c%s", "  ", (char)(219), (char)(219), "  ", (char)(219), (char)(219), "  ");
    GoTo(SIZE * 6 + 2, TOP + 14);
    printf("%c%c%s%c%c", (char)(219), (char)(219), "      ", (char)(219), (char)(219));
}
void reset_data_game(Game *_g)
{

    _g->CountStep1 = 0;
    _g->CountStep2 = 0;
    _g->CountWin1 = 0;
    _g->CountWin2 = 0;
    _g->turn = PLAYER1;
    _g->result = CONTINUE;
}
void reset_kq(Point **kq)
{
    register int i = 0;
    register int j = 0;
    for (i = 0; i < SIZE; i++)
    {
        for (j = 0; j < SIZE; j++)
        {
            kq[i][j]._x = 4 * i + LEFT + 2; //coincides with the horizontal
            kq[i][j]._y = 2 * j + TOP + 1;  //coincides with the pitch of the board
            kq[i][j].check = 0;             // save the results of the cell
        }
    }
}
void f_data(Point **kq, Game *_g)
{

    int ikq = 0;
    for (ikq = 0; ikq < SIZE; ikq++)
    {
        free(kq[ikq]); //Free up memory by row
    }

    free(kq); // Free up memory kq pointer.
    free(_g); // Free up memory of _g
}
char getKey()
{
    char _command;
    if (stdin != NULL)
        fflush(stdin);
    _command = toupper(_getch());
    return _command;
}
void exit_game(Point **kq, int *loop)
{
    GoTo(2 * LEFT, 2 * SIZE + 2 * TOP);
    printf("EXIT GAME.\n");
    *loop = 0;
}
void moveLeft(Board *_b, int _x0)
{
    if (_b->left > _x0)
    {
        _b->left -= 4;
        GoTo(_b->left, _b->top);
    }
}
void moveUp(Board *_b, int _y0)
{
    if (_b->top > _y0)
    {
        _b->top -= 2;
        GoTo(_b->left, _b->top);
    }
}
void moveDown(Board *_b, int _yMax)
{
    if (_b->top < _yMax)
    {
        _b->top += 2;
        GoTo(_b->left, _b->top);
    }
}
void moveRight(Board *_b, int _xMax)
{
    if (_b->left < _xMax)
    {
        _b->left += 4;
        GoTo(_b->left, _b->top);
    }
}
int check_board(Board *_b, Point **kq, Game *_g)
{
    int i, j;
    for (i = 0; i < SIZE; i++)
    {
        for (j = 0; j < SIZE; j++)
        {
            if (kq[i][j]._x == _b->left && kq[i][j]._y == _b->top && kq[i][j].check == 0)
            {
                if (_g->turn == PLAYER1)
                {
                    kq[i][j].check = PLAYER1; // PLAYER1 = -1
                }
                else
                {
                    kq[i][j].check = PLAYER2; // PLAYER2 = 1
                }
                return kq[i][j].check;
            }
        }
    }
    return 0;
}
bool processCheckBoard(Board *_b, Point **kq, Game *_g)
{
    switch (check_board((Board *)_b, (Point **)kq, (Game *)_g))
    {
    case PLAYER1:
        txtColor(11);
        printf("X");
        _g->CountStep1++;
        GoTo(SIZE * 6, TOP + 5);
        txtColor(11);
        printf("%d", _g->CountStep1);

        //print out the second player's turn.
        txtColor(12);
        GoTo(SIZE * 6 + 2, TOP + 10);
        printf("%s%c%c%c%c%c%c%c%c%s", " ", (char)(219), (char)(219), (char)(219), (char)(219), (char)(219), (char)(219), (char)(219), (char)(219), " ");
        GoTo(SIZE * 6 + 2, TOP + 11);
        printf("%c%c%s%c%c", (char)(219), (char)(219), "      ", (char)(219), (char)(219));
        GoTo(SIZE * 6 + 2, TOP + 12);
        printf("%c%c%s%c%c", (char)(219), (char)(219), "      ", (char)(219), (char)(219));
        GoTo(SIZE * 6 + 2, TOP + 13);
        printf("%c%c%s%c%c", (char)(219), (char)(219), "      ", (char)(219), (char)(219));
        GoTo(SIZE * 6 + 2, TOP + 14);
        printf("%s%c%c%c%c%c%c%c%c%s", " ", (char)(219), (char)(219), (char)(219), (char)(219), (char)(219), (char)(219), (char)(219), (char)(219), " ");
        break;
    case PLAYER2:
        txtColor(12);
        printf("O");
        _g->CountStep2++;
        GoTo(SIZE * 7, TOP + 5);
        txtColor(12);
        printf("%d", _g->CountStep2);
        //_g->turn = PLAYER1; //
        //print the first player's turn
        GoTo(SIZE * 6 + 2, TOP + 10);
        printf("%c%c%s%c%c", (char)(219), (char)(219), "      ", (char)(219), (char)(219));
        GoTo(SIZE * 6 + 2, TOP + 11);
        printf("%s%c%c%s%c%c%s", "  ", (char)(219), (char)(219), "  ", (char)(219), (char)(219), "  ");
        GoTo(SIZE * 6 + 2, TOP + 12);
        printf("%s%c%c%s", "    ", (char)(219), (char)(219), "    ");
        GoTo(SIZE * 6 + 2, TOP + 13);
        printf("%s%c%c%s%c%c%s", "  ", (char)(219), (char)(219), "  ", (char)(219), (char)(219), "  ");
        GoTo(SIZE * 6 + 2, TOP + 14);
        printf("%c%c%s%c%c", (char)(219), (char)(219), "      ", (char)(219), (char)(219));
        break;
    case 0:
        return false; // enter a cell that already has data
    }
    return true; // the cell is vacant
}
int duyetNgang(Point **kq, int x0, int y0) // browse horizontally
{
    // browse from x0 to the left
    int i;
    int count = 0;
    for (i = x0; i >= 0; i--)
    {
        if (kq[i][y0].check == kq[x0][y0].check)
        {
            count++;
        }
        else
        {
            break; //  If the cell is different from the data in question, then quit the loop
        }
    }
    // browse from x0 + 1 to the right
    for (i = x0 + 1; i < SIZE; i++)
    {
        if (kq[i][y0].check == kq[x0][y0].check)
        {
            count++;
        }
        else
        {
            break; // If the cell is different from the data in question, then quit the loop
        }
    }
    if (count >= NUMBER_WIN)
    {
        return kq[x0][y0].check;
    }
    return 0;
}

int duyetDoc(Point **kq, int x0, int y0)
{
    // browse from y0 to up
    int i;
    int count = 0;
    for (i = y0; i >= 0; i--) // stop when y = 0
    {
        if (kq[x0][i].check == kq[x0][y0].check)
        {
            count++;
        }
        else
        {
            break;
        }
    }
    // browse from y0 + 1 to yMax = SIZE - 1
    for (i = y0 + 1; i < SIZE; i++)
    {
        if (kq[x0][i].check == kq[x0][y0].check)
        {
            count++;
        }
        else
        {
            break;
        }
    }
    if (count >= NUMBER_WIN)
    {
        return kq[x0][y0].check;
    }
    return 0;
}
int duyetCheoPhai(Point **kq, int x0, int y0) // Browse diagonally from right to the left
{
    int i;
    int count = 0;
    for (i = 0; (y0 - i) >= 0 && (x0 + i) < SIZE; i++)
    {
        if (kq[x0 + i][y0 - i].check == kq[x0][y0].check)
        {
            count++;
        }
        else
        {
            break; // If the cell has a different value than the current one, exit the loop
        }
    }
    for (i = 1; (y0 + i) < SIZE && (x0 - i) >= 0; i++)
    {
        if (kq[x0 - i][y0 + i].check == kq[x0][y0].check)
        {
            count++;
        }
        else
        {
            break;
        }
    }
    if (count >= NUMBER_WIN)
    {
        return kq[x0][y0].check;
    }
    return 0;
}
int duyetCheoTrai(Point **kq, int x0, int y0) // Browse diagonally from left to right
{
    // browse from x0
    int i;
    int count = 0;
    for (i = 0; ((x0 - i) >= 0) && ((y0 - i) >= 0); i++)
    {
        if (kq[x0 - i][y0 - i].check == kq[x0][y0].check)
            count++;
        else
        {
            break;
        }
    }
    for (i = 1; ((x0 + i) < SIZE) && ((y0 + i) < SIZE); i++)
    {
        if (kq[x0 + i][y0 + i].check == kq[x0][y0].check)
            count++;
        else
        {
            break;
        }
    }
    if (count >= NUMBER_WIN)
    {
        return kq[x0][y0].check;
    }
    return 0;
}
// check win-lose game
int process_finish(Board *_b, Point **kq, Game *_g)
{
    int result = 0;
    int i, j;
    int x0, y0; // the index of the results memory cell.
    // find current index
    for (i = 0; i < SIZE; i++)
    {
        for (j = 0; j < SIZE; j++)
        {
            if (kq[i][j]._x == _b->left && kq[i][j]._y == _b->top)
            {
                x0 = i;
                y0 = j;
                break; // exits the loop immediately once the result is found.
            }
        }
    }
    result = duyetNgang((Point **)kq, x0, y0);
    if (result != 0)
        return result;

    result = duyetDoc((Point **)kq, x0, y0);
    if (result != 0)
        return result;

    result = duyetCheoPhai((Point **)kq, x0, y0);
    if (result != 0)
        return result;

    result = duyetCheoTrai((Point **)kq, x0, y0);
    if (result != 0)
        return result;
    if ((_g->CountStep1 + _g->CountStep2) == (SIZE * SIZE))
        return DRAW;
    return CONTINUE;
}
int random(int minN, int maxN){
    return minN + rand() % (maxN + 1 - minN);
}
void bot_func(Board * _b,Point** kq){
    
    int xR = random(0,SIZE-1);//random x;
    int yR = random(0,SIZE -1);// random y;
    _b->left= kq[xR][yR]._x;
    _b->top = kq[xR][yR]._y;
    GoTo(_b->left,_b->top);
}
void start_game(char typeGame)
{
    Board _b;
    Game *_g;
    Point **kq = NULL;
    kq = (Point **)malloc(SIZE * sizeof(Point *)); //create a 2-D array for saves the results.
    int ikq = 0;

    for (ikq = 0; ikq < SIZE; ikq++)
    {
        kq[ikq] = (Point *)malloc(SIZE * sizeof(Point));
    }
    _g = (Game *)malloc(sizeof(Game));
    reset_data_game((Game *)_g);
    reset_kq((Point **)kq);
    the_board((Game *)_g); // create new board game
    // sets the cursor to the first cell.
    _b.left = kq[0][0]._x;
    _b.top = kq[0][0]._y;
    GoTo(_b.left, _b.top);
    //GoTo(kq[0][0]._x,kq[0][0]._y);// sets the cursor to the first cell.
    int _loop = 1; // loop condition
    char _command; // key from keyboard of PC
    while (_loop)
    { 
        if(_g->turn == PLAYER1 || typeGame == HUMAN){
        _command = getKey();
        }else
        {
            _command = (char)(32);
        }
        
        if (_command == 27)
            exit_game((Point **)kq, &_loop); // esc = 27 (ASCII)
        else
        {
            if (_command == 'A' || _command == 75)
                moveLeft(&_b, kq[0][0]._x); // move the cursor to the left
            if (_command == 'W' || _command == 72)
                moveUp(&_b, kq[0][0]._y); // move the cursor up
            if (_command == 'S' || _command == 80)
                moveDown(&_b, kq[0][SIZE - 1]._y); // move the cursor to down
            if (_command == 'D' || _command == 77)
                moveRight(&_b, kq[SIZE - 1][0]._x); // move the cursor to the right
            if (_command == 13 || _command == 32)
            {   // if _g->turn  == PLAYER 2 -> mode BOT
                // mark and check win lose
                if(_g->turn == PLAYER2 && typeGame == BOT_MAN){
                    bot_func(&_b,(Point**)kq);
                }
                if (processCheckBoard(&_b, (Point **)kq, (Game *)_g))
                {
                    switch (process_finish(&_b, (Point **)kq, (Game *)_g))
                    {
                    case PLAYER1:
                        GoTo(2 * LEFT, 2 * SIZE + 2 * TOP);
                        printf(" X win\n END GAME!\n");
                        _loop = 0;
                        break;
                    case PLAYER2:
                        GoTo(2 * LEFT, 2 * SIZE + 2 * TOP);
                        printf(" O win\n END GAME!\n");
                        _loop = 0;
                        break;
                    case DRAW:
                        GoTo(2 * LEFT, 2 * SIZE + 2 * TOP);
                        printf("DRAW!");
                        _loop = 0;
                        break;

                    case CONTINUE:
                        if (_g->turn == PLAYER1)
                            _g->turn = PLAYER2;
                        else
                            _g->turn = PLAYER1;
                        GoTo(_b.left, _b.top);
                        break;
                    default:
                        break;
                    }
                }
            }
        }
    }
    printf("pree 'N' key to continue new game, any key to exit game...\n");
    _command = getKey();
    system("cls");
    if (_command == 'N')
    {
        f_data((Point **)kq, (Game *)_g);
        if(typeGame == HUMAN){
            start_game(HUMAN);
        }else{
            start_game(BOT_MAN);
        }
        
    }
}
void test_data(Point **kq)
{
    register int i = 0;
    register int j = 0;
    //CHECK DYNAMIC_ALLOCATE_RESULT
    for (i = 0; i < SIZE; i++)
    {
        for (j = 0; j < SIZE; j++)
        {
            printf("kq[%d][%d]._x=%d\n", i, j, kq[i][j]._x);
            printf("kq[%d][%d]._y=%d\n", i, j, kq[i][j]._y);
            printf("kq[%d][%d].check=%d\n", i, j, kq[i][j].check);
        }
    }
    printf("DONE");
}
void guid()
{
    system("cls");
    printf("==HOW TO PLAY THIS GAME?==\n");
    printf("-In the game interface, use buttons such as 'A', 'B', 'C', 'D'\n");
    printf(" or the button marked with an arrow to move to the box to be marked.\n");
    printf("-Use the 'space' key or the 'enter' key to highlight a chess piece there.\n");
    printf("-Use ESC to exit game.\n");
    printf("press any key to return the menu...");
    fflush(stdin);
    getchar();
}
int main()
{
    //SetScreenBufferSize(SIZE * 3, SIZE * 2 + 5 * TOP);
    SetConsoleTitle("CARO GAME");
    system("cls");
    int choice = 1;
    int _mainLoop = 1;
    while (_mainLoop)
    {
        choice = menu_main();
        switch (choice)
        {
        case START_GAME:
            start_game(HUMAN);
            _mainLoop = 0;
            break;
        case BOT:
            start_game(BOT_MAN);
            _mainLoop = 0;
            break;
        case REPLAY:
            system("cls");
            printf("UPLOADING...\n");
            printf("press any key to return menu game.");
            fflush(stdin);
            getchar();
            system("cls");
            break;
        case PLAYER_INFOR:
            system("cls");
            printf("The creator of this game: NGUYEN VIET DAN\n");
            printf("Email: Robotgroup.vn@gmail.com\n");
            printf("UPLOADING...\n");
            printf("press any key to return menu game.");
            fflush(stdin);
            getchar();
            system("cls");
            break;
        case GUID:
            system("cls");
            guid();
            system("cls");
            break;
        case EXIT:
            _mainLoop = 0;
            break;
        }
    }
    return 0;
}