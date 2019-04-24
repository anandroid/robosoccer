//
// Created by Anand kumar on 2019-04-24.
//

#ifndef UTAUSTINVILLA3D_RANGE_H
#define UTAUSTINVILLA3D_RANGE_H

#endif //UTAUSTINVILLA3D_RANGE_H

class Range {
private:
    VecPosition bottomLeft;
    VecPosition topRight;

public:
    Range(VecPosition bottomLeft, VecPosition topRight);
    VecPosition getBottomLeft();
    VecPosition getTopRight();
};