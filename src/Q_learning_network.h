#ifndef Q_LEARNINGNETWORK_H
#define Q_LEARNINGNETWORK_H

#include <fstream>
#include <iostream>
#include <vector>
#include "AI_params.h"
#include "map.h"

/******************************************************************************/
/*     Q-LEARNING NETWORK: reinforcement learning applied to autoparking      */
/******************************************************************************/

class Q_LearningNetwork {
    private:
        
        unsigned int n_states;
        unsigned int n_actions;
        std::vector<std::vector<float> > Q;
        std::vector<std::vector<float> > R;
        Map& map;
        unsigned int iter_trained;
        unsigned int target_state;
        
        float get_max_state_quality(int s) const;
        bool initialize_Q();
        void initialize_R();
        void open_conv_log(std::ofstream& log);
        void close_conv_log(std::ofstream& log);
        void add_to_conv_log(std::ofstream& log, unsigned int iter, float avg_delta);
    public:
        /* Constructor */
        Q_LearningNetwork(Map& m);
        
        /* Returns the number of states */
        unsigned int get_n_states() const;
        
        /* Returns the number of actions */
        unsigned int get_n_actions() const;
        
        /* Returns a reference to the map */
        Map& get_map() const;
        
        /* Returns the quality value associate to the pair state-action (s,a). */
        double get_quality(unsigned int s, unsigned int a) const;
        
        /* Returns the reward associated to the pair state-action (s,a). */
        double get_reward(unsigned int s, unsigned int a) const;
        
        /* Returns the code of an action according to epsilon-greedy policy. */
        unsigned int get_eps_greedy_action(int s) const;
        
        /* Returns the code of the best action for the provided state. */
        unsigned int get_best_action(int s) const;

        /* Returns the code of the target state. */
        unsigned int get_target_state() const;

        /* Return the total number of performed training iterations (i.e. the experience) */
        unsigned int get_iter_trained() const;
        
        /* Train the neural network for the specified number of iterations. */
        void train(unsigned int n_iterations, bool force = false);

        /* Reset the Q matrix */
        void reset_Q();

        /* Load the Q matrix from the cache */
        bool restore_from_cache();
        
        /* Save the Q matrix in the cache */
        bool store_into_cache();
};

#endif /* Q_LEARNINGNETWORK_H */

