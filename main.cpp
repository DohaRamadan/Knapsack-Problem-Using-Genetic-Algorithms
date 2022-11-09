#include <bits/stdc++.h>
#include <iostream>
#define weight first
#define value second
using namespace std;

vector<pair<int, int>> weights;
int knapscakWeight, itemsNum;

void GA(){
    cin >> knapscakWeight >> itemsNum;
    weights.resize(itemsNum);
    for (int i = 0; i < itemsNum; ++i) {
        cin >> weights[i].weight >> weights[i].value;
    }

}

int main() {
    int t;
    cin >> t;
    while(t--){
        GA();
    }
}