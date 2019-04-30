#include "naobehavior.h"
#include "../rvdraw/rvdraw.h"
#include "fieldRange.h"
#include "player.h"
#include "action.h"
#include "action.cc"
#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <vector>
#include <sstream>
#include <queue>
#include <fstream>
#include "teammode.h"
#include "teammode.cc"
#include "string"
#include "time.h"
#include "ctime"


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

std::vector <Player> players;

Player player;
TeamMode teamMode;

bool firstPass = true;

ofstream writefile;


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

bool didReachTargetPosition(VecPosition currentPosition, VecPosition targetPosition) {

    if (currentPosition.getDistanceTo(targetPosition) < 0.2) {
        return true;
    }

    return false;
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

FieldRange getRangeForPlayerPositionNumber(int playerPositionNumber) {

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
        centerX = -6 * X / 8;
        centerY = 3 * Y / 8;

    } else if (playerPositionNumber == 3) {
        centerX = -6 * X / 8;
        centerY = -3 * Y / 8;

    } else if (playerPositionNumber == 4) {
        centerX = -4 * X / 8;
        centerY = Y / 4;

    } else if (playerPositionNumber == 5) {
        centerX = -4 * X / 8;
        centerY = -Y / 4;

    } else if (playerPositionNumber == 6) {
        centerX = -3 * X / 8;
        centerY = Y / 2;

    } else if (playerPositionNumber == 7) {
        centerX = -3 * X / 8;
        centerY = 0;

    } else if (playerPositionNumber == 8) {
        centerX = -3 * X / 8;
        centerY = -Y / 2;

    } else if (playerPositionNumber == 9) {
        centerX = -2 * X / 8;
        centerY = 3 * Y / 8;

    } else if (playerPositionNumber == 10) {
        centerX = -2 * X / 8;
        centerY = -3 * Y / 8;

    } else if (playerPositionNumber == 11) {
        centerX = - X / 8;
        centerY = 0.5;
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

void initPlayerObject(WorldModel *worldModel) {

    FieldRange range = getRangeForPlayerPositionNumber(worldModel->getUNum());

    player.Init(worldModel->getUNum(), worldModel->getUNum(), worldModel->getUNum(), range);

}

/*
 * Real game beaming.
 * Filling params x y angle
 */
void NaoBehavior::beam(double &beamX, double &beamY, double &beamAngle) {

    FieldRange range = getRangeForPlayerPositionNumber(worldModel->getUNum());

    beamX = range.getCenterOfRange().getX();
    beamY = range.getCenterOfRange().getY();
    if (worldModel->getUNum() == 11) {
        beamAngle = 270;
    }


}

bool isOurSideKick(WorldModel *worldModel){
    int mode = worldModel->getPlayMode();
    int side  = worldModel->getSide();
    if(side==SIDE_LEFT){
        if(mode == 1 || mode==4 || mode==6){
            return true;
        } else{
            return false;
        }
    }

    if(side==SIDE_RIGHT){
        if(mode==2 || mode==5 || mode==7){
            return true;
        } else {
            return false;
        }
    }

}

int getPlayerClosestToOpponent(WorldModel *worldModel, int opponentNumber) {
    int playerClosestToBall = -1;
    double closestDistanceToBall = 10000;

    WorldObject *opponent = worldModel->getWorldObject(opponentNumber);
    if (!opponent->validPosition) {
        return playerClosestToBall;
    }

    VecPosition opponentPosition = opponent->pos;
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

        double distanceToBall = temp.getDistanceTo(opponentPosition);
        if (distanceToBall < closestDistanceToBall) {
            playerClosestToBall = playerNum;
            closestDistanceToBall = distanceToBall;
        }
    }

    return playerClosestToBall;
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

std::vector<int> getPlayersAheadWithInRange(WorldModel *worldModel, Mode mode) {

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
            if (mode == Attack) {
                if (temp.getX() > myPos.getX()) {
                    cout << "My pos " << myPos.getX() << " Passed pos " << temp.getX() << "\n";
                    playersInRange.push_back(playerNum);
                }
            } else {
                playersInRange.push_back(playerNum);
            }

        }
    }

    return playersInRange;
}


