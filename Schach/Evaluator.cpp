//
//  Evaluator.cpp
//  Schach
//
//  Created by Thomas Redding on 7/8/14.
//  Copyright (c) 2014 Thomas Redding. All rights reserved.
//

#include "Evaluator.h"

Evaluator::Evaluator(Board* aiBoard)
{
    board = aiBoard;
    exchangeSimulator.precomputeExchanges();
    
    for(int x=0; x<8; x++)
    {
        openingHumanPassedPawnBonus[7-x] = openingCpuPassedPawnBonus[x];
        endingHumanPassedPawnBonus[7-x] = endingCpuPassedPawnBonus[x];
        for(int y=0; y<8; y++)
        {
            humanKingValue[8*x+7-y] = cpuKingValue[8*x+y];
            for(int i=0; i<6; i++)
            {
                humanMoveBonus[i][8*x+7-y] = cpuMoveBonus[i][8*x+y];
            }
        }
    }
    
    std::cout << "(READY)";
}

extern int numberOfEvals;

int Evaluator::getPieceCount()
{
    return evaluatorTracker.cpuKnights + evaluatorTracker.cpuBishops + evaluatorTracker.cpuRooks + evaluatorTracker.cpuQueens + evaluatorTracker.humanKnights + evaluatorTracker.humanBishops + evaluatorTracker.humanRooks + evaluatorTracker.humanQueens + 2;
}

uint8_t Evaluator::getManCount()
{
    uint8_t pieces = 0;
    uint64_t x;
    x = board->user | board->cpu;
    while(x > 0) {
        pieces++;
        x -= (x & -x);
    }
    return pieces;
}

float Evaluator::staticExchangeEvaluation(int sq, int layer)
{
    setSpaceControlOfSquare(sq, layer);
    return ((float) pieceSafe(sq, layer))/100.0;
}

void Evaluator::setSpaceControlOfSquare(int sq, int layer)
{
    int x = sq/8;
    int y = sq%8;
    int* arr = spaceControl[layer][sq];
    // pawns
    memset(spaceControl[layer][sq],0,sizeof(spaceControl[layer][sq]));
    /*
     0  human king
     1  human queen
     2  human rook
     3  human minor
     4  human pawn
     5  cpu pawn
     6  cpu minor
     7  cpu rook
     8  cpu queen
     9  cpu king
    */
    
    // pawns
    if(x != 0)
    {
        if(y != 0)
        {
            if(board->b[sq-9] == -1)
            {
                arr[4]++;
            }
        }
        if(y != 7)
        {
            if(board->b[sq-7] == 1)
            {
                arr[5]++;
            }
        }
    }
    if(x != 7)
    {
        if(y != 0)
        {
            if(board->b[sq+7] == -1)
            {
                arr[4]++;
            }
        }
        if(y != 7)
        {
            if(board->b[sq+9] == 1)
            {
                arr[5]++;
            }
        }
    }
    
    // knights
    if(x > 0)
    {
        if(y > 1 && board->b[sq - 10] == 2)
        {
            arr[6]++;
        }
        else if(y > 1 && board->b[sq - 10] == -2)
        {
            arr[3]++;
        }
        
        if(y < 6 && board->b[sq - 6] == 2)
        {
            arr[6]++;
        }
        else if(y < 6 && board->b[sq - 6] == -2)
        {
            arr[3]++;
        }
        
        if(x > 1)
        {
            if(y > 0 && board->b[sq - 17] == 2)
            {
                arr[6]++;
            }
            else if(y > 0 && board->b[sq - 17] == -2)
            {
                arr[3]++;
            }
            
            if(y < 7 && board->b[sq - 15] == 2)
            {
                arr[6]++;
            }
            else if(y < 7 && board->b[sq - 15] == -2)
            {
                arr[3]++;
            }
        }
    }
    if(x < 7)
    {
        if(y > 1 && board->b[sq + 6] == 2)
        {
            arr[6]++;
        }
        else if(y > 1 && board->b[sq + 6] == 2)
        {
            arr[3]++;
        }
        
        if(y < 6 && board->b[sq + 10] == 2)
        {
            arr[6]++;
        }
        else if(y < 6 && board->b[sq + 10] == -2)
        {
            arr[3]++;
        }
        
        if(x < 6)
        {
            if(y > 0 && board->b[sq + 15] == 2)
            {
                arr[6]++;
            }
            else if(y > 0 && board->b[sq + 15] == -2)
            {
                arr[3]++;
            }
            
            if(y < 7 && board->b[sq + 17] == 2)
            {
                arr[6]++;
            }
            else if(y < 7 && board->b[sq + 17] == 2)
            {
                arr[3]++;
            }
        }
    }
    
    // king
    if(x != 0)
    {
        if(board->b[sq - 8] == 6)
        {
            arr[9]++;
        }
        else if(board->b[sq - 8] == -6)
        {
            arr[0]++;
        }
        
        if(y != 0 && board->b[sq - 9] == 6)
        {
            arr[9]++;
        }
        else if(y != 0 && board->b[sq - 9] == 6)
        {
            arr[0]++;
        }
        
        if(y != 7 && board->b[sq - 7] == 6)
        {
            arr[9]++;
        }
        else if(y != 7 && board->b[sq - 7] == -6)
        {
            arr[0]++;
        }
    }
    if(x != 7) {
        if(board->b[sq + 8] == 6)
        {
            arr[9]++;
        }
        else if(board->b[sq + 8] == -6)
        {
            arr[0]++;
        }
        
        if(y != 0 && board->b[sq + 7] == 6)
        {
            arr[9]++;
        }
        else if(y != 0 && board->b[sq + 7] == -6)
        {
            arr[0]++;
        }
        
        if(y != 7 && board->b[sq + 9] == 6)
        {
            arr[9]++;
        }
        if(y != 7 && board->b[sq + 9] == -6)
        {
            arr[0]++;
        }
    }
    if(y != 0)
    {
        if(board->b[sq-1] == 6)
        {
            arr[9]++;
        }
        else if(board->b[sq-1] == -6)
        {
            arr[0]++;
        }
    }
    if(y != 7)
    {
        if(board->b[sq+1] == 6)
        {
            arr[9]++;
        }
        else if(board->b[sq+1] == -6)
        {
            arr[0]++;
        }
    }
    
    int i;
    // rook-movements
    // right
    for(i=sq+8; i<64; i+=8)
    {
        if(board->b[i] != 0)
        {
            if(board->b[i] == 5)
            {
                arr[8]++;
                break;
            }
            else if(board->b[i] == -5)
            {
                arr[1]++;
                break;
            }
            else if(board->b[i] == 4)
            {
                arr[7]++;
            }
            else if(board->b[i] == -4)
            {
                arr[2]++;
            }
        }
    }
    
    // left
    for(i=sq-8; i>=0; i-=8)
    {
        if(board->b[i] != 0)
        {
            if(board->b[i] == 5)
            {
                arr[8]++;
                break;
            }
            else if(board->b[i] == -5)
            {
                arr[1]++;
                break;
            }
            else if(board->b[i] == 4)
            {
                arr[7]++;
            }
            else if(board->b[i] == -4)
            {
                arr[2]++;
            }
        }
    }
    
    // up
    int limit = sq/8;
    limit *= 8+7;
    for(i=sq+1; i<=limit; i++)
    {
        if(board->b[i] != 0)
        {
            if(board->b[i] == 5)
            {
                arr[8]++;
                break;
            }
            else if(board->b[i] == -5)
            {
                arr[1]++;
                break;
            }
            else if(board->b[i] == 4)
            {
                arr[7]++;
            }
            else if(board->b[i] == -4)
            {
                arr[2]++;
            }
        }
    }
    
    // down
    limit = sq/8;
    for(i=sq+1; i>=limit; i--)
    {
        if(board->b[i] != 0)
        {
            if(board->b[i] == 5)
            {
                arr[8]++;
                break;
            }
            else if(board->b[i] == -5)
            {
                arr[1]++;
                break;
            }
            else if(board->b[i] == 4)
            {
                arr[7]++;
            }
            else if(board->b[i] == -4)
            {
                arr[2]++;
            }
        }
    }
    
    
    // down & left
    limit = fmin(x, y);
    int square = sq;
    for(i=0; i<limit; i++)
    {
        square -= 9;
        if(board->b[square] != 0)
        {
            if(board->b[square] == 5)
            {
                arr[8]++;
                break;
            }
            else if(board->b[square] == -5)
            {
                arr[1]++;
                break;
            }
            else if(board->b[square] == 3)
            {
                arr[7]++;
            }
            else if(board->b[square] == -3)
            {
                arr[2]++;
            }
        }
    }
    
    // down & right
    limit = fmin(7-x, y);
    square = sq;
    for(i=0; i<limit; i++)
    {
        square += 7;
        if(board->b[square] != 0)
        {
            if(board->b[square] == 5)
            {
                arr[8]++;
                break;
            }
            else if(board->b[square] == -5)
            {
                arr[1]++;
                break;
            }
            else if(board->b[square] == 3)
            {
                arr[7]++;
            }
            else if(board->b[square] == -3)
            {
                arr[2]++;
            }
        }
    }
    
    // up & left
    limit = fmin(x, 7-y);
    square = sq;
    for(i=0; i<limit; i++)
    {
        square -= 7;
        if(board->b[square] != 0)
        {
            if(board->b[square] == 5)
            {
                arr[8]++;
                break;
            }
            else if(board->b[square] == -5)
            {
                arr[1]++;
                break;
            }
            else if(board->b[square] == 3)
            {
                arr[7]++;
            }
            else if(board->b[square] == -3)
            {
                arr[2]++;
            }
        }
    }
    
    // up & right
    limit = fmin(7-x, 7-y);
    square = sq;
    for(i=0; i<limit; i++)
    {
        square += 9;
        if(board->b[square] != 0)
        {
            if(board->b[square] == 5)
            {
                arr[8]++;
                break;
            }
            else if(board->b[square] == -5)
            {
                arr[1]++;
                break;
            }
            else if(board->b[square] == 3)
            {
                arr[7]++;
            }
            else if(board->b[square] == -3)
            {
                arr[2]++;
            }
        }
    }
}

