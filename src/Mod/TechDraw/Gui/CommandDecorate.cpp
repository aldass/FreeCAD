/***************************************************************************
 *   Copyright (c) 2014 Luke Parry <l.parry@warwick.ac.uk>                 *
 *                                                                         *
 *   This file is part of the FreeCAD CAx development system.              *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License as published by the Free Software Foundation; either          *
 *   version 2 of the License, or (at your option) any later version.      *
 *                                                                         *
 *   This library  is distributed in the hope that it will be useful,      *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Library General Public License for more details.                  *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this library; see the file COPYING.LIB. If not,    *
 *   write to the Free Software Foundation, Inc., 59 Temple Place,         *
 *   Suite 330, Boston, MA  02111-1307, USA                                *
 *                                                                         *
 ***************************************************************************/

#include "PreCompiled.h"
#ifndef _PreComp_
# include <QMessageBox>
# include <iostream>
# include <string>
# include <sstream>
# include <cstdlib>
# include <exception>
#endif  //#ifndef _PreComp_

#include <QGraphicsView>

# include <App/DocumentObject.h>
# include <Gui/Action.h>
# include <Gui/Application.h>
# include <Gui/BitmapFactory.h>
# include <Gui/Command.h>
# include <Gui/Control.h>
# include <Gui/Document.h>
# include <Gui/Selection.h>
# include <Gui/MainWindow.h>
# include <Gui/FileDialog.h>
# include <Gui/ViewProvider.h>
#include <Gui/WaitCursor.h>

# include <Mod/Part/App/PartFeature.h>

#include <Mod/TechDraw/App/DrawView.h>
#include <Mod/TechDraw/App/DrawViewPart.h>
#include <Mod/TechDraw/App/DrawHatch.h>
#include <Mod/TechDraw/App/DrawGeomHatch.h>
#include <Mod/TechDraw/App/DrawPage.h>
#include <Mod/TechDraw/App/DrawUtil.h>
#include <Mod/TechDraw/Gui/QGVPage.h>

#include "DrawGuiUtil.h"
#include "MDIViewPage.h"
#include "TaskGeomHatch.h"
//#include "TaskLeaderLine.h"
//#include "TaskRichAnno.h"
#include "ViewProviderGeomHatch.h"
#include "ViewProviderPage.h"

using namespace TechDrawGui;
using namespace std;


//internal functions
bool _checkSelectionHatch(Gui::Command* cmd);

////===========================================================================
//// TechDraw_Leader
////===========================================================================

//DEF_STD_CMD_A(CmdTechDrawLeaderLine);

//CmdTechDrawLeaderLine::CmdTechDrawLeaderLine()
//  : Command("TechDraw_LeaderLine")
//{
//    sAppModule      = "TechDraw";
//    sGroup          = QT_TR_NOOP("TechDraw");
//    sMenuText       = QT_TR_NOOP("Add a line to a view");
//    sToolTipText    = QT_TR_NOOP("Add a line to a view");
//    sWhatsThis      = "TechDraw_LeaderLine";
//    sStatusTip      = sToolTipText;
//    sPixmap         = "actions/techdraw-mline";
//}

//void CmdTechDrawLeaderLine::activated(int iMsg)
//{
//    Q_UNUSED(iMsg);

//    Gui::TaskView::TaskDialog *dlg = Gui::Control().activeDialog();
//    if (dlg != nullptr) {
//        QMessageBox::warning(Gui::getMainWindow(), QObject::tr("Task In Progress"),
//            QObject::tr("Close active task dialog and try again."));
//        return;
//    }

//    TechDraw::DrawPage* page = DrawGuiUtil::findPage(this);
//    if (!page) {
//        return;
//    }

//    std::vector<Gui::SelectionObject> selection = getSelection().getSelectionEx();
//    TechDraw::DrawView* baseFeat = nullptr;
//    if (!selection.empty()) {
//        baseFeat =  dynamic_cast<TechDraw::DrawView *>(selection[0].getObject());
//        if( baseFeat == nullptr ) {
//            QMessageBox::warning(Gui::getMainWindow(), QObject::tr("Selection Error"),
//                                 QObject::tr("Can not attach leader.  No base View selected."));
//            return;
//        }
//    } else {
//            QMessageBox::warning(Gui::getMainWindow(), QObject::tr("Selection Error"),
//                                 QObject::tr("You must select a base View for the line."));
//            return;
//    }

//    Gui::Control().showDialog(new TechDrawGui::TaskDlgLeaderLine(baseFeat,
//                                                    page));
//}

