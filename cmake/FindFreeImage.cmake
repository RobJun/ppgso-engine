FIND_PATH(FreeImage_INCLUDE_DIR FreeImage/FreeImage.h)
message("FIND_PATH ${GLEW_INCLUDE_DIR}")

FIND_LIBRARY(FreeImage_LIBRARY NAMES FreeImage )
message("FIND_PATH ${FreeImage_LIBRARY}")
find_file(FreeImage_DLL_LIBRARY NAMES /vc2019/FreeImage.dll)
message("FIND_PATH ${FreeImage_DLL_LIBRARY}")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(FreeImage DEFAULT_MSG FreeImage_INCLUDE_DIR FreeImage_LIBRARY)