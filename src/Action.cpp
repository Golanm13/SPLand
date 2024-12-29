#include "Action.h"
#include <iostream>
#include <vector>
extern Simulation* backup;

BaseAction::BaseAction() : errorMsg("") , status(ActionStatus::COMPLETED) // deafualt constractor (empty)
{}

ActionStatus BaseAction::getStatus() const
{
    return status;
}

void BaseAction::complete()
{
    status = ActionStatus::COMPLETED;
}

void BaseAction::error(string errorMsg)// will change the actions status and print the desired errorMsg
{
    status = ActionStatus::ERROR;
    this->errorMsg = errorMsg;
    std::cout << "Error : " << errorMsg << "\n";
}

const string &BaseAction::getErrorMsg() const
{
    return errorMsg;
}
//In every action we make we will determine the right status for it according to the restrictions and
//than adding the action with the updated status into to actionLog
// In every to string action we will make sure we print the right action status 
SimulateStep::SimulateStep(const int numOfSteps): numOfSteps(numOfSteps)
{}

void SimulateStep::act(Simulation &simulation)// this function will be advancing the simulation "numPfSteps" steps
 {
    for (int i = 0; i < numOfSteps; i++)
    {
        simulation.step();
    }
    complete();
    simulation.addAction(this);
}

SimulateStep *SimulateStep::clone() const
{
    return new SimulateStep(*this);
}

const string SimulateStep::toString() const
{
    string sta = "";
    if (getStatus() == ActionStatus :: COMPLETED)
    {
        sta = "COMPLETED";
    }
    else
    {
        sta = "ERROR";
    }

    return "step "+ std::to_string(numOfSteps) +" "+ sta ;
}

AddPlan::AddPlan(const string &settlementName, const string &selectionPolicy)
    : settlementName(settlementName), selectionPolicy(selectionPolicy) 
{}


 void AddPlan::act(Simulation &simulation) {
        if(!simulation.isSettlementExists(settlementName) ||
        (selectionPolicy != "env" && selectionPolicy != "nve" && selectionPolicy != "bal" && selectionPolicy != "eco")){
            error("Cannot create this plan.");
        }
        else{
            Settlement& set = simulation.getSettlement(settlementName);
            if(selectionPolicy == "env"){
                SelectionPolicy* policy = new SustainabilitySelection();
                simulation.addPlan(set, policy);
            }
            else if(selectionPolicy == "bal"){
                SelectionPolicy* policy = new BalancedSelection(0,0,0);
                simulation.addPlan(set, policy);
            }
            else if(selectionPolicy == "nve"){
                SelectionPolicy* policy = new NaiveSelection();
                simulation.addPlan(set, policy);
            }
            else{
                SelectionPolicy* policy = new EconomySelection();
                simulation.addPlan(set, policy);
            }
            complete();
        }
        simulation.addAction(this);
    }



const string AddPlan::toString() const
{
    string sta = "";
    if (getStatus() == ActionStatus :: COMPLETED)
    {
        sta = "COMPLETED";
    }
    else
    {
        sta = "ERROR";
    }
    return "plan " + settlementName + selectionPolicy + sta;
}

AddPlan *AddPlan::clone() const
{
    return new AddPlan(*this); 
}


AddSettlement::AddSettlement(const string &settlementName, SettlementType settlementType) : settlementName(settlementName), settlementType(settlementType)
{}

void AddSettlement::act(Simulation &simulation)
//calling for addSettlement(of simulatio) , it will either do the action or lead to an error
{
    Settlement* toAdd = new Settlement(settlementName, settlementType);
        if(!simulation.addSettlement(toAdd))
    {
        error("Settlement already exists");
    }
    else
    {
        complete();
    }
    simulation.addAction(this);
}

AddSettlement *AddSettlement::clone() const
{
    return new AddSettlement(*this);
}

