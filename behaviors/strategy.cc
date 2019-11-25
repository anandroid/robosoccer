#include "naobehavior.h"
#include "../rvdraw/rvdraw.h"
#include "action.h"
#include <cmath>

extern int agentBodyType;

bool firstPass = false;

/*
 * Real game beaming.
 * Filling params x y angle
 */
void NaoBehavior::beam(double &beamX, double &beamY, double &beamAngle) {

    int playerNum = worldModel->getUNum();
    if (playerNum % 2 == 0) {
        beamY = -HALF_FIELD_Y / 4;
    } else {
        beamY = HALF_FIELD_Y / 4;
    }
    beamX = -(1 / 4) * HALF_FIELD_X;

    beamAngle = 0;
}

bool isNearToGoal(WorldModel *worldModel) {

    double GOAL_RANGE = (3 * HALF_FIELD_X) / 8;
    VecPosition myPosition = worldModel->getMyPosition();
    double distanceToGoal = worldModel->distanceToOppGoal(myPosition);

    if (distanceToGoal < GOAL_RANGE) {
        return true;
    }

    return false;

}

SkillType NaoBehavior::goalingAction() {

    // Choose any random point in in 2d window

    //choosing random Y which is in the goal post range
    double randomY = worldModel->getOppRightGoalPost().getY() +
                     random(GOAL_Y);

    //choosing random X which is in the goal post range
    double randomX = HALF_FIELD_X - random(GOAL_X);

    VecPosition targetBallPosition = VecPosition(randomX, randomY, me.getZ());

    cout << "Goaling Action" << "\n";

    //Go 3/4th of the distance dribbling it and once you are near kick the ball within the goal posts
    if (worldModel->distanceToOppGoal(me) > HALF_FIELD_X / 4) {
        cout << "Goaling Action Dribble " << targetBallPosition << "\n";
        return kickBall(KICK_DRIBBLE, targetBallPosition);
    } else {
        cout << "Goaling Action Kick " << targetBallPosition << "\n";
        return kickBall(KICK_IK, targetBallPosition);
    }

}

SkillType NaoBehavior::dribbleTowardsGoal(VecPosition currentBallPosition) {

    int X_DISTANCE_FACTOR = 2;
    int Y_DISTANCE_FACTOR = 1.5;

    double newX;
    double newY;

    if (currentBallPosition.getY() > HALF_FIELD_Y / 2) {
        newY = currentBallPosition.getY() - Y_DISTANCE_FACTOR / 2;
    } else if (currentBallPosition.getY() < -HALF_FIELD_Y / 2) {
        newY = currentBallPosition.getY() + Y_DISTANCE_FACTOR / 2;
    } else {
        newY = currentBallPosition.getY();
    }

    newX = currentBallPosition.getX() + X_DISTANCE_FACTOR;

    cout << "Dribbling " << newX << " " << newY << "\n";

    return kickBall(KICK_DRIBBLE, VecPosition(newX, newY, currentBallPosition.getZ()));


}

VecPosition getTeamMatePosition(WorldModel *worldModel) {
    VecPosition temp;
    for (int i = WO_TEAMMATE1; i < WO_TEAMMATE1 + NUM_AGENTS; ++i) {
        int playerNum = i - WO_TEAMMATE1 + 1;
        if (worldModel->getUNum() == playerNum) {
            // This is us
        } else {
            WorldObject *teammate = worldModel->getWorldObject(i);
            if (teammate->validPosition) {
                temp = teammate->pos;
            } else {
                continue;
            }
        }
    }

    return temp;
}

int getPlayerClosestToTheBall(WorldModel *worldModel) {
    int playerClosestToBall = -1;
    double closestDistanceToBall = 10000;
    for (int i = WO_TEAMMATE1; i < WO_TEAMMATE1 + NUM_AGENTS; ++i) {
        VecPosition temp;
        int playerNum = i - WO_TEAMMATE1 + 1;
        if (worldModel->getUNum() == playerNum) {
            // This is us
            temp = worldModel->getMyPosition();
        } else {
            WorldObject *teammate = worldModel->getWorldObject(i);
            if (teammate->validPosition) {
                temp = teammate->pos;
            } else {
                continue;
            }
        }
        temp.setZ(0);
        double distanceToBall = temp.getDistanceTo(worldModel->getBall());
        if (distanceToBall < closestDistanceToBall) {
            playerClosestToBall = playerNum;
            closestDistanceToBall = distanceToBall;
        }
    }

    return playerClosestToBall;
}

