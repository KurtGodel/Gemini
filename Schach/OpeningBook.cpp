//
//  OpeningBook.cpp
//  Schach
//
//  Created by Thomas Redding on 7/2/14.
//  Copyright (c) 2014 Thomas Redding. All rights reserved.
//

#include "OpeningBook.h"

OpeningBook::OpeningBook()
{
    int arr[] = {
        -4, -1, 0,  0,  0,  0,  1,  4,
        -2, -1, 0,  0,  0,  0,  1,  2,
        -3, -1, 0,  0,  0,  0,  1,  3,
        -5, -1, 0,  0,  0,  0,  1,  5,
        -6, -1, 0,  0,  0,  0,  1,  6,
        -3, -1, 0,  0,  0,  0,  1,  3,
        -2, -1, 0,  0,  0,  0,  1,  2,
        -4, -1, 0,  0,  0,  0,  1,  4,
        1,  1,  1,  1,  7,  7,  0
    };
    
    for(int i = 0; i < 64; i++)
    {
        board.b[i] = arr[i];
    }
    if(arr[64]==1)
    {
        board.castling[0] = true;
    }
    else
    {
        board.castling[0] = false;
    }
    if(arr[65]==1)
    {
        board.castling[1] = true;
    }
    else
    {
        board.castling[1] = false;
    }
    if(arr[66]==1)
    {
        board.castling[2] = true;
    }
    else
    {
        board.castling[2] = false;
    }
    if(arr[67]==1)
    {
        board.castling[3] = true;
    }
    else
    {
        board.castling[3] = false;
    }
    board.lastMove[0] = arr[68];
    board.lastMove[1] = arr[69];
    board.fiftyMoveDrawCounter = arr[70];
    
    
    std::string line;
    std::ifstream myfile (resourcePath()+"_Opening Book.txt");
    std::string str = "";
    if (myfile.is_open())
    {
        while(getline(myfile,line))
        {
            str += line + "\n";
        }
        myfile.close();
    }
    else
    {
        std::cout << "(Unable to Open _Opening Table.txt)\n";
    }
    
    if(str.length()!=0)
    {
        /*
        str.erase(remove(str.begin(), str.end(), '\t'), str.end());
        str.erase(remove(str.begin(), str.end(), ' '), str.end());
        root(str, 0);
        */
        loadOpeningBook(str);
    }
    else
    {
        std::cout << "(_Opening Table.txt is Empty)";
    }
}

OpeningMove OpeningBook::search(Board* b)
{
    uint64_t index = b->getZorbistHash();
    
    std::unordered_map<uint64_t, OpeningMove>::iterator it = table.find(index);
    OpeningMove move;
    if(it == table.end())
    {
        // position not in table
        return move;
    }
    else
    {
        // position in table
        move.zobristKey = it->second.zobristKey;
        for(int i=0; i<it->second.move.size(); i++)
        {
            move.move.push_back(it->second.move[i]);
        }
        return move;
    }
}

void OpeningBook::root(std::string str, int layer)
{
    int last = -1;
    int parLevel = 0;
    if(str[0]=='(')
    {
        for(int i=0; i<str.length(); i++)
        {
            if(str[i]=='(')
            {
                parLevel++;
            }
            else if(str[i]==')')
            {
                parLevel--;
            }
            if(parLevel==0)
            {
                makeMove(str.substr(last+2, i-last-2), layer);
                last = i;
            }
        }
    }
    else
    {
        // error
    }
}

void OpeningBook::tree(std::string str, int layer)
{
    switchSides();
    int last = -1;
    int parLevel = 0;
    if(str[0]=='(')
    {
        for(int i=0; i<str.length(); i++)
        {
            if(str[i]=='(')
            {
                parLevel++;
            }
            else if(str[i]==')')
            {
                parLevel--;
            }
            if(parLevel==0)
            {
                std::string x = str.substr(last+2, i-last-2);
                makeMove(str.substr(last+2, i-last-2), layer);
                last = i;
            }
        }
    }
    else
    {
        // reached bottom
    }
    switchSides();
}

