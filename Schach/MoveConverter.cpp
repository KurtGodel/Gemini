//
//  MoveConverter.cpp
//  Schach
//
//  Created by Thomas Redding on 7/15/14.
//  Copyright (c) 2014 Thomas Redding. All rights reserved.
//

#include "MoveConverter.h"

std::string MoveConverter::convertMoveToString(Board* b, int from, int to, int colorOfHuman)
{
    for(int i=0; i<64; i++)
    {
        board.b[i] = b->b[i];
    }
    board.castling[0] = b->castling[0];
    board.castling[1] = b->castling[1];
    board.castling[2] = b->castling[2];
    board.castling[3] = b->castling[3];
    board.lastMove[0] = b->lastMove[0];
    board.lastMove[1] = b->lastMove[1];
    board.fiftyMoveDrawCounter = b->fiftyMoveDrawCounter;
    
    if(from>=100)
    {
        from = from%100;
    }
    
    int i;
    
    if(board.b[from]==0)
    {
        std::cout << "(TRANSLATE-ERROR-1)";
        std::cout << "(";
        std::cout << from;
        std::cout << ",";
        std::cout << to;
        std::cout << ") ";
        return "-";
    }
    
    std::string str = "";
    
    int promotion = from/100;
    from = from%100;
    
    bool castling = false;
    
    if(abs(board.b[from])==6 && (to-from==16 || from-to==16))
    {
        castling = true;
        // castling
        if(to-from==16)
        {
            str += "O-O";
        }
        else
        {
            str += "O-O-O";
        }
    }
    
    int illDefinedMove[20];
    int illDefinedMoveLength = 0;
    // check for any extra required specification (row, column)
    if(board.b[from]>0 && !castling)
    {
        // cpu's move
        setMoves(0);
        for(i=0;i<moveLength[0];i+=2)
        {
            if(move[i+1]==to && board.b[move[i]%100]==board.b[from])
            {
                illDefinedMove[illDefinedMoveLength] = i;
                illDefinedMoveLength++;
            }
        }
    }
    else if(!castling)
    {
        // human's move
        setMoves(1);
        for(i=500;i<moveLength[1];i+=2)
        {
            if(move[i+1]==to && board.b[move[i]%100]==board.b[from])
            {
                illDefinedMove[illDefinedMoveLength] = i;
                illDefinedMoveLength++;
            }
        }
    }
    
    if(abs(board.b[from])==2)
    {
        str += "N";
    }
    else if(abs(board.b[from])==3)
    {
        str += "B";
    }
    else if(abs(board.b[from])==4)
    {
        str += "R";
    }
    else if(abs(board.b[from])==5)
    {
        str += "Q";
    }
    else if(abs(board.b[from])==6 && !castling)
    {
        str += "K";
    }
    
    int counter = 0;
    if(illDefinedMoveLength==0 && !castling)
    {
        std::cout << "(TRANSLATE-ERROR-2)";
        std::cout << "(";
        std::cout << from;
        std::cout << ",";
        std::cout << to;
        std::cout << ") ";
        return "";
    }
    else if(illDefinedMoveLength != 1 && !castling && abs(board.b[from]) != 1)
    {
        // ill-defined move
        
        for(i=0;i<illDefinedMoveLength;i++)
        {
            if(move[illDefinedMove[i]]/8==from/8)
            {
                counter++;
            }
        }
        if(counter==1)
        {
            if(colorOfHuman==0)
            {
                if(from/8==0)
                {
                    str += "a";
                }
                else if(from/8==1)
                {
                    str += "b";
                }
                else if(from/8==2)
                {
                    str += "c";
                }
                else if(from/8==3)
                {
                    str += "d";
                }
                else if(from/8==4)
                {
                    str += "e";
                }
                else if(from/8==5)
                {
                    str += "f";
                }
                else if(from/8==6)
                {
                    str += "g";
                }
                else if(from/8==7)
                {
                    str += "h";
                }
            }
            else
            {
                if(from/8==0)
                {
                    str += "a";
                }
                else if(from/8==1)
                {
                    str += "b";
                }
                else if(from/8==2)
                {
                    str += "c";
                }
                else if(from/8==3)
                {
                    str += "d";
                }
                else if(from/8==4)
                {
                    str += "e";
                }
                else if(from/8==5)
                {
                    str += "f";
                }
                else if(from/8==6)
                {
                    str += "g";
                }
                else if(from/8==7)
                {
                    str += "h";
                }
            }
        }
        else
        {
            counter = 0;
            for(i=0;i<illDefinedMoveLength;i++)
            {
                if(move[illDefinedMove[i]]%8==from%8)
                {
                    counter++;
                }
            }
            
            if(counter==1)
            {
                if(colorOfHuman==0)
                {
                    str += std::to_string(8-from%8);
                }
                else
                {
                    str += std::to_string(from%8+1);
                }
            }
            else
            {
                if(colorOfHuman==0)
                {
                    if(from/8==0)
                    {
                        str += "a";
                    }
                    else if(from/8==1)
                    {
                        str += "b";
                    }
                    else if(from/8==2)
                    {
                        str += "c";
                    }
                    else if(from/8==3)
                    {
                        str += "d";
                    }
                    else if(from/8==4)
                    {
                        str += "e";
                    }
                    else if(from/8==5)
                    {
                        str += "f";
                    }
                    else if(from/8==6)
                    {
                        str += "g";
                    }
                    else if(from/8==7)
                    {
                        str += "h";
                    }
                    str += std::to_string(8-from%8);
                }
                else
                {
                    if(from/8==0)
                    {
                        str += "a";
                    }
                    else if(from/8==1)
                    {
                        str += "b";
                    }
                    else if(from/8==2)
                    {
                        str += "c";
                    }
                    else if(from/8==3)
                    {
                        str += "d";
                    }
                    else if(from/8==4)
                    {
                        str += "e";
                    }
                    else if(from/8==5)
                    {
                        str += "f";
                    }
                    else if(from/8==6)
                    {
                        str += "g";
                    }
                    else if(from/8==7)
                    {
                        str += "h";
                    }
                    str += std::to_string(from%8+1);
                }
            }
        }
    }
    
    if(board.b[to]!=0)
    {
        if(abs(board.b[from]) == 1)
        {
            if(from/8 == 0)
            {
                str += "a";
            }
            else if(from/8 == 1)
            {
                str += "b";
            }
            else if(from/8 == 2)
            {
                str += "c";
            }
            else if(from/8 == 3)
            {
                str += "d";
            }
            else if(from/8 == 4)
            {
                str += "e";
            }
            else if(from/8 == 5)
            {
                str += "f";
            }
            else if(from/8 == 6)
            {
                str += "g";
            }
            else if(from/8 == 7)
            {
                str += "h";
            }
        }
        str += "x";
    }
    else if(abs(board.b[from])==1 && board.b[to]==0)
    {
        // check for en Passant
        if(board.b[from]==1)
        {
            if(to-from==-9)
            {
                str += "x";
            }
            else if(to-from==7)
            {
                str += "x";
            }
        }
        else if(board.b[from]==-1)
        {
            if(to-from==9)
            {
                str += "x";
            }
            else if(to-from==-7)
            {
                str += "x";
            }
        }
    }
    
    if(colorOfHuman==0 && !castling)
    {
        if(to/8==0)
        {
            str += "a";
        }
        else if(to/8==1)
        {
            str += "b";
        }
        else if(to/8==2)
        {
            str += "c";
        }
        else if(to/8==3)
        {
            str += "d";
        }
        else if(to/8==4)
        {
            str += "e";
        }
        else if(to/8==5)
        {
            str += "f";
        }
        else if(to/8==6)
        {
            str += "g";
        }
        else if(to/8==7)
        {
            str += "h";
        }
        str += std::to_string(8-to%8);
    }
    else if(!castling)
    {
        if(to/8==0)
        {
            str += "a";
        }
        else if(to/8==1)
        {
            str += "b";
        }
        else if(to/8==2)
        {
            str += "c";
        }
        else if(to/8==3)
        {
            str += "d";
        }
        else if(to/8==4)
        {
            str += "e";
        }
        else if(to/8==5)
        {
            str += "f";
        }
        else if(to/8==6)
        {
            str += "g";
        }
        else if(to/8==7)
        {
            str += "h";
        }
        str += std::to_string(to%8+1);
    }
    
    if(promotion!=0)
    {
        str += "=";
        if(promotion==2)
        {
            str += "N";
        }
        if(promotion==3)
        {
            str += "B";
        }
        if(promotion==4)
        {
            str += "R";
        }
        if(promotion==5)
        {
            str += "Q";
        }
    }
    // str += "(" + std::to_string(from) + "," + std::to_string(to) + ")";
    // TODO: check & mate checker (# and +)
    return str;
}

