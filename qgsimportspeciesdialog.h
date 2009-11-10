#ifndef QGSIMPORTSPECIESDIALOG_H
#define QGSIMPORTSPECIESDIALOG_H

#include "ui_qgsimportspeciesbase.h"
#include <QDialog>

/*
 * Dialog that allow user import species list from CSV to DB
 */
class QgsImportSpeciesDialog: public QDialog, private Ui::QgsImportSpeciesBase
{
   Q_OBJECT
  public:
    QgsImportSpeciesDialog( QWidget* parent );
  public slots:
    void on_pbnBrowse_clicked();
    void on_pbnImport_clicked();
};

#endif // QGSIMPORTSPECIESDIALOG_H
