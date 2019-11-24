//
// Created by Anand kumar on 2019-04-28.
//

#include "action.h"
#include "../headers/headers.h"
#include "../math/vecposition.h"


enum actiontype{IS_GOAL_KICKING,IS_PASSING,IS_MOVING,IS_DRIBBLING};

Action::Action(){

}

void Action::Init(int mActionType,VecPosition mTargetPosition,int mKickType,
                  VecPosition mInitialBallPosition){
    actionType= mActionType;
    targetPosition = mTargetPosition;
    kickType = mKickType;
    initialBallPosition = mInitialBallPosition;

}

int Action::getActionType(){
    return actionType;
}

int Action::getKickType(){
    return kickType;
}

VecPosition Action::getTargetPosition(){
    return targetPosition;
}

VecPosition Action::getInitialBallPosition(){
    return initialBallPosition;
}
