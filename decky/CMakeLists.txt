set(LIBDECKY decky.cpp card.cpp deck.cpp)
add_library(decky STATIC)

target_sources(decky
    PUBLIC ${LIBDECKY}
    PUBLIC FILE_SET HEADERS
    BASE_DIRS . FILES
    ./decky.h
)

# Prevents us from having to explicitly specify every export by hand
# if we ever build a DLL
if (MSVC)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()