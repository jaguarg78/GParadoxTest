#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

#include "GUtilities.h"
#include "GTimer.h"
#include "GPathTests.h"

namespace {
    bool isFindPathEnabled = false;
}

struct GNode {
    float  _fGValue;
    float  _fHValue;
    int    _iPrev;
};

int FindPath(const int iStartX, const int iStartY,
             const int iTargetX, const int iTargetY,
             const unsigned char* pMap,
             const int iMapWidth, const int iMapHeight,
             int* pOutBuffer, const int iOutBufferSize) {

    int iStartIndex = Utilities::GetIndex(iStartX, iStartY, iMapWidth, iMapHeight);
    int iTargetIndex = Utilities::GetIndex(iTargetX, iTargetY, iMapWidth, iMapHeight);
    int iMapSize = iMapWidth * iMapHeight;

    ///////////////////////////////////////////
    // Validate initial information
    ///////////////////////////////////////////
    if (iStartIndex == Constants::INVALID_VALUE || iTargetIndex == Constants::INVALID_VALUE) {
        return Constants::NOK;
    }

    // Variable initialization
    isFindPathEnabled = true;
    bool wasFound = false;
    int counter = 0;

    // Pair <(g() + h()), Index>
    std::priority_queue<std::pair<float, int>,
                        std::vector<std::pair<float, int>>,
                        std::greater<std::pair<float, int>>> pQueue;

    // Array of GNodes
    std::vector<GNode> vecSummary(iMapSize, { Constants::UNDEFINED_VALUE_FLOAT,
                                              Constants::UNDEFINED_VALUE_FLOAT,
                                              Constants::UNDEFINED_VALUE_INT });

    ///////////////////////////////////////////
    // A* Algorithm
    ///////////////////////////////////////////

    vecSummary[iStartIndex] = { 0.0f,                              // _fGValue
                                Utilities::GetHeuristic(iStartX,   // _fHValue
                                                        iStartY,
                                                        iTargetX,
                                                        iTargetY),
                                Constants::UNDEFINED_VALUE_INT };   // _iPrev
    pQueue.push(std::make_pair(0.0f, iStartIndex));


    GTimer timer;
    timer.StartTimer(Constants::MAX_CALCULATION_TIME_US, [] { isFindPathEnabled = false; });

    const auto tmStart = std::chrono::high_resolution_clock::now();

    while (!pQueue.empty() && !wasFound && isFindPathEnabled) {
        int iFromIndex = pQueue.top().second;
        pQueue.pop();

        for (const auto move : Constants::ALLOWED_MOVES) {
            int iToIndex = Constants::UNDEFINED_VALUE_INT;

            if (Utilities::IsValidMove(iToIndex, iFromIndex, iTargetIndex,
                                       iMapWidth, iMapHeight, pMap,
                                       move)) {

                // NOTE: In the case of allowed diagonal moves, use GetWeight() instead Constants::GVALUE_DEFAULT
                if (vecSummary[iToIndex]._fHValue == Constants::UNDEFINED_VALUE_FLOAT ||
                    vecSummary[iToIndex]._fGValue > (vecSummary[iFromIndex]._fGValue + Constants::GVALUE_DEFAULT)) {

                    // Calculate heuristic just once per GNode
                    if (vecSummary[iToIndex]._fHValue == Constants::UNDEFINED_VALUE_FLOAT) {
                        vecSummary[iToIndex]._fHValue = Utilities::GetHeuristic(Utilities::GetX(iToIndex, iMapWidth),
                                                                                Utilities::GetY(iToIndex, iMapWidth),
                                                                                iTargetX,
                                                                                iTargetY);
                    }

                    // Update Values
                    vecSummary[iToIndex]._fGValue = vecSummary[iFromIndex]._fGValue + Constants::GVALUE_DEFAULT;
                    vecSummary[iToIndex]._iPrev = iFromIndex;

                    // Push to priority_queue
                    pQueue.push(std::make_pair(vecSummary[iToIndex]._fGValue + vecSummary[iToIndex]._fHValue, iToIndex));
                }

                counter++;
            }

            if (iToIndex == iTargetIndex) {
                wasFound = true;
            }
        }
    }

    std::cout << "Elapsed: " << std::chrono::duration_cast<std::chrono::microseconds>(
                                       std::chrono::high_resolution_clock::now() - tmStart).count() << " us" << std::endl;

    timer.Stop();

    ///////////////////////////////////////////
    // Final information analysis and Values to return
    ///////////////////////////////////////////

    if (wasFound) {
        std::vector<int> vecPath;
        vecPath.push_back(iTargetIndex);

        unsigned uiIndex = vecSummary[iTargetIndex]._iPrev;
        while (uiIndex != iStartIndex) {
            vecPath.push_back(uiIndex);
            uiIndex = vecSummary[uiIndex]._iPrev;
        }

        if (vecPath.size() <= static_cast<unsigned>(iOutBufferSize)) {
            std::reverse(std::begin(vecPath), std::end(vecPath));
            std::memcpy(pOutBuffer, vecPath.data(), sizeof(int) * vecPath.size());
        }

        return static_cast<int>(vecPath.size());
    }
    else if (!isFindPathEnabled) {
        std::cout << "\tCalculation time is over with no findings" << std::endl;
    }
    else {
        std::cout << "\tPath NOT found :(" << std::endl;
    }

    return Constants::NOK;
}


