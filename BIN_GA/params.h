//////////////////////////////////////////////////////////////
// VZOROVA IMPLEMENTACE JEDNODUCHEHO GENETICKEHO ALGORITMU  //
// JAZYK C, PREKLAD S OHLEDEM NA NORMU C99                  //
//                                                          //
// (c) MICHAL BIDLO, 2011                                   //
//                                                          //
// POKUD NENI UVEDENO JINAK, TENTO KOD SMI BYT POUZIT       //
// VYHRADNE PRO POTREBY RESENI PROJEKTU V PREDMETECH        //
// BIOLOGII INSPIROVANE POCITACE, PRIPADNE                  //
// APLIKOVANE EVOLUCNI ALGORITMY NA FIT VUT V BRNE.         //
//////////////////////////////////////////////////////////////

#ifndef PARAMS_H
#define PARAMS_H

// pro vypis prubehu evoluce na stdout, jinak zakomentujte
#define DEBUG

//----------------------- parametry genetickeho algoritmu ----------------------
// pravdepodobnost mutace
#define PMUT 4
// pocet mutovanych genu v chromozomu
#define MUTAGENES 4
// pravdepodobnost krizeni
#define PCROSS 70
// pocet jedincu v turnajove selekce
#define TOUR 4
// velikost populace
#define POPSIZE 21
// maximalni pocet generaci
#define GENERATIONS 2000000
// delka chromozomu
#define CHLEN 1000


typedef unsigned int UINT;
typedef int BOOL;

// definice typu chromozomu GA - UPRAVTE SI DLE POTREBY
typedef struct {
    UINT chromosome[CHLEN];  // vlastni napln chromozomu
    UINT fitness;   // fitness daneho jedince
    BOOL evaluate;  // zda je treba znovu vyhodnotit fitness
} GA_chromosome;

// prototypy funkci pro GA
UINT urandom(UINT low, UINT high);

void initialize(GA_chromosome *g);
void crossover(GA_chromosome *parent1, GA_chromosome *parent2, 
                GA_chromosome *child1, GA_chromosome *child2);
BOOL mutator(GA_chromosome *genome, UINT _pmut);
UINT fitness(GA_chromosome *genome);
void gprint(GA_chromosome *genome);
void evolve();
BOOL stop();

#endif
