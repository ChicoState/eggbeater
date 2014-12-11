
set(EGGBEATER_SOURCES
"${CMAKE_SOURCE_DIR}/src/Control"
"${CMAKE_SOURCE_DIR}/src/Crypto"
"${CMAKE_SOURCE_DIR}/src/Discovery"
"${CMAKE_SOURCE_DIR}/src/getTempPath"
"${CMAKE_SOURCE_DIR}/src/Options"
"${CMAKE_SOURCE_DIR}/src/packet"
"${CMAKE_SOURCE_DIR}/src/SerialCommunication"
)

set(GTEST_DIR "${EGGBEATER_GTEST_PATH}/fused-src")
set(GTEST_SOURCE "${GTEST_DIR}/gtest/gtest-all")

add_library(GoogleTest ${GTEST_SOURCE})

set(COMMON_SRCS ${EGGBEATER_SOURCES})

include_directories("${GTEST_DIR}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/unit-tests)

add_executable(Crypto_CipherMode_tests Crypto_CipherMode_tests ${COMMON_SRCS})
add_executable(Crypto_tests Crypto_tests ${COMMON_SRCS})
add_executable(CryptoPP_tests CryptoPP_tests ${COMMON_SRCS})
add_executable(Discovery_test_absent Discovery_test_absent ${COMMON_SRCS})
add_executable(Discovery_test_present Discovery_test_present ${COMMON_SRCS})
add_executable(Discovery_tests Discovery_tests ${COMMON_SRCS})
add_executable(Options_tests Options_tests ${COMMON_SRCS})
add_executable(Packet_tests Packet_tests ${COMMON_SRCS})
add_executable(SerialComm_test_port SerialComm_test_port ${COMMON_SRCS})
add_executable(SerialComm_tests SerialComm_tests ${COMMON_SRCS})

target_link_libraries(Crypto_CipherMode_tests CryptoPP GoogleTest lib_EggBeater)
target_link_libraries(Crypto_tests CryptoPP GoogleTest lib_EggBeater)
target_link_libraries(CryptoPP_tests CryptoPP GoogleTest lib_EggBeater)
target_link_libraries(Discovery_test_absent CryptoPP GoogleTest lib_EggBeater)
target_link_libraries(Discovery_test_present CryptoPP GoogleTest lib_EggBeater)
target_link_libraries(Discovery_tests CryptoPP GoogleTest lib_EggBeater)
target_link_libraries(Options_tests CryptoPP GoogleTest lib_EggBeater)
target_link_libraries(Packet_tests CryptoPP GoogleTest lib_EggBeater)
target_link_libraries(SerialComm_test_port CryptoPP GoogleTest lib_EggBeater)
target_link_libraries(SerialComm_tests CryptoPP GoogleTest lib_EggBeater)