int OpeningBook::letterToNumber(char c)
{
    if(c=='a')
    {
        return 0;
    }
    else if(c=='b')
    {
        return 1;
    }
    else if(c=='c')
    {
        return 2;
    }
    else if(c=='d')
    {
        return 3;
    }
    else if(c=='e')
    {
        return 4;
    }
    else if(c=='f')
    {
        return 5;
    }
    else if(c=='g')
    {
        return 6;
    }
    else if(c=='h')
    {
        return 7;
    }
    else if(c=='1')
    {
        return 0;
    }
    else if(c=='2')
    {
        return 1;
    }
    else if(c=='3')
    {
        return 2;
    }
    else if(c=='4')
    {
        return 3;
    }
    else if(c=='5')
    {
        return 4;
    }
    else if(c=='6')
    {
        return 5;
    }
    else if(c=='7')
    {
        return 6;
    }
    else if(c=='8')
    {
        return 7;
    }
    else if(c=='N')
    {
        return 2;
    }
    else if(c=='B')
    {
        return 3;
    }
    else if(c=='R')
    {
        return 4;
    }
    else if(c=='Q')
    {
        return 5;
    }
    else if(c=='K')
    {
        return 6;
    }
    else
    {
        return -1;
    }
}

void OpeningBook::switchSides()
{
    Board oldBoard;
    int i,x,y;
    for(i=0; i<64; i++)
    {
        oldBoard.b[i] = board.b[i];
    }
    oldBoard.castling[0] = board.castling[0];
    oldBoard.castling[1] = board.castling[1];
    oldBoard.castling[2] = board.castling[2];
    oldBoard.castling[3] = board.castling[3];
    oldBoard.lastMove[0] = board.lastMove[0];
    oldBoard.lastMove[1] = board.lastMove[1];
    oldBoard.fiftyMoveDrawCounter = board.fiftyMoveDrawCounter;
    for(i=0; i<64; i++)
    {
        x = i/8;
        y = i%8;
        board.b[i] = -1*oldBoard.b[8*x+7-y];
    }
    board.castling[0] = oldBoard.castling[2];
    board.castling[1] = oldBoard.castling[3];
    board.castling[2] = oldBoard.castling[0];
    board.castling[3] = oldBoard.castling[1];
    x = oldBoard.lastMove[0]/8;
    y = oldBoard.lastMove[0]%8;
    board.lastMove[0] = 8*x+7-y;
    x = oldBoard.lastMove[1]/8;
    y = oldBoard.lastMove[1]%8;
    board.lastMove[1] = 8*x+7-y;
}

