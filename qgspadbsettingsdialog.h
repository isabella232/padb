#ifndef QGSPADBSETTINGSDIALOG_H
#define QGSPADBSETTINGSDIALOG_H

#include "ui_qgssettingsbase.h"
#include <QDialog>

/*
 * Configuration dialog of PADB plugin
 */
class QgsPadbSettingsDialog: public QDialog, private Ui::QgsPadbSettingsBase
{
   Q_OBJECT
  public:
    QgsPadbSettingsDialog( QWidget* parent );
  
  public slots:
    void saveSettings();
    void on_pbnStartImport_clicked();
    void on_pbnStartEdit_clicked();
    // void on_buttonBox_accepted();
    // void on_buttonBox_rejected();
  
  //private slots:
    void on_btnOpen_clicked();
    void on_tblMapping_doubleClicked( int row, int column );
  
  protected:
    // Populates combo box with layers
    void getLayersList();
  
  private:
    QStringList i18nList();
    void fillMappingTable();
    void saveMappingTable() const;
    
};

#endif // QGSPADBSETTINGSDIALOG_H