int MoveConverter::aiInCheck(int king)
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

int MoveConverter::humanInCheck(int kingsq)
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

void MoveConverter::setMoves(int layer)
{
    if(layer%2==0)
    {
        int i;
        int j;
        int dist;
        int begin = 500*layer;
        for(i=0;i<64;i++)
        {
            if(board.b[i]>0)
            {
                if(board.b[i]==1)
                {
                    // pawn
                    if(board.b[i-1]==0)
                    {
                        // forward one
                        if(i%8!=1)
                        {
                            // no promotion
                            move[begin] = i;
                            move[begin+1] = i-1;
                            begin+=2;
                        }
                        else
                        {
                            // promotion
                            move[begin] = 200+i;
                            move[begin+1] = i-1;
                            begin+=2;
                            move[begin] = 300+i;
                            move[begin+1] = i-1;
                            begin+=2;
                            move[begin] = 400+i;
                            move[begin+1] = i-1;
                            begin+=2;
                            move[begin] = 500+i;
                            move[begin+1] = i-1;
                            begin+=2;
                        }
                        if(i%8==6 && board.b[i-2]==0)
                        {
                            // forward two
                            move[begin] = i;
                            move[begin+1] = i-2;
                            begin += 2;
                        }
                    }
                    if(board.b[i-9]<0 && i/8!=0)
                    {
                        if(i%8!=1)
                        {
                            // no promotion
                            move[begin] = i;
                            move[begin+1] = i-9;
                            begin += 2;
                        }
                        else
                        {
                            // promotion
                            move[begin] = 200+i;
                            move[begin+1] = i-9;
                            begin += 2;
                            move[begin] = 300+i;
                            move[begin+1] = i-9;
                            begin += 2;
                            move[begin] = 400+i;
                            move[begin+1] = i-9;
                            begin += 2;
                            move[begin] = 500+i;
                            move[begin+1] = i-9;
                            begin += 2;
                        }
                    }
                    if(board.b[i+7]<0 && i/8!=7)
                    {
                        if(i%8!=1)
                        {
                            // no promotion
                            move[begin] = i;
                            move[begin+1] = i+7;
                            begin += 2;
                        }
                        else
                        {
                            // promotion
                            move[begin] = 200+i;
                            move[begin+1] = i+7;
                            begin += 2;
                            move[begin] = 300+i;
                            move[begin+1] = i+7;
                            begin += 2;
                            move[begin] = 400+i;
                            move[begin+1] = i+7;
                            begin += 2;
                            move[begin] = 500+i;
                            move[begin+1] = i+7;
                            begin += 2;
                        }
                    }
                    
                    // en passant
                    if(i/8!=7)
                    {
                        if(board.b[i+8]==-1)
                        {
                            if(board.lastMove[1]-board.lastMove[0]==2 &&  board.lastMove[1]==i+8 && board.b[i+7]==0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+7;
                                begin += 2;
                            }
                        }
                    }
                    if(i/8!=0)
                    {
                        if(board.b[i-8]==-1)
                        {
                            if(board.lastMove[1]-board.lastMove[0]==2 &&  board.lastMove[1]==i-8 && board.b[i-9]==0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-9;
                                begin += 2;
                            }
                        }
                    }
                }
                else if(board.b[i]==2)
                {
                    // knight
                    if(i/8==0)
                    {
                        if(i%8==0)
                        {
                            if(board.b[10]<=0)
                            {
                                move[begin] = 0;
                                move[begin+1] = 10;
                                begin += 2;
                            }
                            if(board.b[17]<=0)
                            {
                                move[begin] = 0;
                                move[begin+1] = 17;
                                begin += 2;
                            }
                        }
                        else if(i%8==1)
                        {
                            if(board.b[11]<=0)
                            {
                                move[begin] = 1;
                                move[begin+1] = 11;
                                begin += 2;
                            }
                            if(board.b[16]<=0)
                            {
                                move[begin] = 1;
                                move[begin+1] = 16;
                                begin += 2;
                            }
                            if(board.b[18]<=0)
                            {
                                move[begin] = 1;
                                move[begin+1] = 18;
                                begin += 2;
                            }
                        }
                        else if(i%8==6)
                        {
                            if(board.b[12]<=0)
                            {
                                move[begin] = 6;
                                move[begin+1] = 12;
                                begin += 2;
                            }
                            if(board.b[21]<=0)
                            {
                                move[begin] = 6;
                                move[begin+1] = 21;
                                begin += 2;
                            }
                            if(board.b[23]<=0)
                            {
                                move[begin] = 6;
                                move[begin+1] = 23;
                                begin += 2;
                            }
                        }
                        else if(i%8==7)
                        {
                            if(board.b[13]<=0)
                            {
                                move[begin] = 7;
                                move[begin+1] = 13;
                                begin += 2;
                            }
                            if(board.b[22]<=0)
                            {
                                move[begin] = 7;
                                move[begin+1] = 22;
                                begin += 2;
                            }
                        }
                        else
                        {
                            if(board.b[i+6]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+6;
                                begin += 2;
                            }
                            if(board.b[i+10]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+10;
                                begin += 2;
                            }
                            if(board.b[i+15]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+15;
                                begin += 2;
                            }
                            if(board.b[i+17]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+17;
                                begin += 2;
                            }
                        }
                    }
                    else if(i/8==1)
                    {
                        if(i%8==0)
                        {
                            if(board.b[2]<=0)
                            {
                                move[begin] = 8;
                                move[begin+1] = 2;
                                begin += 2;
                            }
                            if(board.b[18]<=0)
                            {
                                move[begin] = 8;
                                move[begin+1] = 18;
                                begin += 2;
                            }
                            if(board.b[25]<=0)
                            {
                                move[begin] = 8;
                                move[begin+1] = 25;
                                begin += 2;
                            }
                        }
                        else if(i%8==1)
                        {
                            if(board.b[3]<=0)
                            {
                                move[begin] = 9;
                                move[begin+1] = 3;
                                begin += 2;
                            }
                            if(board.b[19]<=0)
                            {
                                move[begin] = 9;
                                move[begin+1] = 19;
                                begin += 2;
                            }
                            if(board.b[24]<=0)
                            {
                                move[begin] = 9;
                                move[begin+1] = 24;
                                begin += 2;
                            }
                            if(board.b[26]<=0)
                            {
                                move[begin] = 9;
                                move[begin+1] = 26;
                                begin += 2;
                            }
                        }
                        else if(i%8==6)
                        {
                            if(board.b[4]<=0)
                            {
                                move[begin] = 14;
                                move[begin+1] = 4;
                                begin += 2;
                            }
                            if(board.b[20]<=0)
                            {
                                move[begin] = 14;
                                move[begin+1] = 20;
                                begin += 2;
                            }
                            if(board.b[29]<=0)
                            {
                                move[begin] = 14;
                                move[begin+1] = 29;
                                begin += 2;
                            }
                            if(board.b[31]<=0)
                            {
                                move[begin] = 14;
                                move[begin+1] = 31;
                                begin += 2;
                            }
                        }
                        else if(i%8==7)
                        {
                            if(board.b[5]<=0)
                            {
                                move[begin] = 15;
                                move[begin+1] = 5;
                                begin += 2;;
                            }
                            if(board.b[21]<=0)
                            {
                                move[begin] = 15;
                                move[begin+1] = 21;
                                begin += 2;;
                            }
                            if(board.b[30]<=0)
                            {
                                move[begin] = 15;
                                move[begin+1] = 30;
                                begin += 2;;
                            }
                        }
                        else
                        {
                            if(board.b[i-10]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-10;
                                begin += 2;
                            }
                            if(board.b[i-6]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-6;
                                begin += 2;
                            }
                            if(board.b[i+6]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+6;
                                begin += 2;
                            }
                            if(board.b[i+10]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+10;
                                begin += 2;
                            }
                            if(board.b[i+15]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+15;
                                begin += 2;
                            }
                            if(board.b[i+17]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+17;
                                begin += 2;
                            }
                        }
                    }
                    else if(i/8==6)
                    {
                        if(i%8==0)
                        {
                            if(board.b[33]<=0)
                            {
                                move[begin] = 48;
                                move[begin+1] = 33;
                                begin += 2;
                            }
                            if(board.b[42]<=0)
                            {
                                move[begin] = 48;
                                move[begin+1] = 42;
                                begin += 2;
                            }
                            if(board.b[58]<=0)
                            {
                                move[begin] = 48;
                                move[begin+1] = 58;
                                begin += 2;
                            }
                        }
                        else if(i%8==1)
                        {
                            if(board.b[32]<=0)
                            {
                                move[begin] = 49;
                                move[begin+1] = 32;
                                begin += 2;
                            }
                            if(board.b[34]<=0)
                            {
                                move[begin] = 49;
                                move[begin+1] = 34;
                                begin += 2;
                            }
                            if(board.b[43]<=0)
                            {
                                move[begin] = 49;
                                move[begin+1] = 43;
                                begin += 2;
                            }
                            if(board.b[50]<=0)
                            {
                                move[begin] = 49;
                                move[begin+1] = 59;
                                begin += 2;
                            }
                        }
                        else if(i%8==6)
                        {
                            if(board.b[37]<=0)
                            {
                                move[begin] = 54;
                                move[begin+1] = 37;
                                begin += 2;
                            }
                            if(board.b[39]<=0)
                            {
                                move[begin] = 54;
                                move[begin+1] = 39;
                                begin += 2;
                            }
                            if(board.b[44]<=0)
                            {
                                move[begin] = 54;
                                move[begin+1] = 4;
                                begin += 2;
                            }
                            if(board.b[60]<=0)
                            {
                                move[begin] = 54;
                                move[begin+1] = 60;
                                begin += 2;
                            }
                        }
                        else if(i%8==7)
                        {
                            if(board.b[38]<=0)
                            {
                                move[begin] = 55;
                                move[begin+1] = 38;
                                begin += 2;
                            }
                            if(board.b[45]<=0)
                            {
                                move[begin] = 55;
                                move[begin+1] = 45;
                                begin += 2;
                            }
                            if(board.b[61]<=0)
                            {
                                move[begin] = 55;
                                move[begin+1] = 61;
                                begin += 2;
                            }
                        }
                        else
                        {
                            if(board.b[i-17]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-17;
                                begin += 2;
                            }
                            if(board.b[i-15]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-15;
                                begin += 2;
                            }
                            if(board.b[i-10]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-10;
                                begin += 2;
                            }
                            if(board.b[i-6]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-6;
                                begin += 2;
                            }
                            if(board.b[i+6]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+6;
                                begin += 2;
                            }
                            if(board.b[i+10]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+10;
                                begin += 2;
                            }
                        }
                    }
                    else if(i/8==7)
                    {
                        if(i%8==0)
                        {
                            if(board.b[41]<=0)
                            {
                                move[begin] = 56;
                                move[begin+1] = 41;
                                begin += 2;
                            }
                            if(board.b[50]<=0)
                            {
                                move[begin] = 56;
                                move[begin+1] = 50;
                                begin += 2;
                            }
                        }
                        else if(i%8==1)
                        {
                            if(board.b[40]<=0)
                            {
                                move[begin] = 57;
                                move[begin+1] = 40;
                                begin += 2;
                            }
                            if(board.b[42]<=0)
                            {
                                move[begin] = 57;
                                move[begin+1] = 42;
                                begin += 2;
                            }
                            if(board.b[51]<=0)
                            {
                                move[begin] = 57;
                                move[begin+1] = 51;
                                begin += 2;
                            }
                        }
                        else if(i%8==6)
                        {
                            if(board.b[45]<=0)
                            {
                                move[begin] = 62;
                                move[begin+1] = 45;
                                begin += 2;
                            }
                            if(board.b[47]<=0)
                            {
                                move[begin] = 62;
                                move[begin+1] = 47;
                                begin += 2;
                            }
                            if(board.b[52]<=0)
                            {
                                move[begin] = 62;
                                move[begin+1] = 52;
                                begin += 2;
                            }
                        }
                        else if(i%8==7)
                        {
                            if(board.b[46]<=0)
                            {
                                move[begin] = 63;
                                move[begin+1] = 46;
                                begin += 2;
                            }
                            if(board.b[53]<=0)
                            {
                                move[begin] = 63;
                                move[begin+1] = 53;
                                begin += 2;
                            }
                        }
                        else
                        {
                            if(board.b[i-17]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-17;
                                begin += 2;
                            }
                            if(board.b[i-15]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-15;
                                begin += 2;
                            }
                            if(board.b[i-10]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-10;
                                begin += 2;
                            }
                            if(board.b[i-6]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-6;
                                begin += 2;
                            }
                        }
                    }
                    else
                    {
                        if(i%8==0)
                        {
                            if(board.b[i-15]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-15;
                                begin += 2;
                            }
                            if(board.b[i-6]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-6;
                                begin += 2;
                            }
                            if(board.b[i+10]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+10;
                                begin += 2;
                            }
                            if(board.b[i+17]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+17;
                                begin += 2;
                            }
                        }
                        else if(i%8==1)
                        {
                            if(board.b[i-17]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-17;
                                begin += 2;
                            }
                            if(board.b[i-15]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-15;
                                begin += 2;
                            }
                            if(board.b[i-6]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-6;
                                begin += 2;
                            }
                            if(board.b[i+10]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+10;
                                begin += 2;
                            }
                            if(board.b[i+15]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+15;
                                begin += 2;
                            }
                            if(board.b[i+17]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+17;
                                begin += 2;
                            }
                        }
                        else if(i%8==6)
                        {
                            if(board.b[i-17]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-17;
                                begin += 2;
                            }
                            if(board.b[i-15]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-15;
                                begin += 2;
                            }
                            if(board.b[i-10]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-10;
                                begin += 2;
                            }
                            if(board.b[i+6]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+6;
                                begin += 2;
                            }
                            if(board.b[i+15]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+15;
                                begin += 2;
                            }
                            if(board.b[i+17]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+17;
                                begin += 2;
                            }
                        }
                        else if(i%8==7)
                        {
                            if(board.b[i-17]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-17;
                                begin += 2;
                            }
                            if(board.b[i-10]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-10;
                                begin += 2;
                            }
                            if(board.b[i+6]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+6;
                                begin += 2;
                            }
                            if(board.b[i+15]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+15;
                                begin += 2;
                            }
                        }
                        else
                        {
                            if(board.b[i-17]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-17;
                                begin += 2;
                            }
                            if(board.b[i-15]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-15;
                                begin += 2;
                            }
                            if(board.b[i-10]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-10;
                                begin += 2;
                            }
                            if(board.b[i-6]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-6;
                                begin += 2;
                            }
                            if(board.b[i+6]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+6;
                                begin += 2;
                            }
                            if(board.b[i+10]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+10;
                                begin += 2;
                            }
                            if(board.b[i+15]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+15;
                                begin += 2;
                            }
                            if(board.b[i+17]<=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+17;
                                begin += 2;
                            }
                        }
                    }
                }
                else if(board.b[i]==3 || board.b[i]==5)
                {
                    // bishop
                    
                    // downward and leftward
                    if(i/8<i%8)
                    {
                        dist = i/8;
                    }
                    else
                    {
                        dist = i%8;
                    }
                    for(j=1;j<=dist;j++)
                    {
                        if(board.b[i-9*j]>0)
                        {
                            break;
                        }
                        move[begin] = i;
                        move[begin+1] = i-9*j;
                        begin += 2;
                        if(board.b[i-9*j]<0)
                        {
                            break;
                        }
                    }
                    
                    // downward and rightward
                    if(7-i/8<i%8)
                    {
                        dist = 7-i/8;
                    }
                    else
                    {
                        dist = i%8;
                    }
                    for(j=1;j<=dist;j++)
                    {
                        if(board.b[i+7*j]>0)
                        {
                            break;
                        }
                        move[begin] = i;
                        move[begin+1] = i+7*j;
                        begin += 2;
                        if(board.b[i+7*j]<0)
                        {
                            break;
                        }
                    }
                    
                    // upward and leftward
                    if(i/8<7-i%8)
                    {
                        dist = i/8;
                    }
                    else
                    {
                        dist = 7-i%8;
                    }
                    for(j=1;j<=dist;j++)
                    {
                        if(board.b[i-7*j]>0)
                        {
                            break;
                        }
                        move[begin] = i;
                        move[begin+1] = i-7*j;
                        begin += 2;
                        if(board.b[i-7*j]<0)
                        {
                            break;
                        }
                    }
                    
                    
                    // upward and rightward
                    if(7-i/8<7-i%8)
                    {
                        dist = 7-i/8;
                    }
                    else
                    {
                        dist = 7-i%8;
                    }
                    for(j=1;j<=dist;j++)
                    {
                        if(board.b[i+9*j]>0)
                        {
                            break;
                        }
                        move[begin] = i;
                        move[begin+1] = i+9*j;
                        begin += 2;
                        if(board.b[i+9*j]<0)
                        {
                            break;
                        }
                    }
                }
                else if(board.b[i]==6)
                {
                    // king
                    if(i==0)
                    {
                        if(board.b[1]<=0)
                        {
                            move[begin] = 0;
                            move[begin+1] = 1;
                            begin += 2;
                        }
                        if(board.b[8]<=0)
                        {
                            move[begin] = 0;
                            move[begin+1] = 8;
                            begin += 2;
                        }
                        if(board.b[9]<=0)
                        {
                            move[begin] = 0;
                            move[begin+1] = 9;
                            begin += 2;
                        }
                    }
                    else if(i==7)
                    {
                        if(board.b[6]<=0)
                        {
                            move[begin] = 7;
                            move[begin+1] = 6;
                            begin += 2;
                        }
                        if(board.b[14]<=0)
                        {
                            move[begin] = 7;
                            move[begin+1] = 14;
                            begin += 2;
                        }
                        if(board.b[15]<=0)
                        {
                            move[begin] = 7;
                            move[begin+1] = 15;
                            begin += 2;
                        }
                    }
                    else if(i==56)
                    {
                        if(board.b[48]<=0)
                        {
                            move[begin] = 56;
                            move[begin+1] = 48;
                            begin += 2;
                        }
                        if(board.b[49]<=0)
                        {
                            move[begin] = 56;
                            move[begin+1] = 49;
                            begin += 2;
                        }
                        if(board.b[57]<=0)
                        {
                            move[begin] = 56;
                            move[begin+1] = 57;
                            begin += 2;
                        }
                    }
                    else if(i==63)
                    {
                        if(board.b[54]<=0)
                        {
                            move[begin] = 63;
                            move[begin+1] = 54;
                            begin += 2;
                        }
                        if(board.b[55]<=0)
                        {
                            move[begin] = 63;
                            move[begin+1] = 55;
                            begin += 2;
                        }
                        if(board.b[62]<=0)
                        {
                            move[begin] = 63;
                            move[begin+1] = 62;
                            begin += 2;
                        }
                    }
                    else if(i/8==0)
                    {
                        if(board.b[i-1]<=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i-1;
                            begin += 2;
                        }
                        if(board.b[i+1]<=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i+1;
                            begin += 2;
                        }
                        if(board.b[i+7]<=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i+7;
                            begin += 2;
                        }
                        if(board.b[i+8]<=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i+8;
                            begin += 2;
                        }
                        if(board.b[i+9]<=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i+9;
                            begin += 2;
                        }
                    }
                    else if(i/8==7)
                    {
                        if(board.b[i-1]<=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i-1;
                            begin += 2;
                        }
                        if(board.b[i+1]<=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i+1;
                            begin += 2;
                        }
                        if(board.b[i-7]<=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i-7;
                            begin += 2;
                        }
                        if(board.b[i-8]<=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i-8;
                            begin += 2;
                        }
                        if(board.b[i-9]<=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i-9;
                            begin += 2;
                        }
                    }
                    else if(i%8==0)
                    {
                        if(board.b[i-8]<=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i-8;
                            begin += 2;
                        }
                        if(board.b[i-7]<=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i-7;
                            begin += 2;
                        }
                        if(board.b[i+1]<=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i+1;
                            begin += 2;
                        }
                        if(board.b[i+8]<=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i+8;
                            begin += 2;
                        }
                        if(board.b[i+9]<=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i+9;
                            begin += 2;
                        }
                    }
                    else if(i%8==7)
                    {
                        if(board.b[i+8]<=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i+8;
                            begin += 2;
                        }
                        if(board.b[i+7]<=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i+7;
                            begin += 2;
                        }
                        if(board.b[i-1]<=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i-1;
                            begin += 2;
                        }
                        if(board.b[i-8]<=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i-8;
                            begin += 2;
                        }
                        if(board.b[i-9]<=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i-9;
                            begin += 2;
                        }
                    }
                    else
                    {
                        if(board.b[i-9]<=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i-9;
                            begin += 2;
                        }
                        if(board.b[i-8]<=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i-8;
                            begin += 2;
                        }
                        if(board.b[i-7]<=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i-7;
                            begin += 2;
                        }
                        if(board.b[i-1]<=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i-1;
                            begin += 2;
                        }
                        if(board.b[i+1]<=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i+1;
                            begin += 2;
                        }
                        if(board.b[i+7]<=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i+7;
                            begin += 2;
                        }
                        if(board.b[i+8]<=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i+8;
                            begin += 2;
                        }
                        if(board.b[i+9]<=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i+9;
                            begin += 2;
                        }
                    }
                    
                    // castling
                    if(board.castling[0]==1)
                    {
                        // queen-side
                        if(board.b[15]==0 && board.b[23]==0 && board.b[31]==0)
                        {
                            if(aiInCheck(39)==0 && aiInCheck(31)==0 && aiInCheck(23)==0)
                            {
                                move[begin] = 39;
                                move[begin+1] = 23;
                                begin += 2;
                            }
                        }
                    }
                    if(board.castling[1]==1)
                    {
                        // king-side
                        if(board.b[47]==0 && board.b[55]==0)
                        {
                            if(aiInCheck(39)==0 && aiInCheck(47)==0 && aiInCheck(55)==0)
                            {
                                move[begin] = 39;
                                move[begin+1] = 55;
                                begin += 2;
                            }
                        }
                    }
                }
                if(board.b[i]==4 || board.b[i]==5)
                {
                    // rook
                    
                    // downward
                    dist = i%8;
                    for(j=1;j<=dist;j++)
                    {
                        if(board.b[i-j]>0)
                        {
                            break;
                        }
                        move[begin] = i;
                        move[begin+1] = i-j;
                        begin += 2;
                        if(board.b[i-j]<0)
                        {
                            break;
                        }
                    }
                    
                    // upward
                    dist = 7-i%8;
                    for(j=1;j<=dist;j++)
                    {
                        if(board.b[i+j]>0)
                        {
                            break;
                        }
                        move[begin] = i;
                        move[begin+1] = i+j;
                        begin += 2;
                        if(board.b[i+j]<0)
                        {
                            break;
                        }
                    }
                    
                    // leftward
                    dist = i/8;
                    for(j=1;j<=dist;j++)
                    {
                        if(board.b[i-8*j]>0)
                        {
                            break;
                        }
                        move[begin] = i;
                        move[begin+1] = i-8*j;
                        begin += 2;
                        if(board.b[i-8*j]<0)
                        {
                            break;
                        }
                    }
                    
                    // rightward
                    dist = 7-i/8;
                    for(j=1;j<=dist;j++)
                    {
                        if(board.b[i+8*j]>0)
                        {
                            break;
                        }
                        move[begin] = i;
                        move[begin+1] = i+8*j;
                        begin += 2;
                        if(board.b[i+8*j]<0)
                        {
                            break;
                        }
                    }
                }
            }
        }
        moveLength[layer] = begin;
        
        
        // -2 = no king, -1 = checkmate (lose), 0 = stalemate, 1 = all normal
        int sto;
        int king = -1;
        for(i=0;i<64;i++)
        {
            if(board.b[i]==6)
            {
                king = i;
                break;
            }
        }
        for(i=layer*500;i<moveLength[layer];i+=2)
        {
            if(willMovePutMeInCheck(king, move[i], move[i+1]))
            {
                move[i] = 0;
                move[i+1] = 0;
            }
        }
        for(i=layer*500;i<moveLength[layer];i+=2)
        {
            if(move[i]==move[i+1])
            {
                if(move[i]==0)
                {
                    sto = 0;
                    for(j=moveLength[layer]-2;j>i;j-=2)
                    {
                        if(!(move[j]==0 && move[j+1]==0))
                        {
                            move[i] = move[j];
                            move[i+1] =  move[j+1];
                            move[j] = 0;
                            move[j+1] = 0;
                            sto = 1;
                            break;
                        }
                    }
                    if(sto==0)
                    {
                        moveLength[layer] = i;
                        break;
                    }
                }
            }
        }
    }
    else
    {
        int i;
        int j;
        int dist;
        int begin = 500*layer;
        for(i=0;i<64;i++)
        {
            if(board.b[i]<0)
            {
                if(board.b[i]==-1)
                {
                    // pawn
                    if(board.b[i+1]==0)
                    {
                        // forward one
                        if(i%8!=6)
                        {
                            // no promotion
                            move[begin] = i;
                            move[begin+1] = i+1;
                            begin+=2;
                        }
                        else
                        {
                            // promotion
                            move[begin] = 200+i;
                            move[begin+1] = i+1;
                            begin+=2;
                            move[begin] = 300+i;
                            move[begin+1] = i+1;
                            begin+=2;
                            move[begin] = 400+i;
                            move[begin+1] = i+1;
                            begin+=2;
                            move[begin] = 500+i;
                            move[begin+1] = i+1;
                            begin+=2;
                        }
                        if(i%8==1 && board.b[i+2]==0)
                        {
                            // forward two
                            move[begin] = i;
                            move[begin+1] = i+2;
                            begin += 2;
                        }
                    }
                    if(board.b[i+9]>0 && i/8!=7)
                    {
                        if(i%8!=6)
                        {
                            // no promotion
                            move[begin] = i;
                            move[begin+1] = i+9;
                            begin += 2;
                        }
                        else
                        {
                            // promotion
                            move[begin] = 200+i;
                            move[begin+1] = i+9;
                            begin += 2;
                            move[begin] = 300+i;
                            move[begin+1] = i+9;
                            begin += 2;
                            move[begin] = 400+i;
                            move[begin+1] = i+9;
                            begin += 2;
                            move[begin] = 500+i;
                            move[begin+1] = i+9;
                            begin += 2;
                        }
                    }
                    if(board.b[i-7]>0 && i/8!=0)
                    {
                        if(i%8!=6)
                        {
                            // no promotion
                            move[begin] = i;
                            move[begin+1] = i-7;
                            begin += 2;
                        }
                        else
                        {
                            // promotion
                            move[begin] = 200+i;
                            move[begin+1] = i-7;
                            begin += 2;
                            move[begin] = 300+i;
                            move[begin+1] = i-7;
                            begin += 2;
                            move[begin] = 400+i;
                            move[begin+1] = i-7;
                            begin += 2;
                            move[begin] = 500+i;
                            move[begin+1] = i-7;
                            begin += 2;
                        }
                    }
                    
                    // en passant
                    if(i/8!=7)
                    {
                        if(board.b[i+8]==1)
                        {
                            if(board.lastMove[0]-board.lastMove[1]==2 &&  board.lastMove[1]==i+8 && board.b[i+9]==0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+9;
                                begin += 2;
                            }
                        }
                    }
                    if(i/8!=0)
                    {
                        if(board.b[i-8]==1)
                        {
                            if(board.lastMove[0]-board.lastMove[1]==2 &&  board.lastMove[1]==i-8 && board.b[i-7]==0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-7;
                                begin += 2;
                            }
                        }
                    }
                }
                else if(board.b[i]==-2)
                {
                    // knight
                    if(i/8==0)
                    {
                        if(i%8==0)
                        {
                            if(board.b[10]>=0)
                            {
                                move[begin] = 0;
                                move[begin+1] = 10;
                                begin += 2;
                            }
                            if(board.b[17]>=0)
                            {
                                move[begin] = 0;
                                move[begin+1] = 17;
                                begin += 2;
                            }
                        }
                        else if(i%8==1)
                        {
                            if(board.b[11]>=0)
                            {
                                move[begin] = 1;
                                move[begin+1] = 11;
                                begin += 2;
                            }
                            if(board.b[16]>=0)
                            {
                                move[begin] = 1;
                                move[begin+1] = 16;
                                begin += 2;
                            }
                            if(board.b[18]>=0)
                            {
                                move[begin] = 1;
                                move[begin+1] = 18;
                                begin += 2;
                            }
                        }
                        else if(i%8==6)
                        {
                            if(board.b[12]>=0)
                            {
                                move[begin] = 6;
                                move[begin+1] = 12;
                                begin += 2;
                            }
                            if(board.b[21]>=0)
                            {
                                move[begin] = 6;
                                move[begin+1] = 21;
                                begin += 2;
                            }
                            if(board.b[23]>=0)
                            {
                                move[begin] = 6;
                                move[begin+1] = 23;
                                begin += 2;
                            }
                        }
                        else if(i%8==7)
                        {
                            if(board.b[13]>=0)
                            {
                                move[begin] = 7;
                                move[begin+1] = 13;
                                begin += 2;
                            }
                            if(board.b[22]>=0)
                            {
                                move[begin] = 7;
                                move[begin+1] = 22;
                                begin += 2;
                            }
                        }
                        else
                        {
                            if(board.b[i+6]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+6;
                                begin += 2;
                            }
                            if(board.b[i+10]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+10;
                                begin += 2;
                            }
                            if(board.b[i+15]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+15;
                                begin += 2;
                            }
                            if(board.b[i+17]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+17;
                                begin += 2;
                            }
                        }
                    }
                    else if(i/8==1)
                    {
                        if(i%8==0)
                        {
                            if(board.b[2]>=0)
                            {
                                move[begin] = 8;
                                move[begin+1] = 2;
                                begin += 2;
                            }
                            if(board.b[18]>=0)
                            {
                                move[begin] = 8;
                                move[begin+1] = 18;
                                begin += 2;
                            }
                            if(board.b[25]>=0)
                            {
                                move[begin] = 8;
                                move[begin+1] = 25;
                                begin += 2;
                            }
                        }
                        else if(i%8==1)
                        {
                            if(board.b[3]>=0)
                            {
                                move[begin] = 9;
                                move[begin+1] = 3;
                                begin += 2;
                            }
                            if(board.b[19]>=0)
                            {
                                move[begin] = 9;
                                move[begin+1] = 19;
                                begin += 2;
                            }
                            if(board.b[24]>=0)
                            {
                                move[begin] = 9;
                                move[begin+1] = 24;
                                begin += 2;
                            }
                            if(board.b[26]>=0)
                            {
                                move[begin] = 9;
                                move[begin+1] = 26;
                                begin += 2;
                            }
                        }
                        else if(i%8==6)
                        {
                            if(board.b[4]>=0)
                            {
                                move[begin] = 14;
                                move[begin+1] = 4;
                                begin += 2;
                            }
                            if(board.b[20]>=0)
                            {
                                move[begin] = 14;
                                move[begin+1] = 20;
                                begin += 2;
                            }
                            if(board.b[29]>=0)
                            {
                                move[begin] = 14;
                                move[begin+1] = 29;
                                begin += 2;
                            }
                            if(board.b[31]>=0)
                            {
                                move[begin] = 14;
                                move[begin+1] = 31;
                                begin += 2;
                            }
                        }
                        else if(i%8==7)
                        {
                            if(board.b[5]>=0)
                            {
                                move[begin] = 15;
                                move[begin+1] = 5;
                                begin += 2;;
                            }
                            if(board.b[21]>=0)
                            {
                                move[begin] = 15;
                                move[begin+1] = 21;
                                begin += 2;;
                            }
                            if(board.b[30]>=0)
                            {
                                move[begin] = 15;
                                move[begin+1] = 30;
                                begin += 2;;
                            }
                        }
                        else
                        {
                            if(board.b[i-10]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-10;
                                begin += 2;
                            }
                            if(board.b[i-6]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-6;
                                begin += 2;
                            }
                            if(board.b[i+6]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+6;
                                begin += 2;
                            }
                            if(board.b[i+10]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+10;
                                begin += 2;
                            }
                            if(board.b[i+15]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+15;
                                begin += 2;
                            }
                            if(board.b[i+17]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+17;
                                begin += 2;
                            }
                        }
                    }
                    else if(i/8==6)
                    {
                        if(i%8==0)
                        {
                            if(board.b[33]>=0)
                            {
                                move[begin] = 48;
                                move[begin+1] = 33;
                                begin += 2;
                            }
                            if(board.b[42]>=0)
                            {
                                move[begin] = 48;
                                move[begin+1] = 42;
                                begin += 2;
                            }
                            if(board.b[58]>=0)
                            {
                                move[begin] = 48;
                                move[begin+1] = 58;
                                begin += 2;
                            }
                        }
                        else if(i%8==1)
                        {
                            if(board.b[32]>=0)
                            {
                                move[begin] = 49;
                                move[begin+1] = 32;
                                begin += 2;
                            }
                            if(board.b[34]>=0)
                            {
                                move[begin] = 49;
                                move[begin+1] = 34;
                                begin += 2;
                            }
                            if(board.b[43]>=0)
                            {
                                move[begin] = 49;
                                move[begin+1] = 43;
                                begin += 2;
                            }
                            if(board.b[50]>=0)
                            {
                                move[begin] = 49;
                                move[begin+1] = 59;
                                begin += 2;
                            }
                        }
                        else if(i%8==6)
                        {
                            if(board.b[37]>=0)
                            {
                                move[begin] = 54;
                                move[begin+1] = 37;
                                begin += 2;
                            }
                            if(board.b[39]>=0)
                            {
                                move[begin] = 54;
                                move[begin+1] = 39;
                                begin += 2;
                            }
                            if(board.b[44]>=0)
                            {
                                move[begin] = 54;
                                move[begin+1] = 4;
                                begin += 2;
                            }
                            if(board.b[60]>=0)
                            {
                                move[begin] = 54;
                                move[begin+1] = 60;
                                begin += 2;
                            }
                        }
                        else if(i%8==7)
                        {
                            if(board.b[38]>=0)
                            {
                                move[begin] = 55;
                                move[begin+1] = 38;
                                begin += 2;
                            }
                            if(board.b[45]>=0)
                            {
                                move[begin] = 55;
                                move[begin+1] = 45;
                                begin += 2;
                            }
                            if(board.b[61]>=0)
                            {
                                move[begin] = 55;
                                move[begin+1] = 61;
                                begin += 2;
                            }
                        }
                        else
                        {
                            if(board.b[i-17]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-17;
                                begin += 2;
                            }
                            if(board.b[i-15]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-15;
                                begin += 2;
                            }
                            if(board.b[i-10]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-10;
                                begin += 2;
                            }
                            if(board.b[i-6]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-6;
                                begin += 2;
                            }
                            if(board.b[i+6]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+6;
                                begin += 2;
                            }
                            if(board.b[i+10]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+10;
                                begin += 2;
                            }
                        }
                    }
                    else if(i/8==7)
                    {
                        if(i%8==0)
                        {
                            if(board.b[41]>=0)
                            {
                                move[begin] = 56;
                                move[begin+1] = 41;
                                begin += 2;
                            }
                            if(board.b[50]>=0)
                            {
                                move[begin] = 56;
                                move[begin+1] = 50;
                                begin += 2;
                            }
                        }
                        else if(i%8==1)
                        {
                            if(board.b[40]>=0)
                            {
                                move[begin] = 57;
                                move[begin+1] = 40;
                                begin += 2;
                            }
                            if(board.b[42]>=0)
                            {
                                move[begin] = 57;
                                move[begin+1] = 42;
                                begin += 2;
                            }
                            if(board.b[51]>=0)
                            {
                                move[begin] = 57;
                                move[begin+1] = 51;
                                begin += 2;
                            }
                        }
                        else if(i%8==6)
                        {
                            if(board.b[45]>=0)
                            {
                                move[begin] = 62;
                                move[begin+1] = 45;
                                begin += 2;
                            }
                            if(board.b[47]>=0)
                            {
                                move[begin] = 62;
                                move[begin+1] = 47;
                                begin += 2;
                            }
                            if(board.b[52]>=0)
                            {
                                move[begin] = 62;
                                move[begin+1] = 52;
                                begin += 2;
                            }
                        }
                        else if(i%8==7)
                        {
                            if(board.b[46]>=0)
                            {
                                move[begin] = 63;
                                move[begin+1] = 46;
                                begin += 2;
                            }
                            if(board.b[53]>=0)
                            {
                                move[begin] = 63;
                                move[begin+1] = 53;
                                begin += 2;
                            }
                        }
                        else
                        {
                            if(board.b[i-17]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-17;
                                begin += 2;
                            }
                            if(board.b[i-15]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-15;
                                begin += 2;
                            }
                            if(board.b[i-10]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-10;
                                begin += 2;
                            }
                            if(board.b[i-6]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-6;
                                begin += 2;
                            }
                        }
                    }
                    else
                    {
                        if(i%8==0)
                        {
                            if(board.b[i-15]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-15;
                                begin += 2;
                            }
                            if(board.b[i-6]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-6;
                                begin += 2;
                            }
                            if(board.b[i+17]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+17;
                                begin += 2;
                            }
                        }
                        else if(i%8==1)
                        {
                            if(board.b[i-17]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-17;
                                begin += 2;
                            }
                            if(board.b[i-15]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-15;
                                begin += 2;
                            }
                            if(board.b[i-6]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-6;
                                begin += 2;
                            }
                            if(board.b[i+10]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+10;
                                begin += 2;
                            }
                            if(board.b[i+15]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+15;
                                begin += 2;
                            }
                            if(board.b[i+17]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+17;
                                begin += 2;
                            }
                        }
                        else if(i%8==6)
                        {
                            if(board.b[i-17]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-17;
                                begin += 2;
                            }
                            if(board.b[i-15]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-15;
                                begin += 2;
                            }
                            if(board.b[i-10]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-10;
                                begin += 2;
                            }
                            if(board.b[i+6]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+6;
                                begin += 2;
                            }
                            if(board.b[i+15]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+15;
                                begin += 2;
                            }
                            if(board.b[i+17]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+17;
                                begin += 2;
                            }
                        }
                        else if(i%8==7)
                        {
                            if(board.b[i-17]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-17;
                                begin += 2;
                            }
                            if(board.b[i-10]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-10;
                                begin += 2;
                            }
                            if(board.b[i+6]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+6;
                                begin += 2;
                            }
                            if(board.b[i+15]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+15;
                                begin += 2;
                            }
                        }
                        else
                        {
                            if(board.b[i-17]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-17;
                                begin += 2;
                            }
                            if(board.b[i-15]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-15;
                                begin += 2;
                            }
                            if(board.b[i-10]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-10;
                                begin += 2;
                            }
                            if(board.b[i-6]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i-6;
                                begin += 2;
                            }
                            if(board.b[i+6]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+6;
                                begin += 2;
                            }
                            if(board.b[i+10]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+10;
                                begin += 2;
                            }
                            if(board.b[i+15]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+15;
                                begin += 2;
                            }
                            if(board.b[i+17]>=0)
                            {
                                move[begin] = i;
                                move[begin+1] = i+17;
                                begin += 2;
                            }
                        }
                    }
                }
                else if(board.b[i]==-3 || board.b[i]==-5)
                {
                    // bishop
                    
                    // downward and leftward
                    if(i/8<i%8)
                    {
                        dist = i/8;
                    }
                    else
                    {
                        dist = i%8;
                    }
                    for(j=1;j<=dist;j++)
                    {
                        if(board.b[i-9*j]<0)
                        {
                            break;
                        }
                        move[begin] = i;
                        move[begin+1] = i-9*j;
                        begin += 2;
                        if(board.b[i-9*j]>0)
                        {
                            break;
                        }
                    }
                    
                    // downward and rightward
                    if(7-i/8<i%8)
                    {
                        dist = 7-i/8;
                    }
                    else
                    {
                        dist = i%8;
                    }
                    for(j=1;j<=dist;j++)
                    {
                        if(board.b[i+7*j]<0)
                        {
                            break;
                        }
                        move[begin] = i;
                        move[begin+1] = i+7*j;
                        begin += 2;
                        if(board.b[i+7*j]>0)
                        {
                            break;
                        }
                    }
                    
                    // upward and leftward
                    if(i/8<7-i%8)
                    {
                        dist = i/8;
                    }
                    else
                    {
                        dist = 7-i%8;
                    }
                    for(j=1;j<=dist;j++)
                    {
                        if(board.b[i-7*j]<0)
                        {
                            break;
                        }
                        move[begin] = i;
                        move[begin+1] = i-7*j;
                        begin += 2;
                        if(board.b[i-7*j]>0)
                        {
                            break;
                        }
                    }
                    
                    
                    // upward and rightward
                    if(7-i/8<7-i%8)
                    {
                        dist = 7-i/8;
                    }
                    else
                    {
                        dist = 7-i%8;
                    }
                    for(j=1;j<=dist;j++)
                    {
                        if(board.b[i+9*j]<0)
                        {
                            break;
                        }
                        move[begin] = i;
                        move[begin+1] = i+9*j;
                        begin += 2;
                        if(board.b[i+9*j]>0)
                        {
                            break;
                        }
                    }
                }
                else if(board.b[i]==-6)
                {
                    // king
                    if(i==0)
                    {
                        if(board.b[1]>=0)
                        {
                            move[begin] = 0;
                            move[begin+1] = 1;
                            begin += 2;
                        }
                        if(board.b[8]>=0)
                        {
                            move[begin] = 0;
                            move[begin+1] = 8;
                            begin += 2;
                        }
                        if(board.b[9]>=0)
                        {
                            move[begin] = 0;
                            move[begin+1] = 9;
                            begin += 2;
                        }
                    }
                    else if(i==7)
                    {
                        if(board.b[6]>=0)
                        {
                            move[begin] = 7;
                            move[begin+1] = 6;
                            begin += 2;
                        }
                        if(board.b[14]>=0)
                        {
                            move[begin] = 7;
                            move[begin+1] = 14;
                            begin += 2;
                        }
                        if(board.b[15]>=0)
                        {
                            move[begin] = 7;
                            move[begin+1] = 15;
                            begin += 2;
                        }
                    }
                    else if(i==56)
                    {
                        if(board.b[48]>=0)
                        {
                            move[begin] = 56;
                            move[begin+1] = 48;
                            begin += 2;
                        }
                        if(board.b[49]>=0)
                        {
                            move[begin] = 56;
                            move[begin+1] = 49;
                            begin += 2;
                        }
                        if(board.b[57]>=0)
                        {
                            move[begin] = 56;
                            move[begin+1] = 57;
                            begin += 2;
                        }
                    }
                    else if(i==63)
                    {
                        if(board.b[54]>=0)
                        {
                            move[begin] = 63;
                            move[begin+1] = 54;
                            begin += 2;
                        }
                        if(board.b[55]>=0)
                        {
                            move[begin] = 63;
                            move[begin+1] = 55;
                            begin += 2;
                        }
                        if(board.b[62]>=0)
                        {
                            move[begin] = 63;
                            move[begin+1] = 62;
                            begin += 2;
                        }
                    }
                    else if(i/8==0)
                    {
                        if(board.b[i-1]>=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i-1;
                            begin += 2;
                        }
                        if(board.b[i+1]>=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i+1;
                            begin += 2;
                        }
                        if(board.b[i+7]>=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i+7;
                            begin += 2;
                        }
                        if(board.b[i+8]>=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i+8;
                            begin += 2;
                        }
                        if(board.b[i+9]>=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i+9;
                            begin += 2;
                        }
                    }
                    else if(i/8==7)
                    {
                        if(board.b[i-1]>=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i-1;
                            begin += 2;
                        }
                        if(board.b[i+1]>=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i+1;
                            begin += 2;
                        }
                        if(board.b[i-7]>=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i-7;
                            begin += 2;
                        }
                        if(board.b[i-8]>=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i-8;
                            begin += 2;
                        }
                        if(board.b[i-9]>=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i-9;
                            begin += 2;
                        }
                    }
                    else if(i%8==0)
                    {
                        if(board.b[i-8]>=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i-8;
                            begin += 2;
                        }
                        if(board.b[i-7]>=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i-7;
                            begin += 2;
                        }
                        if(board.b[i+1]>=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i+1;
                            begin += 2;
                        }
                        if(board.b[i+8]>=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i+8;
                            begin += 2;
                        }
                        if(board.b[i+9]>=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i+9;
                            begin += 2;
                        }
                    }
                    else if(i%8==7)
                    {
                        if(board.b[i+8]>=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i+8;
                            begin += 2;
                        }
                        if(board.b[i+7]>=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i+7;
                            begin += 2;
                        }
                        if(board.b[i-1]>=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i-1;
                            begin += 2;
                        }
                        if(board.b[i-8]>=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i-8;
                            begin += 2;
                        }
                        if(board.b[i-9]>=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i-9;
                            begin += 2;
                        }
                    }
                    else
                    {
                        if(board.b[i-9]>=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i-9;
                            begin += 2;
                        }
                        if(board.b[i-8]>=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i-8;
                            begin += 2;
                        }
                        if(board.b[i-7]>=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i-7;
                            begin += 2;
                        }
                        if(board.b[i-1]>=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i-1;
                            begin += 2;
                        }
                        if(board.b[i+1]>=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i+1;
                            begin += 2;
                        }
                        if(board.b[i+7]>=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i+7;
                            begin += 2;
                        }
                        if(board.b[i+8]>=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i+8;
                            begin += 2;
                        }
                        if(board.b[i+9]>=0)
                        {
                            move[begin] = i;
                            move[begin+1] = i+9;
                            begin += 2;
                        }
                    }
                    
                    // castling
                    if(board.castling[2]==1)
                    {
                        // queen-side
                        if(board.b[8]==0 && board.b[16]==0 && board.b[24]==0)
                        {
                            if(humanInCheck(32)==0 && humanInCheck(24)==0 && humanInCheck(16)==0)
                            {
                                move[begin] = 32;
                                move[begin+1] = 16;
                                begin += 2;
                            }
                        }
                    }
                    if(board.castling[3]==1)
                    {
                        // king-side
                        if(board.b[40]==0 && board.b[48]==0)
                        {
                            if(humanInCheck(32)==0 && humanInCheck(40)==0 && humanInCheck(48)==0)
                            {
                                move[begin] = 32;
                                move[begin+1] = 48;
                                begin += 2;
                            }
                        }
                    }
                }
                if(board.b[i]==-4 || board.b[i]==-5)
                {
                    // rook
                    
                    // downward
                    dist = i%8;
                    for(j=1;j<=dist;j++)
                    {
                        if(board.b[i-j]<0)
                        {
                            break;
                        }
                        move[begin] = i;
                        move[begin+1] = i-j;
                        begin += 2;
                        if(board.b[i-j]>0)
                        {
                            break;
                        }
                    }
                    
                    // upward
                    dist = 7-i%8;
                    for(j=1;j<=dist;j++)
                    {
                        if(board.b[i+j]<0)
                        {
                            break;
                        }
                        move[begin] = i;
                        move[begin+1] = i+j;
                        begin += 2;
                        if(board.b[i+j]>0)
                        {
                            break;
                        }
                    }
                    
                    // leftward
                    dist = i/8;
                    for(j=1;j<=dist;j++)
                    {
                        if(board.b[i-8*j]<0)
                        {
                            break;
                        }
                        move[begin] = i;
                        move[begin+1] = i-8*j;
                        begin += 2;
                        if(board.b[i-8*j]>0)
                        {
                            break;
                        }
                    }
                    
                    // rightward
                    dist = 7-i/8;
                    for(j=1;j<=dist;j++)
                    {
                        if(board.b[i+8*j]<0)
                        {
                            break;
                        }
                        move[begin] = i;
                        move[begin+1] = i+8*j;
                        begin += 2;
                        if(board.b[i+8*j]>0)
                        {
                            break;
                        }
                    }
                }
            }
        }
        moveLength[layer] = begin;
        
        
        // -2 = no king, -1 = checkmate (lose), 0 = stalemate, 1 = normal
        int sto;
        int king = -1;
        for(i=0;i<64;i++)
        {
            if(board.b[i]==-6)
            {
                king = i;
                break;
            }
        }
        for(i=layer*500;i<moveLength[layer];i+=2)
        {
            if(willMovePutMeInCheck(king, move[i], move[i+1]))
            {
                move[i] = 0;
                move[i+1] = 0;
            }
        }
        for(i=layer*500;i<moveLength[layer];i+=2)
        {
            if(move[i]==move[i+1])
            {
                if(move[i]==0)
                {
                    sto = 0;
                    for(j=moveLength[layer]-2;j>i;j-=2)
                    {
                        if(!(move[j]==0 && move[j+1]==0))
                        {
                            move[i] = move[j];
                            move[i+1] =  move[j+1];
                            move[j] = 0;
                            move[j+1] = 0;
                            sto = 1;
                            break;
                        }
                    }
                    if(sto==0)
                    {
                        moveLength[layer] = i;
                        break;
                    }
                }
            }
        }
    }
}

