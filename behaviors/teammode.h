//
// Created by Anand kumar on 2019-04-24.
//

#ifndef UTAUSTINVILLABASE_TEAMMODE_H
#define UTAUSTINVILLABASE_TEAMMODE_H

enum Mode{Defence,Normal,Attack,SetPiece};

class TeamMode{

private:
    Mode mode;
    int ourgoals;
    int opponentgoals;
    long currentTime;
    ActionAllocator actionAllocator;


public:
    void setCurrentTime(long time):
    void setGoals(int ourgoals,int opponentgoals);
    Mode getMode();
    ActionAllocator getActionallocator();

};

#endif //UTAUSTINVILLABASE_TEAMMODE_H
