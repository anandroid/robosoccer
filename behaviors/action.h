//
// Created by Anand kumar on 2019-04-28.
//

#ifndef UTAUSTINVILLABASE_ACTION_H
#define UTAUSTINVILLABASE_ACTION_H


class Action{

private:
    bool isKickingAction;
    VecPosition targetPosition;
    int kickType;

public:
    Action();
    void Init(bool isKickingAction,VecPosition targetPosition,int kickType);
    bool getIsKickingAction();
    int  getKickType();
    VecPosition getTargetPosition();

};


#endif //UTAUSTINVILLABASE_ACTION_H
