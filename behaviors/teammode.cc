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
    return Attack;
}

VecPosition TeamMode::getNextTargetPosition(int aggressiveRating,VecPosition currentPosition){

    double newX = currentPosition.getX();
    double newY = currentPosition.getY();

        int MAX_RANGE_X;
        int MIN_RANGE_X;

        bool isAttackPlayer = true;

        if(aggressiveRating==1){
            isAttackPlayer = false;
            MIN_RANGE_X = -7 * HALF_FIELD_X/8;
        }

        if(aggressiveRating==2 || aggressiveRating==3){
            isAttackPlayer = false;
            MIN_RANGE_X = -5 * HALF_FIELD_X / 8;
        }

        if(aggressiveRating==4 || aggressiveRating==5){
            isAttackPlayer = false;
            MIN_RANGE_X =  -3 * HALF_FIELD_X / 8;
        }

        if(aggressiveRating==5){
            isAttackPlayer = true;
            MAX_RANGE_X =  HALF_FIELD_X/8;
        }

        if(aggressiveRating==6 || aggressiveRating==7){
            isAttackPlayer = true;
            MAX_RANGE_X =  2*HALF_FIELD_X/8;
        }

        if(aggressiveRating==8 || aggressiveRating==9){
            isAttackPlayer = true;
            MAX_RANGE_X =  4 * HALF_FIELD_X/8;
        }

        if(aggressiveRating==10 || aggressiveRating==11){
            isAttackPlayer = true;
            MAX_RANGE_X =  6 * HALF_FIELD_X/8;
        }

        if(isAttackPlayer) {

            if (newX + 1 < MAX_RANGE_X) {
                newX++;
            } else {
                newX = newX - 1;
            }

            if (newY > HALF_FIELD_Y / 2) {
                if (newX > HALF_FIELD_X / 2) {
                    newY = newY - 1;
                } else {
                    newY = newY - 0.5;
                }
            } else if (newY < -HALF_FIELD_Y / 2) {
                if (newX > HALF_FIELD_X / 2) {
                    newY = newY + 1;
                } else {
                    newY = newY + 0.5;
                }
            }

        }

        if(!isAttackPlayer){

            if (newX + 1 > MIN_RANGE_X) {
                newX--;
            } else {
                newX = newX + 1;
            }

            if (newY > HALF_FIELD_Y / 2) {
                if (newX > HALF_FIELD_X / 2) {
                    newY = newY - 1;
                } else {
                    newY = newY - 0.5;
                }
            } else if (newY < -HALF_FIELD_Y / 2) {
                if (newX > HALF_FIELD_X / 2) {
                    newY = newY + 1;
                } else {
                    newY = newY + 0.5;
                }
            }
        }

        if(aggressiveRating==1){
            cout<<"Player 1 New Target "<<newX<<":"<<newY<<"\n";
        }else{
            cout<<aggressiveRating;
        }



        return VecPosition(newX,newY,currentPosition.getZ());

}


FieldRange TeamMode::getFieldRange(int playerPositionNumber){

    //Defence,Normal,Attack,SetPiece

    if(getMode() == Attack) {
        return getAttackFieldRange(playerPositionNumber);
    }
    return getFormationFieldRange(playerPositionNumber);
}


FieldRange TeamMode::getFormationFieldRange(int playerPositionNumber) {

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

FieldRange TeamMode::getAttackFieldRange(int playerPositionNumber) {

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
        centerY =  Y / 4;

    } else if (playerPositionNumber == 3) {
        centerX = -5 * X / 8;
        centerY = -Y / 4;

    } else if (playerPositionNumber == 4) {
        centerX = -3 * X / 8;
        centerY = 3*Y / 4;

    } else if (playerPositionNumber == 5) {
        centerX = -3 * X / 8;
        centerY = -3 * Y / 4;

    }  else if (playerPositionNumber == 6) {
        centerX = X / 8;
        centerY = Y / 4;

    } else if (playerPositionNumber == 7) {
        centerX = X / 8;
        centerY = -Y / 4;

    } else if (playerPositionNumber == 8) {
        centerX = 3 * X / 8;
        centerY = 3 * Y / 4;

    } else if (playerPositionNumber == 9) {
        centerX = 3 * X / 8;
        centerY = -3 * Y / 4;

    } else if (playerPositionNumber == 10) {
        centerX = 5 * X / 8;
        centerY = Y / 4;

    } else if (playerPositionNumber == 11) {
        centerX = 5 * X / 8;
        centerY = - Y / 4;

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