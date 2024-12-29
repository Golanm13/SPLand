#include "Simulation.h"
#include <string>
#include <vector>
#include "Facility.h"
#include "Plan.h"
#include "Settlement.h"
#include "Auxiliary.h"
#include "Action.h"
#include <fstream>
using std::string;
using std::vector;

Simulation::Simulation(const string &configFilePath) :isRunning(false), planCounter(0), actionsLog(), plans(), settlements(), facilitiesOptions()
{
    std::ifstream inputFile(configFilePath);

    // Validate that the file can be opened
    if (!inputFile) {
        std::cerr << "Failed to open configuration file: " << configFilePath << std::endl;
        return; // Exit constructor if the file cannot be opened
    }

    std::string line;
    while (std::getline(inputFile, line)) {
        // Ignore lines that are empty or contain only whitespace
        if (line.empty())
            continue;

        // Parse the line into arguments
        vector<string> arguments = Auxiliary::parseArguments(line);

        if (arguments[0] == "settlement") {
            // Create a new Settlement object and add it to the simulation
            auto *settlement = new Settlement(arguments[1], static_cast<SettlementType>(std::stoi(arguments[2])));
            addSettlement(settlement);
        } 
        else if (arguments[0] == "facility") {
            // Add a Facility object to the simulation
            addFacility(FacilityType(arguments[1],
            static_cast<FacilityCategory>(std::stoi(arguments[2])), std::stoi(arguments[3]),
             std::stoi(arguments[4]), std::stoi(arguments[5]), std::stoi(arguments[6])));
        } 
        else if (arguments[0] == "plan") {
            // Loop through settlements to find the one matching the plan
            for (auto &settlementPtr : settlements) {
                if (settlementPtr->getName() == arguments[1]) {
                    // Handle different selection policies
                    if (arguments[2] == "nve") {
                        addPlan(*settlementPtr, new NaiveSelection());
                    } else if (arguments[2] == "bal") {
                        addPlan(*settlementPtr, new BalancedSelection(0, 0, 0));
                    } else if (arguments[2] == "eco") {
                        addPlan(*settlementPtr, new EconomySelection());
                    } else if (arguments[2] == "env") {
                        addPlan(*settlementPtr, new SustainabilitySelection());
                    } else {
                        // Throw an error for unsupported policy types
                        throw std::invalid_argument("Unknown selection policy: " + arguments[2]);
                    }
                    break; // Exit the loop once the correct settlement is found
                }
            }
        }
    }

    // Close the input file after reading is complete
    inputFile.close();
}

Simulation::Simulation(const Simulation &other)
: isRunning(other.isRunning),
planCounter(other.planCounter),
actionsLog(),
plans(),
settlements(),
facilitiesOptions(other.facilitiesOptions)
{
    for (BaseAction* curr : other.actionsLog)
        actionsLog.push_back(curr->clone());

    for (Settlement* curr : other.settlements)
        settlements.push_back(new Settlement(*curr));

    for (Plan curr : other.plans){ // we need to take care of all the plans in plans vector
    {
        const Plan &p = curr;
        Settlement& setl = getSettlement(curr.getSettlementName()); //to keep the settlement
        plans.push_back (Plan(p, setl)); 
    }
    }
    }



Simulation::Simulation(Simulation &&other)
: isRunning(other.isRunning), planCounter(other.planCounter),
actionsLog(other.actionsLog), plans(other.plans),
settlements(other.settlements),
facilitiesOptions(other.facilitiesOptions)
{
    other.settlements.clear();
    other.actionsLog.clear();
}



Simulation::~Simulation()
{
    for (Settlement* curr : settlements){
        delete curr;
    }
    for (BaseAction* curr : actionsLog){
        delete curr;
    }
    actionsLog.clear();
    plans.clear();
    settlements.clear();
    facilitiesOptions.clear();
}



Simulation &Simulation::operator=(const Simulation &other)
{
    if (&other != this) {
        planCounter = other.planCounter;
        isRunning = other.isRunning;
        
        for (BaseAction* curr : actionsLog)
            delete curr; //deleting the objects the current pointers are pointing to.
        actionsLog.clear(); //deleting the current pointers.

        for (Settlement* curr : settlements)
            delete curr;
        settlements.clear();
        plans.clear();
        facilitiesOptions.clear();

        for (BaseAction* curr : other.actionsLog)
            actionsLog.push_back(curr->clone());

        for (Settlement* curr : other.settlements)
            settlements.push_back(new Settlement(*curr));
        
        for (Plan curr : other.plans){ // we need to take care of all the plans in plans vector
        {
            const Plan &p = curr;
            Settlement& setl = getSettlement(curr.getSettlementName()); //to keep the settlement
            plans.push_back (Plan(p, setl)); 
        }
            
        facilitiesOptions.clear();
        for (FacilityType curr : other.facilitiesOptions)
            facilitiesOptions.push_back(curr);
    }
    }
    return *this;

}

