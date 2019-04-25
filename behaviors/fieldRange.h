//
// Created by Anand kumar on 2019-04-24.
//

#ifndef UTAUSTINVILLA3D_RANGE_H
#define UTAUSTINVILLA3D_RANGE_H

class FieldRange {
private:
    VecPosition bottomLeft;
    VecPosition topRight;

public:
    FieldRange(VecPosition bottomLeft, VecPosition topRight);

    VecPosition getBottomLeft();
    VecPosition getTopRight();
};

#endif //UTAUSTINVILLA3D_RANGE_H