int Evaluator::pawnStructureEval(float stage)
{
    int cpuPawn = 0;    // how many pawns are in a column
    int humanPawn = 0;
    
    int lastCpuPawn, lastHumanPawn; // how many pawns were in the last column
    
    int net = 0;
    
    int i,j,k;
    
    int sto;
    
    float stoF;
    
    int cpuPawnCount = 0;
    int humanPawnCount = 0;
    for(i = 0; i < 8; i++)
    {
        lastCpuPawn = cpuPawn;
        lastHumanPawn = humanPawn;
        cpuPawn = 0;
        humanPawn = 0;
        for(j = 0; j < 8; j++)
        {
            k = 8*i+j;
            if((*board).b[k]>0)
            {
                // ai piece
                if((*board).b[k] == 1)
                {
                    cpuPawn++;
                }
            }
            else
            {
                // human piece
                if((*board).b[k] == -1)
                {
                    humanPawn++;
                }
            }
        }
        if(cpuPawn>1)
        {
            net += openingDoubledPawnsBonus * stage + endingDoubledPawnsBonus * (1-stage);
        }
        if(humanPawn>1)
        {
            net -= openingDoubledPawnsBonus * stage + endingDoubledPawnsBonus * (1-stage);
        }
        
        if(cpuPawn>=1 && humanPawn==0)
        {
            
            
            // check for passed pawn
            sto = -1;
            for(j=0;j<8;j++)
            {
                if((*board).b[8*i+j]==1)
                {
                    sto = j;
                }
            }
            stoF = 1;
            if(sto != -1)
            {
                if(i!=0)
                {
                    for(j=sto-1;j>0;j--)
                    {
                        if((*board).b[8*(i-1)+j]==-1)
                        {
                            stoF = 0;
                            break;
                        }
                    }
                }
                if(i!=7 && stoF==1)
                {
                    for(j=sto-1;j>0;j--)
                    {
                        if((*board).b[8*(i+1)+j]==-1)
                        {
                            stoF = 0;
                            break;
                        }
                    }
                }
                if(stoF==1)
                {
                    
                    net += openingCpuPassedPawnBonus[sto] * stage + endingCpuPassedPawnBonus[sto] * (1-stage);
                }
            }
        }
        else if(humanPawn>=1 && cpuPawn==0)
        {
            // check for passed pawn
            sto = -1;
            for(j=8;j>0;j--)
            {
                if((*board).b[8*i+j]==-1)
                {
                    sto = j;
                    break;
                }
            }
            if(sto != -1)
            {
                stoF = 1;
                if(i!=0)
                {
                    for(j=sto+1;j<8;j++)
                    {
                        if((*board).b[8*(i-1)+j]==1)
                        {
                            stoF = 0;
                            break;
                        }
                    }
                }
                if(i!=7 && stoF==1)
                {
                    for(j=sto+1;j<8;j++)
                    {
                        if((*board).b[8*(i+1)+j]==1)
                        {
                            stoF = 0;
                            break;
                        }
                    }
                }
                if(stoF==1)
                {
                    
                    net -= openingHumanPassedPawnBonus[sto] * stage + endingHumanPassedPawnBonus[sto] * (1-stage);
                }
            }
        }
        if(i!=0 && i!=7 && humanPawn==0 && lastHumanPawn!=0)
        {
            net -= openingPawnIslandBonus * stage + endingPawnIslandBonus * (1-stage);
        }
        else if(i!=0 && i!=7 && cpuPawn==0 && lastCpuPawn!=0)
        {
            net += openingPawnIslandBonus * stage + endingPawnIslandBonus * (1-stage);
        }
        humanPawnCount += humanPawn;
        cpuPawnCount += cpuPawn;
        lastHumanPawn = humanPawn;
        lastCpuPawn = cpuPawn;
    }
    if(cpuPawn==0)
    {
        net -= openingPawnIslandBonus * stage + endingPawnIslandBonus * (1-stage);
    }
    if(humanPawn==0)
    {
        net += openingPawnIslandBonus * stage + endingPawnIslandBonus * (1-stage);
    }
    return net;
}