void writeToFile(string content, int p_no) {
    std::stringstream ss;
    ss << "positions_" << p_no << ".txt";
    string fname = ss.str();
    //cout << fname;
    //cout << p_no << endl;
    ofstream myfile;
    myfile.open(fname.c_str(), std::ios_base::app);
    myfile << content;
    myfile.close();
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
        //cout << playerNum << endl;
        //cout << temp.getY() << endl;
        //cout << temp.getX() << endl;
        writeToFile(positions, playerNum);

        if (temp.getX() < 0) {
            playersInRange.push_back(i);
        }
    }

    return playersInRange;
}


int getPlayerNearWithBetterAggressionInTheRange(WorldModel *worldModel, int currentPlayerNumber, Mode mode) {
    int playerWithHigherAggressiveRating = -1;
    int higherAggressiveRating = -1;

    std::vector<int> playersWitInRange = getPlayersAheadWithInRange(worldModel, mode);
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


Action NaoBehavior::kickAccordingToDistance(const VecPosition &target) {

    VecPosition myPos = worldModel->getMyPosition();
    double distance = myPos.getDistanceTo(target);
    VecPosition ballPosition = worldModel->getBall();
    Action action;
    if (distance < 3) {
        action.Init(true, target, KICK_FORWARD, ballPosition);
    } else if (distance < 6) {
        action.Init(true, target, KICK_FORWARD, ballPosition);
    } else if (distance > 6) {
        action.Init(true, target, KICK_IK, ballPosition);
    }
    return action;
}

SkillType NaoBehavior::getSkillTypeFromAction(Action action) {

    if (action.getIsKickingAction()) {
        return kickBall(action.getKickType(), action.getTargetPosition());
    }

    return goToTarget(action.getTargetPosition());
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
        return kickBall(KICK_FORWARD, targetBallPosition);
    } else {
        cout << "Goaling Action Kick " << targetBallPosition << "\n";
        return kickBall(KICK_IK, targetBallPosition);
    }

}


SkillType NaoBehavior::playPassingToHigherAggressive(Player *player, int closestPlayerToBall) {

    if (worldModel->getUNum() == closestPlayerToBall) {
        int nearPlayer = getPlayerNearWithBetterAggressionInTheRange(worldModel, worldModel->getUNum(), Normal);
        VecPosition nearPlayerPosition = worldModel->getWorldObject(nearPlayer)->pos;
        if (!player->getIsInvolvedInAction()) {
            player->setIsInvolvedInAction(true);
        }
        cout << "Action  set : " << player->getPlayerNumber() << "\n";
        Action action = kickAccordingToDistance(nearPlayerPosition);
        player->setActionInvolved(action);
        // return action;
        return kickBall(KICK_FORWARD, nearPlayerPosition);
    } else {
        return SKILL_STAND;
    }

}

SkillType NaoBehavior::setTargetPositionAction(Player *player, VecPosition targetPosition) {
    VecPosition currentPosition = worldModel->getMyPosition();
    targetPosition = collisionAvoidance(true /*teammate*/, false/*opponent*/, false/*ball*/, 1/*proximity thresh*/,
                                        .5/*collision thresh*/, targetPosition, true/*keepDistance*/);
    return goToTarget(targetPosition);
}

Action NaoBehavior::playDefensive(Player *player, int closestPlayerToBall) {
    if (worldModel->getUNum() == closestPlayerToBall) {
        int nearPlayerNumber = getPlayerNearWithBetterAggressionInTheRange(worldModel, worldModel->getUNum(), Defence);

        if (nearPlayerNumber != -1) {
            WorldObject *nearPlayer = worldModel->getWorldObject(nearPlayerNumber);
            if (nearPlayer->validPosition) {
                VecPosition nearPlayerPosition = nearPlayer->pos;
                player->setIsInvolvedInAction(true);
                //cout<<"Kick Action  set : "<<player->getPlayerNumber()<<"\n";
                Action action = kickAccordingToDistance(nearPlayerPosition);
                player->setActionInvolved(action);
                return action;
            }
        }

        VecPosition currentPosition = worldModel->getMyPosition();
        VecPosition targetPosition(currentPosition.getX() + 2, currentPosition.getY(), currentPosition.getZ());
        player->setIsInvolvedInAction(true);
        //cout<<"Kick Action  set : "<<player->getPlayerNumber()<<"\n";
        Action action = kickAccordingToDistance(targetPosition);
        player->setActionInvolved(action);
        return action;

    } else {
        VecPosition currentPosition = worldModel->getMyPosition();
        //VecPosition targetPosition = teamMode.getFieldRange(player->getPlayerNumber()).getCenterOfRange();
        VecPosition targetPosition = teamMode.getNextTargetPosition(player->getAggressiveRating(), currentPosition);
        targetPosition = collisionAvoidance(true /*teammate*/, false/*opponent*/, false/*ball*/, 1/*proximity thresh*/,
                                            .5/*collision thresh*/, targetPosition, true/*keepDistance*/);
        VecPosition dummyPosition(0, 0, 0);
        Action action;
        action.Init(false, targetPosition, 0, dummyPosition);
        player->setIsInvolvedInAction(true);
        player->setActionInvolved(action);
        //cout<<"Move Action  set : "<<player->getPlayerNumber()<<"\n";
        return action;
    }
}

