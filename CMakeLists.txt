cmake_minimum_required(VERSION 3.25)
project(toy_assembler)

set(CMAKE_CXX_STANDARD 17)

add_executable(toy_assembler src/asm_main.cpp src/Assembler.cpp src/Assembler.h src/TokenFunctions.h src/TokenFunctions.cpp)

add_executable(toy_simulator src/sim_main.cpp src/Simulator.cpp src/Simulator.h)

Include(FetchContent)

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.0.1 # or a later release
)

FetchContent_MakeAvailable(Catch2)

add_executable(tests test/asm_tests.cpp test/sim_tests.cpp test/token_tests.cpp src/Simulator.h src/Simulator.cpp src/Assembler.cpp src/Assembler.h  src/TokenFunctions.h src/TokenFunctions.cpp)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)