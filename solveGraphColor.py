from deap import base
from deap import creator
from deap import tools

import random
import numpy

import matplotlib.pyplot as plt
import seaborn as sns
import networkx as nx

import elitism
import graphs

# problem constants:
HARD_CONSTRAINT_PENALTY = 10  # the penalty factor for a hard-constraint violation
FILE_PATH = ""
SHOW_GRAPH = True
SHOW_CONV_STAT = True
SHOW_BOX_STAT = True

# Genetic Algorithm constants:
POPULATION_SIZE = 100
P_CROSSOVER = 0.8  
P_MUTATION = 0.4   
P_M_RANDOM = 0.05    
P_M_SWITCH = 0.2    
P_M_CONFLICT = 0.1  
RUNS = 30
MAX_GENERATIONS = 1000
HALL_OF_FAME_SIZE = 5
MAX_COLORS = 10

# set the random seed:
RANDOM_SEED = 42
random.seed(RANDOM_SEED)

#results
saveFile = 'result2'

toolbox = base.Toolbox()

# create the graph coloring problem instance to be used:
gcp = graphs.GraphColoringProblem(FILE_PATH, nx.mycielski_graph(5), HARD_CONSTRAINT_PENALTY)

# define a single objective, maximizing fitness strategy:
creator.create("FitnessMin", base.Fitness, weights=(-1.0,))

# create the Individual class based on list:
creator.create("Individual", list, fitness=creator.FitnessMin)

# create an operator that randomly returns an integer in the range of participating colors:
toolbox.register("Integers", random.randint, 0, MAX_COLORS - 1)

# create the individual operator to fill up an Individual instance:
toolbox.register("individualCreator", tools.initRepeat, creator.Individual, toolbox.Integers, len(gcp))

# create the population operator to generate a list of individuals:
toolbox.register("populationCreator", tools.initRepeat, list, toolbox.individualCreator)

# fitness calculation: cost of the suggested olution
def getCost(individual):
    return gcp.getCost(individual),  # return a tuple

# do all mutations with probability
def allMutations(individual, mutateSwitchChance, mutateConflictChance,  mutateRandomChance):
    for index in range(len(individual)):
        prob = random.random()
        if prob < mutateRandomChance:
            individual = mutateRandomNodes(index, individual, 0, MAX_COLORS - 1)
        if prob < mutateSwitchChance:
            individual = mutateConflictNodes(index, individual, 0, MAX_COLORS - 1)
        if prob < mutateConflictChance:
            individual = mutateSwitchConflictNodes(index, individual)
    return individual

# mutating random nodes
def mutateRandomNodes(index, individual, low, up):
    individual[index] = random.randrange(low, up + 1)
    return individual

# mutating nodes with conflicts
def mutateConflictNodes(index, individual, low, up):
    if gcp.isInViolation(index, individual) != -1:
        individual[index] = random.randrange(low, up + 1)
    return individual

# mutating node by switching it with another conflict node
def mutateSwitchConflictNodes(index, individual):
    confIndex = gcp.isInViolation(index, individual)
    if confIndex != -1:
        savedColor = individual[index]
        individual[index] = individual[confIndex]
        individual[confIndex] = savedColor
    return individual

# cross nodes so that nodes with conflict are replaced from other parent
def crossNoConflict(ind1, ind2):
    newind1 = ind1.copy()
    newind2 = ind2.copy()
    for index in range(len(ind1)):
        if gcp.isInViolation(index, ind1) != -1:
            ind1[index] = ind2[index]
            ind2[index] = newind1[index]
    for index in range(len(ind2)):
        if gcp.isInViolation(index, ind2) != -1:
            ind2[index] = ind1[index]
            ind1[index] = newind2[index]
    return (ind1, ind2)

toolbox.register("evaluate", getCost)

# genetic operators:
toolbox.register("select", tools.selTournament, tournsize=2)
#toolbox.register("select", tools.selBest)

