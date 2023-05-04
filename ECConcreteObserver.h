#ifndef ECPART1_H
#define ECPART1_H

#include "ECObserver.h"
#include "ECTextViewImp.h"
#include "ECMVC.h"


// ---------------------------------ESCAPE------------------------------------ //
class ECEscapeClass : public ECObserver{
public:
    ECEscapeClass(ECTextViewImp &view, ECTextViewImpCtrl &ctrl);
    virtual ~ECEscapeClass();
    virtual void Update() override;
private:
    ECTextViewImp& view_pointer;
    ECTextViewImpCtrl& ctrl;
};
// ---------------------------------ESCAPE------------------------------------ //

// ---------------------------------CURSOR------------------------------------ //
class ECCursor : public ECObserver {
public:
    ECCursor(ECTextViewImp &view, ECTextViewImpCtrl &ctrl);
    virtual ~ECCursor();
    virtual void Update() override;
private:
    ECTextViewImp &view_pointer;
    ECTextViewImpCtrl &ctrl;
};
// ---------------------------------CURSOR------------------------------------ //

// ---------------------------------TEXT HANDLER------------------------------------ //
class ECTextHandler : public ECObserver {
public:
    ECTextHandler(ECTextViewImp &view, ECTextViewImpCtrl &ctrl);
    virtual ~ECTextHandler();
    virtual void Update() override;
private:
    ECTextViewImp &view_pointer;
    ECTextViewImpCtrl &ctrl;
};
// ---------------------------------TEXT HANDLER------------------------------------ //

// ---------------------------------MODES------------------------------------ //
class ECModeEdit : public ECObserver {
public:
    ECModeEdit(ECTextViewImp &view, ECTextViewImpCtrl &ctrl);
    virtual ~ECModeEdit();
    virtual void Update() override;
private:
    ECTextViewImp &view_pointer;
    ECTextViewImpCtrl &ctrl;
};

class ECModeCommand : public ECObserver {
public:
    ECModeCommand(ECTextViewImp &view, ECTextViewImpCtrl &ctrl);
    virtual ~ECModeCommand();
    virtual void Update() override;
private:
    ECTextViewImp &view_pointer;
    ECTextViewImpCtrl &ctrl;
};
// ---------------------------------MODES------------------------------------ //

// ---------------------------------UNDO/REDO------------------------------------ //
class ECUndoRedo : public ECObserver {
public:
    ECUndoRedo(ECTextViewImp &view, ECTextViewImpCtrl &ctrl);
    virtual ~ECUndoRedo();
    virtual void Update() override;
private:
    ECTextViewImp &view_pointer;
    ECTextViewImpCtrl &ctrl;
};
// ---------------------------------UNDO/REDO------------------------------------ //

#endif