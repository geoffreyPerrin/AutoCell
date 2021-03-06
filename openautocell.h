#ifndef OPENAUTOCELL_H
#define OPENAUTOCELL_H

#include <QDialog>
#include <QFileDialog>
#include "autocell.h"
#include "xmldom.h"
#include "oned.h"


namespace Ui {

class OpenAutoCell;
}

/*!
 * \brief Cette classe est une fenêtre de dialogue servant à parcourir des fichiers pour charger des
 * automates cellulaires / configurations stockés dans des fichiers XML
 */
class OpenAutoCell : public QDialog
{
    Q_OBJECT

public:
    explicit OpenAutoCell(QWidget *parent = 0);
    ~OpenAutoCell();
    const Xml_Dom &getFile() const;
    void setFile(const Xml_Dom *value);
    QString getFileName() const;
public slots:
    void loadFileName();
private:
    Ui::OpenAutoCell *ui;
    const Xml_Dom *file;
};

#endif // OPENAUTOCELL_H
