#include "votacion.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

int main() {
    /* Abrimos el archivo de input */
    FILE *input_file = fopen("C:\\\\Users\\\\x\\\\CLionProjects\\\\untitled7\\\\input.txt", "r");

    /* Abrimos el archivo de output */
    FILE *output_file = fopen("out.txt", "w");

    /* Leemos la cantidad de candidatos y de eventos */
    int N_CANDIDATES;
    fscanf(input_file, "%d", &N_CANDIDATES);

    /* Creamos un arreglo de estructuras Candidate */
    Candidate candidates[N_CANDIDATES];

    /* Inicializamos la estructura de candidatos */
    for (int i = 0; i < N_CANDIDATES; i++) {
        candidates[i].id = i;
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    int N_EVENTS;
    fscanf(input_file, "%d", &N_EVENTS);
    init_votacion(N_CANDIDATES);
    ncandidates = N_CANDIDATES;

    /* String para guardar la instrucción actual */
    char command[32];

    for (int event = 0; event < N_EVENTS; event++) {
        /* Leemos el evento */
        fscanf(input_file, "%s", command);

        /* Implementamos cada evento */
        if (!strcmp(command, "REGISTRAR-VOTO")) {
            int voter_ID, candidate_ID;
            fscanf(input_file, "%d %d", &voter_ID, &candidate_ID);
            registrar_voto(voter_ID, candidate_ID);
        } else if (!strcmp(command, "CONTEO-PARCIAL")) {
            conteo_parcial();
        } else if (!strcmp(command, "CONTEO-TOTAL")) {
            conteo_total();
        } else if (!strcmp(command, "CONTEO-RANGO")) {
            int min_votes, max_votes;
            fscanf(input_file, "%d %d", &min_votes, &max_votes);
            conteo_rango(min_votes, max_votes);
        } else if (!strcmp(command, "ORDENAR-CANDIDATOS")) {
            ordenar_candidatos();
        } else if (!strcmp(command, "ANULAR-VOTO")) {
            int voter_ID, candidate_ID;
            fscanf(input_file, "%d %d", &voter_ID, &candidate_ID);
            anular_voto(voter_ID, candidate_ID);
        } else if (!strcmp(command, "ELIMINAR-CANDIDATO")) {
            eliminar_candidato();
        } else if (!strcmp(command, "TRASPASAR-EXCESO-VOTOS")) {
            int M, N;
            fscanf(input_file, "%d %d", &M, &N);
            transferir_exceso_votos(M, N);
        }
    }

    fclose(input_file);
    fclose(output_file);

    return 0;
}