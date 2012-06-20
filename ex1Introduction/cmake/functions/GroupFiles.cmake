function(group_filesImpl prefix prefixG prefixD)
	file(GLOB elems RELATIVE ${prefix} ${prefixD}*)
	foreach(elem ${elems})
		set(fullelem ${prefix}/${elem})
		set(groupelem ${prefixG}${elem})
		set(direlem ${prefixD}${elem})
		if((IS_DIRECTORY ${fullelem}) AND (NOT(${elem} MATCHES "[._].*")))
			#message(STATUS "grouping ${groupelem}")
			source_group(${groupelem} REGULAR_EXPRESSION "${fullelem}/*")
			group_filesImpl(${fullelem} ${groupelem}\\ ${direlem}/)
		endif()
	endforeach()
endfunction(group_filesImpl)

function(group_files)
	group_filesImpl(${CMAKE_CURRENT_SOURCE_DIR} "" "")
endfunction(group_files)