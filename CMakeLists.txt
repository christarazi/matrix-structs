cmake_minimum_required(VERSION 3.1)

project(matrix_structs CXX)

option(BUILD_TESTS "Build tests" OFF)
option(BUILD_DOC "Build documentation" OFF)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

#
# Find doxygen & generate API documentation.
#
include(Doxygen)

#
# Adjust compiler flags.
#
include(CompilerFlags)

file(DOWNLOAD
    "https://github.com/nlohmann/json/releases/download/v3.1.2/json.hpp"
    ${PROJECT_SOURCE_DIR}/include/json.hpp
    EXPECTED_HASH SHA256=fbdfec4b4cf63b3b565d09f87e6c3c183bdd45c5be1864d3fcb338f6f02c1733)

file(DOWNLOAD
    "https://github.com/mpark/variant/releases/download/v1.3.0/variant.hpp"
    ${PROJECT_SOURCE_DIR}/include/variant.hpp
    EXPECTED_MD5 "be0ce322cdd408e1b347b9f1d59ea67a")

include_directories(include)

set(SRC
    src/events.cpp
    src/requests.cpp
    src/errors.cpp
    src/common.cpp

    src/events/aliases.cpp
    src/events/avatar.cpp
    src/events/canonical_alias.cpp
    src/events/common.cpp
    src/events/create.cpp
    src/events/encrypted.cpp
    src/events/encryption.cpp
    src/events/guest_access.cpp
    src/events/history_visibility.cpp
    src/events/join_rules.cpp
    src/events/member.cpp
    src/events/name.cpp
    src/events/pinned_events.cpp
    src/events/power_levels.cpp
    src/events/redaction.cpp
    src/events/topic.cpp

    src/events/messages/audio.cpp
    src/events/messages/emote.cpp
    src/events/messages/file.cpp
    src/events/messages/image.cpp
    src/events/messages/notice.cpp
    src/events/messages/text.cpp
    src/events/messages/video.cpp

    src/responses/common.cpp
    src/responses/create_room.cpp
    src/responses/crypto.cpp
    src/responses/login.cpp
    src/responses/register.cpp
    src/responses/empty.cpp
    src/responses/media.cpp
    src/responses/messages.cpp
    src/responses/profile.cpp
    src/responses/sync.cpp
    src/responses/version.cpp)

add_library(${PROJECT_NAME} ${SRC})
install(TARGETS ${PROJECT_NAME}
    ARCHIVE DESTINATION lib/static
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin)
install(DIRECTORY include/
    DESTINATION include/${PROJECT_NAME}
    FILES_MATCHING PATTERN "*.hpp")


if (BUILD_TESTS)
    enable_testing()

    file(COPY fixtures DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

    find_package(GTest)

    if (NOT GTest_FOUND)
        include(${CMAKE_SOURCE_DIR}/cmake/GoogleTest.cmake)
    endif()

    include_directories(${GTEST_INCLUDE_DIRS})

    add_executable(identifiers tests/identifiers.cpp)
    target_link_libraries(identifiers ${PROJECT_NAME} ${GTEST_BOTH_LIBRARIES})

    add_executable(events tests/events.cpp)
    target_link_libraries(events ${PROJECT_NAME} ${GTEST_BOTH_LIBRARIES})

    add_executable(messages tests/messages.cpp)
    target_link_libraries(messages ${PROJECT_NAME} ${GTEST_BOTH_LIBRARIES})

    add_executable(responses tests/responses.cpp)
    target_link_libraries(responses ${PROJECT_NAME} ${GTEST_BOTH_LIBRARIES})

    add_executable(requests tests/requests.cpp)
    target_link_libraries(requests ${PROJECT_NAME} ${GTEST_BOTH_LIBRARIES})

    add_executable(errors tests/errors.cpp)
    target_link_libraries(errors ${PROJECT_NAME} ${GTEST_BOTH_LIBRARIES})

    add_executable(crypto tests/crypto.cpp)
    target_link_libraries(crypto ${PROJECT_NAME} ${GTEST_BOTH_LIBRARIES})

    if (NOT GTest_FOUND)
        add_dependencies(identifiers GTest)
        add_dependencies(events GTest)
        add_dependencies(messages GTest)
        add_dependencies(responses GTest)
        add_dependencies(requests GTest)
        add_dependencies(errors GTest)
        add_dependencies(crypto GTest)
    endif()

    add_test(MatrixIdentifiers identifiers)
    add_test(MatrixErrors errors)
    add_test(CryptoStructs crypto)
    add_test(StateEvents events)
    add_test(RoomEvents messages)
    add_test(Responses responses)
    add_test(Requests requests)
endif()
