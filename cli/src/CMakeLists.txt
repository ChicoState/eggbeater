
message(STATUS "System name: '${CMAKE_SYSTEM_NAME}'\nSystem version: '${CMAKE_SYSTEM_VERSION}'")

set(EGGBEATER_SOURCES
  "${CMAKE_SOURCE_DIR}/src/Control"
  "${CMAKE_SOURCE_DIR}/src/Crypto"
  "${CMAKE_SOURCE_DIR}/src/Discovery"
  "${CMAKE_SOURCE_DIR}/src/getTempPath"
  "${CMAKE_SOURCE_DIR}/src/Options"
  "${CMAKE_SOURCE_DIR}/src/packet"
  "${CMAKE_SOURCE_DIR}/src/os/${EGGBEATER_OS}/SerialCommunication"
  "${CMAKE_SOURCE_DIR}/src/os/${EGGBEATER_OS}/Serial"
  #"${CMAKE_SOURCE_DIR}/src/SerialCommunication"
)

set(LIBSERIAL_SOURCES
  "${EGGBEATER_LIBSERIAL_PATH}/serialport"
)

include_directories(${EGGBEATER_LIBSERIAL_PATH})

add_library(lib_EggBeater ${EGGBEATER_SOURCES})

set_property(TARGET lib_EggBeater PROPERTY COMPILE_DEFINITIONS "_EGGBEATER_DEBUG_=1")
target_link_libraries(lib_EggBeater CryptoPP)

add_executable(eggbeater "${CMAKE_SOURCE_DIR}/src/main")

target_link_libraries(eggbeater lib_EggBeater)

set_property(TARGET eggbeater PROPERTY COMPILE_DEFINITIONS "_EGGBEATER_DEBUG_=0" "SERIAL_DEFAULT_OVERLAPPED")
