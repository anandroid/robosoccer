//
// Created by Anand kumar on 2019-04-24.
//

#ifndef UTAUSTINVILLABASE_ACTIONALLOCATOR_H
#define UTAUSTINVILLABASE_ACTIONALLOCATOR_H

#include "actionallocator.h"
#include "teammode.h"

class ActionAllocator{

private:
    TeamMode teamMode;
    Player player;
public:
    ActionAllocator();
    void setTeamMode(TeamMode teamMode);
    SkillType getAction(Player player);
};



#endif //UTAUSTINVILLABASE_ACTIONALLOCATOR_H
