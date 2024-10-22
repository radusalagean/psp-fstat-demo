#include <ctime>
#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <string.h>
#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>
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

int main(int argc, char* argv[])
{
    setup_callbacks();
    pspDebugScreenInit();

    // Open a file and print fstat results
    FILE* file = fopen("../USRDIR/crosshair.png", "rb");
    if (!file)
    {
        pspDebugScreenPrintf("Couldn't open %s: %s", file, strerror(errno));
    }
    else
    {
        pspDebugScreenPrintf("File found!\n");
        struct stat st;

        // fileno() - get the integer file descriptor associated with the stream:
        //  0 - Standard input (stdin)
        //  1 - Standard output (stdout)
        //  2 - Standard error (stderr)
        // ≥3 - Other opened files
        // -1 - Error occurred
        int filenoRes = fileno(file);


        pspDebugScreenPrintf("filenoRes: %d\n", filenoRes);
        int errnoRes = errno;
        pspDebugScreenPrintf("errnoRes: %d (%s)\n", errnoRes, strerror(errnoRes));
        fstat(filenoRes, &st);
        printStatInfo(st);
        fclose(file);
    }

    pspDebugScreenPrintf("\nThe program will exit in 30 seconds...");
    sleep(30);

    return 0;
}
