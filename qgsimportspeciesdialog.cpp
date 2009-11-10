#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QIODevice>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>

#include "settings.h"
#include "resources.h"

#include "qgsimportspeciesdialog.h"

QgsImportSpeciesDialog::QgsImportSpeciesDialog( QWidget* parent )
{
  setupUi( this );
  
  pbnImport->setEnabled( false );
}

void QgsImportSpeciesDialog::on_pbnBrowse_clicked()
{
  QString fileName = QFileDialog::getOpenFileName( this, tr( "Choose the CSV file for import" ), ".", tr( "CSV files (*.csv)" ) );
  if ( !fileName.isEmpty() )
  {
    leFileName->setText( fileName );
      pbnImport->setEnabled( true );
  }
}

void QgsImportSpeciesDialog::on_pbnImport_clicked()
{
  QString str;
  QChar separator;
  QStringList lstFields;

  switch ( pa_db::settings::fieldSeparator() )
  {
    case 0:
      separator = ',';
      break;
    case 1:
      separator = ';';
      break;
    case 2:
      separator = ( pa_db::settings::userSeparator()[0] );
      break;
  }
  
  QFile file ( leFileName->text() );
  if ( !file.open( QIODevice::ReadOnly | QIODevice::Text ) )
  {
    return;
  }
  QTextStream in( &file );
  
  QSqlDatabase db = QSqlDatabase::database( pa_db::text::database_connection_name() );
  if ( db.isOpen() )
  {
    QSqlQuery query( db );
    db.transaction();
    while ( !in.atEnd() )
    {
      str = in.readLine();
      // lstFields = str.split( "," );
      lstFields = str.split( separator );
      
      query.prepare( "INSERT INTO species(nameRu, nameEn, comment, protectstatus) VALUES(:nr,:ne,:cm,:ps)" );
      query.bindValue( ":nr", lstFields[ 0 ] );
      query.bindValue( ":ne", lstFields[ 1 ] );
      query.bindValue( ":cm", lstFields[ 2 ] );
      query.bindValue( ":ps", lstFields[ 3 ] );
      query.exec();
    }
    db.commit();
    QMessageBox::information( this, tr( "Completed" ), tr( "Import completed successfully") );
  }
  else
  {
    qDebug() << "DB is not open";
  }
  pbnImport->setEnabled( false );
}