//bool CmdTechDrawLeaderLine::isActive(void)
//{
//    bool havePage = DrawGuiUtil::needPage(this);
//    bool haveView = DrawGuiUtil::needView(this, false);
//    return (havePage && haveView);
//}

////===========================================================================
//// TechDraw_RichAnno
////===========================================================================

//DEF_STD_CMD_A(CmdTechDrawRichAnno);

//CmdTechDrawRichAnno::CmdTechDrawRichAnno()
//  : Command("TechDraw_RichAnno")
//{
//    sAppModule      = "TechDraw";
//    sGroup          = QT_TR_NOOP("TechDraw");
//    sMenuText       = QT_TR_NOOP("Add a rich text annotation");
//    sToolTipText    = QT_TR_NOOP("Add a rich text annotation");
//    sWhatsThis      = "TechDraw_RichAnno";
//    sStatusTip      = sToolTipText;
//    sPixmap         = "actions/techdraw-textleader";
//}

//void CmdTechDrawRichAnno::activated(int iMsg)
//{
//    Q_UNUSED(iMsg);
//    Gui::TaskView::TaskDialog *dlg = Gui::Control().activeDialog();
//    if (dlg != nullptr) {
//        QMessageBox::warning(Gui::getMainWindow(), QObject::tr("Task In Progress"),
//            QObject::tr("Close active task dialog and try again."));
//        return;
//    }

//    TechDraw::DrawPage* page = DrawGuiUtil::findPage(this);
//    if (!page) {
//        return;
//    }

//    std::vector<Gui::SelectionObject> selection = getSelection().getSelectionEx();
//    TechDraw::DrawView* baseFeat = nullptr;
//    if (!selection.empty()) {
//        baseFeat =  dynamic_cast<TechDraw::DrawView *>(selection[0].getObject());
////        if( baseFeat == nullptr ) {
////            QMessageBox::warning(Gui::getMainWindow(), QObject::tr("Selection Error"),
////                                 QObject::tr("Can not attach leader.  No base View selected."));
////            return;
////        }
////    } else {
////            QMessageBox::warning(Gui::getMainWindow(), QObject::tr("Selection Error"),
////                                 QObject::tr("You must select a base View for the line."));
////            return;
//    }

//    Gui::Control().showDialog(new TaskDlgRichAnno(baseFeat,
//                                                  page));
//}

//bool CmdTechDrawRichAnno::isActive(void)
//{
//    bool havePage = DrawGuiUtil::needPage(this);
//    bool haveView = DrawGuiUtil::needView(this, false);
//    return (havePage && haveView);
//}

//===========================================================================
// TechDraw_NewHatch
//===========================================================================

DEF_STD_CMD_A(CmdTechDrawNewHatch);

CmdTechDrawNewHatch::CmdTechDrawNewHatch()
  : Command("TechDraw_NewHatch")
{
    sAppModule      = "TechDraw";
    sGroup          = QT_TR_NOOP("TechDraw");
    sMenuText       = QT_TR_NOOP("Hatch a Face using image file");
    sToolTipText    = QT_TR_NOOP("Hatch a Face using image file");
    sWhatsThis      = "TechDraw_Hatch";
    sStatusTip      = sToolTipText;
    sPixmap         = "actions/techdraw-hatch";
}

