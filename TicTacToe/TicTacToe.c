/*
 * TicTacToe game
 */

/* screen clear command */
#ifdef _WIN64
#define CLR "cls"
#elif
#define CLR "clear"
#endif
/* include files */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
/* global variables */
char qflag = 0; // quit flag
int mode;       // mode state
                    // 0 human vs human
                    // 1 human vs computer
                    // 2 computer vs human
                    // 3 computer vs computer
/* indication of game state */
typedef struct {
    int turn_p;  // save previous turn
    int turn;    // 1(o turn) -1(x turn) 0(game over)
    int row_p;   // save previous row
    int col_p;   // save previous col
    int row;     // current row
    int col;     // current column
    int step;    // step count
    int size;    // length of grid side
    int* grid;   // play board
}State_t;

/********************************************************************
 * @fn      init
 * @brief   initial game state value
 *          size, turn, turn_p, step, mode, row, col
 *          get memory of each grid
 * @param   s - struct of game state
 * @return  none
 *******************************************************************/
void init(State_t* s)
{
    int size;
    printf( "please input board size:" );
    scanf( "%d", &size );
    getchar();
    s->size = size;
    printf( "please choose mode:\n"    \
            "0 human vs human\n"       \
            "1 human vs computer\n"    \
            "2 computer vs human\n"    \
            "3 computer vs computer\n(0~3):" );
    scanf( "%d", &mode);
    getchar();
    s->turn = ((mode>>1)&1 == 0) ? -1 : 1 ;
    s->turn_p = s->turn;
    s->step =  0;
    s->row  = -1;
    s->col  = -1;
    s->grid = (int*)calloc(size*size, sizeof(int));
    if( s->grid == NULL ) {
        printf("%s: calloc failed\n", __func__);
        exit(-1);
    }
}

/********************************************************************
 * @fn      restart
 * @brief   restart game, clear state
 *          turn, step, row, col
 *          save previous turn state to turn_p
 * @param   s - struct of game state
 * @return  none
 *******************************************************************/
void restart(State_t* s)
{
    int i, size;
    size = s->size;
    s->turn = ((mode>>1)&1 == 0) ? -1 : 1 ;
    s->turn_p = s->turn;
    s->step =  0;
    s->row  = -1;
    s->col  = -1;
    for( i = 0; i < size*size; i+=1 )
        *(s->grid+i) = 0;
}

/********************************************************************
 * @fn      show
 * @brief   print game board
 * @param   s - struct of game state
 * @return  none
 *******************************************************************//* cursor */
void show(const State_t* s)
{
    // show help
    printf( "HELP: u--undo, r--restart, q--quit\n" );

    int i, j, size;
    size = s->size;
    printf("   |");
    for( i = 0; i < size; i+=1 ) {
        printf("%2d |", i);          // column header for board
    }
    printf("\n");
    for( j = 0; j < size; j+=1 ) printf("-----");
    printf("\n");
    for( i = 0; i < size ; i+=1 ) {
        printf( "%2d |", i );        // row header for board
        for( j = 0; j < size; j+=1 ) {
            switch( *(s->grid+i*size+j) ) { // grid body
                case  0 : printf( "   |" ); break;
                case  1 : printf( " o |" ); break;
                case -1 : printf( " x |" ); break;
                default : printf( "###|" ); break;
            }
        }
        printf( "\n" );
        for( j = 0; j < size; j+=1 ) printf("-----");
        printf( "\n" );
    }
    printf( "STEP %d @ R %d C %d\n",s->step, s->row, s->col);
}

/********************************************************************
 * @fn      play
 * @brief   play one step
 *          save previous turn, previous row and column
 * @param   s   - struct of game state
 *          row - board row
 *          col - board column
 * @return  1 - invailed coordination or not an empty grid
 *          0 - play successed
 *******************************************************************/
int play(State_t* s, int row, int col)
{
    int size = s->size;
    if( row >= size || col >= size || row < 0 || col < 0) // invailed coordination
        return 1;
    int offset = row*size + col;
    int grid_val  = *(s->grid+offset);
    if( grid_val ) // not an empty grid
        return 1;
    *(s->grid+offset) = (s->turn);
    s->turn_p = s->turn;
    (s->turn) *= -1;
    s->row_p = s->row;
    s->col_p = s->col;
    s->row = row;
    s->col = col;
    s->step++;
    return 0;
}

/********************************************************************
 * @fn      undo
 * @brief   undo one step
 * @param   s - struct of game state
 * @return  none
 *******************************************************************/
void undo(State_t* s)
{
    if( s->row == -1 )  // origin state
        return;
    int offset = (s->row) * (s->size) + s->col;
    *(s->grid+offset) = 0;
    s->turn = s->turn_p;
    s->row  = s->row_p;
    s->col  = s->col_p;
    s->step--;
}

#define WINNER_CODE(v,size) \
    if ( (v == size) || (v == -size) ) \
        return ( v > 0 ? 1 : -1 );

