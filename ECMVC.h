#ifndef ECMVC_H
#define ECMVC_H

#include <iostream>
#include <vector>
#include <string>
#include "ECCommand.h"
#include "ECTextViewImp.h"

using namespace std;

class ECTextViewImpModel;

// ---------------------------------INSERT------------------------------------ //
class ECInsertCmd : public ECCommand {
public:
    ECInsertCmd(ECTextViewImpModel &textIn, char chIn, int posX, int posY, int lRow);
    virtual ~ECInsertCmd() {}
    virtual void Execute();
    virtual void UnExecute();
private:
    ECTextViewImpModel &text;
    char chInput;
    int posX;
    int posY;
    int lRow;
};
// ---------------------------------INSERT------------------------------------ //

// ---------------------------------REMOVE------------------------------------ //
class ECRemoveMergeCmd : public ECCommand {
public:
    ECRemoveMergeCmd(ECTextViewImpModel &textIn, int posY);
    virtual ~ECRemoveMergeCmd() {}
    virtual void Execute();
    virtual void UnExecute();
private:
    ECTextViewImpModel &text;
    int posY;
    int currRow;
};

class ECRemoveCmd : public ECCommand {
public:
    ECRemoveCmd(ECTextViewImpModel &textIn, int posX, int posY, int lRow);
    virtual ~ECRemoveCmd() {}
    virtual void Execute();
    virtual void UnExecute();
private:
    ECTextViewImpModel &text;
    char rChar;
    int posX;
    int posY;
    int lRow;
};  
// ---------------------------------REMOVE------------------------------------ //

// ---------------------------------ENTER------------------------------------ //
class ECEnterCmd : public ECCommand {
public:
    ECEnterCmd(ECTextViewImpModel &textIn, int posX, int posY);
    virtual ~ECEnterCmd() {}
    virtual void Execute();
    virtual void UnExecute();
private:
    ECTextViewImpModel &text;
    // char chInput;
    int posX;
    int posY;
    int n_posX;
    int n_posY;
};  
// ---------------------------------ENTER------------------------------------ //

// ---------------------------------CONTROLLER------------------------------------ //
class ECTextViewImpCtrl {
public:
    ECTextViewImpCtrl(ECTextViewImpModel &textIn, ECTextViewImp &viewIn);
    virtual ~ECTextViewImpCtrl() {}
    void Escape();
    void InsertText(char chIn);
    void Backspace();
    void BreakLine();

    void MoveCursorUp();
    void MoveCursorDown();
    void MoveCursorLeft();
    void MoveCursorRight();

    void Undo();
    void Redo();

    void ModeChange(bool modeIn);
    void SaveFile();

    void AddEditObs(ECObserver* pObs) {editMode.push_back(pObs);}
    void AddCommandObs(ECObserver* pObs) {commandMode.push_back(pObs);}

    void MoveCursorToValidPos();
    void ClearCommands();

private:
    void KeywordHighlight(vector<string> listIn);
    void RefreshView();
    void HandleObservers();

    ECTextViewImpModel &text;
    ECTextViewImp &view;
    ECCommandHistory histCmds;


    //stuff i added
    std::vector<ECCommand *> listCommands;
    std::vector<ECCommand *> listUndo;

    bool mode;

    vector<ECObserver*> editMode;
    vector<ECObserver*> commandMode;
};
// ---------------------------------CONTROLLER------------------------------------ //

// ---------------------------------MODEL------------------------------------ //
class ECTextViewImpModel {
public:
    ECTextViewImpModel(string file_name);
    virtual ~ECTextViewImpModel();
    const vector<string>& GetText() const {return listStrings;}

    void ReadFromFile();
    void SaveFile();
    void SetText(vector<string> listIn);

    void InsertText(int posX, int posY, char ch, int lRow);
    void RemoveText(int posX, int posY);
    void BreakLine(int posX, int posY);
    void MergeLine(int posX, int posY);
private:
    void TextWrapping(int posX, int posY, int lRow);
    vector<string> listStrings;
    string file_name;
};
// ---------------------------------MODEL------------------------------------ //

#endif