//
// Created by Anand kumar on 2019-04-24.
//

#ifndef UTAUSTINVILLABASE_TEAMMODE_H
#define UTAUSTINVILLABASE_TEAMMODE_H



class TeamMode{

private:
    Mode mode;
    int ourgoals;
    int opponentgoals;
    long currentTime;
    FieldRange getAttackFieldRange(int playerPositionNumber);


public:
    TeamMode();
    void setCurrentTime(long time);
    void setGoals(int ourgoals,int opponentgoals);
    Mode getMode();
    VecPosition getNextTargetPosition(int aggressiveRating,VecPosition currentPosition);
    FieldRange getFieldRange(int playerPositionNumber);
    FieldRange getFormationFieldRange(int playerPositionNumber);


};

#endif //UTAUSTINVILLABASE_TEAMMODE_H
