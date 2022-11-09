#include <bits/stdc++.h>
#include <iostream>
#define weight first
#define value second
using namespace std;

vector<pair<int, int>> weights;
// pair of chromosome and it's fitness
vector<pair<vector<int>, int>> currentGeneration, offsprings;
int knapscakWeight, itemsNum;
const int POP_SIZE = 10, MAX_GENERATION = 20;
const float PC = 0.6, PM = 0.1;

bool sortByFitness(pair<vector<int>, int>& a, pair<vector<int>, int>& b){
    return a.second > b.second;
}

vector<pair<vector<int>, int>> InitializePopulation(){
    vector<pair<vector<int>, int>> newGeneration;
    while(newGeneration.size() < POP_SIZE){
        vector<int> chromosome(itemsNum);
        int weightSum = 0;
        for (int i = 0; i < weights.size(); ++i) {
            // generating a random float between 1 and 0
            float r = (float)rand() / (float)RAND_MAX;
            if(r <= 0.5){
                chromosome[i] = 0;
            }else{
                chromosome[i] = 1;
                weightSum += weights[i].weight;
            }
        }
        if(weightSum <= knapscakWeight){
            newGeneration.push_back({chromosome, -1});
        }
    }
    return newGeneration;
}

void evaluateFitness(){
    for (int i = 0; i < POP_SIZE; ++i) {
        int totalValue = 0, totalWeight = 0;
        for (int j = 0; j < itemsNum; ++j) {
            if(currentGeneration[i].first[j]){
                totalWeight += weights[j].weight;
                totalValue += weights[j].value;
            }
        }
        if(totalWeight > knapscakWeight){
            currentGeneration[i].second = 0;
        }else{
            currentGeneration[i].second = totalValue;
        }
    }
}

pair<vector<int>, vector<int>> crossover(vector<int> parent1, vector<int> parent2){
    int r1 = 1 + (rand() % itemsNum - 1);
    vector<int> offspring1(itemsNum), offspring2(itemsNum);
    for (int i = 0; i <= r1; ++i) {
        offspring1[i] = parent1[i];
        offspring2[i] = parent2[i];
    }
    for (int i = r1+1; i < itemsNum; ++i) {
        offspring1[i] = parent2[i];
        offspring2[i] = parent1[i];
    }
    return {offspring1, offspring2};
}

void Mutation(){
    for(int i=0; i<POP_SIZE; i++){
        for (int j = 0; j < itemsNum; ++j) {
            // generating a random float between 1 and 0
            float ri = (float)rand() / (float)RAND_MAX;
            if(ri <= PM){
                if(offsprings[i].first[j]) offsprings[i].first[j] = 0;
                else offsprings[i].first[j] = 1;
            }
        }
    }
}


void RouletteWheelSelection(){
    int totalFitness = 0;
    vector<int> cumlative_fitness(POP_SIZE);
    for (int i = 0; i < POP_SIZE; ++i) {
        totalFitness += currentGeneration[i].second;
        cumlative_fitness[i] = totalFitness;
    }
    offsprings.clear();
    while (offsprings.size() < POP_SIZE) {
        int r1 =  (rand() % (totalFitness + 1));
        int r2 =  (rand() % (totalFitness + 1));
        int parent1 = lower_bound(cumlative_fitness.begin(), cumlative_fitness.end(), r1) - cumlative_fitness.begin();
        int parent2 = lower_bound(cumlative_fitness.begin(), cumlative_fitness.end(), r2) - cumlative_fitness.begin();
        // generating a random float between 1 and 0
        float r = (float)rand() / (float)RAND_MAX;
        if(r <= PC){
            pair<vector<int>, vector<int>> springs = crossover(currentGeneration[parent1].first, currentGeneration[parent2].first);
            offsprings.push_back({springs.first, -1});
            offsprings.push_back({springs.second, -1});
        }else{
            offsprings.push_back(currentGeneration[parent1]);
            offsprings.push_back(currentGeneration[parent2]);
        }
    }
}


pair<vector<int>, int> GA(){
    cin >> knapscakWeight >> itemsNum;
    weights.resize(itemsNum);
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
    std::sort(currentGeneration.begin(), currentGeneration.end(), sortByFitness);
    pair<vector<int>, int> bestIndividual = currentGeneration[0];
    return bestIndividual;
}

int main() {
    freopen("input.txt", "r", stdin);
    int t;
    cin >> t;
    for (int i = 0; i < t; ++i) {
        pair<vector<int>, int> bestIndividual = GA();
        int selectedItems = 0;
        for (int j = 0; j < bestIndividual.first.size(); ++j) {
            if(bestIndividual.first[j] == 1){
                selectedItems++;
            }
        }
        cout << "test case " << i+1 << ": " << "number of selected items: " << selectedItems << ", total value: " << bestIndividual.second << "\n";
        cout << "weight and value of each selected item: \n";
        for (int j = 0; j < bestIndividual.first.size(); ++j) {
            if(bestIndividual.first[j] == 1){
                cout << weights[j].weight << " " << weights[j].value << "\n";
            }
        }
    }
}