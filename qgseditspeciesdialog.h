#ifndef QGSEDITSPECIESDIALOG_H
#define QGSEDITSPECIESDIALOG_H

#include "ui_qgseditspeciesbase.h"
#include <QDialog>
#include <QSqlTableModel>

/*
 * Dialog that allow user edit existing species records in DB
 */
class QgsEditSpeciesDialog: public QDialog, private Ui::QgsEditSpeciesBase
{
   Q_OBJECT
  public:
    QgsEditSpeciesDialog( QWidget* parent );
  public slots:
    void on_pbnAddRecord_clicked();
    void on_pbnDelRecord_clicked();
  private:
    QSqlTableModel* mModelSpecies;
};

#endif // QGSEDITSPECIESDIALOG_H