Action NaoBehavior::playAggressive(Player *player, int closestPlayerToBall) {
    if (worldModel->getUNum() == closestPlayerToBall) {
        int nearPlayerNumber = getPlayerNearWithBetterAggressionInTheRange(worldModel, worldModel->getUNum(), Attack);

        if (nearPlayerNumber != -1) {
            WorldObject *nearPlayer = worldModel->getWorldObject(nearPlayerNumber);
            if (nearPlayer->validPosition) {
                VecPosition nearPlayerPosition = nearPlayer->pos;
                player->setIsInvolvedInAction(true);
                //cout<<"Kick Action  set : "<<player->getPlayerNumber()<<"\n";
                Action action = kickAccordingToDistance(nearPlayerPosition);
                player->setActionInvolved(action);
                return action;
            }
        }

        VecPosition currentPosition = worldModel->getMyPosition();
        VecPosition targetPosition(currentPosition.getX() + 2, currentPosition.getY(), currentPosition.getZ());
        player->setIsInvolvedInAction(true);
        //cout<<"Kick Action  set : "<<player->getPlayerNumber()<<"\n";
        Action action = kickAccordingToDistance(targetPosition);
        player->setActionInvolved(action);
        return action;

    } else {
        VecPosition currentPosition = worldModel->getMyPosition();
        //VecPosition targetPosition = teamMode.getFieldRange(player->getPlayerNumber()).getCenterOfRange();
        VecPosition targetPosition = teamMode.getNextTargetPosition(player->getAggressiveRating(), currentPosition);
        targetPosition = collisionAvoidance(true /*teammate*/, false/*opponent*/, false/*ball*/, 1/*proximity thresh*/,
                                            .5/*collision thresh*/, targetPosition, true/*keepDistance*/);
        VecPosition dummyPosition(0, 0, 0);
        Action action;
        action.Init(false, targetPosition, 0, dummyPosition);
        player->setIsInvolvedInAction(true);
        player->setActionInvolved(action);
        //cout<<"Move Action  set : "<<player->getPlayerNumber()<<"\n";
        return action;
    }
}



void readFile() {
    string content = "";
    std::string tmp;
    char c = '\0';
    cout << "Reading file\n";
    int length = 0;
    ifstream infile("positions.txt");
    if (infile) {
        length = infile.tellg();//Get file size

        // loop backward over the file

        for (int i = length - 2; i > 0; i--) {
            infile.seekg(i);
            c = infile.get();
            if (c == '\r' || c == '\n')//new line?
                break;
        }

        std::getline(infile, tmp);//read last line
        std::cout << tmp << std::endl; // print it

    }


}


std::string readOpponentPositions(WorldModel *worldModel) {
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
        //cout << playerNum << endl;
        //cout << temp.getY() << endl;
        //cout << temp.getX() << endl;
        writeToFile(positions, playerNum);
    }

    return positions;
}

VecPosition follow(int playerNumber) {
    string str = "python dbnq3_2.py ";
    std::stringstream s;
    s << playerNumber;
    str.append(s.str());
    const char *command = str.c_str();
    system(command);

    string content = "";
    string goal;
    char c = '\0';
    // cout << "Reading file\n";
    int length = 0;
    std::stringstream sss;
    sss << "goal" << playerNumber;
    string fname = sss.str();
    ifstream infile(fname.c_str());
    if (infile) {
        length = infile.tellg();//Get file size

        // loop backward over the file

        for (int i = length - 2; i > 0; i--) {
            infile.seekg(i);
            c = infile.get();
            if (c == '\r' || c == '\n')//new line?
                break;
        }

        std::getline(infile, goal);//read last line
        //std::cout << goal << std::endl; // print it
    }
    VecPosition temp;
    int number;
    std::istringstream iss(goal);
    cout << "Goal :" << goal << endl;
    iss >> number;
    float x = 1.5 + ((number / 10) - 1) * 3 - 15;
    float y = 1 + ((number % 10) - 1) * 2 - 10;
    if (number >= 1 and number <= 100) {
        temp.setX(x);
        temp.setY(y);
        temp.setZ(0);
    } else {
        temp.setX(-30);
        temp.setY(0);
        temp.setZ(0);
    }
    return temp;
}

