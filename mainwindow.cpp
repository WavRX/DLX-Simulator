// Made by WavRX


#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <sstream>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QMenu *fileMenu = new QMenu("File");
    QMenu *editMenu = new QMenu("Edit");
    QMenu *aboutMenu = new QMenu("About");
    ui->menuBar->addMenu(fileMenu);
    ui->menuBar->addMenu(editMenu);
    ui->menuBar->addMenu(aboutMenu);

    //program counter by default at location 0x0
    PC = 0;

    //define instruction types, I,R and J
    //R type instructions
    insDef.append( QPair<QString, QString>("ADD", "R"));
    insDef.append( QPair<QString, QString>("SUB", "R"));
    insDef.append( QPair<QString, QString>("XOR", "R"));
    insDef.append( QPair<QString, QString>("AND", "R"));
    insDef.append( QPair<QString, QString>("OR",  "R"));
    insDef.append( QPair<QString, QString>("SEQ", "R"));
    insDef.append( QPair<QString, QString>("SLE", "R"));
    insDef.append( QPair<QString, QString>("SSL", "R"));
    insDef.append( QPair<QString, QString>("SLT", "R"));
    insDef.append( QPair<QString, QString>("SNE", "R"));
    insDef.append( QPair<QString, QString>("SRA", "R"));
    insDef.append( QPair<QString, QString>("SRL", "R"));

    //I type instructions
    insDef.append( QPair<QString, QString>("ADDI", "I"));
    insDef.append( QPair<QString, QString>("SUBI", "I"));
    insDef.append( QPair<QString, QString>("ANDI", "I"));
    insDef.append( QPair<QString, QString>("BEQZ", "I"));
    insDef.append( QPair<QString, QString>("BNEZ", "I"));
    insDef.append( QPair<QString, QString>("JR", "I"));
    insDef.append( QPair<QString, QString>("LHI", "I"));
    insDef.append( QPair<QString, QString>("LW", "I"));
    insDef.append( QPair<QString, QString>("SW", "I"));
    insDef.append( QPair<QString, QString>("ORI", "I"));
    insDef.append( QPair<QString, QString>("SEQI", "I"));
    insDef.append( QPair<QString, QString>("SLEI", "I"));
    insDef.append( QPair<QString, QString>("SLLI", "I"));
    insDef.append( QPair<QString, QString>("SLTI", "I"));
    insDef.append( QPair<QString, QString>("SNEI", "I"));
    insDef.append( QPair<QString, QString>("SRAI", "I"));
    insDef.append( QPair<QString, QString>("SRLI", "I"));
    insDef.append( QPair<QString, QString>("XORI", "I"));

    //J type instructions
    insDef.append( QPair<QString, QString>("JAL", "J"));
    insDef.append( QPair<QString, QString>("J", "J"));


    PopulateRegistersTable();
    PopulateMemoryTable();

    ui->OutputListWidget->addItem("Welcome to DLX simulator");


    ui->statusBar->showMessage("Made by WavRX");

}

void MainWindow::PopulateRegistersTable()
{
    //DLX registers range from r0 to r31
    //re fill register table with default values

    for(int i = 0; i<32; i++)
    {
        QTableWidgetItem *newItem = new QTableWidgetItem("r" + QString::number(i));
        QTableWidgetItem *zeroItem = new QTableWidgetItem(QString::number(00000000,16).rightJustified(8, '0').toUpper());
        ui->RegistersTableView->setItem(i, 0, newItem);
        ui->RegistersTableView->setItem(i, 1, zeroItem);
    }
}