int Evaluator::kpVk(int layer)
{
    // 1,000,000 < x < 2,000,000 == forced mate
    // if you know turns_to_mate: return 2,000,000 - layer - turns_to_mate
    
    bool userToMove = layer % 2;
    Tile cpuKing = board->lsm(board->bitmaps[CPU_KING + 6]);
    Tile userKing = board->lsm(board->bitmaps[USER_KING + 6]);
    Tile pawn = board->lsm(board->bitmaps[CPU_PAWN + 6]);
    
    // Rule of the Square
    if(pawn.rank == 6)
    {
        // TODO: check
        uint8_t distance = std::max(abs(userKing.file - pawn.file), abs(userKing.rank - (pawn.rank - 1)));
        if(distance >= pawn.rank)
        {
            if(userToMove)
            {
                return 1100000 - layer - 2 * distance;
            }
            else if(distance > pawn.rank)
            {
                return 1100000 - layer - 2 * distance;
            }
        }
    }
    else
    {
        uint8_t distance = std::max(abs(userKing.file - pawn.file), abs(userKing.rank - pawn.rank));
        if(distance > pawn.rank)
        {
            if(userToMove)
            {
                return 1100000 - layer - 2 * distance;
            }
            else if(distance - 1 > pawn.rank)
            {
                return 1100000 - layer - 2 * distance;
            }
        }
    }
    int eval;
    if(pawn.rank < 4)
    {
        eval = 879;
    }
    else
    {
        if(pawn.rank < 6)
        {
            if(pawn.rank == 4)
                eval = 1146;
            else
                eval = 1268;
        }
        else
        {
            if(pawn.rank == 6)
                eval = 1674;
            else
                eval = 1707;
        }
    }
    if(abs(userKing.file - pawn.file) < 2)
    {
        if(userKing.rank + 2 == pawn.rank)
        {
            eval -= 500;
        }
        else if(pawn.rank < 4)
        {
            if(userKing.rank + 1 == pawn.rank)
            {
                eval -= 500;
            }
            else if(pawn.rank == 6 && userKing.rank == 6)
            {
                eval -= 500;
            }
        }
    }
    if(abs(cpuKing.file - pawn.file) < 2)
    {
        if(cpuKing.rank + 2 == pawn.rank)
        {
            eval += 500;
        }
        else if(pawn.rank < 4)
        {
            if(cpuKing.rank + 1 == pawn.rank)
            {
                eval += 500;
            }
            else if(pawn.rank == 6 && cpuKing.rank == 6)
            {
                eval += 500;
            }
        }
    }
    eval += 16 - (pawn.rank + cpuKing.rank);
    return eval;
}

int Evaluator::kVkp(int layer)
{
    bool userToMove = layer % 2;
    Tile cpuKing = board->lsm(board->bitmaps[CPU_KING + 6]);
    Tile userKing = board->lsm(board->bitmaps[USER_KING + 6]);
    Tile pawn = board->lsm(board->bitmaps[CPU_PAWN + 6]);
    
    // Rule of the Square
    if(pawn.rank == 6)
    {
        // TODO: check
        uint8_t distance = std::max(abs(userKing.file - pawn.file), abs(userKing.rank - (pawn.rank - 1)));
        if(distance >= pawn.rank)
        {
            if(userToMove)
            {
                return -1100000 + layer + 2 * distance;
            }
            else if(distance > pawn.rank)
            {
                return -1100000 + layer + 2 * distance;
            }
        }
    }
    else
    {
        uint8_t distance = std::max(abs(userKing.file - pawn.file), abs(userKing.rank - pawn.rank));
        if(distance > pawn.rank)
        {
            if(userToMove)
            {
                return 1100000 - layer - 2 * distance;
            }
            else if(distance - 1 > pawn.rank)
            {
                return 1100000 - layer - 2 * distance;
            }
        }
    }
    int eval;
    if(pawn.rank < 4)
    {
        eval = -879;
    }
    else
    {
        if(pawn.rank < 6)
        {
            if(pawn.rank == 4)
                eval = -1146;
            else
                eval = -1268;
        }
        else
        {
            if(pawn.rank == 6)
                eval = -1674;
            else
                eval = -1707;
        }
    }
    if(abs(userKing.file - pawn.file) < 2)
    {
        if(userKing.rank + 2 == pawn.rank)
        {
            eval += 500;
        }
        else if(pawn.rank < 4)
        {
            if(userKing.rank + 1 == pawn.rank)
            {
                eval += 500;
            }
            else if(pawn.rank == 6 && userKing.rank == 6)
            {
                eval += 500;
            }
        }
    }
    if(abs(cpuKing.file - pawn.file) < 2)
    {
        if(cpuKing.rank + 2 == pawn.rank)
        {
            eval -= 500;
        }
        else if(pawn.rank < 4)
        {
            if(cpuKing.rank + 1 == pawn.rank)
            {
                eval -= 500;
            }
            else if(pawn.rank == 6 && cpuKing.rank == 6)
            {
                eval -= 500;
            }
        }
    }
    eval -= pawn.rank + userKing.rank;
    return eval;
}

int Evaluator::pawnEndgameEval(int layer)
{
    if(board->bitmaps[CPU_PAWN + 6] == 0)
    {
        return kVkp(layer);
    }
    return kpVk(layer);
}

int Evaluator::rookEndgameEval(int layer)
{
    
    int cpuX = evaluatorTracker.cpuKing/8;
    int cpuY = evaluatorTracker.cpuKing%8;
    int humanX = evaluatorTracker.humanKing/8;
    int humanY = evaluatorTracker.humanKing%8;
    if(evaluatorTracker.cpuRooks > evaluatorTracker.humanRooks)
    {
        // cpu has rook
        int net = 1400000;
        int rookX = -1;
        int rookY = -1;
        for(int i=0; i<64; i++)
        {
            if(board->b[i] == 4)
            {
                rookX = i/8;
                rookY = i%8;
            }
        }
        if(rookX == -1)
        {
            // error
            
            return 0;
        }
        
        int kingDistances = fmax(abs(cpuX-humanX), abs(cpuY-humanY));
        int rookCpuDistance = fmax(abs(rookX-cpuX), abs(rookY-cpuY));
        int rookHumanDistance = fmax(abs(rookX-humanX), abs(rookY-humanY));
        int kingCenterDistance = abs(humanX-3.5) + abs(humanY-3.5);
        if(layer == 1)
        {
            // human to move - check if human can capture rook
            if(rookCpuDistance != 1)
            {
                // rook not defended by king
                if(rookHumanDistance == 1)
                {
                    // rook attacked by king
                    // therefore: draw
                    return 0;
                }
            }
        }
        
        net -= fmax(2,kingDistances);
        net += 3*kingCenterDistance;
        return net;
    }
    else
    {
        // human has rook
        int net = -1400000;
        int rookX = -1;
        int rookY = -1;
        for(int i=0; i<64; i++)
        {
            if(board->b[i] == -4)
            {
                rookX = i/8;
                rookY = i%8;
            }
        }
        if(rookX == -1)
        {
            // error
            
            return 0;
        }
        
        int kingDistances = fmax(abs(cpuX-humanX), abs(cpuY-humanY));
        int rookCpuDistance = fmax(abs(rookX-cpuX), abs(rookY-cpuY));
        int rookHumanDistance = fmax(abs(rookX-humanX), abs(rookY-humanY));
        int kingCenterDistance = abs(humanX-3.5) + abs(humanY-3.5);
        
        if(layer == 0)
        {
            // cpu to move - check if cpu can capture rook
            if(rookHumanDistance != 1)
            {
                // rook not defended by king
                if(rookCpuDistance == 1)
                {
                    // rook attacked by king
                    // therefore: draw
                    return 0;
                }
            }
        }
        net += fmax(2,kingDistances);
        net -= 3*kingCenterDistance;
        return net;
    }
}

