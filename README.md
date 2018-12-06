# GParadoxTest
Paradox Test: PathFinding

This is a test to apply to Paradox.

This is an implementation of A\* algorigthm to find the path between 2 points (x, y) - Source, Target - into a map with obstacles.

Features:
    - Map information can be retrieved from test_\*.map files in maps directory or harcoded using the tests. 
    - A\* algorithm Implementation.
    - Basic Timer to define a max time for calculation (limit 5s - 5000000 us).
    - PathSlicing in the case that map is been modified, then path should be recalculated.
    - Unit test (\*).

----
Use file GPath.sln to open the project
----

(\*) Unit Test

The optimal way to do this is using a framework as gtest, but for this test it is done manually.

Cases:

1. 
        Map Dimensions: (4, 3)
        Source Coords : (0, 0)
        Target Coords : (1, 2)
        Buffer Size   : 3
        Retry         : enabled

           | 0 1 2 3
        ---+---------
         0 | 1 1 1 1
         4 | 0 1 0 1
         8 | 0 1 1 0

Path: 1 5 9
2.
        Map Dimensions: (3, 3)
        Source Coords : (2, 0)
        Target Coords : (0, 2)
        Buffer Size   : 7
        Retry         : enabled
             0 1 2 
        ---+-------
         0 | 0 0 1
         3 | 0 1 1
         6 | 1 0 1

Path NOT found
3.
        Map Dimensions: (10, 9)
        Source Coords : (9, 0)
        Target Coords : (9, 5)
        Buffer Size   : 12
        Retry         : enabled

             0 1 2 3 4 5 6 7 8 9
        ---+----------------------
        00 | 1 0 1 1 1 1 1 1 1 1 
        10 | 1 1 1 0 1 1 1 0 1 1  
        20 | 1 1 1 0 1 1 0 1 0 1  
        30 | 0 0 1 0 1 0 0 0 0 1  
        40 | 1 1 1 0 1 1 1 0 1 0  
        50 | 1 0 1 1 1 1 0 1 0 0  
        60 | 1 0 0 0 0 1 0 0 0 1  
        70 | 1 0 1 1 1 1 1 1 1 1  
        80 | 1 1 1 0 0 0 1 0 0 1 

Resizing Buffer 12 -> 19
Path: 8 7 6 16 15 25 24 34 44 45 55 65 75 76 77 78 79 69 59
4.
        Map Dimensions: (10, 9)
        Source Coords : (9, 0)
        Target Coords : (9, 5)
        Buffer Size   : 12
        Retry         : enabled

             0 1 2 3 4 5 6 7 8 9
        ---+----------------------
        00 | 1 0 1 1 1 1 0 1 1 1
        10 | 1 1 1 0 1 1 1 0 1 1
        20 | 1 1 1 0 1 1 0 1 0 1
        30 | 0 0 1 0 1 0 0 0 0 1
        40 | 1 1 1 0 1 1 1 0 1 0
        50 | 1 0 1 1 1 1 0 1 0 0
        60 | 1 0 0 0 0 1 0 0 0 1
        70 | 1 0 1 1 1 1 0 1 1 1
        80 | 1 1 1 0 0 0 1 0 0 1

Path NOT found
5.
        Map Dimensions: (20, 9)
        Source Coords : (1, 0)
        Target Coords : (2, 8)
        Buffer Size   : 45
        Retry         : enabled

              0 1 2 3 4 5 6 7 8 9  0 1 2 3 4 5 6 7 8 9  
        ----+-------------------------------------------
         00 | 0 0 1 1 1 1 1 1 1 1  1 0 1 1 1 1 1 1 1 1
         20 | 1 0 0 0 0 0 0 0 0 0  1 1 1 0 0 0 0 0 0 1
         40 | 1 0 0 1 1 1 0 1 1 1  0 0 0 0 0 1 1 1 0 1
         60 | 0 0 0 1 0 1 0 1 0 1  0 1 1 1 0 1 0 1 0 1
         80 | 1 0 0 1 0 1 0 1 0 1  0 1 0 1 0 1 0 1 0 1
        100 | 1 0 0 1 0 1 0 1 0 1  0 1 0 1 0 1 0 1 0 1
        120 | 1 0 0 1 0 1 0 1 0 1  0 1 0 1 0 1 0 1 0 1
        140 | 1 0 1 1 0 1 1 1 0 1  1 1 0 1 1 1 0 1 1 1
        160 | 1 0 0 0 0 0 0 0 0 0  0 0 0 0 0 0 0 0 0 1 

