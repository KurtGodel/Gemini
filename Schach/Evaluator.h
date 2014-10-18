//
//  Evaluator.h
//  Schach
//
//  Created by Thomas Redding on 7/8/14.
//  Copyright (c) 2014 Thomas Redding. All rights reserved.
//

#ifndef __Schach__Evaluator__
#define __Schach__Evaluator__

#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <unordered_map>

#include "Board.h"
#include "ExchangeSimulator.h"
#include "EvaluatorTracker.h"
#include "Tile.h"

class Evaluator
{
public:
    Evaluator(Board* aiBoard);
    int makeMove(int from, int to, int layer, Board* board);                // run whenever a move is made by AI in its search
    void undoMove(int from, int to, int layer, Board* board, int sto);      // run whenever a move is undone by AI in its search
    void start(Board* board);                                               // run at the start of an AI search
    int eval(int layer, int endOfLastMove);                                 // evaluates a position
    int pieceToValue(int num);                                              // returns 
    int getPieceCount();
    float staticExchangeEvaluation(int sq, int layer);
    
private:
    const int specialEvalFailed = 10000000;
    
    int kpVk(int layer);
    int kVkp(int layer);
    
    int pawnEndgameEval(int layer);
    int rookEndgameEval(int layer);
    int queenEndgameEval(int layer);
    int BBendgameEval(int layer);
    
    EvaluatorTracker evaluatorTracker;
    int pawnStructureEval(float stage);
    std::unordered_map<uint64_t, int> pawnStructureMap;
    
    // 1. Material Values
    int openingPawnValue = 513;
    int endingPawnValue = 879;
    int openingKnightValue = 1466;
    int endingKnightValue = 2299;
    int openingBishopValue = 1406;
    int endingBishopValue = 2366;
    int openingRookValue = 2842;
    int endingRookValue = 4077;
    int openingQueenValue = 5193;
    int endingQueenValue = 7162;
    
    
    
    // 2. Pawns Bonuses
    int openingDoubledPawnsBonus = 0;
    int endingDoubledPawnsBonus = -303;
    int openingPawnIslandBonus = -95;
    int endingPawnIslandBonus = -30;
    int openingPawnAdvancementBonus[8] = {0,0,0,0,0,0,0,0};
    int endingPawnAdvancementBonus[8] = {120,100,80,60,40,20,0,0};
    
    // these are in addition to the normal pawnAdvancementBonus
    int openingHalfPassedPawnBonus[8] = {0,0,0,0,0,0,0,0};
    int endingHalfPassedPawnBonus[8] = {180,150,120,90,60,30,0,0};
    int openingPassedPawnBonus[8] = {4314,1422,619,233,0,  0,0,0};
    int endingPassedPawnBonus[8] =  {6283,828, 795,389,257,0,0,0};
    
    
    
    // 3. Pieces Bonuses
    int openingBishopPairBonus = 271;
    int endingBishopPairBonus = 829;
    int openingRookPairBonus = 0;
    int endingRookPairBonus = -816;
    
    
    // 4. King Values & Bonuses
    // cpuKingValue and endGameKingValue are linearly interpolated between as the game progresses
    int cpuKingValue[64] = {
        100,    150,    200,    250,    300,    350,    400,    450,
        100,    150,    200,    250,    300,    350,    400,    450,
        100,    150,    200,    250,    300,    350,    400,    450,
        100,    150,    200,    250,    300,    350,    400,    450,
        100,    150,    200,    250,    300,    350,    400,    450,
        100,    150,    200,    250,    300,    350,    400,    450,
        100,    150,    200,    250,    300,    350,    400,    450,
        100,    150,    200,    250,    300,    350,    400,    450
    };
    int humanKingValue[64];     // set to opposite of cpuKingValues in constructor
    
    int endGameKingValue[64] = {
        0,  31, 56, 75, 75, 56, 31, 0,
        31, 56, 75, 89, 89, 75, 56, 31,
        56, 75, 89, 97, 97, 89, 75, 56,
        75, 89, 97, 100,100,97, 89, 75,
        75, 89, 97, 100,100,97, 89, 75,
        56, 75, 89, 97, 97, 89, 75, 56,
        31, 56, 75, 89, 89, 75, 56, 31,
        0,  31, 56, 75, 75, 56, 31, 0
    };
    // unmoved pawns to protect king both before and after castling
    int queenPawnsBackBeforeCastlingBonus = 48;
    int queenPawnsSecondBackBeforeCastlingBonus = 16;
    int kingPawnsBackBeforeCastlingBonus = 60;
    int kingPawnsSecondBackBeforeCastlingBonus = 16;
    int queenEmptyBeforeCastling = 12;
    int kingEmptyBeforeCastling = 16;
    int pawnsBackRankAfterCastling = 150;
    int pawnsSecondBackRankAfterCastling = 48;
    int centerCannotCastleBonus = -36;
    
    
    
