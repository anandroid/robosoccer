//
// Created by Anand kumar on 2019-04-24.
//

#ifndef UTAUSTINVILLABASE_TEAMMODE_H
#define UTAUSTINVILLABASE_TEAMMODE_H

#include "../headers/headers.h"

class TeamMode{

private:
    Mode mode;
    int ourgoals;
    int opponentgoals;
    long currentTime;
    ActionAllocator actionAllocator;
    FieldRange getAttackFieldRange(int playerPositionNumber);


public:
    TeamMode();
    void setCurrentTime(long time):
    void setGoals(int ourgoals,int opponentgoals);
    Mode getMode();
    ActionAllocator getActionallocator();
    FieldRange getFieldRange(int playerPositionNumber);
    FieldRange getFormationFieldRange(int playerPositionNumber);


};

#endif //UTAUSTINVILLABASE_TEAMMODE_H
