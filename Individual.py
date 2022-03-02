#Individual chromosome of population
class Individual:
    def __init__(self, chromosome):
        self.chromosome = chromosome
        self.number_of_colors = 0
        self.fitness = 0

#Numer of differents colours used in this result
    def get_num_Of_colours(self):
        pass

#Assign a value to the fitness
    def set_fitness(self):
        pass

#Return the fitness of the individual
    def get_fitness(self):
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