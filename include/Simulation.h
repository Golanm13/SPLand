#pragma once
#include <string>
#include <vector>
#include "Facility.h"
#include "Plan.h"
#include "Settlement.h"
using std::string;
using std::vector;

class BaseAction;
class SelectionPolicy;

class Simulation { 
    public:
        Simulation(const string &configFilePath);
        Simulation(Simulation &&other);
        Simulation(const Simulation &other);
        ~Simulation();
        Simulation& operator=(const Simulation &other);
        Simulation& operator=(Simulation &&other);
        void start();
        void addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy);
        void addAction(BaseAction *action);
        bool addSettlement(Settlement *settlement);
        bool addFacility(FacilityType facility);
        bool isSettlementExists(const string &settlementName);
        Settlement &getSettlement(const string &settlementName);
        Plan &getPlan(const int planID);
        const vector<Plan>& getAllPlans() const; //A new function in order to access the Plans vector
        void step();
        void close();
        void open();
        const vector<BaseAction*> getActionsLog();

    private:
        bool isRunning;
        int planCounter; //For assigning unique plan IDs
        vector<BaseAction*> actionsLog;
        vector<Plan> plans;
        vector<Settlement*> settlements;
        vector<FacilityType> facilitiesOptions;
};