bool isPlayerClosestToTheBall(int currentPlayer, WorldModel *worldModel) {
    int playerClosestToTheBall = getPlayerClosestToTheBall(worldModel);

    if (currentPlayer == playerClosestToTheBall) {
        return true;
    }

    return false;
}


VecPosition getPassablePosition(VecPosition currentPosition,
                                VecPosition opponentPosition, VecPosition teamMatePosition) {

    double xDiff = abs(opponentPosition.getX() - teamMatePosition.getX());

    double yDiff = abs(opponentPosition.getY() - teamMatePosition.getY());

    int X_DISTANCE_FACTOR = 4;
    int Y_DISTANCE_FACTOR = 2;


    double newX;
    double newY;

    newX = teamMatePosition.getX() + X_DISTANCE_FACTOR;

    if (newX > HALF_FIELD_X) {
        newX = newX - X_DISTANCE_FACTOR / 2;
    }

    if (teamMatePosition.getY() > HALF_FIELD_Y / 2) {
        newY = teamMatePosition.getY() - Y_DISTANCE_FACTOR;
    } else if (teamMatePosition.getY() > 0) {
        newY = teamMatePosition.getY() + Y_DISTANCE_FACTOR / 2;
    } else if (teamMatePosition.getY() < -HALF_FIELD_Y / 2) {
        newY = teamMatePosition.getY() + Y_DISTANCE_FACTOR;
    } else if (teamMatePosition.getY() < 0) {
        newY = teamMatePosition.getY() - Y_DISTANCE_FACTOR / 2;
    } else {
        newY = teamMatePosition.getY();
    }

    //toDo check opponents position


    return VecPosition(newX, newY, currentPosition.getZ());

}

bool isOurSideKick(WorldModel *worldModel) {
    int mode = worldModel->getPlayMode();
    int side = worldModel->getSide();
    if (side == SIDE_LEFT) {
        if (mode == 1 || mode == 4 || mode == 6) {
            return true;
        } else {
            return false;
        }
    }
    if (side == SIDE_RIGHT) {
        if (mode == 2 || mode == 5 || mode == 7) {
            return true;
        } else {
            return false;
        }
    }
}


std::vector<int> getPlayersAheadWithInRange(WorldModel *worldModel) {

    std::vector<int> playersInRange;

    int RANGE = 5;

    double closestDistanceToPlayer = 10000;

    VecPosition myPos;

    for (int i = WO_TEAMMATE1; i < WO_TEAMMATE1 + NUM_AGENTS; ++i) {
        VecPosition temp;
        int playerNum = i - WO_TEAMMATE1 + 1;
        if (worldModel->getUNum() == playerNum) {
            // This is us
            temp = worldModel->getMyPosition();
            myPos = temp;
            temp.setZ(0);
            continue;
        } else {
            WorldObject *teammate = worldModel->getWorldObject(i);
            if (teammate->validPosition) {
                temp = teammate->pos;
            } else {
                continue;
            }
        }
        temp.setZ(0);

        double distanceToPlayer = temp.getDistanceTo(myPos);

        if (isOurSideKick(worldModel)) {
            playersInRange.push_back(10);
            firstPass = false;
        }

        if (distanceToPlayer < RANGE) {
            if (temp.getX() > myPos.getX()) {
                cout << "My pos " << myPos.getX() << " Passed pos " << temp.getX() << "\n";
                playersInRange.push_back(playerNum);
            }
        }
    }

    return playersInRange;
}

int getPlayerNearWithBetterAggressionInTheRange(WorldModel *worldModel, int currentPlayerNumber) {
    int playerWithHigherAggressiveRating = -1;
    int higherAggressiveRating = -1;

    std::vector<int> playersWitInRange = getPlayersAheadWithInRange(worldModel);
    //cout<<"--------------------------"<<"\n";
    //cout<<"players with in range "<<playersWitInRange.size()<<"\n";
    for (int i = 0; i < playersWitInRange.size(); i++) {
        //cout<<"player - "<<playersWitInRange[i];
        int agressiveRating = playersWitInRange[i];
        if (higherAggressiveRating < agressiveRating) {
            higherAggressiveRating = agressiveRating;
            playerWithHigherAggressiveRating = playersWitInRange[i];
        }
    }

    //cout<<"players with high aggressive rating "<<playerWithHigherAggressiveRating<<"\n";

    return playerWithHigherAggressiveRating;
}

