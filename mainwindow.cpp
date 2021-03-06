#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtGlobal>
#include <typeinfo>
#include <QRegExp>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),renderArea(new RenderArea()),newCellDialog(new NewAutoCell(this)),openCellDialog(new OpenAutoCell(this)){

	ui->setupUi(this);


	ui->scrollArea->setWidget(renderArea);
	ui->scrollArea->setWidgetResizable(true);
	setCentralWidget(ui->scrollArea);
	QGridLayout *layout = new QGridLayout;
	vitesseSlider = new QSlider(Qt::Horizontal, this);
	vitesseSlider->setMinimumWidth(200);
	vitesseSlider->setMaximumWidth(250);
	vitesseSlider->setRange(1,500);
	vitesseSlider->setValue(400);
	ui->mainToolBar->addWidget(vitesseSlider);
	QObject::connect(ui->actionNew_AutoCell, &QAction::triggered, newCellDialog ,&NewAutoCell::show );
	QObject::connect(ui->actionOpen_AutoCell, &QAction::triggered, openCellDialog, &OpenAutoCell::show);
	QObject::connect(ui->actionplay, &QAction::toggled,renderArea , &RenderArea::playIsChecked);
	QObject::connect(ui->actionprevious, &QAction::triggered, renderArea, &RenderArea::previous);
	QObject::connect(ui->actiongenerate_randomly, &QAction::triggered, renderArea, &RenderArea::generate_randomly);
	QObject::connect(ui->actionnext, &QAction::triggered,renderArea , &RenderArea::next);
	QObject::connect(renderArea, &RenderArea::pause ,this, &MainWindow::pause);
	QObject::connect(newCellDialog, &NewAutoCell::accepted,this ,&MainWindow::createNewAutoCell );
	QObject::connect(openCellDialog, &OpenAutoCell::accepted, this, &MainWindow::openAutoCell);
	QObject::connect(ui->actionSave_AutoCell, &QAction::triggered, this, &MainWindow::saveAutoCell);
	QObject::connect(vitesseSlider, &QSlider::valueChanged, renderArea, &RenderArea::changeVitesse);
	QObject::connect(ui->actionreinit, &QAction::triggered, renderArea, &RenderArea::reinit);
	QObject::connect(ui->actionSave_Automate, &QAction::triggered, this, &MainWindow::saveAutoCellandState);

	renderArea->changeVitesse(vitesseSlider->value());

	openContextAutoCell();


}

void MainWindow::pause()
{
	ui->actionplay->setChecked(false);
}

void MainWindow::createNewAutoCell()
{

	if(newCellDialog->getType()=="Simple one dimension")
	{
		renderArea->setAutoCell(new OneD(newCellDialog->getWidth(),2,newCellDialog->getRule(),3,newCellDialog->getName()));

	}
	else if(newCellDialog->getType()=="Game of life")
	{
		renderArea->setAutoCell(new jeuVie(newCellDialog->getHeight(),newCellDialog->getWidth(),newCellDialog->getNbMinVoisins(),newCellDialog->getNbMaxVoisins(),newCellDialog->getName()));
		ui->actionprevious->setVisible(true);
	}

	else if(newCellDialog->getType()=="Quad life")
	{
		renderArea->setAutoCell(new QuadLife(newCellDialog->getHeight(),newCellDialog->getWidth(),newCellDialog->getNbMinVoisins(),newCellDialog->getNbMaxVoisins(),newCellDialog->getName()));
		ui->actionprevious->setVisible(true);
	}

	ui->actionplay->setEnabled(true);
	ui->actionnext->setEnabled(true);

}

void MainWindow::openAutoCell(){
	Xml_Dom dom;
	dom.readFile(openCellDialog->getFileName());
	openCellDialog->setFile(&dom);
	QRegExp reg(".+_plus_state.xml");
	if(dom.getNoeud("type") == "oneD"){
		renderArea->setAutoCell(new OneD(dom.getNoeud("width").toInt(),2,dom.getNoeud("rule").toInt(),3,dom.getNoeud("name")));
		if(reg.exactMatch(openCellDialog->getFileName())){
			renderArea->getAutoCell()->setCurrentState(dom.getNoeud("currentState").toInt());
			for(int i = 0; i <= renderArea->getAutoCell()->getCurrentState(); i++){
				for(int j = 0; j < renderArea->getAutoCell()->getWidth(); j++){
					renderArea->getAutoCell()->setValueEtat(i,0,j,dom.getNoeud("value_" + QString::number(i) + "_" + QString::number(j)).toInt());
				}
			}
		}
		ui->actionprevious->setVisible(false);

	}
	else if (dom.getNoeud("type") == "jeuVie"){
		renderArea->setAutoCell(new jeuVie(dom.getNoeud("height").toInt(), dom.getNoeud("width").toInt(), dom.getNoeud("nbMinVoisins").toInt(), dom.getNoeud("nbMaxVoisins").toInt(),dom.getNoeud("name") ));
		if(reg.exactMatch(openCellDialog->getFileName())){
			for(int i = 0; i < renderArea->getAutoCell()->getHeight(); i++){
				for(int j = 0; j < renderArea->getAutoCell()->getWidth(); j++){
					renderArea->getAutoCell()->setValueEtat(0,i,j,dom.getNoeud("value_" + QString::number(i) + "_" + QString::number(j)).toInt());
				}
			}
		}
		ui->actionprevious->setVisible(true);

	}else if (dom.getNoeud("type") == "quadLife"){
		renderArea->setAutoCell(new QuadLife(dom.getNoeud("height").toInt(), dom.getNoeud("width").toInt(), dom.getNoeud("nbMinVoisins").toInt(), dom.getNoeud("nbMaxVoisins").toInt(), dom.getNoeud("name")));
		if(reg.exactMatch(openCellDialog->getFileName())){
			for(int i = 0; i < renderArea->getAutoCell()->getHeight(); i++){
				for(int j = 0; j < renderArea->getAutoCell()->getWidth(); j++){
					renderArea->getAutoCell()->setValueEtat(0,i,j,dom.getNoeud("value_" + QString::number(i) + "_" + QString::number(j)).toInt());
				}
			}
		}
		ui->actionprevious->setVisible(true);

	}
}

