#ifndef NOTIFIER_HPP
#define NOTIFIER_HPP

#include <filesystem>
#include <iostream>
#include <vector>
#include <sys/inotify.h>
#include <sys/types.h>
#include <unistd.h>

#include <systemd/sd-bus.h>

#define INO_MODE IN_CREATE | IN_DELETE | IN_MODIFY
#define NAME_MAX 255
#define BUF_LEN (10 * (sizeof(struct inotify_event) + NAME_MAX + 1))

namespace fs = std::filesystem;

class Notifier{
    public:
        int ino_fd;
        fs::path basepath;
        std::vector<fs::path> store;
        Notifier(fs::path basepath);
        void addInotifyWatch();
        void processEvents();
    private:
        void sendmsg(fs::path p, int event);
};

#endif