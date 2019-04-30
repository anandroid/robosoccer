//
// Created by Anand kumar on 2019-04-28.
//

#include "action.h"
#include "../headers/headers.h"
#include "../math/vecposition.h"

Action::Action(){

}

void Action::Init(bool mIsKickingAction,VecPosition mTargetPosition,int mKickType,
        VecPosition mInitialBallPosition){
    isKickingAction= mIsKickingAction;
    targetPosition = mTargetPosition;
    kickType = mKickType;
    initialBallPosition = mInitialBallPosition;

}

bool Action::getIsKickingAction(){
    return isKickingAction;
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
