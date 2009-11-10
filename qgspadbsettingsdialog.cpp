#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QIODevice>
#include <QMenu>
#include <QMessageBox>
#include <QModelIndex>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlTableModel>

#include <qgisinterface.h>
#include "qgsapplication.h"
#include "qgsmaplayerregistry.h"
#include "qgsvectorlayer.h"
#include "qgsvectordataprovider.h"

#include "qgsimportspeciesdialog.h"
#include "qgseditspeciesdialog.h"
#include "settings.h"
#include "resources.h"

#include "qgspadbsettingsdialog.h"

// #include <QFileInfo>

QgsPadbSettingsDialog::QgsPadbSettingsDialog( QWidget* parent ):QDialog()
{
  setupUi( this );
  connect( buttonBox, SIGNAL( accepted() ), this, SLOT( accept() ) );
  connect( buttonBox, SIGNAL( rejected() ), this, SLOT( reject() ) );
  connect( this, SIGNAL( accepted() ), this, SLOT( saveSettings() ) );
  
  tabWidget->setCurrentIndex( 0 );
  
  // locale settings
  lblSystemLocale->setText( tr( "Detected active locale on your system: %1" ).arg( QLocale::system().name() ) );
  lblQgisLocale->setText( tr( "Detected active locale of your QGIS: %1" ).arg( pa_db::settings::qgisLocale() ) );
  grpLocale->setChecked( pa_db::settings::padbOverrideFlag() );
  // populate locale combo box with available locales
  QStringList myI18nList = i18nList();
  cmbLocale->addItems( myI18nList );
  QString myUserLocale = pa_db::settings::padbLocale();
  if ( myI18nList.contains( myUserLocale ) )
  {
    cmbLocale->setCurrentIndex( myI18nList.indexOf( myUserLocale ) );
  }
  
  // database settings
  leDbFileName->setText( pa_db::settings::dbFileName() );
  connect( btnOpen, SIGNAL( clicked( bool ) ), SLOT( on_btnOpen_clicked() ) );
  
  // zoom settings
  spinBoxZoom->setValue( pa_db::settings::zoom() );
  
  // populate combo box with layers
  getLayersList();
  
  // fields mapping
  fillMappingTable();
  connect( tblMapping, SIGNAL( cellDoubleClicked( int, int ) ), SLOT( on_tblMapping_doubleClicked( int, int ) ) );
  
  // species settings
  if ( pa_db::settings::speciesVisibleFlag() == Qt::Checked )
  {
    chkHideSpecies->setCheckState( Qt::Checked );
  }
  else
  {
    chkHideSpecies->setCheckState( Qt::Unchecked );
  }
  
  switch ( pa_db::settings::fieldSeparator() )
  {
    case 0:
      rbSepComma->setChecked( true );
      break;
    case 1:
      rbSepSemicolon->setChecked( true );
      break;
    case 2:
      rbSepUser->setChecked( true );
      leCustomSeparator->setText( pa_db::settings::userSeparator() );
      break;
  }
  
  switch ( pa_db::settings::namingStyle() )
  {
    case 0:
      rbNameRu->setChecked( true );
      break;
    case 1:
      rbNameEn->setChecked( true );
      break;
    case 2:
      rbNameBoth->setChecked( true);
      break;
  }
}

void QgsPadbSettingsDialog::on_btnOpen_clicked()
{
  QString fileName;
  QFileDialog dlgOpenFile( this, tr( "Open PADB file" ), ".", tr( "SQLite database (*.db *.db3 *.sqlite)" ) );
  dlgOpenFile.setFileMode( QFileDialog::AnyFile );
  if ( dlgOpenFile.exec() )
  {
    fileName = dlgOpenFile.selectedFiles()[ 0 ];
  }
  // QString selectedFilter;
  // QString fileName = QFileDialog::getOpenFileName( this, tr( "Open PADB file" ), ".", tr( "SQLite database (*.db *.db3 *.sqlite)" ), &selectedFilter, QFileDialog::DontUseNativeDialog );
  if ( !fileName.isEmpty() )
  {
    leDbFileName->setText( fileName );
  }
}

