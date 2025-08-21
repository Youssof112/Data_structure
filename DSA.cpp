#include "project.cpp"
#include <cassert>
#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;

// ================= Golden Model =================
class golden_model {
public:
    void add_player(const player_data &p) { players.push_back(p); }

    player_data query_by_time(int start, int end) {
        player_data best;
        for (const auto &p : players) {
            if (p.finish_time >= start && p.finish_time <= end) {
                best = player_data::best_player(best, p);
            }
        }
        return best;
    }

    player_data query_by_id(int start_id, int end_id) {
        player_data best;
        for (const auto &p : players) {
            if (p.player_id >= start_id && p.player_id <= end_id) {
                best = player_data::best_player(best, p);
            }
        }
        return best;
    }

    bool update_player(const player_data &new_data) {
        for (auto &p : players) {
            if (p.player_id == new_data.player_id) {
                p = new_data;
                return true;
            }
        }
        return false;
    }

private:
    vector<player_data> players;
};

// ================= Helper Functions =================
void print_player(const string &label, const player_data &p) {
    cout << setw(25) << left << label
         << " | Score: " << setw(4) << p.score
         << " PlayerID: " << setw(3) << p.player_id
         << " Time: " << setw(3) << p.finish_time << endl;
}

bool check_results(const player_data &actual,
                   const player_data &expected,
                   int &correctCount, int &totalCount) {
    totalCount++;
    bool pass = (actual.score == expected.score &&
                 actual.player_id == expected.player_id &&
                 actual.finish_time == expected.finish_time);
    if (pass) {
        correctCount++;
        cout << "[PASS] "<< endl;
    } else {
        cout << "[FAIL] " << endl;
        cout << "   Expected -> ";
        print_player("", expected);
        cout << "   Got      -> ";
        print_player("", actual);
    }
    return pass;
}

void check_bool(bool actual, bool expected,
                int &correctCount, int &totalCount) {
    totalCount++;
    if (actual == expected) {
        correctCount++;
        cout << "[PASS] " << endl;
    } else {
        cout << "[FAIL] " << " Expected=" << expected << " Got=" << actual << endl;
    }
}

// ================= Stress Test =================
void stress_test(int num_players, int num_queries,
                 int &total_count, int &correct_count) {
    cout << "\n--- Stress Test: " << num_players
         << " players, " << num_queries << " queries ---" << endl;

    segment_tree leaderboard(num_players);
    golden_model model;

    // Insert players
    for (int i = 0; i < num_players; ++i) {
        int score = rand() % 1000;
        int finish_time = rand() % 1000;
        player_data p(score, i, finish_time);
        model.add_player(p);
        leaderboard.insert_into_tree(p);
    }

    // Query by time
    for (int i = 0; i < num_queries; ++i) {
        int t1 = rand() % 1000, t2 = rand() % 1000;
        //invalid ranges in the main
        if (t1 > t2) swap(t1, t2);

        player_data actual = leaderboard.query_the_tree_by_time(t1, t2);
        player_data expected = model.query_by_time(t1, t2);
        check_results(actual, expected, correct_count, total_count);
    }

    // Query by ID
    for (int i = 0; i < num_queries; ++i) {
        int id1 = rand() % num_players, id2 = rand() % num_players;
        //invalid ranges in the main
        if (id1 > id2) swap(id1, id2);

        player_data actual = leaderboard.query_the_tree_by_id(id1, id2);
        player_data expected = model.query_by_id(id1, id2);
        check_results(actual, expected, correct_count, total_count);
    }

    // Random updates
    for (int i = 0; i < num_queries; ++i) {
        int id = rand() % num_players;
        player_data new_p(rand() % 1000, id, rand() % 1000);
        bool act = leaderboard.update_player_data(new_p);
        bool exp = model.update_player(new_p);
        check_bool(act, exp,
                   correct_count, total_count);
    }
}

// ================= Main =================
int main() {
    srand(42); // fixed seed for reproducibility
    cout << "=== Time-Range Leaderboard Query System: Formal Tests ===" << endl;
    int correctCount = 0, totalCount = 0;

    // Basic tests (small controlled cases)
    {
        cout << "\n--- Basic Tests ---" << endl;
        segment_tree leaderboard(10);
        golden_model model;

        // Empty query
        check_results( 
                      leaderboard.query_the_tree_by_time(0, 10),
                      player_data(-1, -1, -1),
                      correctCount, totalCount);

        // Insert players
        vector<player_data> players = {
            {50,0,5}, {70,1,10}, {60,2,8}, {90,3,20}, {85,4,15}
        };
        for (auto &p : players) {
            leaderboard.insert_into_tree(p);
            model.add_player(p);
        }

        // Queries
        check_results(
                      leaderboard.query_the_tree_by_time(0,30),
                      model.query_by_time(0,30),
                      correctCount, totalCount);

        check_results(
                      leaderboard.query_the_tree_by_id(1,3),
                      model.query_by_id(1,3),
                      correctCount, totalCount);

        // Update
        player_data upd(95,2,12);
        check_bool(leaderboard.update_player_data(upd),
                   model.update_player(upd),
                   correctCount, totalCount);

        check_results(
                      leaderboard.query_the_tree_by_time(0,20),
                      model.query_by_time(0,20),
                      correctCount, totalCount);
    }
    // Invalid ranges in the main
    {
        cout << "\n--- Invalid Ranges Tests ---" << endl;
          segment_tree leaderboard(10);
          for (int i = 0; i < 10; ++i) {
              int score = rand() % 1000;
              int finish_time = rand() % 1000;
              player_data p(score, i, finish_time);
              leaderboard.insert_into_tree(p);
          }
          // Invalid time range
          player_data p = leaderboard.query_the_tree_by_time(30, 0);
          check_results(p, player_data(-1, -1, -1), correctCount, totalCount);
          player_data p2 = leaderboard.query_the_tree_by_time(-1, 5);
          check_results(p2, player_data(-1, -1, -1), correctCount, totalCount);

          // Invalid ID range
          player_data p3 = leaderboard.query_the_tree_by_id(3, 1);
          check_results(p3, player_data(-1, -1, -1), correctCount, totalCount);

     }
    // Stress tests
    stress_test(500, 100, totalCount, correctCount);
    stress_test(1000, 200, totalCount, correctCount);
    stress_test(5000, 500, totalCount, correctCount);

    // Final report
    cout << "\n=== Final Report ===" << endl;
    cout << "Correct: " << correctCount << " / " << totalCount
         << " (" << fixed << setprecision(2)
         << (100.0*correctCount/totalCount) << "%)" << endl;

    return 0;
}
