//
//  Board.cpp
//  Schach
//
//  Created by Thomas Redding on 7/8/14.
//  Copyright (c) 2014 Thomas Redding. All rights reserved.
//

#include "Board.h"

const int RANDOM_ARRAY_LENGTH = 901;
uint64_t randomArr[RANDOM_ARRAY_LENGTH];

Board::Board()
{
    for(int i=0; i<64; i++)
    {
        b[i] = 0;
    }
    castling[0] = 0;
    castling[1] = 0;
    castling[2] = 0;
    castling[3] = 0;
    lastMove[0] = 0;
    lastMove[1] = 0;
    fiftyMoveDrawCounter = 0;
    recalculateBitboards();
    history.clear();
}

void Board::switchSides()
{
    if(history.size() == 0)
    {
        uint8_t i;
        for(i = 0; i < 64; i++)
        {
            if(b[i] == 0)
            {
                break;
            }
        }
        Move m;
        m.from = i;
        m.to = i;
        makeMove(m);
        fiftyMoveDrawCounter = 0;
    }
    
    uint8_t store;
    bool castleStore;
    
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 4; j++) {
            store = b[8 * i + j];
            b[8 * i + j] = b[8 * i + (7 - j)];
            b[8 * i + (7 - j)] = store;
        }
    }
    for(int i = 0; i < 64; i++) {
        b[i] *= -1;
    }
    
    
    int oldValue = lastMove[0];
    int promotion = oldValue/100;
    int x = (oldValue - 100*promotion)/8;
    int y = (oldValue - 100*promotion)%8;
    lastMove[0] = 8*x+7-y + 100*promotion;
    x = lastMove[1]/8;
    y = lastMove[1]%8;
    lastMove[1] = 8*x+7-y;
    
    
    lastMove[2] *= -1;
    castleStore = castling[0];
    castling[0] = castling[2];
    castling[2] = castleStore;
    castleStore = castling[1];
    castling[1] = castling[3];
    castling[3] = castleStore;
    for(int i = 0; i < history.size(); i += 5)
    {
        x = history[i]/8;
        y = history[i]%8;
        history[i] = 8*x+7-y;
        x = history[i+1]/8;
        y = history[i+1]%8;
        history[i + 1] = 8*x+7-y;
        history[i + 2] *= -1;
        store = history[i + 3]; // castling
        history[i + 3] = (history[i + 3] >> 2) & (history[i + 3] << 2);
    }
}

Board::Board(const Board& board)
{
    for(int i = 0; i < 64; i++)
    {
        b[i] = board.b[i];
    }
    for(int i = 0; i < board.history.size(); i++)
    {
        history.push_back(board.history[i]);
    }
    for(int i = 0; i < 13; i++) {
        bitmaps[i] = board.bitmaps[i];
    }
    lastMove[0] = board.lastMove[0];
    lastMove[1] = board.lastMove[1];
    fiftyMoveDrawCounter = board.fiftyMoveDrawCounter;
    for(int i = 0; i < 4; i++)
    {
        castling[i] = board.castling[i];
    }
    user = board.user;
    cpu = board.cpu;
    history.clear();
    for(int i = 0; i < board.history.size(); i++) {
        history.push_back(board.history[i]);
    }
}

void Board::setRandomArr()
{
    uint64_t R0, R1, R2, R3;
    for(int i=0; i<RANDOM_ARRAY_LENGTH; i++)
    {
        R0 = (uint64_t)random() << 48;
        R1 = (uint64_t)random() << 48 >> 16;
        R2 = (uint64_t)random() << 48 >> 32;
        R3 = (uint64_t)random() << 48 >> 48;
        randomArr[i] = R0 | R1 | R2 | R3;
    }
}