bool isValidOpponentPosition(VecPosition position) {

    if (position.getX() < -HALF_FIELD_X) {
        return false;
    }

    return true;
}

SkillType NaoBehavior::selectSkill() {

    cout<<"Game Mode"<<worldModel->getPlayMode()<<"\n";

    if (!player.getIsInitialized()) {
        initPlayerObject(worldModel);
    }

    if(worldModel->getPlayMode()==PM_KICK_OFF_LEFT){

        if(worldModel->getUNum()==11){


        }

    }

    if(worldModel->getPlayMode()>=PM_PLAY_ON) {


        int playerClosestToBall = getPlayerClosestToTheBall(worldModel);
        bool isCurrentPlayerClosestToBall = false;

        if (worldModel->getUNum() == playerClosestToBall) {
            isCurrentPlayerClosestToBall = true;
        }

        bool shouldOVerrideAction = false;


        //goal action is highest prioirty
        if (isCurrentPlayerClosestToBall) {
            if (isNearToGoal(worldModel)) {
                player.setIsInvolvedInAction(false);
                return goalingAction();
            }
        }



        // if ball came to current player change his current action
        if (isCurrentPlayerClosestToBall) {
            if (player.getIsInvolvedInAction()) {
                Action action = player.getActionInvolved();
                if (!action.getIsKickingAction()) {
                    //let the code continue
                    shouldOVerrideAction = true;
                } else {
                    //cout << "Action " << player.getPlayerNumber() << " " << action.getKickType()
                    //    << " " << action.getTargetPosition() << "\n";
                }
            }
        } else {
            if (player.getActionInvolved().getIsKickingAction()) {
                //cout << "Other player closer to ball " << playerClosestToBall << "\n";
                shouldOVerrideAction = true;
            }
        }

        int defencePlayerNumberMax;

        if (teamMode.getMode() == Attack) {
            defencePlayerNumberMax = WO_TEAMMATE1 + 4;
        } else {
            defencePlayerNumberMax = WO_TEAMMATE1 + 5;
        }


        if (player.getPlayerNumber() <= defencePlayerNumberMax && player.getAggressiveRating() != 1) {
            std::vector<int> opponentsPlayersInOurField = readOpponentPositionsAndReturnWithInRange(worldModel);

            for (int i = 0; i < opponentsPlayersInOurField.size(); i++) {
                int closePlayerToOpponent = getPlayerClosestToOpponent(worldModel, opponentsPlayersInOurField[i]);
                if (closePlayerToOpponent == worldModel->getUNum()) {
                    VecPosition opponentPosition = follow(opponentsPlayersInOurField[i]);
                    cout << "Opponent Position" << opponentPosition << "\n";
                    if (isValidOpponentPosition(opponentPosition)) {
                        return setTargetPositionAction(&player, opponentPosition);
                    }
                }
            }
        }


        //check if targetIsReached
        if (player.getIsInvolvedInAction() && !player.getActionInvolved().getIsKickingAction()) {
            bool isTargetReached = didReachTargetPosition(worldModel->getMyPosition(),
                                                          player.getActionInvolved().getTargetPosition());
            if (isTargetReached) {
                shouldOVerrideAction = true;
            }
        }


        if (player.getIsInvolvedInAction() && !shouldOVerrideAction) {
            //cout<<"Action involved "<<player.getPlayerNumber();
            return getSkillTypeFromAction(player.getActionInvolved());
            //return SKILL_STAND;
        }

        //Defence,Normal,Attack,SetPiece


        if (teamMode.getMode() == Attack) {
            Action action = playAggressive(&player, playerClosestToBall);
            return getSkillTypeFromAction(action);
        } else {
            Action action = playAggressive(&player, playerClosestToBall);
            return getSkillTypeFromAction(action);
        }


    }

    return SKILL_STAND;

    //if(player.getIsInvolvedInAction()){
    //cout <<"Invovlved in Action selectSkill"<<"\n";
    // }


    //goToTarget(player.getRange().getCenterOfRange());

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


