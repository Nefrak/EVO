# BIN - Optimalizace problému barvení grafu genetickým algoritmem

## Zadání
Problémem barvení grafu (Graph Coloring Problem) je problém jak obarvit množinu vrcholů spojených hranami co nejmenším počtem barev tak, aby žádné dva sousední vrcholy neměly stejnou barvu. Tento problém je NP-úplný (neexistuje způsob jak efektivně najít jeho optimální řešení) a je proto úlohou vhodnou pro optimalizaci pomocí evolučních algoritmů.

Pro zvolený problém barvení grafu proveďte sady experimentů využívajících různých variant genetických operátorů (selekce, křížení atd.).

## Knihovny
- deap
- random
- numpy
- collections
- matplotlib
- seaborn
- networkx
- yaml

## Spuštění
python solveGraphColor.py

## Vstup
Graf nad kterým chcete algoritmus spustit je uložen v graph.yaml. Jméno vstupního souboru je možno změnit pomocí FILE_PATH.
Pokud není zadán soubor je možné graph nahrát pomocí networkx, které obsahuje spoustu grafů na experimenty.

## Výstup
Výsledek je fitness a počet konfliktů. Výsledný graf je vyplocen, pokud je parametr SHOW_GRAPH nastaven na True.

## Parametry
Parametry je možné upravit v solveGraphColor.py
- velikost populace: POPULATION_SIZE
- šance křížení: P_CROSSOVER
- šance provedení mutace na jednotlivci: P_MUTATION
    - šance mutace nody na náhodnou hodnotu: P_M_RANDOM
    - šance prohození s konfliktní nodou: P_M_SWITCH
    - šance mutace pokud je konfliktní: P_M_CONFLICT
- počet běhů: RUNS
- generace: MAX_GENERATIONS
- počet nejlepších jedinců z předchozí generace: HALL_OF_FAME_SIZE
- maximální počet barev: MAX_COLORS
- seed: RANDOM_SEED

## Autor
Jaromír Franěk (xfrane16)

## Převzatý kód
https://github.com/PacktPublishing/Hands-On-Genetic-Algorithms-with-Python/tree/master/Chapter05