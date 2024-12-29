#include "SelectionPolicy.h"
#include <vector>
#include "Facility.h"
using std::vector;
#include <algorithm>
#include <climits>


NaiveSelection::NaiveSelection() : lastSelectedIndex(-1) {}


const FacilityType &NaiveSelection::selectFacility(const vector<FacilityType> &facilitiesOptions)
{
    lastSelectedIndex = (lastSelectedIndex + 1) % facilitiesOptions.size();
    return facilitiesOptions[lastSelectedIndex];
}

const string NaiveSelection::toString() const
{
    return "nve";
}

NaiveSelection *NaiveSelection::clone() const
{
   return new NaiveSelection(*this); // Deep copy
}


BalancedSelection::BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore) : 
LifeQualityScore(LifeQualityScore), EconomyScore(EconomyScore), EnvironmentScore(EnvironmentScore) {}

const FacilityType &BalancedSelection::selectFacility(const vector<FacilityType> &facilitiesOptions)
{
    int indexToSelect = 0;
    int minDifference = INT_MAX;
    int temp_index = 0;
    for (FacilityType curr : facilitiesOptions)//finding the facilityType that will make the 
    //plan's score minimal
    {
        int eco = EconomyScore + curr.getEconomyScore();
        int env = EnvironmentScore + curr.getEnvironmentScore();
        int LQ = LifeQualityScore + curr.getLifeQualityScore();
        int currDifference = std::max({eco,env,LQ}) - std::min({eco,env,LQ});
        if( currDifference < minDifference)
        {
            minDifference= currDifference;
            temp_index = indexToSelect;
        }
        indexToSelect++;
    }
    //updating the scores in balanced Policy
    EconomyScore = EconomyScore + facilitiesOptions[temp_index].getEconomyScore();
    EnvironmentScore = EnvironmentScore + facilitiesOptions[temp_index].getEnvironmentScore();
    LifeQualityScore = LifeQualityScore + facilitiesOptions[temp_index].getLifeQualityScore();
    return facilitiesOptions[temp_index];
}

    

const string BalancedSelection::toString() const
{
    return "bal";
}

BalancedSelection *BalancedSelection::clone() const
{
    return new BalancedSelection(*this); // calling the copy constructor to create a deep copy
}




EconomySelection::EconomySelection() : lastSelectedIndex(-1)
{}

const FacilityType &EconomySelection::selectFacility(const vector<FacilityType> &facilitiesOptions) // select the facility with thehighest economy score
{
    bool found = false;
    lastSelectedIndex = (lastSelectedIndex+1)%facilitiesOptions.size() ;
   // int ans_index = 0;
    for (int i = lastSelectedIndex ; !found ; i= (i+1)%(facilitiesOptions.size()))
    {
        if(facilitiesOptions[i].getCategory() == FacilityCategory::ECONOMY)
        { 
            lastSelectedIndex = i;
            found = true;
        }
    }
return facilitiesOptions[lastSelectedIndex];
}

const string EconomySelection::toString() const
{
    return "eco";
}

EconomySelection *EconomySelection::clone() const // !!!!
{
    return new EconomySelection(*this) ;
}





SustainabilitySelection::SustainabilitySelection() :lastSelectedIndex(-1)
{}

const FacilityType &SustainabilitySelection::selectFacility(const vector<FacilityType> &facilitiesOptions)
{
    lastSelectedIndex = (lastSelectedIndex+1)%facilitiesOptions.size() ;
    for (int i = lastSelectedIndex ; ; i = (i+1)%(facilitiesOptions.size()))
    {
        if(facilitiesOptions[i].getCategory() == FacilityCategory::ENVIRONMENT) 
        {
            lastSelectedIndex = i;
            return facilitiesOptions[lastSelectedIndex];
        }
    }
return facilitiesOptions[lastSelectedIndex];
}

const string SustainabilitySelection::toString() const
{
    return "env";
}

SustainabilitySelection *SustainabilitySelection::clone() const
{
    return new SustainabilitySelection(*this) ;
}
