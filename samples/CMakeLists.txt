cmake_minimum_required(VERSION 3.1)
project(criterion_samples)

set(CMAKE_C_FLAGS "-std=c99 -Wall -Wextra -pedantic")

include_directories(../include)

set(SAMPLES
  signal
  report
  suites
  fixtures
  asserts
  more-suites
  long-messages
  description
  other-crashes
  simple
)

foreach(sample ${SAMPLES})
    add_executable(${sample} ${sample}.c)
    target_link_libraries(${sample} criterion)
endforeach()