    // 5. Mobility (flipped for human)
    /*
     0 - pawn
     1 - knight
     2 - bishop
     3 - rook
     4 - queen
     5 - king
    */
    // human                            cpu
    
    int cpuMoveBonus[6][64] = {
        {
            28, 28, 28, 28, 28, 28, 28, 28,
            28, 28, 28, 28, 28, 28, 28, 28,
            28, 28, 28, 28, 28, 28, 28, 28,
            28, 28, 28, 50, 50, 28, 28, 28,
            28, 28, 28, 50, 50, 28, 28, 28,
            28, 28, 28, 28, 28, 28, 28, 28,
            28, 28, 28, 28, 28, 28, 28, 28,
            28, 28, 28, 28, 28, 28, 28, 28
        },
        {
            63, 63, 63, 63, 63, 63, 63, 63,
            63, 63, 63, 63, 63, 63, 63, 63,
            63, 63, 63, 63, 63, 63, 63, 63,
            63, 63, 63, 63, 63, 63, 63, 63,
            63, 63, 63, 63, 63, 63, 63, 63,
            63, 63, 63, 63, 63, 63, 63, 63,
            63, 63, 63, 63, 63, 63, 63, 63,
            63, 63, 63, 63, 63, 63, 63, 63
        },
        {
            63, 63, 63, 63, 63, 63, 63, 63,
            63, 63, 63, 63, 63, 63, 63, 63,
            63, 63, 63, 63, 63, 63, 63, 63,
            63, 63, 63, 63, 63, 63, 63, 63,
            63, 63, 63, 63, 63, 63, 63, 63,
            63, 63, 63, 63, 63, 63, 63, 63,
            63, 63, 63, 63, 63, 63, 63, 63,
            63, 63, 63, 63, 63, 63, 63, 63
        },
        {
            99, 99, 1,  1,  1,  1,  1,  1,
            99, 99, 1,  1,  1,  1,  1,  1,
            99, 99, 1,  1,  1,  1,  1,  1,
            99, 99, 1,  1,  1,  1,  1,  1,
            99, 99, 1,  1,  1,  1,  1,  1,
            99, 99, 1,  1,  1,  1,  1,  1,
            99, 99, 1,  1,  1,  1,  1,  1,
            99, 99, 1,  1,  1,  1,  1,  1
        },
        {
            0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0
        },
        {
            0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0,
            0,  0,  0,  0,  0,  0,  0,  0
        },
    };
    int humanMoveBonus[6][64];
    
    // the importance of mobility changes as the game progresses
    float openingMoveBonusMultiplier = 1;
    float endingMoveBonusMultiplier = 0;
    
    
    // 6. Threats - values are based roughly on the average value of each piece; this is confirmed through a 200-game simulation to be roughly ideal, indicating that our guess as to whether a piece is hanging is pretty good
    // rough average value of pieces
    // pawn, minors, rook, queen, king
    int openingExchangeValue[5] = {562, 1827, 2842, 5193, 100000};
    int endingExchangeValue[5] = {879, 2333, 4077, 7162, 300000};
    int closeToKingSafetyBonus = 0;
    int closeToKingDangerBonus = -135;
    int openingInitiativeBonus = 75;
    int endingInitiativeBonus = 75;
    
    int contemptFactorSlope = 100;          // multiplied by stage (between 1.0 and 0.0)
    
    Board* board;                           // pointer to the current board
    
    int spaceControl[30][64][10];
    int humanEvalMove(int skip, int layer);
    int aiEvalMove(int skip, int layer);
    int aiEvalMoveOneSquare(int i, int layer);
    int humanEvalMoveOneSquare(int i, int layer);
    int exchangeTree(int layer);
    sf::Vector2i simulateExchanges(int turn, int cpu, int human, int cpuIndex, int humanIndex);
    int exchangeOption[100];
    int exchangeOptionLength = 0;
    int exchangeValue[5];
    int hangingCpuPiece[64] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    int hangingHumanPiece[64] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    int hangingCpuPieceLength = 0;
    int hangingHumanPieceLength = 0;
    
    uint8_t getManCount();
    int threeManEval(int layer);
    int fourManEval(int layer);
    int hangingToCheck(float value);
    int signum(int x);
    int pieceSafe(int sq, int layer);
    sf::Vector2i inDepthTactics(float stage, int layer);
    sf::Vector2i kingSafety(int cpuKing, int humanKing, float stage, int layer);
    ExchangeSimulator exchangeSimulator;
    void setSpaceControlOfSquare(int sq, int layer);
};

#endif /* defined(__Schach__Evaluator__) */