void CmdTechDrawNewHatch::activated(int iMsg)
{
    Q_UNUSED(iMsg);
    if (!_checkSelectionHatch(this)) {
        return;
    }

    std::vector<Gui::SelectionObject> selection = getSelection().getSelectionEx();
    auto partFeat( dynamic_cast<TechDraw::DrawViewPart *>(selection[0].getObject()) );
    if( partFeat == nullptr ) {
        return;
    }
    const std::vector<std::string> &subNames = selection[0].getSubNames();
    TechDraw::DrawPage* page = partFeat->findParentPage();
    std::string PageName = page->getNameInDocument();
    std::vector<int> selFaces;
    for (auto& s: subNames) {
        int f = TechDraw::DrawUtil::getIndexFromName(s);
        selFaces.push_back(f);
    }

    bool removeOld = false;
    std::vector<TechDraw::DrawHatch*> hatchObjs = partFeat->getHatches();
    for (auto& s: subNames) {                             //all the faces selected in DVP
        int face = TechDraw::DrawUtil::getIndexFromName(s);
        if (TechDraw::DrawHatch::faceIsHatched(face, hatchObjs)) {
            QMessageBox::StandardButton rc =
                    QMessageBox::question(Gui::getMainWindow(), QObject::tr("Replace Hatch?"),
                            QObject::tr("Some Faces in selection are already hatched.  Replace?"));
            if (rc == QMessageBox::StandardButton::NoButton) {
                return;
            } else {
                removeOld = true;
                break;
            }
        }
    }

    openCommand("Create Hatch");
    if (removeOld) {
        std::vector<std::pair< int, TechDraw::DrawHatch*> > toRemove;
        for (auto& h: hatchObjs) {             //all the hatch objects for selected DVP
            std::vector<std::string> hatchSubs = h->Source.getSubValues();
            for (auto& hs: hatchSubs) {        //all the Faces in this hatch object
                int hatchFace = TechDraw::DrawUtil::getIndexFromName(hs);
                std::vector<int>::iterator it = std::find(selFaces.begin(), selFaces.end(), hatchFace);
                if (it != selFaces.end()) {
                    std::pair< int, TechDraw::DrawHatch*> removeItem;
                    removeItem.first = hatchFace;
                    removeItem.second = h;
                    toRemove.push_back(removeItem);
                }
            }
        }
        for (auto& r: toRemove) {
            r.second->removeSub(r.first);
            if (r.second->empty()) {
                doCommand(Doc,"App.activeDocument().removeObject('%s')",r.second->getNameInDocument());
            }
        }
    }

    std::string FeatName = getUniqueObjectName("Hatch");
    std::stringstream featLabel;
    featLabel << FeatName << "F" << 
                    TechDraw::DrawUtil::getIndexFromName(subNames.at(0)); //use 1st face# for label

    doCommand(Doc,"App.activeDocument().addObject('TechDraw::DrawHatch','%s')",FeatName.c_str());
    doCommand(Doc,"App.activeDocument().%s.Label = '%s'",FeatName.c_str(),featLabel.str().c_str());

    auto hatch( static_cast<TechDraw::DrawHatch *>(getDocument()->getObject(FeatName.c_str())) );
    hatch->Source.setValue(partFeat, subNames);

    //should this be: doCommand(Doc,"App..Feat..Source = [(App...%s,%s),(App..%s,%s),...]",objs[0]->getNameInDocument(),subs[0],...);
    //seems very unwieldy

    commitCommand();

    //Horrible hack to force Tree update  ??still required?? 
    //WF: yes. ViewProvider will not claim children without this!
    double x = partFeat->X.getValue();
    partFeat->X.setValue(x);
    getDocument()->recompute();
}


bool CmdTechDrawNewHatch::isActive(void)
{
    bool havePage = DrawGuiUtil::needPage(this);
    bool haveView = DrawGuiUtil::needView(this);
    return (havePage && haveView);
}

//===========================================================================
// TechDraw_NewGeomHatch
//===========================================================================

DEF_STD_CMD_A(CmdTechDrawNewGeomHatch);

CmdTechDrawNewGeomHatch::CmdTechDrawNewGeomHatch()
  : Command("TechDraw_NewGeomHatch")
{
    sAppModule      = "TechDraw";
    sGroup          = QT_TR_NOOP("TechDraw");
    sMenuText       = QT_TR_NOOP("Apply geometric hatch to a Face");
    sToolTipText    = QT_TR_NOOP("Apply geometric hatch to a Face");
    sWhatsThis      = "TechDraw_GeomHatch";
    sStatusTip      = sToolTipText;
    sPixmap         = "actions/techdraw-geomhatch";
}

