file(GLOB_RECURSE MIDEA_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

idf_component_register(SRCS ${MIDEA_SOURCES}
                       INCLUDE_DIRS include include/Appliance include/Appliance/AirConditioner include/Frame include/Helpers
                       REQUIRES arduino)