VecPosition getOpponentPosition(WorldModel *worldModel) {
    WorldObject *opponent = worldModel->getWorldObject(WO_OPPONENT1);
    return opponent->pos;

}

std::vector<int> readOpponentPositionsAndReturnWithInRange(WorldModel *worldModel) {

    std::vector<int> playersInRange;

    std::string positions = "";
    time_t now = time(0);
    char *dt = ctime(&now);
    int playerNum = 0;

    for (int i = WO_OPPONENT1; i < WO_OPPONENT1 + NUM_AGENTS; ++i) {
        VecPosition temp;


        WorldObject *opponent = worldModel->getWorldObject(i);
        if (opponent->validPosition) {
            temp = opponent->pos;
            playerNum = i;
        } else {
            continue;
        }

        temp.setZ(0);
        std::string out_string;
        std::stringstream ss;

        ss << dt << "\t";
        ss << playerNum << "\t";
        ss << temp.getX() << "\t";
        ss << temp.getY() << "\n";
        out_string = ss.str();
        positions.append(out_string);


        if (temp.getX() < 0) {
            playersInRange.push_back(i);
        }
    }

    return playersInRange;
}


SkillType NaoBehavior::getHighestPossibleAction() {


    if (isPlayerClosestToTheBall(worldModel->getUNum(), worldModel)) {

        if (isOurSideKick(worldModel)) {
            VecPosition teamMatePosition = getTeamMatePosition(worldModel);
            return kickBall(KICK_FORWARD, teamMatePosition);
        }

        if (isNearToGoal(worldModel)) {
            return goalingAction();
        }

        // to pass or to dribble
        int teamMateNum =
                getPlayerNearWithBetterAggressionInTheRange(worldModel, worldModel->getUNum());

        if (teamMateNum == -1) {
            //no player nearby, so dribble

            //dribble against the opponent

            return dribbleTowardsGoal(worldModel->getBall());

        } else {
            VecPosition teamMatePosition = getTeamMatePosition(worldModel);

            if(teamMatePosition.getX()<worldModel->getMyPosition().getX()){
                return dribbleTowardsGoal(worldModel->getBall());
            }

            return kickBall(KICK_FORWARD, teamMatePosition);


            // return dribbleTowardsGoal(worldModel->getBall());
        }


    } else {

        if (isOurSideKick(worldModel)) {
            return SKILL_STAND;
        }

        int REACHABLEDISTANCE = 3;

        VecPosition opponentPosition = getOpponentPosition(worldModel);
        VecPosition teamMatePosition = getTeamMatePosition(worldModel);
        VecPosition myPosition = worldModel->getMyPosition();

        VecPosition targetPosition = getPassablePosition(myPosition, opponentPosition, teamMatePosition);

        return goToTarget(targetPosition);

    }


}

SkillType NaoBehavior::getControlFunction() {
    return getHighestPossibleAction();
}


SkillType NaoBehavior::selectSkill() {

    SkillType controlFunction = getControlFunction();
    return controlFunction;
    // My position and angle
    //cout << worldModel->getUNum() << ": " << worldModel->getMyPosition() << ",\t" << worldModel->getMyAngDeg() << "\n";

    // Position of the ball
    //cout << worldModel->getBall() << "\n";

    // Example usage of the roboviz drawing system and RVSender in rvdraw.cc.
    // Agents draw the position of where they think the ball is
    // Also see example in naobahevior.cc for drawing agent position and
    // orientation.
    /*
    worldModel->getRVSender()->clear(); // erases drawings from previous cycle
    worldModel->getRVSender()->drawPoint("ball", ball.getX(), ball.getY(), 10.0f, RVSender::MAGENTA);
    */

    // ### Demo Behaviors ###

    // Walk in different directions
    //return goToTargetRelative(VecPosition(1,0,0), 0); // Forward
    //return goToTargetRelative(VecPosition(-1,0,0), 0); // Backward
    //return goToTargetRelative(VecPosition(0,1,0), 0); // Left
    //return goToTargetRelative(VecPosition(0,-1,0), 0); // Right
    //return goToTargetRelative(VecPosition(1,1,0), 0); // Diagonal
    //return goToTargetRelative(VecPosition(0,1,0), 90); // Turn counter-clockwise
    //return goToTargetRelative(VecPdosition(0,-1,0), -90); // Turn clockwise
    //return goToTargetRelative(VecPosition(1,0,0), 15); // Circle

    // Walk to the ball
    //return goToTarget(ball);

    // Turn in place to face ball
    /*double distance, angle;
    getTargetDistanceAndAngle(ball, distance, angle);
    if (abs(angle) > 10) {
      return goToTargetRelative(VecPosition(), angle);
    } else {
      return SKILL_STAND;
    }*/

    // Walk to ball while always facing forward
    //return goToTargetRelative(worldModel->g2l(ball), -worldModel->getMyAngDeg());

    // Dribble ball toward opponent's goal
    //return kickBall(KICK_DRIBBLE, VecPosition(HALF_FIELD_X, 0, 0));

    // Kick ball toward opponent's goal
    //return kickBall(KICK_FORWARD, VecPosition(HALF_FIELD_X, 0, 0)); // Basic kick
    //return kickBall(KICK_IK, VecPosition(HALF_FIELD_X, 0, 0)); // IK kick

    // Just stand in place
    //return SKILL_STAND;

    // Demo behavior where players form a rotating circle and kick the ball
    // back and forth
    return demoKickingCircle();
}


