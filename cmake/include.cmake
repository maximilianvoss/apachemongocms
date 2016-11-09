cmake_minimum_required(VERSION 3.0)

find_package(APR REQUIRED)
find_package(APACHE REQUIRED)
find_package(BSON REQUIRED)
find_package(MONGOC REQUIRED)
find_package(IMAGEMAGICK REQUIRED)

include_directories(${APR_INCLUDE_DIR})
include_directories(${APACHE_INCLUDE_DIR})
include_directories(${BSON_INCLUDE_DIR})
include_directories(${MONGOC_INCLUDE_DIR})
include_directories(${IMAGEMAGICK_INCLUDE_DIR})
include_directories(${IMAGEMAGICK_INCLUDE_DIR_CONFIG})