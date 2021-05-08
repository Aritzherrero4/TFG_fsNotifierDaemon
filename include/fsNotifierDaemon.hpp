#ifndef FSNOTIFIER_HPP
#define FSNOTIFIER_HPP

#include <signal.h>
#include <syslog.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <systemd/sd-journal.h>
#include <systemd/sd-daemon.h>
#include <systemd/sd-bus.h>
#include <thread>

#include "../include/notifier.hpp"
#include "../include/utils.hpp"

#define SD_EMERG   "<0>"  /* system is unusable */
#define SD_ALERT   "<1>"  /* action must be taken immediately */
#define SD_CRIT    "<2>"  /* critical conditions */
#define SD_ERR     "<3>"  /* error conditions */
#define SD_WARNING "<4>"  /* warning conditions */
#define SD_NOTICE  "<5>"  /* normal but significant condition */
#define SD_INFO    "<6>"  /* informational */
#define SD_DEBUG   "<7>"  /* debug-level messages */

static FILE *log_file;
static sd_bus_error error = SD_BUS_ERROR_NULL;
static sd_bus_message *reply = NULL;
static sd_bus *bus = NULL;

#endif