void MainWindow::PopulateMemoryTable()
{
    //populate memory slots by default values, zeros
    //Here we have a memory of 1kb = 1000bytes
    int j = 0;
    for(int i =0; i<999; i++)
    {
        //each memory address points to 4bytes in memory, 1 word
        //for each index add 4 to get memory address
        QTableWidgetItem *newItem = new QTableWidgetItem("x" + QString::number(j,16).rightJustified(5, '0').toUpper());
        QTableWidgetItem *zeroItem = new QTableWidgetItem(QString::number(00000000,16).rightJustified(8, '0').toUpper());
        ui->MemoryTableView->insertRow(ui->MemoryTableView->rowCount());
        ui->MemoryTableView->setItem(ui->MemoryTableView->rowCount()-1, 0, newItem);
        ui->MemoryTableView->setItem(ui->MemoryTableView->rowCount()-1, 1, zeroItem);
        j+=4;
    }
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_LoadButton_clicked()
{
    //check if instruction input is empty
    if(ui->InstructionInput->text() == "")
        ui->OutputListWidget->addItem("Error: Input is empty!");
    else
    {
        //Validate instruction input
        QString Operation = ui->InstructionInput->text().split(" ").at(0);
        int RType = insDef.indexOf(QPair<QString, QString>(Operation, "R"));
        int IType = insDef.indexOf(QPair<QString, QString>(Operation, "I"));
        int JType = insDef.indexOf(QPair<QString, QString>(Operation, "J"));
        if (RType != -1 || IType != -1 || JType != -1)
        {
            //if instruction type is valid
          ui->InstructionsListView->addItem(ui->InstructionInput->text());
          ui->OutputListWidget->addItem("Loaded inst. into Mem");
          ui->InstructionInput->clear();
        }
        else
        {
            ui->OutputListWidget->addItem("Error: Not recognized");
        }
    }
}



void MainWindow::on_StepButton_clicked()
{
    //execute instruction at current PC
    QListWidgetItem *instructionIte =  ui->InstructionsListView->item(PC/4);
    ui->OutputListWidget->addItem("Executing inst. at PC: 0x" + QString::number(PC,16).rightJustified(8,'0').toUpper());
    ui->OutputListWidget->addItem("-> " + instructionIte->text());

    //Extract operation
    QString instruction = instructionIte->text();
    QString Operation = instruction.split(" ").at(0);
    int RType = insDef.indexOf(QPair<QString, QString>(Operation, "R"));
    int IType = insDef.indexOf(QPair<QString, QString>(Operation, "I"));
    int JType = insDef.indexOf(QPair<QString, QString>(Operation, "J"));
    instruction.insert(instruction.indexOf(" "), QString(","));
    instruction.remove(' ');

    //Perform R type instruction
    if(RType != -1)
    {
        QStringList list2 = instruction.split(',', QString::SkipEmptyParts);
        //Operation
        //ui->OutputListWidget->addItem(list2.at(0));
        QString rd = list2.at(1);
        QString rs1 = list2.at(2);
        QString rs2 = list2.at(3);
        int RS1 = 0;
        int RS2 = 0;
        int RD = 0;

        //get rs1 register
        QList<QTableWidgetItem *> ItemList1 = ui->RegistersTableView->findItems(rs1, Qt::MatchExactly);
        if(ItemList1.count()>0)
        {
            QTableWidgetItem *registerValue =  ui->RegistersTableView->item(ItemList1[0]->row(),1);
            ui->OutputListWidget->addItem(registerValue->text());
            std::stringstream ss;
            ss << std::hex <<registerValue->text().toStdString();
            ss >> RS1;
            ui->OutputListWidget->addItem("rs1=0x" + registerValue->text());
        }

        //get rs2 register
        QList<QTableWidgetItem *> ItemList2 = ui->RegistersTableView->findItems(rs2, Qt::MatchExactly);
        if(ItemList2.count()>0)
        {
            QTableWidgetItem *registerValue =  ui->RegistersTableView->item(ItemList2[0]->row(),1);
            std::stringstream ss;
            ss << std::hex <<registerValue->text().toStdString();
            ss >> RS2;
            ui->OutputListWidget->addItem("rs2=0x" +registerValue->text());
        }

        //get rd register
        QList<QTableWidgetItem *> ItemListd = ui->RegistersTableView->findItems(rd, Qt::MatchExactly);
        if(ItemListd.count()>0)
        {
            QTableWidgetItem *registerValue =  ui->RegistersTableView->item(ItemListd[0]->row(),1);
            std::stringstream ss;
            ss << std::hex <<registerValue->text().toStdString();
            ss >> RD;
            ui->OutputListWidget->addItem("rd=0x" + registerValue->text());
        }

        if(Operation == "ADD")
        {
            RD = RS1 + RS2;
            ui->OutputListWidget->addItem("Sum = 0x" + QString::number(RD, 16).rightJustified(8,'0'));
            //update register table view
            QList<QTableWidgetItem *> ItemList = ui->RegistersTableView->findItems(rd, Qt::MatchExactly);
            if(ItemList.count()>0)
            {
                QTableWidgetItem *registerValue =  ui->RegistersTableView->item(ItemList[0]->row(),1);
                registerValue->setText(QString::number(RD,16).rightJustified(8,'0'));
                ui->RegistersTableView->itemChanged(registerValue);
            }

        }
        else if(Operation == "SUB")
        {
                RD = RS1 - RS2;
                ui->OutputListWidget->addItem("Sum = 0x" + QString::number(RD, 16).rightJustified(8,'0'));
                //update register table view
                QList<QTableWidgetItem *> ItemList = ui->RegistersTableView->findItems(rd, Qt::MatchExactly);
                if(ItemList.count()>0)
                {
                    QTableWidgetItem *registerValue =  ui->RegistersTableView->item(ItemList[0]->row(),1);
                    registerValue->setText(QString::number(RD,16).rightJustified(8,'0'));
                    ui->RegistersTableView->itemChanged(registerValue);
                }

        }
        else if(Operation == "AND")
        {
                RD = RS1 & RS2;
                ui->OutputListWidget->addItem("Sum = 0x" + QString::number(RD, 16).rightJustified(8,'0'));
                //update register table view
                QList<QTableWidgetItem *> ItemList = ui->RegistersTableView->findItems(rd, Qt::MatchExactly);
                if(ItemList.count()>0)
                {
                    QTableWidgetItem *registerValue =  ui->RegistersTableView->item(ItemList[0]->row(),1);
                    registerValue->setText(QString::number(RD,16).rightJustified(8,'0'));
                    ui->RegistersTableView->itemChanged(registerValue);
                }

        }
        else if(Operation == "OR")
        {
                RD = RS1 | RS2;
                ui->OutputListWidget->addItem("Sum = 0x" + QString::number(RD, 16).rightJustified(8,'0'));
                //update register table view
                QList<QTableWidgetItem *> ItemList = ui->RegistersTableView->findItems(rd, Qt::MatchExactly);
                if(ItemList.count()>0)
                {
                    QTableWidgetItem *registerValue =  ui->RegistersTableView->item(ItemList[0]->row(),1);
                    registerValue->setText(QString::number(RD,16).rightJustified(8,'0'));
                    ui->RegistersTableView->itemChanged(registerValue);
                }

        }
        else if(Operation == "XOR")
        {
                RD = RS1 ^ RS2;
                ui->OutputListWidget->addItem("Sum = 0x" + QString::number(RD, 16).rightJustified(8,'0'));
                //update register table view
                QList<QTableWidgetItem *> ItemList = ui->RegistersTableView->findItems(rd, Qt::MatchExactly);
                if(ItemList.count()>0)
                {
                    QTableWidgetItem *registerValue =  ui->RegistersTableView->item(ItemList[0]->row(),1);
                    registerValue->setText(QString::number(RD,16).rightJustified(8,'0'));
                    ui->RegistersTableView->itemChanged(registerValue);
                }

        }
        else if(Operation == "SEQ")
        {
                RD = (RS1 == RS2 ?1:0);
                ui->OutputListWidget->addItem("Sum = 0x" + QString::number(RD, 16).rightJustified(8,'0'));
                //update register table view
                QList<QTableWidgetItem *> ItemList = ui->RegistersTableView->findItems(rd, Qt::MatchExactly);
                if(ItemList.count()>0)
                {
                    QTableWidgetItem *registerValue =  ui->RegistersTableView->item(ItemList[0]->row(),1);
                    registerValue->setText(QString::number(RD,16).rightJustified(8,'0'));
                    ui->RegistersTableView->itemChanged(registerValue);
                }

        }
        else if(Operation == "SLE")
        {
                RD = (RS1 <= RS2 ?1:0);
                ui->OutputListWidget->addItem("Sum = 0x" + QString::number(RD, 16).rightJustified(8,'0'));
                //update register table view
                QList<QTableWidgetItem *> ItemList = ui->RegistersTableView->findItems(rd, Qt::MatchExactly);
                if(ItemList.count()>0)
                {
                    QTableWidgetItem *registerValue =  ui->RegistersTableView->item(ItemList[0]->row(),1);
                    registerValue->setText(QString::number(RD,16).rightJustified(8,'0'));
                    ui->RegistersTableView->itemChanged(registerValue);
                }

        }


    }
    //I type instructions
    else if(IType != -1)
    {
        QStringList list2 = instruction.split(',', QString::SkipEmptyParts);
        //Operation
        //ui->OutputListWidget->addItem(list2.at(0));
        QString rd = list2.at(2);
        QString rs1 = list2.at(1);
        QString immediate = list2.at(3);
        int RS1 = 0;
        int Immediate=0;
        std::stringstream ss;
        ss << std::hex <<immediate.toStdString();
        ss >> Immediate;

        ui->OutputListWidget->addItem("Immediate= 0x"+immediate);
        int RD = 0;

        //get rs1 register
        QList<QTableWidgetItem *> ItemList1 = ui->RegistersTableView->findItems(rs1, Qt::MatchExactly);
        if(ItemList1.count()>0)
        {
            QTableWidgetItem *registerValue =  ui->RegistersTableView->item(ItemList1[0]->row(),1);
            ui->OutputListWidget->addItem(registerValue->text());
            std::stringstream ss;
            ss << std::hex <<registerValue->text().toStdString();
            ss >> RS1;
            ui->OutputListWidget->addItem("rs1=0x" + registerValue->text());
        }


        //get rd register
        QList<QTableWidgetItem *> ItemListd = ui->RegistersTableView->findItems(rd, Qt::MatchExactly);
        if(ItemListd.count()>0)
        {
            QTableWidgetItem *registerValue =  ui->RegistersTableView->item(ItemListd[0]->row(),1);
            std::stringstream ss;
            ss << std::hex <<registerValue->text().toStdString();
            ss >> RD;
            ui->OutputListWidget->addItem("rd=0x" + registerValue->text());
        }

        if(Operation == "LW")
        {
            RD = RS1 + Immediate;

            ui->OutputListWidget->addItem("rd= 0x" + QString::number(RD, 16).rightJustified(8,'0'));
            //update register table view
            QList<QTableWidgetItem *> ItemList = ui->RegistersTableView->findItems(rd, Qt::MatchExactly);
            if(ItemList.count()>0)
            {
                QTableWidgetItem *registerValue =  ui->RegistersTableView->item(ItemList[0]->row(),1);
                registerValue->setText(QString::number(RD,16).rightJustified(8,'0'));
                ui->RegistersTableView->itemChanged(registerValue);
            }

        }
        if(Operation == "LHI")
        {
            RD = Immediate << 16;

            ui->OutputListWidget->addItem("rd= 0x" + QString::number(RD, 16).rightJustified(8,'0'));
            //update register table view
            QList<QTableWidgetItem *> ItemList = ui->RegistersTableView->findItems(rd, Qt::MatchExactly);
            if(ItemList.count()>0)
            {
                QTableWidgetItem *registerValue =  ui->RegistersTableView->item(ItemList[0]->row(),1);
                registerValue->setText(QString::number(RD,16).rightJustified(8,'0'));
                ui->RegistersTableView->itemChanged(registerValue);
            }

        }
        else if(Operation == "SW")
        {
            //memory at rs1 + immediate = rd
            int address = RS1 + Immediate;
            QList<QTableWidgetItem *> ItemList = ui->MemoryTableView->findItems("x"+QString::number(address,16).rightJustified(5,'0'),Qt::MatchExactly);
            if(ItemList.count()>0)
            {
                QTableWidgetItem *memoryValue =  ui->MemoryTableView->item(ItemList[0]->row(),1);
                memoryValue->setText(QString::number(RD,16).rightJustified(8,'0'));
                ui->MemoryTableView->itemChanged(memoryValue);
            }
        }
        else if(Operation == "ADDI")
        {
            RD = RS1 + Immediate;
            ui->OutputListWidget->addItem("rd= 0x" + QString::number(RD, 16).rightJustified(8,'0'));
            //update register table view
            QList<QTableWidgetItem *> ItemList = ui->RegistersTableView->findItems(rd, Qt::MatchExactly);
            if(ItemList.count()>0)
            {
                QTableWidgetItem *registerValue =  ui->RegistersTableView->item(ItemList[0]->row(),1);
                registerValue->setText(QString::number(RD,16).rightJustified(8,'0'));
                ui->RegistersTableView->itemChanged(registerValue);
            }
        }
        else if(Operation == "SUBI")
        {
            RD = RS1 - Immediate;
            ui->OutputListWidget->addItem("rd= 0x" + QString::number(RD, 16).rightJustified(8,'0'));
            //update register table view
            QList<QTableWidgetItem *> ItemList = ui->RegistersTableView->findItems(rd, Qt::MatchExactly);
            if(ItemList.count()>0)
            {
                QTableWidgetItem *registerValue =  ui->RegistersTableView->item(ItemList[0]->row(),1);
                registerValue->setText(QString::number(RD,16).rightJustified(8,'0'));
                ui->RegistersTableView->itemChanged(registerValue);
            }
        }
        else if(Operation == "ANDI")
        {
            RD = RS1 & Immediate;
            ui->OutputListWidget->addItem("rd= 0x" + QString::number(RD, 16).rightJustified(8,'0'));
            //update register table view
            QList<QTableWidgetItem *> ItemList = ui->RegistersTableView->findItems(rd, Qt::MatchExactly);
            if(ItemList.count()>0)
            {
                QTableWidgetItem *registerValue =  ui->RegistersTableView->item(ItemList[0]->row(),1);
                registerValue->setText(QString::number(RD,16).rightJustified(8,'0'));
                ui->RegistersTableView->itemChanged(registerValue);
            }
        }
        else if(Operation == "ORI")
        {
            RD = RS1 | Immediate;
            ui->OutputListWidget->addItem("rd= 0x" + QString::number(RD, 16).rightJustified(8,'0'));
            //update register table view
            QList<QTableWidgetItem *> ItemList = ui->RegistersTableView->findItems(rd, Qt::MatchExactly);
            if(ItemList.count()>0)
            {
                QTableWidgetItem *registerValue =  ui->RegistersTableView->item(ItemList[0]->row(),1);
                registerValue->setText(QString::number(RD,16).rightJustified(8,'0'));
                ui->RegistersTableView->itemChanged(registerValue);
            }
        }
        else if(Operation == "XORI")
        {
            RD = RS1 ^ Immediate;
            ui->OutputListWidget->addItem("rd= 0x" + QString::number(RD, 16).rightJustified(8,'0'));
            //update register table view
            QList<QTableWidgetItem *> ItemList = ui->RegistersTableView->findItems(rd, Qt::MatchExactly);
            if(ItemList.count()>0)
            {
                QTableWidgetItem *registerValue =  ui->RegistersTableView->item(ItemList[0]->row(),1);
                registerValue->setText(QString::number(RD,16).rightJustified(8,'0'));
                ui->RegistersTableView->itemChanged(registerValue);
            }
        }
        else if(Operation == "SEQI")
        {
            RD = (RS1 == Immediate ? 1: 0);
            ui->OutputListWidget->addItem("rd= 0x" + QString::number(RD, 16).rightJustified(8,'0'));
            //update register table view
            QList<QTableWidgetItem *> ItemList = ui->RegistersTableView->findItems(rd, Qt::MatchExactly);
            if(ItemList.count()>0)
            {
                QTableWidgetItem *registerValue =  ui->RegistersTableView->item(ItemList[0]->row(),1);
                registerValue->setText(QString::number(RD,16).rightJustified(8,'0'));
                ui->RegistersTableView->itemChanged(registerValue);
            }
        }
        else if(Operation == "SLEI")
        {
            RD = (RS1 <= Immediate ? 1: 0);
            ui->OutputListWidget->addItem("rd= 0x" + QString::number(RD, 16).rightJustified(8,'0'));
            //update register table view
            QList<QTableWidgetItem *> ItemList = ui->RegistersTableView->findItems(rd, Qt::MatchExactly);
            if(ItemList.count()>0)
            {
                QTableWidgetItem *registerValue =  ui->RegistersTableView->item(ItemList[0]->row(),1);
                registerValue->setText(QString::number(RD,16).rightJustified(8,'0'));
                ui->RegistersTableView->itemChanged(registerValue);
            }
        }
        else if(Operation == "BEQZ")
        {
                PC+=(RS1 == 0? Immediate : 0);
                ui->OutputListWidget->addItem("PC = 0x" + QString::number(PC, 16).rightJustified(8,'0'));
        }
        else if(Operation == "BNEZ")
        {
                PC+=(RS1 != 0? Immediate : 0);
                ui->OutputListWidget->addItem("PC = 0x" + QString::number(PC, 16).rightJustified(8,'0'));
        }
        else if(Operation == "JR")
        {
                PC = (RS1);
                ui->OutputListWidget->addItem("PC = 0x" + QString::number(PC, 16).rightJustified(8,'0'));
        }


    }
    //increment program counter by 4
    PC +=4;
}