Simulation &Simulation::operator=(Simulation &&other)
{
    if (&other != this) {
        isRunning = other.isRunning;
        planCounter = other.planCounter;

        for (BaseAction* curr : actionsLog)
            delete curr; //deleting the objects the current pointers are pointing to.
        actionsLog = other.actionsLog;
        other.actionsLog.clear();

        for (Settlement* curr : settlements)
            delete curr;
        settlements = other.settlements;
        other.settlements.clear();

        plans.clear();
        for (Plan curr : other.plans)
            plans.push_back(curr);
        other.plans.clear();
        
        facilitiesOptions.clear();
        for (FacilityType curr : other.facilitiesOptions)
            facilitiesOptions.push_back(curr);
        other.facilitiesOptions.clear();
    }
    return *this;
}



void Simulation::start() {
    std::cout << "The simulation has started \n";
    std::string line;
    
    if(!isRunning){
        open();
    }
    while (isRunning) {
        std::getline(std::cin, line); 
        vector<string> command = Auxiliary::parseArguments(line);
        if (command[0] == "close") {
            Close *close = new Close();
            close->act(*this);
        }
        else if (command[0] == "step") {
            SimulateStep* step = new SimulateStep(std::stoi(command[1]));
            step->act(*this);
        }
        else if (command[0] == "plan") {
            AddPlan* addplan = new AddPlan(command[1], command[2]);
            addplan->act(*this);
        }
        else if (command[0] == "settlement") {
            AddSettlement* addSett = new AddSettlement(command[1], static_cast<SettlementType>(std::stoi(command[2])));
            addSett->act(*this);
        }
        else if (command[0] == "facility") {
            AddFacility* addfac = new AddFacility(command[1], static_cast<FacilityCategory>(std::stoi(command[2])),
            std::stoi(command[3]), std::stoi(command[4]), std::stoi(command[5]), std::stoi(command[6]));
            addfac->act(*this);
        }
        else if (command[0] == "planStatus") {
            PrintPlanStatus* pps = new PrintPlanStatus(std::stoi(command[1]));
            pps->act(*this);
        }
        else if (command[0] == "changePolicy") {
            ChangePlanPolicy* changepol = new ChangePlanPolicy(std::stoi(command[1]), command[2]);
            changepol->act(*this);
        }
        else if (command[0] == "log") {
            PrintActionsLog* printlog = new PrintActionsLog();
            printlog->act(*this);
        }
        else if (command[0] == "backup") {
            BackupSimulation* backupsim = new BackupSimulation();
            backupsim->act(*this);
            delete backupsim;
        }
        else if (command[0] == "restore") {
            RestoreSimulation* restsim = new RestoreSimulation();
            restsim->act(*this);
        }
        else
            std::cout << "Command is ilegal" << std::endl;
    }
}

void Simulation::addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy) //add after cheacking the input is legal, and adds to plans (the field)
{
    plans.push_back(Plan(planCounter, settlement, selectionPolicy, facilitiesOptions));
    planCounter = planCounter + 1 ; 
}

void Simulation::addAction(BaseAction *action)
{
    actionsLog.push_back(action);
}

bool Simulation::addSettlement(Settlement *settlement)
{  
    if(!(isSettlementExists(settlement->getName()))){
    settlements.push_back(settlement);
    }
    else {
        delete settlement;
    }
    return true;
}



bool Simulation::addFacility(FacilityType facility)
{
    for(FacilityType curr : facilitiesOptions){ // checks if the facility exist
        if(curr.getName() == (facility.getName())){
            return false;
        }
    }
        facilitiesOptions.push_back(facility);
        return true;
}

bool Simulation::isSettlementExists(const string &settlementName)
{
    for(Settlement* setl : settlements)
        if (setl->getName() == settlementName)
        {
            return true;
        }
    return false;
}

Settlement &Simulation::getSettlement(const string &settlementName)
{
    for(Settlement* setl : settlements)
    {
        if (setl->getName() == settlementName)
        {
            return *setl;
        }
    }
    //assuming we wont reach this line.
    throw std::runtime_error("Settlement not found in the simulation");
}

Plan &Simulation::getPlan(const int planID) {
    return plans[planID];
}

const vector<Plan> &Simulation::getAllPlans() const //A new function in order to access the Plans vector
{
    return plans;
}

void Simulation::step()
{
    for(Plan &curr: plans)
    {
        curr.step();
    }
}
void Simulation::close()
{
    isRunning = false;
}

void Simulation::open()
{
    isRunning = true;
}
const vector<BaseAction*> Simulation::getActionsLog() {
    return actionsLog;
}