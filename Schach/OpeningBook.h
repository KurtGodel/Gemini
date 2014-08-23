//
//  OpeningBook.h
//  Schach
//
//  Created by Thomas Redding on 7/2/14.
//  Copyright (c) 2014 Thomas Redding. All rights reserved.
//

#ifndef __Schach__OpeningBook__
#define __Schach__OpeningBook__

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <ResourcePath.hpp>
#include <OpeningMove.h>
#include <Board.h>


class OpeningBook
{
public:
    OpeningBook();
    int searchRandom(Board* b);
    void setPrincipleLine(Board* b, std::vector < int > * vect);
private:
    OpeningMove search(Board* b);
    int getRandomOpeningMove();
    void root(std::string str, int layer);
    void tree(std::string str, int layer);
    void makeMove(std::string str, int layer);
    int cpuAlgebraicToNumeric(std::string str, int layer);
    int indexOfAttackingPiece(int index, int piece, int column, int row);
    void logBoard();
    void store(int from, int to, int weight);
    int letterToNumber(char c);
    void switchSides();
    
    int minThreshold = 10;
    float minPercentThreshold = 0.0225;
    
    std::unordered_map<uint64_t, OpeningMove> table;
    Board board;
    int counter = 0;
};

#endif /* defined(__Schach__OpeningBook__) */
