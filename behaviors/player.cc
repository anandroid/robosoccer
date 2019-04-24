//
// Created by zoro on 4/23/19.
//

#include "player.h"

Player::Player(int mPlayerNumber,int mPlayerPositionNumber,int mAggressiveRating){
    playerNumber = mPlayerNumber;
    playerPositionNumber = mPlayerPositionNumber;
    aggressiveRating = mAggressiveRating;
}

void Player::setAggressiveRating(int mAggressiveRating){
    aggressiveRating = mAggressiveRating;
}

void Player::setActionInvolved(SkillType mAction){
    action = mAction;
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
    return aggressiveRating;
}

FieldRange Player::getRange(){
    return range;
}