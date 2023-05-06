#include "ECMVC.h"
#include <iostream>
#include <cctype>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

using namespace std;

// ---------------------------------INSERT------------------------------------ //
ECInsertCmd :: ECInsertCmd(ECTextViewImpModel &textIn, char chIn, int posX, int posY, int lRow) 
    : text(textIn), chInput(chIn), posX(posX), posY(posY), lRow(lRow)
{
}

void ECInsertCmd :: Execute() {
    text.InsertText(posX, posY,chInput, lRow);
}

void ECInsertCmd :: UnExecute() {
    text.RemoveText(posX+1, posY);
}
// ---------------------------------INSERT------------------------------------ //

// ---------------------------------REMOVE------------------------------------ //
ECRemoveMergeCmd :: ECRemoveMergeCmd(ECTextViewImpModel &textIn, int posY)
    : text(textIn), posY(posY), currRow(0)
{
}

void ECRemoveMergeCmd::Execute() {
    vector<string> temp = text.GetText();
        if (posY > 0 && posY < (int)temp.size()) {
            currRow = temp[posY - 1].length();
            temp[posY - 1] += temp[posY];
            temp.erase(temp.begin() + posY);
        }
        text.SetText(temp);
}

void ECRemoveMergeCmd::UnExecute() {
    vector<string> temp = text.GetText();
        temp.insert(temp.begin() + posY, "");
        temp[posY] = temp[posY - 1].substr(currRow) + temp[posY];
        temp[posY - 1] = temp[posY - 1].substr(0, currRow);
        text.SetText(temp);
}

ECRemoveCmd::ECRemoveCmd(ECTextViewImpModel& textIn, int posX, int posY, int lRow)
    : text(textIn), posX(posX), posY(posY), lRow(lRow), rChar('\0')
{
}

void ECRemoveCmd::Execute()
{
    vector<string> temp = text.GetText();
    if (posY >= 0 && posY < (int)temp.size() && posX >= 0 && posX <= (int)temp[posY].length()) {
        rChar = temp[posY][posX];
        text.RemoveText(posX, posY);
    }
}

void ECRemoveCmd::UnExecute()
{
    text.InsertText(posX-1, posY, rChar, lRow);
}
// ---------------------------------REMOVE------------------------------------ //

// ---------------------------------ENTER------------------------------------ //
ECEnterCmd :: ECEnterCmd(ECTextViewImpModel &textIn, int posX, int posY) 
    : text(textIn), posX(posX), posY(posY), n_posX(posX), n_posY(posY)
{
}

void ECEnterCmd :: Execute() {
    text.BreakLine(posX, posY);
    n_posX = posX;
    n_posY = posY;
    posY = 0;
}

void ECEnterCmd :: UnExecute() {
    text.MergeLine(n_posX, n_posY);
    posX = n_posX;
    posY = n_posY;
}
// ---------------------------------ENTER------------------------------------ //

// ---------------------------------CONTROLLER------------------------------------ //
ECTextViewImpCtrl :: ECTextViewImpCtrl(ECTextViewImpModel &textIn, ECTextViewImp &viewIn) 
    : text(textIn), view(viewIn), mode(false)
{
    RefreshView();
}

void ECTextViewImpCtrl::RefreshView() {
    view.ClearColor();
    view.InitRows();
    vector<string> temp = text.GetText();
    int lengthRows = view.GetRowNumInView();
    for (int i = 0; i < min(lengthRows, static_cast<int>(temp.size())); i++) {
        if(temp[i].size() > view.GetColNumInView())
        {
            TextWrap(temp);
        }
        else view.AddRow(temp[i]);
    }
    KeywordHighlight();
    view.Refresh();
}


void ECTextViewImpCtrl :: Escape() {
    view.InitRows();
    view.AddRow("Sneh Patel");
    view.ClearColor();
    view.Refresh();
}

