#include "naobehavior.h"
#include "../rvdraw/rvdraw.h"
#include "fieldRange.h"
#include "player.h"
#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <vector>
#include <sstream>
#include <queue>

using namespace std;

extern int agentBodyType;
WorldModel *gworldModel;
std::string row_array[10] = {"R1", "R2", "R3", "R4", "R5", "R6", "R7", "R8", "R9", "R10"};
std::string column_array[10] = {"C1", "C2", "C3", "C4", "C5", "C6", "C7", "C8", "C9", "C10"};

bool enrolled_in_task[11] = {false, false, false, false, false, false, false, false, false, false};

VecPosition target_positions_for_player[11];

bool threadExecutionForPlayer[11] = {false, false, false, false, false, false, false, false, false, false, false};

//int opp_prev_row = -1;
//int opp_prev_column = -1;

string opponent_position_array[100];
int opponent_position_index = 0;

string opponent_action_array[100];
int opponent_action_index = 0;

string opponent_random_goal = "";

std::vector<Player> players;



class PlayerTask {

public:
    int playerNum;
    VecPosition targetPosition;

    PlayerTask(int mplayerNum, VecPosition mtargetPosition) {
        playerNum = mplayerNum;
        targetPosition = mtargetPosition;
    }
};

queue <PlayerTask> tasksQueue;

int getRandomInRange(int range) {
    srand(time(NULL));
    int random = rand();
    return random % range;
}


/*
 * Real game beaming.
 * Filling params x y angle
 */
void NaoBehavior::beam(double &beamX, double &beamY, double &beamAngle) {

    if (worldModel->getUNum() == 1) {
        beamX = -HALF_FIELD_X / getRandomInRange(15);
        beamY = 0;
        beamAngle = 0;
    } else {
        beamX = -HALF_FIELD_X / 4;
        beamY = 0;
        beamAngle = 0;
    }


    int randomValue = getRandomInRange(4);
    if (randomValue < 1) {
        opponent_random_goal = "R1 ";
    } else if (randomValue < 2) {
        opponent_random_goal = "R4 ";
    } else if (randomValue < 3) {
        opponent_random_goal = "R6 ";
    } else if (randomValue < 4) {
        opponent_random_goal = "R8 ";
    }

    randomValue = getRandomInRange(2);
    if (randomValue < 1) {
        opponent_random_goal = opponent_random_goal + "C1";
    } else {
        opponent_random_goal = opponent_random_goal + "C2";
    }


}

std::string exec(const char *cmd) {
    char buffer[128];
    std::string result = "";
    FILE *pipe = popen(cmd, "r");
    if (!pipe) throw std::runtime_error("popen() failed!");
    try {
        while (fgets(buffer, sizeof buffer, pipe) != NULL) {
            result += buffer;
        }
    } catch (...) {
        pclose(pipe);
        throw;
    }
    pclose(pipe);
    return result;
}

vector <string> split(string str, char delimiter) {
    vector <string> internal;
    stringstream ss(str); // Turn the string into a stream.
    string tok;

    while (getline(ss, tok, delimiter)) {
        internal.push_back(tok);
    }

    return internal;
}

string calculateActionInString(int previous_row, int previous_column, int next_row, int next_column) {

    //up
    if (previous_row > next_row) {
        return "2";
    }
    //down
    if (previous_row < next_row) {
        return "0";
    }
    //left
    if (previous_column > next_column) {
        return "3";
    }
    //right
    if (previous_column < next_column) {
        return "2";
    }

    return "-1";


}

namespace patch {
    template<typename T>
    std::string to_string(const T &n) {
        std::ostringstream stm;
        stm << n;
        return stm.str();
    }
}

string intToChar(int value) {
    return patch::to_string(value);
}

int getRandom() {
    srand(time(NULL));
    int random = rand();
    return random % 10;
}

string getStateFromLocation(string loc) {

    int row = 0;
    int column = 0;

    row = ((int) loc.at(1)) - 48;
    column = ((int) loc.at(4)) - 48;


    int result = (column * 10) + row;

    return intToChar(result);

}

string getStringFromOpponentActionsAndStates() {

    string result = "{";
    for (int i = 0; i < opponent_position_index; i++) {

        if (i < opponent_action_index) {
            result = result + "('S'," + getStateFromLocation(opponent_position_array[i]) + "):" + intToChar(i) +
                     ",";
            result = result + "('A'," + opponent_action_array[i] + "):" + intToChar(i);
        } else {
            result = result + "('S'," + getStateFromLocation(opponent_position_array[i]) + "):" + intToChar(i);
        }

        if (i < opponent_position_index - 1) {
            result = result + ",";
        }

    }

    result = result + "}";
    return result;
}