void CmdTechDrawNewGeomHatch::activated(int iMsg)
{
    Q_UNUSED(iMsg);
    if (!_checkSelectionHatch(this)) {                 //same requirements as hatch - page, DrawViewXXX, face
        return;
    }

    std::vector<Gui::SelectionObject> selection = getSelection().getSelectionEx();
    auto objFeat( dynamic_cast<TechDraw::DrawViewPart *>(selection[0].getObject()) );
    if( objFeat == nullptr ) {
        return;
    }
    const std::vector<std::string> &subNames = selection[0].getSubNames();
    TechDraw::DrawPage* page = objFeat->findParentPage();
    std::string PageName = page->getNameInDocument();

    std::string FeatName = getUniqueObjectName("GeomHatch");
    std::stringstream featLabel;
    featLabel << FeatName << "FX" << TechDraw::DrawUtil::getIndexFromName(subNames.at(0));

    openCommand("Create GeomHatch");
    doCommand(Doc,"App.activeDocument().addObject('TechDraw::DrawGeomHatch','%s')",FeatName.c_str());
    doCommand(Doc,"App.activeDocument().%s.Label = '%s'",FeatName.c_str(),featLabel.str().c_str());

    auto geomhatch( static_cast<TechDraw::DrawGeomHatch *>(getDocument()->getObject(FeatName.c_str())) );
    geomhatch->Source.setValue(objFeat, subNames);
    Gui::ViewProvider* vp = Gui::Application::Instance->getDocument(getDocument())->getViewProvider(geomhatch);
    TechDrawGui::ViewProviderGeomHatch* hvp = dynamic_cast<TechDrawGui::ViewProviderGeomHatch*>(vp);
    if (!hvp) {
        Base::Console().Log("ERROR - CommandDecorate - GeomHatch has no ViewProvider\n");
        return;
    }

    // dialog to fill in hatch values
    Gui::Control().showDialog(new TaskDlgGeomHatch(geomhatch,hvp,true));


    commitCommand();

    //Horrible hack to force Tree update  ??still required??
    double x = objFeat->X.getValue();
    objFeat->X.setValue(x);
    getDocument()->recompute();
}

bool CmdTechDrawNewGeomHatch::isActive(void)
{
    bool havePage = DrawGuiUtil::needPage(this);
    bool haveView = DrawGuiUtil::needView(this);
    return (havePage && haveView);
}

//===========================================================================
// TechDraw_Image
//===========================================================================

DEF_STD_CMD_A(CmdTechDrawImage);

CmdTechDrawImage::CmdTechDrawImage()
  : Command("TechDraw_Image")
{
    // setting the Gui eye-candy
    sGroup        = QT_TR_NOOP("TechDraw");
    sMenuText     = QT_TR_NOOP("Insert bitmap image");
    sToolTipText  = QT_TR_NOOP("Inserts a bitmap from a file into a Page");
    sWhatsThis    = "TechDraw_Image";
    sStatusTip    = QT_TR_NOOP("Inserts a bitmap from a file into a Page");
    sPixmap       = "actions/techdraw-image";
}

void CmdTechDrawImage::activated(int iMsg)
{
    Q_UNUSED(iMsg);
    TechDraw::DrawPage* page = DrawGuiUtil::findPage(this);
    if (!page) {
        return;
    }
    std::string PageName = page->getNameInDocument();

    // Reading an image
    std::string defaultDir = App::Application::getResourceDir();
    QString qDir = QString::fromUtf8(defaultDir.data(),defaultDir.size());
    QString fileName = Gui::FileDialog::getOpenFileName(Gui::getMainWindow(),
                                                   QString::fromUtf8(QT_TR_NOOP("Select an Image File")),
                                                   qDir,
                                                   QString::fromUtf8(QT_TR_NOOP("Image (*.png *.jpg *.jpeg)")));

    if (!fileName.isEmpty())
    {
        std::string FeatName = getUniqueObjectName("Image");
        openCommand("Create Image");
        doCommand(Doc,"App.activeDocument().addObject('TechDraw::DrawViewImage','%s')",FeatName.c_str());
        doCommand(Doc,"App.activeDocument().%s.ImageFile = '%s'",FeatName.c_str(),fileName.toUtf8().constData());
        doCommand(Doc,"App.activeDocument().%s.addView(App.activeDocument().%s)",PageName.c_str(),FeatName.c_str());
        updateActive();
        commitCommand();
    }
}

bool CmdTechDrawImage::isActive(void)
{
    return DrawGuiUtil::needPage(this);
}

//===========================================================================
// TechDraw_ToggleFrame
//===========================================================================

DEF_STD_CMD_A(CmdTechDrawToggleFrame);

CmdTechDrawToggleFrame::CmdTechDrawToggleFrame()
  : Command("TechDraw_ToggleFrame")
{
    sAppModule      = "TechDraw";
    sGroup          = QT_TR_NOOP("TechDraw");
    sMenuText       = QT_TR_NOOP("Turn View Frames On/Off");
    sToolTipText    = QT_TR_NOOP("Turn View Frames On/Off");
    sWhatsThis      = "TechDraw_Toggle";
    sStatusTip      = sToolTipText;
    sPixmap         = "actions/techdraw-toggleframe";
}

