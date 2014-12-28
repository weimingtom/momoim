#include <fcntl.h>
#include <logger.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdio.h>
#include <sys/mman.h>
#include <string.h>

#include "user.h"
#include "data/user.h"

//Get User Directory Path
//17 characters at least
void UserGetDir(char *path, uint32_t uid, const char *relPath)
{
    sprintf(path, "user/%02d/%d/%s", uid % 100, uid / 100, relPath);
}

void UserCreateDirectory(uint32_t uid)
{
    char userDir[20], path[100];
    sprintf(userDir, "user");
    if (mkdir(userDir, 0700) != 0 && errno != EEXIST)
    {
        log_error("User", "Cannot create directory %s\n", userDir);
        return;
    }
    sprintf(userDir, "user/%02d", uid % 100);
    if (mkdir(userDir, 0700) != 0 && errno != EEXIST)
    {
        log_error("User", "Cannot create directory %s\n", userDir);
        return;
    }

    sprintf(userDir, "user/%02d/%d", uid % 100, uid / 100);
    if (mkdir(userDir, 0700) != 0 && errno != EEXIST)
    {
        log_error("User", "Cannot create directory %s\n", userDir);
        return;
    }

    sprintf(path, "%s/info", userDir);
    UserCreateInfoFile(uid, path);

    sprintf(path, "%s/friends", userDir);
    UserCreateFriendsFile(path);
}

void UserCreateInfoFile(uint32_t uid, char *path)
{
    UserInfo info = {
            .uid=uid,
            .nickName="Baby",
            .sex=0
    };
    UserSaveInfoFile(&info, path);
}

int UserSaveInfoFile(UserInfo *info, char *path)
{
    int fd = open(path, O_CREAT | O_WRONLY | O_TRUNC, 0600);
    if (fd == -1)
    {
        log_error("User", "Cannot create user info file %s.\n", path);
        return 1;
    }
    write(fd, info, sizeof(UserInfo));
    close(fd);
    return 0;
}

UserInfo *UserGetInfo(uint32_t uid)
{
    char infoFile[30];
    UserGetDir(infoFile, uid, "info");
    if (access(infoFile, R_OK))
    {
        UserCreateInfoFile(uid, infoFile);
        if (access(infoFile, R_OK))
        {
            return NULL;
        }
    }
    int fd = open(infoFile, O_RDONLY);
    if (fd == -1)
    {
        log_error("User", "Cannot open user info file %s.\n", infoFile);
        return NULL;
    }
    UserInfo *info = (UserInfo *) malloc(sizeof(UserInfo));
    read(fd, info, sizeof(UserInfo));
    close(fd);
    return info;
}

void UserFreeInfo(UserInfo *friends)
{
    if (friends)
        free(friends);
}

void UserCreateFriendsFile(char *path)
{
    uint32_t mfriends[] = {
            10000
    };
    UserGroup groups[] = {
            {
                    .groupId=0,
                    .groupName="Friends",
                    .friendCount=1,
                    .friends=mfriends
            },
            {
                    .groupId=UINT8_MAX,
                    .groupName="Blacklist",
                    .friendCount=0
            }
    };
    UserFriends friends = {
            .groupCount=2,
            .groups=groups
    };
    UserSaveFriendsFile(&friends, path);
};

int UserSaveFriendsFile(UserFriends *friends, char *path)
{
    int fd = open(path, O_CREAT | O_RDWR | O_TRUNC, 0600);
    if (fd == -1)
    {
        log_error("User", "Cannot create user friends file %s.\n", path);
        return 1;
    }
    size_t length = UserFriendsSize(friends);
    if (lseek(fd, length - 1, SEEK_SET) == -1)
    {
        log_error("User", "Cannot expand user friends file %s.\n", path);
        return 3;
    }
    write(fd, "\0", 1);
    void *addr = mmap(NULL, length, PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED)
    {
        log_error("User", "Cannot mmap user friends file %s.%s\n", path, strerror(errno));
        close(fd);
        return 2;
    }

    UserFriendsEncode(friends, addr);
    munmap(addr, length);
    close(fd);
    return 0;
}

UserFriends *UserGetFriends(uint32_t uid)
{
    UserFriends *friends = NULL;
    char friendFile[30];
    UserGetDir(friendFile, uid, "friends");
    int fd = open(friendFile, O_RDONLY);
    if (fd == -1)
    {
        log_error("User", "Cannot read user friends file %s.\n", friendFile);
        return NULL;
    }
    //Map File To Memory
    struct stat statBuf;
    if (fstat(fd, &statBuf))
        goto cleanup;
    size_t len = (size_t) statBuf.st_size;
    void *addr = mmap(NULL, len, PROT_READ, MAP_PRIVATE, fd, 0);
    if (addr == MAP_FAILED)
        goto cleanup;

    friends = UserFriendsDecode(addr);
    cleanup:
    if (addr && addr != MAP_FAILED)
        munmap(addr, len);
    close(fd);
    return friends;
}