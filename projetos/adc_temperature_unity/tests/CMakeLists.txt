add_executable(temperature_tests
    test_temperature.c
    ../src/temperature.c
    unity/unity.c
)

target_include_directories(temperature_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/unity
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
)

# Para permitir testes nativos no PC
if(CMAKE_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    add_test(NAME temperature_tests COMMAND temperature_tests)
endif()