void ECTextViewImpCtrl :: InsertText(char chIn) {
    if (mode) {
        int posX = view.GetCursorX();
        int posY = view.GetCursorY();
        int lRow = view.GetColNumInView();

        ECInsertCmd *pCmd = new ECInsertCmd(text, chIn, posX, posY, lRow);
        histCmds.ExecuteCmd(pCmd);

        // Move cursor to the right
        MoveCursorRight();
        if (posX == lRow) {
            // Move cursor down to the next row
            view.SetCursorY(posY + 1);
            // Set cursor position to the beginning of the row
            view.SetCursorX(0);
            // Update cursor position variables
            posX = 0;
            posY = view.GetCursorY();
        }
        RefreshView();
    }
}

void ECTextViewImpCtrl :: TextWrap(vector<string> listIn) {
    for (const auto& line : listIn) {
    string wrappedLine = "";
    int lineLength = line.length();
    int colNumInView = view.GetColNumInView();

    // Check if the line needs to be wrapped
    if (lineLength > colNumInView) {
        int startIndex = 0;
        int endIndex = 0;

        // Wrap the line
        while (endIndex < lineLength) {
            endIndex = min(startIndex + colNumInView, lineLength);
            wrappedLine += line.substr(startIndex, endIndex - startIndex) + '\n';
            startIndex = endIndex;
        }
    } else {
        wrappedLine = line + '\n';
    }
    view.AddRow(wrappedLine);
    }

}

void ECTextViewImpCtrl::Backspace() {
    if (mode) {
        int posX = view.GetCursorX();
        int posY = view.GetCursorY();
        int lRow = view.GetColNumInView();

        if (posX > 0) {
            ECRemoveCmd *pCmd = new ECRemoveCmd(this->text, posX, posY, lRow);
            histCmds.ExecuteCmd(pCmd);
            MoveCursorLeft();
            RefreshView();
        }
        else if (posY > 0) {
            int prevLength = text.GetText()[posY - 1].length();
            ECRemoveMergeCmd *pCmd = new ECRemoveMergeCmd(this->text, posY);
            histCmds.ExecuteCmd(pCmd);
            
            view.SetCursorX(prevLength); 
            view.SetCursorY(posY - 1);
            RefreshView();
        }
    }
}

void ECTextViewImpCtrl::BreakLine() {
    if (mode) {
        int posX = view.GetCursorX();
        int posY = view.GetCursorY();

        ECEnterCmd *pCmd = new ECEnterCmd(text, posX, posY);
        histCmds.ExecuteCmd(pCmd);

        view.SetCursorX(0);
        MoveCursorDown();
        RefreshView();
    }
}

void ECTextViewImpCtrl::WrapCursor(vector<string> listIn) {
    int cRow = 0;
    int cCol = 0;
    int lRow = view.GetColNumInView();
    int textRow = viewRow;

    // calculate the row and column of the cursor based on wrapped text
    for (int i = 0; i < view.GetCursorX(); ++i) {
        int len = static_cast<int>(listIn[textRow].length());
        cRow += (len + lRow) / lRow;
        textRow += len / lRow + 1;
    }
    int len = static_cast<int>(listIn[textRow].length());
    cRow += view.GetCursorY() / lRow;
    cCol += view.GetCursorY() % lRow;
    if (cRow > view.GetRowNumInView() - 1) {
        viewRow += (cRow - view.GetRowNumInView() + 1);
        if (viewRow + view.GetRowNumInView() > static_cast<int>(listIn.size())) {
            viewRow = static_cast<int>(listIn.size()) - view.GetRowNumInView();
        }
        RefreshView();
        textRow = viewRow;
        cRow = view.GetCursorY() / lRow;
        cCol = view.GetCursorY() % lRow;
    }
    view.SetCursorY(cRow);
    view.SetCursorX(cCol);
}

