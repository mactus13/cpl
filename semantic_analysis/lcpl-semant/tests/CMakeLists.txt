include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(TESTRIG_FILES TestRig.h TestRig.cpp)

# Create one executable for all error-free tests
add_executable(test-error-free
  TestRig.h
  TestRig.cpp
  ErrorFreeTestRig.cpp
  )
target_link_libraries(test-error-free ${LCPL_SEMANT})

# Testing
add_custom_target(run-all)

file(GLOB ERROR_FREE_JSONS "*.json" "simple/*.json" "advanced/*.json" "complex/*.json")
foreach(JSON ${ERROR_FREE_JSONS})
  string(REGEX REPLACE ".*/([^/]+)\\.lcpl\\.json" "\\1" JSON_NAME ${JSON})   
  add_custom_target(run-${JSON_NAME}
    COMMAND test-error-free ${JSON}
    DEPENDS test-error-free
   )
  add_dependencies(run-all run-${JSON_NAME})
endforeach()



add_subdirectory(error)

