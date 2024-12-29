#pragma once
#include <vector>
#include "Facility.h"
#include "Settlement.h"
#include "SelectionPolicy.h"
using std::vector;

enum class PlanStatus {
    AVALIABLE,
    BUSY,s
};

class Plan {
    public:
        Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions);
        Plan(const Plan &other);
        Plan(const Plan &other , const Settlement &setl); //help for the backup simulation
        Plan(Plan &&other);
        ~Plan();
        Plan();
        Plan &operator=(const Plan &other) = delete;
        Plan operator= (Plan &&other) = delete;
        const int getlifeQualityScore() const;
        const int getEconomyScore() const;
        const int getEnvironmentScore() const;
        void setSelectionPolicy(SelectionPolicy *selectionPolicy);
        void step();
        void printStatus();
        const vector<Facility*> &getFacilities() const;
        const vector<Facility*> &getUnderConstruction();
        void addFacility(Facility* facility);
        const string toString() const;
        const int getPlanId();
        const  string selectionPolicyToString();
        const string getSettlementName();
      
        

    private:
        int plan_id;
        const Settlement &settlement;
        SelectionPolicy *selectionPolicy; //What happens if we change this to a reference?
        PlanStatus status;
        vector<Facility*> facilities;
        vector<Facility*> underConstruction;
        const vector<FacilityType> &facilityOptions;
        int life_quality_score, economy_score, environment_score;
};