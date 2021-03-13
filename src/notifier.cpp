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





