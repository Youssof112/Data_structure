#pragma once 
/**
 *  @brief Represents player data in the leaderboard.
*/
class player_data{
public:
    int score;      ///< Player's score
    int player_id;  ///< Player's ID
    int finish_time;///< Player's finish time

    /**
    *  @brief Default constructor for player_data.
    */
    player_data();

    /**
     * @brief Constructor for player_data given values.
     * @param score Player's score
     * @param player_id Player's ID
     * @param finish_time Player's finish time
     */
    player_data(int score, int player_id, int finish_time);

    /**
    * @brief Compares two player_data parameters to determine the better player.
    * @param a The first player_data parameters.
    * @param b The second player_data parameters.
    * @return The player_data parameters representing the better player.
    */
    static player_data best_player(const player_data &a, const player_data &b);
};

/**
*  @brief Represents a node in the segment tree.
*/
class tree_node{
    public:
        player_data p;          ///< Player data
        int l, r;               ///< Left and right bounds
        tree_node* left_ptr;    ///< Pointer to left child
        tree_node* right_ptr;   ///< Pointer to right child

        /**
         * @brief Constructor for tree_node with bounds.
         * @param L The left bound.
         * @param R The right bound.
         */
        tree_node(int L, int R);

        /**
         * @brief Constructor for tree_node with bounds and player data.
         * @param L The left bound.
         * @param R The right bound.
         * @param score Player's score.
         * @param player_id Player's ID.
         * @param finish_time Player's finish time.
         */
        tree_node(int L, int R, int score, int player_id, int finish_time);

};

class segment_tree{
    private:
        tree_node *root_node;   ///< Pointer to the root node of the segment tree
        int max_num_of_players; ///< Maximum number of players
        int reached_index;      ///< Index of the last reached player

        /**
         * @brief Inserts a player_data into the segment tree.
         * @param node The pointer to the root node.
         * @param left The left bound of the current segment.
         * @param right The right bound of the current segment.
         * @param index The index of the player_data to insert.
         * @param data The player_data to insert.
         */
        void insert_helper_function(tree_node* &node, int left, int right, int index, player_data data);

        /**
         * @brief Queries the segment tree for the best player within a time range.
         * @param start_interval The start of the time range.
         * @param end_interval The end of the time range.
         * @return The player_data representing the best player within the time range.
         */

        player_data query_helper_function(tree_node* &node, int start_interval, int end_interval);

        /**
         * @brief Updates the player data in the segment tree.
         * @param new_data The new player_data to update.
         * @param node The pointer to the root node.
         * @param left The left bound of the current segment.
         * @param right The right bound of the current segment.
         * @return True if the update was successful, false otherwise.
         */
        bool update(tree_node* &node, int left, int right, player_data new_data);

        /**
         * @brief Queries the segment tree by player ID range.
         * @param node The pointer to the root node.
         * @param left The left bound of the current segment.
         * @param right The right bound of the current segment.
         * @param start_id The start ID of the player range.
         * @param end_id The end ID of the player range.
         * @return The player_data representing the best player within the ID range.
         */
        player_data query_by_index_helper(tree_node* &node, int left, int right, int start_id, int end_id);

    public:

    /**
     * @brief Constructor for segment_tree.
     * @param size The size of the segment tree.
     */
        segment_tree(int size);

    /**
     * @brief Inserts a player_data into the segment tree.
     * @param p The player_data to insert.
     */
        void insert_into_tree(player_data p);

    /**
     * @brief Queries the segment tree for the best player within a time range.
     * @param start_time The start of the time range.
     * @param finish_time The end of the time range.
     * @return The player_data representing the best player within the time range.
     */
        player_data query_the_tree_by_time(int start_time, int finish_time);

        /**
         * @brief Queries the segment tree by player ID range.
         * @param begin_id The start ID of the player range.
         * @param end_id The end ID of the player range.
         * @return The player_data representing the best player within the ID range.
         */
        player_data query_the_tree_by_id(int begin_id, int end_id);

        /**
         * @brief Updates the player data in the segment tree.
         * @param new_player_data The new player_data to update.
         * @return True if the update was successful, false otherwise.
         */
        bool update_player_data(player_data new_player_data);
};