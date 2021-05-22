#include "../include/notifier.hpp"

Notifier::Notifier(fs::path path){
    basepath = path;
    ino_fd = inotify_init();
}

Notifier::~Notifier(){
    store.clear();
    reverse_store.clear();
}
void Notifier::addInotifyWatch(fs::path basepath)
{
    //std::cout << "ADD WATCH FOLDER: " << basepath.c_str() << "\n";
    int i = inotify_add_watch(ino_fd,basepath.c_str(), INO_MODE);
    store[i] = basepath;
    reverse_store[basepath]=i;
    fs::file_status s;
    for(auto& p: fs::recursive_directory_iterator(basepath)){
        s = fs::symlink_status(p);
        if(fs::is_directory(s) && !fs::is_symlink(s)){
            int i = inotify_add_watch(ino_fd,p.path().c_str(), INO_MODE );
            //std::cout << "ADD WATCH FOLDER: " << p.path().c_str() << "\n";
            store[i] =  p.path();
            reverse_store[p.path()]=i;
         }

    }
}

void Notifier::removeDir(fs::path path, int wd){
    int wd_r = reverse_store.at(path);
    inotify_rm_watch(ino_fd,wd_r);
    store.erase(wd_r);
    reverse_store.erase(path);
    //std::cout << "Initial path to remove: " <<path << std::endl;
}





