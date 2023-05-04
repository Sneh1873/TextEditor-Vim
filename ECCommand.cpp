#include "ECCommand.h"
#include "ECMVC.h"
#include <iostream>

using namespace std;

// ---------------------------------COMMAND HISTORY------------------------------------ //
ECCommandHistory :: ECCommandHistory() : posCurrCmd(-1)
{
}

ECCommandHistory :: ~ECCommandHistory()
{
    for(unsigned int i=0; i<listCommands.size(); ++i)
    {
        delete listCommands[i];
    }
    listCommands.clear();
    posCurrCmd = -1;
}

bool ECCommandHistory::Undo()
{
    if (listCommands.empty()) {
        return false;
    }
    
    for(auto it = listCommands.rbegin(); it!=listCommands.rend(); it++)
    {
        (*it)->UnExecute();
        listUndo.push_back(*it);
    }
    listCommands.clear();
    return true;
}

bool ECCommandHistory::Redo()
{
    if(listUndo.empty()) return false;
   
    for(auto it = listUndo.rbegin(); it!= listUndo.rend(); it++)
    {
        (*it)->Execute();
        listCommands.push_back(*it);
    }
    listUndo.clear();

    return true;
}

void ECCommandHistory :: ExecuteCmd( ECCommand *pCmd )
{
    pCmd->Execute();
    listCommands.push_back(pCmd);
    ++posCurrCmd;
}

int ECCommandHistory :: GetListLen() const {
    return listCommands.size();
}

void ECCommandHistory :: ClearCommands() {
    for (int i = 0; i < (int)listCommands.size(); ++i) {
        delete listCommands[i];
    }

    for(int i = 0; i > (int)listUndo.size(); i++)
    {
        delete listUndo[i];
    }
    listCommands.clear();
    listUndo.clear();
    posCurrCmd = -1;
}

void ECCommandHistory :: pushNull() {
    listCommands.push_back(nullptr);
    posCurrCmd++;
}
// ---------------------------------COMMAND HISTORY------------------------------------ //