void QgsPadbSettingsDialog::on_tblMapping_doubleClicked( int row, int column )
{
  if ( column == 2 && tblMapping->item( row, column ) )
  {
    QMenu mnuMapping;
    bool found = false;
    
    const QMap< QString, QgsMapLayer* > layers = QgsMapLayerRegistry::instance()->mapLayers();
    for ( QMap< QString, QgsMapLayer* >::const_iterator iter = layers.begin(); iter != layers.end(); ++iter )
    {
      if ( QgsVectorLayer *vLayer = qobject_cast< QgsVectorLayer* >( iter.value() ) )
      {
        if ( vLayer->name() == cmbLayer->currentText() )
        {
          QgsVectorDataProvider *vProvider = vLayer->dataProvider();
          const QgsFieldMap fieldMap = vProvider->fields();
          for ( QgsFieldMap::const_iterator it = fieldMap.constBegin(); it != fieldMap.constEnd(); ++it )
          {
            found = true;
            mnuMapping.addAction( it.value().name() );
          }
        }
      }
    }
    
    if ( found )
    {
      QAction *act = mnuMapping.exec( QCursor::pos() );
      if ( act )
      {
        tblMapping->item( row, column )->setText( act->text() );
      }
    }
  }
}

void QgsPadbSettingsDialog::saveSettings()
{
  // create new DB if not exists or save path to existing one
  const QString dbFile = leDbFileName->text();
  if ( !QFile().exists( dbFile ) )
  {
    // file not exists. Ask to create empty DB
    if ( QMessageBox::question( this, 
                                tr( "PADB plugin" ),
                                tr( "DB file does not exist, create empty DB file?" ),
                                QMessageBox::Yes | QMessageBox::No ) == QMessageBox::Yes )
    {
      if ( pa_db::settings::createEmptyDb( dbFile ) )
      {
        // new empty file created successfully
        pa_db::settings::setDbFileName( dbFile );
      }
      else
      {
        QMessageBox::critical( this, tr( "PADB plugin" ), tr( "Error while creating empty DB file." ) );
        return;
      }
    }
  }
  
  // save zoom
  pa_db::settings::setZoom( spinBoxZoom->text() );
  
  // save oopt layer
  pa_db::settings::setOoptLayer( cmbLayer->currentText() );
  
  // save plugin locale
  pa_db::settings::setPadbLocale( cmbLocale->currentText() );
  pa_db::settings::setPadbOverrideFlag( grpLocale->isChecked() );
  
  // save db field mapping
  saveMappingTable();
  
  // save species settings
  pa_db::settings::setSpeciesVisibleFlag( chkHideSpecies->checkState() );
  
  if ( rbSepComma->isChecked() )
  {
    pa_db::settings::setFieldSeparator( 0 );
  }
  else if ( rbSepSemicolon->isChecked() )
  {
    pa_db::settings::setFieldSeparator( 1 );
  }
  else
  {
    pa_db::settings::setFieldSeparator( 2 );
    pa_db::settings::setUserSeparator( leCustomSeparator->text() );
  }
  
  if ( rbNameRu->isChecked() )
  {
    pa_db::settings::setNamingStyle( 0 );
  }
  else if ( rbNameEn->isChecked() )
  {
    pa_db::settings::setNamingStyle( 1 );
  }
  else
  {
    pa_db::settings::setNamingStyle( 2 );
  }
}

void QgsPadbSettingsDialog::on_pbnStartImport_clicked()
{
  QgsImportSpeciesDialog dlg( this );
  dlg.exec();
}

void QgsPadbSettingsDialog::on_pbnStartEdit_clicked()
{
  QgsEditSpeciesDialog dlg( this );
  dlg.exec();
}