int Evaluator::queenEndgameEval(int layer)
{
    int cpuX = evaluatorTracker.cpuKing/8;
    int cpuY = evaluatorTracker.cpuKing%8;
    int humanX = evaluatorTracker.humanKing/8;
    int humanY = evaluatorTracker.humanKing%8;
    if(evaluatorTracker.cpuQueens > evaluatorTracker.humanQueens)
    {
        // cpu has queen
        int net = 1500000;
        int queenX = -1;
        int queenY = -1;
        for(int i=0; i<64; i++)
        {
            if(board->b[i] == 5)
            {
                queenX = i/8;
                queenY = i%8;
            }
        }
        if(queenX == -1)
        {
            // error
            
            return 0;
        }
        
        int kingDistances = fmax(abs(cpuX-humanX), abs(cpuY-humanY));
        int queenCpuDistance = fmax(abs(queenX-cpuX), abs(queenY-cpuY));
        int queenHumanDistance = fmax(abs(queenX-humanX), abs(queenY-humanY));
        int kingCenterDistance = abs(humanX-3.5) + abs(humanY-3.5);
        if(layer == 1)
        {
            // human to move - check if human can capture queen
            if(queenCpuDistance != 1)
            {
                // queen not defended by king
                if(queenHumanDistance == 1)
                {
                    // queen attacked by king
                    // therefore: draw
                    return 0;
                }
            }
        }
        
        net -= fmax(2,kingDistances);
        net += 3*kingCenterDistance;
        return net;
    }
    else
    {
        // human has queen
        int net = -1500000;
        int queenX = -1;
        int queenY = -1;
        for(int i=0; i<64; i++)
        {
            if(board->b[i] == -5)
            {
                queenX = i/8;
                queenY = i%8;
            }
        }
        if(queenX == -1)
        {
            // error
            
            return 0;
        }
        
        int kingDistances = fmax(abs(cpuX-humanX), abs(cpuY-humanY));
        int queenCpuDistance = fmax(abs(queenX-cpuX), abs(queenY-cpuY));
        int queenHumanDistance = fmax(abs(queenX-humanX), abs(queenY-humanY));
        int kingCenterDistance = abs(humanX-3.5) + abs(humanY-3.5);
        
        if(layer == 0)
        {
            // cpu to move - check if cpu can capture queen
            if(queenHumanDistance != 1)
            {
                // queen not defended by king
                if(queenCpuDistance == 1)
                {
                    // queen attacked by king
                    // therefore: draw
                    return 0;
                }
            }
        }
        net += fmax(2,kingDistances);
        net -= 3*kingCenterDistance;
        return net;
    }
}

int Evaluator::threeManEval(int layer) {
    if(evaluatorTracker.humanPawns + evaluatorTracker.cpuPawns != 0)
    {
        return pawnEndgameEval(layer);
    }
    if(evaluatorTracker.cpuRooks + evaluatorTracker.humanRooks != 0)
    {
        return rookEndgameEval(layer);
    }
    if(evaluatorTracker.cpuQueens + evaluatorTracker.humanQueens != 0) {
        return queenEndgameEval(layer);
    }
    
    // tie if it is a bishop or knight endgame, but preserve material advantage
    return evaluatorTracker.cpuKnights+evaluatorTracker.cpuBishops-evaluatorTracker.humanKnights-evaluatorTracker.humanBishops;
}

int Evaluator::fourManEval(int layer)
{
    if(evaluatorTracker.cpuBishops == 2 || evaluatorTracker.humanBishops == 2)
    {
        return BBendgameEval(layer);
    }
    else if(evaluatorTracker.cpuBishops == 1 && evaluatorTracker.humanBishops == 1)
    {
        return evaluatorTracker.cpuBishops - evaluatorTracker.humanBishops;
    }
    return specialEvalFailed;
}

int Evaluator::BBendgameEval(int layer)
{
    if(evaluatorTracker.cpuBishops == 2)
    {
        // cpu has two bishops
        int bishopA = -1;
        int bishopB = -1;
        for(int i=0; i<64; i++)
        {
            if(board->b[i] == 3)
            {
                bishopB = bishopA;
                bishopA = i;
            }
        }
        if(bishopB == -1)
        {
            // error
            
            return specialEvalFailed;
        }
        if((bishopA/8 + bishopA%8)%2 == (bishopB/8 + bishopB%8)%2)
        {
            // both bishops are of the same color - preserve material advantage
            return evaluatorTracker.cpuBishops - evaluatorTracker.humanBishops;
        }
        
        int net = 1300000;
        
        int kingDist = fmax(evaluatorTracker.cpuKing/8 - evaluatorTracker.humanKing/8, evaluatorTracker.cpuKing%8 - evaluatorTracker.humanKing%8);
        int kingSideDistA = abs(evaluatorTracker.humanKing/8 - 3.5);
        int kingSideDistB = abs(evaluatorTracker.humanKing%8 - 3.5);
        if(kingSideDistA > kingSideDistB)
        {
            // closer to sideB
            net -= 10*kingSideDistB + kingSideDistA;
        }
        else
        {
            // closer to sideA
            net -= 10*kingSideDistA + kingSideDistB;
        }
        return net - 5*fmax(2, kingDist);
    }
    else
    {
        // human has two bishops
        int bishopA = -1;
        int bishopB = -1;
        for(int i=0; i<64; i++)
        {
            if(board->b[i] == -3)
            {
                bishopB = bishopA;
                bishopA = i;
            }
        }
        if(bishopB == -1)
        {
            // error
            
            return -1*specialEvalFailed;
        }
        if((bishopA/8 + bishopA%8)%2 == (bishopB/8 + bishopB%8)%2)
        {
            // both bishops are of the same color
            return evaluatorTracker.cpuBishops - evaluatorTracker.humanBishops;
        }
        
        int net = -1300000;
        
        int kingDist = fmax(evaluatorTracker.cpuKing/8 - evaluatorTracker.humanKing/8, evaluatorTracker.cpuKing%8 - evaluatorTracker.humanKing%8);
        int kingSideDistA = abs(evaluatorTracker.cpuKing/8 - 3.5);
        int kingSideDistB = abs(evaluatorTracker.cpuKing%8 - 3.5);
        if(kingSideDistA > kingSideDistB)
        {
            // closer to sideB
            net += 10*kingSideDistB + kingSideDistA;
        }
        else
        {
            // closer to sideA
            net += 10*kingSideDistA + kingSideDistB;
        }
        return net + 5*fmax(2, kingDist);
    }
    return specialEvalFailed;
}

// -2 mill - -1 mill = human victor
// -1 mill - 1 mill  = normal eval  (0 = draw)
// 1 mill - 2 mill   = cpu victory

