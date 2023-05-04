// Test code for editor
#include "ECTextViewImp.h"
#include "ECConcreteObserver.h"
#include "ECMVC.h"
#include "ECCommand.h"
#include <iostream>
#include <fstream>
#include <unistd.h>

using namespace  std;

int myCounter = 0;

int main(int argc, char *argv[])
{

    if (argc != 2) {
        std::cout << "Usage: ./myeditor [filename]" << std::endl;
        return 1;
    }
    ECTextViewImp wndTest;
    ECTextViewImpModel model(argv[1]);
    ECTextViewImpCtrl ctrl(model, wndTest);
    ECCursor CursorLeft(wndTest, ctrl);
    ECTextHandler TextInsert(wndTest, ctrl);
    ECModeEdit ModeEdit(wndTest, ctrl);
    ECModeCommand ModeCommand(wndTest, ctrl);
    ECUndoRedo UndoRedo(wndTest, ctrl);
    
    ctrl.AddEditObs(&ModeCommand);
    ctrl.AddEditObs(&TextInsert);

    ctrl.AddCommandObs(&ModeEdit);
    ctrl.AddCommandObs(&UndoRedo);

    // wndTest.AddRow("");
    // model.SetText("CSE 3150");
    // wndTest.AddRow("CSE 3150");
    wndTest.SetColor(0, 0, 0, TEXT_COLOR_RED);
    wndTest.SetColor(0, 1, 1, TEXT_COLOR_GREEN);
    wndTest.SetColor(0, 2, 2, TEXT_COLOR_BLUE);
    wndTest.SetColor(0, 4, 4, TEXT_COLOR_RED);
    wndTest.SetColor(0, 5, 5, TEXT_COLOR_GREEN);
    wndTest.SetColor(0, 6, 6, TEXT_COLOR_BLUE);
    wndTest.SetColor(0, 7, 7, TEXT_COLOR_MAGENTA);
    // model.SetText("This is a very simple demo of the ECTextViewImp functionalities.");
    // wndTest.AddRow("This is a very simple demo of the ECTextViewImp functionalities.");
    wndTest.SetColor(1, 10, 13, TEXT_COLOR_GREEN);
    // model.SetText("Press ctrl-q to quit");
    // wndTest.AddRow("Press ctrl-q to quit");
    wndTest.SetColor(2, 0, 1, TEXT_COLOR_GREEN);
    wndTest.SetColor(2, 0, 1, TEXT_COLOR_DEF);

    // add a status bar
    // wndTest.AddStatusRow("Editor", "For demo only", true);
    wndTest.ClearColor();

    ctrl.ModeChange(false);

    wndTest.Show();
    
    return 0;
}
