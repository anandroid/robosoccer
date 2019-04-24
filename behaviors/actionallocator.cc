#include <actionallocator.h>

ActionAllocator::ActionAllocator(){
    //is default constructor is not need in C
}

void ActionAllocator :: setTeamMode(TeamMode mTeamMode){
    teamMode = mTeamMode;
}

SkillType ActionAllocator::getAction(Player mPlayer){
    player = mPlayer;
}

