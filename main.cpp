#include <ctime>
#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <string.h>
#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <pspiofilemgr.h>
#include <sys/stat.h>

PSP_MODULE_INFO("PSP fstat demo", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER);

int exit_callback(int arg1, int arg2, void *common) {
    sceKernelExitGame();
    return 0;
}

int callback_thread(SceSize args, void *argp) {
    int cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
    sceKernelRegisterExitCallback(cbid);
    sceKernelSleepThreadCB();
    return 0;
}

int setup_callbacks(void) {
    int thid = sceKernelCreateThread("update_thread", callback_thread, 0x11, 0xFA0, 0, 0);
    if(thid >= 0)
        sceKernelStartThread(thid, 0, 0);
    return thid;
}

void printStatInfo(const struct stat& sb) 
{
    pspDebugScreenPrintf("=== BEGIN printStatInfo ===\n");
    pspDebugScreenPrintf("File type:                ");
    switch (sb.st_mode & S_IFMT) {
        case S_IFBLK:  pspDebugScreenPrintf("block device\n");            break;
        case S_IFCHR:  pspDebugScreenPrintf("character device\n");        break;
        case S_IFDIR:  pspDebugScreenPrintf("directory\n");               break;
        case S_IFIFO:  pspDebugScreenPrintf("FIFO/pipe\n");               break;
        case S_IFLNK:  pspDebugScreenPrintf("symlink\n");                 break;
        case S_IFREG:  pspDebugScreenPrintf("regular file\n");            break;
        case S_IFSOCK: pspDebugScreenPrintf("socket\n");                  break;
        default:       pspDebugScreenPrintf("unknown?\n");                break;
    }

    pspDebugScreenPrintf("I-node number:            %d\n", sb.st_ino);
    pspDebugScreenPrintf("Mode:                     %o\n", sb.st_mode);
    pspDebugScreenPrintf("Link count:               %d\n", sb.st_nlink);
    pspDebugScreenPrintf("Ownership:                UID=%d GID=%d\n", sb.st_uid, sb.st_gid);
    pspDebugScreenPrintf("Preferred I/O block size: %d bytes\n", sb.st_blksize);
    pspDebugScreenPrintf("File size:                %d bytes\n", sb.st_size);
    pspDebugScreenPrintf("Blocks allocated:         %d\n", sb.st_blocks);

    pspDebugScreenPrintf("Last status change:       %s\n", std::ctime(&sb.st_ctime));
    pspDebugScreenPrintf("Last file access:         %s\n", std::ctime(&sb.st_atime));
    pspDebugScreenPrintf("Last file modification:   %s\n", std::ctime(&sb.st_mtime));

    pspDebugScreenPrintf("Permissions:              ");
    pspDebugScreenPrintf("%c%c%c%c%c%c%c\n",
        ((sb.st_mode & S_IRUSR) ? 'r' : '-'),
        ((sb.st_mode & S_IWUSR) ? 'w' : '-'),
        ((sb.st_mode & S_IXUSR) ? 'x' : '-'),
        ((sb.st_mode & S_IRGRP) ? 'r' : '-'),
        ((sb.st_mode & S_IWGRP) ? 'w' : '-'),
        ((sb.st_mode & S_IXGRP) ? 'x' : '-'),
        ((sb.st_mode & S_IROTH) ? 'r' : '-'),
        ((sb.st_mode & S_IWOTH) ? 'w' : '-'),
        ((sb.st_mode & S_IXOTH) ? 'x' : '-')
    );

    pspDebugScreenPrintf("Special bits:             ");
    pspDebugScreenPrintf("%s%s%s\n",
        ((sb.st_mode & S_ISUID) ? "setuid " : ""),
        ((sb.st_mode & S_ISGID) ? "setgid " : ""),
        ((sb.st_mode & S_ISVTX) ? "sticky " : "")
    );
    pspDebugScreenPrintf("=== END ===\n");
}

void printSceStatInfo(SceIoStat& stat)
{
    #define pspDebug pspDebugScreenPrintf
    pspDebug("=== BEGIN sceIoGetstat ===\n");
    pspDebug("stat.st_mode:                   0x%08x\n", stat.st_mode);
    pspDebug("stat.st_mode (IO Access Mode):  ");
    switch (stat.st_mode & FIO_S_IFMT) {
        case FIO_S_IFLNK:  pspDebug("Symbolic link\n");            break;
        case FIO_S_IFDIR:  pspDebug("Directory\n");                break;
        case FIO_S_IFREG:  pspDebug("Regular file\n");             break;
        default:           pspDebug("unknown?\n");                 break;
    }
    pspDebug("stat.st_attr:                   0x%08x\n", stat.st_attr);
    pspDebug("stat.st_attr (IO File Mode):    ");
    switch (stat.st_attr & FIO_SO_IFMT) {
        case FIO_SO_IFLNK:  pspDebug("Symbolic link\n");           break;
        case FIO_SO_IFDIR:  pspDebug("Directory\n");               break;
        case FIO_SO_IFREG:  pspDebug("Regular file\n");            break;
        default:            pspDebug("unknown?\n");                break;
    }
    pspDebug("File size:                      %d bytes\n", stat.st_size);
    pspDebugScreenPrintf("=== END ===\n");
}

int main(int argc, char* argv[])
{
    setup_callbacks();
    pspDebugScreenInit();

    // PSP file stat
    SceIoStat sceIoStat1;
    int sceIoGetstatRes1 = sceIoGetstat("disc0:/PSP_GAME/USRDIR/crosshair.png", &sceIoStat1);
    pspDebugScreenPrintf("sceIoGetstatRes() for \"disc0:/PSP_GAME/USRDIR/crosshair.png\"\n");
    if (sceIoGetstatRes1 >= 0)
    {
        pspDebugScreenPrintf("Operation successful\n");
        printSceStatInfo(sceIoStat1);
    }
    else
    {
        pspDebugScreenPrintf("Operation failed with response code %d\n", sceIoGetstatRes1);
    }
    
    SceIoStat sceIoStat2;
    int sceIoGetstatRes2 = sceIoGetstat("ms0:/PSP/GAME/crosshair.png", &sceIoStat2);
    pspDebugScreenPrintf("\n\nsceIoGetstatRes() for \"ms0:/PSP/GAME/crosshair.png\"\n");
    if (sceIoGetstatRes2 >= 0)
    {
        pspDebugScreenPrintf("Operation successful\n");
        printSceStatInfo(sceIoStat2);
    }
    else
    {
        pspDebugScreenPrintf("Operation failed with response code %d\n", sceIoGetstatRes2);
    }

    pspDebugScreenPrintf("\n\nThe program will exit in 30 seconds...");
    sleep(30);

    return 0;
}