void ECTextViewImpCtrl :: MoveCursorUp() {
    // Getting the current X and Y positions of the cursor
    int cursorX = view.GetCursorX();
    int cursorY = view.GetCursorY(); 

    // Text from the editor
    const vector<string>& temp = text.GetText();

    // Checking if the cursor is not at the top row
    if (cursorY > 0) {
        // Move cursor up by one row
        view.SetCursorY(cursorY - 1);

        // Getting the length of the row above
        int r_length = temp[cursorY - 1].length(); 

        // Checking if cursor X position is within bounds of the row above
        if (cursorX < r_length) {
            // Setting cursor X position to the same value
            view.SetCursorX(cursorX);
        } else {
            // Setting cursor X position to the end of the row above
            view.SetCursorX(r_length);
        }
    }
}

void ECTextViewImpCtrl::MoveCursorDown() {
    int cursorX = view.GetCursorX();
    int cursorY = view.GetCursorY();
    int rowLimit = view.GetRowNumInView() - 1;

    // get the text from the editor
    const vector<string>& temp = text.GetText();

    if (cursorY < (int)(temp.size() - 1) && cursorY < rowLimit) {
        // move the cursor down by one row
        view.SetCursorY(cursorY + 1);

        // get the length of the row below
        int r_length = temp[cursorY + 1].length();

        // check if cursor X position is within bounds of the row below
        if (cursorX < r_length) {
            // set cursor X position to the same value
            view.SetCursorX(cursorX);
        } else {
            // set cursor X position to the end of the row below
            view.SetCursorX(r_length);
        }
    }
}

void ECTextViewImpCtrl :: MoveCursorLeft() {
    // Getting the current X and Y positions of the cursor
    int cursorX = view.GetCursorX();
    int cursorY = view.GetCursorY(); 
    // Text from the editor
    const vector<string>& temp = text.GetText();

    // Checking if cursor can be moved to the left within the same row
    if (cursorX > 0) {
        // Moving the cursor left by one column
        view.SetCursorX(cursorX - 1);
    }
    // If the cursor is at the beginning of a row, checking if it can be moved to the previous row
    else if (cursorY > 0) {
        // Moving cursor up by one row
        view.SetCursorY(cursorY - 1);
        // Setting the cursorX position to the end of the row above
        view.SetCursorX(temp[cursorY - 1].length());
    }
}

void ECTextViewImpCtrl :: MoveCursorRight() {
    // Getting the current X and Y positions of the cursor
    int cursorX = view.GetCursorX();
    int cursorY = view.GetCursorY();
    int lRow = view.GetColNumInView();
    // Text from the editor
    const vector<string>& temp = text.GetText();
    // Length of the current row
    int length = temp[cursorY].size();

    // Checking if the cursor can be moved to the right within the same row
    if(cursorX < length)
    {
        // Moving cursor right by one column
        view.SetCursorX(cursorX + 1);
    }
}

void ECTextViewImpCtrl::Undo() {
    if (histCmds.Undo()) {
        MoveCursorToValidPos();
        RefreshView();
    }
}


void ECTextViewImpCtrl :: Redo() {
    if (histCmds.Redo()) {
        MoveCursorToValidPos();
        RefreshView();
    }
}

void ECTextViewImpCtrl :: ModeChange(bool modeIn) {
    HandleObservers();
    mode = modeIn;
}

void ECTextViewImpCtrl :: SaveFile() {
    text.SaveFile();
}

void ECTextViewImpCtrl :: HandleObservers() {
    // Attaching the observers
    if (mode) {
        for (auto i : commandMode) {
            view.Detach(i);
        }
        for (auto i : editMode) {
            view.Attach(i);
        }
        view.ClearStatusRows();
        //histCmds.ClearCommands();
        view.AddStatusRow("Editor", "Mode: Edit", true);
    }
    else {
        for (auto i : editMode) {
            view.Detach(i);
        }
        for (auto i : commandMode) {
            view.Attach(i);
        }
        view.ClearStatusRows();
        view.AddStatusRow("Editor", "Mode: Command", true);
    }
}

