//
// Created by zoro on 4/23/19.
//

#include "player.h"
#include "../headers/headers.h"
#include "../math/vecposition.h"
#include "fieldRange.h"

Player::Player(int mPlayerNumber, int mPlayerPositionNumber, int mAggressiveRating,
               FieldRange mRange) : range(mRange) {
    playerNumber = mPlayerNumber;
    playerPositionNumber = mPlayerPositionNumber;
    agressiveRating = mAggressiveRating;
    range = mRange;
}

void Player::setAggressiveRating(int mAggressiveRating){
    agressiveRating = mAggressiveRating;
}

void Player::setActionInvolved(SkillType mAction){
    actionInvolved = mAction;
}

void Player::setRange(FieldRange mRange){
    range = mRange;
}

void Player::setPlayerPosition(VecPosition mPosition){
    position = mPosition;
}

int Player::getPlayerNumber(){
    return playerNumber;
}

int Player::getPlayerPositionNumber(){
    return playerPositionNumber;
}

VecPosition Player::getPosition(){
    return position;
}

int Player::getAggressiveRating(){
    return agressiveRating;
}

SkillType Player::getActionInvolved(){
    return actionInvolved;
}

FieldRange Player::getRange(){
    return range;
}

//FieldRange::FieldRange() {}
