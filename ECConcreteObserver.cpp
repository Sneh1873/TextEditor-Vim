#include "ECConcreteObserver.h"

// ---------------------------------ESCAPE------------------------------------ //
ECEscapeClass :: ECEscapeClass(ECTextViewImp &view, ECTextViewImpCtrl &ctrl) 
    : view_pointer(view), ctrl(ctrl)
{
    view_pointer.Attach(this);
}

ECEscapeClass :: ~ECEscapeClass() {
    view_pointer.Detach(this);
}

void ECEscapeClass::Update() {
    int key = view_pointer.GetPressedKey();
    if (key == ESC) {
        ctrl.Escape();
    }
}
// ---------------------------------ESCAPE------------------------------------ //

// ---------------------------------CURSOR------------------------------------ //
ECCursor :: ECCursor(ECTextViewImp &view, ECTextViewImpCtrl &ctrl)
    : view_pointer(view), ctrl(ctrl)
{
    view_pointer.Attach(this);
}

ECCursor :: ~ECCursor() {
    view_pointer.Detach(this);
}

void ECCursor :: Update() {
    int key = view_pointer.GetPressedKey();
    if (key == ARROW_RIGHT) {
        ctrl.MoveCursorRight();
    }
    else if (key == ARROW_LEFT) {
        ctrl.MoveCursorLeft();
    }
    else if (key == ARROW_DOWN) {
        ctrl.MoveCursorDown();
    }
    else if (key == ARROW_UP) {
        ctrl.MoveCursorUp();
    }
}
// ---------------------------------CURSOR------------------------------------ //

// ---------------------------------TEXT HANDLER------------------------------------ //
ECTextHandler :: ECTextHandler(ECTextViewImp &view, ECTextViewImpCtrl &ctrl) 
    : view_pointer(view), ctrl(ctrl)
{
}

ECTextHandler :: ~ECTextHandler() {
}

void ECTextHandler :: Update() {
    int key = view_pointer.GetPressedKey();
    if (key >= 32 && key <= 126) {
        char c = static_cast<char>(key);
        ctrl.InsertText(c);
    }
    else if (key == BACKSPACE) {
        ctrl.Backspace();
    }
    else if (key == ENTER) {
        ctrl.BreakLine();
    }
}
// ---------------------------------TEXT HANDLER------------------------------------ //

// ---------------------------------MODES------------------------------------ //
ECModeEdit :: ECModeEdit(ECTextViewImp &view, ECTextViewImpCtrl &ctrl)
    : view_pointer(view), ctrl(ctrl)
{
}

ECModeEdit :: ~ECModeEdit() 
{
}

void ECModeEdit :: Update() {
    int key = view_pointer.GetPressedKey();
    if (key == 105) {
        ctrl.ModeChange(true);
    }
}

ECModeCommand :: ECModeCommand(ECTextViewImp &view, ECTextViewImpCtrl &ctrl)
    : view_pointer(view), ctrl(ctrl)
{
}

ECModeCommand :: ~ECModeCommand() 
{
}

void ECModeCommand :: Update() {
    int key = view_pointer.GetPressedKey();
    if (key == ESC || key == CTRL_A) {
        ctrl.ModeChange(false);
    }
}
// ---------------------------------MODES------------------------------------ //

// ---------------------------------UNDO/REDO------------------------------------ //
ECUndoRedo :: ECUndoRedo(ECTextViewImp &view, ECTextViewImpCtrl &ctrl)
    : view_pointer(view), ctrl(ctrl)
{
}

ECUndoRedo :: ~ECUndoRedo() {
}

void ECUndoRedo :: Update() {
    int key = view_pointer.GetPressedKey();
    if (key == CTRL_Z) {
        ctrl.Undo();
    }
    else if (key == CTRL_Y) {
        ctrl.Redo();
    }
}
// ---------------------------------UNDO/REDO------------------------------------ //