void ECTextViewImpCtrl :: MoveCursorToValidPos() {
    int numRows = text.GetText().size();
    int cursorY = view.GetCursorY();
    int cursorX = view.GetCursorX();
    if (cursorY >= numRows) {
        cursorY = numRows - 1;
    }
    int rowLen = text.GetText()[cursorY].length();
    if (cursorX > rowLen) {
        cursorX = rowLen;
    }
    view.SetCursorY(cursorY);
    view.SetCursorX(cursorX);
}

void ECTextViewImpCtrl::KeywordHighlight() {
    vector<string> temp = text.GetText();
    vector<string> keywords;
    ifstream keywordFile("keywords.txt");
    if (keywordFile) {
        string word;
        while (getline(keywordFile, word)) {
            keywords.push_back(word);
        }
    }
    keywordFile.close();

    for (int i = 0; i < keywords.size(); i++) {
        string keyword = keywords[i];
        string::size_type pos = 0;
        while (pos < temp.size()) {
            string line = temp[pos];
            string::size_type wordPos = line.find(keyword, 0);
            while (wordPos != string::npos) {
                view.SetColor(pos, wordPos, wordPos + keyword.length() - 1, TEXT_COLOR_BLUE);
                wordPos = line.find(keyword, wordPos + 1);
            }
            pos++;
        }
    }
}

// ---------------------------------CONTROLLER------------------------------------ //

// ---------------------------------MODEL------------------------------------ //
ECTextViewImpModel :: ECTextViewImpModel(string file_name)
    : file_name(file_name)
{ 
    this->ReadFromFile();
}

ECTextViewImpModel :: ~ECTextViewImpModel() {
    this->SaveFile();
}

void ECTextViewImpModel :: SetText(vector<string> listIn) {
    listStrings = listIn;
}

void ECTextViewImpModel :: ReadFromFile() {
    ifstream file(file_name);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            listStrings.push_back(line);
        }
        if (listStrings.empty()) {
            listStrings.push_back("");
        }
        file.close();
    }
    else {
        listStrings.push_back("");
    }
}

void ECTextViewImpModel :: SaveFile() {
    ofstream file(file_name);
    if (file.is_open()) {
        for (const auto& line : listStrings) {
            file << line << endl;
        }
        file.close();
    } else {
        return;
    }
}

void ECTextViewImpModel :: InsertText(int posX, int posY, char ch, int lRow) {
    if ((int)listStrings.size() <= posY) {
        listStrings.push_back("");
        listStrings[posY].push_back(ch);
    } 
    else if (posX < lRow) {
        listStrings[posY].insert(posX, 1, ch); // insert the character at the specified position
    } 
}

void ECTextViewImpModel::RemoveText(int posX, int posY) {
    if (posX > 0 && (int)(listStrings[posY].size()) >= posX) {
        listStrings[posY].erase(posX - 1, 1);
    }
}

void ECTextViewImpModel :: BreakLine(int posX, int posY) {
    if (posY < (int)listStrings.size()) { // If cursor is not at the last line
        string line = listStrings[posY]; 
        string fh = line.substr(0, posX); // Get substring of line before cursor position
        string sh = line.substr(posX); // Get substring of line after cursor position

        listStrings.insert(listStrings.begin() + posY + 1, sh);
        listStrings[posY] = fh;  
    }
    else {
        listStrings.resize(posY + 1); // Resize temp to accommodate the last line
        listStrings.push_back("");
    }
}

void ECTextViewImpModel :: MergeLine(int posX, int posY) {
    if (posY < (int)listStrings.size() - 1) {
        // Merge the next line onto the current line
        listStrings[posY] += listStrings[posY + 1];

        // Remove the next line
        listStrings.erase(listStrings.begin() + posY + 1);
    }
}
// ---------------------------------MODEL------------------------------------ //