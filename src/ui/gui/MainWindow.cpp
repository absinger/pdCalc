// Copyright 2016 Adam B. Singer
// Contact: PracticalDesignBook@gmail.com
//
// This file is part of pdCalc.
//
// pdCalc is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// pdCalc is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with pdCalc; if not, see <http://www.gnu.org/licenses/>.

#include "MainWindow.h"
#include "Display.h"
#include "InputWidget.h"
#include <QVBoxLayout>
#include <string>
#include "backend/Stack.h"
#include <memory>
#include <QGridLayout>
#include <iostream>
#include "CommandButton.h"
#include <QValidator>
#include "LookAndFeel.h"
#include "StoredProcedureDialog.h"
#include <sstream>
#include "GuiModel.h"

using std::ostringstream;
using std::cout;
using std::endl;
using std::string;
using std::vector;

namespace pdCalc {

class MainWindow::MainWindowImpl : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindowImpl(MainWindow* parent);
    void showMessage(const string& m);
    void stackChanged();
    void setupFinalButtons();
    void addCommandButton(const std::string& dispPrimaryCmd, const std::string& primaryCmd, const std::string& dispShftCmd, const std::string& shftCmd);

public slots:
    void onCommandEntered(std::string cmd);
    void onShowMessage(std::string m);

private slots:
    void onProcedure();

private:
    void connectInputToModel();
    void doLayout();
    void setupShift();
    void allocateCoreCommandButtons();
    CommandButton* allocAndConnect(const std::string& cmd, const std::string& disp);
    CommandButton* allocAndConnect(const std::string& pCmd, const std::string& pDisp, const std::string& sCmd, const std::string& sDisp);

    MainWindow& parent_;
    int nLinesStack_;
    Display* display_;
    InputWidget* inputWidget_;
    QGridLayout* gridLayout_;
    GuiModel* guiModel_;
    int nAddedButtons_;
};

MainWindow::MainWindowImpl::MainWindowImpl(MainWindow* parent)
: QWidget{parent}
, parent_(*parent)
, nLinesStack_{6}
, nAddedButtons_{0}
{
    guiModel_ = new GuiModel{this};
    display_ = new Display{*guiModel_, this, nLinesStack_};

    connect(guiModel_, SIGNAL(modelChanged()), display_, SLOT(onModelChanged()));

    gridLayout_ = new QGridLayout;

    inputWidget_ = new InputWidget{gridLayout_, this};

    setupShift();

    doLayout();

    connectInputToModel();

    allocateCoreCommandButtons();
}

void MainWindow::MainWindowImpl::doLayout()
{
    auto vblayout = new QVBoxLayout{this};
    vblayout->addWidget(display_);
    vblayout->addLayout(gridLayout_);
    gridLayout_->setOriginCorner(Qt::BottomLeftCorner);
    vblayout->addStretch();
}

void MainWindow::MainWindowImpl::setupShift()
{
    auto shiftButton = new CommandButton{"Shift", "", this};
    gridLayout_->addWidget(shiftButton, 4, 3);
    connect(shiftButton, SIGNAL(clicked(std::string,std::string)), guiModel_, SLOT(onShift()));
    shiftButton->registerShortcut(Qt::Key_S);
    shiftButton->setButtonTextColor( LookAndFeel::Instance().getShiftColor() );

    return;
}

void MainWindow::MainWindowImpl::showMessage(const string& m)
{
    display_->showMessage(m);

    return;
}

void MainWindow::MainWindowImpl::stackChanged()
{
    auto v = Stack::Instance().getElements(nLinesStack_);
    guiModel_->stackChanged(v);

    return;
}

void MainWindow::MainWindowImpl::onProcedure()
{
    StoredProcedureDialog dialog;
    if(dialog.exec() == QDialog::Accepted)
    {
        ostringstream oss;
        oss << "proc:" << dialog.getProcedure();
        guiModel_->onCommandEntered(oss.str(), "");
    }

    return;
}

void MainWindow::MainWindowImpl::connectInputToModel()
{
    connect(inputWidget_, SIGNAL(backspacePressed()), guiModel_, SLOT(onBackspace()));
    connect(inputWidget_, SIGNAL(enterPressed()), guiModel_, SLOT(onEnter()));
    connect(inputWidget_, SIGNAL(characterEntered(char)), guiModel_, SLOT(onCharacterEntered(char)));
    connect(inputWidget_, SIGNAL(plusMinusPressed()), guiModel_, SLOT(onPlusMinus()));
    connect(guiModel_, SIGNAL(commandEntered(std::string)), this, SLOT(onCommandEntered(std::string)));
    connect(guiModel_, SIGNAL(errorDetected(std::string)), this, SLOT(onShowMessage(std::string)));

    return;
}

CommandButton* MainWindow::MainWindowImpl::allocAndConnect(const std::string& cmd, const std::string& disp)
{
    auto btn = new CommandButton{cmd, disp, this};
    connect(btn, SIGNAL(clicked(std::string,std::string)), guiModel_, SLOT(onCommandEntered(std::string, std::string)));

    return btn;
}

