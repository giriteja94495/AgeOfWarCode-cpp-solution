#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

bool get_advantage(const string& my_clan, const string& op_clan) {
    unordered_map< string, vector<string> > advantages = {
        {"Militia", {"Spearmen", "LightCavalry"}},
        {"Spearmen", {"LightCavalry", "HeavyCavalry"}},
        {"LightCavalry", {"FootArcher", "CavalryArcher"}},
        {"HeavyCavalry", {"Militia", "FootArcher", "LightCavalry"}},
        {"CavalryArcher", {"Spearmen", "HeavyCavalry"}},
        {"FootArcher", {"Militia", "CavalryArcher"}}
    };
    auto clan_advantages = advantages[my_clan];
    for (auto advantage : clan_advantages) {
        if (advantage == op_clan) {
            return true;
        }
    }
    return false;
}

string battle_result(int my_count, int opp_count, bool adv) {
    int doubleOfOpponetSolideir = opp_count * 2;
    if (adv) {
        return (my_count > doubleOfOpponetSolideir) ? "You Win" : "Draw";
    } else {
        return (my_count < opp_count) ? "You Lose" : "Draw";
    }
}

vector<string> plan_attack(const vector<string>& myPlatoons, const vector<string>& enemyPlatoons) {
    vector< pair<string, int> > results;

    for (const string& my_platoon : myPlatoons) {
        size_t pos = my_platoon.find('#');
        string platoon_class = my_platoon.substr(0, pos);
        int my_soldiers = stoi(my_platoon.substr(pos + 1));

        int possible_wins = 0;

        for (const string& enemy_platoon : enemyPlatoons) {
            size_t pos = enemy_platoon.find('#');
            string enemy_class = enemy_platoon.substr(0, pos);
            int enemy_soldiers = stoi(enemy_platoon.substr(pos + 1));

            bool advantage = get_advantage(platoon_class, enemy_class);
            string result = battle_result(my_soldiers, enemy_soldiers, advantage);

            if (result == "You Win") {
                possible_wins++;
            }
        }

        results.push_back(make_pair(my_platoon, possible_wins));
    }

    sort(results.begin(), results.end(), [](const pair<string, int>& a, const pair<string, int>& b) {
        return a.second > b.second;
    });

    vector<string> top_results;
    for (const auto& platoon : results) {
        top_results.push_back(platoon.first);
    }

    int num_results = min(5, (int)results.size());
    return vector<string>(top_results.begin(), top_results.begin() + num_results);
}

int main() {
    string myPlatoonsInput;
    getline(cin, myPlatoonsInput);
    string opponentPlatoonsInput;
    getline(cin, opponentPlatoonsInput);
    
    vector<string> myPlatoons;
    vector<string> opponentPlatoons;
    
    size_t pos = 0;
    while ((pos = myPlatoonsInput.find(";")) != string::npos) {
        string platoon = myPlatoonsInput.substr(0, pos);
        myPlatoons.push_back(platoon);
        myPlatoonsInput.erase(0, pos + 1);
    }
    if (!myPlatoonsInput.empty()) {
        myPlatoons.push_back(myPlatoonsInput);
    }
    
    pos = 0;
    while ((pos = opponentPlatoonsInput.find(";")) != string::npos) {
        string platoon = opponentPlatoonsInput.substr(0, pos);
        opponentPlatoons.push_back(platoon);
        opponentPlatoonsInput.erase(0, pos + 1);
    }
    if (!opponentPlatoonsInput.empty()) {
        opponentPlatoons.push_back(opponentPlatoonsInput);
    }
    vector<string> result = plan_attack(myPlatoons, opponentPlatoons);

    if (result.size() < 3) {
        cout << "There is no chance of winning" << endl;
    } else {
        bool first = true; 
        for (const string& platoon : result) {
            if (!first) {
                cout << ";"; 
            } else {
                first = false; 
            }
            cout << platoon; 
        }
        cout << endl; 
    }
    return 0;
}