int Evaluator::eval(int layer, int endOfLastMove)
{
    if(evaluatorTracker.cpuKing == -1 && evaluatorTracker.humanKing == -1)
    {
        // no kings
        return 0;
    }
    else if(evaluatorTracker.cpuKing == -1)
    {
        // no cpu king
        return -10000000;
    }
    else if(evaluatorTracker.humanKing == -1)
    {
        // no human king
        return 10000000;
    }
    
    evaluatorTracker.set(board);
    
    numberOfEvals++;
    
    float stage = getPieceCount() + evaluatorTracker.cpuQueens + evaluatorTracker.humanQueens;      // between 2 and 18
    stage = fminf(stage, 14);
    stage = fmaxf(stage, 4);
    stage = (stage-4)/10;
    // stage = 0.0, 0.1, 0.2, ..., 1.0
    
    if(stage == 0) {
        // 4 or fewer pieces
        int manCount = getManCount();
        if(manCount == 2)
        {
            return 0;
        }
        else if(manCount == 3)
        {
            return threeManEval(layer);
        }
        else if(manCount == 4)
        {
            int value = fourManEval(layer);
            if(value != specialEvalFailed)
            {
                return value;
            }
            else
            {
                // use normal eval()
            }
        }
        else
        {
            // use normal eval()
        }
    }
    
    
    
    // set value of board to 0
    int net = 0;
    
    // 1. material
    net += (evaluatorTracker.cpuPawns - evaluatorTracker.humanPawns) * (openingPawnValue * stage + endingPawnValue * (1-stage));
    net += (evaluatorTracker.cpuKnights - evaluatorTracker.humanKnights) * (openingKnightValue * stage + endingKnightValue * (1-stage));
    net += (evaluatorTracker.cpuBishops - evaluatorTracker.humanBishops) * (openingBishopValue * stage + endingBishopValue * (1-stage));
    net += (evaluatorTracker.cpuRooks - evaluatorTracker.humanRooks) * (openingRookValue * stage + endingRookValue * (1-stage));
    net += (evaluatorTracker.cpuQueens - evaluatorTracker.humanQueens) * (openingQueenValue * stage + endingQueenValue * (1-stage));
    
    net += cpuKingValue[evaluatorTracker.cpuKing] * stage + endGameKingValue[evaluatorTracker.cpuKing] * (1-stage);
    net -= humanKingValue[evaluatorTracker.humanKing] * stage + endGameKingValue[evaluatorTracker.humanKing] * (1-stage);

    // 2. Pawn Structure
    uint64_t hash = 1369 * stage + board->bitmaps[5] * 37 + board->bitmaps[7];
    std::unordered_map<uint64_t, int>::iterator it = pawnStructureMap.find(hash);
    if(it != pawnStructureMap.end())
    {
        net += it->second;
    }
    else
    {
        net += pawnStructureEval(stage);
    }
    
    // 3. Pieces
    if(evaluatorTracker.cpuBishops >= 2)
    {
        net += openingBishopPairBonus * stage + endingBishopPairBonus * (1-stage);
    }
    if(evaluatorTracker.humanBishops >= 2)
    {
        net -= openingBishopPairBonus * stage + endingBishopPairBonus * (1-stage);
    }
    
    if(evaluatorTracker.cpuRooks >= 2)
    {
        net += openingRookPairBonus * stage + endingRookPairBonus * (1-stage);
    }
    if(evaluatorTracker.humanRooks >= 2)
    {
        net -= openingRookPairBonus * stage + endingRookPairBonus * (1-stage);
    }
    
    // set x-ray vision for pressure and hangingPiece determination
    memset(spaceControl[layer],0,sizeof(spaceControl[layer]));
    
    exchangeValue[0] = openingExchangeValue[0] * stage + endingExchangeValue[0] * (1-stage);
    exchangeValue[1] = openingExchangeValue[1] * stage + endingExchangeValue[1] * (1-stage);
    exchangeValue[2] = openingExchangeValue[2] * stage + endingExchangeValue[2] * (1-stage);
    exchangeValue[3] = openingExchangeValue[3] * stage + endingExchangeValue[3] * (1-stage);
    exchangeValue[4] = openingExchangeValue[4] * stage + endingExchangeValue[4] * (1-stage);
    
    net += (openingMoveBonusMultiplier * stage + endingMoveBonusMultiplier * (1-stage)) * aiEvalMove(endOfLastMove, layer);
    net -= (openingMoveBonusMultiplier * stage + endingMoveBonusMultiplier * (1-stage)) * humanEvalMove(endOfLastMove, layer);
    
    if(pieceSafe(endOfLastMove, layer)==0)
    {
        if((*board).b[endOfLastMove]>0)
        {
            // cpu
            net += (openingMoveBonusMultiplier * stage + endingMoveBonusMultiplier * (1-stage)) * aiEvalMoveOneSquare(endOfLastMove, layer);
        }
        else
        {
            // human
            net -= (openingMoveBonusMultiplier * stage + endingMoveBonusMultiplier * (1-stage)) * humanEvalMoveOneSquare(endOfLastMove, layer);
        }
    }
    
    sf::Vector2i result = inDepthTactics(stage, layer);
    
    net += result.x;
    net -= result.y;
    
    result = kingSafety(evaluatorTracker.cpuKing, evaluatorTracker.humanKing, stage, layer);
    
    net += stage * result.x;
    net -= stage * result.y;
    
    if(layer%2 == 0)
    {
        net += openingInitiativeBonus * stage + endingInitiativeBonus * (1-stage);
    }
    else
    {
        net -= openingInitiativeBonus * stage + endingInitiativeBonus * (1-stage);
    }
    
    // contempt factor
    net += contemptFactorSlope * (stage-0.5);
    
    return net;
}

sf::Vector2i Evaluator::inDepthTactics(float stage, int layer)
{
    int sto;
    float stoF;
    int cpu = 0;
    int human = 0;
    // in-depth tactics & pressure
    hangingCpuPieceLength = 0;
    hangingHumanPieceLength = 0;
    for(int i = 0; i < 64; i++)
    {
        if((*board).b[i]>0)
        {
            // cpu piece
            stoF = pieceSafe(i, layer);
            if(stoF==0)
            {
                // piece safe
            }
            else
            {
                // piece in danger
                hangingCpuPiece[hangingCpuPieceLength] = stoF;
                hangingCpuPieceLength++;
            }
        }
        else if((*board).b[i]<0)
        {
            // human piece
            stoF = pieceSafe(i, layer);
            if(stoF==0)
            {
                // piece safe
            }
            else
            {
                // piece in danger
                hangingHumanPiece[hangingHumanPieceLength] = stoF;
                hangingHumanPieceLength++;
            }
        }
    }
    
    
    for(int i = 0; i < hangingCpuPieceLength; i++)
    {
        for(int j=1; j<hangingCpuPieceLength; j++)
        {
            if(hangingCpuPiece[j] < hangingCpuPiece[j-1])
            {
                sto = hangingCpuPiece[j];
                hangingCpuPiece[j] = hangingCpuPiece[j-1];
                hangingCpuPiece[j-1] = sto;
            }
        }
    }
    for(int i = 0; i < hangingHumanPieceLength; i++)
    {
        for(int j=1; j < hangingHumanPieceLength; j++)
        {
            if(hangingHumanPiece[j] < hangingHumanPiece[j-1])
            {
                sto = hangingHumanPiece[j];
                hangingHumanPiece[j] = hangingHumanPiece[j-1];
                hangingHumanPiece[j-1] = sto;
            }
        }
    }
    
    if(layer%2 == 0 && hangingCpuPieceLength == 1 && hangingHumanPieceLength == 0)
    {
        return sf::Vector2i(hangingToCheck(hangingCpuPiece[0])+cpu, human);
    }
    else if(layer%2 == 1 && hangingHumanPieceLength == 1 && hangingCpuPieceLength == 0)
    {
        return sf::Vector2i(cpu, hangingToCheck(hangingHumanPiece[0])+human);
    }
    
    return simulateExchanges(layer%2, cpu, human, 0, 0);
}

