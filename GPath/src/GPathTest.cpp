#include "GPathTests.h"

#include <cctype>

void GPathTest::InputPopulation(Input& input) {
    int x, y;

    std::cout << "Map Dimensions: (width height): ";
    std::cin >> x >> y;
    input._mapDimensions.first = x;
    input._mapDimensions.second = y;

    std::cout << "Map Data File Name: ";
    std::cin >> input._mapDataFileName;

    std::cout << "Source Coordinates (x y): ";
    std::cin >> x >> y;
    input._sourceCoord.first = x;
    input._sourceCoord.second = y;

    std::cout << "Target Coordinates (x y): ";
    std::cin >> x >> y;
    input._targetCoord.first = x;
    input._targetCoord.second = y;

    std::cout << "Buffer Size: ";
    std::cin >> input._iBufferSize;

    std::cout << "Enable Retry (1/0): ";
    std::cin >> input._isRetryEnabled;
}

bool GPathTest::AreCoordsValid(const Input& input) {
    return input._sourceCoord.first >= 0 && input._sourceCoord.first < input._mapDimensions.first &&
           input._sourceCoord.second >= 0 && input._sourceCoord.second < input._mapDimensions.second &&
           input._targetCoord.first >= 0 && input._targetCoord.first < input._mapDimensions.first &&
           input._targetCoord.second >= 0 && input._targetCoord.second < input._mapDimensions.second;
}

int GPathTest::GetMapDataFromFile(std::vector<unsigned char>& map, const Input& input) {
    std::ifstream mapFile(input._mapDataFileName.c_str(), std::ios::in | std::ios::ate);

    if (mapFile.is_open()) {
        std::streamoff stSize = mapFile.tellg();

        if (stSize != map.size()) {
            std::cerr << "Error. Map dimensions don't match with expected values or file has additional characters."
                << " Expected: " << input._mapDimensions.first * input._mapDimensions.second
                << " FileSize: " << stSize << "." << std::endl;

            return Constants::NOK;
        }

        mapFile.seekg(0, std::ios::beg);
        mapFile.read(reinterpret_cast<char *>(map.data()), stSize);

        mapFile.close();

        std::transform(map.begin(),
            map.end(),
            map.begin(),
            [](char cValue) -> char {
            return std::isdigit(static_cast<unsigned char>(cValue)) ?
                                                 (cValue - '0') : 1; });
    }  else {
        std::cerr << "Error. "
                  << "File: " << input._mapDataFileName
                  << " doesn't exist or couldn't be opened." << std::endl;

        return Constants::NOK;
    }

    return Constants::OK;
}

