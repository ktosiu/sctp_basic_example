#
# CMakeLists.txt
#
#  Created on: 06/giu/2014
#      Author: Luca Muratore (luca.muratore@iit.it)
#

# show warnings
if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  # Update if necessary
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")
endif()

project(sctp_basic_example)

# minimum cmake version required
cmake_minimum_required(VERSION 2.8.7)

# set up our program
add_executable(sctp_server sctp_server.c)
add_executable(sctp_client sctp_client.c)

#link lib
target_link_libraries(sctp_server sctp)
target_link_libraries(sctp_client sctp)

