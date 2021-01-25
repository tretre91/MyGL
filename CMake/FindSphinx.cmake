#Look for an executable called sphinx-build
find_program(
	SPHINX_EXECUTABLE
	NAMES sphinx-build
	DOC "find_path add_custom_command sphinx-build executable"
)

include(FindPackageHandleStandardArgs)

#Handle standard arguments to find_package like REQUIRED and QUIET
find_package_handle_standard_args(
	Sphinx
	"Failed to find sphinx-build executable"
	SPHINX_EXECUTABLE
)