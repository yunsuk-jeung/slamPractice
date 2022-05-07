#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "flann::flann_cpp" for configuration "Debug"
set_property(TARGET flann::flann_cpp APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(flann::flann_cpp PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/flann_cppd.lib"
  IMPORTED_LINK_INTERFACE_LIBRARIES_DEBUG "-Wl,--push-state,--no-as-needed;-Wl,--pop-state"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/flann_cppd.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS flann::flann_cpp )
list(APPEND _IMPORT_CHECK_FILES_FOR_flann::flann_cpp "${_IMPORT_PREFIX}/lib/flann_cppd.lib" "${_IMPORT_PREFIX}/bin/flann_cppd.dll" )

# Import target "flann::flann_cpp_s" for configuration "Debug"
set_property(TARGET flann::flann_cpp_s APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(flann::flann_cpp_s PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/flann_cpp_sd.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS flann::flann_cpp_s )
list(APPEND _IMPORT_CHECK_FILES_FOR_flann::flann_cpp_s "${_IMPORT_PREFIX}/lib/flann_cpp_sd.lib" )

# Import target "flann::flann" for configuration "Debug"
set_property(TARGET flann::flann APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(flann::flann PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/flannd.lib"
  IMPORTED_LINK_INTERFACE_LIBRARIES_DEBUG "-Wl,--push-state,--no-as-needed;-Wl,--pop-state"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/flannd.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS flann::flann )
list(APPEND _IMPORT_CHECK_FILES_FOR_flann::flann "${_IMPORT_PREFIX}/lib/flannd.lib" "${_IMPORT_PREFIX}/bin/flannd.dll" )

# Import target "flann::flann_s" for configuration "Debug"
set_property(TARGET flann::flann_s APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(flann::flann_s PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "CXX"
  IMPORTED_LINK_INTERFACE_LIBRARIES_DEBUG "-Wl,--push-state,--no-as-needed;-Wl,--pop-state"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/flann_sd.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS flann::flann_s )
list(APPEND _IMPORT_CHECK_FILES_FOR_flann::flann_s "${_IMPORT_PREFIX}/lib/flann_sd.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