void QgsPadbSettingsDialog::getLayersList()
{
  const QMap< QString, QgsMapLayer* > layers = QgsMapLayerRegistry::instance()->mapLayers();
  for ( QMap< QString, QgsMapLayer* >::const_iterator iter = layers.begin(); iter != layers.end(); ++iter )
  {
    if ( QgsVectorLayer *vLayer = qobject_cast< QgsVectorLayer* >( iter.value() ) )
    {
      cmbLayer->addItem( vLayer->name() );
    }
  }
}

void QgsPadbSettingsDialog::fillMappingTable()
{
  QSqlDatabase db = QSqlDatabase::database( "pa_db_plugin_database_connection" );
  if ( db.isOpen() )
  {
    QSqlQuery query( db );
    if ( query.exec( QString( "SELECT objects.*, status.name AS status_name,profile.name AS profile_name,category.name AS category_name "\
                              "FROM objects,status,profile,category "\
                              "WHERE objects.status_id=status.id AND objects.profile_id=profile.id AND objects.category_id=category.id" ) ) )
    {
      if ( query.next() )
      {
        const QSqlRecord rec = query.record();
        tblMapping->setColumnCount( 3 );
        tblMapping->setHorizontalHeaderLabels( QStringList() << "" << tr( "DB" ) << tr( "Shapefile" ) );
        tblMapping->horizontalHeader()->setClickable( false );
        tblMapping->verticalHeader()->setClickable( false );
        tblMapping->setColumnWidth( 0, 30 );
        tblMapping->horizontalHeader()->setResizeMode( 1, QHeaderView::Stretch );
        tblMapping->horizontalHeader()->setResizeMode( 2, QHeaderView::Stretch );
        tblMapping->setRowCount( rec.count() - 3 );
        int item = 0;
        for ( int i = 0; i < rec.count(); ++i )
        {
          const QString fieldName = rec.fieldName( i );
          if ( fieldName != "status_id" && fieldName != "category_id" && fieldName != "profile_id" )
          {
            QTableWidgetItem *piState = new QTableWidgetItem( "" );
            piState->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable );
            piState->setCheckState( pa_db::settings::isDbFieldMapped( fieldName ) ? Qt::Checked : Qt::Unchecked );
            tblMapping->setItem( item, 0, piState );
            
            QTableWidgetItem *piName = new QTableWidgetItem( fieldName );
            piName->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
            tblMapping->setItem( item, 1, piName );
            
            QTableWidgetItem *piMapName = new QTableWidgetItem( pa_db::settings::nameDbFieldMapped( fieldName ) );
            piMapName->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
            tblMapping->setItem( item, 2, piMapName );
            item++;
          }
        }
      }
    }
  }
}

void QgsPadbSettingsDialog::saveMappingTable() const
{
  const int rowCount = tblMapping->rowCount();
  for ( int i = 0; i < rowCount; ++i )
  {
    const QString fieldName = tblMapping->item( i, 1 )->text();
    pa_db::settings::setDbFieldMapped( fieldName, tblMapping->item( i, 0 )->checkState() == Qt::Checked );
    pa_db::settings::setNameDbFieldMapped( fieldName, tblMapping->item( i, 2 )->text() );
  }
}

QStringList QgsPadbSettingsDialog::i18nList()
{
  QStringList myList;
  myList << "en_US"; //there is no qm file for this so we add it manually
  QString myI18nPath = QgsApplication::i18nPath();
  QDir myDir( myI18nPath, "padb_*.qm" );
  QStringList myFileList = myDir.entryList();
  QStringListIterator myIterator( myFileList );
  while ( myIterator.hasNext() )
  {
    QString myFileName = myIterator.next();
    myList << myFileName.replace( "padb_", "" ).replace( ".qm", "" );
  }
  return myList;
}

// void QgsPadbSettingsDialog::on_buttonBox_accepted()
// {
  // saveSettings();
  // accept();
// }

// void QgsPadbSettingsDialog::on_buttonBox_rejected()
// {
  // reject();
// }
