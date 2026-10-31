cmake_minimum_required(VERSION 3.16)

project(AI_Device_Bridge VERSION 0.1.0 LANGUAGES CXX)

add_executable(aidevicebridge src/main.cpp)
target_compile_features(aidevicebridge PRIVATE cxx_std_20)

install(TARGETS aidevicebridge RUNTIME DESTINATION bin)

enable_testing()
add_test(NAME startup_greeting COMMAND aidevicebridge)
set_tests_properties(startup_greeting PROPERTIES
    PASS_REGULAR_EXPRESSION "Greetings, and Welcome to AI-Device-Bridge\\."
)
