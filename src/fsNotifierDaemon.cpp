#include "../include/fsNotifierDaemon.hpp"

void signal_handler(int sig) {
    if (sig == SIGTERM) {
        fprintf (log_file,SD_INFO "SIGTERM Received, daemon exiting\n");
        fclose(log_file);
        exit (1);
    }
    if (sig == SIGINT) {
      fprintf (log_file,SD_INFO "SIGINT Received, daemon exiting\n");
      fclose(log_file);
      exit (1);
    }
    if (sig == SIGHUP){
        fprintf (log_file,SD_INFO "SIGHUP Received, reloading config\n");
        exit (1);        
    }
}

int initialize(){
    int fd = sd_journal_stream_fd("fsNotifierDaemon", LOG_INFO, 1);

    if (fd < 0) {
        fprintf(stderr, "Failed to create stream fd: %s\n", strerror(-fd));
        exit(1);
    }

    log_file = fdopen(fd, "w");

    if (!log_file) {
        fprintf(stderr, "Failed to create file object: %m\n");
        close(fd);
        exit(1);
    }

    signal(SIGTERM, signal_handler);
    signal(SIGHUP, signal_handler);
    signal(SIGINT, signal_handler); //This will be removed

    /** Bus init **/
    int r;
    r = sd_bus_open_user(&bus);
    if (r < 0) {
        fprintf(log_file, "Failed to connect to system bus: %s\n", strerror(-r));
        return r;
    }
    return 0;
}

int send_bus_message(int event, fs::path path){
        int r;
        r = sd_bus_call_method(bus,
                               "net.aritzherrero.fsCheck",           /* service to contact */
                               "/net/aritzherrero/fsCheck",           /* object path */
                               "net.aritzherrero.fsCheck",           /* interface name */
                               "getUpdate",                          /* method name */
                               &error,                               /* object to return error in */
                               &m,                                   /* return message on success */
                               "is",                                 /* input signature */
                                event,                               /* first argument */
                               path.c_str());                        /* second argument */
        if (r < 0) {
                fprintf(log_file, "Failed to issue method call: %s\n", error.message);
                return r;
        }
        return 0; 
}


int main()
{
    int r;
    char buf[BUF_LEN];
    struct inotify_event *event = NULL;

    fs::path name = "/home/aritz/test/root";
    Notifier *nt = new Notifier(name);

    r = initialize();
    if (r < 0) {
            goto dbus_error;
    } 
    
    nt->addInotifyWatch();

    fprintf(log_file,SD_INFO "Watch system initialized\n");
    fflush(log_file);
    while (1) {
            int n = read(nt->ino_fd, buf, BUF_LEN);
            char* p = buf;
            fs::path tmp_path;
            while (p < buf + n) {
                event = (struct inotify_event*)p;
                uint32_t mask = event->mask;
                tmp_path = nt->store[event->wd -1];
                tmp_path += "/";
                tmp_path += event->name;
                if (mask & IN_DELETE) {
                    fprintf(log_file, SD_WARNING "File has been deleted: %s\n", tmp_path.c_str());
                    r = send_bus_message(2, tmp_path);
                }
                if (mask & IN_CREATE) {
                    fprintf(log_file, SD_WARNING "File has been created: %s\n", tmp_path.c_str());
                    r = send_bus_message(1, tmp_path);
                }
                if (mask & IN_MODIFY) {
                    fprintf(log_file, SD_WARNING "File has been modified: %s\n", tmp_path.c_str());
                    r = send_bus_message(3, tmp_path);
                }
                fflush(log_file);
                p += sizeof(struct inotify_event) + event->len;
            }
    }

    return 0;
//If a DBUS related error is detected, unreference from dbus and 
//exit
dbus_error:
        sd_bus_error_free(&error);
        sd_bus_message_unref(m);
        sd_bus_unref(bus);
        return r < 0 ? EXIT_FAILURE : EXIT_SUCCESS;
}