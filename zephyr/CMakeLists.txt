####
# This sets up the build system for the 'KLEO' project, including
# components and deployments from project.cmake. In addition, it imports the core F Prime components.
####

cmake_minimum_required(VERSION 3.24.2)

# Enable Zephyr when not performing a unit testing build
if (NOT BUILD_TESTING)
    find_package(Zephyr HINTS "${CMAKE_CURRENT_LIST_DIR}/lib/zephyr-workspace")
endif()

project(KLEO C CXX)

###
# F' Core Setup
# This includes all of the F prime core components, and imports the make-system.
###
include("${CMAKE_CURRENT_LIST_DIR}/lib/fprime/cmake/FPrime.cmake")
# NOTE: register custom targets between these two lines
fprime_setup_included_code()

# This includes project-wide objects
add_fprime_subdirectory("${CMAKE_CURRENT_LIST_DIR}/config")
add_fprime_subdirectory("${CMAKE_CURRENT_LIST_DIR}/KLEO")
add_fprime_subdirectory("${CMAKE_CURRENT_LIST_DIR}/KLEODeployment/")
