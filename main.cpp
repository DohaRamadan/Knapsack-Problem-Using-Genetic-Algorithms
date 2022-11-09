#include <bits/stdc++.h>
#include <iostream>
#define weight first
#define value second
using namespace std;

vector<pair<int, int>> weights;
int knapscakWeight, itemsNum;
const int POPSIZE = 30, MAXGENERATION = 20;

vector<pair<vector<int>, int>> InitializePopulation(){
    vector<pair<vector<int>, int>> newGeneration;
    while(newGeneration.size() < POPSIZE){
        vector<int> chromosome(itemsNum);
        int weightSum = 0;
        for (int i = 0; i < weights.size(); ++i) {
            // generating a random double between 1 and 0
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

void evaluateFitness(vector<pair<vector<int>, int>> generation){
    for (int i = 0; i < POPSIZE; ++i) {
        int totalValue = 0, totalWeight = 0;
        for (int j = 0; j < itemsNum; ++j) {
            if(generation[i].first[j]){
                totalWeight += weights[j].weight;
                totalValue += weights[j].value;
            }
        }
        if(totalWeight > knapscakWeight){
            generation[i].second = 0;
        }else{
            generation[i].second = totalValue;
        }
    }
}

void GA(){
    cin >> knapscakWeight >> itemsNum;
    weights.resize(itemsNum);
    for (int i = 0; i < itemsNum; ++i) {
        cin >> weights[i].weight >> weights[i].value;
    }
    vector<pair<vector<int>, int>> generation = InitializePopulation();
    for (int i = 1; i < MAXGENERATION; ++i) {
        evaluateFitness(generation);
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