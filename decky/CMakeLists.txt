set(LIBDECKY_SRC card.cpp deck.cpp solitaire.cpp)

add_library(the_deck)
target_sources(the_deck PUBLIC ${LIBDECKY_SRC}
        PUBLIC FILE_SET HEADERS
        BASE_DIRS . FILES
        the_deck.h)
set_target_properties(the_deck PROPERTIES
        POSITION_INDEPENDENT_CODE ${BUILD_SHARED_LIBS})

# Prevents us from having to explicitly specify every export by hand
# if we ever build a DLL.
if (MSVC)
    include(GenerateExportHeader)
    generate_export_header(the_deck)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    target_compile_definitions(the_deck PUBLIC DLL_EXPORTS)
endif()
