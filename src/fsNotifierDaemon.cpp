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

void initialize(){
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

}


int main()
{
    initialize();


    fs::path name = "/home/aritz/test/root";
    Notifier *n = new Notifier(name);
    n->addInotifyWatch();

    fprintf(log_file,SD_INFO "Watch system initialized\n");
    fflush(log_file);

    n->processEvents();

    return 0;
}