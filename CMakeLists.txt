cmake_minimum_required(VERSION 3.7)
project(ex3_simulator)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        simulator/asmparser.cpp
        simulator/asmparser.h
        simulator/cpu.cpp
        simulator/cpu.h
        simulator/debugger.cpp
        simulator/debugger.h
        simulator/defs.h
        simulator/ex3_asm.cpp
        simulator/ex3_asm_def.h
        simulator/ex3_asmparser.cpp
        simulator/ex3_asmparser.h
        simulator/ex3_cpu.cpp
        simulator/ex3_cpu.h
        simulator/ex3_insnset.cpp
        simulator/ex3_insnset.h
        simulator/ex3_memory.cpp
        simulator/ex3_memory.h
        simulator/ex3_terminalsystem.cpp
        simulator/ex3_terminalsystem.h
        simulator/insnset.cpp
        simulator/insnset.h
        simulator/label.cpp
        simulator/label.h
        simulator/memory.cpp
        simulator/memory.h
        simulator/system.cpp
        simulator/system.h
        simulator/utils.h)

add_executable(ex3_simulator ${SOURCE_FILES})