void MainWindow::saveAutoCell(){
	Xml_Dom dom;
	if(typeid(*(renderArea->getAutoCell())).name() == typeid(OneD).name()){
		dom.setNoeud("type", "oneD");
		dom.setNoeud("rule", QString::number(((OneD*) renderArea->getAutoCell())->getRule()));

	}
	else if (typeid(*(renderArea->getAutoCell())).name() == typeid(jeuVie).name()){
		dom.setNoeud("type", "jeuVie");
		dom.setNoeud("nbMaxVoisins", QString::number(((jeuVie*) renderArea->getAutoCell())->getNbMaxVoisins()));
		dom.setNoeud("nbMinVoisins", QString::number(((jeuVie*) renderArea->getAutoCell())->getNbMinVoisins()));
	}
	else{
		dom.setNoeud("type", "quadLife");
		dom.setNoeud("nbMaxVoisins", QString::number(((QuadLife*) renderArea->getAutoCell())->getNbMaxVoisins()));
		dom.setNoeud("nbMinVoisins", QString::number(((QuadLife*) renderArea->getAutoCell())->getNbMinVoisins()));
	}
	dom.setNoeud("width", QString::number(renderArea->getAutoCell()->getWidth()));
	dom.setNoeud("height", QString::number(renderArea->getAutoCell()->getHeight()) );
	dom.setNoeud("name",renderArea->getAutoCell()->getName());
	dom.writeFile(renderArea->getAutoCell()->getName());
}


void MainWindow::saveAutoCellandState()
{
	Xml_Dom dom;
	if(typeid(*(renderArea->getAutoCell())).name() == typeid(OneD).name()){
		dom.setNoeud("type", "oneD");
		dom.setNoeud("rule", QString::number(((OneD*) renderArea->getAutoCell())->getRule()));
		dom.setNoeud("currentState", QString::number(renderArea->getAutoCell()->getCurrentState()));
		for(int i = 0; i < renderArea->getAutoCell()->getEtats().size(); i++){
			for(int j = 0; j < renderArea->getAutoCell()->getEtats()[i].getMatrice()[0].size(); j++){
				dom.setNoeud("value_" + QString::number(i) +"_"+ QString::number(j),QString::number(renderArea->getAutoCell()->getEtats()[i].getMatrice()[0][j]));
			}
		}
	}
	else{
		if (typeid(*(renderArea->getAutoCell())).name() == typeid(jeuVie).name()){
			dom.setNoeud("type", "jeuVie");
			dom.setNoeud("nbMaxVoisins", QString::number(((jeuVie*) renderArea->getAutoCell())->getNbMaxVoisins()));
			dom.setNoeud("nbMinVoisins", QString::number(((jeuVie*) renderArea->getAutoCell())->getNbMinVoisins()));

		}
		else{
			dom.setNoeud("type", "quadLife");
			dom.setNoeud("nbMaxVoisins", QString::number(((QuadLife*) renderArea->getAutoCell())->getNbMaxVoisins()));
			dom.setNoeud("nbMinVoisins", QString::number(((QuadLife*) renderArea->getAutoCell())->getNbMinVoisins()));

		}


		Etat e = renderArea->getAutoCell()->getEtats()[renderArea->getAutoCell()->getCurrentState()%10];
		for(int i = 0; i < e.getMatrice().size(); i++){
			for(int j = 0; j < e.getMatrice()[i].size(); j++){
				dom.setNoeud("value_" + QString::number(i) +"_"+ QString::number(j),QString::number(e.getMatrice()[i][j]));
			}
		}

	}
	dom.setNoeud("width", QString::number(renderArea->getAutoCell()->getWidth()));
	dom.setNoeud("height", QString::number(renderArea->getAutoCell()->getHeight()) );
	dom.setNoeud("name",renderArea->getAutoCell()->getName());
	dom.writeFile(renderArea->getAutoCell()->getName()+"_plus_state");
}

