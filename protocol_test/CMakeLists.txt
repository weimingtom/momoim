include_directories(include)
include_directories(../logger/include)
include_directories(../protocol/include)
include_directories(../imcommon/include)
SET(PROTOCOL_SOURCE_FILE
    main.c
    status.c
    logintest.c
    message.c
    info.c
    frienddata.c
    friendrequest.c
    infodata.c
    inforequest.c
    )
add_executable(protocol_test ${PROTOCOL_SOURCE_FILE})
target_link_libraries(protocol_test protocol)
target_link_libraries(protocol_test logger)
target_link_libraries(protocol_test pthread)
target_link_libraries(protocol_test mcrypt)
target_link_libraries(protocol_test crypto)