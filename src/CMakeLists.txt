file(GLOB SOURCES "*.c")

add_library(cs3642-assignment-two SHARED ${SOURCES})
set_target_properties(cs3642-assignment-two PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")