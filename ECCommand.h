#ifndef ECCOMMAND_H
#define ECCOMMAND_H

#include <vector>

// ---------------------------------COMMAND------------------------------------ //
class ECCommand {
public:
    virtual ~ECCommand() {}
    virtual void Execute() = 0;
    virtual void UnExecute() = 0;
};
// ---------------------------------COMMAND------------------------------------ //

// ---------------------------------COMMAND HISTORY------------------------------------ //
class ECCommandHistory {
public:
    ECCommandHistory();
    virtual ~ECCommandHistory();
    bool Undo();
    bool Redo();
    void ExecuteCmd(ECCommand *pCmd);
    int GetListLen() const;
    void ClearCommands();
    void pushNull();
private:
    std::vector<ECCommand*> listCommands;
    std::vector<ECCommand *> listUndo;
    int posCurrCmd;
};
// ---------------------------------COMMAND HISTORY------------------------------------ //

#endif