#toolbox.register("mate", tools.cxOnePoint)
toolbox.register("mate", tools.cxTwoPoint)
#toolbox.register("mate", crossNoConflict)

toolbox.register("mutate", allMutations, mutateSwitchChance=P_M_SWITCH, mutateConflictChance=P_M_CONFLICT, mutateRandomChance=P_M_RANDOM)

# show statistics
def showConv(logbooks, runs):
    # extract statistics:
    minAvg = numpy.array(logbooks[0].select("min"))
    meanAvg = numpy.array(logbooks[0].select("avg"))
    maxAvg = numpy.array(logbooks[0].select("max"))
    for i in range(1, runs):
        minAvg = minAvg + numpy.array(logbooks[i].select("min"))
        meanAvg = meanAvg + numpy.array(logbooks[i].select("avg"))
        maxAvg = maxAvg + numpy.array(logbooks[i].select("max"))
    minFitnessValues = minAvg / runs
    meanFitnessValues = meanAvg / runs
    maxFitnessValues = maxAvg / runs

    plt.figure(1)
    sns.set_style("whitegrid")
    x = numpy.arange(0, MAX_GENERATIONS + 1)
    y = [minFitnessValues, meanFitnessValues, maxFitnessValues]
    plt.xscale('log')
    plt.plot(x,y[1],color='C0')
    plt.fill_between(x,y[0],y[2], color='C0',alpha=0.4)
    plt.axhline(minFitnessValues[-1], color="black", linestyle="--")
    plt.xlabel('Pocet evaluaci')
    plt.ylabel('Fitness')
    plt.title('Konvergencni krivka')

# show boxplot
def showBox(logbooks, runs):
    lastEval = [logbooks[0].select("avg")[-1]]
    lastEvalMin = [logbooks[0].select("min")[-1]]
    for i in range(1, runs):
       lastEval.append(logbooks[i].select("avg")[-1])
       lastEvalMin.append(logbooks[i].select("min")[-1])

    numpy.save(saveFile, numpy.array(lastEvalMin))

    plt.figure(2)
    plt.boxplot([lastEval, lastEvalMin], labels=['avg', 'min'])
    plt.xlabel('Fitness')
    plt.ylabel('Outputs')
    plt.title('Boxplot')

# get results
def printResult(hofs, logbooks, runs):
    # print info for best solution found:
    best = hofs[0].items[0]
    for hof in hofs:
        if best.fitness.values[0] > hof.items[0].fitness.values[0]:
            best = hof.items[0]

    print("-- Best Individual = ", best)
    print("-- Best Fitness = ", best.fitness.values[0])
    print("number of colors = ", gcp.getNumberOfColors(best))
    print("Number of violations = ", gcp.getViolationsCount(best))
    print("Cost = ", gcp.getCost(best))

    # plot statistics:
    if SHOW_CONV_STAT:
        showConv(logbooks, runs)
             
    if SHOW_BOX_STAT:
        showBox(logbooks, runs)

    # plot best solution:
    if SHOW_GRAPH:
        plt.figure(3)
        gcp.plotGraph(best)

    plt.show()

# Genetic Algorithm flow:
def main():
    # prepare the statistics object:
    stats = tools.Statistics(lambda ind: ind.fitness.values)
    stats.register("min", numpy.min)
    stats.register("avg", numpy.mean)
    stats.register("max", numpy.max)

    # define the hall-of-fame object:
    hofs = []
    for _ in range(RUNS):
        hofs.append(tools.HallOfFame(HALL_OF_FAME_SIZE))

    # perform the Genetic Algorithm flow with elitism:
    logbooks = elitism.eaSimpleWithElitism(POPULATION_SIZE, toolbox, cxpb=P_CROSSOVER, mutpb=P_MUTATION, runs=RUNS,
                                              ngen=MAX_GENERATIONS, stats=stats, halloffame=hofs, verbose=False)

    printResult(hofs, logbooks, RUNS)


if __name__ == "__main__":
    main()
