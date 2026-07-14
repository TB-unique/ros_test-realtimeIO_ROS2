# generated from ament/cmake/core/templates/nameConfig.cmake.in

# prevent multiple inclusion
if(_low_prior_CONFIG_INCLUDED)
  # ensure to keep the found flag the same
  if(NOT DEFINED low_prior_FOUND)
    # explicitly set it to FALSE, otherwise CMake will set it to TRUE
    set(low_prior_FOUND FALSE)
  elseif(NOT low_prior_FOUND)
    # use separate condition to avoid uninitialized variable warning
    set(low_prior_FOUND FALSE)
  endif()
  return()
endif()
set(_low_prior_CONFIG_INCLUDED TRUE)

# output package information
if(NOT low_prior_FIND_QUIETLY)
  message(STATUS "Found low_prior: 0.0.0 (${low_prior_DIR})")
endif()

# warn when using a deprecated package
if(NOT "" STREQUAL "")
  set(_msg "Package 'low_prior' is deprecated")
  # append custom deprecation text if available
  if(NOT "" STREQUAL "TRUE")
    set(_msg "${_msg} ()")
  endif()
  # optionally quiet the deprecation message
  if(NOT ${low_prior_DEPRECATED_QUIET})
    message(DEPRECATION "${_msg}")
  endif()
endif()

# flag package as ament-based to distinguish it after being find_package()-ed
set(low_prior_FOUND_AMENT_PACKAGE TRUE)

# include all config extra files
set(_extras "")
foreach(_extra ${_extras})
  include("${low_prior_DIR}/${_extra}")
endforeach()
