#include "project.cpp"
#include <cassert>
#include <iostream>
#include <vector>
using namespace std;

void print_player(const string &label, const player_data &p) {
    cout << label << " -> Score: " << p.score 
         << " PlayerID: " << p.player_id 
         << " FinishTime: " << p.finish_time << endl;
}

int main() {
    cout << "=== Time-Range Leaderboard Query System: Extended Tests ===" << endl;

    segment_tree leaderboard(20);  // allow up to 20 players

    // --- Test 1: Query empty tree ---
    cout << "\n[Test 1: Empty Tree]" << endl;
    player_data emptyQ = leaderboard.query_the_tree_by_time(0, 10);
    assert(emptyQ.score == -1);

    // --- Test 2: Insert players ---
    cout << "\n[Test 2: Insert Players]" << endl;
    leaderboard.insert_into_tree(player_data(50, 0, 5));   // ID=0
    leaderboard.insert_into_tree(player_data(70, 1, 10));  // ID=1
    leaderboard.insert_into_tree(player_data(60, 2, 8));   // ID=2
    leaderboard.insert_into_tree(player_data(90, 3, 20));  // ID=3
    leaderboard.insert_into_tree(player_data(85, 4, 15));  // ID=4

    // Verify insertion
    player_data qAll = leaderboard.query_the_tree_by_time(0, 30);
    print_player("Query all [0,30]", qAll);
    assert(qAll.score == 90);

    // --- Test 3: Query by time ---
    cout << "\n[Test 3: Query by Time]" << endl;
    //Error : Player is registered twice ??
    player_data q1 = leaderboard.query_the_tree_by_time(0, 10);
    print_player("Query [0,10]", q1);
    assert(q1.score == 70);

    player_data q2 = leaderboard.query_the_tree_by_time(10, 20);
    print_player("Query [10,20]", q2);
    assert(q2.score == 90);

    // --- Test 4: Query by ID range ---
    cout << "\n[Test 4: Query by ID]" << endl;
    player_data q3 = leaderboard.query_the_tree_by_id(1, 3);
    print_player("Query ID [1,3]", q3);
    assert(q3.score == 90);

    // --- Test 5: Update player ---
    cout << "\n[Test 5: Update]" << endl;
    bool updated = leaderboard.update_player_data(player_data(95, 2, 12)); 
    assert(updated);
    player_data q4 = leaderboard.query_the_tree_by_time(0, 20);
    print_player("After update, [0,20]", q4);
    assert(q4.score == 95);

    // --- Test 6: Invalid queries ---
    cout << "\n[Test 6: Invalid Queries]" << endl;
    player_data invalid1 = leaderboard.query_the_tree_by_time(-5, 10); // invalid time
    assert(invalid1.score == -1);

    player_data invalid2 = leaderboard.query_the_tree_by_id(10, 20);   // invalid IDs
    assert(invalid2.score == -1);

    bool update_invalid = leaderboard.update_player_data(player_data(100, 99, 30));
    assert(update_invalid == false);

    // --- Test 7: Tie-breaking ---
    cout << "\n[Test 7: Tie-breaking]" << endl;
    segment_tree tie_tree(10);
    tie_tree.insert_into_tree(player_data(80, 0, 10)); // ID=0
    tie_tree.insert_into_tree(player_data(80, 1, 15)); // ID=1 -> same score, later finish time
    tie_tree.insert_into_tree(player_data(80, 2, 15)); // ID=2 -> same score, same time, higher ID

    player_data tieQ = tie_tree.query_the_tree_by_time(0, 20);
    print_player("Tie-breaking result", tieQ);
    // Expect ID=2 to win (same score, same time, larger ID)
    assert(tieQ.player_id == 2);
//Tie-breaking result -> Score: 80 PlayerID: 2 FinishTime: 15


    // --- Test 8: Stress Test (random inserts/queries) ---
    cout << "\n[Test 8: Stress Test]" << endl;
    segment_tree stress_tree(1000);
    for (int i = 0; i < 100; i++) {
        stress_tree.insert_into_tree(player_data(rand()%1000, i, rand()%500));
    }
    // Perform random queries
    int count = 0;
    for (int i = 0; i < 50; i++) {
        int t1 = rand()%500;
        int t2 = rand()%500;
        if (t1 > t2) swap(t1,t2);
        player_data res = stress_tree.query_the_tree_by_time(t1,t2);
        // No assert here: just ensure program doesn't crash
        if (res.score != -1) {
            cout << "Stress Query [" << t1 << "," << t2 
                 << "] -> Score: " << res.score << endl;
                 count ++;
        }
    }

    cout << "\nAll detailed tests passed , count = "<<count << endl;
    return 0;
}