int OpeningBook::indexOfAttackingPiece(int index, int piece, int column, int row)
{
    int j;
    int dist;
    std::vector < int > possibleAnswer;
    if(piece == 2)
    {
        if(index/8>=2 && index%8>=1)
        {
            if(board.b[index-17]==2)
            {
                possibleAnswer.push_back(index-17);
            }
        }
        if(index/8>=2 && index%8<=6)
        {
            if(board.b[index-15]==2)
            {
                possibleAnswer.push_back(index-15);
            }
        }
        if(index/8>=1 && index%8>=2)
        {
            if(board.b[index-10]==2)
            {
                possibleAnswer.push_back(index-10);
            }
        }
        if(index/8>=1 && index%8<=5)
        {
            if(board.b[index-6]==2)
            {
                possibleAnswer.push_back(index-6);
            }
        }
        if(index/8<=6 && index%8>=2)
        {
            if(board.b[index+6]==2)
            {
                possibleAnswer.push_back(index+6);
            }
        }
        if(index/8<=6 && index%8<=5)
        {
            if(board.b[index+10]==2)
            {
                possibleAnswer.push_back(index+10);
            }
        }
        if(index/8<=5 && index%8>=1)
        {
            if(board.b[index+15]==2)
            {
                possibleAnswer.push_back(index+15);
            }
        }
        if(index/8<=5 && index%8<=6)
        {
            if(board.b[index+17]==2)
            {
                possibleAnswer.push_back(index+17);
            }
        }
    }
    else if(piece==3)
    {
        // bishop
        // downward and leftward
        if(index/8<index%8)
        {
            dist = index/8;
        }
        else
        {
            dist = index%8;
        }
        for(j=1;j<=dist;j++)
        {
            if(board.b[index-9*j]==3)
            {
                possibleAnswer.push_back(index-9*j);
                break;
            }
            else if(board.b[index-9*j]!=0)
            {
                break;
            }
        }
        
        // downward and rightward
        if(7-index/8<index%8)
        {
            dist = 7-index/8;
        }
        else
        {
            dist = index%8;
        }
        for(j=1;j<=dist;j++)
        {
            if(board.b[index+7*j]==3)
            {
                possibleAnswer.push_back(index+7*j);
                break;
            }
            else if(board.b[index+7*j]!=0)
            {
                break;
            }
        }
        
        // upward and leftward
        if(index/8<7-index%8)
        {
            dist = index/8;
        }
        else
        {
            dist = 7-index%8;
        }
        for(j=1;j<=dist;j++)
        {
            if(board.b[index-7*j]==3)
            {
                possibleAnswer.push_back(index-7*j);
                break;
            }
            else if(board.b[index-7*j]!=0)
            {
                break;
            }
        }
        
        
        // upward and rightward
        if(7-index/8<7-index%8)
        {
            dist = 7-index/8;
        }
        else
        {
            dist = 7-index%8;
        }
        for(j=1;j<=dist;j++)
        {
            if(board.b[index+9*j]==3)
            {
                possibleAnswer.push_back(index+9*j);
                break;
            }
            else if(board.b[index+9*j]!=0)
            {
                break;
            }
        }
    }
    else if(piece==4)
    {
        // rook
        // downward
        dist = index%8;
        for(j=1;j<=dist;j++)
        {
            if(board.b[index-j]==4)
            {
                possibleAnswer.push_back(index-j);
                break;
            }
            else if(board.b[index-j]!=0)
            {
                break;
            }
        }
        
        // upward
        dist = 7-index%8;
        for(j=1;j<=dist;j++)
        {
            if(board.b[index+j]==4)
            {
                possibleAnswer.push_back(index+j);
                break;
            }
            else if(board.b[index+j]!=0)
            {
                break;
            }
        }
        
        // leftward
        dist = index/8;
        for(j=1;j<=dist;j++)
        {
            if(board.b[index-8*j]==4)
            {
                possibleAnswer.push_back(index-8*j);
                break;
            }
            else if(board.b[index-8*j]!=0)
            {
                break;
            }
        }
        
        // rightward
        dist = 7-index/8;
        for(j=1;j<=dist;j++)
        {
            if(board.b[index+8*j]==4)
            {
                possibleAnswer.push_back(index+8*j);
                break;
            }
            else if(board.b[index+8*j]!=0)
            {
                break;
            }
        }
    }
    else if(piece==5)
    {
        // queen
        // downward and leftward
        if(index/8<index%8)
        {
            dist = index/8;
        }
        else
        {
            dist = index%8;
        }
        for(j=1;j<=dist;j++)
        {
            if(board.b[index-9*j]==5)
            {
                possibleAnswer.push_back(index-9*j);
                break;
            }
            else if(board.b[index-9*j]!=0)
            {
                break;
            }
        }
        
        // downward and rightward
        if(7-index/8<index%8)
        {
            dist = 7-index/8;
        }
        else
        {
            dist = index%8;
        }
        for(j=1;j<=dist;j++)
        {
            if(board.b[index+7*j]==5)
            {
                possibleAnswer.push_back(index+7*j);
                break;
            }
            else if(board.b[index+7*j]!=0)
            {
                break;
            }
        }
        
        // upward and leftward
        if(index/8<7-index%8)
        {
            dist = index/8;
        }
        else
        {
            dist = 7-index%8;
        }
        for(j=1;j<=dist;j++)
        {
            if(board.b[index-7*j]==5)
            {
                possibleAnswer.push_back(index-7*j);
                break;
            }
            else if(board.b[index-7*j]!=0)
            {
                break;
            }
        }
        
        
        // upward and rightward
        if(7-index/8<7-index%8)
        {
            dist = 7-index/8;
        }
        else
        {
            dist = 7-index%8;
        }
        for(j=1;j<=dist;j++)
        {
            if(board.b[index+9*j]==5)
            {
                possibleAnswer.push_back(index+9*j);
                break;
            }
            else if(board.b[index+9*j]!=0)
            {
                break;
            }
        }
        
        // downward
        dist = index%8;
        for(j=1;j<=dist;j++)
        {
            if(board.b[index-j]==5)
            {
                possibleAnswer.push_back(index-j);
                break;
            }
            else if(board.b[index-j]!=0)
            {
                break;
            }
        }
        
        // upward
        dist = 7-index%8;
        for(j=1;j<=dist;j++)
        {
            if(board.b[index+j]==5)
            {
                possibleAnswer.push_back(index+j);
                break;
            }
            else if(board.b[index+j]!=0)
            {
                break;
            }
        }
        
        // leftward
        dist = index/8;
        for(j=1;j<=dist;j++)
        {
            if(board.b[index-8*j]==5)
            {
                possibleAnswer.push_back(index-8*j);
                break;
            }
            else if(board.b[index-8*j]!=0)
            {
                break;
            }
        }
        
        // rightward
        dist = 7-index/8;
        for(j=1;j<=dist;j++)
        {
            if(board.b[index+8*j]==5)
            {
                possibleAnswer.push_back(index+8*j);
                break;
            }
            else if(board.b[index+8*j]!=0)
            {
                break;
            }
        }
    }
    else if(piece==6)
    {
        // king
        if(index%8 != 0)
        {
            if(index/8 != 0)
            {
                if(board.b[index-9]==6)
                {
                    possibleAnswer.push_back(index-9);
                }
                if(board.b[index-8]==6)
                {
                    possibleAnswer.push_back(index-8);
                }
                if(board.b[index-1]==6)
                {
                    possibleAnswer.push_back(index-1);
                }
            }
            if(index/8 != 7)
            {
                if(board.b[index+8]==6)
                {
                    possibleAnswer.push_back(index+8);
                }
                if(board.b[index+7]==6)
                {
                    possibleAnswer.push_back(index+7);
                }
                if(board.b[index-1]==6)
                {
                    possibleAnswer.push_back(index-1);
                }
            }
        }
        if(index%8 != 7)
        {
            if(index/8 != 0)
            {
                if(board.b[index-7]==6)
                {
                    possibleAnswer.push_back(index-7);
                }
                if(board.b[index-8]==6)
                {
                    possibleAnswer.push_back(index-8);
                }
                if(board.b[index+1]==6)
                {
                    possibleAnswer.push_back(index+1);
                }
            }
            if(index/8 != 7)
            {
                if(board.b[index+9]==6)
                {
                    possibleAnswer.push_back(index+9);
                }
                if(board.b[index+8]==6)
                {
                    possibleAnswer.push_back(index+8);
                }
                if(board.b[index+1]==6)
                {
                    possibleAnswer.push_back(index+1);
                }
            }
        }
    }
    
    if(possibleAnswer.size() == 0)
    {
        return -1;
    }
    
    if(column == -1)
    {
        if(row == -1)
        {
            return possibleAnswer[0];
        }
        else
        {
            for(int i=0; i<possibleAnswer.size(); i++)
            {
                if(possibleAnswer[i]%8 == row)
                {
                    return possibleAnswer[i];
                }
            }
            return -1;
        }
    }
    else
    {
        if(row == -1)
        {
            for(int i=0; i<possibleAnswer.size(); i++)
            {
                if(possibleAnswer[i]/8 == column)
                {
                    return possibleAnswer[i];
                }
            }
            return -1;
        }
        else
        {
            for(int i=0; i<possibleAnswer.size(); i++)
            {
                if(possibleAnswer[i]/8==column && possibleAnswer[i]%8==row)
                {
                    return possibleAnswer[i];
                }
            }
        }
    }
    return -1;
}