bool Board::set(char* arr)
{
    bool rtn = true;
    for(int i=0; i<64; i++)
    {
        if(b[i] != arr[i])
        {
            rtn = false;
        }
        b[i] = arr[i];
    }
    for(int i=0; i<4; i++)
    {
        if(castling[i] != arr[64+i])
        {
            rtn = false;
        }
        castling[i] = arr[64+i];
    }
    if(lastMove[0] != arr[68])
    {
        rtn = false;
    }
    lastMove[0] = arr[68];
    if(lastMove[1] != arr[69])
    {
        rtn = false;
    }
    lastMove[1] = arr[69];
    if(fiftyMoveDrawCounter != arr[70])
    {
        rtn = false;
    }
    fiftyMoveDrawCounter = arr[70];
    
    history.clear();

    recalculateBitboards();
    return rtn;
}

bool Board::set(const Board &newBoard)
{
    bool rtn = true;
    for(int i=0; i<64; i++)
    {
        if(b[i] != newBoard.b[i])
        {
            rtn = false;
        }
        b[i] = newBoard.b[i];
    }
    for(int i=0; i<4; i++)
    {
        if(castling[i] != newBoard.castling[i])
        {
            rtn = false;
        }
        castling[i] = newBoard.castling[i];
    }
    if(lastMove[0] != newBoard.lastMove[0])
    {
        rtn = false;
    }
    lastMove[0] = newBoard.lastMove[0];
    if(lastMove[1] != newBoard.lastMove[1])
    {
        rtn = false;
    }
    lastMove[1] = newBoard.lastMove[1];
    if(fiftyMoveDrawCounter != newBoard.fiftyMoveDrawCounter)
    {
        rtn = false;
    }
    fiftyMoveDrawCounter = newBoard.fiftyMoveDrawCounter;
    
    history.clear();
    for(int i = 0; i < newBoard.history.size(); i++) {
        history.push_back(newBoard.history[i]);
    }
    
    recalculateBitboards();
    return rtn;
}

uint64_t Board::getZorbistHash()
{
    uint64_t index = 0;
    for(int i=0; i<64; i++)
    {
        index += randomArr[13*i+b[i]+6];
    }
    index += randomArr[832]*castling[0];
    index += randomArr[833]*castling[1];
    index += randomArr[834]*castling[2];
    index += randomArr[835]*castling[3];
    if(abs(b[lastMove[1]])==1 && abs(lastMove[1]-lastMove[0])==16)
    {
        // a pawn was moved two spaces last move
        if(lastMove[1]/8 == 0)
        {
            // left-most column
            if(b[lastMove[1]+8] == -1*b[lastMove[1]])
            {
                index += randomArr[836+lastMove[1]%100];
            }
        }
        else if(lastMove[1]/8 == 7)
        {
            // right-most column
            if(b[lastMove[1]-8] == -1*b[lastMove[1]])
            {
                index += randomArr[836+lastMove[1]%100];
            }
        }
        else
        {
            // middle column
            if(b[lastMove[1]+8] == -1*b[lastMove[1]])
            {
                index += randomArr[836+lastMove[1]%100];
            }
            else if(b[lastMove[1]-8] == -1*b[lastMove[1]])
            {
                index += randomArr[836+lastMove[1]%100];
            }
        }
    }
    
    // turn
    if(b[lastMove[1]%100] > 0)
    {
        index += randomArr[900];
    }
    return index;
}

std::string Board::toString()
{
    std::string rtn = "";
    for(int i=0; i<64; i++)
    {
        if(b[i] >= 0) {
            rtn += " ";
        }
        rtn += std::to_string(b[i]) + ", ";
        if(i%8 == 7)
        {
            rtn += "\n";
        }
    }
    rtn += std::to_string(castling[0]) + ",";
    rtn += std::to_string(castling[1]) + ",";
    rtn += std::to_string(castling[2]) + ",";
    rtn += std::to_string(castling[3]) + ",";
    rtn += std::to_string(lastMove[0]) + ",";
    rtn += std::to_string(lastMove[1]) + ",";
    rtn += std::to_string(fiftyMoveDrawCounter) + "\n";
    rtn += std::to_string(getZorbistHash()) + "\n";
    rtn += "history: ";
    for(int i = 0; i < history.size(); i += 5)
    {
        rtn += std::to_string((int)history[i]);
        rtn += " --> ";
        rtn += std::to_string((int)history[i + 1]);
        rtn += " [";
        rtn += std::to_string((int)history[i + 2]);
        rtn += "] [";
        rtn += std::to_string((int)history[i + 3]);
        rtn += "] [";
        rtn += std::to_string((int)history[i + 3]);
        rtn += "], ";
    }
    rtn += "\n";
    return rtn;
}

