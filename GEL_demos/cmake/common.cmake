################################################################################
# Functions
################################################################################
function(directory_source_group GROUP DIRECTORY EXTENSION)
	file(GLOB FILES "${DIRECTORY}/*.${EXTENSION}")
	source_group("${GROUP}" FILES ${FILES})

	file(GLOB SUBDIRECTORIES ${DIRECTORY}/*)
	foreach(SUBDIRECTORY ${SUBDIRECTORIES})
		if(IS_DIRECTORY ${SUBDIRECTORY})
			# Advance recursion
			string(REGEX MATCH "[a-zA-Z0-9]*$" DIRECTORY_NAME ${SUBDIRECTORY})
			set(GROUP "${GROUP}\\${DIRECTORY_NAME}")
		
			directory_source_group(${GROUP} ${SUBDIRECTORY} ${EXTENSION})

			# Regress recursion
			string(REGEX REPLACE "\\\\[a-zA-Z0-9]*$" "" GROUP ${GROUP})
		endif()
	endforeach()
endfunction()