//
//  AI_Support.cpp
//  Schach
//
//  Created by Thomas Redding on 7/9/14.
//  Copyright (c) 2014 Thomas Redding. All rights reserved.
//

#include "AI.h"

void AI::setMoves(int layer)
{
    moveLength[layer] = 500 * layer + movementCalculator.calculateMoves(&move[500 * layer], layer % 2);
}

int AI::aiInCheck(int king)
{
    // 1 = in check; 0 = safe
    int i;
    // pawn attacks
    if(king%8!=0)
    {
        if(king/8!=0)
        {
            if(board.b[king-9]==-1)
            {
                return 1;
            }
        }
        if(king/8!=7)
        {
            if(board.b[king+7]==-1)
            {
                return 1;
            }
        }
    }
    
    // knight attacks
    if(king%8>0)
    {
        if(king/8>1 && board.b[king-17]==-2)
        {
            return 1;
        }
        if(king/8<6 && board.b[king+15]==-2)
        {
            return 1;
        }
    }
    if(king%8>1)
    {
        if(king/8>0 && board.b[king-10]==-2)
        {
            return 1;
        }
        if(king/8<7 && board.b[king+6]==-2)
        {
            return 1;
        }
    }
    if(king%8<6)
    {
        if(king/8>0 && board.b[king-6]==-2)
        {
            return 1;
        }
        if(king/8<7 && board.b[king+10]==-2)
        {
            return 1;
        }
    }
    if(king%8<7)
    {
        if(king/8>1 && board.b[king-15]==-2)
        {
            return 1;
        }
        if(king/8<6 && board.b[king+17]==-2)
        {
            return 1;
        }
    }
    
    // bishop attacks
    int dist;
    // down-left
    if(king/8<king%8)
    {
        dist = king/8;
    }
    else
    {
        dist = king%8;
    }
    for(i=1;i<=dist;i++)
    {
        if(board.b[king-9*i]==-3 || board.b[king-9*i]==-5)
        {
            return 1;
        }
        if(board.b[king-9*i]!=0)
        {
            break;
        }
    }
    // up-right
    if(king/8<king%8)
    {
        dist = 7-king%8;
    }
    else
    {
        dist = 7-king/8;
    }
    for(i=1;i<=dist;i++)
    {
        if(board.b[king+9*i]==-3 || board.b[king+9*i]==-5)
        {
            return 1;
        }
        if(board.b[king+9*i]!=0)
        {
            break;
        }
    }
    // down-right
    if(7-king/8<king%8)
    {
        dist = 7-king/8;
    }
    else
    {
        dist = king%8;
    }
    for(i=1;i<=dist;i++)
    {
        if(board.b[king+7*i]==-3 || board.b[king+7*i]==-5)
        {
            return 1;
        }
        if(board.b[king+7*i]!=0)
        {
            break;
        }
    }
    // down-left
    if(king/8<7-king%8)
    {
        dist = king/8;
    }
    else
    {
        dist = 7-king%8;
    }
    for(i=1;i<=dist;i++)
    {
        if(board.b[king-7*i]==-3 || board.b[king-7*i]==-5)
        {
            return 1;
        }
        if(board.b[king-7*i]!=0)
        {
            break;
        }
    }
    
    // rook attacks
    // forward
    dist = 7-king%8;
    for(i=1;i<=dist;i++)
    {
        if(board.b[king+i]==-4 || board.b[king+i]==-5)
        {
            return 1;
        }
        if(board.b[king+i]!=0)
        {
            break;
        }
    }
    
    // backward
    dist = king%8;
    for(i=1;i<=dist;i++)
    {
        if(board.b[king-i]==-4 || board.b[king-i]==-5)
        {
            return 1;
        }
        if(board.b[king-i]!=0)
        {
            break;
        }
    }
    // left
    dist = king/8;
    for(i=1;i<=dist;i++)
    {
        if(board.b[king-8*i]==-4 || board.b[king-8*i]==-5)
        {
            return 1;
        }
        if(board.b[king-8*i]!=0)
        {
            break;
        }
    }
    
    // right
    dist = 7-king/8;
    for(i=1;i<=dist;i++)
    {
        if(board.b[king+8*i]==-4 || board.b[king+8*i]==-5)
        {
            return 1;
        }
        if(board.b[king+8*i]!=0)
        {
            break;
        }
    }
    
    // king attacks
    if(king/8>0)
    {
        if(board.b[king-8]==-6)
        {
            return 1;
        }
        if(king%8>0 && board.b[king-9]==-6)
        {
            return 1;
        }
        if(king%8<7 && board.b[king-7]==-6)
        {
            return 1;
        }
    }
    if(king/8<7)
    {
        if(board.b[king+8]==-6)
        {
            return 1;
        }
        if(king%8>0 && board.b[king+7]==-6)
        {
            return 1;
        }
        if(king%8<7 && board.b[king+9]==-6)
        {
            return 1;
        }
    }
    if(king%8>0 && board.b[king-1]==-6)
    {
        return 1;
    }
    if(king%8<7 && board.b[king+1]==-6)
    {
        return 1;
    }
    return 0;
}

