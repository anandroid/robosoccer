//
// Created by Anand kumar on 2019-04-24.
//

#ifndef UTAUSTINVILLABASE_RANGE_H
#define UTAUSTINVILLABASE_RANGE_H

class FieldRange {
private:
    VecPosition bottomLeft;
    VecPosition topRight;

public:
//    FieldRange(VecPosition bottomLeft, VecPosition topRight);

    VecPosition getBottomLeft();
    VecPosition getTopRight();
//    VecPosition getCenterOfRange();

    FieldRange(){

    }
    FieldRange(VecPosition mBottomLeft, VecPosition mTopRight){
        bottomLeft = mBottomLeft;
        topRight = mTopRight;
    }
    VecPosition getCenterOfRange() {
        return VecPosition((bottomLeft.getX()+topRight.getX())/2, (bottomLeft.getY()+topRight.getY())/2,0);
    }
};

#endif //UTAUSTINVILLABASE_RANGE_H

