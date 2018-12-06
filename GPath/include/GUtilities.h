#ifndef UTILITIES_H
#define UTILITIES_H

#include <iostream>
#include <sstream>
#include <vector>

namespace Constants {
    const std::vector<std::pair<int, int>> ALLOWED_MOVES = {
        { 0, -1 }, // up
        {-1,  0 }, // left
        { 1,  0 }, // right
        { 0,  1 }  // down
    };

    const int OK = 0;
    const int NOK = -1;

    const int UNDEFINED_VALUE_INT = -1;
    const float UNDEFINED_VALUE_FLOAT = -1.0f;
    const int INVALID_VALUE = -1;

    const float GVALUE_DEFAULT = 1.0f;
    const int ENABLED_CELL_VALUE = 1;

    const long long MAX_CALCULATION_TIME_US = 5000000LL;

    typedef enum eVectorOrder {
        ROW_MAJOR,
        COLUMN_MAJOR
    } VectorOrder;

    typedef enum eHeuristicType {
        MANHATTAN,
        DIAGONAL,
        EUCLIDIAN
    } HeuristicType;
}

namespace Utilities {
    int GetX(int iIndex, int iWidth);
    int GetY(int iIndex, int iWidth);
    int GetIndex(int iX, int iY, int iWidth, int iHeight);

    float GetWeight(int iFromX, int iFromY,
                    int iToX, int iToY);
    float GetHeuristic(int iIndexX, int iIndexY,
                       int iTargetIndexX, int iTargetIndexY,
                       Constants::HeuristicType type = Constants::HeuristicType::MANHATTAN);

    int GetToIndex(const int iFromIndex,
                   const int iWidth, const int iHeight,
                   const std::pair<int, int>& move);
    bool IsValidMove(int& iToIndex,
                     const int iFromIndex,
                     const int iTargetIndex,
                     const int iWidth, const int iHeight,
                     const unsigned char* pMap,
                     const std::pair<int, int>& move);
}

#endif /* UTILITIES_H */
