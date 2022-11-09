#include <bits/stdc++.h>
#include <iostream>
#define weight first
#define value second
using namespace std;


struct Individual{
    vector<int> chromosome;
    int fitness;
    int totalWeight=0, totalValue = 0;
};

bool sortByFitness(Individual& a, Individual& b){
    return a.fitness > b.fitness;
}

vector<pair<int, int>> weights;
vector<Individual> currentGeneration, offsprings;
int knapscakWeight, itemsNum;
const int POP_SIZE = 100, MAX_GENERATION = 1000;
const float PC = 0.6, PM = 0.1;

vector<Individual> InitializePopulation(){
    vector<Individual> newGeneration;
    while(newGeneration.size() < POP_SIZE){
        Individual v;
        v.chromosome.resize(itemsNum);
        int weightSum = 0;
        for (int i = 0; i < weights.size(); ++i) {
            // generating a random float between 1 and 0
            float r = (float)rand() / (float)RAND_MAX;
            if(r <= 0.5){
                v.chromosome[i] = 0;
            }else{
                v.chromosome[i] = 1;
                v.totalValue += weights[i].value;
                v.totalWeight += weights[i].weight;
            }
        }
        if(v.totalWeight <= knapscakWeight){
            newGeneration.push_back(v);
        }
    }
    return newGeneration;
}

void evaluateFitness(){
    for (int i = 0; i < POP_SIZE; ++i) {
        if(currentGeneration[i].totalWeight <= knapscakWeight){
            currentGeneration[i].fitness = currentGeneration[i].totalValue;
        }else{
            currentGeneration[i].fitness = 0;
        }
    }

}
pair<Individual, Individual> crossover(Individual parent1, Individual parent2){
    int r1 = 1 + (rand() % itemsNum - 1);
    Individual offspring1, offspring2;
    offspring1.chromosome.resize(itemsNum);
    offspring2.chromosome.resize(itemsNum);
    for (int i = 0; i <= r1; ++i) {
        offspring1.chromosome[i] = parent1.chromosome[i];
        offspring2.chromosome[i] = parent2.chromosome[i];
        if(offspring1.chromosome[i]){
            offspring1.totalWeight += weights[i].weight;
            offspring1.totalValue  += weights[i].value;
        }
        if(offspring2.chromosome[i]){
            offspring2.totalWeight += weights[i].weight;
            offspring2.totalValue  += weights[i].value;
        }
    }
    for (int i = r1+1; i < itemsNum; ++i) {
        offspring1.chromosome[i] = parent2.chromosome[i];
        offspring2.chromosome[i] = parent1.chromosome[i];
        if(offspring1.chromosome[i]){
            offspring1.totalWeight += weights[i].weight;
            offspring1.totalValue  += weights[i].value;
        }
        if(offspring2.chromosome[i]){
            offspring2.totalWeight += weights[i].weight;
            offspring2.totalValue  += weights[i].value;
        }
    }
    return {offspring1, offspring2};
}

void Mutation(){
    for(int i=0; i<POP_SIZE; i++){
        for (int j = 0; j < itemsNum; ++j) {
            // generating a random float between 1 and 0
            float ri = (float)rand() / (float)RAND_MAX;
            if(ri <= PM){
                if(offsprings[i].chromosome[j]){
                    offsprings[i].chromosome[j] = 0;
                    offsprings[i].totalWeight -= weights[j].weight;
                    offsprings[i].totalValue -= weights[j].value;
                } else{
                    offsprings[i].chromosome[j] = 1;
                    offsprings[i].totalWeight += weights[j].weight;
                    offsprings[i].totalValue += weights[j].value;
                }
            }
        }
    }
}


void RouletteWheelSelection(){
    int totalFitness = 0;
    vector<int> cumulative_fitness(POP_SIZE);
    for (int i = 0; i < POP_SIZE; ++i) {
        totalFitness += currentGeneration[i].fitness;
        cumulative_fitness[i] = totalFitness;
    }
    offsprings.clear();
    while (offsprings.size() < POP_SIZE) {
        int r1 =  (rand() % (totalFitness + 1));
        int r2 =  (rand() % (totalFitness + 1));
        int parent1 = lower_bound(cumulative_fitness.begin(), cumulative_fitness.end(), r1) - cumulative_fitness.begin();
        int parent2 = lower_bound(cumulative_fitness.begin(), cumulative_fitness.end(), r2) - cumulative_fitness.begin();
        // generating a random float between 1 and 0
        float r = (float)rand() / (float)RAND_MAX;
        if(r <= PC){
            pair<Individual, Individual> springs = crossover(currentGeneration[parent1], currentGeneration[parent2]);
            offsprings.push_back(springs.first);
            offsprings.push_back(springs.second);
        }else{
            offsprings.push_back(currentGeneration[parent1]);
            offsprings.push_back(currentGeneration[parent2]);
        }
    }
}

Individual GA(){
    cin >> knapscakWeight >> itemsNum;
    weights.resize(itemsNum);
    currentGeneration.resize(POP_SIZE);
    for (int i = 0; i < itemsNum; ++i) {
        cin >> weights[i].weight >> weights[i].value;
    }
    currentGeneration = InitializePopulation();
    for (int i = 1; i < MAX_GENERATION; ++i) {
        evaluateFitness();
        RouletteWheelSelection();
        Mutation();
        currentGeneration = offsprings;
    }
    evaluateFitness();
    std::sort(currentGeneration.begin(), currentGeneration.end(), sortByFitness);
    Individual bestIndividual = currentGeneration[0];
    return bestIndividual;
}

int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    int t;
    cin >> t;
    for (int i = 0; i < t; ++i) {
        Individual bestIndividual = GA();
        int selectedItems = 0;
        for (int j = 0; j < bestIndividual.chromosome.size(); ++j) {
            if(bestIndividual.chromosome[j] == 1){
                selectedItems++;
            }
        }
        cout << "test case " << i+1 << ": " << "number of selected items: " << selectedItems << ", total value: " << bestIndividual.totalValue << "\n";
        cout << "weight and value of each selected item: \n";
        for (int j = 0; j < bestIndividual.chromosome.size(); ++j) {
            if(bestIndividual.chromosome[j] == 1){
                cout << weights[j].weight << " " << weights[j].value << "\n";
            }
        }
//        cout << bestIndividual.totalValue << "\n";
    }
}