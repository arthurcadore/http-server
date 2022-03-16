// Incluindo bibliotecas e protótipos de função.
#include "functions.h"

// Retorna uma string formatada com o horário de execução (no sistema).
char *current_time_string() {
    time_t current_time;
    struct tm *time_info;

    char *timeString =
        malloc(sizeof(char) * 20);  // space for "dd/MM/yyyy HH:mm:ss\0"

    // Devolve a leitura do relógio do sistema.
    time(&current_time);

    // Transformando o valor da leitura em uma estrutura.
    time_info = localtime(&current_time);

    strftime(timeString, 20, "%d/%m/%G %H:%M:%S", time_info);

    return timeString;
}

// Recebe o valor de tempo e o tipo de evento e armazena no arquivo de LOG.
void log_event(char type[], char text[]) {
    FILE *file;

    char *time;

    time = current_time_string();

    file = fopen("log.txt", "a");

    fprintf(file, "[%s]\t%s :\t%s\n", type, time, text);

    fclose(file);

    return;
}