SkillType NaoBehavior::kickAccordingToDistance(const VecPosition &target) {

    VecPosition myPos = worldModel->getMyPosition();
    double distance = myPos.getDistanceTo(target);
    VecPosition ballPosition = worldModel->getBall();
    if (distance < 1.5) {
        return kickBall(KICK_DRIBBLE, target);
    } else if (distance < 6) {
        return kickBall(KICK_FORWARD, target);
    } else {
        return kickBall(KICK_IK, target);
    }
}


/*
 * Demo behavior where players form a rotating circle and kick the ball
 * back and forth
 */
SkillType NaoBehavior::demoKickingCircle() {
    // Parameters for circle
    VecPosition center = VecPosition(-HALF_FIELD_X / 2.0, 0, 0);
    double circleRadius = 5.0;
    double rotateRate = 2.5;

    // Find closest player to ball
    int playerClosestToBall = -1;
    double closestDistanceToBall = 10000;
    for (int i = WO_TEAMMATE1; i < WO_TEAMMATE1 + NUM_AGENTS; ++i) {
        VecPosition temp;
        int playerNum = i - WO_TEAMMATE1 + 1;
        if (worldModel->getUNum() == playerNum) {
            // This is us
            temp = worldModel->getMyPosition();
        } else {
            WorldObject *teammate = worldModel->getWorldObject(i);
            if (teammate->validPosition) {
                temp = teammate->pos;
            } else {
                continue;
            }
        }
        temp.setZ(0);

        double distanceToBall = temp.getDistanceTo(ball);
        if (distanceToBall < closestDistanceToBall) {
            playerClosestToBall = playerNum;
            closestDistanceToBall = distanceToBall;
        }
    }

    if (playerClosestToBall == worldModel->getUNum()) {
        // Have closest player kick the ball toward the center
        return kickBall(KICK_FORWARD, center);
    } else {
        // Move to circle position around center and face the center
        VecPosition localCenter = worldModel->g2l(center);
        SIM::AngDeg localCenterAngle = atan2Deg(localCenter.getY(), localCenter.getX());

        // Our desired target position on the circle
        // Compute target based on uniform number, rotate rate, and time
        VecPosition target = center + VecPosition(circleRadius, 0, 0).rotateAboutZ(360.0 / (NUM_AGENTS - 1) *
                                                                                   (worldModel->getUNum() -
                                                                                    (worldModel->getUNum() >
                                                                                     playerClosestToBall ? 1 : 0)) +
                                                                                   worldModel->getTime() * rotateRate);

        // Adjust target to not be too close to teammates or the ball
        target = collisionAvoidance(true /*teammate*/, false/*opponent*/, true/*ball*/, 1/*proximity thresh*/,
                                    .5/*collision thresh*/, target, true/*keepDistance*/);

        if (me.getDistanceTo(target) < .25 && abs(localCenterAngle) <= 10) {
            // Close enough to desired position and orientation so just stand
            return SKILL_STAND;
        } else if (me.getDistanceTo(target) < .5) {
            // Close to desired position so start turning to face center
            return goToTargetRelative(worldModel->g2l(target), localCenterAngle);
        } else {
            // Move toward target location
            return goToTarget(target);
        }
    }
}


