find_package(Doxygen REQUIRED)

# Find all MyGL header files
file(GLOB_RECURSE MYGL_HEADER_FILES ${MYGL_INCLUDE_DIR}/MyGL/*.hpp)

set(DOXYGEN_INPUT_DIR ${PROJECT_SOURCE_DIR}/include/MyGL)
set(DOXYGEN_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR})
set(DOXYGEN_INDEX_FILE ${DOXYGEN_OUTPUT_DIR}/html/index.html)
set(DOXYFILE_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
set(DOXYFILE_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
set(DOXYGEN_AWESOME_DIR ${CMAKE_CURRENT_SOURCE_DIR}/doxygen-awesome)

# Create the variables holding the names of the html,css,js and md files used with Doxygen
set(MARKDOWN_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/index.md ${CMAKE_CURRENT_SOURCE_DIR}/install.md ${CMAKE_CURRENT_SOURCE_DIR}/usage.md)
set(STYLESHEETS ${DOXYGEN_AWESOME_DIR}/doxygen-awesome.css ${DOXYGEN_AWESOME_DIR}/custom-style.css)
set(EXTRA_FILES ${DOXYGEN_AWESOME_DIR}/doxygen-awesome-darkmode-toggle.js ${DOXYGEN_AWESOME_DIR}/footer.js
                ${DOXYGEN_AWESOME_DIR}/img/dark_mode_black_24dp.svg ${DOXYGEN_AWESOME_DIR}/img/light_mode_white_24dp.svg
)

string(REPLACE ";" " " MARKDOWN_SOURCES "${MARKDOWN_SOURCE_FILES}")
string(REPLACE ";" " " DOXYGEN_CUSTOM_CSS "${STYLESHEETS}")
string(REPLACE ";" " " DOXYGEN_EXTRA_FILES "${EXTRA_FILES}")
set(DOXYGEN_HTML_HEADER ${DOXYGEN_AWESOME_DIR}/header.html)
set(DOXYGEN_HTML_FOOTER ${DOXYGEN_AWESOME_DIR}/footer.html)

# Set the variables in Doxyfile.in
configure_file(${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY)

# Add a command to generate the doxygen html files
add_custom_command(
  OUTPUT ${DOXYGEN_INDEX_FILE}
  DEPENDS ${MYGL_HEADER_FILES} ${MARKDOWN_SOURCE_FILES} ${DOXYGEN_HTML_HEADER} ${DOXYGEN_HTML_FOOTER} ${STYLESHEETS} ${EXTRA_FILES}
  COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_OUT}
  MAIN_DEPENDENCY ${DOXYFILE_OUT}
  COMMENT "Generating docs"
)

# Add a build target for Doxygen
add_custom_target(Doxygen ALL DEPENDS ${DOXYGEN_INDEX_FILE})

# Documentation install target
include(GNUInstallDirs)
install(DIRECTORY ${DOXYGEN_OUTPUT_DIR}/html DESTINATION ${CMAKE_INSTALL_DOCDIR})
