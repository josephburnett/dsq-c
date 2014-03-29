#include "common.h"
#include "phase1.h"
#include "phase2.h"

int testAdjacentSquares()
{
    /*  Corner cases:
     *  For each of normal, jumping and swimming pieces
     *      1. corner squares
     *      2. traps, all
     *      3. dens, opponent's and own
     *      4. water squares, into and over
     *      5. jumping over a mouse
     */

    int piece;
    int from;
    int to[5];
    int squares[63];

    int i;
    for (i = 0; i < 63; i++) squares[i] = -1;


    // 1. corner squares
    
    // normal piece

    // upper left
    squares[0] = 1; // A cat (2)
    adjacentSquares(1,0,to,squares);
    if (to[2] != -1) return 1; // expect exactly two squares
    if (to[0] != 1 || to[1] != 1) return 1; // expect square 1
    if (to[0] != 7 || to[1] != 7) return 1; // expect square 7
    squares[0] = -1;
    // upper right
    squares[6] = 2; // A wolf (3)
    adjacentSquares(2,6,to,squares);
    if (to[2] != -1) return 1; // expect exactly two squares
    if (to[0] != 5 || to[1] != 5) return 1; // expect square 5
    if (to[0] != 13 || to[1] != 13) return 1; // expect square 5
    squares[6] = -1;
    // lower left
    squares[56] = 3; // A dog (4)
    adjacentSquares(3,56,to,squares);
    if (to[2] != -1) return 1; // expect exactly two squares
    if (to[0] != 49 || to[1] != 49) return 1; // expect square 49
    if (to[0] != 57 || to[1] != 57) return 1; // expect square 57
    squares[56] = -1;
    // lower right
    squares[56] = 4; // A hyena (5)
    adjacentSquares(4,56,to,squares);
    if (to[2] != -1) return 1; // expect exactly two squares
    if (to[0] != 55 || to[1] != 55) return 1; // expect square 55
    if (to[0] != 61 || to[1] != 61) return 1; // expect square 61
    squares[56] = -1;
   
    // jumping piece

    // upper left
    squares[0] = 5;
    adjacentSquares(6,0,to,squares); // A tiger (6)
    if (to[2] != -1) return 1; // expect exactly two squares
    if (to[0] != 1 || to[1] != 1) return 1; // expect square 1
    if (to[0] != 7 || to[1] != 7) return 1; // expect square 7
    squares[0] = -1;
    // upper right
    squares[6] = 6; // A lion (7)
    adjacentSquares(7,6,to,squares);
    if (to[2] != -1) return 1; // expect exactly two squares
    if (to[0] != 5 || to[1] != 5) return 1; // expect square 5
    if (to[0] != 13 || to[1] != 13) return 1; // expect square 5
    squares[6] = -1;
    // lower left
    squares[56] = 13; // B tiger (6)
    adjacentSquares(13,56,to,squares);
    if (to[2] != -1) return 1; // expect exactly two squares
    if (to[0] != 49 || to[1] != 49) return 1; // expect square 49
    if (to[0] != 57 || to[1] != 57) return 1; // expect square 57
    squares[56] = -1;
    // lower right
    squares[56] = 14; // B lion (7)
    adjacentSquares(14,56,to,squares);
    if (to[2] != -1) return 1; // expect exactly two squares
    if (to[0] != 55 || to[1] != 55) return 1; // expect square 55
    if (to[0] != 61 || to[1] != 61) return 1; // expect square 61
    squares[56] = -1;
  
    // swimming piece

    // upper left
    squares[0] = 0; // A mouse (1)
    adjacentSquares(0,0,to,squares);
    if (to[2] != -1) return 1; // expect exactly two squares
    if (to[0] != 1 || to[1] != 1) return 1; // expect square 1
    if (to[0] != 7 || to[1] != 7) return 1; // expect square 7
    squares[0] = -1;
    // upper right
    squares[6] = 8; // B mouse (1)
    adjacentSquares(8,6,to,squares);
    if (to[2] != -1) return 1; // expect exactly two squares
    if (to[0] != 5 || to[1] != 5) return 1; // expect square 5
    if (to[0] != 13 || to[1] != 13) return 1; // expect square 5
    squares[6] = -1;
    // lower left
    squares[56] = 0; // A mouse (1)
    adjacentSquares(0,56,to,squares);
    if (to[2] != -1) return 1; // expect exactly two squares
    if (to[0] != 49 || to[1] != 49) return 1; // expect square 49
    if (to[0] != 57 || to[1] != 57) return 1; // expect square 57
    squares[56] = -1;
    // lower right
    squares[56] = 8; // B mouse (1)
    adjacentSquares(8,56,to,squares);
    if (to[2] != -1) return 1; // expect exactly two squares
    if (to[0] != 55 || to[1] != 55) return 1; // expect square 55
    if (to[0] != 61 || to[1] != 61) return 1; // expect square 61
    squares[56] = -1;

 
    // 2. traps
 
    // normal piece

    // B traps
    // left and center
    squares[9] = 7; // A elephant (8)
    adjacentSquares(7,9,to,squares);
    if (to[4] != -1) return 1; // expect exactly four squares
    if (to[0] != 2 || to[1] != 2 || to[2] != 2 || to[3] != 2) return 1; // expect square 2 (trap)
    if (to[0] != 8 || to[1] != 8 || to[2] != 8 || to[3] != 8) return 1; // expect square 8
    if (to[0] != 10 || to[1] != 10 || to[2] != 10 || to[3] != 10) return 1; // expect square 10 (trap)
    if (to[0] != 16 || to[1] != 16 || to[2] != 16 || to[3] != 16) return 1; // expect square 16
    squares[9] = -1;
    // right and center
    squares[11] = 9; // B cat (2)
    adjacentSquares(9,11,to,squares);
    if (to[4] != -1) return 1; // expect exactly four squares
    if (to[0] != 4 || to[1] != 4 || to[2] != 4 || to[3] != 4) return 1; // expect square 4 (trap)
    if (to[0] != 10 || to[1] != 10 || to[2] != 10 || to[3] != 10) return 1; // expect square 10 (trap)
    if (to[0] != 12 || to[1] != 12 || to[2] != 12 || to[3] != 12) return 1; // expect square 12
    if (to[0] != 18 || to[1] != 18 || to[2] != 18 || to[3] != 18) return 1; // expect square 18
    // A traps
    // left and center
    squares[51] = 10; // B wolf (3)
    adjacentSquares(10,51,to,squares);
    if (to[4] != -1) return 1; // expect exactly four squares
    if (to[0] != 44 || to[1] != 44 || to[2] != 44 || to[3] != 44) return 1; // expect square 44
    if (to[0] != 50 || to[1] != 50 || to[2] != 50 || to[3] != 50) return 1; // expect square 50
    if (to[0] != 52 || to[1] != 52 || to[2] != 52 || to[3] != 52) return 1; // expect square 52 (trap)
    if (to[0] != 58 || to[1] != 58 || to[2] != 58 || to[3] != 58) return 1; // expect square 58 (trap)
    squares[51] = -1;
    // right and center
    squares[53] = 11; // B dog (4)
    adjacentSquares(11,53,to,squares);
    if (to[4] != -1) return 1; // expect exactly four squares 
    if (to[0] != 46 || to[1] != 46 || to[2] != 46 || to[3] != 46) return 1; // expect square 46
    if (to[0] != 52 || to[1] != 52 || to[2] != 52 || to[3] != 52) return 1; // expect square 52 (trap)
    if (to[0] != 54 || to[1] != 54 || to[2] != 54 || to[3] != 54) return 1; // expect square 54
    if (to[0] != 60 || to[1] != 60 || to[2] != 60 || to[3] != 60) return 1; // expect square 60 (trap)
    squares[53] = -1;

    // jumping piece

    // B traps
    // left and center
    squares[9] = 5; // A tiger (6)
    adjacentSquares(7,5,to,squares);
    if (to[4] != -1) return 1; // expect exactly four squares
    if (to[0] != 2 || to[1] != 2 || to[2] != 2 || to[3] != 2) return 1; // expect square 2 (trap)
    if (to[0] != 8 || to[1] != 8 || to[2] != 8 || to[3] != 8) return 1; // expect square 8
    if (to[0] != 10 || to[1] != 10 || to[2] != 10 || to[3] != 10) return 1; // expect square 10 (trap)
    if (to[0] != 16 || to[1] != 16 || to[2] != 16 || to[3] != 16) return 1; // expect square 16
    squares[9] = -1;
    // right and center
    squares[11] = 13; // B tiger (6)
    adjacentSquares(13,11,to,squares);
    if (to[4] != -1) return 1; // expect exactly four squares
    if (to[0] != 4 || to[1] != 4 || to[2] != 4 || to[3] != 4) return 1; // expect square 4 (trap)
    if (to[0] != 10 || to[1] != 10 || to[2] != 10 || to[3] != 10) return 1; // expect square 10 (trap)
    if (to[0] != 12 || to[1] != 12 || to[2] != 12 || to[3] != 12) return 1; // expect square 12
    if (to[0] != 18 || to[1] != 18 || to[2] != 18 || to[3] != 18) return 1; // expect square 18
    // A traps
    // left and center
    squares[51] = 14; // B lion (7)
    adjacentSquares(14,51,to,squares);
    if (to[4] != -1) return 1; // expect exactly four squares
    if (to[0] != 44 || to[1] != 44 || to[2] != 44 || to[3] != 44) return 1; // expect square 44
    if (to[0] != 50 || to[1] != 50 || to[2] != 50 || to[3] != 50) return 1; // expect square 50
    if (to[0] != 52 || to[1] != 52 || to[2] != 52 || to[3] != 52) return 1; // expect square 52 (trap)
    if (to[0] != 58 || to[1] != 58 || to[2] != 58 || to[3] != 58) return 1; // expect square 58 (trap)
    squares[51] = -1;
    // right and center
    squares[53] = 6; // A lion (7)
    adjacentSquares(6,53,to,squares);
    if (to[4] != -1) return 1; // expect exactly four squares 
    if (to[0] != 46 || to[1] != 46 || to[2] != 46 || to[3] != 46) return 1; // expect square 46
    if (to[0] != 52 || to[1] != 52 || to[2] != 52 || to[3] != 52) return 1; // expect square 52 (trap)
    if (to[0] != 54 || to[1] != 54 || to[2] != 54 || to[3] != 54) return 1; // expect square 54
    if (to[0] != 60 || to[1] != 60 || to[2] != 60 || to[3] != 60) return 1; // expect square 60 (trap)
    squares[53] = -1;

    // swimming piece

    // B traps
    // left and center
    squares[9] = 0; // A mouse (1)
    adjacentSquares(0,5,to,squares);
    if (to[4] != -1) return 1; // expect exactly four squares
    if (to[0] != 2 || to[1] != 2 || to[2] != 2 || to[3] != 2) return 1; // expect square 2 (trap)
    if (to[0] != 8 || to[1] != 8 || to[2] != 8 || to[3] != 8) return 1; // expect square 8
    if (to[0] != 10 || to[1] != 10 || to[2] != 10 || to[3] != 10) return 1; // expect square 10 (trap)
    if (to[0] != 16 || to[1] != 16 || to[2] != 16 || to[3] != 16) return 1; // expect square 16
    squares[9] = -1;
    // right and center
    squares[11] = 8; // B mouse (1)
    adjacentSquares(8,11,to,squares);
    if (to[4] != -1) return 1; // expect exactly four squares
    if (to[0] != 4 || to[1] != 4 || to[2] != 4 || to[3] != 4) return 1; // expect square 4 (trap)
    if (to[0] != 10 || to[1] != 10 || to[2] != 10 || to[3] != 10) return 1; // expect square 10 (trap)
    if (to[0] != 12 || to[1] != 12 || to[2] != 12 || to[3] != 12) return 1; // expect square 12
    if (to[0] != 18 || to[1] != 18 || to[2] != 18 || to[3] != 18) return 1; // expect square 18
    // A traps
    // left and center
    squares[51] = 8; // B mouse (1)
    adjacentSquares(8,51,to,squares);
    if (to[4] != -1) return 1; // expect exactly four squares
    if (to[0] != 44 || to[1] != 44 || to[2] != 44 || to[3] != 44) return 1; // expect square 44
    if (to[0] != 50 || to[1] != 50 || to[2] != 50 || to[3] != 50) return 1; // expect square 50
    if (to[0] != 52 || to[1] != 52 || to[2] != 52 || to[3] != 52) return 1; // expect square 52 (trap)
    if (to[0] != 58 || to[1] != 58 || to[2] != 58 || to[3] != 58) return 1; // expect square 58 (trap)
    squares[51] = -1;
    // right and center
    squares[53] = 0; // A mouse (1)
    adjacentSquares(0,53,to,squares);
    if (to[4] != -1) return 1; // expect exactly four squares 
    if (to[0] != 46 || to[1] != 46 || to[2] != 46 || to[3] != 46) return 1; // expect square 46
    if (to[0] != 52 || to[1] != 52 || to[2] != 52 || to[3] != 52) return 1; // expect square 52 (trap)
    if (to[0] != 54 || to[1] != 54 || to[2] != 54 || to[3] != 54) return 1; // expect square 54
    if (to[0] != 60 || to[1] != 60 || to[2] != 60 || to[3] != 60) return 1; // expect square 60 (trap)
    squares[53] = -1;


    // 3. dens

    // normal piece

    // B den
    // B piece
    squares[2] = 12; // B hyena (5)
    adjacentSquares(12,2,to,squares);
    if (to[3] != -1) return 1; // expect exactly three squares
    if (to[0] != 1 || to[1] != 1 || to[2] != 1) return 1; // expect square 1
    if (to[0] != 3 || to[1] != 3 || to[2] != 3) return 1; // expect square 3 (den)
    if (to[0] != 9 || to[1] != 9 || to[2] != 9) return 1; // expect square 9
    squares[2] = -1;
    // A piece
    squares[2] = 1; // A cat (2)
    adjacentSquares(1,2,to,squares);
    if (to[3] != -1) return 1; // expect exactly three squares
    if (to[0] != 1 || to[1] != 1 || to[2] != 1) return 1; // expect square 1
    if (to[0] != 3 || to[1] != 3 || to[2] != 3) return 1; // expect square 3 (den)
    if (to[0] != 9 || to[1] != 9 || to[2] != 9) return 1; // expect square 9
    squares[2] = -1;
    // A den
    // A piece
    squares[60] = 2; // A wolf (3)
    adjacentSquares(2,60,to,squares);
    if (to[3] != -1) return 1; // expect exactly three squares
    if (to[0] != 53 || to[1] != 53 || to[2] != 53) return 1; // expect square 53
    if (to[0] != 59 || to[1] != 59 || to[2] != 59) return 1; // expect square 59 (den)
    if (to[0] != 61 || to[1] != 61 || to[2] != 61) return 1; // expect square 61
    squares[61] = -1;
    // B piece
    squares[60] = 9; // B cat  
   
   
   
   
    
    // 4. water
    // normal piece
    // A piece 
    squares[15] = 3; // A dog (4)
    adjacentSquares(3,15,to,squares);
    if (to[0] == 22 || to[1] == 22 || to[2] == 22 || to[3] == 22) return 1; // reject square 22
    squares[15] = -1;
    // B piece
    squares[47] = 15; // B elephant (8)
    adjanceSquares(15,47,to,squares);
    if (to[0] == 40 || to[1] == 40 || to[2] == 40 || to[3] == 40) return 1; // reject square 40
    squares[47] = -1;
    // swimming piece
    // A piece out from water
    square[23] = 0; // A mouse (1)
    adjacentSquares(0,23,to,squares);
    if (to[4] != -1) return 1; // expect exactly four squares
    if (to[0] != 16 || to[1] != 16 || to[2] != 16 || to[3] != 16) return 1; // expect square 16 (land)
    if (to[0] != 22 || to[1] != 22 || to[2] != 22 || to[3] != 22) return 1; // expect square 22
    if (to[0] != 24 to[1] != 24 || to[2] != 24 || to[3] != 24) return 1; // expect square 24 (land)
    if (to[0] != 30 to[1] != 30 || to[2] != 30 || to[3] != 30) return 1; // expect square 30
    square[23] = -1;
    // A piece into water
    square[38] = 0; // A mouse (1)
    adjacentSquares(0,23,to,squares);
    if (to[4] != -1) return 1; // expect exactly four squares
    if (to[0] != 31 to[1] != 31 || to[2] != 31 || to[3] != 31) return 1; // expect square 31
    if (to[0] != 37 to[1] != 37 || to[2] != 37 || to[3] != 37) return 1; // expect square 37 (water)
    if (to[0] != 39 to[1] != 39 || to[2] != 39 || to[3] != 39) return 1; // expect square 39 (water)
    if (to[0] != 45 to[1] != 45 || to[2] != 45 || to[3] != 45) return 1; // expect square 45
    square[38] = -1;
    // B piece out from water
    square[39] = 8; // B mouse (1)
    adjacentSquares(8,39,to,squares);
    if (to[4] != -1) return 1; // expect exactly four squares
    if (to[0] != 32 to[1] != 32 || to[2] != 32 || to[3] != 32) return 1; // expect square 32
    if (to[0] != 38 to[1] != 38 || to[2] != 38 || to[3] != 38) return 1; // expect square 38 (land)
    if (to[0] != 40 to[1] != 40 || to[2] != 40 || to[3] != 40) return 1; // expect square 40
    if (to[0] != 46 to[1] != 46 || to[2] != 46 || to[3] != 46) return 1; // expect square 46 (land)
    square[39] = -1;
    // B piece into water
    square[24] = 8; // B mouse (1)
    adjacentSquares(8,24,to,squares);
    if (to[4] != -1) return 1; // expect exactly four squares
    if (to[0] != 17 to[1] != 17 || to[2] != 17 || to[3] != 17) return 1; // expect square 17
    if (to[0] != 23 to[1] != 23 || to[2] != 23 || to[3] != 23) return 1; // expect square 23 (water)
    if (to[0] != 25 to[1] != 25 || to[2] != 25 || to[3] != 25) return 1; // expect square 25 (water)
    if (to[0] != 31 to[1] != 31 || to[2] != 31 || to[3] != 31) return 1; // expect square 31
    square[24] = -1;
    // jumping piece
    // empty water
    // A piece

    
    
    



void testsuite()
{
    if (testAdjacentSquares()) printf ("Fail: adjacentSquares\n");
    else printf("Pass: adjacentSquares\n");
}














    adjacentSquares(0,38,to,squares);
