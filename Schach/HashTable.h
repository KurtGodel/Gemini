//
//  HashTable.h
//  Schach
//
//  Created by Thomas Redding on 7/21/14.
//  Copyright (c) 2014 Thomas Redding. All rights reserved.
//

#ifndef __Schach__HashTable__
#define __Schach__HashTable__

#include <iostream>
#include <unordered_map>
#include "Node.h"

class HashTable
{
public:
    void clear();
    void insert(uint64_t key, int bestMove, int value, int betaSearchedAt, float depthSearched, bool searchCompleted);
    Node* searchFor(uint64_t key);
    std::unordered_map < uint64_t, Node > table;
    std::mutex lock;
};

#endif /* defined(__Schach__HashTable__) */
