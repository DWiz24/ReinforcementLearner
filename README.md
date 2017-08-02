# ReinforcementLearner
The Q-learning code is in Learner.cpp.  discreteLearn() takes three function pointers:
  1. gives a starting state  
  2. gives pointers to the entries in the Q-table of the available actions
  3. takes the current state and the chosen action.  The action is represented by the index of the chosen action in the list returned by #2.  Returns the reward obtained and th next state.  An empty state means that the simulation is done and the learner should ask for a new start state.
discreteLearn() also takes:
  4. the number of runs, the number of times to get a new start state
  5. the learning rate. (0,1]
  6. the ratio the agent discredits future rewards. [0,1]
  7. the probability the agent picks a random action [0,1]
  
  GridWorld will print a full run every n runs. N is stored in printFreq.
  
  IceWorld prints the number of turns it took to reach the goal each run.
  
  MazeBot will print a full run every n runs. N is stored in printFreq.
  The agent is an '@', walls are 'x', the three colors of paint are '.' (default), '*', and '-'.  Each run ends when the agent returns to his starting position.
  
  MazeBot does not successfully learn a tree traversal, it just learns to immediately return to the start.  The agent paints the space he leaves and then moves to any adjacent painted space.
