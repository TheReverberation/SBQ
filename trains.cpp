#include <algorithm>
#include <cassert>
#include <iostream>
#include <map>
#include <vector>
#include <set>

using std::cin;
using std::cout;
using std::map;
using std::vector;
using std::set;
using std::sort;


struct Train {
    uint32_t begin, end;
    uint32_t id, cost;
};


std::istream& operator>>(std::istream& in, Train& train) {
    return in >> train.begin >> train.end >> train.id >> train.cost;
}

std::ostream& operator<<(std::ostream& out, Train& train) {
    return out << train.begin << " " << train.end << " " << train.id << train.cost;
}

/* 
 * Returns sotred used strains ids
 */
vector<uint32_t> maximazeCost(vector<Train> trains) {
    // discretisation
    set<uint32_t> times;

    for (auto& train : trains) {
        times.insert(train.begin);
        times.insert(train.end);
    } 

    map<uint32_t, uint32_t> timeId;
    uint32_t id = 0;
    for (uint32_t time : times) {
        timeId[time] = id++;
    }
    for (auto& train : trains) {
        train.begin = timeId[train.begin];
        train.end = timeId[train.end];
    }

    vector<vector<Train>> groupsByEnd(times.size());
    for (auto& train : trains) {
        groupsByEnd[train.end].push_back(train);
    }

    // for recover ans
    // used train id for ith time
    // -1 identify empty 
    vector<int32_t> used(times.size(), -1);
    vector<int32_t> lastTime(times.size(), -1);

    vector<uint32_t> dp(times.size());
    dp[0] = 0;
    for (size_t i = 0; i < times.size(); ++i) {
        if (i > 0) {
            // first time we don`t use some trains
            dp[i] = dp[i - 1];
            lastTime[i] = i - 1;
        }
        for (Train& train : groupsByEnd[i]) {
            if (dp[train.begin] + train.cost > dp[i]) {
                dp[i] = dp[train.begin] + train.cost;
                lastTime[i] = train.begin;
                used[i] = train.id;
            }
        }
    }

    // recover trains ids
    vector<uint32_t> usedTrain;
    for (int32_t time = times.size() - 1; time != -1;) {
        if (used[time] != -1) {
            usedTrain.push_back(used[time]);
        }
        time = lastTime[time];
    }
    sort(usedTrain.begin(), usedTrain.end());
    return usedTrain;
}


void test() {
    vector<Train> trains;
    trains = {
        {0, 5, 1, 2},
        {3, 7, 2, 5},
        {6, 9, 3, 3},
        {6, 7, 4, 3},
        {7, 9, 5, 1}
    };
    assert(maximazeCost(trains) == vector<uint32_t>({2, 5}));


    trains = {
        {0, 3, 1, 2},
        {2, 7, 2, 2},
        {4, 6, 3, 1},
        {8, 10, 4, 1}
    };
    assert(maximazeCost(trains) == vector<uint32_t>({1, 3, 4}));

    trains = {
        {10, 100, 1, 100}
    };
    assert(maximazeCost(trains) == vector<uint32_t>({1}));

    trains = {
        {0, 4, 1, 2},
        {2, 6, 2, 4},
        {4, 8, 3, 1}
    };
    assert(maximazeCost(trains) == vector<uint32_t>({2}));

    // add your test here
}


int main() {
    test();
    return 0;
}
