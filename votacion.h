#ifndef VOTACION_H
#define VOTACION_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_CANDIDATES 1000
#define MAX_VOTERS 10000
typedef struct {
    int id;
    int votes;
    bool eliminated;
} Candidate;

void init_votacion(int num_candidates);
void registrar_voto(int voter_id, int candidate_id);
void anular_voto(int voter_id, int candidate_id);
void conteo_parcial(void);
void conteo_total(void);
void conteo_rango(int min_votes, int max_votes);
void ordenar_candidatos(void);
int compare_candidates(const void *a, const void *b);
void eliminar_candidato();
void transferir_exceso_votos(int candidate_id, int M);
int candidate_votes[MAX_CANDIDATES] = {0};
int voter_voted[MAX_VOTERS] = {0};
int votes[MAX_CANDIDATES][MAX_VOTERS] = {0};
int ncandidates;


void init_votacion(int num_candidates) {
    memset(candidate_votes, 0, num_candidates * sizeof(int));
    memset(voter_voted, 0, MAX_VOTERS * sizeof(int));
    memset(votes, 0, num_candidates * MAX_VOTERS * sizeof(int));
}

void registrar_voto(int voter_id, int candidate_id) {
    if (voter_voted[voter_id] != 0) {
        printf("El votante %d ya ha votado\n", voter_id);
        return;
    }

    if (candidate_id < 0 || candidate_id >= ncandidates) {
        printf("El candidato %d no es válido\n", candidate_id);
        return;
    }

    votes[candidate_id][candidate_votes[candidate_id]] = voter_id;
    candidate_votes[candidate_id]++;
    voter_voted[voter_id] = candidate_id + 1;

    printf("VOTO REGISTRADO %d\n", voter_id);
}

void conteo_parcial() {
    printf("CONTEO-PARCIAL\n");
    int total_votes = 0;
    for (int i = 0; i < ncandidates; i++) {
        printf("CANDIDATE %d\n", i);
        if (candidate_votes[i] == 0) {
            printf("\tNO HAY VOTOS REGISTRADOS\n");
        } else {
            for (int j = 0; j < candidate_votes[i]; j++) {
                printf("\tVOTE %d\n", votes[i][j]);
            }
            total_votes += candidate_votes[i];
        }
    }
    printf("TOTAL PARCIAL DE VOTOS: %d\n", total_votes);
}

void conteo_total() {
    printf("CONTEO-TOTAL\n");
    int total_votes = 0;
    for (int i = 0; i < ncandidates; i++) {
        printf("CANDIDATO %d: %d\n", i, candidate_votes[i]);
        total_votes += candidate_votes[i];
    }
    printf("TOTAL VOTOS: %d\n", total_votes);
}

// Parte B

void conteo_rango(int min_votes, int max_votes) {
    printf("CONTEO-RANGO %d %d\n", min_votes, max_votes);
    int total_range_votes = 0;
    for (int i = 0; i < ncandidates; i++) {
        if (candidate_votes[i] >= min_votes && candidate_votes[i] <= max_votes) {
            printf("CANDIDATO %d: %d\n", i, candidate_votes[i]);
            total_range_votes += candidate_votes[i];
        }
    }
    printf("TOTAL DE VOTOS RANGO: %d\n", total_range_votes);
}

// Función de comparación para ordenar los candidatos por cantidad de votos, de
// mayor a menor
int compare_candidates(const void *a, const void *b) {
    Candidate *candidate_a = (Candidate *)a;
    Candidate *candidate_b = (Candidate *)b;

    if (candidate_a->votes != candidate_b->votes) {
        return candidate_b->votes - candidate_a->votes;
    }

    return candidate_a->id - candidate_b->id;
}

void ordenar_candidatos() {
    printf("CANDIDATOS-ORDENADOS\n");

    // Creamos una estructura Candidate para cada candidato
    Candidate candidates[ncandidates];
    for (int i = 0; i < ncandidates; i++) {
        candidates[i].id = i;
        candidates[i].votes = candidate_votes[i];
    }

    // Ordenamos los candidatos usando la función de comparación
    qsort(candidates, ncandidates, sizeof(Candidate), compare_candidates);

    // Imprimimos los candidatos y sus votos
    int total_votes = 0;
    for (int i = 0; i < ncandidates; i++) {
        printf("CANDIDATO %d: %d\n", candidates[i].id, candidates[i].votes);
        total_votes += candidates[i].votes;
    }
    printf("TOTAL DE VOTOS: %d\n", total_votes);
}

