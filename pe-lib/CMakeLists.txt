set(SOURCE_FILES
        pe-maths/gauss-sum.cpp
        )

set(HEADER_FILES
        pe-maths/gauss-sum.h
        )

add_library(pe-lib ${SOURCE_FILES} ${HEADER_FILES})
target_compile_definitions(pe-lib PRIVATE DOCTEST_CONFIG_DISABLE)

file(GLOB subdirs ${CMAKE_CURRENT_SOURCE_DIR}/*)
foreach(subdir ${subdirs})
    if(IS_DIRECTORY ${subdir})
        cmake_path(GET subdir FILENAME sub)
        set(matches "")
        foreach(src ${SOURCE_FILES})
            if(src MATCHES "^(${sub})/")
                list(APPEND matches ${src})
            endif()
        endforeach()
        string(CONCAT exe_name ${sub} "-test")
        add_executable(${exe_name} ${matches} ../doctest/main.cpp)
    endif()
endforeach()