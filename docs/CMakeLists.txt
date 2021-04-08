find_package(Doxygen REQUIRED)

# Find all MyGL header files
file(GLOB_RECURSE MYGL_HEADER_FILES ${INCLUDE_DIR}/MyGL/*.hpp)

set(DOXYGEN_INPUT_DIR ${PROJECT_SOURCE_DIR}/include/MyGL)
set(DOXYGEN_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR})
set(DOXYGEN_INDEX_FILE ${DOXYGEN_OUTPUT_DIR}/html/index.html)
set(DOXYFILE_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
set(DOXYFILE_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

# Create the variable holding the names of the css used with Doxygen
set(STYLESHEETS
  ${CMAKE_CURRENT_SOURCE_DIR}/doxygen-awesome.css
  ${CMAKE_CURRENT_SOURCE_DIR}/custom-style.css
)
foreach(FILE IN LISTS STYLESHEETS)
  string(APPEND DOXYGEN_CUSTOM_CSS "${FILE} ")
endforeach()

# Create the variable with the names of the markdown files included in
# the documentation generation
set(MARKDOWN_SOURCE_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/index.md
  ${CMAKE_CURRENT_SOURCE_DIR}/install.md
  ${CMAKE_CURRENT_SOURCE_DIR}/usage.md
)
foreach(FILE IN LISTS MARKDOWN_SOURCE_FILES)
  string(APPEND MARKDOWN_SOURCES "${FILE} ")
endforeach()


# Set the variables in Doxyfile.in
configure_file(${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY)

# Create the output directory
file(MAKE_DIRECTORY ${DOXYGEN_OUTPUT_DIR})

# Add a command to generate the doxygen html files, they are
# rebuilt when any of the header files changes 
add_custom_command(
  OUTPUT ${DOXYGEN_INDEX_FILE}
  DEPENDS ${MYGL_HEADER_FILES} ${MARKDOWN_SOURCE_FILES}
  COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_OUT}
  MAIN_DEPENDENCY ${DOXYFILE_OUT} ${DOXYFILE_IN}
  COMMENT "Generating docs"
)

# Add a build target for Doxygen, rebuilt only when
# the doxygen index file changes
add_custom_target(Doxygen ALL DEPENDS ${DOXYGEN_INDEX_FILE})

# Documentation install target
include(GNUInstallDirs)
install(DIRECTORY ${DOXYGEN_OUTPUT_DIR} DESTINATION ${CMAKE_INSTALL_DOCDIR})