void CmdTechDrawToggleFrame::activated(int iMsg)
{
    Q_UNUSED(iMsg);
    TechDraw::DrawPage* page = DrawGuiUtil::findPage(this);
    if (!page) {
        return;
    }
    std::string PageName = page->getNameInDocument();

    Gui::Document* activeGui = Gui::Application::Instance->getDocument(page->getDocument());
    Gui::ViewProvider* vp = activeGui->getViewProvider(page);
    ViewProviderPage* vpp = dynamic_cast<ViewProviderPage*>(vp);

    if (vpp != nullptr) {
        vpp->toggleFrameState();
    } else {
        QMessageBox::warning(Gui::getMainWindow(), QObject::tr("No TechDraw Page"),
            QObject::tr("Need a TechDraw Page for this command"));
        return;
    }
}

bool CmdTechDrawToggleFrame::isActive(void)
{
    bool havePage = DrawGuiUtil::needPage(this);
    bool haveView = DrawGuiUtil::needView(this,false);
    return (havePage && haveView);
}

//===========================================================================
// TechDraw_Redraw
//===========================================================================

DEF_STD_CMD_A(CmdTechDrawRedraw);

CmdTechDrawRedraw::CmdTechDrawRedraw()
  : Command("TechDraw_Redraw")
{
    sAppModule      = "TechDraw";
    sGroup          = QT_TR_NOOP("TechDraw");
    sMenuText       = QT_TR_NOOP("Redraw a page");
    sToolTipText    = QT_TR_NOOP("Redraw a page");
    sWhatsThis      = "TechDraw_Redraw";
    sStatusTip      = sToolTipText;
    sPixmap         = "actions/techdraw-forceredraw";
}

void CmdTechDrawRedraw::activated(int iMsg)
{
    Q_UNUSED(iMsg);
    TechDraw::DrawPage* page = DrawGuiUtil::findPage(this);
    if (!page) {
        return;
    }
    Gui::WaitCursor wc;

    page->forceRedraw(true);
    page->updateAllViews();
    page->forceRedraw(false);
}

bool CmdTechDrawRedraw::isActive(void)
{
    bool havePage = DrawGuiUtil::needPage(this);
    return (havePage);
}

void CreateTechDrawCommandsDecorate(void)
{
    Gui::CommandManager &rcCmdMgr = Gui::Application::Instance->commandManager();

    rcCmdMgr.addCommand(new CmdTechDrawNewHatch());
    rcCmdMgr.addCommand(new CmdTechDrawNewGeomHatch());
    rcCmdMgr.addCommand(new CmdTechDrawImage());
    rcCmdMgr.addCommand(new CmdTechDrawToggleFrame());
    rcCmdMgr.addCommand(new CmdTechDrawRedraw());
//    rcCmdMgr.addCommand(new CmdTechDrawLeaderLine());
//    rcCmdMgr.addCommand(new CmdTechDrawRichAnno());
}

//===========================================================================
// Selection Validation Helpers
//===========================================================================

bool _checkSelectionHatch(Gui::Command* cmd) {
    std::vector<Gui::SelectionObject> selection = cmd->getSelection().getSelectionEx();
    if (selection.size() == 0) {
        QMessageBox::warning(Gui::getMainWindow(), QObject::tr("Incorrect selection"),
                             QObject::tr("Select a Face first"));
        return false;
    }

    TechDraw::DrawViewPart * objFeat = dynamic_cast<TechDraw::DrawViewPart *>(selection[0].getObject());
    if(!objFeat) {
        QMessageBox::warning(Gui::getMainWindow(), QObject::tr("Incorrect selection"),
                             QObject::tr("No TechDraw object in selection"));
        return false;
    }

    std::vector<App::DocumentObject*> pages = cmd->getDocument()->getObjectsOfType(TechDraw::DrawPage::getClassTypeId());
    if (pages.empty()){
        QMessageBox::warning(Gui::getMainWindow(), QObject::tr("Incorrect selection"),
            QObject::tr("Create a page to insert."));
        return false;
    }

    const std::vector<std::string> &SubNames = selection[0].getSubNames();
    if (SubNames.empty()) {
        QMessageBox::warning(Gui::getMainWindow(), QObject::tr("Incorrect Selection"),
        QObject::tr("No Faces to hatch in this selection"));
        return false;
    }
    std::string gType = TechDraw::DrawUtil::getGeomTypeFromName(SubNames.at(0));
    if (!(gType == "Face")) {
        QMessageBox::warning(Gui::getMainWindow(), QObject::tr("Incorrect Selection"),
        QObject::tr("No Faces to hatch in this selection"));
        return false;
    }

    return true;
}
