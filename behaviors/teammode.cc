#include <teammode.h>


void TeamMode::setCurrentTime(long mtime){
    currentTime = mtime;
}

void TeamMode::setGoals(int mOurgoals,int mOpponentgoals){
    ourgoals = mOurgoals;
    opponentgoals = mOpponentgoals;
}

Mode TeamMode::getMode(){
    //calculate mode
}

ActionAllocator TeamMode::getActionallocator(){
    //call ActionAllocator Method to return something

}