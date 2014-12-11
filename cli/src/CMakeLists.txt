
set(EGGBEATER_SOURCES
"${CMAKE_SOURCE_DIR}/src/Control"
"${CMAKE_SOURCE_DIR}/src/Crypto"
"${CMAKE_SOURCE_DIR}/src/Discovery"
"${CMAKE_SOURCE_DIR}/src/getTempPath"
"${CMAKE_SOURCE_DIR}/src/main"
"${CMAKE_SOURCE_DIR}/src/Options"
"${CMAKE_SOURCE_DIR}/src/packet"
"${CMAKE_SOURCE_DIR}/src/SerialCommunication"
)

add_library(lib_EggBeater "${CMAKE_SOURCE_DIR}/src/main")

set_property(TARGET lib_EggBeater PROPERTY COMPILE_DEFINITIONS "_EGGBEATER_DEBUG_=1")
target_link_libraries(lib_EggBeater CryptoPP)

add_executable(eggbeater ${EGGBEATER_SOURCES})

target_link_libraries(eggbeater lib_EggBeater)

set_property(TARGET eggbeater PROPERTY COMPILE_DEFINITIONS "_EGGBEATER_DEBUG_=1")
