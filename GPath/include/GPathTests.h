#ifndef GPATHTEST_H
#define GPATHTEST_H

#include <fstream>
#include <algorithm>
#include <mutex>

#include "GUtilities.h"

typedef enum eTestCase {
    TEST_FILE,
    TEST_HARDCODED_1,
    TEST_HARDCODED_2,
    TEST_HARDCODED_3,
    TEST_HARDCODED_4,
    TEST_HARDCODED_5,
    NO_TEST
} TestCase;

namespace {
    std::mutex      mtxData;
}

struct Input {
    std::pair<int, int> _mapDimensions;
    std::string			_mapDataFileName;
    std::pair<int, int> _sourceCoord;
    std::pair<int, int> _targetCoord;
    int 				_iBufferSize;
    int					_isRetryEnabled;

    std::vector<int>    _path;
};

std::ostream& operator<<(std::ostream& os, const Input& input);

namespace GPathTest {

    void InputPopulation(Input& input);
    bool AreCoordsValid(const Input& input);

    int GetMapDataFromFile(std::vector<unsigned char>& map,
                           const Input& input);
    int GetTestData(Input& input,
                    std::vector<unsigned char>& map,
                    const TestCase tCase);
}

#endif /* GPATHTEST_H */
