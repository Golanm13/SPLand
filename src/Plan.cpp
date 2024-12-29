#include "Plan.h"
#include <vector>
#include "Facility.h"
#include "Settlement.h"
#include "SelectionPolicy.h"
#include <iostream>
using std::vector;

Plan::Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions) 
: plan_id(planId),settlement(settlement),selectionPolicy(selectionPolicy),status(PlanStatus::AVALIABLE),facilities(),underConstruction(), facilityOptions(facilityOptions)
, life_quality_score(0) , economy_score(0), environment_score(0)
{}

Plan::Plan(const Plan &other)
: plan_id(other.plan_id),
settlement(other.settlement),
selectionPolicy(),
status(other.status),
facilities(),
underConstruction(),
facilityOptions(other.facilityOptions),
life_quality_score(other.life_quality_score),
economy_score(other.economy_score),
environment_score(other.environment_score) 
{   
    if (other.selectionPolicy != nullptr) // if selection policy exists clone so wont use the same pointer
        selectionPolicy = other.selectionPolicy->clone();
    else
        selectionPolicy = nullptr;
    
    for (Facility* curr: other.facilities) //add the facilities pointers to our current plan
        facilities.push_back(new Facility (*curr));
    
    for (Facility* curr: other.underConstruction)
        underConstruction.push_back(new Facility (*curr));
}


Plan::Plan(const Plan &other ,const Settlement &setl)
: plan_id(other.plan_id),
settlement(setl),
selectionPolicy(),
status(other.status),
facilities(),
underConstruction(),
facilityOptions(other.facilityOptions),
life_quality_score(other.life_quality_score),
economy_score(other.economy_score),
environment_score(other.environment_score) 
{   
    if (other.selectionPolicy != nullptr) // if selection policy exists clone so wont use the same pointer
        selectionPolicy = other.selectionPolicy->clone();
    else
        selectionPolicy = nullptr;
    
    for (Facility* curr: other.facilities) //add the facilities pointers to our current plan
        facilities.push_back(new Facility (*curr));
    
    for (Facility* curr: other.underConstruction)
        underConstruction.push_back(new Facility (*curr));
}

Plan::Plan(Plan &&other)
: plan_id(other.plan_id),
settlement(other.settlement),
selectionPolicy(other.selectionPolicy),
status(other.status),
facilities(other.facilities),
underConstruction(other.underConstruction),
facilityOptions(other.facilityOptions),
life_quality_score(other.life_quality_score),
economy_score(other.economy_score),
environment_score(other.environment_score)
{
    other.selectionPolicy = nullptr; //remove the pointers so it wont call the distractour and prevent memory leak
    other.facilities.clear();
    other.underConstruction.clear();
}

Plan::~Plan() {
    delete selectionPolicy;
    for (Facility* curr: facilities)
        delete curr;
    for (Facility* curr: underConstruction)
        delete curr;        
}

const int Plan::getlifeQualityScore() const
{
    return life_quality_score;
}

const int Plan::getEconomyScore() const
{
    return economy_score;
}

const int Plan::getEnvironmentScore() const
{
    return environment_score;
}

void Plan::setSelectionPolicy(SelectionPolicy *selectionPolicy)
{
    delete this->selectionPolicy;
    this->selectionPolicy = selectionPolicy;
}

void Plan::step()
{
    int constraction_limit;

    if (settlement.getType() == SettlementType::VILLAGE) 
        constraction_limit = 1;
    if (settlement.getType() == SettlementType::CITY) 
        constraction_limit = 2;
    if (settlement.getType() == SettlementType::METROPOLIS) 
        constraction_limit = 3;
    
    while (status == PlanStatus::AVALIABLE) // adding more facilities to the underConstruction vector
    {
        Plan::addFacility(new Facility((selectionPolicy->selectFacility(facilityOptions)), settlement.getName()));
    }
    // Iterate over all facilities in underConstruction
    for (int i = static_cast<int>(underConstruction.size())-1; i>=0; i--) 
    {
        // Call the step() method on each Facility object
        underConstruction[i]->step();  
        if (underConstruction[i]->getStatus() == FacilityStatus::OPERATIONAL)
        {
            // Add the facility to the 'facilities' vector
            Plan::addFacility(underConstruction[i]); // a ready facility
            // Update the plan's scores
            life_quality_score += underConstruction[i]->getLifeQualityScore();
            economy_score += underConstruction[i]->getEnvironmentScore();
            environment_score += underConstruction[i]->getEconomyScore();       
            // Remove the facility from underConstruction
            underConstruction.erase(underConstruction.begin() + i); 
            if (constraction_limit == static_cast<int>(underConstruction.size()))
               status = PlanStatus::BUSY;
            else
               status = PlanStatus::AVALIABLE;
        }
    }
}

void Plan::printStatus()
{
    string statusToString ;

   if (status == PlanStatus::AVALIABLE)
   {
        statusToString = "AVALIABLE";
   }

    else
    {
        statusToString = "BUSY";
    }

    string selPolicy = selectionPolicyToString();
    string partA = "PlanID: " + std:: to_string(plan_id) + " \n" +
    "Settlement" + settlement.toString() + " \n" +
    "PlanStatus: " + statusToString + " \n" +
    "SelectionPolicy: "  + selPolicy + " \n" +
    "LifeQualityScore: " +std :: to_string(getlifeQualityScore()) + " \n" + 
    "EconomyScore: " + std :: to_string(getEconomyScore()) + " \n" +
    "EnvrionmentScore: " + std :: to_string(getEnvironmentScore()) + "\n ";
    
    string facilitiesString = "" ;
        for (unsigned i = 0; i < facilities.size(); i++)
        { 
            facilitiesString = facilitiesString + facilities[i]->toString();
        }
         for (unsigned i = 0; i < underConstruction.size(); i++)
        { 
            facilitiesString = facilitiesString + underConstruction[i]->toString();
        }
    std:: cout << partA + facilitiesString +"\n";
}

const vector<Facility *> &Plan::getFacilities() const
{
    return facilities;
}

const vector<Facility *> &Plan::getUnderConstruction()
{
    return underConstruction;
}

void Plan::addFacility(Facility *facility)
{// will be used in step method
if (facility->getStatus() == FacilityStatus::OPERATIONAL)
    facilities.push_back(facility);
else
    underConstruction.push_back(facility);    
if (underConstruction.size() == static_cast<unsigned>(settlement.getType()) + 1)
        {
            status = PlanStatus::BUSY;
        }
else
    status = PlanStatus::AVALIABLE;
}

const string Plan::toString() const
{
    return "PlanID:" + std::to_string(plan_id) + "\n"
    "SettlementName: " + settlement.toString() + "\n"
    "LifeQualityScore:" + std::to_string(getlifeQualityScore()) + "\n"
    "EconomyScore:" + std::to_string(getEconomyScore()) + "\n"
    "EnvrionmentScore:" + std::to_string(getEnvironmentScore());
}


const int Plan::getPlanId()
{
    return plan_id;
}

const string Plan::selectionPolicyToString()
{
    return selectionPolicy->toString();
}

const string Plan::getSettlementName(){
    return settlement.getName();
}