CommandButton* MainWindow::MainWindowImpl::allocAndConnect(const std::string& pCmd, const std::string& pDisp, const std::string& sCmd, const std::string& sDisp)
{
    auto btn = new CommandButton{pCmd, pDisp, sCmd, sDisp, this};
    connect(btn, SIGNAL(clicked(std::string,std::string)), guiModel_, SLOT(onCommandEntered(std::string, std::string)));

    return btn;
}

void MainWindow::MainWindowImpl::allocateCoreCommandButtons()
{
    auto plus = allocAndConnect("+", "+");
    // oddly required for the + above the = on my US keyboard
    plus->registerShortcut(QKeySequence{Qt::SHIFT | Qt::Key_Plus});
    plus->registerShortcut(QKeySequence{Qt::Key_Plus});
    gridLayout_->addWidget(plus, 0, 3);

    auto minus = allocAndConnect("-", "-");
    minus->registerShortcut(Qt::Key_Minus);
    gridLayout_->addWidget(minus, 1, 3);

    auto multiply = allocAndConnect("*", "*");
    // oddly required for the * above the 8 on my US keyboard
    multiply->registerShortcut(QKeySequence{Qt::SHIFT | Qt::Key_Asterisk});
    multiply->registerShortcut(QKeySequence{Qt::Key_Asterisk});
    gridLayout_->addWidget(multiply, 2, 3);

    auto divide = allocAndConnect("/", "/");
    divide->registerShortcut(Qt::Key_Slash);
    gridLayout_->addWidget(divide, 3, 3);

    auto sin = allocAndConnect("sin", "sin", "asin", "arcsin");
    gridLayout_->addWidget(sin, 5, 0);

    auto cos = allocAndConnect("cos", "cos", "acos", "arccos");
    gridLayout_->addWidget(cos, 5, 1);

    auto tan = allocAndConnect("tan", "tan", "atan", "arctan");
    gridLayout_->addWidget(tan, 5, 2);

    auto pow = allocAndConnect("pow", "pow", "root", "root");
    gridLayout_->addWidget(pow, 5, 3);

    return;
}

void MainWindow::MainWindowImpl::setupFinalButtons()
{
    int atRow = gridLayout_->rowCount();
    auto undo = allocAndConnect("undo", "undo");
    undo->registerShortcut( QKeySequence{QKeySequence::Undo} );
    gridLayout_->addWidget(undo, atRow, 0);

    auto redo = allocAndConnect("redo", "redo");
    gridLayout_->addWidget(redo, atRow, 1);
    redo->registerShortcut( QKeySequence{QKeySequence::Redo} );

    auto procedure = new CommandButton{"proc", "proc", this};
    connect(procedure, SIGNAL(clicked(std::string, std::string)), this, SLOT(onProcedure()));
    gridLayout_->addWidget(procedure, atRow, 2);
    procedure->registerToolTip("Open a dialog to enter a procedure");
}

void MainWindow::MainWindowImpl::addCommandButton(const string& dispPrimaryCmd, const string& primaryCmd,
const string& dispShftCmd, const string& shftCmd)
{
    int nCols{ gridLayout_->columnCount() };
    int nRows{ gridLayout_->rowCount() };
    int atCol{ nAddedButtons_ % nCols };
    int atRow{(atCol == 0 ? nRows : nRows - 1) };

    auto cb = allocAndConnect(dispPrimaryCmd, primaryCmd, dispShftCmd, shftCmd);
    gridLayout_->addWidget(cb, atRow, atCol);

    nAddedButtons_++;

    return;
}

void MainWindow::MainWindowImpl::onCommandEntered(std::string cmd)
{
    parent_.UserInterface::raise(UserInterface::CommandEntered, std::make_shared<CommandData>(cmd));

    return;
}

void MainWindow::MainWindowImpl::onShowMessage(std::string m)
{
    showMessage(m);
}

MainWindow::MainWindow(int, char*[], QWidget* parent)
: QMainWindow{parent}
{
    pimpl_ = new MainWindowImpl{this};
    setCentralWidget(pimpl_);
}

void MainWindow::postMessage(const std::string& m)
{
    pimpl_->showMessage(m);

    return;
}

void MainWindow::stackChanged()
{
    pimpl_->stackChanged();
}

void MainWindow::addCommandButton(const string& dispPrimaryCmd, const string& primaryCmd,
const string& dispShftCmd, const string& shftCmd)
{
    pimpl_->addCommandButton(dispPrimaryCmd, primaryCmd, dispShftCmd, shftCmd);
}

void MainWindow::setupFinalButtons()
{
    pimpl_->setupFinalButtons();

    return;
}

void MainWindow::fixSize()
{
    setFixedSize( size() );
}

}

#include "MainWindow.moc"