int GPathTest::GetTestData(Input& input, std::vector<unsigned char>& map, const TestCase tCase) {
    std::lock_guard<std::mutex> guard(mtxData);

    switch (tCase) {
    case TestCase::TEST_FILE:
        std::cout << "/////////////////  Test File:  ////////////////" << std::endl;
        // Map information from file and std::cin
        // Test 1 -> 4 3; test_1.map; 0 0; 1 2; 3; 1
        //        Path: 1 5 9
        // Test 2 -> 3 3; test_2.map; 2 0; 0 2; 7; 1
        //        Path NOT found
        // Test 3 -> 10 9; test_3.map; 9 0; 9 5; 12; 1
        //        Path: 8 7 6 16 15 25 24 34 44 45 55 65 75 76 77 78 79 69 59
        // Test 4 -> 10 9; test_4.map; 9 0; 9 5; 12; 
        //        Path NOT found
        // Test 5 -> 20 9; test_5.map; 1 0; 2 8; 45; 1
        //        Path: 2 3 4 5 6 7 8 9 10 30 31 32 12 13 14 15 16 17 18 19 39 59 79 99 119 
        //              139 159 158 157 137 117 97 77 57 56 55 75 95 115 135 155 154 153 133 
        //              113 93 73 72 71 91 111 131 151 150 149 129 109 89 69 49 48 47 67 87  
        //              107 127 147 146 145 125 105 85 65 45 44 43 63 83 103 123 143 142 162
        //#if 0
        InputPopulation(input);
        if (!AreCoordsValid(input)) {
            std::cerr << "Error. Either Source or Target coords are out map Dimensions."
                << " Map   : (" << input._mapDimensions.first << ", " << input._mapDimensions.second << ")\n"
                << " Source: (" << input._sourceCoord.first << ", " << input._sourceCoord.second << ")\n"
                << " Target: (" << input._targetCoord.first << ", " << input._targetCoord.second << ")" << std::endl;

            return Constants::NOK;
        }

        map.resize(input._mapDimensions.first * input._mapDimensions.second);

        std::cout << input << std::endl;

        return GetMapDataFromFile(map, input);
    case TestCase::TEST_HARDCODED_1:
        std::cout << "///////////////// Test Case 1: ////////////////" << std::endl;
        // Test 1 -> Path: 1 5 9
        input._mapDimensions = std::make_pair(4, 3);
        input._sourceCoord = std::make_pair(0, 0);
        input._targetCoord = std::make_pair(1, 2);
        input._iBufferSize = 3;
        input._isRetryEnabled = 1;
        map =
            { 1, 1, 1, 1,
              0, 1, 0, 1,
              0, 1, 1, 0 };

        break;
    case TestCase::TEST_HARDCODED_2:
        std::cout << "///////////////// Test Case 2: ////////////////" << std::endl;
        // Test 2 -> Path NOT found
        input._mapDimensions = std::make_pair(3, 3);
        input._sourceCoord = std::make_pair(2, 0);
        input._targetCoord = std::make_pair(0, 2);
        input._iBufferSize = 7;
        input._isRetryEnabled = 1;
        map =
            { 0, 0, 1,
              0, 1, 1,
              1, 0, 1 };

        break;
    case TestCase::TEST_HARDCODED_3:
        std::cout << "///////////////// Test Case 3: ////////////////" << std::endl;
        // Test 3 -> BufferSize not enough. Resize 12 -> 19
        // Path: 8 7 6 16 15 25 24 34 44 45 55 65 75 76 77 78 79 69 59
        input._mapDimensions = std::make_pair(10, 9);
        input._sourceCoord = std::make_pair(9, 0);
        input._targetCoord = std::make_pair(9, 5);
        input._iBufferSize = 12;
        input._isRetryEnabled = 1;
        map =
            // 0  1  2  3  4  5  6  7  8  9
             { 1, 0, 1, 1, 1, 1, 1, 1, 1, 1,     // 00
               1, 1, 1, 0, 1, 1, 1, 0, 1, 1,     // 10 
               1, 1, 1, 0, 1, 1, 0, 1, 0, 1,     // 20 
               0, 0, 1, 0, 1, 0, 0, 0, 0, 1,     // 30 
               1, 1, 1, 0, 1, 1, 1, 0, 1, 0,     // 40 
               1, 0, 1, 1, 1, 1, 0, 1, 0, 0,     // 50 
               1, 0, 0, 0, 0, 1, 0, 0, 0, 1,     // 60 
               1, 0, 1, 1, 1, 1, 1, 1, 1, 1,     // 70 
               1, 1, 1, 0, 0, 0, 1, 0, 0, 1 };   // 80

        break;
    case TestCase::TEST_HARDCODED_4:
        std::cout << "///////////////// Test Case 4: ////////////////" << std::endl;
        // Test 4 -> Path NOT found
        input._mapDimensions = std::make_pair(10, 9);
        input._sourceCoord = std::make_pair(9, 0);
        input._targetCoord = std::make_pair(9, 5);
        input._iBufferSize = 12;
        input._isRetryEnabled = 1;
        map =
            // 0  1  2  3  4  5  6  7  8  9
             { 1, 0, 1, 1, 1, 1, 0, 1, 1, 1,     // 00
               1, 1, 1, 0, 1, 1, 1, 0, 1, 1,     // 10 
               1, 1, 1, 0, 1, 1, 0, 1, 0, 1,     // 20 
               0, 0, 1, 0, 1, 0, 0, 0, 0, 1,     // 30 
               1, 1, 1, 0, 1, 1, 1, 0, 1, 0,     // 40 
               1, 0, 1, 1, 1, 1, 0, 1, 0, 0,     // 50 
               1, 0, 0, 0, 0, 1, 0, 0, 0, 1,     // 60 
               1, 0, 1, 1, 1, 1, 0, 1, 1, 1,     // 70 
               1, 1, 1, 0, 0, 0, 1, 0, 0, 1 };   // 80

        break;
    case TestCase::TEST_HARDCODED_5:
        std::cout << "///////////////// Test Case 5: ////////////////" << std::endl;
        // Test 5 -> BufferSize not enough. Resize 45 -> 83
        // Path: 2 3 4 5 6 7 8 9 10 30 31 32 12 13 14 15 16 17 18 19 39 59 79 99 119 
        //       139 159 158 157 137 117 97 77 57 56 55 75 95 115 135 155 154 153 133 
        //       113 93 73 72 71 91 111 131 151 150 149 129 109 89 69 49 48 47 67 87  
        //       107 127 147 146 145 125 105 85 65 45 44 43 63 83 103 123 143 142 162
        input._mapDimensions = std::make_pair(20, 9);
        input._sourceCoord = std::make_pair(1, 0);
        input._targetCoord = std::make_pair(2, 8);
        input._iBufferSize = 45;
        input._isRetryEnabled = 1;
        map =
            // 0  1  2  3  4  5  6  7  8  9   0  1  2  3  4  5  6  7  8  9
             { 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,  1, 0, 1, 1, 1, 1, 1, 1, 1, 1,   // 00
               1, 0, 0, 0, 0, 0, 0, 0, 0, 0,  1, 1, 1, 0, 0, 0, 0, 0, 0, 1,   // 20 
               1, 0, 0, 1, 1, 1, 0, 1, 1, 1,  0, 0, 0, 0, 0, 1, 1, 1, 0, 1,   // 40 
               0, 0, 0, 1, 0, 1, 0, 1, 0, 1,  0, 1, 1, 1, 0, 1, 0, 1, 0, 1,   // 60 
               1, 0, 0, 1, 0, 1, 0, 1, 0, 1,  0, 1, 0, 1, 0, 1, 0, 1, 0, 1,   // 80 
               1, 0, 0, 1, 0, 1, 0, 1, 0, 1,  0, 1, 0, 1, 0, 1, 0, 1, 0, 1,   // 100 
               1, 0, 0, 1, 0, 1, 0, 1, 0, 1,  0, 1, 0, 1, 0, 1, 0, 1, 0, 1,   // 120 
               1, 0, 1, 1, 0, 1, 1, 1, 0, 1,  1, 1, 0, 1, 1, 1, 0, 1, 1, 1,   // 140 
               1, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 1 }; // 160

        break;
    default:
        break;
    }

    std::cout << input << std::endl;

    return Constants::OK;
}

std::ostream& operator<<(std::ostream& os, const Input& input) {
    os << "**********************" << "\n"
        << "\t" << "Map Dimensions: (" << input._mapDimensions.first << ", " << input._mapDimensions.second << ")\n"
        << "\t" << "Map DataFile  : " << input._mapDataFileName << "\n"
        << "\t" << "Source Coords : (" << input._sourceCoord.first << ", " << input._sourceCoord.second << ")\n"
        << "\t" << "Target Coords : (" << input._targetCoord.first << ", " << input._targetCoord.second << ")\n"
        << "\t" << "Buffer Size   : " << input._iBufferSize << "\n"
        << "\t" << "Retry         : " << (!!input._isRetryEnabled ? "enabled" : "disabled") << "\n"
        << "**********************" << std::endl;

    return os;
}