sf::Vector2i Evaluator::simulateExchanges(int turn, int cpu, int human, int cpuIndex, int humanIndex)
{
    if(cpuIndex == hangingCpuPieceLength && humanIndex == hangingHumanPieceLength)
    {
        // done
        return sf::Vector2i(cpu,human);
    }
    else if(cpuIndex == hangingCpuPieceLength)
    {
        if(turn == 0)
        {
            // capture human piece
            human += hangingHumanPiece[humanIndex];
            humanIndex++;
        }
        else if(turn == 1)
        {
            // save human piece
            humanIndex++;
        }
    }
    else if(humanIndex == hangingHumanPieceLength)
    {
        if(turn == 0)
        {
            // save cpu piece
            cpuIndex++;
        }
        else if(turn == 1)
        {
            // capture cpu piece
            cpu += hangingCpuPiece[cpuIndex];
            cpuIndex++;
        }
    }
    else
    {
        if(turn == 0)
        {
            if(hangingCpuPiece[cpuIndex] < hangingHumanPiece[humanIndex])
            {
                // save cpu piece
                cpuIndex++;
            }
            else
            {
                // capture human piece
                human += hangingHumanPiece[humanIndex];
                humanIndex++;
            }
        }
        else if(turn == 1)
        {
            if(hangingHumanPiece[humanIndex] < hangingCpuPiece[cpuIndex])
            {
                // save human piece
                humanIndex++;
            }
            else
            {
                // capture cpu piece
                cpu += hangingCpuPiece[cpuIndex];
                cpuIndex++;
            }
        }
    }
    
    return simulateExchanges(1-turn, cpu, human, cpuIndex, humanIndex);
}