Board& Board::operator=(Board b)
{
    set(b);
    return *this;
}

bool Board::operator!=(const Board &board)
{
    return !(*this == board);
}

void Board::recalculateBitboards()
{
    for(int i = 0; i < 13; i++) {
        bitmaps[i] = 0;
    }
    for(int i = 0; i < 64; i++) {
        bitmaps[b[i] + 6] += (uint64_t)1 << i;
    }
    user = bitmaps[0] | bitmaps[1] | bitmaps[2] | bitmaps[3] | bitmaps[4] | bitmaps[5];
    cpu = bitmaps[7] | bitmaps[8] | bitmaps[9] | bitmaps[10] | bitmaps[11] | bitmaps[12];
}

void Board::updateTile(uint8_t tile, int8_t replace)
{
    uint64_t tile64 = (uint64_t)1 << tile;
    
    // make empty
    if(b[tile] != 0) {
        bitmaps[EMPTY] += tile64;
        bitmaps[b[tile] + 6] -= tile64;
        if(b[tile] < 0) {
            user -= tile64;
        }
        else {
            cpu -= tile64;
        }
    }
    
    // replace
    if(replace < 0) {
        bitmaps[EMPTY] -= tile64;
        bitmaps[replace + 6] += tile64;
        user += tile64;
    }
    else if(replace > 0) {
        bitmaps[EMPTY] -= tile64;
        bitmaps[replace + 6] += tile64;
        cpu += tile64;
    }
}

bool Board::operator==(const Board &board)
{
    for(int i = 0; i < 64; i++)
    {
        if(b[i] != board.b[i])
        {
            std::cout << "inequality due to tiles\n";
            return false;
        }
    }
    if(history.size() != board.history.size())
    {
        std::cout << "inequality due to history size\n";
        return false;
    }
    for(int i = 0; i < history.size(); i++)
    {
        if(history[i] != board.history[i])
        {
            std::cout << "inequality due to history element " << i << "\n";
            return false;
        }
    }
    for(int i = 0; i < 13; i++) {
        if(bitmaps[i] != board.bitmaps[i])
        {
            std::cout << "inequality due to bitmap[" << i << "]\n";
            return false;
        }
    }
    if(lastMove[0] != board.lastMove[0])
    {
        std::cout << "inequality due to lastMove[0]\n";
        return false;
    }
    if(lastMove[1] != board.lastMove[1])
    {
        std::cout << "inequality due to lastMove[1]\n";
        return false;
    }
    if(fiftyMoveDrawCounter != board.fiftyMoveDrawCounter)
    {
        std::cout << "inequality due to fiftyMoveDrawCounter\n";
        return false;
    }
    for(int i = 0; i < 4; i++)
    {
        if(castling[i] != board.castling[i])
        {
            std::cout << "inequality due to castling[" << i << "]\n";
            return false;
        }
        
    }
    if(user != board.user)
    {
        std::cout << "inequality due to user-bitmap\n";
        return  false;
    }
    if(cpu != board.cpu)
    {
        std::cout << "inequality due to cpu-bitmap\n";
        return false;
    }
    return true;
}

std::string Board::printBitmap(uint64_t input)
{
    std::string r = "";
    for(int i = 0; i < 64; i++) {
        if(i % 8 == 0) {
            r += "\n";
        }
        if(input % 2 == 1) {
            r += "1";
        }
        else {
            r += "0";
        }
        input >>= 1;
    }
    r += "\n";
    return r;
}

