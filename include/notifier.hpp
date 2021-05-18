#ifndef NOTIFIER_HPP
#define NOTIFIER_HPP

#include <filesystem>
#include <iostream>
#include <vector>
#include <map>
#include <sys/inotify.h>
#include <sys/types.h>
#include <unistd.h>
#include <algorithm>

#include <systemd/sd-bus.h>

#define INO_MODE IN_CREATE | IN_DELETE | IN_MODIFY |  IN_ISDIR | IN_IGNORED
#define NAME_MAX 255
#define BUF_LEN (10 * (sizeof(struct inotify_event) + NAME_MAX + 1))

namespace fs = std::filesystem;

class Notifier{
    public:
        int ino_fd;
        fs::path basepath;
        /*** 
         * Since there space ocuppied by each <key, value> pair is not that big to
         * be a problem even in large filesystems, the store is duplicated to allow
         * reverse lookups more eficiently.
         ***/
        std::map<int, fs::path> store;
        std::map<fs::path, int> reverse_store;

        Notifier(fs::path basepath);
        void addInotifyWatch(fs::path basepath);
        void processEvents();
        void removeDir(fs::path path, int wd);
    private:
        void sendmsg(fs::path p, int event);
};

#endif