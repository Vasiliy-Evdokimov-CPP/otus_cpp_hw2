# определяем минимальную версию CMake, при помощи которой можно обрабатывать текущий файл проекта
cmake_minimum_required(VERSION 3.14)

# устанавливаем кэшированную переменную PATCH_VERSION со значением "2"
set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
# устанавливаем переменную PROJECT_VERSION
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

# устанавливаем имя проекта и версию
project(ip_filter VERSION ${PROJECT_VESRION})

# копируем файл version.h.in в файл version.h, при этом выполняя подстановку переменной внутри файла
configure_file(version.h.in version.h)

# создаем исполняемый файл ip_filter на основе ip_filter.cpp
add_executable(ip_filter ip_filter.cpp)

# устанавливаем свойства для цели (targets) ip_filter 
# указывая, что требуется стандарт C++ 14 и он обязателен для компиляции
set_target_properties(
    ip_filter
    PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
)
# добавляем директорию "${CMAKE_BINARY_DIR}" в качестве приватной директории для цели ip_filter; 
# это означает, что эта директория будет доступна только для цели ip_filter
target_include_directories(
    ip_filter
    PRIVATE "${CMAKE_BINARY_DIR}"
)

# определяем для целей опции компиляторов (MSVC и прочих)
if (MSVC)
    target_compile_options(ip_filter PRIVATE
        /W4
    )
    # устанавливаем генератор упаковки на "ZIP" - файл релиза будет упакован в .zip архив
    set(CPACK_GENERATOR ZIP)
else ()
    target_compile_options(ip_filter PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    # устанавливаем генератор упаковки на "DEB" - будут сгенерирован пакет .deb для упаковки проекта
    set(CPACK_GENERATOR DEB)
endif()

# указываем, что при установке (копировании) цели с именем "ip_filter", 
# ее исполняемый файл должен быть скопирован в папку "bin"
install(TARGETS ip_filter RUNTIME DESTINATION bin)

# устанавливаем переменные, используемые CPack (инструмент для упаковки проекта в различные форматы пакетов) для версии пакета
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

# устанавливаем контактный email для пакета при использовании CPack
set(CPACK_PACKAGE_CONTACT vasiliyevdokimov84@gmail.com)

# включаем использование CPack в проекте
include(CPack)