set(LIBDECKY_SRC card.cpp deck.cpp solitaire.cpp)
set(ENCRYPTOR_SRC main_encrypt.cpp)
set(DECRYPTOR_SRC main_decrypt.cpp)

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

add_executable(${ENCRYPTOR_APP} ${LIBDECKY_SRC} ${ENCRYPTOR_SRC})
add_executable(${DECRYPTOR_APP} ${LIBDECKY_SRC} ${DECRYPTOR_SRC})