/********************************************************************
 * @fn      phase
 * @brief   check game state
 * @param   s - struct of game state
 * @return  1 - 'o' win
 *         -1 - 'x' win
 *          2 - draw
 *          0 - unknow
 *******************************************************************/
int phase(const State_t* s)
{
    int i, cnt, base;
    int size = s->size;
    int row  = s->row;
    int col  = s->col;
    // traverse current row
    base = row * size;
    cnt = 0;
    for( i = 0; i < size; i+=1 ) {
        cnt += *(s->grid+base+i);
    }
    WINNER_CODE(cnt, size);
    // traverse current column
    base = col;
    cnt = 0;
    for( i = 0; i < size*size; i+=size ) {
        cnt += *(s->grid+base+i);
    }
    WINNER_CODE(cnt, size);
    // backslash
    if( row == col ) {
        cnt  = 0;
        base = 0;
        for(  i = 0; i < size; i+=1 ) {
            cnt += *(s->grid+i*(size+1));
        }
        WINNER_CODE(cnt, size);
    }
    // slash
    if( (row+col) == size-1 ) {
        cnt  = 0;
        base = size - 1;
        for( i = 0; i < size; i+=1) {
            cnt += *(s->grid+i*size+base-i);
        }
        WINNER_CODE(cnt, size);
    }
    // draw ?
    if (s->step == size*size)
        return 2;
    return 0;
}

/********************************************************************
 * @fn      computerAnalyser
 * @brief   computer play
 * @param   s - struct of game state
 * @return  none
 *******************************************************************/
void computerAnalyser(State_t* s)
{
    int i, j;
    int size = s->size;
    if( s->step == (size*size) )
        return;
    // 1. don't allowed foe win
    s->turn *= -1; // become foe
    for( i = 0; i < size; i+=1 ) {
        for( j = 0; j < size; j+=1 ) {
            if ( !play(s,i,j) ) {
                if( phase(s) == s->turn*(-1) ) {
                    undo(s);
                    s->turn *= -1; // defeat foe
                    play(s,i,j);
                    return;
                }
                undo(s);
            }
        }
    }
    // 2. try to win
    s->turn *= -1; // self side
    for( i = 0; i < size; i+=1 ) {
        for( j = 0; j < size; j+=1 ) {
            if ( !play(s,i,j) ) {
                if( phase(s) == s->turn*(-1) ) {
                    play(s,i,j);
                    return;
                }
                undo(s);
            }
        }
    }
    // 3. move on
    while(play(s, rand()%size, rand()%size));
    return;
}

/********************************************************************
 * @fn      inputHandler
 * @brief   game functions
 * @param   s     - struct of game state
 *          input - user input string
 * @return  none
 *******************************************************************/
void inputHandler(State_t* s, char* input)
{
    switch( *input ) {
        case 'q': qflag = 1;  break;
        case 'r': restart(s); break;
        case 'u': undo(s);    break;
        default:              break;
    }
}

/********************************************************************
 * @fn      humanPlay
 * @brief   human play
 * @param   s - struct of game state
 * @return  none
 *******************************************************************/
void humanPlay(State_t* s)
{
    char input[20];
    int row, col;
    scanf( "%[^\n]", &input );
    getchar();
    if( sscanf(input, "%d %d", &row, &col) == 2 ) // play
        play(s, row, col);
    inputHandler(s, input);
}

/********************************************************************
 * @fn      info
 * @brief   show game information
 * @param   s - struct of game state
 * @return  none
 *******************************************************************/
void info(State_t* s)
{
    switch( phase(s) ) {
        case  1: printf( "o win\n" ); s->turn_p = s->turn; s->turn = 0; break;
        case -1: printf( "x win\n" ); s->turn_p = s->turn; s->turn = 0; break;
        case  2: printf( "draw\n" );  s->turn_p = s->turn; s->turn = 0; break;
        case  0:
                printf( "next move is " );
                printf( (s->turn == 1) ? "'o':":"'x':" );
                break;
        default:
                break;
    }
    if( !(s->turn) ) {
        printf( "game over\n");
        char ch[2];
        while (!(ch[0] = getchar()));
        inputHandler(s, ch);
    }
}

// run program
int main(int argc, const char *argv[])
{
    srand((unsigned)time(0)); // rand seed

    State_t state;            // game state

    // first show
    init(&state);
    system("cls");
    show(&state);
    printf( "next move is " );
    printf( state.turn == 1 ? "'o':":"'x':" );

    // loop
    while( !qflag ) {
        // o's turn
        ((mode>>1)&1) ? computerAnalyser(&state) : humanPlay(&state);

        system(CLR);
        show(&state);
        info(&state);

        if( qflag ) break;

        // x's turn
        ( mode & 1 )  ? computerAnalyser(&state) : humanPlay(&state);

        system(CLR);
        show(&state);
        info(&state);
    }

    free(state.grid);
    return 0;
}
