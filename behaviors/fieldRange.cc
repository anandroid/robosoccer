


#include "fieldRange.h"

/*FieldRange::FieldRange() {

}*/

FieldRange::FieldRange() {

}

FieldRange::FieldRange(VecPosition mBottomLeft, VecPosition mTopRight):topRight(mTopRight),bottomLeft(mBottomLeft){
    bottomLeft = mBottomLeft;
    topRight = mTopRight;
}

VecPosition FieldRange::getBottomLeft(){
    return bottomLeft;
}

VecPosition FieldRange::getTopRight(){
    return topRight;
}

VecPosition FieldRange::getCenterOfRange() {
    return VecPosition((bottomLeft.getX()+topRight.getX())/2, (bottomLeft.getY+topRight.getY())/2,0);
}