int OpeningBook::cpuAlgebraicToNumeric(std::string str, int layer)
{
    int from = 0;
    int to = 0;
    int column, row;
    if(str[str.length()-1]=='+' || str[str.length()-1]=='#')
    {
        // get rid of check or checkmate
        str = str.substr(0, str.length()-1);
    }
    if(str == "O-O")
    {
        // king-side castling
        from = 39;
        to = 55;
    }
    else if(str == "O-O-O")
    {
        // queen-side castling
        from = 39;
        to = 23;
    }
    else if(str.length()<=2 || (str.length()==4 && islower(str[0])!=0 && str[1]=='x'))
    {
        // pawn move
        if(str.length() == 2)
        {
            // simple pawn advance
            if(layer%2 == 1)
            {
                row = letterToNumber(str[1]);
            }
            else
            {
                row = 7-letterToNumber(str[1]);
            }
            column = letterToNumber(str[0]);
            to = 8*column+row;
            if(board.b[to+1]==1)
            {
                from = to+1;
            }
            else
            {
                from = to+2;
            }
        }
        else if(str[str.length()-2] == '=')
        {
            if(str.length() == 4)
            {
                // non-capture promotion
                int index = cpuAlgebraicToNumeric(str.substr(0,2), layer);
                from = index/100;
                to = index%100;
                to += 100*letterToNumber(str[3]);
            }
            else
            {
                // capture promotion
                int index = cpuAlgebraicToNumeric(str.substr(0,4), layer);
                from = index/100;
                to = index%100;
                to += 100*letterToNumber(str[5]);
            }
        }
        else
        {
            // non-promotion capture    exd6
            if(layer%2 == 1)
            {
                row = letterToNumber(str[3]);
            }
            else
            {
                row = 7-letterToNumber(str[3]);
            }
            column = letterToNumber(str[2]);
            to = 8*column+row;
            column = letterToNumber(str[0]);
            from = column*8+row+1;
        }
    }
    else
    {
        // piece move
        if(str[str.length()-3]=='x')
        {
            // piece capture
            if(str.length()==4)
            {
                // normal (e.g. Nxa1)
                from = cpuAlgebraicToNumeric(str.substr(0,1)+str.substr(2), layer);
                return from;
            }
            else if(str.length()==5)
            {
                if(isnumber(str[0])==0)
                {
                    // uses column (e.g. Naxb1)
                    from = cpuAlgebraicToNumeric(str.substr(0,2)+str.substr(3), layer);
                    return from;
                }
                else
                {
                    // uses row (e.g. N1xb2)
                    from = cpuAlgebraicToNumeric(str.substr(0,2)+str.substr(3), layer);
                    return from;
                }
            }
            else if(str.length()==5)
            {
                // uses row AND column (e.g. Na1xb2)
                from = cpuAlgebraicToNumeric(str.substr(0,3)+str.substr(4), layer);
                return from;
            }
        }
        else
        {
            // piece non-capture
            if(str.length()==3)
            {
                // normal (e.g. Na1)
                if(layer%2 == 1)
                {
                    row = letterToNumber(str[2]);
                }
                else
                {
                    row = 7-letterToNumber(str[2]);
                }
                column = letterToNumber(str[1]);
                to = 8*column+row;
                from = indexOfAttackingPiece(to, letterToNumber(str[0]), -1, -1);
            }
            else if(str.length()==4)
            {
                if(layer%2 == 1)
                {
                    row = letterToNumber(str[3]);
                }
                else
                {
                    row = 7-letterToNumber(str[3]);
                }
                column = letterToNumber(str[2]);
                to = 8*column+row;
                if(isnumber(str[1])==0)
                {
                    // uses column (e.g. Nab1)
                    column = letterToNumber(str[1]);
                    from = indexOfAttackingPiece(to, letterToNumber(str[0]), column, -1);
                }
                else
                {
                    // uses row (e.g. N1b2)
                    if(layer%2 == 1)
                    {
                        row = letterToNumber(str[1]);
                    }
                    else
                    {
                        row = 7-letterToNumber(str[1]);
                    }
                    from = indexOfAttackingPiece(to, letterToNumber(str[0]), -1, row);
                }
            }
            else if(str.length()==5)
            {
                // uses row AND column (e.g. Na1b2)
                if(layer%2 == 1)
                {
                    column = letterToNumber(str[3]);
                    row = letterToNumber(str[4]);
                    to = 8*column+row;
                    column = letterToNumber(str[1]);
                    row = letterToNumber(str[2]);
                    from = 8*column+row;
                }
                else
                {
                    column = 7-letterToNumber(str[3]);
                    row = 7-letterToNumber(str[4]);
                    to = 8*column+row;
                    column = letterToNumber(str[1]);
                    row = 7-letterToNumber(str[2]);
                    from = 8*column+row;
                }
            }
        }
    }
    return 100*from+to;
}

