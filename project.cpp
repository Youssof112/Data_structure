#include <iostream>

using namespace std;

class player_data
{
public:
    int score;
    int player_id;
    int finish_time;

    player_data() : score(-1), player_id(-1), finish_time(-1)
    {
    }

    player_data(int score, int player_id, int finish_time) : score(score), player_id(player_id), finish_time(finish_time)
    {
    }

    static player_data best_player(const player_data &a, const player_data &b)
    {
        if (a.score == -1 && b.score == -1)
        {
            return player_data();
        } // no players

        if (a.score == -1)
        {
            return b;
        }

        if (b.score == -1)
        {
            return a;
        }

        if (a.score > b.score)
        {
            return a;
        }
        else if (b.score > a.score)
        {
            return b;
        }
        else
        {
            if (a.finish_time < b.finish_time)
            {
                return a;
            }
            else if (b.finish_time < a.finish_time)
            {
                return b;
            }
            else
            {
                if (a.player_id > b.player_id)
                {
                    return a;
                }
                else if (b.player_id > a.player_id)
                {
                    return b;
                }
                // no duplication allowed in player registration for the ID
                else
                {
                    cerr << "Error : Player is registered twice" << endl;

                    return a;
                }
            }
        }
    }
};

class tree_node
{
public:
    player_data p;
    int l, r;
    tree_node *left_ptr;
    tree_node *right_ptr;

    tree_node(int L, int R) : l(L), r(R), left_ptr(0), right_ptr(0)
    {
        p = player_data();
    }

    tree_node(int L, int R, int score, int player_id, int finish_time) : l(L), r(R), left_ptr(0), right_ptr(0)
    {
        p = player_data(score, player_id, finish_time);
    }
};

class segment_tree
{
private:
    tree_node *root_node;
    int max_num_of_players;
    int reached_index; // cannot be global , due multible instances of the segment tree will make indexing wrong

    void insert_helper_function(tree_node *&node, int left, int right, int index, player_data data)
    {
        if (!node)
        {
            node = new tree_node(left, right);
        }

        if (left == right)
        {
            node->p = data;

            return;
        }

        int mid = (left + right) / 2;

        if (index <= mid)
        {
            insert_helper_function(node->left_ptr, left, mid, index, data);
        }
        else
        {
            insert_helper_function(node->right_ptr, mid + 1, right, index, data);
        }

        player_data p1;
        player_data p2;

        if (node->left_ptr)
        {
            p1 = node->left_ptr->p;
        }

        if (node->right_ptr)
        {
            p2 = node->right_ptr->p;
        }

        node->p = player_data::best_player(p1, p2);
    }

    player_data query_helper_function(tree_node *&node, int start_interval, int end_interval)
    {
        if (!node)
        {
            return player_data();
        }

        if (node->left_ptr == 0 && node->right_ptr == 0)
        {
            if (node->p.finish_time >= start_interval && node->p.finish_time <= end_interval)
            {
                return node->p;
            }
            else
            {
                return player_data();
            }
        }

        player_data left = query_helper_function(node->left_ptr, start_interval, end_interval);
        player_data right = query_helper_function(node->right_ptr, start_interval, end_interval);

        return player_data ::best_player(left, right);
    }

    bool update(tree_node *&node, int left, int right, player_data new_data)
    {
        if (!node)
        {
            return false;
        }

        if (left == right && left == new_data.player_id)
        {
            node->p = new_data;

            return true;
        }

        bool check = false;
        int mid = (left + right) / 2;

        if (new_data.player_id <= mid && node->left_ptr)
        {
            check = update(node->left_ptr, left, mid, new_data);
        }
        else if (new_data.player_id > mid && node->right_ptr)
        {
            check = update(node->right_ptr, mid + 1, right, new_data);
        }

        if (check)
        {
            player_data left_player;
            player_data right_player;

            if (node->left_ptr)
            {
                left_player = node->left_ptr->p;
            }
            else
            {
                left_player = player_data();
            }

            if (node->right_ptr)
            {
                right_player = node->right_ptr->p;
            }
            else
            {
                right_player = player_data();
            }

            node->p = player_data ::best_player(left_player, right_player);
        }

        return check;
    }

    player_data query_by_index_helper(tree_node *&node, int left, int right, int start_id, int end_id)
    {
        if (!node || end_id < left || start_id > right)
        {
            return player_data();
        }

        if (start_id <= left && right <= end_id)
        {
            return node->p;
        }

        int mid = (left + right) / 2;
        player_data left_player = query_by_index_helper(node->left_ptr, left, mid, start_id, end_id);
        player_data right_player = query_by_index_helper(node->right_ptr, mid + 1, right, start_id, end_id);

        return player_data::best_player(left_player, right_player);
    }

public:
    segment_tree(int size) : root_node(0), max_num_of_players(size)
    {
        this->reached_index = -1;
    } // initialize reached_index to -1

    void insert_into_tree(player_data p)
    {
        this->reached_index++;
        player_data p1(p.score, this->reached_index, p.finish_time);

        return insert_helper_function(root_node, 0, max_num_of_players - 1, this->reached_index, p1);
    }
    // the tree is created by indices , not time ,thus complexity is close to O(n)
    player_data query_the_tree_by_time(int start_time, int finish_time)
    {
        if (start_time < 0 || finish_time < 0)
        {
            cerr << "Wrong Interval";

            return player_data();
        }

        return query_helper_function(root_node, start_time, finish_time);
    }

    player_data query_the_tree_by_id(int begin_id, int end_id)
    {
        if (begin_id < 0 || end_id > this->reached_index)
        {
            cerr << "Invalid ID " << endl;

            return player_data();
        }
        else if (begin_id > end_id)
        {
            cerr << "Invalid Interval" << endl;

            return player_data();
        }

        return query_by_index_helper(root_node, 0, max_num_of_players - 1, begin_id, end_id);
    }

    bool update_player_data(player_data new_player_data)
    {
        bool check = false;

        if (new_player_data.player_id < 0 || new_player_data.player_id > this->reached_index)
        {
            cerr << "Invalid Player ID" << endl;

            return false;
        }
        else
        {
            player_data p1(new_player_data.score, new_player_data.player_id, new_player_data.finish_time);
            check = update(root_node, 0, max_num_of_players - 1, p1);

            return check;
        }

        return check;
    }
};
