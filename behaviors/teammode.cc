#include "teammode.h"
#include "fieldRange.h"

TeamMode::TeamMode(){

}

void TeamMode::setCurrentTime(long mtime){
    currentTime = mtime;
}

void TeamMode::setGoals(int mOurgoals,int mOpponentgoals){
    ourgoals = mOurgoals;
    opponentgoals = mOpponentgoals;
}

Mode TeamMode::getMode(){
    //Defence,Normal,Attack,SetPiece
    //calculate mode
    return Attack
}

ActionAllocator TeamMode::getActionallocator(){
    //call ActionAllocator Method to return something

}

FieldRange TeamMode::getFieldRange(int playerPositionNumber){

    //Defence,Normal,Attack,SetPiece

    if(getMode() == Attack) {
        return getAttackFieldRange(playerPositionNumber);
    }
    return getFormationFieldRange(playerPositionNumber);
}

FieldRange getFormationFieldRange(int playerPositionNumber) {

    //cout << "playerPositionNumber" << playerPositionNumber << "\n";
    int X = HALF_FIELD_X;
    int Y = HALF_FIELD_Y;

    int bottomLeftX;
    int bottomLeftY;

    int topRightX;
    int topRightY;

    int centerX;
    int centerY;

    if (playerPositionNumber == 1) {
        centerX = -7 * X / 8;
        centerY = 0;
    } else if (playerPositionNumber == 2) {
        centerX = -5 * X / 8;
        centerY = 3 * Y / 4;

    } else if (playerPositionNumber == 3) {
        centerX = -5 * X / 8;
        centerY = Y / 4;

    } else if (playerPositionNumber == 4) {
        centerX = -5 * X / 8;
        centerY = -Y / 4;

    } else if (playerPositionNumber == 5) {
        centerX = -5 * X / 8;
        centerY = -3 * Y / 4;

    } else if (playerPositionNumber == 6) {
        centerX = -3 * X / 8;
        centerY = 3 * Y / 4;

    } else if (playerPositionNumber == 7) {
        centerX = -3 * X / 8;
        centerY = Y / 4;

    } else if (playerPositionNumber == 8) {
        centerX = -3 * X / 8;
        centerY = -Y / 4;

    } else if (playerPositionNumber == 9) {
        centerX = -3 * X / 8;
        centerY = -3 * Y / 4;

    } else if (playerPositionNumber == 10) {
        centerX = -X / 8;
        centerY = Y / 4;

    } else if (playerPositionNumber == 11) {
        centerX = -X / 8;
        centerY = -Y / 4;

    }


    bottomLeftX = centerX - 1;
    bottomLeftY = centerY + 1;

    topRightX = centerX + 1;
    topRightY = centerY - 1;

    VecPosition bottomLeft(bottomLeftX, bottomLeftY, 0);
    VecPosition topRight(topRightX, topRightY, 0);

    FieldRange range(bottomLeft, topRight);
    return range;

}

FieldRange getAttackFieldRange(int playerPositionNumber) {

    //cout << "playerPositionNumber" << playerPositionNumber << "\n";
    int X = HALF_FIELD_X;
    int Y = HALF_FIELD_Y;

    int bottomLeftX;
    int bottomLeftY;

    int topRightX;
    int topRightY;

    int centerX;
    int centerY;

    if (playerPositionNumber == 1) {
        centerX = -7 * X / 8;
        centerY = 0;
    } else if (playerPositionNumber == 2) {
        centerX = -5 * X / 8;
        centerY = 3 * Y / 4;

    } else if (playerPositionNumber == 3) {
        centerX = -5 * X / 8;
        centerY = Y / 4;

    } else if (playerPositionNumber == 4) {
        centerX = -5 * X / 8;
        centerY = -Y / 4;

    } else if (playerPositionNumber == 5) {
        centerX = -5 * X / 8;
        centerY = -3 * Y / 4;

    } else if (playerPositionNumber == 11) {
        centerX = 3 * X / 8;
        centerY = 3 * Y / 4;

    } else if (playerPositionNumber == 10) {
        centerX = 3 * X / 8;
        centerY = Y / 4;

    } else if (playerPositionNumber == 9) {
        centerX = 3 * X / 8;
        centerY = -Y / 4;

    } else if (playerPositionNumber == 8) {
        centerX = 3 * X / 8;
        centerY = -3 * Y / 4;

    } else if (playerPositionNumber == 7) {
        centerX = X / 8;
        centerY = Y / 4;

    } else if (playerPositionNumber == 6) {
        centerX = X / 8;
        centerY = Y / 4;

    }


    bottomLeftX = centerX - 1;
    bottomLeftY = centerY + 1;

    topRightX = centerX + 1;
    topRightY = centerY - 1;

    VecPosition bottomLeft(bottomLeftX, bottomLeftY, 0);
    VecPosition topRight(topRightX, topRightY, 0);

    FieldRange range(bottomLeft, topRight);
    return range;

}