void OpeningBook::store(int from, int to, int weight)
{
    if(weight >= minThreshold)
    {
        uint64_t index = board.getZorbistHash();
        
        std::unordered_map<uint64_t, OpeningMove>::iterator it = table.find(index);
        if(it == table.end())
        {
            // no current move
            OpeningMove move;
            move.move.push_back(from);
            move.move.push_back(to) ;
            move.move.push_back(weight);
            table.insert(std::make_pair(index, move));
        }
        else
        {
            // add to current
            for(int i=0; i<it->second.move.size(); i+=3)
            {
                if(it->second.move[i] == from && it->second.move[i+1] == to)
                {
                    return;
                }
            }
            it->second.move.push_back(from);
            it->second.move.push_back(to);
            it->second.move.push_back(weight);
        }
    }
}

void OpeningBook::makeMove(std::string str, int layer)
{
    counter++;
    int i;
    int endBracket = -1;
    for(i=0; i<str.length(); i++)
    {
        if(str[i]==']')
        {
            endBracket = i;
        }
        if(str[i]=='(')
        {
            break;
        }
        else if(str[i]==')')
        {
            break;
        }
    }
    int q, from, to;
    if(i == str.length())
    {
        if(endBracket == -1)
        {
            q = cpuAlgebraicToNumeric(str, layer);
            from = q/100;
            to = q%100;
            store(from, to, 1);
        }
        else
        {
            std::string pear = str.substr(endBracket+1);
            q = cpuAlgebraicToNumeric(str.substr(endBracket+1), layer);
            from = q/100;
            to = q%100;
            if(endBracket==1)
            {
                // empty brackets
                store(from, to, 1);
            }
            else
            {
                store(from, to, std::stoi(str.substr(1,endBracket-1)));
            }
        }
    }
    else
    {
        if(endBracket==-1)
        {
            q = cpuAlgebraicToNumeric(str.substr(0, i), layer);
            from = q/100;
            to = q%100;
            store(from, to, 1);
        }
        else
        {
            q = cpuAlgebraicToNumeric(str.substr(endBracket+1, i-endBracket-1), layer);
            from = q/100;
            to = q%100;
            std::string apple = str;
            store(from, to, std::stoi(str.substr(1,i-1)));
        }
        str = str.substr(i,str.length()-i);
    }
    
    int sto, stoB;
    int stoC = board.lastMove[0];
    int stoD = board.lastMove[1];
    board.lastMove[0] = from;
    board.lastMove[1] = to;
    if(board.b[from%100]>0)
    {
        // cpu
        if(from>=100 && board.b[from%100]==1 && to%8==0)
        {
            // promotion
            sto = board.b[to];
            board.b[to] = (int) (from/100);
            board.b[from%100] = 0;
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
            
            tree(str, layer+1);
            
            board.castling[0] = sto0;
            board.castling[1] = sto1;
            board.castling[2] = sto2;
            board.castling[3] = sto3;
            board.fiftyMoveDrawCounter = stoB;
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
            if(to==23 && board.b[7]==4 && board.b[23]==0 && board.b[31]==0 && board.b[39]==6)
            {
                board.b[7] = 0;
                board.b[23] = 6;
                board.b[31] = 4;
                board.b[39] = 0;
                tree(str, layer+1);
                board.b[7] = 4;
                board.b[23] = 0;
                board.b[31] = 0;
                board.b[39] = 6;
            }
            else if(to==55 && board.b[39]==6 && board.b[47]==0 && board.b[55]==0 && board.b[63]==4)
            {
                board.b[39] = 0;
                board.b[47] = 4;
                board.b[55] = 6;
                board.b[63] = 0;
                tree(str, layer+1);
                board.b[39] = 6;
                board.b[47] = 0;
                board.b[55] = 0;
                board.b[63] = 4;
            }
            else
            {
                std::cout << "(ILLEGAL CASTLE IN OPENING BOOK)\n";
            }
            board.fiftyMoveDrawCounter--;
            board.castling[0] = sto0;
            board.castling[1] = sto1;
        }
        else if(board.b[from]==1 && board.b[to]==0 && (to-from==-9 || to-from==7))
        {
            // En passant
            if(board.b[to+1]==-1)
            {
                board.b[to+1] = 0;
                board.b[to] = 1;
                board.b[from] = 0;
                stoB = board.fiftyMoveDrawCounter;
                board.fiftyMoveDrawCounter = 0;
                
                tree(str, layer+1);
                
                board.fiftyMoveDrawCounter = stoB;
                board.b[to+1] = -1;
                board.b[to] = 0;
                board.b[from] = 1;
            }
            else
            {
                std::cout << "(ILLEGAL EN PASSANT IN OPENING BOOK)\n";
            }
        }
        else
        {
            if(str[0]=='N' && board.b[from]!=2)
            {
                std::cout << "(ILLEGAL KNIGHT MOVE IN OPENING BOOK)";
            }
            else if(str[0]=='B' && board.b[from]!=3)
            {
                std::cout << "(ILLEGAL BISHOP MOVE IN OPENING BOOK)";
            }
            else if(str[0]=='R' && board.b[from]!=4)
            {
                std::cout << "(ILLEGAL ROOK MOVE IN OPENING BOOK)";
            }
            else if(str[0]=='Q' && board.b[from]!=5)
            {
                std::cout << "(ILLEGAL QUEEN MOVE IN OPENING BOOK)";
            }
            else if(str[0]=='K' && board.b[from]!=6)
            {
                std::cout << "(ILLEGAL KING MOVE IN OPENING BOOK)";
            }
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
            
            stoB = board.fiftyMoveDrawCounter;
            if(sto==0)
            {
                board.fiftyMoveDrawCounter++;
            }
            else
            {
                board.fiftyMoveDrawCounter = 0;
            }
            std::vector < std::string > varvarvara;
            for(i=0; i<str.length(); i+=1000)
            {
                varvarvara.push_back(str.substr(i));
            }
            tree(str, layer+1);
            
            board.fiftyMoveDrawCounter = stoB;
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
        std::cout << "(ILLEGAL MOVE IN OPENING BOOK: " << from << "," << to << ")\n";
    }
    board.lastMove[0] = stoC;
    board.lastMove[1] = stoD;
}

int OpeningBook::getRandomOpeningMove()
{
    Board board;
    char arr[71] = {
        -4, -1, 0, 0, 0, 0, 1, 4,
        -2, -1, 0, 0, 0, 0, 1, 2,
        -3, -1, 0, 0, 0, 0, 1, 3,
        -5, -1, 0, 0, 0, 0, 1, 5,
        -6, -1, 0, 0, 0, 0, 1, 6,
        -3, -1, 0, 0, 0, 0, 1, 3,
        -2, -1, 0, 0, 0, 0, 1, 2,
        -4, -1, 0, 0, 0, 0, 1, 4,
        1,1,1,1,0,0,0
    };
    
    board.set(arr);
    
    Board* b = &board;
    
    OpeningMove hash = search(b);
    
    if(hash.move.size()==0)
    {
        return -1;
    }
    int i;
    int sum = 0;
    for(i=0; i<hash.move.size(); i+=3)
    {
        sum += hash.move[i+2];
    }
    int threshold = minPercentThreshold*sum;
    sum = 0;
    for(i=0; i<hash.move.size(); i+=3)
    {
        if(hash.move[i+2] > threshold)
        {
            sum += hash.move[i+2];
        }
    }
    
    int r = rand()%sum;
    if(r<0)
    {
        r += sum;
    }
    
    for(i=0; i<hash.move.size(); i+=3)
    {
        if(hash.move[i+2] > threshold)
        {
            r -= hash.move[i+2];
            if(r < 0)
            {
                return hash.move[i]*100+hash.move[i+1];
            }
        }
    }
    return -1;
}

int OpeningBook::searchRandom(Board* b)
{
    char arr[64] = {
        -4, -1, 0, 0, 0, 0, 1, 4,
        -2, -1, 0, 0, 0, 0, 1, 2,
        -3, -1, 0, 0, 0, 0, 1, 3,
        -5, -1, 0, 0, 0, 0, 1, 5,
        -6, -1, 0, 0, 0, 0, 1, 6,
        -3, -1, 0, 0, 0, 0, 1, 3,
        -2, -1, 0, 0, 0, 0, 1, 2,
        -4, -1, 0, 0, 0, 0, 1, 4,
    };
    
    int openingBoard = true;
    for(int i=0; i<64; i++)
    {
        if(b->b[i] != arr[i])
        {
            openingBoard = false;
            break;
        }
    }
    for(int i=0; i<4; i++)
    {
        if(b->castling[0] != 1)
        {
            openingBoard = false;
            break;
        }
    }
    if(b->fiftyMoveDrawCounter != 0)
    {
        openingBoard = false;
    }
    
    if(openingBoard)
    {
        return getRandomOpeningMove();
    }
    
    OpeningMove hash = search(b);
    if(hash.move.size()==0)
    {
        return -1;
    }
    int i;
    int sum = 0;
    for(i=0; i<hash.move.size(); i+=3)
    {
        std::cout << hash.move[i] << "," << hash.move[i+1] << "," << hash.move[i+2] << "\n";
        sum += hash.move[i+2];
    }
    std::cout << "\n\n";
    int threshold = minPercentThreshold*sum;
    sum = 0;
    for(i=0; i<hash.move.size(); i+=3)
    {
        if(hash.move[i+2] > threshold)
        {
            sum += hash.move[i+2];
        }
    }
    int r = rand()%sum;
    if(r<0)
    {
        r += sum;
    }
    for(i=0; i<hash.move.size(); i+=3)
    {
        if(hash.move[i+2] > threshold)
        {
            r -= hash.move[i+2];
            if(r < 0)
            {
                return hash.move[i]*100+hash.move[i+1];
            }
        }
    }
    return -1;
}

void OpeningBook::toInt(int bytes) {
    if(bytes == 8) {
        uint64_t rtn = 0;
        for(uint8_t i=0; i<bytes; i++) {
            rtn += (uint64_t)charsForConversion[i] << (8*i);
        }
        int64ForConversion = rtn;
    }
    else if(bytes == 4) {
        uint32_t rtn = 0;
        for(uint8_t i=0; i<bytes; i++) {
            rtn += (uint32_t)charsForConversion[i] << (8*i);
        }
        int32ForConversion = rtn;
    }
    else if(bytes == 2) {
        uint16_t rtn = 0;
        for(uint8_t i=0; i<bytes; i++) {
            rtn += (uint16_t)charsForConversion[i] << (8*i);
        }
        int16ForConversion = rtn;
    }
    else if(bytes == 1) {
        uint8_t rtn = 0;
        for(uint8_t i=0; i<bytes; i++) {
            rtn += (uint8_t)charsForConversion[i] << (8*i);
        }
        int8ForConversion = rtn;
    }
}

void OpeningBook::toString(uint64_t input, int bytes) {
    uint8_t *ptr = (uint8_t*)&input;
    for(uint8_t i=0; i<bytes; i++) {
        charsForConversion[i] = ptr[i];
    }
}


void OpeningBook::loadOpeningBook(std::string str)
{
    int i, j;
    uint64_t key;
    int numberOfMoves;
    uint16_t move;
    uint8_t weight;
    
    for(i=0; i<str.size(); i=i)
    {
        // get key
        if(i+8 >= str.size())
        {
            break;
        }
        
        for(j=0; j<8; j++) {
            
            charsForConversion[j] = str[i+j];
        }
        
        toInt(8);
        key = int64ForConversion;
        
        if(key == 784518378181513948)
        {
            
            int q = 2;
            q = 512;
        }
        
        i += 8;
        
        if(i+1 >= str.size())
        {
            break;
        }
        
        // get number of moves
        charsForConversion[0] = str[i];
        toInt(1);
        numberOfMoves = int8ForConversion;
        
        i++;
        
        for(j=0; j<numberOfMoves; j++)
        {
            if(i+3 >= str.size())
            {
                break;
            }
            charsForConversion[0] = str[i];
            charsForConversion[1] = str[i+1];
            i += 2;
            toInt(2);
            move = int16ForConversion;
            charsForConversion[0] = str[i];
            toInt(1);
            weight = int8ForConversion;
            i++;
            newStore(key, move/100, move%100, weight);
        }
    }
}

void OpeningBook::newStore(uint64_t key, int from, int to, int weight)
{
    if(weight >= minThreshold)
    {
        std::unordered_map<uint64_t, OpeningMove>::iterator it = table.find(key);
        if(it == table.end())
        {
            // no current position in table
            OpeningMove move;
            move.move.push_back(from);
            move.move.push_back(to) ;
            move.move.push_back(weight);
            table.insert(std::make_pair(key, move));
        }
        else
        {
            // add to current
            for(int i=0; i<it->second.move.size(); i+=3)
            {
                if(it->second.move[i] == from && it->second.move[i+1] == to)
                {
                    return;
                }
            }
            it->second.move.push_back(from);
            it->second.move.push_back(to);
            it->second.move.push_back(weight);
        }
    }
}
