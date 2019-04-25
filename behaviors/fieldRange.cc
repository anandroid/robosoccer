


#include "fieldRange.h"

FieldRange::FieldRange() {

}

FieldRange :: FieldRange(VecPosition mBottomLeft, VecPosition mTopRight){
    bottomLeft = mBottomLeft;
    topRight = mTopRight;
}

VecPosition FieldRange::getBottomLeft(){
    return bottomLeft;
}

VecPosition FieldRange::getTopRight(){
    return topRight;
}

