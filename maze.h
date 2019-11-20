#ifndef MAZE_H
#define MAZE_H
#include<QTime>
#include<qqueue.h>
const int MAXLENGTH = 19;
static const int MAXNUMBER=10;
static const int ROUTE=0;
static const int WALL=11;
static const int LEFT=-1,RIGHT=-2,UP=-3,DOWN=-4;

class MyMaze{
private:
    int maze[MAXLENGTH][MAXLENGTH];
public:
    MyMaze(){
        for (int i=0;i<MAXLENGTH;i++)
            for(int j=0;j<MAXLENGTH;j++)
                maze[i][j]=ROUTE;
}
    void MazeToAry(int* s) {
        for (int i=0;i<MAXLENGTH;i++)
            for (int j=0;j<MAXLENGTH;j++)
                s[i*MAXLENGTH+j]=maze[i][j];
    }

    void CreateMazeWithWeights(){
        qsrand((uint)QTime::currentTime().msec());
        for (int i=1;i<MAXLENGTH-1;i++)
            for (int j=1;j<MAXLENGTH-1;j++){
                maze[i][j]=qrand()%MAXNUMBER + 1;
            }
        maze[1][1]=ROUTE;maze[MAXLENGTH-2][MAXLENGTH-2]=ROUTE;
        for (int i=0;i<MAXLENGTH;i++)
        {
            maze[i][MAXLENGTH-1]=WALL;
            maze[MAXLENGTH-1][i]=WALL;
            maze[0][i]=WALL;
            maze[i][0]=WALL;
        }
    }
};
#endif // MAZE_H
