# #Generation-Assisted-With-GPT-6#
# #Generation-Assisted-with-OpenAI#
cmake_minimum_required(VERSION 3.16)
project(AI_Device_Bridge LANGUAGES CXX)

add_executable(aidevicebridge src/main.cpp)
target_compile_features(aidevicebridge PRIVATE cxx_std_20)

if(WIN32)
  target_link_libraries(aidevicebridge PRIVATE setupapi cfgmgr32)
endif()

if(MSVC)
  target_compile_options(aidevicebridge PRIVATE /W4)
else()
  target_compile_options(aidevicebridge PRIVATE -Wall -Wextra -Wpedantic)
endif()

install(TARGETS aidevicebridge RUNTIME DESTINATION bin)

enable_testing()
add_test(NAME startup_greeting COMMAND aidevicebridge --version)
set_tests_properties(startup_greeting PROPERTIES PASS_REGULAR_EXPRESSION "Greetings, and Welcome to AI-Device-Bridge\\.")
add_test(NAME release_version COMMAND aidevicebridge --version)
set_tests_properties(release_version PROPERTIES PASS_REGULAR_EXPRESSION "0002")
add_test(NAME security_record COMMAND aidevicebridge --security-version)
set_tests_properties(security_record PROPERTIES PASS_REGULAR_EXPRESSION "0000-0000-0000-0001")
add_test(NAME device_discovery COMMAND aidevicebridge --devices)
set_tests_properties(device_discovery PROPERTIES PASS_REGULAR_EXPRESSION "AI device discovery")
add_test(NAME diagnostics COMMAND aidevicebridge --diagnose)
set_tests_properties(diagnostics PROPERTIES PASS_REGULAR_EXPRESSION "Firmware health / inference")
add_test(NAME models COMMAND aidevicebridge --models)
set_tests_properties(models PROPERTIES PASS_REGULAR_EXPRESSION "HEF model discovery")

add_test(NAME diagnose_system COMMAND aidevicebridge --diagnose-system)
set_tests_properties(diagnose_system PROPERTIES PASS_REGULAR_EXPRESSION "System diagnostics")
add_test(NAME diagnose_device COMMAND aidevicebridge --diagnose-device)
set_tests_properties(diagnose_device PROPERTIES PASS_REGULAR_EXPRESSION "Device diagnostics")
add_test(NAME diagnose_driver COMMAND aidevicebridge --diagnose-driver)
set_tests_properties(diagnose_driver PROPERTIES PASS_REGULAR_EXPRESSION "Driver diagnostics")

add_test(NAME website_links COMMAND aidevicebridge --links)
set_tests_properties(website_links PROPERTIES PASS_REGULAR_EXPRESSION "Website Links")
add_test(NAME repair_guide COMMAND aidevicebridge --repair-guide)
set_tests_properties(repair_guide PROPERTIES PASS_REGULAR_EXPRESSION "no commands executed")

add_test(NAME developer_menu COMMAND aidevicebridge --help)
set_tests_properties(developer_menu PROPERTIES PASS_REGULAR_EXPRESSION "developer-menu")
