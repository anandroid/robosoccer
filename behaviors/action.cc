//
// Created by Anand kumar on 2019-04-28.
//

Action::Action(){

}

void Action::Init(bool mIsKickingAction,VecPosition mTargetPosition,int mKickType){
    isKickingAction= mIsKickingAction;
    targetPosition = mTargetPosition;
    kickType = mKickType;
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