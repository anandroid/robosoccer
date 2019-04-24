//
// Created by zoro on 4/23/19.
//

#ifndef UTAUSTINVILLABASE_PLAYER_H
#define UTAUSTINVILLABASE_PLAYER_H


#include "../math/vecposition.h"


class Range{
    VecPosition bottomLeft;
    VecPosition topRIght;
};


class player {
  int playerNumber;
  int playerPositionNumber;
  VecPosition position;
  int agressiveRating;
  Action actionInvolved;
  Range range;

};



#endif //UTAUSTINVILLABASE_PLAYER_H
