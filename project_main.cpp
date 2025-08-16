#include "project1.cpp"


int main() {
    segment_tree st(10); // max 10 players

    // Insert players
    st.insert_into_tree(player_data(50, -1, 5));   // Player 0
    st.insert_into_tree(player_data(70, -1, 3));   // Player 1
    st.insert_into_tree(player_data(70, -1, 7));   // Player 2
    st.insert_into_tree(player_data(40, -1, 2));   // Player 3
    st.insert_into_tree(player_data(90, -1, 9));   // Player 4

    cout << "Inserted 5 players.\n";

    // Query by ID range
    player_data q1 = st.query_the_tree_by_id(0, 4);
    cout << "Best player [ID 0-4]: score=" << q1.score << " id=" << q1.player_id << " time=" << q1.finish_time << endl;

    // Query smaller ID range
    player_data q2 = st.query_the_tree_by_id(1, 2);
    cout << "Best player [ID 1-2]: score=" << q2.score << " id=" << q2.player_id << " time=" << q2.finish_time << endl;

    // Query by finish_time interval
    player_data q3 = st.query_the_tree_by_time(2, 5);
    cout << "Best player [time 2-5]: score=" << q3.score << " id=" << q3.player_id << " time=" << q3.finish_time << endl;

    // Update a player's score
    bool updated = st.update_player_data(player_data(95, 2, 7)); // update ID=2
    cout << "Update result: " << (updated ? "success" : "fail") << endl;

    // Query again after update
    player_data q4 = st.query_the_tree_by_id(0, 4);
    cout << "Best player [ID 0-4] after update: score=" << q4.score << " id=" << q4.player_id << " time=" << q4.finish_time << endl;

    // Invalid query
    player_data q5 = st.query_the_tree_by_id(5, 6);
    cout << "Query [ID 5-6]: score=" << q5.score << " id=" << q5.player_id << " time=" << q5.finish_time << endl;

    return 0;
}
