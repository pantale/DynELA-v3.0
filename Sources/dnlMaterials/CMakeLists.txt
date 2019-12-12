FILE(GLOB SRCS *.h *.C)

ADD_LIBRARY(dnlMaterials SHARED ${SRCS})
TARGET_LINK_LIBRARIES(dnlMaterials dnlMaths dnlKernel lapacke lapack blas)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

# Include of previous directories
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/dnlKernel)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/dnlMaths)

SOURCE_GROUP(base             REGULAR_EXPRESSION ".*\\.(C|i|h)")
SOURCE_GROUP(generated        REGULAR_EXPRESSION ".*\\.(cxx|py)")
