//
// Created by Anand kumar on 2019-04-28.
//

#ifndef UTAUSTINVILLABASE_ACTION_H
#define UTAUSTINVILLABASE_ACTION_H

#include "../headers/headers.h"
#include "../math/vecposition.h"


class Action{

private:
    int actionType;
    VecPosition targetPosition;
    int kickType;
    VecPosition initialBallPosition;

public:
    /* Action(){

     }

     void Init(bool mIsKickingAction,VecPosition mTargetPosition,int mKickType){
         isKickingAction= mIsKickingAction;
         targetPosition = mTargetPosition;
         kickType = mKickType;
     }*/
    Action();
    void Init(int actiontype,VecPosition targetPosition,int kickType,VecPosition initialBallPosition);
    int  getKickType();
    VecPosition getTargetPosition();
    VecPosition getInitialBallPosition();

};


#endif //UTAUSTINVILLABASE_ACTION_H
