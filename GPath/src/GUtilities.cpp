#include "GUtilities.h"

#include <algorithm>

int Utilities::GetX(int iIndex, int iWidth) {
    return iIndex % iWidth;
}

int Utilities::GetY(int iIndex, int iWidth) {
    return iIndex / iWidth;
}

int Utilities::GetIndex(int iX, int iY, int iWidth, int iHeight) {
    // Out of bounds
    if (iX < 0 || iX >= iWidth || iY < 0 || iY >= iHeight) {
        return Constants::INVALID_VALUE;
    }

    return (iY * iWidth) + iX;
}

float Utilities::GetWeight(int iFromX, int iFromY,
    int iToX, int iToY) {

    if (iToX - iFromX == 0.0f && iToY - iFromY != 0.0f) {
        // Vertical Move
        return static_cast<float>(std::abs(iToY - iFromY));
    }
    else if (iToX - iFromX != 0.0f && iToY - iFromY == 0.0f) {
        // Horizontal Move
        return static_cast<float>(std::abs(iToX - iFromX));
    }

    // Diagonal Move
    return std::sqrt(std::pow(static_cast<float>(iToX - iFromX), 2) + std::pow(static_cast<float>(iToY - iFromY), 2));
}

float Utilities::GetHeuristic(int iIndexX, int iIndexY,
                              int iTargetIndexX, int iTargetIndexY,
                              Constants::HeuristicType type) {
    switch (type) {
    case Constants::HeuristicType::MANHATTAN:
        return static_cast<float>(std::abs(iTargetIndexX - iIndexX) + std::abs(iTargetIndexY - iIndexY));
    case Constants::HeuristicType::DIAGONAL:
        return static_cast<float>(std::max(std::abs(iTargetIndexX - iIndexX), std::abs(iTargetIndexY - iIndexY)));
    case Constants::HeuristicType::EUCLIDIAN:
        return std::sqrt(std::pow(static_cast<float>(iTargetIndexX - iIndexX), 2) + std::pow(static_cast<float>(iTargetIndexY - iIndexY), 2));
    default:
        break;
    }

    return Constants::UNDEFINED_VALUE_FLOAT;
}

int Utilities::GetToIndex(const int iFromIndex,
                          const int iWidth, const int iHeight,
                          const std::pair<int, int>& move) {
    return GetIndex(GetX(iFromIndex, iWidth) + move.first,
                    GetY(iFromIndex, iWidth) + move.second,
                    iWidth, iHeight);
}

bool Utilities::IsValidMove(int& iToIndex,
                            const int iFromIndex,
                            const int iTargetIndex,
                            const int iWidth, const int iHeight,
                            const unsigned char* pMap,
                            const std::pair<int, int>& move) {
    iToIndex = GetToIndex(iFromIndex, iWidth, iHeight, move);

    return iToIndex != -1 &&                                    // To Index whithin Map
           (pMap[iToIndex] == Constants::ENABLED_CELL_VALUE ||  // Enabled Cell
                  iToIndex == iTargetIndex);                    // Target can be 0
}
