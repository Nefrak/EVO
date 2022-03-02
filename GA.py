from Individual import Individual
from Graph import Graph

#Genetic algorithm
class GA:
    def __init__(self, init_population_size, graph):
        self.population = []
        self.population_size = init_population_size
        self.graph = graph
        self.best_solution = 0

#Fitness of a population
    def calculate_fitness(self):
        pass

#Fitness of a specific individual
    def calculate_specific_fitness(self, population_pos):
        pass

#It will find collisions of a individual
    def find_collisions(self, population_pos):
        pass

#It will find the bests Individuals of the current population
    def find_best_individuals(self, amount):
        pass

#Will generate a new population based on a previous one, using the reproduce method
    def reproduce(self):
        pass

#It will create a new population
    def create_new_population(self):
        pass

#Offspring will be percentage of first parent and rest of second parent
    def cross_percentage(self, parent1, parent2, percentage):
        pass

#Offspring will be from non conflict nodes
    def cross_no_conflict(self):
        pass

#It will mutate random individuals
    def mutate_random(self):
        pass

#It will mutate worst nodes in random individuals
    def mutate_worst_nodes_random(self):
        pass

#It will mutate all individuals
    def mutate_all(self):
        pass

#It will mutate worst nodes in all individuals
    def mutate_worst_nodes_all(self):
        pass

#Print the Chromosome and its colours 
    def print_chromosome(self, population_pos):
        pass