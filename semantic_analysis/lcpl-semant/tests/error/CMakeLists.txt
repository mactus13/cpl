set(ERROR_TESTS
  arg_dup
  attr_dup
  attr_dup2
  assign_mismatch
  assign_mismatch2
  assign_mismatch3
  assign_mismatch4
  assign_mismatch5
  attr-miss
  attr-miss2
  attr-miss3
  call_invalid
  call_invalid2
  call_invalid3
  call_invalid4
  cast-invalid
  class_dup
  empty-miss
  local_out_scope2
  local_out_scope
  main-miss2
  main-miss
  met_dup2
  met_dup
  met_over2
  met_over3
  met_over4
  met_over
  new-invalid2
  new-invalid
  op_mismatch1
  op_mismatch2
  op_mismatch3
  op_mismatch4
  op_mismatch5
  param_mismatch2
  param_mismatch3
  param_mismatch4
  param_mismatch5
  param_mismatch6
  param_mismatch
  parent_int
  parent_loop
  parent_miss
  parent_same
  parent_string
  return_mismatch2
  return_mismatch3
  return_mismatch5
  return_mismatch6
  return_mismatch8
  return_mismatch
  type-miss2
  type-miss3
  type-miss
  while_invalid
  )

# Create an executable for each test that must catch an error
foreach(TEST ${ERROR_TESTS})
  add_executable(${TEST}
    ../TestRig.h
    ../TestRig.cpp
    ${TEST}TestRig.cpp
    )
  target_link_libraries(${TEST} ${LCPL_SEMANT})
  
  #testing
  add_custom_target(run-${TEST}
    COMMAND ${TEST} ${TEST}.lcpl.json
    DEPENDS ${TEST}
   )
  add_dependencies(run-all run-${TEST})
  
  
endforeach()