void MainWindow::saveAutoCellQuit()
{
	Xml_Dom dom;
	if(typeid(*(renderArea->getAutoCell())).name() == typeid(OneD).name()){
		dom.setNoeud("type", "oneD");
		dom.setNoeud("rule", QString::number(((OneD*) renderArea->getAutoCell())->getRule()));
		dom.setNoeud("currentState", QString::number(renderArea->getAutoCell()->getCurrentState()));
		for(int i = 0; i < renderArea->getAutoCell()->getEtats().size(); i++){
			for(int j = 0; j < renderArea->getAutoCell()->getEtats()[i].getMatrice()[0].size(); j++){
				dom.setNoeud("value_" + QString::number(i) +"_"+ QString::number(j),QString::number(renderArea->getAutoCell()->getEtats()[i].getMatrice()[0][j]));
			}
		}
	}
	else{
		if (typeid(*(renderArea->getAutoCell())).name() == typeid(jeuVie).name()){
			dom.setNoeud("type", "jeuVie");
			dom.setNoeud("nbMaxVoisins", QString::number(((jeuVie*) renderArea->getAutoCell())->getNbMaxVoisins()));
			dom.setNoeud("nbMinVoisins", QString::number(((jeuVie*) renderArea->getAutoCell())->getNbMinVoisins()));

		}
		else{
			dom.setNoeud("type", "quadLife");
			dom.setNoeud("nbMaxVoisins", QString::number(((QuadLife*) renderArea->getAutoCell())->getNbMaxVoisins()));
			dom.setNoeud("nbMinVoisins", QString::number(((QuadLife*) renderArea->getAutoCell())->getNbMinVoisins()));

		}


		Etat e = renderArea->getAutoCell()->getEtats()[renderArea->getAutoCell()->getCurrentState()%10];
		for(int i = 0; i < e.getMatrice().size(); i++){
			for(int j = 0; j < e.getMatrice()[i].size(); j++){
				dom.setNoeud("value_" + QString::number(i) +"_"+ QString::number(j),QString::number(e.getMatrice()[i][j]));
			}
		}

	}
	dom.setNoeud("width", QString::number(renderArea->getAutoCell()->getWidth()));
	dom.setNoeud("height", QString::number(renderArea->getAutoCell()->getHeight()) );
	dom.setNoeud("name",renderArea->getAutoCell()->getName());
	dom.setNoeud("speed",QString::number(vitesseSlider->value()));
	dom.writeFile("sauvegarde");
}

void MainWindow::openContextAutoCell(){
	Xml_Dom dom;
	QFile xml_doc("sauvegarde.xml");

	if(!xml_doc.open(QIODevice::ReadOnly))
	{
		return;
	}
	dom.readFile("sauvegarde.xml");

	if(dom.getNoeud("type") == "oneD"){
		ui->actionprevious->setVisible(true);
		renderArea->setAutoCell(new OneD(dom.getNoeud("width").toInt(),2,dom.getNoeud("rule").toInt(),3,dom.getNoeud("name")));
		renderArea->getAutoCell()->setCurrentState(dom.getNoeud("currentState").toInt());
		for(int i = 0; i <= renderArea->getAutoCell()->getCurrentState(); i++){
			for(int j = 0; j < renderArea->getAutoCell()->getWidth(); j++){
				renderArea->getAutoCell()->setValueEtat(i,0,j,dom.getNoeud("value_" + QString::number(i) + "_" + QString::number(j)).toInt());
			}
		}
	}
	else if (dom.getNoeud("type") == "jeuVie"){
		renderArea->setAutoCell(new jeuVie(dom.getNoeud("height").toInt(), dom.getNoeud("width").toInt(), dom.getNoeud("nbMinVoisins").toInt(), dom.getNoeud("nbMaxVoisins").toInt(),dom.getNoeud("name")));
		ui->actionprevious->setVisible(true);
		for(int i = 0; i < renderArea->getAutoCell()->getHeight(); i++){
			for(int j = 0; j < renderArea->getAutoCell()->getWidth(); j++){
				renderArea->getAutoCell()->setValueEtat(0,i,j,dom.getNoeud("value_" + QString::number(i) + "_" + QString::number(j)).toInt());
			}
		}
	}else if (dom.getNoeud("type") == "quadLife"){
		renderArea->setAutoCell(new QuadLife(dom.getNoeud("height").toInt(), dom.getNoeud("width").toInt(), dom.getNoeud("nbMinVoisins").toInt(), dom.getNoeud("nbMaxVoisins").toInt(),dom.getNoeud("name")));
		ui->actionprevious->setVisible(true);
		for(int i = 0; i < renderArea->getAutoCell()->getHeight(); i++){
			for(int j = 0; j < renderArea->getAutoCell()->getWidth(); j++){
				renderArea->getAutoCell()->setValueEtat(0,i,j,dom.getNoeud("value_" + QString::number(i) + "_" + QString::number(j)).toInt());
			}
		}
	}




	vitesseSlider->setValue(dom.getNoeud("speed").toInt());
}






MainWindow::~MainWindow()
{
	delete ui;
}

