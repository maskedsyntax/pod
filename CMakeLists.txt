cmake_minimum_required(VERSION 3.16)
project(Pod)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(pod src/main.cpp resources.qrc)
target_link_libraries(pod PRIVATE Qt6::Widgets)

# Installation
install(TARGETS pod DESTINATION bin)
install(DIRECTORY assets DESTINATION share/pod)
install(FILES style.qss DESTINATION share/pod)

# CPack / Debian Packaging
set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_NAME "pod")
set(CPACK_PACKAGE_VERSION "1.1.1")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A minimalist, ricing-friendly powermenu for i3wm")
set(CPACK_PACKAGE_CONTACT "Maintainer <maintainer@example.com>")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Pod Contributors")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6, libqt6widgets6, libqt6gui6, libqt6core6, i3lock")
set(CPACK_DEBIAN_PACKAGE_SECTION "utils")
set(CPACK_PACKAGING_INSTALL_PREFIX "/usr")

include(CPack)
