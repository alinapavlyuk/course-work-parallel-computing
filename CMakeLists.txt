cmake_minimum_required(VERSION 3.27)
project(course_work)

set(CMAKE_CXX_STANDARD 17)

add_executable(course_work main.cpp
        src/ThreadPool.cpp
        src/ThreadPool.h
        src/TaskQueue.h
        src/TaskQueue.cpp
        src/Server.cpp
        src/Server.h
        src/Invertedndex.cpp
        src/Invertedndex.h
        src/FileManager.cpp
        src/FileManager.h)
