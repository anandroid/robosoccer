//
// Created by zoro on 4/23/19.
//

#ifndef UTAUSTINVILLABASE_PLAYER_H
#define UTAUSTINVILLABASE_PLAYER_H


#include "../math/vecposition.h"
#include "fieldRange.h"

class Player {

private:
    //Player(int mPlayerNumber, int mPlayerPositionNumber, int mAggressiveRating, FieldRange range);

    int playerNumber;
    int playerPositionNumber;
    VecPosition position;
    int agressiveRating;
    SkillType actionInvolved;
    FieldRange range;
    bool isInvolvedInAction;
    bool isInitialized;
public :
    Player();
    void Init(int mPlayerNumber, int mPlayerPositionNumber, int mAggressiveRating,FieldRange range);

    void setAggressiveRating(int aggressiveRating);
    void setActionInvolved(SkillType mAction);
    void setRange(FieldRange mRange);
    void setPlayerPosition(VecPosition mPosition);
    void setIsInvolvedInAction(bool value);



    int getPlayerNumber();
    int getPlayerPositionNumber();
    VecPosition getPosition();
    int getAggressiveRating();
    SkillType getActionInvolved();
    FieldRange getRange();
    bool getIsInvolvedInAction();
    bool getIsInitialized();

};


#endif //UTAUSTINVILLABASE_PLAYER_H