void Board::makeMove(const Move& move)
{
    history.push_back(move.from);
    history.push_back(move.to);
    history.push_back(b[move.to]);
    history.push_back((castling[0] << 3) + (castling[1] << 2) + (castling[2] << 1) + (castling[3] << 0));
    history.push_back(fiftyMoveDrawCounter);
    lastMove[0] = move.from;
    lastMove[1] = move.to;
    lastMove[2] = b[move.to];
    
    if(move.from > 100) {
        
        // promotion: from + 100 * value
        
        int8_t f = move.from % 100;
        int8_t piece = move.from / 100;
        if(b[f] < 0) {
            piece *= -1;
        }
        
        if(abs(lastMove[2]) == 4) {
            if(move.to == 0) {
                castling[3] = false;
            }
            else if(move.to == 56) {
                castling[2] = false;
            }
            else if(move.to == 7) {
                castling[0] = false;
            }
            else if(move.to == 63) {
                castling[1] = false;
            }
        }
        
        updateTile(move.to, piece);
        updateTile(f, 0);
        b[move.to] = piece;
        b[f] = 0;
        
        fiftyMoveDrawCounter = 0;
    }
    else if(abs(b[move.from]) == 1 && abs(move.from - move.to) > 2 && b[move.to] == 0) {
        // en passant
        if(b[move.from] == 1) {
            // cpu
            updateTile(move.to + 1, 0);
            updateTile(move.to, 1);
            updateTile(move.from, 0);
            b[move.to + 1] = 0;
            b[move.to] = 1;
            b[move.from] = 0;
        }
        else {
            // user
            updateTile(move.to - 1, 0);
            updateTile(move.to, -1);
            updateTile(move.from, 0);
            b[move.to - 1] = 0;
            b[move.to] = -1;
            b[move.from] = 0;
        }
        fiftyMoveDrawCounter = 0;
    }
    else if(abs(b[move.from]) == 6 && abs(move.from - move.to) > 10) {
        // castle
        if(b[move.from] > 0) {
            // cpu
            if(move.to == 23) {
                updateTile(7, 0);
                updateTile(23, 6);
                updateTile(31, 4);
                updateTile(39, 0);
                b[7] = 0;
                b[23] = 6;
                b[31] = 4;
                b[39] = 0;
            }
            else {
                updateTile(39, 0);
                updateTile(47, 4);
                updateTile(55, 6);
                updateTile(63, 0);
                b[39] = 0;
                b[47] = 4;
                b[55] = 6;
                b[63] = 0;
            }
            castling[0] = false;
            castling[1] = false;
        }
        else {
            // user
            if(move.to == 16) {
                updateTile(0, 0);
                updateTile(16, -6);
                updateTile(24, -4);
                updateTile(32, 0);
                b[0] = 0;
                b[16] = -6;
                b[24] = -4;
                b[32] = 0;
            }
            else {
                updateTile(32, 0);
                updateTile(40, -4);
                updateTile(48, -6);
                updateTile(56, 0);
                b[32] = 0;
                b[40] = -4;
                b[48] = -6;
                b[56] = 0;
            }
            castling[2] = false;
            castling[3] = false;
        }
        fiftyMoveDrawCounter++;
    }
    else {
        if(abs(b[move.from]) == 1 || b[move.to] != 0) {
            fiftyMoveDrawCounter = 0;
        }
        else {
            fiftyMoveDrawCounter++;
        }
        if(history[history.size() - 2] != 0)
        {
            if(b[move.to] == 4)
            {
                if(move.to == 63)
                {
                    castling[1] = false;
                }
                else if(move.to == 7)
                {
                    castling[0] = false;
                }
            }
            else if(b[move.to] == -4)
            {
                if(move.to == 56)
                {
                    castling[3] = false;
                }
                else if(move.to == 0)
                {
                    castling[2] = false;
                }
            }
            if(b[move.from] == 6) {
                castling[0] = false;
                castling[1] = false;
            }
            else if(b[move.from] == -6) {
                castling[2] = false;
                castling[3] = false;
            }
            else if(b[move.from] == 4) {
                if(move.from == 63)
                {
                    castling[1] = false;
                }
                else if(move.from == 7)
                {
                    castling[0] = false;
                }
            }
            else if(b[move.from] == -4) {
                if(move.from == 56)
                {
                    castling[3] = false;
                }
                else if(move.from == 0)
                {
                    castling[2] = false;
                }
            }
        }
        updateTile(move.to, b[move.from]);
        updateTile(move.from, 0);
        b[move.to] = b[move.from];
        b[move.from] = 0;
    }
}

