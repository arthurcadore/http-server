#include "functions.h"

char *current_time_string() {
    time_t current_time;
    struct tm *time_info;
    char *timeString =
        malloc(sizeof(char) * 20);  // space for "dd/MM/yyyy HH:mm:ss\0"

    time(&current_time);
    time_info = localtime(&current_time);

    strftime(timeString, 20, "%d/%m/%G %H:%M:%S", time_info);

    return timeString;
}

void log_event(char type[], char text[]) {
    FILE *file;

    char *time;

    time = current_time_string();

    file = fopen("log.txt", "a");

    fprintf(file, "[%s]\t%s :\t%s\n", type, time, text);

    fclose(file);

    return;
}
