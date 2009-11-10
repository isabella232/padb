#include "settings_form.h"
#include <QVBoxLayout>
#include <QGroupBox>
#include <QComboBox>
#include <QPushButton>
#include <QLineEdit>
#include <QFormLayout>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QTableWidget>
#include <QCoreApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QLabel>
#include <QHeaderView>
#include <QMenu>
#include <qgisinterface.h>
#include "resources.h"
#include "settings.h"
#include "qgsmaplayerregistry.h"
#include "qgsvectorlayer.h"
#include "qgsvectordataprovider.h"

pa_db::settings_form::settings_form( QWidget *parent )
	: QDialog( parent )
	, m_data_mapping( 0 )
{
	setWindowFlags( windowFlags( ) | Qt::MSWindowsFixedSizeDialogHint );
	setWindowFlags( windowFlags( ) &~ Qt::WindowContextHelpButtonHint );
	QVBoxLayout *main_layout = new QVBoxLayout( this );
	QGroupBox *group_box_db = new QGroupBox( text::group_box_db( ), this );
	main_layout->addWidget( group_box_db );
	create_db_settings( group_box_db );

	QGroupBox *group_box_zoom = new QGroupBox( text::group_box_zoom( ), this );
	main_layout->addWidget( group_box_zoom );
	QFormLayout *zoom_box_layout = new QFormLayout( group_box_zoom );
	zoom_box_layout->addRow( text::min_zoom( ), m_min_zoom = new QLineEdit( group_box_zoom ) );
	m_min_zoom->setText( settings::min_zoom( ) );

	QGroupBox *group_box_exp_layer = new QGroupBox( text::exp_layer( ) );
	main_layout->addWidget( group_box_exp_layer );
	QHBoxLayout *exp_l_l = new QHBoxLayout( group_box_exp_layer );
	exp_l_l->addWidget( m_exp_layer = new QComboBox( group_box_exp_layer ) );
	m_exp_layer->setSizeAdjustPolicy( QComboBox::AdjustToContents );
	exp_l_l->addStretch( );
	fill_layrs_list( );
	const QString name_exp_lay = settings::name_export_layer( );
	if ( !name_exp_lay.isEmpty( )
		&& m_exp_layer->findText( name_exp_lay ) != -1 )
	{
		m_exp_layer->setCurrentIndex( m_exp_layer->findText( name_exp_lay ) );
	}

	QGroupBox *group_box_mapping = new QGroupBox( text::db_mapping( ) );
	main_layout->addWidget( group_box_mapping );
	QVBoxLayout *mapping_layout = new QVBoxLayout( group_box_mapping );
	mapping_layout->addWidget( new QLabel( text::db_mapping_comment( ), group_box_mapping ) );
	m_data_mapping = new QTableWidget( group_box_mapping );
	mapping_layout->addWidget( m_data_mapping );
	connect( m_data_mapping, SIGNAL( cellDoubleClicked( int, int ) ),
		SLOT( on_cell_double_clicked( int, int ) ) );
	fill_mapping_table( );

	QHBoxLayout *buttons_layout = new QHBoxLayout;
	buttons_layout->addStretch( );

	QPushButton *bt_ok = new QPushButton( text::ok( ), this );
	connect( bt_ok, SIGNAL( clicked( bool ) ), SLOT( on_ok( ) ) );
	buttons_layout->addWidget( bt_ok );

	QPushButton *bt_cancel = new QPushButton( text::cancel( ), this );
	connect( bt_cancel, SIGNAL( clicked( bool ) ), SLOT( on_cancel( ) ) );
	buttons_layout->addWidget( bt_cancel );

	main_layout->addLayout( buttons_layout );

	setMinimumWidth( 400 );
}

void pa_db::settings_form::on_allpy( )
{
	const QString new_file_name = m_db_path->text( );
	if ( !QFile::exists( new_file_name )
		&& QMessageBox::question( this, QCoreApplication::applicationName( ),
		text::question_create_empty_db( ), QMessageBox::Yes|QMessageBox::No ) == QMessageBox::Yes )

	{
		if ( settings::create_empy_db_file( m_db_path->text( ) ) )
		{
			settings::set_db_file_name( m_db_path->text( ) );
		}
		else
		{
			QMessageBox::critical( this, QCoreApplication::applicationName( ),
				text::error_create_empty_db( ) );
			return;
		}
	}
	settings::set_min_zoom( m_min_zoom->text( ) );
	apply_mapping_table( );
	settings::set_name_export_layer( m_exp_layer->currentText( ) );
}

void pa_db::settings_form::on_cancel( )
{
	close( );
}

void pa_db::settings_form::on_ok( )
{
	on_allpy( );
	close( );
}

void pa_db::settings_form::create_db_settings( QGroupBox *db_box )
{
	QFormLayout *db_layout = new QFormLayout( db_box );
	QHBoxLayout *db_path_layout = new QHBoxLayout;
	db_path_layout->addWidget( m_db_path = new QLineEdit( db_box ) );
	m_db_path->setText( settings::db_file_name( ) );
	QPushButton *open_path = new QPushButton( "...", db_box );
	db_path_layout->addWidget( open_path );
	connect( open_path, SIGNAL( clicked( bool ) ), SLOT( on_open_path( ) ) );
	db_layout->addRow( text::db_file( ), db_path_layout );
	db_layout->addRow( new QLabel( text::locate_db_file( ), db_box ) );
}

