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

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "params.h"

// jednotky pro specifikaci pravdepodobnosti genetickych operatoru:
// 100 - procenta, 1000 - promile. NENI TREBA MENIT
const UINT unit = 100;
// delka chromozomu - s touto promennou pracuje GA
const UINT glength = CHLEN;
// maximalni fitness - zde odpovida delce jedince, protoze se pouze snazime
// nalezt retezec se samymi 1 geny
const UINT max_fit = CHLEN;

#ifdef DEBUG
const UINT generations = 0; // 0 - pocet generaci neni pri ladeni omezen
#else
const UINT generations = GENERATIONS;   // po tomto poctu je GA zastaven
#endif

// !!!! nas GA pracuje pouze se sudymi pocty jedincu v populaci !!!!
UINT _popsize = (POPSIZE & 1) ? POPSIZE + 1 : POPSIZE;

// ------------------- implementace genetickeho algoritmu ----------------------
// *****************************************************************************

GA_chromosome best;	// njelepsi dosud nalezene reseni
UINT best_ever; // fitness dosud nejlepsiho jedince

UINT generation;    // pocitadlo generaci
GA_chromosome *population;
GA_chromosome *next_population;
// pracovni populace - pouze sudy pocet jedincu !!!!
GA_chromosome pool1[(POPSIZE & 1) ? POPSIZE + 1 : POPSIZE];
GA_chromosome pool2[(POPSIZE & 1) ? POPSIZE + 1 : POPSIZE];

// evolucni cyklus SGA: nova populace o _popsize jedincich je tvorena krizenimi
// a mutaci turnajem vybranych jedincu predchazejici generace. 
void evolve()
{
    // inicializace promennych
    generation = 0;
    best.fitness = 0;
    best_ever = 0;
    GA_chromosome ind1_new, ind2_new;
    UINT _tour = (TOUR >= 2 ? TOUR : 2);
    UINT i1;

    // inicializace populace
    for (UINT i = 0; i < _popsize; i++)
    {
        initialize(&pool1[i]);
        pool1[i].evaluate = 1;
    }
    // evolucni cyklus
    do
    {
        generation++;
        if (generation & 1)
        {
            population = pool1;
            next_population = pool2;
        }
        else
        {
            population = pool2;
            next_population = pool1;
        }
        // ohodnoceni populace
        for (UINT i = 0; i < _popsize; i++)
        {
            if (population[i].evaluate)
            {
                population[i].fitness = fitness(&population[i]);
                if (population[i].fitness >= best.fitness)
                    best = population[i];
                population[i].evaluate = 0;
            }
        }
        // elitizmus
        next_population[0] = best;  // dosud nejlepsi nalezeny jedinec...
        GA_chromosome mutant = best; mutator(&mutant, unit);
        next_population[1] = mutant;    // ...a mutant nejlepsiho
        // tvorba nove populace
        for (UINT i = 2; i < _popsize; i += 2)
        {
            GA_chromosome *ind1 = NULL, *ind2 = NULL;
            // turnajovy vyber jedincu
            for (UINT t = 0; t < _tour; t++)
            {
                i1 = urandom(0, _popsize - 1);
                if (ind1 == NULL) ind1 = &population[i1];
                else if (ind2 == NULL) ind2 = &population[i1];
                else if (population[i1].fitness > ind1->fitness)
                    ind1 = &population[i1];
                else if (population[i1].fitness > ind2->fitness)
                    ind2 = &population[i1];
            }
            // krizeni
            if (urandom(0, unit) < PCROSS)
            {
                crossover(ind1, ind2, &ind1_new, &ind2_new);
                ind1_new.evaluate = 1;
                ind2_new.evaluate = 1;
            }
            else    // prechod jedincu bez krizeni
            {
                ind1_new = *ind1;
                ind2_new = *ind2;
            }
            // mutace
            if (mutator(&ind1_new, PMUT)) ind1_new.evaluate = 1;
            if (mutator(&ind2_new, PMUT)) ind2_new.evaluate = 1;
            // umisteni potomku do nove populace
            next_population[i] = ind1_new;
            next_population[i + 1] = ind2_new;
        }
    } while (!stop());
}

// *****************************************************************************
// --------------- geneticke operatory a podpurne funkce pro GA ----------------

// generuje cele cislo v rozsahu low-high vcetne
UINT urandom(UINT low, UINT high)
{
    return rand() % (high - low + 1) + low;
}

// vypis chromozomu - ZMENTE SI DLE POTREBY
void gprint(GA_chromosome *genome)
{
    for (UINT i = 0; i < glength; i++)
        printf("%d ", genome->chromosome[i]);
    putchar('\n');
}

// inicializace populace nahodne - ZMENTE SI DLE POTREBY
void initialize(GA_chromosome *genome)
{
    for (UINT i = 0; i < glength; i++)
        genome->chromosome[i] = urandom(0, 1);
}

// krizeni - ZMENTE SI DLE POTREBY
void crossover(GA_chromosome *parent1, GA_chromosome *parent2,
                GA_chromosome *child1, GA_chromosome *child2)
{
    // zde standardni jednobodove krizeni
    UINT cpoint = urandom(1, glength - 1);

    for (UINT i = 0; i < glength; i++)
    {
        if (i < cpoint)
        {
            child1->chromosome[i] = parent1->chromosome[i];
            child2->chromosome[i] = parent2->chromosome[i];
        }
        else
        {
            child1->chromosome[i] = parent2->chromosome[i];
            child2->chromosome[i] = parent1->chromosome[i];
        }
    }
}

// mutace - ZMENTE SI DLE POTREBY. je vsak treba zachovat navratovou hodnotu!
BOOL mutator(GA_chromosome *genome, UINT _pmut)
{
    if (urandom(0, unit) <= _pmut)  // mutace s pravdepodobnosti _pmut
    {
        for (UINT i = 0; i < MUTAGENES; i++)
        {
            UINT g = urandom(0, glength - 1);
            genome->chromosome[g] = 1 - genome->chromosome[g]; // inverze genu
        }

        return 1;   // probehla-li mutace, vratim true...
    }

    return 0;   // ...jinak vracim false
}

// test na zastaveni evoluce - V PRIPADE POTREBY ZMENTE
BOOL stop()
{
    if (best.fitness > best_ever)
    {
        best_ever = best.fitness;
#ifdef DEBUG
        printf("Fitness = %d in generation %d\n", 
                        best_ever,      generation);
//        gprint(&best);
#endif
    }

    if (best_ever == max_fit || (generations > 0 && generation == generations))
    {
        if (best_ever == max_fit)
        {
            printf("success; generation=%d\n", generation);
            gprint(&best);
        }
        else printf("failed; generation=%d, fitness=%d\n", generation, best_ever);
        return 1;
    }

    return 0;
}

// evaluace fitness pro zadaneho jedince - ZMENTE PRO RESENI SVEHO PROBLEMU
UINT fitness(GA_chromosome *genome)
{
    UINT fit = 0;
    // zde je cilem pouze nalezt jedicne se samymi 1 geny

    for (UINT i = 0; i < glength; i++)
        if (genome->chromosome[i] == 1)
            fit++;

    return fit;
}

// *****************************************************************************
// ------------------------------ hlavni program -------------------------------

int main(int argc, char *argv[])
{
    srand(time(0)); // random seed - NUTNE
    evolve();

    return 0;
}
