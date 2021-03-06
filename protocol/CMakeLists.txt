include_directories(include)
include_directories(../imcommon/include)

SET(LIBPROTOCOL_SRC
    init.c
    packets/friend/Request.c
    packets/friend/Data.c
    packets/friend/SearchByNickname.c
    packets/friend/UserList.c
    packets/friend/Notify.c
    packets/friend/Add.c
    packets/friend/Accept.c
    packets/friend/Move.c
    packets/friend/Delete.c
    packets/friend/GroupAdd.c
    packets/friend/GroupDelete.c
    packets/friend/GroupMove.c
    packets/friend/GroupRename.c
    packets/net/FriendDiscover.c

    packets/info/Request.c
    packets/info/Data.c
    packets/info/StatusChange.c
    packets/info/PasswordChange.c

    packets/login/Login.c
    packets/login/Accept.c
    packets/login/Logout.c
    packets/login/Register.c

    packets/status/Failure.c
    packets/status/Hello.c
    packets/status/KeepAlive.c
    packets/status/OK.c
    packets/status/Crash.c
    packets/status/Kick.c
    packets/status/Cancel.c
    packets/status/SwitchProtocol.c

    packets/file/Data.c
    packets/file/Request.c
    packets/file/DataStart.c
    packets/file/DataEnd.c
    packets/file/Reset.c
    packets/file/StoreRequest.c
    packets/file/Proxy.c

    packets/message/Normal.c
    packets/message/QueryOffline.c
    packets/message/RecordData.c
    packets/message/RecordQuery.c

    packets/net/NATRegister.c
    packets/net/NATRequest.c
    packets/net/NATAccept.c
    packets/net/NATRefuse.c
    packets/net/NATReady.c
    packets/net/QualityTest.c
    packets/net/Detected.c
    packets/net/InetAddress.c
    packets/net/DiscoverAccept.c
    packets/net/DiscoverRefuse.c
    )
ADD_LIBRARY(protocol SHARED ${LIBPROTOCOL_SRC})
target_link_libraries(protocol mcrypt)
target_link_libraries(protocol imcommon)