void Board::unmakeMove()
{
    
//    std::cout << "unmakeMove(" << from << ", " << to << "); history.size() = " << history.size() << "\n";
    
    int32_t from = history[history.size() - 5];
    int32_t to = history[history.size() - 4];
    int32_t replaced = history[history.size() - 3];
    int32_t castle = history[history.size() - 2];
    fiftyMoveDrawCounter = history[history.size() - 1];
    history.erase(history.end() - 5, history.end());
    lastMove[0] = history[history.size() - 5];
    lastMove[1] = history[history.size() - 4];
    lastMove[2] = history[history.size() - 3];
    
    castling[0] = (castle & 8) > 0;
    castling[1] = (castle & 4) > 0;
    castling[2] = (castle & 2) > 0;
    castling[3] = (castle & 1) > 0;
    
    if(from > 100) {
        // std::cout << "un promote\n";
        int8_t f = from % 100;
        int8_t piece = from / 100;
        if(b[to] < 0) {
            piece *= -1;
        }
        
        uint64_t from64 = (uint64_t)1 << f;
        uint64_t to64 = (uint64_t)1 << to;
        
        // remove promoted piece from board
        bitmaps[piece + 6] -= to64;
        bitmaps[EMPTY] += to64;
        if(piece < 0) {
            user -= to64;
        }
        else if(piece > 0) {
            cpu -= to64;
        }
        
        // add replaced piece
        if(replaced != 0) {
            bitmaps[replaced + 6] += to64;
            if(replaced < 0) {
                user += to64;
            }
            else {
                cpu += to64;
            }
        }
        
        // add pawn
        if(b[to] < 0) {
            user += from64;
            bitmaps[USER_PAWN] += from64;
        }
        else {
            cpu += from64;
            bitmaps[CPU_PAWN] += from64;
        }
        
        b[f] = piece < 0 ? -1 : 1;
        b[to] = replaced;
    }
    else if(abs(b[to]) == 1 && abs(from - to) > 2 && replaced == 0) {
        // en passant
        if(b[to] == 1) {
            updateTile(to + 1, -1);
            updateTile(to, 0);
            updateTile(from, 1);
            b[to + 1] = -1;
            b[to] = 0;
            b[from] = 1;
        }
        else {
            // user
            updateTile(to - 1, 1);
            updateTile(to, 0);
            updateTile(from, -1);
            b[to - 1] = 1;
            b[to] = 0;
            b[from] = -1;
        }
    }
    else if(abs(b[to]) == 6 && abs(from - to) > 10) {
        // castle
        if(b[to] > 0) {
            // cpu
            if(to == 23) {
                updateTile(7, 4);
                updateTile(23, 0);
                updateTile(31, 0);
                updateTile(39, 6);
                b[7] = 4;
                b[23] = 0;
                b[31] = 0;
                b[39] = 6;
            }
            else {
                updateTile(39, 6);
                updateTile(47, 0);
                updateTile(55, 0);
                updateTile(63, 4);
                b[39] = 6;
                b[47] = 0;
                b[55] = 0;
                b[63] = 4;
            }
        }
        else {
            // user
            if(to == 16) {
                updateTile(0, -4);
                updateTile(16, 0);
                updateTile(24, 0);
                updateTile(32, -6);
                b[0] = -4;
                b[16] = 0;
                b[24] = 0;
                b[32] = -6;
            }
            else {
                updateTile(32, -6);
                updateTile(40, 0);
                updateTile(48, 0);
                updateTile(56, -4);
                b[32] = -6;
                b[40] = 0;
                b[48] = 0;
                b[56] = -4;
            }
        }
    }
    else {
        updateTile(from, b[to]);
        updateTile(to, replaced);
        b[from] = b[to];
        b[to] = replaced;
    }
}

uint8_t Board::lsm(const uint64_t& input) const {
    return lsmArray[((input & -input) * 0x022fdd63cc95386d) >> 58];
}