sf::Vector2i Evaluator::kingSafety(int cpuKing, int humanKing, float stage, int layer)
{
    // king safety
    // castling (cpu-queen: 0; cpu-king: 1; human-queen: 2; human-king:3)
    float cpu = 0;
    float human = 0;
    float stoF = 0;
    float stoFB = 0;
    int i,j;
    if((*board).castling[0]==1 && (*board).castling[1]==1)
    {
        if((*board).b[6]==1)
        {
            stoF += queenPawnsBackBeforeCastlingBonus;
        }
        else if((*board).b[5]==1)
        {
            stoF += queenPawnsSecondBackBeforeCastlingBonus;
        }
        
        if((*board).b[14]==1)
        {
            stoF += queenPawnsBackBeforeCastlingBonus;
        }
        else if((*board).b[13]==1)
        {
            stoF += queenPawnsSecondBackBeforeCastlingBonus;
        }
        
        if((*board).b[22]==1)
        {
            stoF += queenPawnsBackBeforeCastlingBonus;
        }
        else if((*board).b[21]==1)
        {
            stoF += queenPawnsSecondBackBeforeCastlingBonus;
        }
        
        if((*board).b[15]==0)
        {
            stoF += queenEmptyBeforeCastling;
        }
        if((*board).b[23]==0)
        {
            stoF += queenEmptyBeforeCastling;
        }
        if((*board).b[31]==0)
        {
            stoF += queenEmptyBeforeCastling;
        }
        
        if((*board).b[46]==1)
        {
            stoFB += kingPawnsBackBeforeCastlingBonus;
        }
        else if((*board).b[45]==1)
        {
            stoFB += kingPawnsSecondBackBeforeCastlingBonus;
        }
        
        if((*board).b[54]==1)
        {
            stoFB += kingPawnsBackBeforeCastlingBonus;
        }
        else if((*board).b[53]==1)
        {
            stoFB += kingPawnsSecondBackBeforeCastlingBonus;
        }
        
        if((*board).b[62]==1)
        {
            stoFB += kingPawnsBackBeforeCastlingBonus;
        }
        else if((*board).b[61]==1)
        {
            stoFB += kingPawnsSecondBackBeforeCastlingBonus;
        }
        
        if((*board).b[47]==0)
        {
            stoFB += kingEmptyBeforeCastling;
        }
        if((*board).b[55]==0)
        {
            stoFB += kingEmptyBeforeCastling;
        }
        
        if(stoF<stoFB)
        {
            stoF /= 5;
            stoF += stoFB;
        }
        else
        {
            stoF += stoFB/5;
        }
    }
    else if((*board).castling[0]==1)
    {
        if((*board).b[6]==1)
        {
            stoF += queenPawnsBackBeforeCastlingBonus;
        }
        else if((*board).b[5]==1)
        {
            stoF += queenPawnsSecondBackBeforeCastlingBonus;
        }
        
        if((*board).b[14]==1)
        {
            stoF += queenPawnsBackBeforeCastlingBonus;
        }
        else if((*board).b[13]==1)
        {
            stoF += queenPawnsSecondBackBeforeCastlingBonus;
        }
        
        if((*board).b[22]==1)
        {
            stoF += queenPawnsBackBeforeCastlingBonus;
        }
        else if((*board).b[21]==1)
        {
            stoF += queenPawnsSecondBackBeforeCastlingBonus;
        }
        
        if((*board).b[15]==0)
        {
            stoF += queenEmptyBeforeCastling;
        }
        if((*board).b[23]==0)
        {
            stoF += queenEmptyBeforeCastling;
        }
        if((*board).b[31]==0)
        {
            stoF += queenEmptyBeforeCastling;
        }
    }
    else if((*board).castling[1]==1)
    {
        if((*board).b[46]==1)
        {
            stoF += kingPawnsBackBeforeCastlingBonus;
        }
        else if((*board).b[45]==1)
        {
            stoF += kingPawnsSecondBackBeforeCastlingBonus;
        }
        
        if((*board).b[54]==1)
        {
            stoF += kingPawnsBackBeforeCastlingBonus;
        }
        else if((*board).b[53]==1)
        {
            stoF += kingPawnsSecondBackBeforeCastlingBonus;
        }
        
        if((*board).b[62]==1)
        {
            stoF += kingPawnsBackBeforeCastlingBonus;
        }
        else if((*board).b[61]==1)
        {
            stoF += kingPawnsSecondBackBeforeCastlingBonus;
        }
        
        if((*board).b[47]==0)
        {
            stoF += kingEmptyBeforeCastling;
        }
        if((*board).b[55]==0)
        {
            stoF += kingEmptyBeforeCastling;
        }
    }
    else if(cpuKing/8<3 || cpuKing/8>5)
    {
        if(cpuKing%8>0)
        {
            if(cpuKing/8!=0)
            {
                if((*board).b[cpuKing-9]==1)
                {
                    stoF += pawnsBackRankAfterCastling;
                }
                else if((*board).b[cpuKing-10]==1)
                {
                    stoF += pawnsSecondBackRankAfterCastling;
                }
            }
            if(cpuKing/8!=7)
            {
                if((*board).b[cpuKing+7]==1)
                {
                    stoF += pawnsBackRankAfterCastling;
                }
                else if((*board).b[cpuKing+6]==1)
                {
                    stoF += pawnsSecondBackRankAfterCastling;
                }
            }
            if((*board).b[cpuKing-1]==1)
            {
                stoF += pawnsBackRankAfterCastling;
            }
            else if((*board).b[cpuKing-2]==1)
            {
                stoF += pawnsSecondBackRankAfterCastling;
            }
        }
    }
    
    cpu += stoF*stage;
    stoF = 0;
    stoFB = 0;
    
    if((*board).castling[2]==1 && (*board).castling[3]==1)
    {
        if((*board).b[1]==-1)
        {
            stoF += queenPawnsBackBeforeCastlingBonus;
        }
        else if((*board).b[2]==-1)
        {
            stoF += queenPawnsSecondBackBeforeCastlingBonus;
        }
        if((*board).b[9]==-1)
        {
            stoF += queenPawnsBackBeforeCastlingBonus;
        }
        else if((*board).b[10]==-1)
        {
            stoF += queenPawnsSecondBackBeforeCastlingBonus;
        }
        if((*board).b[17]==-1)
        {
            stoF += queenPawnsBackBeforeCastlingBonus;
        }
        else if((*board).b[18]==-1)
        {
            stoF += queenPawnsSecondBackBeforeCastlingBonus;
        }
        if((*board).b[8]==0)
        {
            stoF += queenEmptyBeforeCastling;
        }
        if((*board).b[16]==0)
        {
            stoF += queenEmptyBeforeCastling;
        }
        if((*board).b[24]==0)
        {
            stoF += queenEmptyBeforeCastling;
        }
        if((*board).b[41]==-1)
        {
            stoFB += kingPawnsBackBeforeCastlingBonus;
        }
        else if((*board).b[42]==-1)
        {
            stoF += kingPawnsSecondBackBeforeCastlingBonus;
        }
        if((*board).b[49]==-1)
        {
            stoFB += kingPawnsBackBeforeCastlingBonus;
        }
        else if((*board).b[50]==-1)
        {
            stoF += kingPawnsSecondBackBeforeCastlingBonus;
        }
        if((*board).b[57]==-1)
        {
            stoFB += kingPawnsBackBeforeCastlingBonus;
        }
        else if((*board).b[58]==-1)
        {
            stoF += kingPawnsSecondBackBeforeCastlingBonus;
        }
        if((*board).b[40]==0)
        {
            stoFB += kingEmptyBeforeCastling;
        }
        if((*board).b[48]==0)
        {
            stoFB += kingEmptyBeforeCastling;
        }
        if(stoF<stoFB)
        {
            stoF /= 5;
            stoF += stoFB;
        }
        else
        {
            stoF += stoFB/5;
        }
    }
    else if((*board).castling[2]==1)
    {
        if((*board).b[1]==-1)
        {
            stoF += queenPawnsBackBeforeCastlingBonus;
        }
        else if((*board).b[2]==-1)
        {
            stoF += queenPawnsSecondBackBeforeCastlingBonus;
        }
        if((*board).b[9]==-1)
        {
            stoF += queenPawnsBackBeforeCastlingBonus;
        }
        else if((*board).b[10]==-1)
        {
            stoF += queenPawnsSecondBackBeforeCastlingBonus;
        }
        if((*board).b[17]==-1)
        {
            stoF += queenPawnsBackBeforeCastlingBonus;
        }
        else if((*board).b[18]==-1)
        {
            stoF += queenPawnsSecondBackBeforeCastlingBonus;
        }
        if((*board).b[8]==0)
        {
            stoF += queenEmptyBeforeCastling;
        }
        if((*board).b[16]==0)
        {
            stoF += queenEmptyBeforeCastling;
        }
        if((*board).b[24]==0)
        {
            stoF += queenEmptyBeforeCastling;
        }
    }
    else if((*board).castling[3]==1)
    {
        if((*board).b[41]==-1)
        {
            stoF += kingPawnsBackBeforeCastlingBonus;
        }
        else if((*board).b[42]==-1)
        {
            stoF += kingPawnsSecondBackBeforeCastlingBonus;
        }
        if((*board).b[49]==-1)
        {
            stoF += kingPawnsBackBeforeCastlingBonus;
        }
        else if((*board).b[50]==-1)
        {
            stoF += kingPawnsSecondBackBeforeCastlingBonus;
        }
        if((*board).b[57]==-1)
        {
            stoF += kingPawnsBackBeforeCastlingBonus;
        }
        else if((*board).b[58]==-1)
        {
            stoF += kingPawnsSecondBackBeforeCastlingBonus;
        }
        if((*board).b[40]==0)
        {
            stoF += kingEmptyBeforeCastling;
        }
        if((*board).b[48]==0)
        {
            stoF += kingEmptyBeforeCastling;
        }
    }
    else if(humanKing/8<3 || humanKing/8>5)
    {
        if(humanKing%8<6)
        {
            if(humanKing/8!=0)
            {
                if((*board).b[humanKing-7]==-1)
                {
                    stoF += pawnsBackRankAfterCastling;
                }
                else if((*board).b[humanKing-6]==-1)
                {
                    stoF += pawnsSecondBackRankAfterCastling;
                }
            }
            if(humanKing/8!=7)
            {
                if((*board).b[humanKing+9]==-1)
                {
                    stoF += pawnsBackRankAfterCastling;
                }
                else if((*board).b[humanKing+10]==-1)
                {
                    stoF += pawnsSecondBackRankAfterCastling;
                }
            }
            if((*board).b[humanKing+1]==-1)
            {
                stoF += pawnsBackRankAfterCastling;
            }
            else if((*board).b[humanKing+2]==-1)
            {
                stoF += pawnsSecondBackRankAfterCastling;
            }
        }
    }
    
    human += stoF*stage;
    
    // cpu king safety from pressure
    if(cpuKing/8>0)
    {
        i = cpuKing-8;
        stoF = 0;
        for(j=5;j<10;j++)
        {
            stoF += spaceControl[layer][i][j];
        }
        for(j=0;j<5;j++)
        {
            stoF -= spaceControl[layer][i][j];
        }
        if(stoF>=0)
        {
            cpu += closeToKingSafetyBonus*sqrt(stoF);
        }
        else
        {
            cpu += closeToKingDangerBonus*sqrt(abs(stoF));
        }
        if(cpuKing%8>0)
        {
            i = cpuKing-9;
            stoF = 0;
            for(j=5;j<10;j++)
            {
                stoF += spaceControl[layer][i][j];
            }
            for(j=0;j<5;j++)
            {
                stoF -= spaceControl[layer][i][j];
            }
            if(stoF>=0)
            {
                cpu += closeToKingSafetyBonus*sqrt(stoF);
            }
            else
            {
                cpu += closeToKingDangerBonus*sqrt(abs(stoF));
            }
        }
        if(cpuKing%8<7)
        {
            i = cpuKing-7;
            stoF = 0;
            for(j=5;j<10;j++)
            {
                stoF += spaceControl[layer][i][j];
            }
            for(j=0;j<5;j++)
            {
                stoF -= spaceControl[layer][i][j];
            }
            if(stoF>=0)
            {
                cpu += closeToKingSafetyBonus*sqrt(stoF);
            }
            else
            {
                cpu += closeToKingDangerBonus*sqrt(abs(stoF));
            }
        }
    }
    if(cpuKing/8<7)
    {
        i = cpuKing + 8;
        stoF = 0;
        for(j=5;j<10;j++)
        {
            stoF += spaceControl[layer][i][j];
        }
        for(j=0;j<5;j++)
        {
            stoF -= spaceControl[layer][i][j];
        }
        if(stoF>=0)
        {
            cpu += closeToKingSafetyBonus*sqrt(stoF);
        }
        else
        {
            cpu -= closeToKingDangerBonus*sqrt(abs(stoF));
        }
        if(cpuKing%8>0)
        {
            i = cpuKing+7;
            stoF = 0;
            for(j=5;j<10;j++)
            {
                stoF += spaceControl[layer][i][j];
            }
            for(j=0;j<5;j++)
            {
                stoF -= spaceControl[layer][i][j];
            }
            if(stoF>=0)
            {
                cpu += closeToKingSafetyBonus*sqrt(stoF);
            }
            else
            {
                cpu -= closeToKingDangerBonus*sqrt(abs(stoF));
            }
        }
        if(cpuKing%8<7)
        {
            i = cpuKing+9;
            stoF = 0;
            for(j=5;j<10;j++)
            {
                stoF += spaceControl[layer][i][j];
            }
            for(j=0;j<5;j++)
            {
                stoF -= spaceControl[layer][i][j];
            }
            if(stoF>=0)
            {
                cpu += closeToKingSafetyBonus*sqrt(stoF);
            }
            else
            {
                cpu -= closeToKingDangerBonus*sqrt(abs(stoF));
            }
        }
    }
    if(cpuKing%8>0)
    {
        i = cpuKing - 1;
        stoF = 0;
        for(j=5;j<10;j++)
        {
            stoF += spaceControl[layer][i][j];
        }
        for(j=0;j<5;j++)
        {
            stoF -= spaceControl[layer][i][j];
        }
        if(stoF>=0)
        {
            cpu += closeToKingSafetyBonus*sqrt(stoF);
        }
        else
        {
            cpu += closeToKingDangerBonus*sqrt(abs(stoF));
        }
    }
    if(cpuKing%8<7)
    {
        i = cpuKing + 1;
        stoF = 0;
        for(j=5;j<10;j++)
        {
            stoF += spaceControl[layer][i][j];
        }
        for(j=0;j<5;j++)
        {
            stoF -= spaceControl[layer][i][j];
        }
        if(stoF>=0)
        {
            cpu += closeToKingSafetyBonus*sqrt(stoF);
        }
        else
        {
            cpu += closeToKingDangerBonus*sqrt(abs(stoF));
        }
    }
    
    // human king safety from pressure
    if(humanKing/8>0)
    {
        i = humanKing-8;
        stoF = 0;
        for(j=5;j<10;j++)
        {
            stoF -= spaceControl[layer][i][j];
        }
        for(j=0;j<5;j++)
        {
            stoF += spaceControl[layer][i][j];
        }
        if(stoF>=0)
        {
            human += closeToKingSafetyBonus*sqrt(stoF);
        }
        else
        {
            human += closeToKingDangerBonus*sqrt(abs(stoF));
        }
        if(humanKing%8>0)
        {
            i = humanKing-9;
            stoF = 0;
            for(j=5;j<10;j++)
            {
                stoF -= spaceControl[layer][i][j];
            }
            for(j=0;j<5;j++)
            {
                stoF += spaceControl[layer][i][j];
            }
            if(stoF>=0)
            {
                human += closeToKingSafetyBonus*sqrt(stoF);
            }
            else
            {
                human += closeToKingDangerBonus*sqrt(abs(stoF));
            }
        }
        if(humanKing%8<7)
        {
            i = humanKing-7;
            stoF = 0;
            for(j=5;j<10;j++)
            {
                stoF -= spaceControl[layer][i][j];
            }
            for(j=0;j<5;j++)
            {
                stoF += spaceControl[layer][i][j];
            }
            if(stoF>=0)
            {
                human += closeToKingSafetyBonus*sqrt(stoF);
            }
            else
            {
                human += closeToKingDangerBonus*sqrt(abs(stoF));
            }
        }
    }
    if(humanKing/8<7)
    {
        i = humanKing + 8;
        stoF = 0;
        for(j=5;j<10;j++)
        {
            stoF -= spaceControl[layer][i][j];
        }
        for(j=0;j<5;j++)
        {
            stoF += spaceControl[layer][i][j];
        }
        if(stoF>=0)
        {
            human += closeToKingSafetyBonus*sqrt(stoF);
        }
        else
        {
            human -= closeToKingDangerBonus*sqrt(abs(stoF));
        }
        if(humanKing%8>0)
        {
            i = humanKing+7;
            stoF = 0;
            for(j=5;j<10;j++)
            {
                stoF -= spaceControl[layer][i][j];
            }
            for(j=0;j<5;j++)
            {
                stoF += spaceControl[layer][i][j];
            }
            if(stoF>=0)
            {
                human += closeToKingSafetyBonus*sqrt(stoF);
            }
            else
            {
                human -= closeToKingDangerBonus*sqrt(abs(stoF));
            }
        }
        if(humanKing%8<7)
        {
            i = humanKing+9;
            stoF = 0;
            for(j=5;j<10;j++)
            {
                stoF -= spaceControl[layer][i][j];
            }
            for(j=0;j<5;j++)
            {
                stoF += spaceControl[layer][i][j];
            }
            if(stoF>=0)
            {
                human += closeToKingSafetyBonus*sqrt(stoF);
            }
            else
            {
                human -= closeToKingDangerBonus*sqrt(abs(stoF));
            }
        }
    }
    if(humanKing%8>0)
    {
        i = humanKing-1;
        stoF = 0;
        for(j=5;j<10;j++)
        {
            stoF -= spaceControl[layer][i][j];
        }
        for(j=0;j<5;j++)
        {
            stoF += spaceControl[layer][i][j];
        }
        if(stoF>=0)
        {
            human += closeToKingSafetyBonus*sqrt(stoF);
        }
        else
        {
            human += closeToKingDangerBonus*sqrt(abs(stoF));
        }
    }
    if(humanKing%8<7)
    {
        i = humanKing + 1;
        stoF = 0;
        for(j=5;j<10;j++)
        {
            stoF -= spaceControl[layer][i][j];
        }
        for(j=0;j<5;j++)
        {
            stoF += spaceControl[layer][i][j];
        }
        if(stoF>=0)
        {
            human += closeToKingSafetyBonus*sqrt(stoF);
        }
        else
        {
            human += closeToKingDangerBonus*sqrt(abs(stoF));
        }
    }
    
    return sf::Vector2i(cpu, human);
}

int Evaluator::signum(int x)
{
    if(x>0)
    {
        return 1;
    }
    else if(x<0)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

void Evaluator::start(Board* board)
{
    evaluatorTracker.set(board);
    pawnStructureMap.clear();
}

int Evaluator::makeMove(int from, int to, int layer, Board* board)
{
    return evaluatorTracker.makeMove(from, to, board);
}

void Evaluator::undoMove(int from, int to, int layer, Board* board, int sto)
{
    evaluatorTracker.undoMove(from, to, board, sto);
}

int Evaluator::hangingToCheck(float value)
{
    if(value<-2000)
    {
        return -10;
    }
    else
    {
        return value/100;
    }
}
