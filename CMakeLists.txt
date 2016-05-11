project (FDim)
cmake_minimum_required (VERSION 2.8)

set(fdim_src 
  ./src/fdim.cc 
  ./src/FdimFrame.cc 
  ./src/BitmapFractal.cc 
  ./src/BitmapView.cc 
  ./src/Line.cc
  ./src/LineView.cc )
  
set(fdim_h
  ./src/utils.h
  ./src/FdimFrame.h
  ./src/BitmapFractal.h
  ./src/BitmapView.h
  ./src/Line.h
  ./src/LineView.h )
  
# wxWidgets
find_package(wxWidgets REQUIRED gl core base adv)
include(${wxWidgets_USE_FILE})

IF(NOT APPLE)
  find_package(OpenMP)
  if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
  endif()
ENDIF()

# fdim target
add_executable( fdim ${fdim_src} ${fdim_h})
target_link_libraries(fdim ${wxWidgets_LIBRARIES}  )

if (APPLE)
# postprocessing (strip, create app folder on mac etc)
  set(APP_NAME "FDim")
  set(APP_VERSION "1.1")

  add_custom_target(app ALL
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${APP_NAME}.app
    COMMAND ${CMAKE_COMMAND} -E make_directory ${APP_NAME}.app/Contents/MacOS
    COMMAND ${CMAKE_COMMAND} -E make_directory ${APP_NAME}.app/Contents/Resources/English.lproj
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/src/Info.plist" ${APP_NAME}.app/Contents/Info.plist
    COMMAND ${CMAKE_COMMAND} -E copy fdim ${APP_NAME}.app/Contents/MacOS/FDim
    COMMAND ${CMAKE_COMMAND} -E echo "APPLFDIM" > ${APP_NAME}.app/Contents/PkgInfo
    COMMAND strip ${APP_NAME}.app/Contents/MacOS/${APP_NAME}
    COMMAND iconutil -c icns ${CMAKE_CURRENT_SOURCE_DIR}/src/icon.iconset -o ${APP_NAME}.app/Contents/Resources/icon.icns
    COMMAND codesign -s - -f --entitlements ${CMAKE_CURRENT_SOURCE_DIR}/src/entitlement.plist ${APP_NAME}.app/
    DEPENDS fdim 
    COMMENT "Creating ${APP_NAME}.app"
    SOURCES ./src/Info.plist ./src/entitlement.plist ./src/icon.iconset
  )
   
	# Make clean should remove the app also
	set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${APP_NAME}.app")

	# compress with UPX if found
   INCLUDE(${CMAKE_ROOT}/Modules/FindSelfPackers.cmake)
   IF (SELF_PACKER_FOR_EXECUTABLE)
      ADD_CUSTOM_COMMAND(
           TARGET app POST_BUILD
           COMMAND ${SELF_PACKER_FOR_EXECUTABLE}
           ARGS ${SELF_PACKER_FOR_EXECUTABLE_FLAGS}
                ${APP_NAME}.app/Contents/MacOS/${APP_NAME}
          )
   ENDIF (SELF_PACKER_FOR_EXECUTABLE)

	# create target for packaging
	add_custom_target(dmg
    COMMAND ${CMAKE_COMMAND} -E remove "${APP_NAME}-${APP_VERSION}.dmg"
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/package.sh" ./package.sh
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/src/Background_simple.png" ./Background.png
    COMMAND ./package.sh ${APP_NAME} ${APP_VERSION} Background.png
    COMMAND ${CMAKE_COMMAND} -E remove package.sh
    COMMAND ${CMAKE_COMMAND} -E remove Background.png
    DEPENDS app
    COMMENT "Packaging ${APP_NAME}-${APP_VERSION}.dmg"
    SOURCES ./src/Background_simple.png ./package.sh
	)
	# Make clean should remove the app also
	set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${APP_NAME}-${APP_VERSION}.dmg")

endif (APPLE)
