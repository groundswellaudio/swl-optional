cmake_minimum_required(VERSION 3.20)

project(swl.optional 
	VERSION 1.0.0 
	LANGUAGES CXX
	HOMEPAGE_URL https://github.com/groundswellaudio/swl-optional)

add_library(swl-optional INTERFACE)

target_include_directories(swl-optional INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:./include>)

target_compile_features(swl-optional INTERFACE cxx_std_20) 



if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
	
	include(CTest)

	file(GLOB test-sources CONFIGURE_DEPENDS tests/*.cpp)
	
	foreach (source IN LISTS test-sources)
		get_filename_component(name "${source}" NAME_WE)
		set(test "${PROJECT_NAME}-test-${name}")
		add_executable(${test} "${source}")
		target_link_libraries(${test} swl-optional)
		add_test(NAME ${PROJECT_NAME}::test::${name} COMMAND ${test})
	endforeach()
	
endif()