int ExecuteFindPath(Input& input, const std::vector<unsigned char>& map);
void PathSplicing(Input& input, const std::vector<unsigned char>& map, const std::vector<int>& modifiedNodes);

void ExecuteTests();
void PrintPath(const std::vector<int>& oBuffer, int iPathSize);
/////////////////////////////////////////
// main Code
/////////////////////////////////////////

int main(int argc, char *argv[]) {
    std::cout << "Start App..." << std::endl;

    ExecuteTests();

    std::cout << "End App..." << std::endl;

    std::cin.get();

    return 0;
}

////////////////////////////////////////////////////////
// Input functions implementation
////////////////////////////////////////////////////////

void PrintPath(const std::vector<int>& oBuffer, int iPathSize) {
    std::cout << "//////// Path: ";
    for (int i = 0; i < iPathSize; ++i) {
        std::cout << oBuffer[i] << " ";
    }

    std::cout << "//////////////////////" << std::endl;
}

int ExecuteFindPath(Input& input, const std::vector<unsigned char>& map) {
    std::lock_guard<std::mutex> guard(mtxData);

    input._path.resize(input._iBufferSize);

    int iResult;
    while ((iResult = FindPath(input._sourceCoord.first, input._sourceCoord.second,
                               input._targetCoord.first, input._targetCoord.second,
                               map.data(),
                               input._mapDimensions.first, input._mapDimensions.second,
                               input._path.data(), input._iBufferSize)) != Constants::NOK &&
                               input._isRetryEnabled) {

        if (iResult > input._iBufferSize && input._isRetryEnabled) {
            std::cout << "Resizing Buffer " << input._path.size() << " -> " << iResult << std::endl;

            input._path.resize(iResult);
            input._iBufferSize = iResult;
        } else {
            break;
        }

        input._isRetryEnabled = 0;
    }

    if (iResult != Constants::NOK) {
        if (iResult <= input._iBufferSize) {
            PrintPath(input._path, iResult);
        }
        else {
            std::cerr << "\tBuffer Size less than specified and retry disabled." << std::endl;
        }
    }

    return iResult;
}