void pa_db::settings_form::on_open_path( )
{
	const QFileInfo info( settings::db_file_name( ) );
	QFileDialog file_dlg( this, text::open_file( ) );
	file_dlg.setFilter( text::open_file_filter( ) );
	file_dlg.setAcceptMode( QFileDialog::AcceptSave );
	file_dlg.setConfirmOverwrite( false );
	if ( info.exists( ) )
	{
		file_dlg.setDirectory( info.absolutePath( ) );
		file_dlg.selectFile( info.fileName( ) );
	}
	if ( file_dlg.exec( ) )
	{
		const QStringList file_names = file_dlg.selectedFiles( );
		if ( file_names.count( ) )
		{
			m_db_path->setText( file_names[ 0 ] );
		}
	}
}

void pa_db::settings_form::fill_mapping_table( )
{
	QSqlDatabase db = QSqlDatabase::database( pa_db::text::database_connection_name( ) );
	if ( db.isOpen( ) )
	{
		QSqlQuery query( db );
		if ( query.exec( QString(
			"SELECT objects.*, status.name AS status_name,profile.name AS profile_name,category.name AS category_name "\
			"FROM objects,status,profile,category "\
			"WHERE objects.status_id=status.id AND objects.profile_id=profile.id AND objects.category_id=category.id" )
			) )
		{
			if ( query.next( ) )
			{
				const QSqlRecord rec_desc = query.record( );
				m_data_mapping->setColumnCount( 3 );
				m_data_mapping->setHorizontalHeaderLabels( QStringList( ) << "" << text::field1( ) << text::field2( ) );
				m_data_mapping->horizontalHeader( )->setClickable( false );
				m_data_mapping->verticalHeader( )->setClickable( false );
				m_data_mapping->setColumnWidth( 0, 30 );
				m_data_mapping->horizontalHeader( )->setResizeMode( 1, QHeaderView::Stretch );
				m_data_mapping->horizontalHeader( )->setResizeMode( 2, QHeaderView::Stretch );
				m_data_mapping->setRowCount( rec_desc.count( ) - 3 );
				int cur_item = 0;
				for ( int nn = 0; nn < rec_desc.count( ); ++nn )
				{
					const QString field_name = rec_desc.fieldName( nn );
					if ( field_name != "status_id"
						&& field_name != "category_id"
						&& field_name != "profile_id" )
					{
						QTableWidgetItem *pi_state = new QTableWidgetItem( "" );
						pi_state->setFlags( Qt::ItemIsSelectable|Qt::ItemIsEnabled|Qt::ItemIsUserCheckable );
						pi_state->setCheckState( settings::is_db_field_mapped( field_name ) ? Qt::Checked : Qt::Unchecked );
						m_data_mapping->setItem( cur_item, 0, pi_state );

						QTableWidgetItem *pi_name = new QTableWidgetItem( field_name );
						pi_name->setFlags( Qt::ItemIsSelectable|Qt::ItemIsEnabled );
						m_data_mapping->setItem( cur_item, 1, pi_name );

						QTableWidgetItem *pi_map_name = new QTableWidgetItem( settings::name_db_field_mapped( field_name ) );
						pi_map_name->setFlags( Qt::ItemIsSelectable|Qt::ItemIsEnabled );
						m_data_mapping->setItem( cur_item, 2, pi_map_name );
						cur_item++;
					}
				}
			}
		}
	}
}

void pa_db::settings_form::apply_mapping_table( ) const
{
	const int row_count = m_data_mapping->rowCount( );
	for ( int nn = 0; nn < row_count; ++nn )
	{
		const QString field_name = m_data_mapping->item( nn, 1 )->text( );
		settings::set_db_field_mapped( field_name, m_data_mapping->item( nn, 0 )->checkState( ) == Qt::Checked );
		settings::set_name_db_field_mapped( field_name, m_data_mapping->item( nn, 2 )->text( ) );
	}
}

void pa_db::settings_form::fill_layrs_list( )
{
	const QMap< QString, QgsMapLayer* > layers = QgsMapLayerRegistry::instance( )->mapLayers( );
	for ( QMap< QString, QgsMapLayer* >::const_iterator it_layer = layers.begin( );
		it_layer != layers.end( ); ++it_layer )
	{
		if ( QgsVectorLayer *cur_layrer = qobject_cast< QgsVectorLayer* >( it_layer.value( ) ) )
		{
			m_exp_layer->addItem( cur_layrer->name( ) );
		}
	}
}

void pa_db::settings_form::on_cell_double_clicked( int row, int column )
{
	if ( column == 2
		&& m_data_mapping->item( row, column ) )
	{
		QMenu mn;
		bool found = false;
		const QMap< QString, QgsMapLayer* > layers = QgsMapLayerRegistry::instance( )->mapLayers( );
		for ( QMap< QString, QgsMapLayer* >::const_iterator it_layer = layers.begin( );
			it_layer != layers.end( ); ++it_layer )
		{
			if ( QgsVectorLayer *cur_layrer = qobject_cast< QgsVectorLayer* >( it_layer.value( ) ) )
			{
				if ( cur_layrer->name( ) == m_exp_layer->currentText( ) )
				{
					QgsVectorDataProvider *data_provider = cur_layrer->dataProvider( );
					const QgsFieldMap fi = data_provider->fields( );
					for ( QgsFieldMap::const_iterator it = fi.constBegin( );
						it != fi.constEnd( ); ++it )
					{
						found = true;
						mn.addAction( it.value( ).name( ) );
					}
				}
			}
		}
		if ( found )
		{
			QAction *sel_ac = mn.exec( QCursor::pos( ) );
			if ( sel_ac )
			{
				m_data_mapping->item( row, column )->setText( sel_ac->text( ) );
			}
		}

	}
}

