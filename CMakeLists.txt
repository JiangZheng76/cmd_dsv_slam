cmake_minimum_required(VERSION 2.8.3)
project(direct_stereo_slam)

# set(BUILD_TYPE RelWithDebInfo)
set(CMAKE_BUILD_TYPE RelWithDebInfo)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-march=native \
-Wno-deprecated-declarations \
-Wno-deprecated \
-Wno-ignored-qualifiers \
-Wno-reorder \
-Wno-missing-braces \
-fPIC")

find_package(OpenCV REQUIRED)
# find_package(Eigen3 REQUIRED)
find_package(PCL 1.0 REQUIRED)
find_package(g2o REQUIRED )
find_package(fmt REQUIRED)
find_package(yaml-cpp REQUIRED)

find_package(catkin_simple REQUIRED)
catkin_simple(ALL_DEPS_REQUIRED)

# catkin_package()
set(EIGEN3_INCLUDE_DIRS /home/sysu/eigen-3.3.7)
include_directories(
  ${PROJECT_SOURCE_DIR}
  ${Pangolin_INCLUDE_DIRS}
  ${EIGEN3_INCLUDE_DIRS}
  ${PCL_INCLUDE_DIRS}
  ${catkin_INCLUDE_DIRS}
)

link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})

if (NOT DSO_PATH)
    set(DSO_PATH ${CMAKE_SOURCE_DIR}/thrid-party/dso)
endif()
set(DSO_INCLUDE_DIRS "${DSO_PATH}/src" "${DSO_PATH}/thirdparty/Sophus")
set(DSO_LIBRARY_PATH "${DSO_PATH}/build/lib")
find_library(DSO_LIBRARY dso ${DSO_LIBRARY_PATH})
message("-- Found DSO library at \"${DSO_LIBRARY}\"")

find_package(Boost COMPONENTS system thread)
find_package(Pangolin REQUIRED)

set(DSO_HELPERS_SOURCE_FILES
  src/dso_helpers/FrontEndDebugStuff.cpp
  src/dso_helpers/FrontEndMarginalize.cpp
  src/dso_helpers/FrontEndOptimize.cpp
  src/dso_helpers/FrontEndOptPoint.cpp
)

include_directories(${DSO_INCLUDE_DIRS} src)

cs_add_executable(
  slam_node 
  src/main.cpp 
  src/FrontEnd.cpp
  ${DSO_HELPERS_SOURCE_FILES} 
  src/scale_optimization/TrackerAndScaler.cpp 
  src/loop_closure/LoopHandler.cpp 
  src/loop_closure/pangolin_viewer/PangolinLoopViewer.cpp 
  src/loop_closure/pangolin_viewer/KeyFrameDisplay.cpp 
  src/loop_closure/loop_detection/ScanContext.cpp 
  src/loop_closure/pose_estimation/PoseEstimator.cpp
  # CMD_MODE
  src/Comm/vo_comm.cpp
  )

# message(${catkin_LIBRARIES})
target_link_libraries(slam_node
    ${catkin_LIBRARIES}
    fmt::fmt # fmt需要在 yaml 前面，不然会出现链接顺序错误
    ${DSO_LIBRARY}
    ${Pangolin_LIBRARIES}
    ${OpenCV_LIBS}
    ${PCL_LIBRARIES}
    g2o_core g2o_types_slam3d g2o_solver_eigen g2o_stuff
    
    yaml-cpp)