/////////////////////////////////////
// Basic Path Splicing
/////////////////////////////////////
/*
* Path recalculation was triggered, whether by cycling recalculation or by the action from any map changing event.
* This function is called with the list of modifiedNodes, if any of those nodes are in the found path, the minIndex
* is found to know the nodes that will be discarded and from where the new calculation should start. Otherwise,
* no additional calculation is made.
*/
void PathSplicing(Input& input,
    const std::vector<unsigned char>& map,
    const std::vector<int>& modifiedNodes) {

    std::cout << "Path Splicing. Recalculating..." << std::endl;
    // Finding min changed Index in path
    std::size_t uiMinIndex = input._path.size();
    for (int node : modifiedNodes) {
        std::vector<std::pair<int, int>> toValidIndex(Constants::ALLOWED_MOVES.begin(), Constants::ALLOWED_MOVES.end());
        toValidIndex.push_back(std::make_pair(0, 0));

        for (const auto move : toValidIndex) {
            int iToIndex = Utilities::GetToIndex(node,
                                                 input._mapDimensions.first, input._mapDimensions.second,
                                                 move);

            if (iToIndex != Constants::UNDEFINED_VALUE_INT) {
                auto pathIndex = std::find(input._path.begin(), input._path.end(), iToIndex);

                if (pathIndex != input._path.end() && uiMinIndex > static_cast<unsigned>(pathIndex - input._path.begin())) {
                    uiMinIndex = pathIndex - input._path.begin();
                }
            }
        }
    }

    if (uiMinIndex != input._path.size()) {
        // Recalculating path from minIndex - 1
        if (uiMinIndex > 0) {
            Input tmpInput;
            tmpInput._mapDimensions = input._mapDimensions;
            tmpInput._sourceCoord = std::make_pair(Utilities::GetX(input._path[uiMinIndex - 1], input._mapDimensions.first),
                                                   Utilities::GetY(input._path[uiMinIndex - 1], input._mapDimensions.first));
            tmpInput._targetCoord = input._targetCoord;
            tmpInput._iBufferSize = input._iBufferSize;
            tmpInput._isRetryEnabled = 1;

            int iResult = ExecuteFindPath(tmpInput, map);
            if (iResult != Constants::NOK) {
                if (input._path.size() != uiMinIndex + iResult) {
                    input._path.resize(uiMinIndex + iResult);
                    std::copy(tmpInput._path.begin(), tmpInput._path.begin() + iResult, input._path.begin() + uiMinIndex);
                }

                PrintPath(input._path, static_cast<int>(input._path.size()));
            } else {
                std::cerr << "Error. Recaulculation failed. ("
                    << tmpInput._sourceCoord.first << ", " << tmpInput._sourceCoord.second << ") -> ("
                    << tmpInput._targetCoord.first << ", " << tmpInput._targetCoord.second << ")" << std::endl;
            }
        } else {
            int iResult = ExecuteFindPath(input, map);

            if (iResult != Constants::NOK) {
                PrintPath(input._path, static_cast<int>(input._path.size()));
            }
        }
    }
}

void ExecuteTests() {
    std::cout << "Start Tests..." << std::endl;
    TestCase tCase = TestCase::TEST_HARDCODED_1;

    while (tCase != TestCase::NO_TEST) {
        Input input;
        std::vector<unsigned char> map;

        GPathTest::GetTestData(input, map, tCase);
        ExecuteFindPath(input, map);

        // Path recalculation Test
        if (tCase == TestCase::TEST_HARDCODED_5) {
            /*
            * Other thread or event can change the map then Path must be recalculated
            * based on the list of modified Nodes.
            */
            map =
                // 0  1  2  3  4  5  6  7  8  9   0  1  2  3  4  5  6  7  8  9
                 { 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,  1, 0, 1, 1, 1, 1, 1, 1, 1, 1,   // 00
                   1, 0, 0, 0, 0, 0, 0, 0, 0, 0,  1, 1, 1, 0, 0, 0, 0, 0, 0, 1,   // 20 
                   1, 0, 0, 1, 1, 1, 0, 1, 1, 1,  0, 0, 0, 0, 0, 1, 1, 1, 0, 1,   // 40 
                   1, 0, 0, 1, 0, 1, 0, 1, 0, 1,  0, 1, 1, 1, 0, 1, 0, 1, 0, 1,   // 60 
                   1, 0, 0, 1, 0, 1, 0, 1, 0, 1,  0, 1, 0, 1, 0, 1, 0, 1, 0, 1,   // 80 
                   1, 0, 0, 1, 1, 1, 0, 1, 0, 1,  0, 1, 0, 1, 0, 1, 0, 1, 0, 1,   // 100 
                   1, 0, 0, 1, 0, 1, 0, 1, 0, 1,  0, 1, 0, 1, 0, 1, 0, 1, 0, 1,   // 120 
                   1, 0, 1, 1, 0, 1, 1, 1, 0, 1,  1, 1, 0, 1, 1, 1, 0, 1, 1, 1,   // 140 
                   1, 1, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 1 }; // 160
            std::vector<int> modifiedNodes = { 60, 161, 104 };

            PathSplicing(input, map, modifiedNodes);
        }

        switch (tCase) {
        case TestCase::TEST_HARDCODED_1:
            tCase = TestCase::TEST_HARDCODED_2;

            break;
        case TestCase::TEST_HARDCODED_2:
            tCase = TestCase::TEST_HARDCODED_3;

            break;
        case TestCase::TEST_HARDCODED_3:
            tCase = TestCase::TEST_HARDCODED_4;

            break;
        case TestCase::TEST_HARDCODED_4:
            tCase = TestCase::TEST_FILE;

            break;
        case TestCase::TEST_FILE:
            tCase = TestCase::TEST_HARDCODED_5;

            break;
        case TestCase::TEST_HARDCODED_5:
            tCase = TestCase::NO_TEST;

            break;
        default: break;
        }
    }

    std::cout << "End Tests..." << std::endl;
}