bool MoveConverter::willMovePutMeInCheck(int king, int from, int to)
{
    int sto, stoB;
    int answer;
    if(board.b[king]==6)
    {
        // cpu
        if(from>=100)
        {
            // promotion
            sto = board.b[to];
            board.b[to] = (int) (from/100);
            board.b[from%100] = 0;
            if(sto==0)
            {
                board.fiftyMoveDrawCounter++;
            }
            else
            {
                stoB = board.fiftyMoveDrawCounter;
                board.fiftyMoveDrawCounter = 0;
            }
            
            int sto0 = board.castling[0];
            int sto1 = board.castling[1];
            int sto2 = board.castling[2];
            int sto3 = board.castling[3];
            if(board.b[to]==-4)
            {
                if(to==0)
                {
                    board.castling[2] = 0;
                }
                else if(to==56)
                {
                    board.castling[3] = 0;
                }
            }
            
            answer = aiInCheck(king);
            
            board.castling[0] = sto0;
            board.castling[1] = sto1;
            board.castling[2] = sto2;
            board.castling[3] = sto3;
            
            if(sto==0)
            {
                board.fiftyMoveDrawCounter--;
            }
            else
            {
                board.fiftyMoveDrawCounter = stoB;
            }
            board.fiftyMoveDrawCounter--;
            board.b[to] = sto;
            board.b[from%100] = 1;
        }
        else if(board.b[from]==6 && (to-from==16 || from-to==16))
        {
            int sto0 = board.castling[0];
            int sto1 = board.castling[1];
            board.castling[0] = 0;
            board.castling[1] = 0;
            board.fiftyMoveDrawCounter++;
            // castle
            if(to==23)
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
            
            answer = aiInCheck(to);
            
            if(to==23)
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
        else if(board.b[from]==1 && board.b[to]==0 && (to-from==7 || to-from==-9))
        {
            // En passant
            board.b[to+1] = 0;
            board.b[to] = 1;
            board.b[from] = 0;
            stoB = board.fiftyMoveDrawCounter;
            board.fiftyMoveDrawCounter = 0;
            
            answer = aiInCheck(king);
            
            board.fiftyMoveDrawCounter = stoB;
            board.b[to+1] = -1;
            board.b[to] = 0;
            board.b[from] = 1;
        }
        else
        {
            int sto0 = board.castling[0];
            int sto1 = board.castling[1];
            int sto2 = board.castling[2];
            int sto3 = board.castling[3];
            if(board.b[from]==6)
            {
                board.castling[0] = 0;
                board.castling[1] = 0;
            }
            else if(board.b[from]==4)
            {
                if(from==7)
                {
                    board.castling[0] = 0;
                }
                else if(from==63)
                {
                    board.castling[1] = 0;
                }
            }
            if(board.b[to]==-4)
            {
                if(to==0)
                {
                    board.castling[2] = 0;
                }
                else if(to==56)
                {
                    board.castling[3] = 0;
                }
            }
            
            
            sto = board.b[to];
            board.b[to] = board.b[from];
            board.b[from] = 0;
            
            if(sto==0)
            {
                board.fiftyMoveDrawCounter++;
            }
            else
            {
                stoB = board.fiftyMoveDrawCounter;
                board.fiftyMoveDrawCounter = 0;
            }
            
            if(board.b[to]==6)
            {
                answer = aiInCheck(to);
            }
            else
            {
                answer = aiInCheck(king);
            }
            
            if(sto==0)
            {
                board.fiftyMoveDrawCounter--;
            }
            else
            {
                board.fiftyMoveDrawCounter = stoB;
            }
            board.castling[0] = sto0;
            board.castling[1] = sto1;
            board.castling[2] = sto2;
            board.castling[3] = sto3;
            board.b[from] = board.b[to];
            board.b[to] = sto;
        }
    }
    else
    {
        // human
        if(from>=100)
        {
            // promotion
            sto = board.b[to];
            board.b[to] = (int) (from/100);
            board.b[from%100] = 0;
            board.fiftyMoveDrawCounter++;
            
            int sto0 = board.castling[0];
            int sto1 = board.castling[1];
            int sto2 = board.castling[2];
            int sto3 = board.castling[3];
            if(board.b[to]==4)
            {
                if(to==7)
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
            board.b[to] = sto;
            board.b[from%100] = -1;
        }
        else if(board.b[from]==-6 && (to-from==16 || from-to==16))
        {
            int sto0 = board.castling[2];
            int sto1 = board.castling[3];
            board.castling[2] = 0;
            board.castling[3] = 0;
            // castling
            if(to==16)
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
            
            answer = humanInCheck(to);
            
            board.fiftyMoveDrawCounter--;
            if(to==16)
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
        else if(board.b[from]==-1 && board.b[to]==0 && (to-from==9 || to-from==-7))
        {
            // En passant
            board.b[to-1] = 0;
            board.b[to] = -1;
            board.b[from] = 0;
            stoB = board.fiftyMoveDrawCounter;
            board.fiftyMoveDrawCounter = 0;
            
            answer = humanInCheck(king);
            
            board.fiftyMoveDrawCounter = stoB;
            board.b[to-1] = 1;
            board.b[to] = 0;
            board.b[from] = -1;
        }
        else
        {
            int sto0 = board.castling[0];
            int sto1 = board.castling[1];
            int sto2 = board.castling[2];
            int sto3 = board.castling[3];
            if(board.b[from]==-6)
            {
                board.castling[2] = 0;
                board.castling[3] = 0;
            }
            else if(board.b[from]==-4)
            {
                if(from==0)
                {
                    board.castling[2] = 0;
                }
                else if(from==56)
                {
                    board.castling[3] = 0;
                }
            }
            if(board.b[to]==4)
            {
                if(to==7)
                {
                    board.castling[0] = 0;
                }
                else
                {
                    board.castling[1] = 0;
                }
            }
            
            sto = board.b[to];
            board.b[to] = board.b[from];
            board.b[from] = 0;
            if(sto==0)
            {
                board.fiftyMoveDrawCounter++;
            }
            else
            {
                stoB = board.fiftyMoveDrawCounter;
                board.fiftyMoveDrawCounter = 0;
            }
            
            if(board.b[to]==-6)
            {
                answer = humanInCheck(to);
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
            board.b[from] = board.b[to];
            board.b[to] = sto;
            board.castling[0] = sto0;
            board.castling[1] = sto1;
            board.castling[2] = sto2;
            board.castling[3] = sto3;
        }
    }
    return answer;
}

Board MoveConverter::convertFenToBoard(std::string str, bool aiWhite)
{
    Board board;
    std::vector < std::string > splitString;
    int lastSplit = -1;
    for(int i=0; i<str.size(); i++)
	{
        if(str[i] == '/')
        {
            splitString.push_back(str.substr(lastSplit+1, i-lastSplit-1));
            lastSplit = i;
        }
	}
    splitString.push_back(str.substr(lastSplit+1));
    
    
    if(aiWhite)
    {
        for(int rowNum=0; rowNum<splitString.size(); rowNum++)
        {
            int x = 0;
            for(int i=0; i<splitString.size(); i++)
            {
                int q = splitString[rowNum][i] - '0';
                if(q>=0 && q<=9)
                {
                    x += q-1;
                }
                else if(splitString[rowNum][i] == 'p')
                {
                    board.b[8*x+rowNum] = -1;
                }
                else if(splitString[rowNum][i] == 'n')
                {
                    board.b[8*x+rowNum] = -2;
                }
                else if(splitString[rowNum][i] == 'b')
                {
                    board.b[8*x+rowNum] = -3;
                }
                else if(splitString[rowNum][i] == 'r')
                {
                    board.b[8*x+rowNum] = -4;
                }
                else if(splitString[rowNum][i] == 'q')
                {
                    board.b[8*x+rowNum] = -5;
                }
                else if(splitString[rowNum][i] == 'k')
                {
                    board.b[8*x+rowNum] = -6;
                }
                else if(splitString[rowNum][i] == 'P')
                {
                    board.b[8*x+rowNum] = 1;
                }
                else if(splitString[rowNum][i] == 'N')
                {
                    board.b[8*x+rowNum] = 2;
                }
                else if(splitString[rowNum][i] == 'B')
                {
                    board.b[8*x+rowNum] = 3;
                }
                else if(splitString[rowNum][i] == 'R')
                {
                    board.b[8*x+rowNum] = 4;
                }
                else if(splitString[rowNum][i] == 'Q')
                {
                    board.b[8*x+rowNum] = 5;
                }
                else if(splitString[rowNum][i] == 'K')
                {
                    board.b[8*x+rowNum] = 6;
                }
                x++;
            }
        }
    }
    else
    {
        for(int rowNum=0; rowNum<splitString.size(); rowNum++)
        {
            int x = 0;
            for(int i=0; i<splitString.size(); i++)
            {
                int q = splitString[rowNum][i] - '0';
                if(q>=0 && q<=9)
                {
                    x += q-1;
                }
                else if(splitString[rowNum][i] == 'p')
                {
                    board.b[8*x+7-rowNum] = 1;
                }
                else if(splitString[rowNum][i] == 'n')
                {
                    board.b[8*x+7-rowNum] = 2;
                }
                else if(splitString[rowNum][i] == 'b')
                {
                    board.b[8*x+7-rowNum] = 3;
                }
                else if(splitString[rowNum][i] == 'r')
                {
                    board.b[8*x+7-rowNum] = 4;
                }
                else if(splitString[rowNum][i] == 'q')
                {
                    board.b[8*x+7-rowNum] = 5;
                }
                else if(splitString[rowNum][i] == 'k')
                {
                    board.b[8*x+7-rowNum] = 6;
                }
                else if(splitString[rowNum][i] == 'P')
                {
                    board.b[8*x+7-rowNum] = -1;
                }
                else if(splitString[rowNum][i] == 'N')
                {
                    board.b[8*x+7-rowNum] = -2;
                }
                else if(splitString[rowNum][i] == 'B')
                {
                    board.b[8*x+7-rowNum] = -3;
                }
                else if(splitString[rowNum][i] == 'R')
                {
                    board.b[8*x+7-rowNum] = -4;
                }
                else if(splitString[rowNum][i] == 'Q')
                {
                    board.b[8*x+7-rowNum] = -5;
                }
                else if(splitString[rowNum][i] == 'K')
                {
                    board.b[8*x+7-rowNum] = -6;
                }
                x++;
            }
        }
    }
    
    
    return board;
}