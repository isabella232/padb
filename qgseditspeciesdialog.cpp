#include <QDebug>
#include <QMessageBox>
#include <QModelIndex>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QSqlQuery>

#include "settings.h"
#include "resources.h"

#include "qgseditspeciesdialog.h"

QgsEditSpeciesDialog::QgsEditSpeciesDialog( QWidget* parent )
{
  setupUi( this );
  
  // linking database table and table on form
  QSqlDatabase db = QSqlDatabase::database( pa_db::text::database_connection_name() );
  mModelSpecies = new QSqlTableModel( 0, db );
  mModelSpecies->setTable( "species" );
  mModelSpecies->setEditStrategy( QSqlTableModel::OnFieldChange );
  // header
  mModelSpecies->setHeaderData(0, Qt::Horizontal, tr("Id"));
  mModelSpecies->setHeaderData(1, Qt::Horizontal, tr("Name (russian)"));
  mModelSpecies->setHeaderData(2, Qt::Horizontal, tr("Name (english)"));
  mModelSpecies->setHeaderData(3, Qt::Horizontal, tr("Comments"));
  mModelSpecies->setHeaderData(4, Qt::Horizontal, tr("Protection status"));
  
  mModelSpecies->select();
  tableViewSpecies->setModel( mModelSpecies );
}

void QgsEditSpeciesDialog::on_pbnAddRecord_clicked()
{
  int row = mModelSpecies->rowCount();
  if ( !mModelSpecies->insertRow( row ) )
  {
    qDebug() << "Can't insert row";
  }
  mModelSpecies->submitAll();
}

void QgsEditSpeciesDialog::on_pbnDelRecord_clicked()
{
  QModelIndex index = tableViewSpecies->currentIndex();
  if ( index.isValid() )
  {
    int row = index.row();
    QString name = mModelSpecies->record( row ).value( "nameRu" ).toString();
    if ( QMessageBox::question( this, tr( "Delete" ), tr( "Delete species %1?" ).arg( name ), QMessageBox::Yes | QMessageBox::No ) == QMessageBox::No)
    {
      return;
    }
    mModelSpecies->removeRow( row );
    mModelSpecies->submitAll();
  }
  else
  {
    QMessageBox::warning( this, tr( "PADB" ), tr( "You need to select record before deleting" ) );
    return;
  }
}