const string AddSettlement::toString() const
{
        string sta = "";
    if (getStatus() == ActionStatus :: COMPLETED)
    {
        sta = "COMPLETED";
    }
    else
    {
        sta = "ERROR";
    }

    string type="";
    if (settlementType == SettlementType::VILLAGE)
    {
       type = "VILLAGE";
    }
    else if(settlementType == SettlementType::CITY)
    {
        type = "CITY";
    }
    else if (settlementType == SettlementType::METROPOLIS)
    {
        type = "METROPOLIS";
    }

    return "settlement  " + settlementName + "  " + type + "  " + sta +"\n";
}

AddFacility::AddFacility(const string &facilityName, const FacilityCategory facilityCategory, const int price, const int lifeQualityScore, const int economyScore, const int environmentScore)
: facilityName(facilityName), facilityCategory(facilityCategory), price(price), lifeQualityScore(lifeQualityScore), economyScore(economyScore), environmentScore(environmentScore)
{}

void AddFacility::act(Simulation &simulation)
//calling for facilityType(of simulatio) , it will either do the action or lead to an error
{
    FacilityType toAdd =  FacilityType(facilityName, facilityCategory, price, lifeQualityScore, economyScore, environmentScore);
    if(!simulation.addFacility(toAdd))
    {
        error("facility already exiest");
    }
    else
    {
        complete();
    }
    simulation.addAction(this);
}

AddFacility *AddFacility::clone() const
{
    return new AddFacility(*this);
}

const string AddFacility::toString() const
{
    string sta = "";
    if (getStatus() == ActionStatus :: COMPLETED)
    {
        sta = "COMPLETED";
    }
    else
    {
        sta = "ERROR";
    }

    string cat="";
    if (facilityCategory == FacilityCategory::LIFE_QUALITY)
    {
       cat = "LIFE_QUALITY";
    }
    if(facilityCategory == FacilityCategory::ENVIRONMENT)
    {
        cat = "ENVIRONMENT";
    }
    if(facilityCategory == FacilityCategory::ECONOMY)
    {
        cat = "ECONOMY";
    }
    return "facility " + facilityName + " " + cat + std::to_string(price) + " "
    + std::to_string(lifeQualityScore)  + " " + std::to_string(economyScore) + " "+ std::to_string(environmentScore) + " "
    + sta + "\n";
}

PrintPlanStatus::PrintPlanStatus(int planId): planId(planId)
{}

void PrintPlanStatus::act(Simulation &simulation)
//iterating through the 
{
if (planId > static_cast<int>(simulation.getAllPlans().size() - 1))
    {
        error("Plan doesn`t exist");
    }
    else
    {
        simulation.getPlan(planId).printStatus();
        complete();
    }
    simulation.addAction(this);
}

PrintPlanStatus *PrintPlanStatus::clone() const
{
    return new PrintPlanStatus(*this);
}

const string PrintPlanStatus::toString() const
{
    string sta = "";
    if (getStatus() == ActionStatus :: COMPLETED)
    {
        sta = "COMPLETED";
    }
    else
    {
        sta = "ERROR";
    }
    return  "planStatus " + std::to_string(planId) + " " + sta +"\n";
}

ChangePlanPolicy::ChangePlanPolicy(const int planId, const string &newPolicy): 
planId(planId) , newPolicy(newPolicy)
{}