int AI::humanInCheck(int kingsq)
{
    // 1 = in check; 0 = safe; -1 = no king
    int i;
    // pawn attacks
    if(kingsq%8!=7)
    {
        if(kingsq/8!=0)
        {
            if(board.b[kingsq-7]==1)
            {
                return 1;
            }
        }
        if(kingsq/8!=7)
        {
            if(board.b[kingsq+9]==1)
            {
                return 1;
            }
        }
    }
    
    // knight attacks
    if(kingsq%8>0)
    {
        if(kingsq/8>1 && board.b[kingsq-17]==2)
        {
            return 1;
        }
        if(kingsq/8<6 && board.b[kingsq+15]==2)
        {
            return 1;
        }
    }
    if(kingsq%8>1)
    {
        if(kingsq/8>0 && board.b[kingsq-10]==2)
        {
            return 1;
        }
        if(kingsq/8<7 && board.b[kingsq+6]==2)
        {
            return 1;
        }
    }
    if(kingsq%8<6)
    {
        if(kingsq/8>0 && board.b[kingsq-6]==2)
        {
            return 1;
        }
        if(kingsq/8<7 && board.b[kingsq+10]==2)
        {
            return 1;
        }
    }
    if(kingsq%8<7)
    {
        if(kingsq/8>1 && board.b[kingsq-15]==2)
        {
            return 1;
        }
        if(kingsq/8<6 && board.b[kingsq+17]==2)
        {
            return 1;
        }
    }
    // bishop attacks
    int dist;
    // down-left
    if(kingsq/8<kingsq%8)
    {
        dist = kingsq/8;
    }
    else
    {
        dist = kingsq%8;
    }
    for(i=1;i<=dist;i++)
    {
        if(board.b[kingsq-9*i]==3 || board.b[kingsq-9*i]==5)
        {
            return 1;
        }
        if(board.b[kingsq-9*i]!=0)
        {
            break;
        }
    }
    // up-right
    if(kingsq/8<kingsq%8)
    {
        dist = 7-kingsq%8;
    }
    else
    {
        dist = 7-kingsq/8;
    }
    for(i=1;i<=dist;i++)
    {
        if(board.b[kingsq+9*i]==3 || board.b[kingsq+9*i]==5)
        {
            return 1;
        }
        if(board.b[kingsq+9*i]!=0)
        {
            break;
        }
    }
    // down-right
    if(7-kingsq/8<kingsq%8)
    {
        dist = 7-kingsq/8;
    }
    else
    {
        dist = kingsq%8;
    }
    for(i=1;i<=dist;i++)
    {
        if(board.b[kingsq+7*i]==3 || board.b[kingsq+7*i]==5)
        {
            return 1;
        }
        if(board.b[kingsq+7*i]!=0)
        {
            break;
        }
    }
    // up-left
    if(kingsq/8<7-kingsq%8)
    {
        dist = kingsq/8;
    }
    else
    {
        dist = 7-kingsq%8;
    }
    for(i=1;i<=dist;i++)
    {
        if(board.b[kingsq-7*i]==3 || board.b[kingsq-7*i]==5)
        {
            return 1;
        }
        if(board.b[kingsq-7*i]!=0)
        {
            break;
        }
    }
    
    // rook attacks
    // forward
    dist = 7-kingsq%8;
    for(i=1;i<=dist;i++)
    {
        if(board.b[kingsq+i]==4 || board.b[kingsq+i]==5)
        {
            return 1;
        }
        if(board.b[kingsq+i]!=0)
        {
            break;
        }
    }
    
    // backward
    dist = kingsq%8;
    for(i=1;i<=dist;i++)
    {
        if(board.b[kingsq-i]==4 || board.b[kingsq-i]==5)
        {
            return 1;
        }
        if(board.b[kingsq-i]!=0)
        {
            break;
        }
    }
    
    // left
    dist = kingsq/8;
    for(i=1;i<=dist;i++)
    {
        if(board.b[kingsq-8*i]==4 || board.b[kingsq-8*i]==5)
        {
            return 1;
        }
        if(board.b[kingsq-8*i]!=0)
        {
            break;
        }
    }
    
    // right
    dist = 7-kingsq/8;
    for(i=1;i<=dist;i++)
    {
        if(board.b[kingsq+8*i]==4 || board.b[kingsq+8*i]==5)
        {
            return 1;
        }
        if(board.b[kingsq+8*i]!=0)
        {
            break;
        }
    }
    
    // kingsq attacks
    if(kingsq/8>0)
    {
        if(board.b[kingsq-8]==6)
        {
            return 1;
        }
        if(kingsq%8>0 && board.b[kingsq-9]==6)
        {
            return 1;
        }
        if(kingsq%8<7 && board.b[kingsq-7]==6)
        {
            return 1;
        }
    }
    if(kingsq/8<7)
    {
        if(board.b[kingsq+8]==6)
        {
            return 1;
        }
        if(kingsq%8>0 && board.b[kingsq+7]==6)
        {
            return 1;
        }
        if(kingsq%8<7 && board.b[kingsq+9]==6)
        {
            return 1;
        }
    }
    if(kingsq%8>0 && board.b[kingsq-1]==6)
    {
        return 1;
    }
    if(kingsq%8<7 && board.b[kingsq+1]==6)
    {
        return 1;
    }
    return 0;
}

