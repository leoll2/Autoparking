#ifndef Q_LEARNINGNETWORK_H
#define Q_LEARNINGNETWORK_H

#include <vector>
#include "AI_params.h"
#include "map.h"


class Q_LearningNetwork {
    private:
        
        unsigned int n_states;
        unsigned int n_actions;
        std::vector<std::vector<float> > Q;
        std::vector<std::vector<float> > R;
        Map& map;
        unsigned int target_state;
        
        float get_max_state_quality(int s);
        bool initialize_Q();
        void initialize_R();
        void train(unsigned int n_iterations);
        bool restore_from_cache();
        bool store_into_cache();
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
        unsigned int get_eps_greedy_action(int s);
        /* Returns the code of the best action for the provided state. */
        unsigned int get_best_action(int s);
        /* Simulates and displays one episode on the basis of the current state of Q */
        void simulate_episode();
};

#endif /* Q_LEARNINGNETWORK_H */

