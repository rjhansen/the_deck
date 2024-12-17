set(LIBDECKY_SRC card.cpp deck.cpp)
set(APPDECKY_SRC decky.cpp)

add_library(decky STATIC)

target_sources(decky
    PUBLIC ${LIBDECKY_SRC}
    PUBLIC FILE_SET HEADERS
    BASE_DIRS . FILES
    ./decky.h
)

# Prevents us from having to explicitly specify every export by hand
# if we ever build a DLL
if (MSVC)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

add_executable(${PROJECT_NAME} ${LIBDECKY_SRC} ${APPDECKY_SRC})