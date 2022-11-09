#include <bits/stdc++.h>
#include <iostream>
#define weight first
#define value second
using namespace std;

vector<pair<int, int>> weights;
// pair of chromosome and it's fitness
vector<pair<vector<int>, int>> currentGeneration;
int knapscakWeight, itemsNum;
const int POP_SIZE = 20, MAX_GENERATION = 20;
const float PC = 0.6, PM = 0.1;

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

void RouletteWheelSelection(){
    int totalFitness = 0;
    for (int i = 0; i < POP_SIZE; ++i) {
        totalFitness += currentGeneration[i].second;
    }

}

pair<vector<int>, vector<int>> crossover(vector<int> parent1, vector<int> parent2){
    int r1 = 1 + (rand() % itemsNum - 1);
    vector<int> offspring1(itemsNum), offspring2(itemsNum);
    for (int i = 0; i <= r1; ++i) {
        offspring1[i] = parent1[i];
    }
    for (int i = r1+1; i < itemsNum; ++i) {
        offspring1[i] = parent2[i];
    }

    for (int i = 0; i <= r1; ++i) {
        offspring1[i] = parent2[i];
    }
    for (int i = r1+1; i < itemsNum; ++i) {
        offspring1[i] = parent1[i];
    }
    return {offspring1, offspring2};
}

void Mutation(vector<int> offspring){
    for (int i = 0; i < itemsNum; ++i) {
        // generating a random float between 1 and 0
        float ri = (float)rand() / (float)RAND_MAX;
        if(ri <= PM){
            offspring[i] = ~offspring[i];
        }
    }
}

void GA(){
    cin >> knapscakWeight >> itemsNum;
    weights.resize(itemsNum);
    for (int i = 0; i < itemsNum; ++i) {
        cin >> weights[i].weight >> weights[i].value;
    }
    currentGeneration = InitializePopulation();
    for (int i = 1; i < MAX_GENERATION; ++i) {
        evaluateFitness();
        RouletteWheelSelection();
    }
}

int main() {
    freopen("input.txt", "r", stdin);
    int t;
    cin >> t;
    while(t--){
        GA();
    }
}