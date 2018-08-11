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
        void initialize_Q();
        void initialize_R();
    public:
        /* Constructor */
        Q_LearningNetwork(Map& m);

        
};

#endif /* Q_LEARNINGNETWORK_H */