Resizing Buffer 45 -> 83
Path: 2 3 4 5 6 7 8 9 10 30 31 32 12 13 14 15 16 17 18 19 39 59 79 99 119 139 159 158 157 137 117 97 77 57 56 55 75 95 115 135 155 154 153 133 113 93 73 72 71 91 111 131 151 150 149 129 109 89 69 49 48 47 67 87 107 127 147 146 145 125 105 85 65 45 44 43 63 83 103 123 143 142 162
           
              0 1 2 3 4 5 6 7 8 9  0 1 2 3 4 5 6 7 8 9  
        ----+-------------------------------------------
         00 | 0 0 1 1 1 1 1 1 1 1  1 0 1 1 1 1 1 1 1 1
         20 | 1 0 0 0 0 0 0 0 0 0  1 1 1 0 0 0 0 0 0 1
         40 | 1 0 0 1 1 1 0 1 1 1  0 0 0 0 0 1 1 1 0 1
         60 |*1*0 0 1 0 1 0 1 0 1  0 1 1 1 0 1 0 1 0 1
         80 | 1 0 0 1 0 1 0 1 0 1  0 1 0 1 0 1 0 1 0 1
        100 | 1 0 0 1*1*1 0 1 0 1  0 1 0 1 0 1 0 1 0 1
        120 | 1 0 0 1 0 1 0 1 0 1  0 1 0 1 0 1 0 1 0 1
        140 | 1 0 1 1 0 1 1 1 0 1  1 1 0 1 1 1 0 1 1 1
        160 | 1*1*0 0 0 0 0 0 0 0  0 0 0 0 0 0 0 0 0 1                                                                                                                                                                                                                                             ^
Path Splicing. Recalculating...
Trailing Path: 105 104 103 123 143 142 162

Path: 2 3 4 5 6 7 8 9 10 30 31 32 12 13 14 15 16 17 18 19 39 59 79 99 119 139 159 158 157 137 117 97 77 57 56 55 75 95 115 135 155 154 153 133 113 93 73 72 71 91 111 131 151 150 149 129 109 89 69 49 48 47 67 87 107 127 147 146 145 125 105 104 103 123 143 142 162
                                                                                                                                                                                                                                            ^
6. 
Map information from file
    Map Dimensions: (width height): 20 9
    Map Data File Name: maps\test_5.map
    Source Coordinates (x y): 1 0
    Target Coordinates (x y): 2 8
    Buffer Size: 45
    Enable Retry (1/0): 1

              0 1 2 3 4 5 6 7 8 9  0 1 2 3 4 5 6 7 8 9  
        ----+-------------------------------------------
         00 | 0 0 1 1 1 1 1 1 1 1  1 0 1 1 1 1 1 1 1 1
         20 | 1 0 0 0 0 0 0 0 0 0  1 1 1 0 0 0 0 0 0 1
         40 | 1 0 0 1 1 1 0 1 1 1  0 0 0 0 0 1 1 1 0 1
         60 | 0 0 0 1 0 1 0 1 0 1  0 1 1 1 0 1 0 1 0 1
         80 | 1 0 0 1 0 1 0 1 0 1  0 1 0 1 0 1 0 1 0 1
        100 | 1 0 0 1 0 1 0 1 0 1  0 1 0 1 0 1 0 1 0 1
        120 | 1 0 0 1 0 1 0 1 0 1  0 1 0 1 0 1 0 1 0 1
        140 | 1 0 1 1 0 1 1 1 0 1  1 1 0 1 1 1 0 1 1 1
        160 | 1 0 0 0 0 0 0 0 0 0  0 0 0 0 0 0 0 0 0 1 

Resizing Buffer 45 -> 83
Path: 2 3 4 5 6 7 8 9 10 30 31 32 12 13 14 15 16 17 18 19 39 59 79 99 119 139 159 158 157 137 117 97 77 57 56 55 75 95 115 135 155 154 153 133 113 93 73 72 71 91 111 131 151 150 149 129 109 89 69 49 48 47 67 87 107 127 147 146 145 125 105 85 65 45 44 43 63 83 103 123 143 142 162
