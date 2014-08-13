//
//  Evaluator_Support.cpp
//  Schach
//
//  Created by Thomas Redding on 7/18/14.
//  Copyright (c) 2014 Thomas Redding. All rights reserved.
//

#include "Evaluator.h"

int Evaluator::humanEvalMove(int skip, int layer)
{
    int i;
    int j;
    int dist;
    int human = 0;
    for(i=0;i<64;i++)
    {
        if(i==skip)
        {
            continue;
        }
        if((*board).b[i]<0)
        {
            if((*board).b[i]==-1)
            {
                // pawn
                if(i/8!=7)
                {
                    spaceControl[layer][i+9][4]++;
                    human += humanMoveBonus[0][i+9];
                }
                if(i/8!=0)
                {
                    spaceControl[layer][i-7][4]++;
                    human += humanMoveBonus[0][i-7];
                }
                
                // en passant
                if(i/8!=7)
                {
                    if((*board).b[i+8]==1)
                    {
                        if((*board).lastMove[0]-(*board).lastMove[1]==2 &&  (*board).lastMove[1]==i+8 && (*board).b[i+9]==0)
                        {
                            spaceControl[layer][i+8][4]++;
                            human += humanMoveBonus[0][i+8];
                        }
                    }
                }
                if(i/8!=0)
                {
                    if((*board).b[i-8]==1)
                    {
                        if((*board).lastMove[0]-(*board).lastMove[1]==2 &&  (*board).lastMove[1]==i-8 && (*board).b[i-7]==0)
                        {
                            spaceControl[layer][i-8][4]++;
                            human += humanMoveBonus[0][i-8];
                        }
                    }
                }
            }
            else if((*board).b[i]==-2)
            {
                // knight
                if(i/8==0)
                {
                    if(i%8==0)
                    {
                        spaceControl[layer][10][3]++;
                        human += humanMoveBonus[1][10];
                        spaceControl[layer][17][3]++;
                        human += humanMoveBonus[1][17];
                    }
                    else if(i%8==1)
                    {
                        spaceControl[layer][11][3]++;
                        human += humanMoveBonus[1][11];
                        spaceControl[layer][16][3]++;
                        human += humanMoveBonus[1][16];
                        spaceControl[layer][18][3]++;
                        human += humanMoveBonus[1][18];
                    }
                    else if(i%8==6)
                    {
                        spaceControl[layer][12][3]++;
                        human += humanMoveBonus[1][12];
                        spaceControl[layer][21][3]++;
                        human += humanMoveBonus[1][21];
                        spaceControl[layer][23][3]++;
                        human += humanMoveBonus[1][23];
                    }
                    else if(i%8==7)
                    {
                        spaceControl[layer][13][3]++;
                        human += humanMoveBonus[1][13];
                        spaceControl[layer][2][3]++;
                        human += humanMoveBonus[1][2];
                    }
                    else
                    {
                        spaceControl[layer][i+6][3]++;
                        human += humanMoveBonus[1][i+6];
                        spaceControl[layer][i+10][3]++;
                        human += humanMoveBonus[1][i+10];
                        spaceControl[layer][i+15][3]++;
                        human += humanMoveBonus[1][i+15];
                        spaceControl[layer][i+17][3]++;
                        human += humanMoveBonus[1][i+17];
                    }
                }
                else if(i/8==1)
                {
                    if(i%8==0)
                    {
                        spaceControl[layer][2][3]++;
                        human += humanMoveBonus[1][2];
                        spaceControl[layer][18][3]++;
                        human += humanMoveBonus[1][18];
                        spaceControl[layer][25][3]++;
                        human += humanMoveBonus[1][25];
                    }
                    else if(i%8==1)
                    {
                        spaceControl[layer][3][3]++;
                        human += humanMoveBonus[1][13];
                        spaceControl[layer][19][3]++;
                        human += humanMoveBonus[1][13];
                        spaceControl[layer][24][3]++;
                        human += humanMoveBonus[1][24];
                        spaceControl[layer][26][3]++;
                        human += humanMoveBonus[1][26];
                    }
                    else if(i%8==6)
                    {
                        spaceControl[layer][4][3]++;
                        human += humanMoveBonus[1][4];
                        spaceControl[layer][20][3]++;
                        human += humanMoveBonus[1][20];
                        spaceControl[layer][29][3]++;
                        human += humanMoveBonus[1][29];
                        spaceControl[layer][31][3]++;
                        human += humanMoveBonus[1][31];
                    }
                    else if(i%8==7)
                    {
                        spaceControl[layer][5][3]++;
                        human += humanMoveBonus[1][5];
                        spaceControl[layer][21][3]++;
                        human += humanMoveBonus[1][21];
                        spaceControl[layer][30][3]++;
                        human += humanMoveBonus[1][30];
                    }
                    else
                    {
                        spaceControl[layer][i-10][3]++;
                        human += humanMoveBonus[1][i-10];
                        spaceControl[layer][i-6][3]++;
                        human += humanMoveBonus[1][i-6];
                        spaceControl[layer][i+6][3]++;
                        human += humanMoveBonus[1][i+6];
                        spaceControl[layer][i+10][3]++;
                        human += humanMoveBonus[1][i+10];
                        spaceControl[layer][i+15][3]++;
                        human += humanMoveBonus[1][i+15];
                        spaceControl[layer][i+17][3]++;
                        human += humanMoveBonus[1][i+17];
                    }
                }
                else if(i/8==6)
                {
                    if(i%8==0)
                    {
                        spaceControl[layer][33][3]++;
                        human += humanMoveBonus[1][33];
                        spaceControl[layer][42][3]++;
                        human += humanMoveBonus[1][42];
                        spaceControl[layer][58][3]++;
                        human += humanMoveBonus[1][58];
                    }
                    else if(i%8==1)
                    {
                        spaceControl[layer][32][3]++;
                        human += humanMoveBonus[1][32];
                        spaceControl[layer][34][3]++;
                        human += humanMoveBonus[1][34];
                        spaceControl[layer][43][3]++;
                        human += humanMoveBonus[1][43];
                        spaceControl[layer][59][3]++;
                        human += humanMoveBonus[1][59];
                    }
                    else if(i%8==6)
                    {
                        spaceControl[layer][37][3]++;
                        human += humanMoveBonus[1][37];
                        spaceControl[layer][39][3]++;
                        human += humanMoveBonus[1][39];
                        spaceControl[layer][4][3]++;
                        human += humanMoveBonus[1][4];
                        spaceControl[layer][60][3]++;
                        human += humanMoveBonus[1][60];
                    }
                    else if(i%8==7)
                    {
                        spaceControl[layer][38][3]++;
                        human += humanMoveBonus[1][38];
                        spaceControl[layer][45][3]++;
                        human += humanMoveBonus[1][45];
                        spaceControl[layer][61][3]++;
                        human += humanMoveBonus[1][61];
                    }
                    else
                    {
                        spaceControl[layer][i-17][3]++;
                        human += humanMoveBonus[1][i-17];
                        spaceControl[layer][i-15][3]++;
                        human += humanMoveBonus[1][i-15];
                        spaceControl[layer][i-10][3]++;
                        human += humanMoveBonus[1][i-10];
                        spaceControl[layer][i-6][3]++;
                        human += humanMoveBonus[1][i-6];
                        spaceControl[layer][i+6][3]++;
                        human += humanMoveBonus[1][i+6];
                        spaceControl[layer][i+10][3]++;
                        human += humanMoveBonus[1][i+10];
                    }
                }
                else if(i/8==7)
                {
                    if(i%8==0)
                    {
                        spaceControl[layer][41][3]++;
                        human += humanMoveBonus[1][41];
                        spaceControl[layer][50][3]++;
                        human += humanMoveBonus[1][50];
                    }
                    else if(i%8==1)
                    {
                        spaceControl[layer][40][3]++;
                        human += humanMoveBonus[1][40];
                        spaceControl[layer][42][3]++;
                        human += humanMoveBonus[1][42];
                        spaceControl[layer][51][3]++;
                        human += humanMoveBonus[1][51];
                    }
                    else if(i%8==6)
                    {
                        spaceControl[layer][45][3]++;
                        human += humanMoveBonus[1][45];
                        spaceControl[layer][47][3]++;
                        human += humanMoveBonus[1][47];
                        spaceControl[layer][52][3]++;
                        human += humanMoveBonus[1][52];
                    }
                    else if(i%8==7)
                    {
                        spaceControl[layer][46][3]++;
                        human += humanMoveBonus[1][46];
                        spaceControl[layer][53][3]++;
                        human += humanMoveBonus[1][53];
                    }
                    else
                    {
                        spaceControl[layer][i-17][3]++;
                        human += humanMoveBonus[1][i-17];
                        spaceControl[layer][i-15][3]++;
                        human += humanMoveBonus[1][i-15];
                        spaceControl[layer][i-10][3]++;
                        human += humanMoveBonus[1][i-10];
                        spaceControl[layer][i-6][3]++;
                        human += humanMoveBonus[1][i-6];
                    }
                }
                else
                {
                    if(i%8==0)
                    {
                        spaceControl[layer][i-15][3]++;
                        human += humanMoveBonus[1][i-15];
                        spaceControl[layer][i-6][3]++;
                        human += humanMoveBonus[1][i-6];
                        spaceControl[layer][i+17][3]++;
                        human += humanMoveBonus[1][i+17];
                    }
                    else if(i%8==1)
                    {
                        spaceControl[layer][i-17][3]++;
                        human += humanMoveBonus[1][i-17];
                        spaceControl[layer][i-15][3]++;
                        human += humanMoveBonus[1][i-15];
                        spaceControl[layer][i-6][3]++;
                        human += humanMoveBonus[1][i-6];
                        spaceControl[layer][i+10][3]++;
                        human += humanMoveBonus[1][i+10];
                        spaceControl[layer][i+15][3]++;
                        human += humanMoveBonus[1][i+15];
                        spaceControl[layer][i+17][3]++;
                        human += humanMoveBonus[1][i+17];
                    }
                    else if(i%8==6)
                    {
                        spaceControl[layer][i-17][3]++;
                        human += humanMoveBonus[1][i-17];
                        spaceControl[layer][i-15][3]++;
                        human += humanMoveBonus[1][i-15];
                        spaceControl[layer][i-10][3]++;
                        human += humanMoveBonus[1][i-10];
                        spaceControl[layer][i+6][3]++;
                        human += humanMoveBonus[1][i+6];
                        spaceControl[layer][i+15][3]++;
                        human += humanMoveBonus[1][i+15];
                        spaceControl[layer][i+17][3]++;
                        human += humanMoveBonus[1][i+17];
                    }
                    else if(i%8==7)
                    {
                        spaceControl[layer][i-17][3]++;
                        human += humanMoveBonus[1][i-17];
                        spaceControl[layer][i-10][3]++;
                        human += humanMoveBonus[1][i-10];
                        spaceControl[layer][i+6][3]++;
                        human += humanMoveBonus[1][i+6];
                        spaceControl[layer][i+15][3]++;
                        human += humanMoveBonus[1][i+15];
                    }
                    else
                    {
                        spaceControl[layer][i-17][3]++;
                        human += humanMoveBonus[1][i-17];
                        spaceControl[layer][i-15][3]++;
                        human += humanMoveBonus[1][i-15];
                        spaceControl[layer][i-10][3]++;
                        human += humanMoveBonus[1][i-10];
                        spaceControl[layer][i-6][3]++;
                        human += humanMoveBonus[1][i-6];
                        spaceControl[layer][i+6][3]++;
                        human += humanMoveBonus[1][i+6];
                        spaceControl[layer][i+10][3]++;
                        human += humanMoveBonus[1][i+10];
                        spaceControl[layer][i+15][3]++;
                        human += humanMoveBonus[1][i+15];
                        spaceControl[layer][i+17][3]++;
                        human += humanMoveBonus[1][i+17];
                    }
                }
            }
            else if((*board).b[i]==-3)
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
                    spaceControl[layer][i-9*j][3]++;
                    human += humanMoveBonus[2][i-9*j];
                    if((*board).b[i-9*j]!=0)
                    {
                        if((*board).b[i-9*j]!=-3)
                        {
                            break;
                        }
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
                    spaceControl[layer][i+7*j][3]++;
                    human += humanMoveBonus[2][i+7*j];
                    if((*board).b[i+7*j]!=0)
                    {
                        if((*board).b[i+7*j]!=-3)
                        {
                            break;
                        }
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
                    spaceControl[layer][i-7*j][3]++;
                    human += humanMoveBonus[2][i-7*j];
                    if((*board).b[i-7*j]!=0)
                    {
                        if((*board).b[i-7*j]==-1)
                        {
                            if(j!=dist)
                            {
                                spaceControl[layer][i-7*(j+1)][3]++;
                                human += humanMoveBonus[2][i-7*(j+1)];
                            }
                            break;
                        }
                        else if((*board).b[i-7*j]!=-3)
                        {
                            break;
                        }
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
                    spaceControl[layer][i+9*j][3]++;
                    human += humanMoveBonus[2][i+9*j];
                    if((*board).b[i+9*j]!=0)
                    {
                        if((*board).b[i+9*j]==-1)
                        {
                            if(j!=dist)
                            {
                                spaceControl[layer][i+9*(j+1)][3]++;
                                human += humanMoveBonus[2][i+9*(j+1)];
                            }
                            break;
                        }
                        else if((*board).b[i+9*j]!=-3)
                        {
                            break;
                        }
                    }
                }
            }
            else if((*board).b[i]==-6)
            {
                // king
                if(i==0)
                {
                    spaceControl[layer][1][0]++;
                    human += humanMoveBonus[5][1];
                    spaceControl[layer][8][0]++;
                    human += humanMoveBonus[5][8];
                    spaceControl[layer][9][0]++;
                    human += humanMoveBonus[5][9];
                }
                else if(i==7)
                {
                    spaceControl[layer][6][0]++;
                    human += humanMoveBonus[5][6];
                    spaceControl[layer][14][0]++;
                    human += humanMoveBonus[5][14];
                    spaceControl[layer][15][0]++;
                    human += humanMoveBonus[5][15];
                }
                else if(i==56)
                {
                    spaceControl[layer][58][0]++;
                    human += humanMoveBonus[5][58];
                    spaceControl[layer][59][0]++;
                    human += humanMoveBonus[5][59];
                    spaceControl[layer][57][0]++;
                    human += humanMoveBonus[5][57];
                }
                else if(i==63)
                {
                    spaceControl[layer][54][0]++;
                    human += humanMoveBonus[5][54];
                    spaceControl[layer][55][0]++;
                    human += humanMoveBonus[5][55];
                    spaceControl[layer][62][0]++;
                    human += humanMoveBonus[5][62];
                }
                else if(i/8==0)
                {
                    spaceControl[layer][i-1][0]++;
                    human += humanMoveBonus[5][i-1];
                    spaceControl[layer][i+1][0]++;
                    human += humanMoveBonus[5][i+1];
                    spaceControl[layer][i+7][0]++;
                    human += humanMoveBonus[5][i+7];
                    spaceControl[layer][i+8][0]++;
                    human += humanMoveBonus[5][i+8];
                    spaceControl[layer][i+9][0]++;
                    human += humanMoveBonus[5][i+9];
                }
                else if(i/8==7)
                {
                    spaceControl[layer][i-1][0]++;
                    human += humanMoveBonus[5][i-1];
                    spaceControl[layer][i+1][0]++;
                    human += humanMoveBonus[5][i+1];
                    spaceControl[layer][i-7][0]++;
                    human += humanMoveBonus[5][i-7];
                    spaceControl[layer][i-8][0]++;
                    human += humanMoveBonus[5][i-8];
                    spaceControl[layer][i-9][0]++;
                    human += humanMoveBonus[5][i-9];
                }
                else if(i%8==0)
                {
                    spaceControl[layer][i-8][0]++;
                    human += humanMoveBonus[5][i-8];
                    spaceControl[layer][i-7][0]++;
                    human += humanMoveBonus[5][i-7];
                    spaceControl[layer][i+1][0]++;
                    human += humanMoveBonus[5][i+1];
                    spaceControl[layer][i+8][0]++;
                    human += humanMoveBonus[5][i+8];
                    spaceControl[layer][i+9][0]++;
                    human += humanMoveBonus[5][i+9];
                }
                else if(i%8==7)
                {
                    spaceControl[layer][i+8][0]++;
                    human += humanMoveBonus[5][i+8];
                    spaceControl[layer][i+7][0]++;
                    human += humanMoveBonus[5][i+7];
                    spaceControl[layer][i-1][0]++;
                    human += humanMoveBonus[5][i-1];
                    spaceControl[layer][i-8][0]++;
                    human += humanMoveBonus[5][i-8];
                    spaceControl[layer][i-9][0]++;
                    human += humanMoveBonus[5][i-9];
                }
                else
                {
                    spaceControl[layer][i-9][0]++;
                    human += humanMoveBonus[5][i-9];
                    spaceControl[layer][i-8][0]++;
                    human += humanMoveBonus[5][i-8];
                    spaceControl[layer][i-7][0]++;
                    human += humanMoveBonus[5][i-7];
                    spaceControl[layer][i-1][0]++;
                    human += humanMoveBonus[5][i-1];
                    spaceControl[layer][i+1][0]++;
                    human += humanMoveBonus[5][i+1];
                    spaceControl[layer][i+7][0]++;
                    human += humanMoveBonus[5][i+7];
                    spaceControl[layer][i+8][0]++;
                    human += humanMoveBonus[5][i+8];
                    spaceControl[layer][i+9][0]++;
                    human += humanMoveBonus[5][i+9];
                }
            }
            if((*board).b[i]==-4)
            {
                // rook
                
                // downward
                dist = i%8;
                for(j=1;j<=dist;j++)
                {
                    spaceControl[layer][i-j][2]++;
                    human += humanMoveBonus[3][i-j];
                    if((*board).b[i-j]!=0)
                    {
                        if((*board).b[i-j]!=-4)
                        {
                            break;
                        }
                    }
                }
                
                // upward
                dist = 7-i%8;
                for(j=1;j<=dist;j++)
                {
                    spaceControl[layer][i+j][2]++;
                    human += humanMoveBonus[3][i+j];
                    if((*board).b[i+j]!=0)
                    {
                        if((*board).b[i+j]!=-4)
                        {
                            break;
                        }
                    }
                }
                
                // leftward
                dist = i/8;
                for(j=1;j<=dist;j++)
                {
                    spaceControl[layer][i-8*j][2]++;
                    human += humanMoveBonus[3][i-8*j];
                    if((*board).b[i-8*j]!=0)
                    {
                        if((*board).b[i-8*j]!=-4)
                        {
                            break;
                        }
                    }
                }
                
                // rightward
                dist = 7-i/8;
                for(j=1;j<=dist;j++)
                {
                    spaceControl[layer][i+8*j][2]++;
                    human += humanMoveBonus[3][i+8*j];
                    if((*board).b[i+8*j]!=0)
                    {
                        if((*board).b[i+8*j]!=-4)
                        {
                            break;
                        }
                    }
                }
            }
            else if((*board).b[i]==-5)
            {
                // queen
                
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
                    spaceControl[layer][i-9*j][1]++;
                    human += humanMoveBonus[4][i-9*j];
                    if((*board).b[i-9*j]!=0)
                    {
                        if((*board).b[i-9*j]!=-3 && (*board).b[i-9*j]!=-5)
                        {
                            break;
                        }
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
                    spaceControl[layer][i+7*j][1]++;
                    human += humanMoveBonus[4][i+7*j];
                    if((*board).b[i+7*j]!=0)
                    {
                        if((*board).b[i+7*j]!=-3 && (*board).b[i+7*j]!=-5)
                        {
                            break;
                        }
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
                    spaceControl[layer][i-7*j][1]++;
                    human += humanMoveBonus[4][i-7*j];
                    if((*board).b[i-7*j]!=0)
                    {
                        if((*board).b[i-7*j]==-1)
                        {
                            if(j!=dist)
                            {
                                spaceControl[layer][i-7*(j+1)][1]++;
                                human += humanMoveBonus[4][i-7*(j+1)];
                            }
                            break;
                        }
                        else if((*board).b[i-7*j]!=-3 && (*board).b[i-7*j]!=-5)
                        {
                            break;
                        }
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
                    spaceControl[layer][i+9*j][1]++;
                    human += humanMoveBonus[4][i+9*j];
                    if((*board).b[i+9*j]!=0)
                    {
                        if((*board).b[i+9*j]==-1)
                        {
                            if(j!=dist)
                            {
                                spaceControl[layer][i+9*(j+1)][1]++;
                                human += humanMoveBonus[4][i+9*(j+1)];
                            }
                            break;
                        }
                        else if((*board).b[i+9*j]!=-3 && (*board).b[i+9*j]!=-5)
                        {
                            break;
                        }
                    }
                }
                
                // downward
                dist = i%8;
                for(j=1;j<=dist;j++)
                {
                    spaceControl[layer][i-j][1]++;
                    human += humanMoveBonus[4][i-j];
                    if((*board).b[i-j]!=0)
                    {
                        if((*board).b[i-j]!=-4 && (*board).b[i-j]!=-5)
                        {
                            break;
                        }
                    }
                }
                
                // upward
                dist = 7-i%8;
                for(j=1;j<=dist;j++)
                {
                    spaceControl[layer][i+j][1]++;
                    human += humanMoveBonus[4][i+j];
                    if((*board).b[i+j]!=0)
                    {
                        if((*board).b[i+j]!=-4 && (*board).b[i+j]!=-5)
                        {
                            break;
                        }
                    }
                }
                
                // leftward
                dist = i/8;
                for(j=1;j<=dist;j++)
                {
                    spaceControl[layer][i-8*j][1]++;
                    human += humanMoveBonus[4][i-8*j];
                    if((*board).b[i-8*j]!=0)
                    {
                        if((*board).b[i-8*j]!=-4 && (*board).b[i-8*j]!=-5)
                        {
                            break;
                        }
                    }
                }
                
                // rightward
                dist = 7-i/8;
                for(j=1;j<=dist;j++)
                {
                    spaceControl[layer][i+8*j][1]++;
                    human += humanMoveBonus[4][i+8*j];
                    if((*board).b[i+8*j]!=0)
                    {
                        if((*board).b[i+8*j]!=-4 && (*board).b[i+8*j]!=-5)
                        {
                            break;
                        }
                    }
                }
            }
        }
    }
    return human;
}

int Evaluator::aiEvalMove(int skip, int layer)
{
    int i;
    int j;
    int dist;
    int cpu = 0;
    for(i=0;i<64;i++)
    {
        if(i==skip)
        {
            continue;
        }
        if((*board).b[i]>0)
        {
            if((*board).b[i]==1)
            {
                // pawn
                if(i/8!=0)
                {
                    spaceControl[layer][i-9][5]++;
                    cpu += cpuMoveBonus[0][i-9];
                }
                if(i/8!=7)
                {
                    spaceControl[layer][i+7][5]++;
                    cpu += cpuMoveBonus[0][i+7];
                }
                
                // en passant
                if(i/8!=7)
                {
                    if((*board).b[i+8]==-1)
                    {
                        if((*board).lastMove[1]-(*board).lastMove[0]==2 &&  (*board).lastMove[1]==i+8 && (*board).b[i+7]==0)
                        {
                            spaceControl[layer][i+8][5]++;
                            cpu += cpuMoveBonus[0][i+8];
                        }
                    }
                }
                if(i/8!=0)
                {
                    if((*board).b[i-8]==-1)
                    {
                        if((*board).lastMove[1]-(*board).lastMove[0]==2 &&  (*board).lastMove[1]==i-8 && (*board).b[i-9]==0)
                        {
                            spaceControl[layer][i-8][5]++;
                            cpu += cpuMoveBonus[0][i-8];
                        }
                    }
                }
            }
            else if((*board).b[i]==2)
            {
                // knight
                if(i/8==0)
                {
                    if(i%8==0)
                    {
                        spaceControl[layer][10][6]++;
                        cpu += cpuMoveBonus[1][10];
                        spaceControl[layer][17][6]++;
                        cpu += cpuMoveBonus[1][17];
                    }
                    else if(i%8==1)
                    {
                        spaceControl[layer][11][6]++;
                        cpu += cpuMoveBonus[1][11];
                        spaceControl[layer][16][6]++;
                        cpu += cpuMoveBonus[1][16];
                        spaceControl[layer][18][6]++;
                        cpu += cpuMoveBonus[1][18];
                    }
                    else if(i%8==6)
                    {
                        spaceControl[layer][12][6]++;
                        cpu += cpuMoveBonus[1][12];
                        spaceControl[layer][21][6]++;
                        cpu += cpuMoveBonus[1][21];
                        spaceControl[layer][23][6]++;
                        cpu += cpuMoveBonus[1][23];
                    }
                    else if(i%8==7)
                    {
                        spaceControl[layer][13][6]++;
                        cpu += cpuMoveBonus[1][13];
                        spaceControl[layer][2][6]++;
                        cpu += cpuMoveBonus[1][2];
                    }
                    else
                    {
                        spaceControl[layer][i+6][6]++;
                        cpu += cpuMoveBonus[1][i+6];
                        spaceControl[layer][i+10][6]++;
                        cpu += cpuMoveBonus[1][i+10];
                        spaceControl[layer][i+15][6]++;
                        cpu += cpuMoveBonus[1][i+15];
                        spaceControl[layer][i+17][6]++;
                        cpu += cpuMoveBonus[1][i+17];
                    }
                }
                else if(i/8==1)
                {
                    if(i%8==0)
                    {
                        spaceControl[layer][2][6]++;
                        cpu += cpuMoveBonus[1][2];
                        spaceControl[layer][18][6]++;
                        cpu += cpuMoveBonus[1][18];
                        spaceControl[layer][25][6]++;
                        cpu += cpuMoveBonus[1][25];
                    }
                    else if(i%8==1)
                    {
                        spaceControl[layer][3][6]++;
                        cpu += cpuMoveBonus[1][3];
                        spaceControl[layer][19][6]++;
                        cpu += cpuMoveBonus[1][19];
                        spaceControl[layer][24][6]++;
                        cpu += cpuMoveBonus[1][24];
                        spaceControl[layer][26][6]++;
                        cpu += cpuMoveBonus[1][26];
                    }
                    else if(i%8==6)
                    {
                        spaceControl[layer][4][6]++;
                        cpu += cpuMoveBonus[1][4];
                        spaceControl[layer][20][6]++;
                        cpu += cpuMoveBonus[1][20];
                        spaceControl[layer][29][6]++;
                        cpu += cpuMoveBonus[1][29];
                        spaceControl[layer][31][6]++;
                        cpu += cpuMoveBonus[1][31];
                    }
                    else if(i%8==7)
                    {
                        spaceControl[layer][5][6]++;
                        cpu += cpuMoveBonus[1][5];
                        spaceControl[layer][21][6]++;
                        cpu += cpuMoveBonus[1][21];
                        spaceControl[layer][30][6]++;
                        cpu += cpuMoveBonus[1][30];
                    }
                    else
                    {
                        spaceControl[layer][i-10][6]++;
                        cpu += cpuMoveBonus[1][i-10];
                        spaceControl[layer][i-6][6]++;
                        cpu += cpuMoveBonus[1][i-6];
                        spaceControl[layer][i+6][6]++;
                        cpu += cpuMoveBonus[1][i+6];
                        spaceControl[layer][i+10][6]++;
                        cpu += cpuMoveBonus[1][i+10];
                        spaceControl[layer][i+15][6]++;
                        cpu += cpuMoveBonus[1][i+15];
                        spaceControl[layer][i+17][6]++;
                        cpu += cpuMoveBonus[1][i+17];
                    }
                }
                else if(i/8==6)
                {
                    if(i%8==0)
                    {
                        spaceControl[layer][33][6]++;
                        cpu += cpuMoveBonus[1][33];
                        spaceControl[layer][42][6]++;
                        cpu += cpuMoveBonus[1][42];
                        spaceControl[layer][58][6]++;
                        cpu += cpuMoveBonus[1][58];
                    }
                    else if(i%8==1)
                    {
                        spaceControl[layer][32][6]++;
                        cpu += cpuMoveBonus[1][32];
                        spaceControl[layer][34][6]++;
                        cpu += cpuMoveBonus[1][34];
                        spaceControl[layer][43][6]++;
                        cpu += cpuMoveBonus[1][43];
                        spaceControl[layer][59][6]++;
                        cpu += cpuMoveBonus[1][59];
                    }
                    else if(i%8==6)
                    {
                        spaceControl[layer][37][6]++;
                        cpu += cpuMoveBonus[1][37];
                        spaceControl[layer][39][6]++;
                        cpu += cpuMoveBonus[1][39];
                        spaceControl[layer][4][6]++;
                        cpu += cpuMoveBonus[1][4];
                        spaceControl[layer][60][6]++;
                        cpu += cpuMoveBonus[1][60];
                    }
                    else if(i%8==7)
                    {
                        spaceControl[layer][38][6]++;
                        cpu += cpuMoveBonus[1][38];
                        spaceControl[layer][45][6]++;
                        cpu += cpuMoveBonus[1][45];
                        spaceControl[layer][61][6]++;
                        cpu += cpuMoveBonus[1][61];
                    }
                    else
                    {
                        spaceControl[layer][i-17][6]++;
                        cpu += cpuMoveBonus[1][i-17];
                        spaceControl[layer][i-15][6]++;
                        cpu += cpuMoveBonus[1][i-15];
                        spaceControl[layer][i-10][6]++;
                        cpu += cpuMoveBonus[1][i-10];
                        spaceControl[layer][i-6][6]++;
                        cpu += cpuMoveBonus[1][i-6];
                        spaceControl[layer][i+6][6]++;
                        cpu += cpuMoveBonus[1][i+6];
                        spaceControl[layer][i+10][6]++;
                        cpu += cpuMoveBonus[1][i+10];
                    }
                }
                else if(i/8==7)
                {
                    if(i%8==0)
                    {
                        spaceControl[layer][41][6]++;
                        cpu += cpuMoveBonus[1][41];
                        spaceControl[layer][50][6]++;
                        cpu += cpuMoveBonus[1][50];
                    }
                    else if(i%8==1)
                    {
                        spaceControl[layer][40][6]++;
                        cpu += cpuMoveBonus[1][40];
                        spaceControl[layer][42][6]++;
                        cpu += cpuMoveBonus[1][42];
                        spaceControl[layer][51][6]++;
                        cpu += cpuMoveBonus[1][51];
                    }
                    else if(i%8==6)
                    {
                        spaceControl[layer][45][6]++;
                        cpu += cpuMoveBonus[1][45];
                        spaceControl[layer][47][6]++;
                        cpu += cpuMoveBonus[1][47];
                        spaceControl[layer][52][6]++;
                        cpu += cpuMoveBonus[1][52];
                    }
                    else if(i%8==7)
                    {
                        spaceControl[layer][46][6]++;
                        cpu += cpuMoveBonus[1][46];
                        spaceControl[layer][53][6]++;
                        cpu += cpuMoveBonus[1][53];
                    }
                    else
                    {
                        spaceControl[layer][i-17][6]++;
                        cpu += cpuMoveBonus[1][i-17];
                        spaceControl[layer][i-15][6]++;
                        cpu += cpuMoveBonus[1][i-15];
                        spaceControl[layer][i-10][6]++;
                        cpu += cpuMoveBonus[1][i-10];
                        spaceControl[layer][i-6][6]++;
                        cpu += cpuMoveBonus[1][i-6];
                    }
                }
                else
                {
                    if(i%8==0)
                    {
                        spaceControl[layer][i-15][6]++;
                        cpu += cpuMoveBonus[1][i-15];
                        spaceControl[layer][i-6][6]++;
                        cpu += cpuMoveBonus[1][i-6];
                        spaceControl[layer][i+17][6]++;
                        cpu += cpuMoveBonus[1][i+17];
                    }
                    else if(i%8==1)
                    {
                        spaceControl[layer][i-17][6]++;
                        cpu += cpuMoveBonus[1][i-17];
                        spaceControl[layer][i-15][6]++;
                        cpu += cpuMoveBonus[1][i-15];
                        spaceControl[layer][i-6][6]++;
                        cpu += cpuMoveBonus[1][i-6];
                        spaceControl[layer][i+10][6]++;
                        cpu += cpuMoveBonus[1][i+10];
                        spaceControl[layer][i+15][6]++;
                        cpu += cpuMoveBonus[1][i+15];
                        spaceControl[layer][i+17][6]++;
                        cpu += cpuMoveBonus[1][i+17];
                    }
                    else if(i%8==6)
                    {
                        spaceControl[layer][i-17][6]++;
                        cpu += cpuMoveBonus[1][i-17];
                        spaceControl[layer][i-15][6]++;
                        cpu += cpuMoveBonus[1][i-15];
                        spaceControl[layer][i-10][6]++;
                        cpu += cpuMoveBonus[1][i-10];
                        spaceControl[layer][i+6][6]++;
                        cpu += cpuMoveBonus[1][i+6];
                        spaceControl[layer][i+15][6]++;
                        cpu += cpuMoveBonus[1][i+15];
                        spaceControl[layer][i+17][6]++;
                        cpu += cpuMoveBonus[1][i+17];
                    }
                    else if(i%8==7)
                    {
                        spaceControl[layer][i-17][6]++;
                        cpu += cpuMoveBonus[1][i-17];
                        spaceControl[layer][i-10][6]++;
                        cpu += cpuMoveBonus[1][i-10];
                        spaceControl[layer][i+6][6]++;
                        cpu += cpuMoveBonus[1][i+6];
                        spaceControl[layer][i+15][6]++;
                        cpu += cpuMoveBonus[1][i+15];
                    }
                    else
                    {
                        spaceControl[layer][i-17][6]++;
                        cpu += cpuMoveBonus[1][i-17];
                        spaceControl[layer][i-15][6]++;
                        cpu += cpuMoveBonus[1][i-15];
                        spaceControl[layer][i-10][6]++;
                        cpu += cpuMoveBonus[1][i-10];
                        spaceControl[layer][i-6][6]++;
                        cpu += cpuMoveBonus[1][i-6];
                        spaceControl[layer][i+6][6]++;
                        cpu += cpuMoveBonus[1][i+6];
                        spaceControl[layer][i+10][6]++;
                        cpu += cpuMoveBonus[1][i+10];
                        spaceControl[layer][i+15][6]++;
                        cpu += cpuMoveBonus[1][i+15];
                        spaceControl[layer][i+17][6]++;
                        cpu += cpuMoveBonus[1][i+17];
                    }
                }
            }
            else if((*board).b[i]==3)
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
                    spaceControl[layer][i-9*j][6]++;
                    cpu += cpuMoveBonus[2][i-9*j];
                    if((*board).b[i-9*j]!=0)
                    {
                        if((*board).b[i-9*j]==1)
                        {
                            if(j!=dist)
                            {
                                spaceControl[layer][i-9*(j+1)][6]++;
                                cpu += cpuMoveBonus[2][i-9*(j+1)];
                            }
                            break;
                        }
                        else if((*board).b[i-9*j]!=3)
                        {
                            break;
                        }
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
                    spaceControl[layer][i+7*j][6]++;
                    cpu += cpuMoveBonus[2][i+7*j];
                    if((*board).b[i+7*j]!=0)
                    {
                        if((*board).b[i+7*j]==1)
                        {
                            if(j!=dist)
                            {
                                spaceControl[layer][i+7*(j+1)][6]++;
                                cpu += cpuMoveBonus[2][i+7*(j+1)];
                            }
                            break;
                        }
                        else if((*board).b[i+7*j]!=3)
                        {
                            break;
                        }
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
                    spaceControl[layer][i-7*j][6]++;
                    cpu += cpuMoveBonus[2][i-7*j];
                    if((*board).b[i-7*j]!=0)
                    {
                        if((*board).b[i-7*j]!=3)
                        {
                            break;
                        }
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
                    spaceControl[layer][i+9*j][6]++;
                    cpu += cpuMoveBonus[2][i+9*j];
                    if((*board).b[i+9*j]!=0)
                    {
                        if((*board).b[i+9*j]!=3)
                        {
                            break;
                        }
                    }
                }
            }
            else if((*board).b[i]==6)
            {
                // king
                if(i==0)
                {
                    spaceControl[layer][1][9]++;
                    cpu += cpuMoveBonus[5][1];
                    spaceControl[layer][8][9]++;
                    cpu += cpuMoveBonus[5][8];
                    spaceControl[layer][9][9]++;
                    cpu += cpuMoveBonus[5][9];
                }
                else if(i==7)
                {
                    spaceControl[layer][6][9]++;
                    cpu += cpuMoveBonus[5][6];
                    spaceControl[layer][14][9]++;
                    cpu += cpuMoveBonus[5][14];
                    spaceControl[layer][15][9]++;
                    cpu += cpuMoveBonus[5][15];
                }
                else if(i==56)
                {
                    spaceControl[layer][58][9]++;
                    cpu += cpuMoveBonus[5][58];
                    spaceControl[layer][59][9]++;
                    cpu += cpuMoveBonus[5][59];
                    spaceControl[layer][57][9]++;
                    cpu += cpuMoveBonus[5][57];
                }
                else if(i==63)
                {
                    spaceControl[layer][54][9]++;
                    cpu += cpuMoveBonus[5][54];
                    spaceControl[layer][55][9]++;
                    cpu += cpuMoveBonus[5][55];
                    spaceControl[layer][62][9]++;
                    cpu += cpuMoveBonus[5][62];
                }
                else if(i/8==0)
                {
                    spaceControl[layer][i-1][9]++;
                    cpu += cpuMoveBonus[5][i-1];
                    spaceControl[layer][i+1][9]++;
                    cpu += cpuMoveBonus[5][i+1];
                    spaceControl[layer][i+7][9]++;
                    cpu += cpuMoveBonus[5][i+7];
                    spaceControl[layer][i+8][9]++;
                    cpu += cpuMoveBonus[5][i+8];
                    spaceControl[layer][i+9][9]++;
                    cpu += cpuMoveBonus[5][i+9];
                }
                else if(i/8==7)
                {
                    spaceControl[layer][i-1][9]++;
                    cpu += cpuMoveBonus[5][i-1];
                    spaceControl[layer][i+1][9]++;
                    cpu += cpuMoveBonus[5][i+1];
                    spaceControl[layer][i-7][9]++;
                    cpu += cpuMoveBonus[5][i-7];
                    spaceControl[layer][i-8][9]++;
                    cpu += cpuMoveBonus[5][i-8];
                    spaceControl[layer][i-9][9]++;
                    cpu += cpuMoveBonus[5][i-9];
                }
                else if(i%8==0)
                {
                    spaceControl[layer][i-8][9]++;
                    cpu += cpuMoveBonus[5][i-8];
                    spaceControl[layer][i-7][9]++;
                    cpu += cpuMoveBonus[5][i-7];
                    spaceControl[layer][i+1][9]++;
                    cpu += cpuMoveBonus[5][i+1];
                    spaceControl[layer][i+8][9]++;
                    cpu += cpuMoveBonus[5][i+8];
                    spaceControl[layer][i+9][9]++;
                    cpu += cpuMoveBonus[5][i+9];
                }
                else if(i%8==7)
                {
                    spaceControl[layer][i+8][9]++;
                    cpu += cpuMoveBonus[5][i+8];
                    spaceControl[layer][i+7][9]++;
                    cpu += cpuMoveBonus[5][i+7];
                    spaceControl[layer][i-1][9]++;
                    cpu += cpuMoveBonus[5][i-1];
                    spaceControl[layer][i-8][9]++;
                    cpu += cpuMoveBonus[5][i-8];
                    spaceControl[layer][i-9][9]++;
                    cpu += cpuMoveBonus[5][i-9];
                }
                else
                {
                    spaceControl[layer][i-9][9]++;
                    cpu += cpuMoveBonus[5][i-9];
                    spaceControl[layer][i-8][9]++;
                    cpu += cpuMoveBonus[5][i-8];
                    spaceControl[layer][i-7][9]++;
                    cpu += cpuMoveBonus[5][i-7];
                    spaceControl[layer][i-1][9]++;
                    cpu += cpuMoveBonus[5][i-1];
                    spaceControl[layer][i+1][9]++;
                    cpu += cpuMoveBonus[5][i+1];
                    spaceControl[layer][i+7][9]++;
                    cpu += cpuMoveBonus[5][i+7];
                    spaceControl[layer][i+8][9]++;
                    cpu += cpuMoveBonus[5][i+8];
                    spaceControl[layer][i+9][9]++;
                    cpu += cpuMoveBonus[5][i+9];
                }
            }
            if((*board).b[i]==4)
            {
                // rook
                
                // downward
                dist = i%8;
                for(j=1;j<=dist;j++)
                {
                    spaceControl[layer][i-j][7]++;
                    cpu += cpuMoveBonus[3][i-j];
                    if((*board).b[i-j]!=0)
                    {
                        if((*board).b[i-j]!=4)
                        {
                            break;
                        }
                    }
                }
                
                // upward
                dist = 7-i%8;
                for(j=1;j<=dist;j++)
                {
                    spaceControl[layer][i+j][7]++;
                    cpu += cpuMoveBonus[3][i+j];
                    if((*board).b[i+j]!=0)
                    {
                        if((*board).b[i+j]!=4)
                        {
                            break;
                        }
                    }
                }
                
                // leftward
                dist = i/8;
                for(j=1;j<=dist;j++)
                {
                    spaceControl[layer][i-8*j][7]++;
                    cpu += cpuMoveBonus[3][i-8*j];
                    if((*board).b[i-8*j]!=0)
                    {
                        if((*board).b[i-8*j]!=4)
                        {
                            break;
                        }
                    }
                }
                
                // rightward
                dist = 7-i/8;
                for(j=1;j<=dist;j++)
                {
                    spaceControl[layer][i+8*j][7]++;
                    cpu += cpuMoveBonus[3][i+8*j];
                    if((*board).b[i+8*j]!=0)
                    {
                        if((*board).b[i+8*j]!=4)
                        {
                            break;
                        }
                    }
                }
            }
            else if((*board).b[i]==5)
            {
                // queen
                
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
                    spaceControl[layer][i-9*j][8]++;
                    cpu += cpuMoveBonus[4][i-9*j];
                    if((*board).b[i-9*j]!=0)
                    {
                        if((*board).b[i-9*j]==1)
                        {
                            if(j!=dist)
                            {
                                spaceControl[layer][i-9*(j+1)][8]++;
                                cpu += cpuMoveBonus[4][i-9*(j+1)];
                            }
                            break;
                        }
                        else if((*board).b[i-9*j]!=3 && (*board).b[i-9*j]!=5)
                        {
                            break;
                        }
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
                    spaceControl[layer][i+7*j][8]++;
                    cpu += cpuMoveBonus[4][i+7*j];
                    if((*board).b[i+7*j]!=0)
                    {
                        if((*board).b[i+7*j]==1)
                        {
                            if(j!=dist)
                            {
                                cpu += cpuMoveBonus[4][i+7*(j+1)];
                                spaceControl[layer][i+7*(j+1)][8]++;
                            }
                            break;
                        }
                        else if((*board).b[i+7*j]!=3 && (*board).b[i+7*j]!=5)
                        {
                            break;
                        }
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
                    spaceControl[layer][i-7*j][8]++;
                    cpu += cpuMoveBonus[4][i-7*j];
                    if((*board).b[i-7*j]!=0)
                    {
                        if((*board).b[i-7*j]!=3 && (*board).b[i-7*j]!=5)
                        {
                            break;
                        }
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
                    spaceControl[layer][i+9*j][8]++;
                    cpu += cpuMoveBonus[4][i+9*j];
                    if((*board).b[i+9*j]!=0)
                    {
                        if((*board).b[i+9*j]!=3 && (*board).b[i+9*j]!=5)
                        {
                            break;
                        }
                    }
                }
                
                // downward
                dist = i%8;
                for(j=1;j<=dist;j++)
                {
                    spaceControl[layer][i-j][8]++;
                    cpu += cpuMoveBonus[4][i-j];
                    if((*board).b[i-j]!=0)
                    {
                        if((*board).b[i-j]!=4 && (*board).b[i-j]!=5)
                        {
                            break;
                        }
                    }
                }
                
                // upward
                dist = 7-i%8;
                for(j=1;j<=dist;j++)
                {
                    spaceControl[layer][i+j][8]++;
                    cpu += cpuMoveBonus[4][i+j];
                    if((*board).b[i+j]!=0)
                    {
                        if((*board).b[i+j]!=4 && (*board).b[i+j]!=5)
                        {
                            break;
                        }
                    }
                }
                
                // leftward
                dist = i/8;
                for(j=1;j<=dist;j++)
                {
                    spaceControl[layer][i-8*j][8]++;
                    cpu += cpuMoveBonus[4][i-8*j];
                    if((*board).b[i-8*j]!=0)
                    {
                        if((*board).b[i-8*j]!=4 && (*board).b[i-8*j]!=5)
                        {
                            break;
                        }
                    }
                }
                
                // rightward
                dist = 7-i/8;
                for(j=1;j<=dist;j++)
                {
                    spaceControl[layer][i+8*j][8]++;
                    cpu += cpuMoveBonus[4][i+8*j];
                    if((*board).b[i+8*j]!=0)
                    {
                        if((*board).b[i+8*j]!=4 && (*board).b[i+8*j]!=5)
                        {
                            break;
                        }
                    }
                }
            }
        }
    }
    return cpu;
}

int Evaluator::aiEvalMoveOneSquare(int i, int layer)
{
    int j;
    int dist;
    int cpu = 0;
    if((*board).b[i]>0)
    {
        if((*board).b[i]==1)
        {
            // pawn
            if(i/8!=0)
            {
                spaceControl[layer][i-9][5]++;
                cpu += cpuMoveBonus[0][i-9];
            }
            if(i/8!=7)
            {
                spaceControl[layer][i+7][5]++;
                cpu += cpuMoveBonus[0][i+7];
            }
            
            // en passant
            if(i/8!=7)
            {
                if((*board).b[i+8]==-1)
                {
                    if((*board).lastMove[1]-(*board).lastMove[0]==2 &&  (*board).lastMove[1]==i+8 && (*board).b[i+7]==0)
                    {
                        spaceControl[layer][i+8][5]++;
                        cpu += cpuMoveBonus[0][i+8];
                    }
                }
            }
            if(i/8!=0)
            {
                if((*board).b[i-8]==-1)
                {
                    if((*board).lastMove[1]-(*board).lastMove[0]==2 &&  (*board).lastMove[1]==i-8 && (*board).b[i-9]==0)
                    {
                        spaceControl[layer][i-8][5]++;
                        cpu += cpuMoveBonus[0][i-8];
                    }
                }
            }
        }
        else if((*board).b[i]==2)
        {
            // knight
            if(i/8==0)
            {
                if(i%8==0)
                {
                    spaceControl[layer][10][6]++;
                    cpu += cpuMoveBonus[1][10];
                    spaceControl[layer][17][6]++;
                    cpu += cpuMoveBonus[1][17];
                }
                else if(i%8==1)
                {
                    spaceControl[layer][11][6]++;
                    cpu += cpuMoveBonus[1][11];
                    spaceControl[layer][16][6]++;
                    cpu += cpuMoveBonus[1][16];
                    spaceControl[layer][18][6]++;
                    cpu += cpuMoveBonus[1][18];
                }
                else if(i%8==6)
                {
                    spaceControl[layer][12][6]++;
                    cpu += cpuMoveBonus[1][12];
                    spaceControl[layer][21][6]++;
                    cpu += cpuMoveBonus[1][21];
                    spaceControl[layer][23][6]++;
                    cpu += cpuMoveBonus[1][23];
                }
                else if(i%8==7)
                {
                    spaceControl[layer][13][6]++;
                    cpu += cpuMoveBonus[1][13];
                    spaceControl[layer][2][6]++;
                    cpu += cpuMoveBonus[1][2];
                }
                else
                {
                    spaceControl[layer][i+6][6]++;
                    cpu += cpuMoveBonus[1][i+6];
                    spaceControl[layer][i+10][6]++;
                    cpu += cpuMoveBonus[1][i+10];
                    spaceControl[layer][i+15][6]++;
                    cpu += cpuMoveBonus[1][i+15];
                    spaceControl[layer][i+17][6]++;
                    cpu += cpuMoveBonus[1][i+17];
                }
            }
            else if(i/8==1)
            {
                if(i%8==0)
                {
                    spaceControl[layer][2][6]++;
                    cpu += cpuMoveBonus[1][2];
                    spaceControl[layer][18][6]++;
                    cpu += cpuMoveBonus[1][18];
                    spaceControl[layer][25][6]++;
                    cpu += cpuMoveBonus[1][25];
                }
                else if(i%8==1)
                {
                    spaceControl[layer][3][6]++;
                    cpu += cpuMoveBonus[1][3];
                    spaceControl[layer][19][6]++;
                    cpu += cpuMoveBonus[1][19];
                    spaceControl[layer][24][6]++;
                    cpu += cpuMoveBonus[1][24];
                    spaceControl[layer][26][6]++;
                    cpu += cpuMoveBonus[1][26];
                }
                else if(i%8==6)
                {
                    spaceControl[layer][4][6]++;
                    cpu += cpuMoveBonus[1][4];
                    spaceControl[layer][20][6]++;
                    cpu += cpuMoveBonus[1][20];
                    spaceControl[layer][29][6]++;
                    cpu += cpuMoveBonus[1][29];
                    spaceControl[layer][31][6]++;
                    cpu += cpuMoveBonus[1][31];
                }
                else if(i%8==7)
                {
                    spaceControl[layer][5][6]++;
                    cpu += cpuMoveBonus[1][5];
                    spaceControl[layer][21][6]++;
                    cpu += cpuMoveBonus[1][21];
                    spaceControl[layer][30][6]++;
                    cpu += cpuMoveBonus[1][30];
                }
                else
                {
                    spaceControl[layer][i-10][6]++;
                    cpu += cpuMoveBonus[1][i-10];
                    spaceControl[layer][i-6][6]++;
                    cpu += cpuMoveBonus[1][i-6];
                    spaceControl[layer][i+6][6]++;
                    cpu += cpuMoveBonus[1][i+6];
                    spaceControl[layer][i+10][6]++;
                    cpu += cpuMoveBonus[1][i+10];
                    spaceControl[layer][i+15][6]++;
                    cpu += cpuMoveBonus[1][i+15];
                    spaceControl[layer][i+17][6]++;
                    cpu += cpuMoveBonus[1][i+17];
                }
            }
            else if(i/8==6)
            {
                if(i%8==0)
                {
                    spaceControl[layer][33][6]++;
                    cpu += cpuMoveBonus[1][33];
                    spaceControl[layer][42][6]++;
                    cpu += cpuMoveBonus[1][42];
                    spaceControl[layer][58][6]++;
                    cpu += cpuMoveBonus[1][58];
                }
                else if(i%8==1)
                {
                    spaceControl[layer][32][6]++;
                    cpu += cpuMoveBonus[1][32];
                    spaceControl[layer][34][6]++;
                    cpu += cpuMoveBonus[1][34];
                    spaceControl[layer][43][6]++;
                    cpu += cpuMoveBonus[1][43];
                    spaceControl[layer][59][6]++;
                    cpu += cpuMoveBonus[1][59];
                }
                else if(i%8==6)
                {
                    spaceControl[layer][37][6]++;
                    cpu += cpuMoveBonus[1][37];
                    spaceControl[layer][39][6]++;
                    cpu += cpuMoveBonus[1][39];
                    spaceControl[layer][4][6]++;
                    cpu += cpuMoveBonus[1][4];
                    spaceControl[layer][60][6]++;
                    cpu += cpuMoveBonus[1][60];
                }
                else if(i%8==7)
                {
                    spaceControl[layer][38][6]++;
                    cpu += cpuMoveBonus[1][38];
                    spaceControl[layer][45][6]++;
                    cpu += cpuMoveBonus[1][45];
                    spaceControl[layer][61][6]++;
                    cpu += cpuMoveBonus[1][61];
                }
                else
                {
                    spaceControl[layer][i-17][6]++;
                    cpu += cpuMoveBonus[1][i-17];
                    spaceControl[layer][i-15][6]++;
                    cpu += cpuMoveBonus[1][i-15];
                    spaceControl[layer][i-10][6]++;
                    cpu += cpuMoveBonus[1][i-10];
                    spaceControl[layer][i-6][6]++;
                    cpu += cpuMoveBonus[1][i-6];
                    spaceControl[layer][i+6][6]++;
                    cpu += cpuMoveBonus[1][i+6];
                    spaceControl[layer][i+10][6]++;
                    cpu += cpuMoveBonus[1][i+10];
                }
            }
            else if(i/8==7)
            {
                if(i%8==0)
                {
                    spaceControl[layer][41][6]++;
                    cpu += cpuMoveBonus[1][41];
                    spaceControl[layer][50][6]++;
                    cpu += cpuMoveBonus[1][50];
                }
                else if(i%8==1)
                {
                    spaceControl[layer][40][6]++;
                    cpu += cpuMoveBonus[1][40];
                    spaceControl[layer][42][6]++;
                    cpu += cpuMoveBonus[1][42];
                    spaceControl[layer][51][6]++;
                    cpu += cpuMoveBonus[1][51];
                }
                else if(i%8==6)
                {
                    spaceControl[layer][45][6]++;
                    cpu += cpuMoveBonus[1][45];
                    spaceControl[layer][47][6]++;
                    cpu += cpuMoveBonus[1][47];
                    spaceControl[layer][52][6]++;
                    cpu += cpuMoveBonus[1][52];
                }
                else if(i%8==7)
                {
                    spaceControl[layer][46][6]++;
                    cpu += cpuMoveBonus[1][46];
                    spaceControl[layer][53][6]++;
                    cpu += cpuMoveBonus[1][53];
                }
                else
                {
                    spaceControl[layer][i-17][6]++;
                    cpu += cpuMoveBonus[1][i-17];
                    spaceControl[layer][i-15][6]++;
                    cpu += cpuMoveBonus[1][i-15];
                    spaceControl[layer][i-10][6]++;
                    cpu += cpuMoveBonus[1][i-10];
                    spaceControl[layer][i-6][6]++;
                    cpu += cpuMoveBonus[1][i-6];
                }
            }
            else
            {
                if(i%8==0)
                {
                    spaceControl[layer][i-15][6]++;
                    cpu += cpuMoveBonus[1][i-15];
                    spaceControl[layer][i-6][6]++;
                    cpu += cpuMoveBonus[1][i-6];
                    spaceControl[layer][i+17][6]++;
                    cpu += cpuMoveBonus[1][i+17];
                }
                else if(i%8==1)
                {
                    spaceControl[layer][i-17][6]++;
                    cpu += cpuMoveBonus[1][i-17];
                    spaceControl[layer][i-15][6]++;
                    cpu += cpuMoveBonus[1][i-15];
                    spaceControl[layer][i-6][6]++;
                    cpu += cpuMoveBonus[1][i-6];
                    spaceControl[layer][i+10][6]++;
                    cpu += cpuMoveBonus[1][i+10];
                    spaceControl[layer][i+15][6]++;
                    cpu += cpuMoveBonus[1][i+16];
                    spaceControl[layer][i+17][6]++;
                    cpu += cpuMoveBonus[1][i+17];
                }
                else if(i%8==6)
                {
                    spaceControl[layer][i-17][6]++;
                    cpu += cpuMoveBonus[1][i-17];
                    spaceControl[layer][i-15][6]++;
                    cpu += cpuMoveBonus[1][i-15];
                    spaceControl[layer][i-10][6]++;
                    cpu += cpuMoveBonus[1][i-10];
                    spaceControl[layer][i+6][6]++;
                    cpu += cpuMoveBonus[1][i+6];
                    spaceControl[layer][i+15][6]++;
                    cpu += cpuMoveBonus[1][i+15];
                    spaceControl[layer][i+17][6]++;
                    cpu += cpuMoveBonus[1][i+17];
                }
                else if(i%8==7)
                {
                    spaceControl[layer][i-17][6]++;
                    cpu += cpuMoveBonus[1][i-17];
                    spaceControl[layer][i-10][6]++;
                    cpu += cpuMoveBonus[1][i-10];
                    spaceControl[layer][i+6][6]++;
                    cpu += cpuMoveBonus[1][i+6];
                    spaceControl[layer][i+15][6]++;
                    cpu += cpuMoveBonus[1][i+15];
                }
                else
                {
                    spaceControl[layer][i-17][6]++;
                    cpu += cpuMoveBonus[1][i-17];
                    spaceControl[layer][i-15][6]++;
                    cpu += cpuMoveBonus[1][i-15];
                    spaceControl[layer][i-10][6]++;
                    cpu += cpuMoveBonus[1][i-10];
                    spaceControl[layer][i-6][6]++;
                    cpu += cpuMoveBonus[1][i-6];
                    spaceControl[layer][i+6][6]++;
                    cpu += cpuMoveBonus[1][i+6];
                    spaceControl[layer][i+10][6]++;
                    cpu += cpuMoveBonus[1][i+10];
                    spaceControl[layer][i+15][6]++;
                    cpu += cpuMoveBonus[1][i+15];
                    spaceControl[layer][i+17][6]++;
                    cpu += cpuMoveBonus[1][i+17];
                }
            }
        }
        else if((*board).b[i]==3)
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
                spaceControl[layer][i-9*j][6]++;
                cpu += cpuMoveBonus[2][i-9*j];
                if((*board).b[i-9*j]!=0)
                {
                    if((*board).b[i-9*j]==1)
                    {
                        if(j!=dist)
                        {
                            spaceControl[layer][i-9*(j+1)][6]++;
                            cpu += cpuMoveBonus[2][i-9*(j+1)];
                        }
                        break;
                    }
                    else if((*board).b[i-9*j]!=3)
                    {
                        break;
                    }
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
                spaceControl[layer][i+7*j][6]++;
                cpu += cpuMoveBonus[2][i+7*j];
                if((*board).b[i+7*j]!=0)
                {
                    if((*board).b[i+7*j]==1)
                    {
                        if(j!=dist)
                        {
                            spaceControl[layer][i+7*(j+1)][6]++;
                            cpu += cpuMoveBonus[2][i+7*(j+1)];
                        }
                        break;
                    }
                    else if((*board).b[i+7*j]!=3)
                    {
                        break;
                    }
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
                spaceControl[layer][i-7*j][6]++;
                cpu += cpuMoveBonus[2][i-7*j];
                if((*board).b[i-7*j]!=0)
                {
                    if((*board).b[i-7*j]!=3)
                    {
                        break;
                    }
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
                spaceControl[layer][i+9*j][6]++;
                cpu += cpuMoveBonus[2][i+9*j];
                if((*board).b[i+9*j]!=0)
                {
                    if((*board).b[i+9*j]!=3)
                    {
                        break;
                    }
                }
            }
        }
        else if((*board).b[i]==6)
        {
            // king
            if(i==0)
            {
                spaceControl[layer][1][9]++;
                cpu += cpuMoveBonus[5][1];
                spaceControl[layer][8][9]++;
                cpu += cpuMoveBonus[5][8];
                spaceControl[layer][9][9]++;
                cpu += cpuMoveBonus[5][9];
            }
            else if(i==7)
            {
                spaceControl[layer][6][9]++;
                cpu += cpuMoveBonus[5][6];
                spaceControl[layer][14][9]++;
                cpu += cpuMoveBonus[5][14];
                spaceControl[layer][15][9]++;
                cpu += cpuMoveBonus[5][15];
            }
            else if(i==56)
            {
                spaceControl[layer][58][9]++;
                cpu += cpuMoveBonus[5][58];
                spaceControl[layer][59][9]++;
                cpu += cpuMoveBonus[5][59];
                spaceControl[layer][57][9]++;
                cpu += cpuMoveBonus[5][57];
            }
            else if(i==63)
            {
                spaceControl[layer][54][9]++;
                cpu += cpuMoveBonus[5][54];
                spaceControl[layer][55][9]++;
                cpu += cpuMoveBonus[5][55];
                spaceControl[layer][62][9]++;
                cpu += cpuMoveBonus[5][62];
            }
            else if(i/8==0)
            {
                spaceControl[layer][i-1][9]++;
                cpu += cpuMoveBonus[5][i-1];
                spaceControl[layer][i+1][9]++;
                cpu += cpuMoveBonus[5][i+1];
                spaceControl[layer][i+7][9]++;
                cpu += cpuMoveBonus[5][i+7];
                spaceControl[layer][i+8][9]++;
                cpu += cpuMoveBonus[5][i+8];
                spaceControl[layer][i+9][9]++;
                cpu += cpuMoveBonus[5][i+9];
            }
            else if(i/8==7)
            {
                spaceControl[layer][i-1][9]++;
                cpu += cpuMoveBonus[5][i-1];
                spaceControl[layer][i+1][9]++;
                cpu += cpuMoveBonus[5][i+1];
                spaceControl[layer][i-7][9]++;
                cpu += cpuMoveBonus[5][i-7];
                spaceControl[layer][i-8][9]++;
                cpu += cpuMoveBonus[5][i-8];
                spaceControl[layer][i-9][9]++;
                cpu += cpuMoveBonus[5][i-9];
            }
            else if(i%8==0)
            {
                spaceControl[layer][i-8][9]++;
                cpu += cpuMoveBonus[5][i-8];
                spaceControl[layer][i-7][9]++;
                cpu += cpuMoveBonus[5][i-7];
                spaceControl[layer][i+1][9]++;
                cpu += cpuMoveBonus[5][i+1];
                spaceControl[layer][i+8][9]++;
                cpu += cpuMoveBonus[5][i+8];
                spaceControl[layer][i+9][9]++;
                cpu += cpuMoveBonus[5][i+9];
            }
            else if(i%8==7)
            {
                spaceControl[layer][i+8][9]++;
                cpu += cpuMoveBonus[5][i+8];
                spaceControl[layer][i+7][9]++;
                cpu += cpuMoveBonus[5][i+7];
                spaceControl[layer][i-1][9]++;
                cpu += cpuMoveBonus[5][i-1];
                spaceControl[layer][i-8][9]++;
                cpu += cpuMoveBonus[5][i-8];
                spaceControl[layer][i-9][9]++;
                cpu += cpuMoveBonus[5][i-9];
            }
            else
            {
                spaceControl[layer][i-9][9]++;
                cpu += cpuMoveBonus[5][i-9];
                spaceControl[layer][i-8][9]++;
                cpu += cpuMoveBonus[5][i-8];
                spaceControl[layer][i-7][9]++;
                cpu += cpuMoveBonus[5][i-7];
                spaceControl[layer][i-1][9]++;
                cpu += cpuMoveBonus[5][i-1];
                spaceControl[layer][i+1][9]++;
                cpu += cpuMoveBonus[5][i+1];
                spaceControl[layer][i+7][9]++;
                cpu += cpuMoveBonus[5][i+7];
                spaceControl[layer][i+8][9]++;
                cpu += cpuMoveBonus[5][i+8];
                spaceControl[layer][i+9][9]++;
                cpu += cpuMoveBonus[5][i+9];
            }
        }
        if((*board).b[i]==4)
        {
            // rook
            
            // downward
            dist = i%8;
            for(j=1;j<=dist;j++)
            {
                spaceControl[layer][i-j][7]++;
                cpu += cpuMoveBonus[3][i-j];
                if((*board).b[i-j]!=0)
                {
                    if((*board).b[i-j]!=4)
                    {
                        break;
                    }
                }
            }
            
            // upward
            dist = 7-i%8;
            for(j=1;j<=dist;j++)
            {
                spaceControl[layer][i+j][7]++;
                cpu += cpuMoveBonus[3][i+j];
                if((*board).b[i+j]!=0)
                {
                    if((*board).b[i+j]!=4)
                    {
                        break;
                    }
                }
            }
            
            // leftward
            dist = i/8;
            for(j=1;j<=dist;j++)
            {
                spaceControl[layer][i-8*j][7]++;
                cpu += cpuMoveBonus[3][i-8*j];
                if((*board).b[i-8*j]!=0)
                {
                    if((*board).b[i-8*j]!=4)
                    {
                        break;
                    }
                }
            }
            
            // rightward
            dist = 7-i/8;
            for(j=1;j<=dist;j++)
            {
                spaceControl[layer][i+8*j][7]++;
                cpu += cpuMoveBonus[3][i+8*j];
                if((*board).b[i+8*j]!=0)
                {
                    if((*board).b[i+8*j]!=4)
                    {
                        break;
                    }
                }
            }
        }
        else if((*board).b[i]==5)
        {
            // queen
            
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
                spaceControl[layer][i-9*j][8]++;
                cpu += cpuMoveBonus[4][i-9*j];
                if((*board).b[i-9*j]!=0)
                {
                    if((*board).b[i-9*j]==1)
                    {
                        if(j!=dist)
                        {
                            spaceControl[layer][i-9*(j+1)][8]++;
                            cpu += cpuMoveBonus[4][i-9*(j+1)];
                        }
                        break;
                    }
                    else if((*board).b[i-9*j]!=3 && (*board).b[i-9*j]!=5)
                    {
                        break;
                    }
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
                spaceControl[layer][i+7*j][8]++;
                cpu += cpuMoveBonus[4][i+7*j];
                if((*board).b[i+7*j]!=0)
                {
                    if((*board).b[i+7*j]==1)
                    {
                        if(j!=dist)
                        {
                            spaceControl[layer][i+7*(j+1)][8]++;
                            cpu += cpuMoveBonus[4][i+7*(j+1)];
                        }
                        break;
                    }
                    else if((*board).b[i+7*j]!=3 && (*board).b[i+7*j]!=5)
                    {
                        break;
                    }
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
                spaceControl[layer][i-7*j][8]++;
                cpu += cpuMoveBonus[4][i-7*j];
                if((*board).b[i-7*j]!=0)
                {
                    if((*board).b[i-7*j]!=3 && (*board).b[i-7*j]!=5)
                    {
                        break;
                    }
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
                spaceControl[layer][i+9*j][8]++;
                cpu += cpuMoveBonus[4][i+9*j];
                if((*board).b[i+9*j]!=0)
                {
                    if((*board).b[i+9*j]!=3 && (*board).b[i+9*j]!=5)
                    {
                        break;
                    }
                }
            }
            
            // downward
            dist = i%8;
            for(j=1;j<=dist;j++)
            {
                spaceControl[layer][i-j][8]++;
                cpu += cpuMoveBonus[4][i-j];
                if((*board).b[i-j]!=0)
                {
                    if((*board).b[i-j]!=4 && (*board).b[i-j]!=5)
                    {
                        break;
                    }
                }
            }
            
            // upward
            dist = 7-i%8;
            for(j=1;j<=dist;j++)
            {
                spaceControl[layer][i+j][8]++;
                cpu += cpuMoveBonus[4][i+j];
                if((*board).b[i+j]!=0)
                {
                    if((*board).b[i+j]!=4 && (*board).b[i+j]!=5)
                    {
                        break;
                    }
                }
            }
            
            // leftward
            dist = i/8;
            for(j=1;j<=dist;j++)
            {
                spaceControl[layer][i-8*j][8]++;
                cpu += cpuMoveBonus[4][i-8*j];
                if((*board).b[i-8*j]!=0)
                {
                    if((*board).b[i-8*j]!=4 && (*board).b[i-8*j]!=5)
                    {
                        break;
                    }
                }
            }
            
            // rightward
            dist = 7-i/8;
            for(j=1;j<=dist;j++)
            {
                spaceControl[layer][i+8*j][8]++;
                cpu += cpuMoveBonus[4][i+8*j];
                if((*board).b[i+8*j]!=0)
                {
                    if((*board).b[i+8*j]!=4 && (*board).b[i+8*j]!=5)
                    {
                        break;
                    }
                }
            }
        }
    }
    return cpu;
}

int Evaluator::humanEvalMoveOneSquare(int i, int layer)
{
    
    int j;
    int dist;
    int human = 0;
    if((*board).b[i]<0)
    {
        if((*board).b[i]==-1)
        {
            // pawn
            if(i/8!=7)
            {
                spaceControl[layer][i+9][4]++;
                human += humanMoveBonus[0][i+9];
            }
            if(i/8!=0)
            {
                spaceControl[layer][i-7][4]++;
                human += humanMoveBonus[0][i-7];
            }
            
            // en passant
            if(i/8!=7)
            {
                if((*board).b[i+8]==1)
                {
                    if((*board).lastMove[0]-(*board).lastMove[1]==2 &&  (*board).lastMove[1]==i+8 && (*board).b[i+9]==0)
                    {
                        spaceControl[layer][i+8][4]++;
                        human += humanMoveBonus[0][i+8];
                    }
                }
            }
            if(i/8!=0)
            {
                if((*board).b[i-8]==1)
                {
                    if((*board).lastMove[0]-(*board).lastMove[1]==2 &&  (*board).lastMove[1]==i-8 && (*board).b[i-7]==0)
                    {
                        spaceControl[layer][i-8][4]++;
                        human += humanMoveBonus[0][i-8];
                    }
                }
            }
        }
        else if((*board).b[i]==-2)
        {
            // knight
            if(i/8==0)
            {
                if(i%8==0)
                {
                    spaceControl[layer][10][3]++;
                    human += humanMoveBonus[1][10];
                    spaceControl[layer][17][3]++;
                    human += humanMoveBonus[1][17];
                }
                else if(i%8==1)
                {
                    spaceControl[layer][11][3]++;
                    human += humanMoveBonus[1][11];
                    spaceControl[layer][16][3]++;
                    human += humanMoveBonus[1][16];
                    spaceControl[layer][18][3]++;
                    human += humanMoveBonus[1][18];
                }
                else if(i%8==6)
                {
                    spaceControl[layer][12][3]++;
                    human += humanMoveBonus[1][12];
                    spaceControl[layer][21][3]++;
                    human += humanMoveBonus[1][21];
                    spaceControl[layer][23][3]++;
                    human += humanMoveBonus[1][23];
                }
                else if(i%8==7)
                {
                    spaceControl[layer][13][3]++;
                    human += humanMoveBonus[1][13];
                    spaceControl[layer][2][3]++;
                    human += humanMoveBonus[1][2];
                }
                else
                {
                    spaceControl[layer][i+6][3]++;
                    human += humanMoveBonus[1][i+6];
                    spaceControl[layer][i+10][3]++;
                    human += humanMoveBonus[1][i+10];
                    spaceControl[layer][i+15][3]++;
                    human += humanMoveBonus[1][i+15];
                    spaceControl[layer][i+17][3]++;
                    human += humanMoveBonus[1][i+17];
                }
            }
            else if(i/8==1)
            {
                if(i%8==0)
                {
                    spaceControl[layer][2][3]++;
                    human += humanMoveBonus[1][2];
                    spaceControl[layer][18][3]++;
                    human += humanMoveBonus[1][18];
                    spaceControl[layer][25][3]++;
                    human += humanMoveBonus[1][25];
                }
                else if(i%8==1)
                {
                    spaceControl[layer][3][3]++;
                    human += humanMoveBonus[1][3];
                    spaceControl[layer][19][3]++;
                    human += humanMoveBonus[1][19];
                    spaceControl[layer][24][3]++;
                    human += humanMoveBonus[1][24];
                    spaceControl[layer][26][3]++;
                    human += humanMoveBonus[1][26];
                }
                else if(i%8==6)
                {
                    spaceControl[layer][4][3]++;
                    human += humanMoveBonus[1][4];
                    spaceControl[layer][20][3]++;
                    human += humanMoveBonus[1][20];
                    spaceControl[layer][29][3]++;
                    human += humanMoveBonus[1][29];
                    spaceControl[layer][31][3]++;
                    human += humanMoveBonus[1][31];
                }
                else if(i%8==7)
                {
                    spaceControl[layer][5][3]++;
                    human += humanMoveBonus[1][5];
                    spaceControl[layer][21][3]++;
                    human += humanMoveBonus[1][21];
                    spaceControl[layer][30][3]++;
                    human += humanMoveBonus[1][30];
                }
                else
                {
                    spaceControl[layer][i-10][3]++;
                    human += humanMoveBonus[1][i-10];
                    spaceControl[layer][i-6][3]++;
                    human += humanMoveBonus[1][i-6];
                    spaceControl[layer][i+6][3]++;
                    human += humanMoveBonus[1][i+6];
                    spaceControl[layer][i+10][3]++;
                    human += humanMoveBonus[1][i+10];
                    spaceControl[layer][i+15][3]++;
                    human += humanMoveBonus[1][i+15];
                    spaceControl[layer][i+17][3]++;
                    human += humanMoveBonus[1][i+17];
                }
            }
            else if(i/8==6)
            {
                if(i%8==0)
                {
                    spaceControl[layer][33][3]++;
                    human += humanMoveBonus[1][33];
                    spaceControl[layer][42][3]++;
                    human += humanMoveBonus[1][42];
                    spaceControl[layer][58][3]++;
                    human += humanMoveBonus[1][58];
                }
                else if(i%8==1)
                {
                    spaceControl[layer][32][3]++;
                    human += humanMoveBonus[1][32];
                    spaceControl[layer][34][3]++;
                    human += humanMoveBonus[1][34];
                    spaceControl[layer][43][3]++;
                    human += humanMoveBonus[1][43];
                    spaceControl[layer][59][3]++;
                    human += humanMoveBonus[1][59];
                }
                else if(i%8==6)
                {
                    spaceControl[layer][37][3]++;
                    human += humanMoveBonus[1][37];
                    spaceControl[layer][39][3]++;
                    human += humanMoveBonus[1][39];
                    spaceControl[layer][4][3]++;
                    human += humanMoveBonus[1][4];
                    spaceControl[layer][60][3]++;
                    human += humanMoveBonus[1][60];
                }
                else if(i%8==7)
                {
                    spaceControl[layer][38][3]++;
                    human += humanMoveBonus[1][38];
                    spaceControl[layer][45][3]++;
                    human += humanMoveBonus[1][45];
                    spaceControl[layer][61][3]++;
                    human += humanMoveBonus[1][61];
                }
                else
                {
                    spaceControl[layer][i-17][3]++;
                    human += humanMoveBonus[1][i-17];
                    spaceControl[layer][i-15][3]++;
                    human += humanMoveBonus[1][i-15];
                    spaceControl[layer][i-10][3]++;
                    human += humanMoveBonus[1][i-10];
                    spaceControl[layer][i-6][3]++;
                    human += humanMoveBonus[1][i-6];
                    spaceControl[layer][i+6][3]++;
                    human += humanMoveBonus[1][i+6];
                    spaceControl[layer][i+10][3]++;
                    human += humanMoveBonus[1][i+10];
                }
            }
            else if(i/8==7)
            {
                if(i%8==0)
                {
                    spaceControl[layer][41][3]++;
                    human += humanMoveBonus[1][41];
                    spaceControl[layer][50][3]++;
                    human += humanMoveBonus[1][50];
                }
                else if(i%8==1)
                {
                    spaceControl[layer][40][3]++;
                    human += humanMoveBonus[1][40];
                    spaceControl[layer][42][3]++;
                    human += humanMoveBonus[1][42];
                    spaceControl[layer][51][3]++;
                    human += humanMoveBonus[1][51];
                }
                else if(i%8==6)
                {
                    spaceControl[layer][45][3]++;
                    human += humanMoveBonus[1][45];
                    spaceControl[layer][47][3]++;
                    human += humanMoveBonus[1][47];
                    spaceControl[layer][52][3]++;
                    human += humanMoveBonus[1][52];
                }
                else if(i%8==7)
                {
                    spaceControl[layer][46][3]++;
                    human += humanMoveBonus[1][46];
                    spaceControl[layer][53][3]++;
                    human += humanMoveBonus[1][53];
                }
                else
                {
                    spaceControl[layer][i-17][3]++;
                    human += humanMoveBonus[1][i-17];
                    spaceControl[layer][i-15][3]++;
                    human += humanMoveBonus[1][i-15];
                    spaceControl[layer][i-10][3]++;
                    human += humanMoveBonus[1][i-10];
                    spaceControl[layer][i-6][3]++;
                    human += humanMoveBonus[1][i-6];
                }
            }
            else
            {
                if(i%8==0)
                {
                    spaceControl[layer][i-15][3]++;
                    human += humanMoveBonus[1][i-15];
                    spaceControl[layer][i-6][3]++;
                    human += humanMoveBonus[1][i-6];
                    spaceControl[layer][i+17][3]++;
                    human += humanMoveBonus[1][i+17];
                }
                else if(i%8==1)
                {
                    spaceControl[layer][i-17][3]++;
                    human += humanMoveBonus[1][i-17];
                    spaceControl[layer][i-15][3]++;
                    human += humanMoveBonus[1][i-15];
                    spaceControl[layer][i-6][3]++;
                    human += humanMoveBonus[1][i-6];
                    spaceControl[layer][i+10][3]++;
                    human += humanMoveBonus[1][i+10];
                    spaceControl[layer][i+15][3]++;
                    human += humanMoveBonus[1][i+15];
                    spaceControl[layer][i+17][3]++;
                    human += humanMoveBonus[1][i+17];
                }
                else if(i%8==6)
                {
                    spaceControl[layer][i-17][3]++;
                    human += humanMoveBonus[1][i-17];
                    spaceControl[layer][i-15][3]++;
                    human += humanMoveBonus[1][i-15];
                    spaceControl[layer][i-10][3]++;
                    human += humanMoveBonus[1][i-10];
                    spaceControl[layer][i+6][3]++;
                    human += humanMoveBonus[1][i+6];
                    spaceControl[layer][i+15][3]++;
                    human += humanMoveBonus[1][i+15];
                    spaceControl[layer][i+17][3]++;
                    human += humanMoveBonus[1][i+17];
                }
                else if(i%8==7)
                {
                    spaceControl[layer][i-17][3]++;
                    human += humanMoveBonus[1][i-17];
                    spaceControl[layer][i-10][3]++;
                    human += humanMoveBonus[1][i-10];
                    spaceControl[layer][i+6][3]++;
                    human += humanMoveBonus[1][i+6];
                    spaceControl[layer][i+15][3]++;
                    human += humanMoveBonus[1][i+15];
                }
                else
                {
                    spaceControl[layer][i-17][3]++;
                    human += humanMoveBonus[1][i-17];
                    spaceControl[layer][i-15][3]++;
                    human += humanMoveBonus[1][i-15];
                    spaceControl[layer][i-10][3]++;
                    human += humanMoveBonus[1][i-10];
                    spaceControl[layer][i-6][3]++;
                    human += humanMoveBonus[1][i-6];
                    spaceControl[layer][i+6][3]++;
                    human += humanMoveBonus[1][i+6];
                    spaceControl[layer][i+10][3]++;
                    human += humanMoveBonus[1][i+10];
                    spaceControl[layer][i+15][3]++;
                    human += humanMoveBonus[1][i+15];
                    spaceControl[layer][i+17][3]++;
                    human += humanMoveBonus[1][i+17];
                }
            }
        }
        else if((*board).b[i]==-3)
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
                spaceControl[layer][i-9*j][3]++;
                human += humanMoveBonus[2][i-9*j];
                if((*board).b[i-9*j]!=0)
                {
                    if((*board).b[i-9*j]!=-3)
                    {
                        break;
                    }
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
                spaceControl[layer][i+7*j][3]++;
                human += humanMoveBonus[2][i+7*j];
                if((*board).b[i+7*j]!=0)
                {
                    if((*board).b[i+7*j]!=-3)
                    {
                        break;
                    }
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
                spaceControl[layer][i-7*j][3]++;
                human += humanMoveBonus[2][i-7*j];
                if((*board).b[i-7*j]!=0)
                {
                    if((*board).b[i-7*j]==-1)
                    {
                        if(j!=dist)
                        {
                            spaceControl[layer][i-7*(j+1)][3]++;
                            human += humanMoveBonus[2][i-7*(j+1)];
                        }
                        break;
                    }
                    else if((*board).b[i-7*j]!=-3)
                    {
                        break;
                    }
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
                spaceControl[layer][i+9*j][3]++;
                human += humanMoveBonus[2][i+9*j];
                if((*board).b[i+9*j]!=0)
                {
                    if((*board).b[i+9*j]==-1)
                    {
                        if(j!=dist)
                        {
                            spaceControl[layer][i+9*(j+1)][3]++;
                            human += humanMoveBonus[2][i-9*(j+1)];
                        }
                        break;
                    }
                    else if((*board).b[i+9*j]!=-3)
                    {
                        break;
                    }
                }
            }
        }
        else if((*board).b[i]==-6)
        {
            // king
            if(i==0)
            {
                spaceControl[layer][1][0]++;
                human += humanMoveBonus[5][1];
                spaceControl[layer][8][0]++;
                human += humanMoveBonus[5][8];
                spaceControl[layer][9][0]++;
                human += humanMoveBonus[5][9];
            }
            else if(i==7)
            {
                spaceControl[layer][6][0]++;
                human += humanMoveBonus[5][6];
                spaceControl[layer][14][0]++;
                human += humanMoveBonus[5][14];
                spaceControl[layer][15][0]++;
                human += humanMoveBonus[5][15];
            }
            else if(i==56)
            {
                spaceControl[layer][58][0]++;
                human += humanMoveBonus[5][58];
                spaceControl[layer][59][0]++;
                human += humanMoveBonus[5][59];
                spaceControl[layer][57][0]++;
                human += humanMoveBonus[5][57];
            }
            else if(i==63)
            {
                spaceControl[layer][54][0]++;
                human += humanMoveBonus[5][54];
                spaceControl[layer][55][0]++;
                human += humanMoveBonus[5][55];
                spaceControl[layer][62][0]++;
                human += humanMoveBonus[5][62];
            }
            else if(i/8==0)
            {
                spaceControl[layer][i-1][0]++;
                human += humanMoveBonus[5][i-1];
                spaceControl[layer][i+1][0]++;
                human += humanMoveBonus[5][i+1];
                spaceControl[layer][i+7][0]++;
                human += humanMoveBonus[5][i+7];
                spaceControl[layer][i+8][0]++;
                human += humanMoveBonus[5][i+8];
                spaceControl[layer][i+9][0]++;
                human += humanMoveBonus[5][i+9];
            }
            else if(i/8==7)
            {
                spaceControl[layer][i-1][0]++;
                human += humanMoveBonus[5][i-1];
                spaceControl[layer][i+1][0]++;
                human += humanMoveBonus[5][i+1];
                spaceControl[layer][i-7][0]++;
                human += humanMoveBonus[5][i-7];
                spaceControl[layer][i-8][0]++;
                human += humanMoveBonus[5][i-8];
                spaceControl[layer][i-9][0]++;
                human += humanMoveBonus[5][i-9];
            }
            else if(i%8==0)
            {
                spaceControl[layer][i-8][0]++;
                human += humanMoveBonus[5][i-8];
                spaceControl[layer][i-7][0]++;
                human += humanMoveBonus[5][i-7];
                spaceControl[layer][i+1][0]++;
                human += humanMoveBonus[5][i+1];
                spaceControl[layer][i+8][0]++;
                human += humanMoveBonus[5][i+8];
                spaceControl[layer][i+9][0]++;
                human += humanMoveBonus[5][i+9];
            }
            else if(i%8==7)
            {
                spaceControl[layer][i+8][0]++;
                human += humanMoveBonus[5][i+8];
                spaceControl[layer][i+7][0]++;
                human += humanMoveBonus[5][i+7];
                spaceControl[layer][i-1][0]++;
                human += humanMoveBonus[5][i-1];
                spaceControl[layer][i-8][0]++;
                human += humanMoveBonus[5][i-8];
                spaceControl[layer][i-9][0]++;
                human += humanMoveBonus[5][i-9];
            }
            else
            {
                spaceControl[layer][i-9][0]++;
                human += humanMoveBonus[5][i-9];
                spaceControl[layer][i-8][0]++;
                human += humanMoveBonus[5][i-8];
                spaceControl[layer][i-7][0]++;
                human += humanMoveBonus[5][i-7];
                spaceControl[layer][i-1][0]++;
                human += humanMoveBonus[5][i-1];
                spaceControl[layer][i+1][0]++;
                human += humanMoveBonus[5][i+1];
                spaceControl[layer][i+7][0]++;
                human += humanMoveBonus[5][i+7];
                spaceControl[layer][i+8][0]++;
                human += humanMoveBonus[5][i+8];
                spaceControl[layer][i+9][0]++;
                human += humanMoveBonus[5][i+9];
            }
        }
        if((*board).b[i]==-4)
        {
            // rook
            
            // downward
            dist = i%8;
            for(j=1;j<=dist;j++)
            {
                spaceControl[layer][i-j][2]++;
                human += humanMoveBonus[3][i-j];
                if((*board).b[i-j]!=0)
                {
                    if((*board).b[i-j]!=-4)
                    {
                        break;
                    }
                }
            }
            
            // upward
            dist = 7-i%8;
            for(j=1;j<=dist;j++)
            {
                spaceControl[layer][i+j][2]++;
                human += humanMoveBonus[3][i+j];
                if((*board).b[i+j]!=0)
                {
                    if((*board).b[i+j]!=-4)
                    {
                        break;
                    }
                }
            }
            
            // leftward
            dist = i/8;
            for(j=1;j<=dist;j++)
            {
                spaceControl[layer][i-8*j][2]++;
                human += humanMoveBonus[3][i-8*j];
                if((*board).b[i-8*j]!=0)
                {
                    if((*board).b[i-8*j]!=-4)
                    {
                        break;
                    }
                }
            }
            
            // rightward
            dist = 7-i/8;
            for(j=1;j<=dist;j++)
            {
                spaceControl[layer][i+8*j][2]++;
                human += humanMoveBonus[3][i+8*j];
                if((*board).b[i+8*j]!=0)
                {
                    if((*board).b[i+8*j]!=-4)
                    {
                        break;
                    }
                }
            }
        }
        else if((*board).b[i]==-5)
        {
            // queen
            
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
                spaceControl[layer][i-9*j][1]++;
                human += humanMoveBonus[4][i-9*j];
                if((*board).b[i-9*j]!=0)
                {
                    if((*board).b[i-9*j]!=-3 && (*board).b[i-9*j]!=-5)
                    {
                        break;
                    }
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
                spaceControl[layer][i+7*j][1]++;
                human += humanMoveBonus[4][i+7*j];
                if((*board).b[i+7*j]!=0)
                {
                    if((*board).b[i+7*j]!=-3 && (*board).b[i+7*j]!=-5)
                    {
                        break;
                    }
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
                spaceControl[layer][i-7*j][1]++;
                human += humanMoveBonus[4][i-7*j];
                if((*board).b[i-7*j]!=0)
                {
                    if((*board).b[i-7*j]==-1)
                    {
                        if(j!=dist)
                        {
                            spaceControl[layer][i-7*(j+1)][1]++;
                            human += humanMoveBonus[4][i-7*(j+1)];
                        }
                        break;
                    }
                    else if((*board).b[i-7*j]!=-3 && (*board).b[i-7*j]!=-5)
                    {
                        break;
                    }
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
                spaceControl[layer][i+9*j][1]++;
                human += humanMoveBonus[4][i+9*j];
                if((*board).b[i+9*j]!=0)
                {
                    if((*board).b[i+9*j]==-1)
                    {
                        if(j!=dist)
                        {
                            spaceControl[layer][i+9*(j+1)][1]++;
                            human += humanMoveBonus[4][i+9*(j+1)];
                        }
                        break;
                    }
                    else if((*board).b[i+9*j]!=-3 && (*board).b[i+9*j]!=-5)
                    {
                        break;
                    }
                }
            }
            
            // downward
            dist = i%8;
            for(j=1;j<=dist;j++)
            {
                spaceControl[layer][i-j][1]++;
                human += humanMoveBonus[4][i-j];
                if((*board).b[i-j]!=0)
                {
                    if((*board).b[i-j]!=-4 && (*board).b[i-j]!=-5)
                    {
                        break;
                    }
                }
            }
            
            // upward
            dist = 7-i%8;
            for(j=1;j<=dist;j++)
            {
                spaceControl[layer][i+j][1]++;
                human += humanMoveBonus[4][i+j];
                if((*board).b[i+j]!=0)
                {
                    if((*board).b[i+j]!=-4 && (*board).b[i+j]!=-5)
                    {
                        break;
                    }
                }
            }
            
            // leftward
            dist = i/8;
            for(j=1;j<=dist;j++)
            {
                spaceControl[layer][i-8*j][1]++;
                human += humanMoveBonus[4][i-8*j];
                if((*board).b[i-8*j]!=0)
                {
                    if((*board).b[i-8*j]!=-4 && (*board).b[i-8*j]!=-5)
                    {
                        break;
                    }
                }
            }
            
            // rightward
            dist = 7-i/8;
            for(j=1;j<=dist;j++)
            {
                spaceControl[layer][i+8*j][1]++;
                human += humanMoveBonus[4][i+8*j];
                if((*board).b[i+8*j]!=0)
                {
                    if((*board).b[i+8*j]!=-4 && (*board).b[i+8*j]!=-5)
                    {
                        break;
                    }
                }
            }
        }
    }
    return human;
}

int Evaluator::pieceToValue(int num)
{
    if(num == 0)
    {
        return 0;
    }
    if(num==-6)
    {
        return -1*exchangeValue[4];
    }
    if(num==-5)
    {
        return -1*exchangeValue[3];
    }
    else if(num==-4)
    {
        return -1*exchangeValue[2];
    }
    else if(num==-3)
    {
        return -1*exchangeValue[1];
    }
    else if(num==-2)
    {
        return -1*exchangeValue[1];
    }
    else if(num==-1)
    {
        return -1*exchangeValue[0];
    }
    else if(num==1)
    {
        return exchangeValue[0];
    }
    else if(num==2)
    {
        return exchangeValue[1];
    }
    else if(num==3)
    {
        return exchangeValue[1];
    }
    else if(num==4)
    {
        return exchangeValue[2];
    }
    else if(num==5)
    {
        return exchangeValue[3];
    }
    else if(num==6)
    {
        return exchangeValue[4];
    }
    return 0;
}

int Evaluator::pieceSafe(int sq, int layer)
{
    if(board->b[sq] == 0)
    {
        return 0;
    }
    
    /*
    int precomputedValue = exchangeSimulator.getPrecomputedValue(&spaceControl[layer][sq][0], board->b[sq]);
    if(precomputedValue != -1000000)
    {
        return precomputedValue;
    }
    */
    
    if((*board).b[sq]>0)
    {
        // cpu's piece
        int cpuX = 0;
        int cpuCounter = 0;
        int humanX = 0;
        int humanCounter = 0;
        float currentOnSquare = pieceToValue((*board).b[sq]);
        exchangeOption[0] = 0;
        exchangeOptionLength = 1;
        for(int i=0;i<100;i++)
        {
            if(i%2==0)
            {
                // human's turn
                for(humanX=humanX;humanX<5;humanX++)
                {
                    if(spaceControl[layer][sq][4-humanX]-humanCounter>0)
                    {
                        exchangeOption[exchangeOptionLength] = exchangeOption[exchangeOptionLength-1]-currentOnSquare;
                        exchangeOptionLength++;
                        currentOnSquare = exchangeValue[humanX];
                        humanCounter++;
                        break;
                    }
                    humanCounter = 0;
                }
                if(humanX==5)
                {
                    // has run out of pieces
                    break;
                }
            }
            else
            {
                // cpu's turn
                for(cpuX=cpuX;cpuX<5;cpuX++)
                {
                    if(spaceControl[layer][sq][5+cpuX]-cpuCounter>0)
                    {
                        exchangeOption[exchangeOptionLength] = exchangeOption[exchangeOptionLength-1]+currentOnSquare;
                        exchangeOptionLength++;
                        currentOnSquare = exchangeValue[cpuX];
                        cpuCounter++;
                        break;
                    }
                    cpuCounter = 0;
                }
                if(cpuX==5)
                {
                    // has run out of pieces
                    break;
                }
            }
        }
        
        return exchangeTree(0);
    }
    else
    {
        // human piece
        int i;
        int cpuX = 0;
        int cpuCounter = 0;
        int humanX = 0;
        int humanCounter = 0;
        float currentOnSquare = abs(pieceToValue((int) board->b[sq]));
        exchangeOption[0] = 0;
        exchangeOptionLength = 1;
        for(i=0;i<100;i++)
        {
            if(i%2==1)
            {
                // human's turn
                for(humanX=humanX;humanX<5;humanX++)
                {
                    if(spaceControl[layer][sq][4-humanX]-humanCounter>0)
                    {
                        exchangeOption[exchangeOptionLength] = exchangeOption[exchangeOptionLength-1]+currentOnSquare;
                        exchangeOptionLength++;
                        currentOnSquare = exchangeValue[humanX];
                        humanCounter++;
                        break;
                    }
                    humanCounter = 0;
                }
                if(humanX==5)
                {
                    // has run out of pieces
                    break;
                }
            }
            else
            {
                // cpu's turn
                for(cpuX=cpuX;cpuX<5;cpuX++)
                {
                    if(spaceControl[layer][sq][5+cpuX]-cpuCounter>0)
                    {
                        exchangeOption[exchangeOptionLength] = exchangeOption[exchangeOptionLength-1]-currentOnSquare;
                        exchangeOptionLength++;
                        currentOnSquare = exchangeValue[cpuX];
                        cpuCounter++;
                        break;
                    }
                    cpuCounter = 0;
                }
                if(cpuX==5)
                {
                    // has run out of pieces
                    break;
                }
            }
        }
        return exchangeTree(0);
    }
}

int Evaluator::exchangeTree(int layer)
{
    if(layer==exchangeOptionLength-1)
    {
        return exchangeOption[exchangeOptionLength-1];
    }
    int alternative = exchangeTree(layer+1);
    
    if(layer%2==0)
    {
        if(alternative<exchangeOption[layer])
        {
            return alternative;
        }
        else
        {
            return exchangeOption[layer];
        }
    }
    else
    {
        if(alternative>exchangeOption[layer])
        {
            return alternative;
        }
        else
        {
            return exchangeOption[layer];
        }
    }
    return 0;
}