bool AI::willMovePutMeInCheck(int king, Move move)
{
    int sto, stoB;
    int answer;
    if(board.b[king]==6)
    {
        // cpu
        if(move.from>=100)
        {
            // promotion
            sto = board.b[move.to];
            board.b[move.to] = (int) (move.from/100);
            board.b[move.from%100] = 0;
            stoB = board.fiftyMoveDrawCounter;
            if(sto==0)
            {
                board.fiftyMoveDrawCounter++;
            }
            else
            {
                board.fiftyMoveDrawCounter = 0;
            }
            
            int sto0 = board.castling[0];
            int sto1 = board.castling[1];
            int sto2 = board.castling[2];
            int sto3 = board.castling[3];
            if(board.b[move.to]==-4)
            {
                if(move.to==0)
                {
                    board.castling[2] = 0;
                }
                else if(move.to==56)
                {
                    board.castling[3] = 0;
                }
            }
            
            answer = aiInCheck(king);
            
            board.castling[0] = sto0;
            board.castling[1] = sto1;
            board.castling[2] = sto2;
            board.castling[3] = sto3;
            
            board.fiftyMoveDrawCounter = stoB;
            board.b[move.to] = sto;
            board.b[move.from%100] = 1;
        }
        else if(board.b[move.from]==6 && (move.to-move.from==16 || move.from-move.to==16))
        {
            int sto0 = board.castling[0];
            int sto1 = board.castling[1];
            board.castling[0] = 0;
            board.castling[1] = 0;
            board.fiftyMoveDrawCounter++;
            // castle
            if(move.to==23)
            {
                board.b[7] = 0;
                board.b[23] = 6;
                board.b[31] = 4;
                board.b[39] = 0;
            }
            else
            {
                board.b[39] = 0;
                board.b[47] = 4;
                board.b[55] = 6;
                board.b[63] = 0;
            }
            
            answer = aiInCheck(move.to);
            
            if(move.to==23)
            {
                board.b[7] = 4;
                board.b[23] = 0;
                board.b[31] = 0;
                board.b[39] = 6;
            }
            else
            {
                board.b[39] = 6;
                board.b[47] = 0;
                board.b[55] = 0;
                board.b[63] = 4;
            }
            board.fiftyMoveDrawCounter--;
            board.castling[0] = sto0;
            board.castling[1] = sto1;
        }
        else if(board.b[move.from]==1 && board.b[move.to]==0 && (move.to-move.from==7 || move.to-move.from==-9))
        {
            // En passant
            board.b[move.to+1] = 0;
            board.b[move.to] = 1;
            board.b[move.from] = 0;
            stoB = board.fiftyMoveDrawCounter;
            board.fiftyMoveDrawCounter = 0;
            
            answer = aiInCheck(king);
            
            board.fiftyMoveDrawCounter = stoB;
            board.b[move.to+1] = -1;
            board.b[move.to] = 0;
            board.b[move.from] = 1;
        }
        else
        {
            int sto0 = board.castling[0];
            int sto1 = board.castling[1];
            int sto2 = board.castling[2];
            int sto3 = board.castling[3];
            if(board.b[move.from]==6)
            {
                board.castling[0] = 0;
                board.castling[1] = 0;
            }
            else if(board.b[move.from]==4)
            {
                if(move.from==7)
                {
                    board.castling[0] = 0;
                }
                else if(move.from==63)
                {
                    board.castling[1] = 0;
                }
            }
            if(board.b[move.to]==-4)
            {
                if(move.to==0)
                {
                    board.castling[2] = 0;
                }
                else if(move.to==56)
                {
                    board.castling[3] = 0;
                }
            }
            
            
            sto = board.b[move.to];
            board.b[move.to] = board.b[move.from];
            board.b[move.from] = 0;
            
            stoB = board.fiftyMoveDrawCounter;
            if(sto==0)
            {
                board.fiftyMoveDrawCounter++;
            }
            else
            {
                board.fiftyMoveDrawCounter = 0;
            }
            
            if(board.b[move.to]==6)
            {
                answer = aiInCheck(move.to);
            }
            else
            {
                answer = aiInCheck(king);
            }
            
            
            board.fiftyMoveDrawCounter = stoB;
            board.castling[0] = sto0;
            board.castling[1] = sto1;
            board.castling[2] = sto2;
            board.castling[3] = sto3;
            board.b[move.from] = board.b[move.to];
            board.b[move.to] = sto;
        }
    }
    else
    {
        // human
        if(move.from>=100)
        {
            // promotion
            sto = board.b[move.to];
            board.b[move.to] = (int) (move.from/100);
            board.b[move.from%100] = 0;
            board.fiftyMoveDrawCounter++;
            
            int sto0 = board.castling[0];
            int sto1 = board.castling[1];
            int sto2 = board.castling[2];
            int sto3 = board.castling[3];
            if(board.b[move.to]==4)
            {
                if(move.to==7)
                {
                    board.castling[0] = 0;
                }
                else
                {
                    board.castling[1] = 0;
                }
            }
            
            answer = humanInCheck(king);
            
            board.castling[0] = sto0;
            board.castling[1] = sto1;
            board.castling[2] = sto2;
            board.castling[3] = sto3;
            
            board.fiftyMoveDrawCounter--;
            board.b[move.to] = sto;
            board.b[move.from%100] = -1;
        }
        else if(board.b[move.from]==-6 && (move.to-move.from==16 || move.from-move.to==16))
        {
            int sto0 = board.castling[2];
            int sto1 = board.castling[3];
            board.castling[2] = 0;
            board.castling[3] = 0;
            // castling
            if(move.to==16)
            {
                board.b[0] = 0;
                board.b[16] = -6;
                board.b[24] = -4;
                board.b[32] = 0;
            }
            else
            {
                board.b[32] = 0;
                board.b[40] = -4;
                board.b[48] = -6;
                board.b[56] = 0;
            }
            board.fiftyMoveDrawCounter++;
            
            answer = humanInCheck(move.to);
            
            board.fiftyMoveDrawCounter--;
            if(move.to==16)
            {
                board.b[0] = -4;
                board.b[16] = 0;
                board.b[24] = 0;
                board.b[32] = -6;
            }
            else
            {
                board.b[32] = -6;
                board.b[40] = 0;
                board.b[48] = 0;
                board.b[56] = -4;
            }
            board.castling[2] = sto0;
            board.castling[3] = sto1;
        }
        else if(board.b[move.from]==-1 && board.b[move.to]==0 && (move.to-move.from==9 || move.to-move.from==-7))
        {
            // En passant
            board.b[move.to-1] = 0;
            board.b[move.to] = -1;
            board.b[move.from] = 0;
            stoB = board.fiftyMoveDrawCounter;
            board.fiftyMoveDrawCounter = 0;
            
            answer = humanInCheck(king);
            
            board.fiftyMoveDrawCounter = stoB;
            board.b[move.to-1] = 1;
            board.b[move.to] = 0;
            board.b[move.from] = -1;
        }
        else
        {
            int sto0 = board.castling[0];
            int sto1 = board.castling[1];
            int sto2 = board.castling[2];
            int sto3 = board.castling[3];
            if(board.b[move.from]==-6)
            {
                board.castling[2] = 0;
                board.castling[3] = 0;
            }
            else if(board.b[move.from]==-4)
            {
                if(move.from==0)
                {
                    board.castling[2] = 0;
                }
                else if(move.from==56)
                {
                    board.castling[3] = 0;
                }
            }
            if(board.b[move.to]==4)
            {
                if(move.to==7)
                {
                    board.castling[0] = 0;
                }
                else
                {
                    board.castling[1] = 0;
                }
            }
            
            sto = board.b[move.to];
            board.b[move.to] = board.b[move.from];
            board.b[move.from] = 0;
            if(sto==0)
            {
                board.fiftyMoveDrawCounter++;
            }
            else
            {
                stoB = board.fiftyMoveDrawCounter;
                board.fiftyMoveDrawCounter = 0;
            }
            
            if(board.b[move.to]==-6)
            {
                answer = humanInCheck(move.to);
            }
            else
            {
                answer = humanInCheck(king);
            }
            
            if(sto==0)
            {
                board.fiftyMoveDrawCounter--;
            }
            else
            {
                board.fiftyMoveDrawCounter = stoB;
            }
            board.b[move.from] = board.b[move.to];
            board.b[move.to] = sto;
            board.castling[0] = sto0;
            board.castling[1] = sto1;
            board.castling[2] = sto2;
            board.castling[3] = sto3;
        }
    }
    return answer;
}