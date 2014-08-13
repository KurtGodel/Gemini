//
//  HashTable.cpp
//  Schach
//
//  Created by Thomas Redding on 7/21/14.
//  Copyright (c) 2014 Thomas Redding. All rights reserved.
//

#include "HashTable.h"

void HashTable::insert(uint64_t key, int bestMove, int value, int betaSearchedAt, float depthSearched, bool searchCompleted)
{
    Node node;
    node.bestMove = bestMove;
    node.value = value;
    node.searchCompleted = searchCompleted;
    node.betaSearchedAt = betaSearchedAt;
    node.depthSearched = depthSearched;
    Node* current = searchFor(key);
    if(current == nullptr)
    {
        // add new node to hash table
        table.insert(std::make_pair(key, node));
    }
    else
    {
        // possibly update a node in a hash table
        if(current->searchCompleted)
        {
            if(node.searchCompleted)
            {
                if(node.betaSearchedAt > current->betaSearchedAt)
                {
                    table[key] = node;
                }
                else if(node.betaSearchedAt == current->betaSearchedAt)
                {
                    if(node.depthSearched >= current->depthSearched)
                    {
                        table[key] = node;
                    }
                    else
                    {
                        
                        // retain old node
                    }
                }
                else
                {
                    
                    // retain old node
                }
            }
            else
            {
                
                // retain old node
            }
        }
        else
        {
            if(node.searchCompleted)
            {
                table[key] = node;
            }
            else
            {
                if(node.betaSearchedAt > current->betaSearchedAt)
                {
                    table[key] = node;
                }
                else if(node.betaSearchedAt == current->betaSearchedAt)
                {
                    if(node.depthSearched >= current->depthSearched)
                    {
                        table[key] = node;
                    }
                    else
                    {
                        
                        // retain old node
                    }
                }
                else
                {
                    
                    // retain old node
                }
            }
        }
    }
}

void HashTable::clear()
{
    table.clear();
}

Node* HashTable::searchFor(uint64_t key)
{
    auto it = table.find(key);
    if(it == table.end())
    {
        return nullptr;
    }
    else
    {
        return &it->second;
    }
}