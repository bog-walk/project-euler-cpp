set(SOURCE_FILES
        pe-combinatorics/binomial-coeff.cpp
        pe-custom/big-int.cpp
        pe-custom/extension.cpp
        pe-custom/pyramid-tree.cpp
        pe-custom/rolling-queue.cpp
        pe-maths/factorial.cpp
        pe-maths/gauss-sum.cpp
        pe-maths/is-prime.cpp
        pe-maths/prime-factors.cpp
        pe-maths/primes.cpp
        pe-maths/pythagorean.cpp
        pe-strings/palindrome.cpp
        pe-strings/utility.cpp
        pe-tests/get-test-resource.cpp
        )

set(HEADER_FILES
        pe-combinatorics/binomial-coeff.h
        pe-custom/big-int.h
        pe-custom/extension.h
        pe-custom/pyramid-tree.h
        pe-custom/rolling-queue.h
        pe-maths/factorial.h
        pe-maths/gauss-sum.h
        pe-maths/is-prime.h
        pe-maths/prime-factors.h
        pe-maths/primes.h
        pe-maths/pythagorean.cpp
        pe-strings/palindrome.h
        pe-strings/utility.h
        pe-tests/get-test-resource.h
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