void ChangePlanPolicy::act(Simulation &simulation)
{
    if (planId > static_cast<int>(simulation.getAllPlans().size() - 1)
         ||simulation.getPlan(planId).selectionPolicyToString() == newPolicy)
        error("Cannot change selection policy");// need to acces the selection policy of the plan
    else
    {
        complete();
        if (newPolicy =="nve")
        {
            NaiveSelection* newSel = new NaiveSelection();
            simulation.getPlan(planId).setSelectionPolicy(newSel);
        }

        if (newPolicy =="bal")
        {
            int setLQ = simulation.getPlan(planId).getlifeQualityScore();
            int setEn = simulation.getPlan(planId).getEnvironmentScore();
            int setEc = simulation.getPlan(planId).getEconomyScore();
            for(const Facility* curr : simulation.getPlan(planId).getUnderConstruction())
          {
                setLQ += curr->getLifeQualityScore();
                setEn += curr->getEnvironmentScore();
                setEc += curr->getEconomyScore();
          }
            BalancedSelection* newSel = new BalancedSelection(setLQ, setEn, setEc);
            simulation.getPlan(planId).setSelectionPolicy(newSel);
        }
        if (newPolicy =="eco")
        {
            EconomySelection* newSel = new EconomySelection();
            simulation.getPlan(planId).setSelectionPolicy(newSel);
        }
        if (newPolicy =="env")
        {
            SustainabilitySelection* newSel = new SustainabilitySelection();
            simulation.getPlan(planId).setSelectionPolicy(newSel);
        }   
    } 
    simulation.addAction(this);   
}

ChangePlanPolicy *ChangePlanPolicy::clone() const
{
    return new ChangePlanPolicy(*this);
}

const string ChangePlanPolicy::toString() const
{
    string sta = "";
    if (getStatus() == ActionStatus :: COMPLETED)
    {
        sta = "COMPLETED";
    }
    else
    {
        sta = "ERROR";
    }

    return "changePolicy "+ std::to_string(planId)+ " " + newPolicy + " " + sta + "\n";
}

PrintActionsLog::PrintActionsLog()
{}

void PrintActionsLog::act(Simulation &simulation)
{
    for(BaseAction* curr : simulation.getActionsLog() )
    {
        std:: cout << curr->toString() + "\n";
    }
        complete();
        simulation.addAction(this);
}

PrintActionsLog *PrintActionsLog::clone() const
{
    return new PrintActionsLog(*this);
}

const string PrintActionsLog::toString() const
{
    string sta = "";
    if (getStatus() == ActionStatus :: COMPLETED)
    {
        sta = "COMPLETED";
    }
    else
    {
        sta = "ERROR";
    }
    return "printActionsLog " + sta + "\n";
}

Close::Close()
{
}

void Close::act(Simulation &simulation)
{
    for(Plan curr : simulation.getAllPlans())
    {
       std:: cout << curr.toString() + "\n";
    }//need to ensure memory is freed.
    complete();
    simulation.addAction(this);
    simulation.close();
}

Close *Close::clone() const
{
    return new Close(*this);
}

const string Close::toString() const
{
    string sta = "";
    if (getStatus() == ActionStatus :: COMPLETED)
    {
        sta = "COMPLETED";
    }
    else
    {
        sta = "ERROR";
    }
    return "close "+sta + "\n";
}

BackupSimulation::BackupSimulation(): BaseAction()
{}

//Saving a backup of the current simulation's state, Adding the action to the list.
void BackupSimulation::act(Simulation &simulation)
{
    if(backup == nullptr) {
       backup = new Simulation(simulation);
    }
    else {
        *backup = simulation;
    }
    complete();
    simulation.addAction(this->clone());
}

BackupSimulation *BackupSimulation::clone() const
{
    return new BackupSimulation(*this);
}

const string BackupSimulation::toString() const
{
    string sta = "";
    if (getStatus() == ActionStatus :: COMPLETED)
    {
        sta = "COMPLETED";
    }
    else
    {
        sta = "ERROR";
    }
    return "backup "+sta + "\n";
}

RestoreSimulation::RestoreSimulation()
{}

void RestoreSimulation::act(Simulation &simulation)
{
    if(backup == nullptr)
    {
        error("No backup available");
    }
    else
    {
        simulation = *backup;
        complete();
    }
    simulation.addAction(this);
}

RestoreSimulation *RestoreSimulation::clone() const
{
    return new RestoreSimulation(*this);
}

const string RestoreSimulation::toString() const
{
    string sta = "";
    if (getStatus() == ActionStatus :: COMPLETED)
    {
        sta = "COMPLETED";
    }
    else
    {
        sta = "ERROR";
    }
    return "restore "+sta + "\n";
}