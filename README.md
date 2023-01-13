# Project Euler __ C++

A C++ solution set for the first 100 problems corresponding to the [Project Euler](https://projecteuler.net/archives) 
challenges.

10/100 solved: :zap: :zap: :black_circle: :black_circle: :black_circle: :black_circle: :black_circle: 
:black_circle: :black_circle: :black_circle:

Problem content on the Project Euler site is licensed under [CC BY-NC-SA 4.0](https://projecteuler.net/copyright).

Some problems have been altered to either generalise their goal or implement further constraints, as influenced by the 
problem outlines found on [HackerRank](https://www.hackerrank.com/contests/projecteuler/challenges).

## :open_file_folder: Structure Guideline

Inside the **pe-solutions folder**, problem solutions are separated into batches with 10 solutions per sub-folder.

[The doctest framework](https://github.com/doctest/doctest) is used for testing, so test cases for each problem 
are located at the bottom of each solution's source file.

If a function is used in multiple solutions, it is elevated out of its original source file and placed in the 
**pe-lib folder**. This library contains custom classes and helper functions (as well as their unit tests) for 
combinatorics, mathematics, search algorithms, string processing, and test automation.

## :handshake: Sibling Repositories

The original solution set was written in Kotlin :desert_island:
- [Project Euler __ Kotlin](https://github.com/bog-walk/project-euler-kotlin)

Another project repository has been created to practise Python :snake:
- [Project Euler __ Python](https://github.com/bog-walk/project-euler-python)