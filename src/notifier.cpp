#include "../include/notifier.hpp"

Notifier::Notifier(fs::path path){
    basepath = path;
    ino_fd = inotify_init();
    std::cout << "ADD WATCH FOLDER: " << basepath.c_str() << "\n";
    inotify_add_watch(ino_fd,basepath.c_str(), INO_MODE);
    store.push_back(path);
}

void Notifier::addInotifyWatch()
{
    fs::file_status s;
    for(auto& p: fs::recursive_directory_iterator(basepath)){
        s = fs::symlink_status(p);
        if(fs::is_directory(s) && !fs::is_symlink(s)){
            inotify_add_watch(ino_fd,p.path().c_str(), INO_MODE );
            std::cout << "ADD WATCH FOLDER: " << p.path().c_str() << "\n";
            store.push_back(p.path());
         }

    }
}


void Notifier::processEvents(){
    char buf[BUF_LEN];
    struct inotify_event *event = NULL;
    while (1) {
            int n = read(ino_fd, buf, BUF_LEN);
            char* p = buf;
            fs::path tmp_path;
            while (p < buf + n) {
                event = (struct inotify_event*)p;
                uint32_t mask = event->mask;
                tmp_path = store[event->wd];
                tmp_path += event->name;
                if (mask & IN_DELETE) {
                    printf("File has been deleted: %s\n", tmp_path.c_str());
                }
                if (mask & IN_CREATE) {
                    printf("File has been created: %s\n", tmp_path.c_str());
                }
                if (mask & IN_MODIFY) {
                    printf("File has been modified\n");
                }

                p += sizeof(struct inotify_event) + event->len;
            }
    }
}

