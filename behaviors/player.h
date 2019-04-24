//
// Created by zoro on 4/23/19.
//

#ifndef UTAUSTINVILLABASE_PLAYER_H
#define UTAUSTINVILLABASE_PLAYER_H



class player {

private:
    int playerNumber;
    int playerPositionNumber;
    VecPosition position;
    int agressiveRating;
    SkillType actionInvolved;
    FieldRange range;
public :
    Player(int playerNumber,int playerPositionNumber,int aggressiveRating);

    void setAggressiveRating();
    void setActionInvolved();
    void setRange();
    void setPlayerPosition();


    int getPlayerNumber();
    int getPlayerPositionNumber();
    VecPosition getPlayerPosition();
    int getAggressiveRating();
    SkillType getActionInvolved();
    FieldRange getRange();

};


#endif //UTAUSTINVILLABASE_PLAYER_H
