# CMakeLists.txt for libacu

# Source files
set(SOURCE_FILES
        acu.cc
        aggregation.cc
        correlation.cc
        incoming_alert.cc
        outgoing_alert.cc
        receiver.cc
        sender.cc
        storage.cc
        threshold.cc)

# Library definition + linking
add_library(acu ${SOURCE_FILES})
target_link_libraries(acu ${LINK_LIBS})