void anular_voto(int voter_id, int candidate_id) {
    if (voter_voted[voter_id] == 0) {
        printf("NO SE ENCONTRO UN VOTO VALIDO CON ID %d\n", voter_id);
        return;
    }

    if (voter_voted[voter_id] != candidate_id + 1) {
        printf("NO SE ENCONTRO UN VOTO VALIDO DE VOTANTE %d POR CANDIDATO %d\n",
               voter_id, candidate_id);
        return;
    }

    int candidate_votes_count = candidate_votes[candidate_id];
    int candidate_vote_index = -1;
    for (int i = 0; i < candidate_votes_count; i++) {
        if (votes[candidate_id][i] == voter_id) {
            candidate_vote_index = i;
            break;
        }
    }

    if (candidate_vote_index == -1) {
        printf("NO SE ENCONTRO UN VOTO VALIDO DE VOTANTE %d POR CANDIDATO %d\n",
               voter_id, candidate_id);
        return;
    }

    // Eliminar el voto del candidato
    for (int i = candidate_vote_index; i < candidate_votes_count - 1; i++) {
        votes[candidate_id][i] = votes[candidate_id][i + 1];
    }
    candidate_votes[candidate_id]--;

    // Restablecer el estado del votante
    voter_voted[voter_id] = 0;

    printf("VOTO ELIMINADO CORRECTAMENTE\n");
}// Parte C
void eliminar_candidato() {
    int min_votes = candidate_votes[0];
    int min_id = 0;
    for (int i = 1; i < ncandidates; i++) {
        if (candidate_votes[i] < min_votes) {
            min_votes = candidate_votes[i];
            min_id = i;
        } else if (candidate_votes[i] == min_votes) {
            min_id = (i < min_id) ? i : min_id;
        }
    }

    int closest_id = -1;
    int closest_distance = ncandidates + 1;
    for (int j = 0; j < ncandidates; j++) {
        if (j != min_id) {
            int distance = abs(j - min_id);
            if (distance < closest_distance) {
                closest_distance = distance;
                closest_id = j;
            }
        }
    }

    printf("CANDIDATO %d HA SIDO ELIMINADO\n", min_id);
    printf("CANDIDATO %d HA RECIBIDO %d VOTOS\n", closest_id, candidate_votes[closest_id]);
    // Transfiere los votos del candidato eliminado al candidato más cercano
    for (int i = 0; i < candidate_votes[min_id]; i++) {
        int voter_id = votes[min_id][i];
        votes[closest_id][candidate_votes[closest_id]] = voter_id;
        candidate_votes[closest_id]++;
        voter_voted[voter_id] = closest_id + 1;
    }
    candidate_votes[min_id] = 0;
}

void transferir_exceso_votos(int candidate_id, int M) {
    if (candidate_votes[candidate_id] > M) {
        int exceso = candidate_votes[candidate_id] - M;
        candidate_votes[candidate_id] = M;

        int j = candidate_id + 1;
        while (j < ncandidates && exceso > 0) {
            if (candidate_votes[j] != -1 && candidate_votes[j] != 0) {
                int votes_transferred = exceso;
                candidate_votes[j] += votes_transferred;
                exceso -= votes_transferred;
                // Transfiere los últimos V - M votos
                for (int i = candidate_votes[candidate_id]; i < candidate_votes[candidate_id] + votes_transferred; i++) {
                    int voter_id = votes[candidate_id][i];
                    votes[j][candidate_votes[j]] = voter_id;
                    voter_voted[voter_id] = j + 1;
                }
            }
            j++;
        }

        printf("CANDIDATO %d HA SIDO ELEGIDO\n", candidate_id);
        printf("CANDIDATO %d HA RECIBIDO %d VOTOS\n", j - 1, candidate_votes[j - 1]);
    }
}



#endif
