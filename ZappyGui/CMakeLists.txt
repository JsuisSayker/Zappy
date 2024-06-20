cmake_minimum_required(VERSION 3.10)

project(
  ZappyGui
  VERSION 0.1
  DESCRIPTION "A simple zappy gui"
  LANGUAGES CXX)

########## DOXYGEN ###########

# first we can indicate the documentation build as an option and set it to ON by
# default
option(BUILD_DOC "Build documentation" ON)

# check if Doxygen is installed
find_package(Doxygen)
if(DOXYGEN_FOUND)
  # set input and output files
  set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
  set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

  # request to configure the file
  configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
  message("Doxygen build started")

  # note the option ALL which allows to build the docs together with the
  # application
  add_custom_target(
    doc_doxygen ALL
    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
    WORKING_DIRECTORY ${MAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating API documentation with Doxygen"
    VERBATIM)

  # Conditionally compile LaTeX to PDF and copy it
  # Ensure Doxygen and LaTeX are available
  find_package(LATEX) # This check is optional and serves to ensure Latex is
                      # installed
  if(LATEX_FOUND)
    add_custom_command(
      TARGET doc_doxygen
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E make_directory
              ${CMAKE_CURRENT_SOURCE_DIR}/doc_doxygen/latex
      COMMAND ${CMAKE_COMMAND} -E chdir
              ${CMAKE_CURRENT_SOURCE_DIR}/doc_doxygen/latex make
      COMMAND
        ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/doc_doxygen/latex/refman.pdf
        ${CMAKE_CURRENT_SOURCE_DIR}/doc/Documentation.pdf
      COMMENT
        "Building PDF documentation with LaTeX and copying to doc directory")
  else()
    message("LaTeX not found, PDF documentation will not be generated.")
  endif()

else(DOXYGEN_FOUND)
  message("Doxygen need to be installed to generate the doxygen documentation")
endif(DOXYGEN_FOUND)