void *asuBackgroundFunction(void *playerNumberargs) {

    int playerNumber = *((int *) playerNumberargs);
    free(playerNumberargs);

    cout << "player " << playerNumber << "\n";

    threadExecutionForPlayer[playerNumber] = true;

    /*** ----------------------------------------  ***/




    float horizontal_tile_size = FIELD_X / 10;
    int horizontal_tile = 5 + ((gworldModel->getMyPosition().getX()) / horizontal_tile_size);

    float vertical_tile_size = FIELD_Y / 10;
    int vertical_tile = 5 + ((gworldModel->getMyPosition().getY()) / vertical_tile_size);


    VecPosition opponentVecPosition = gworldModel->getTeammate(1);

    int opp_horizontal_tile = 5 + ((opponentVecPosition.getX()) / horizontal_tile_size);
    int opp_vertical_tile = 5 + ((opponentVecPosition.getY()) / vertical_tile_size);

    string opp_loc = row_array[opp_vertical_tile] + " " + column_array[opp_horizontal_tile];

    opponent_position_array[opponent_position_index++] = opp_loc;

    if (opponent_position_index > 1) {

        string prev_pos = opponent_position_array[opponent_position_index - 2];
        string curr_pos = opponent_position_array[opponent_position_index - 1];

        cout << "--prev pos --" << prev_pos;
        cout << "--curr pos --" << curr_pos << "\n";

        string prev_action =
                calculateActionInString((int) prev_pos.at(1), (int) prev_pos.at(4), (int) curr_pos.at(1),
                                        (int) curr_pos.at(4));

        if (prev_action != "-1") {
            opponent_action_array[opponent_action_index++] = prev_action;
        }

    }


    string stateAndActionConstructor = getStringFromOpponentActionsAndStates();

    string command = "python dbnq3.py \"" + stateAndActionConstructor + "\" \"" + row_array[vertical_tile] + " " +
                     column_array[horizontal_tile] + "\" ";

    cout << command << "\n";



    //string rawresult="##something##";
    string rawresult = exec(command.c_str());
    unsigned first = rawresult.find("##");
    unsigned last = rawresult.find_last_of("##");
    string result = "";

    //it reached the state
    if (result.find("step") != string::npos) {
        //check threading issue
        enrolled_in_task[playerNumber] = false;

    } else {

        if (first >= 0 && last > first) {
            result = rawresult.substr(first, last - first);
        }

        cout << result << "For 2" << "\n";


        vector <string> sep = split(result, ' ');

        int row = 0;
        int column = 0;

        if (sep.size() > 3) {
            row = ((int) sep[2].at(1)) - 48;
            column = ((int) sep[3].at(1)) - 48;
        }

        cout << "------" << "\n";
        cout << "Player 2 row " << row << " column " << column << "\n";
        cout << "------" << "\n";


        float vertical_y = vertical_tile_size * (row - 5);
        float horizontal_x = horizontal_tile_size * (column - 5);

        VecPosition shouldMoveTo = VecPosition(horizontal_x, vertical_y, 0);


        PlayerTask playerTask = PlayerTask(playerNumber, shouldMoveTo);
        tasksQueue.push(playerTask);

    }

    /*** ----------------------------------------  ***/

    threadExecutionForPlayer[playerNumber] = false;

    pthread_exit(NULL);
}

FieldRange getRangeForPlayerPositionNumber(int playerPositionNumber){

    if(playerPositionNumber==0){
         VecPosition bottomLeft(-HALF_FIELD_X/2-1,1,0);
         VecPosition topRight(-HALF_FIELD_X/2,0,0);

         FieldRange range(bottomLeft,topRight);
         return range;
    } else {
        VecPosition bottomLeft(HALF_FIELD_X/2-1,1,0);
        VecPosition topRight(HALF_FIELD_X/2,0,0);

        FieldRange range(bottomLeft,topRight);
        return range;

    }

}

Player getPlayerObject(WorldModel *worldModel){

    for (int i=0;i<players.size();i++){
           if(worldModel->getUNum()==players[i].getPlayerNumber()){

               return players[i];
           }
    }

    FieldRange range = getRangeForPlayerPositionNumber(players.size()-1);

    Player player(worldModel->getUNum(),players.size(),players.size(),range);
    players.push_back(player);

    return player;

}


SkillType NaoBehavior::selectSkill() {

    Player player = getPlayerObject(worldModel);

    goToTarget(player.getRange().getCenterOfRange());

    /*gworldModel = worldModel;

    pthread_t thread;

    if (threadExecutionForPlayer[worldModel->getUNum()] == false && enrolled_in_task[worldModel->getUNum()] == false) {

        int *arg = (int *) malloc(sizeof(*arg));

        *arg = worldModel->getUNum();


        int rc;

        rc = pthread_create(&thread, NULL, asuBackgroundFunction, (void *) arg);

        if (rc) {
            cout << "Error:unable to create thread," << rc << endl;
            exit(-1);
        }
    }



    if (tasksQueue.size() > 0 || enrolled_in_task[worldModel->getUNum()]) {

        PlayerTask playerTask = tasksQueue.front();

        //for new tasks
        if (playerTask.playerNum == worldModel->getUNum()) {
            tasksQueue.pop();
            enrolled_in_task[worldModel->getUNum()] = true;
            target_positions_for_player[worldModel->getUNum()] = playerTask.targetPosition;
        }

        return reachPosition(target_positions_for_player[worldModel->getUNum()]);

    }
    return SKILL_STAND;*/
}

SkillType NaoBehavior::reachPosition(VecPosition target) {

    if (me.getDistanceTo(target) < .10) {
        // Close enough to desired position and orientation so just stand
        enrolled_in_task[worldModel->getUNum()] = false;
        return SKILL_STAND;
    } else {
        // Move toward target location
        return goToTarget(target);
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


