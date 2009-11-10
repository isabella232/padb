#include "pa_manager.h"
#include <QAction>
#include <QCoreApplication>
#include <QDebug>
#include <QIcon>
#include <QInputDialog>
#include <QLabel>
#include <QMainWindow>
#include <QMenu>
#include <QMessageBox>
#include <QToolBar>
#include <QTranslator>
#include <QVBoxLayout>
#include <QFile>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <qgisinterface.h>
#include "qgsapplication.h"
#include "qgsmapcanvas.h"
#include "qgsmaptool.h"
#include "qgsmaplayerregistry.h"
#include "dock_widget.h"
#include "resources.h"
#include "properties_form.h"
#include "settings.h"
#include "objects_tree_widget.h"
#include "free_elements_widget.h"
#include "species_widget.h"
#include "select_object_tool.h"
// #include "qgsspeciesimportdialog.h"
#include "qgspadbsettingsdialog.h"
#include "qgsspeciespropertiesdialog.h"

namespace { namespace aux {
	void test_tool( QgsMapTool *tool, QAction *action, QgsMapTool *action_tool )
	{
		if ( action )
		{
			action->setCheckable( true );
			action->setChecked( tool == action_tool );
		}
	}
} }

pa_db::pa_manager::pa_manager( QgisInterface *the_interface, QObject *parent )
	: QObject( parent )
	, m_qgis_iface( the_interface )
	, m_objects_list( this )
	, m_free_elements_cntrl( this )
	, m_objects_tree( the_interface, this )
	, m_elements_list( this )
	, mSpeciesController( this )
	, mSpeciesList( this )
{
	QString myLocale = settings::padbLocale();
	QTranslator *translator = new QTranslator( this );
	if ( translator->load( QString( "padb_" ) + myLocale, QgsApplication::i18nPath() ) )
	{
		QCoreApplication::installTranslator( translator );
	}
	
	// QTranslator *translator = new QTranslator( this );
	// bool load_tr = translator->load( "padb_ru_RU", QgsApplication::i18nPath( ) );
	// Q_ASSERT( load_tr );
	// QCoreApplication::installTranslator( translator );

	QMainWindow *main_window = qobject_cast< QMainWindow*>( m_qgis_iface->mainWindow( ) );
	Q_ASSERT( main_window );
	if ( main_window )
	{
		dock_widget *dock = new dock_widget( pa_db::text::pa_db_plugin_name_in_dock_widget( ), main_window );
		m_objects_tree.set_tree_widget( dock->objects_tree( ) );
		m_free_elements_cntrl.set_list_widget( dock->free_elements_list( ) );
		mSpeciesList.reload();
		mSpeciesController.setListWidget( dock->speciesList() );
		//mSpeciesList.reload();
		mSpeciesController.render( mSpeciesList );
		main_window->addDockWidget( Qt::RightDockWidgetArea, dock );
		m_docker = dock;
		for ( int nn = 0; nn < m_objects_list.count( ); ++nn )
			m_objects_tree.add_object( m_objects_list.at( nn ) );
		if ( !m_objects_list.is_db_open( ) )
			dock->show_error_message( );
		create_tool_bar( );
		m_tool_select = new select_object_tool( this );
	}
	else
	{
		qDebug( ) << "invalid pointer to main window";
	}

	m_settings_action = create_plugin_menu_action( text::settings( ), SLOT( onShowSettings() ) );
	m_ac_map_all_objs = create_plugin_menu_action( text::map_all_objects( ), SLOT( on_map_all_objects( ) ) );
	m_ac_map_all_objs->setEnabled( false );
	// mImportSpeciesAction = create_plugin_menu_action( tr( "Import species" ), SLOT( onImport() ) );
	m_info_action = create_plugin_menu_action( text::about( ), SLOT( on_about( ) ) );
//	connect( QgsMapLayerRegistry::instance( ), SIGNAL( layerWillBeRemoved( QString ) ), SLOT( on_change_layers_count( ) ) );
//	connect( QgsMapLayerRegistry::instance( ), SIGNAL( layerWasAdded( QgsMapLayer* ) ), SLOT( on_change_layers_count( ) ) );
//	connect( QgsMapLayerRegistry::instance( ), SIGNAL( removedAll( ) ), SLOT( on_change_layers_count( ) ) );
	connect( m_qgis_iface->mapCanvas( ), SIGNAL( mapToolSet( QgsMapTool* ) ),
		SLOT( on_map_tool_set( QgsMapTool* ) ) );
	
	if ( m_objects_list.is_db_open() )
	{
		// set visibility of species widget
		dock_widget *dw = qobject_cast< dock_widget*> ( m_docker );
		if ( pa_db::settings::speciesVisibleFlag() == Qt::Checked )
		{
			dw->hideSpecies();
		}
		else
		{
			dw->showSpecies();
		}
	}
}

QPointer< QAction > pa_db::pa_manager::create_plugin_menu_action( const QString &text, const char *sl )
{
	QPointer< QAction > result( new QAction( text, this ) );
	connect( result, SIGNAL( triggered( ) ), sl );
	m_qgis_iface->addPluginToMenu( text::pa_db_plugin_name( ), result );
	return result;
}

pa_db::pa_manager::~pa_manager( )
{
	if ( m_docker )
		m_docker->deleteLater( );
	if ( m_tool_bar )
		m_tool_bar->deleteLater( );
	if ( m_info_action )
		m_info_action->deleteLater( );
	if ( m_settings_action )
		m_settings_action->deleteLater( );
	if ( m_tool_select )
		m_tool_select->deleteLater( );
	if ( m_ac_map_all_objs )
		m_ac_map_all_objs->deleteLater( );
}

void pa_db::pa_manager::create_tool_bar( )
{
	m_tool_bar = m_qgis_iface->addToolBar( pa_db::text::pa_db_plugin_name_in_tool_bar( ) );
	m_tool_bar->setObjectName( pa_db::text::pa_db_plugin_name_in_tool_bar( ) );

	QAction *load_list = m_tool_bar->addAction( QPixmap( pa_db::icon::oopt_toolbar_list ), pa_db::text::load_object_list( ) );
	connect( load_list, SIGNAL( triggered( ) ), SLOT( on_load_pa_objects_list( ) ) );
	QAction *create_object = m_tool_bar->addAction( QPixmap( pa_db::icon::oopt_toolbar_add ), pa_db::text::create_object( ) );
	connect( create_object, SIGNAL( triggered( ) ), SLOT( on_create_pa_object( ) ) );
	m_action_select = m_tool_bar->addAction( QPixmap( pa_db::icon::oopt_toolbar_select ), pa_db::text::select_object( ) );
	connect( m_action_select, SIGNAL( triggered( ) ), SLOT( on_select_object( ) ) );
}

void pa_db::pa_manager::on_load_pa_objects_list( )
{
	m_elements_list.reload( );
	update_widget( );
}

void pa_db::pa_manager::on_create_pa_object( )
{
	create_object( );
}

const pa_db::pa_object* pa_db::pa_manager::create_object( const QString &obj_name )
{
	QString new_object_name = QInputDialog::getText( m_qgis_iface->mainWindow( ),
		pa_db::text::pa_db_plugin_name( ),
		pa_db::text::input_new_object_name( ),
		QLineEdit::Normal,
		obj_name );
	if ( !new_object_name.isEmpty( ) )
	{
		const pa_object *new_obj = m_objects_list.create_object( new_object_name );
		if ( new_obj )
			m_objects_tree.add_object( *new_obj );
		return new_obj;
	}
	return 0;
}

void pa_db::pa_manager::onShowSettings()
{
	//settings_form form( qgis_iface( )->mainWindow( ) );
	//form.exec( );
	QgsPadbSettingsDialog form( qgis_iface()->mainWindow() );
	form.exec();
	
	mSpeciesList.reload();
	mSpeciesController.render( mSpeciesList );
	
	m_objects_list.load();
	m_objects_tree.render( m_objects_list );
	m_objects_tree.update_icons();
	
	if ( m_objects_list.is_db_open() )
	{
		// set visibility of species widget
		dock_widget *dw = qobject_cast< dock_widget*> ( m_docker );
		if ( pa_db::settings::speciesVisibleFlag() == Qt::Checked )
		{
			dw->hideSpecies();
		}
		else
		{
			dw->showSpecies();
		}
	}
}

// void pa_db::pa_manager::onImport()
// {
	// QgsSpeciesImportDialog form( qgis_iface()->mainWindow() );
	// form.exec();
	// mSpeciesList.reload();
	// mSpeciesController.render( mSpeciesList );
// }

void pa_db::pa_manager::show_object_menu( const QPoint &pos, int object_id )
{
	if ( object_id != -1 )
	{
		const int index = m_objects_list.find_object( object_id );
		if ( index != -1 )
		{
			QMenu menu;
			QAction *ac_select = menu.addAction( text::select( ) );
			QAction *ac_select_zoom = menu.addAction( text::select_and_zoom( ) );
			menu.addSeparator( );
			const QAction *ac_delete = menu.addAction( pa_db::text::delete_object( ) );
			menu.addSeparator( );
			const QAction *ac_properties = menu.addAction( pa_db::text::properties( ) );
			menu.addSeparator( );
			QAction *ac_mapping = menu.addAction( text::mapping( ) );
			if ( m_objects_list.at( index ).elements( ).count( ) == 0 )
			{
				ac_select->setEnabled( false );
				ac_select_zoom->setEnabled( false );
				ac_mapping->setEnabled( false );
			}

			const QAction *result = menu.exec( pos );
			if ( result == ac_delete )
				delete_object( object_id );
			else if ( result == ac_select )
				select( object_id, false );
			else if ( result == ac_select_zoom )
				select( object_id, true );
			else if ( result == ac_properties )
				show_properties_form_ALTER( object_id );
			else if ( result == ac_mapping )
				map_object_data_to_shape( object_id );
		}
	}
	else
	{
		QMenu menu;
		connect( menu.addAction( pa_db::text::create_object( ) ),
			SIGNAL( triggered( ) ), SLOT( on_create_pa_object( ) ) );
		menu.exec( pos );
	}
}

void pa_db::pa_manager::move_free_element_to_obj( const pa_element &el, int object_id )
{
	m_objects_list.add_element( object_id, el );
	m_objects_tree.add_element( object_id, el );
	update_widget( );
}

void pa_db::pa_manager::move_element_from_obj_to_free( const pa_element &el )
{
	const int obj_index = m_objects_list.find_object( el );
	if ( obj_index != -1 )
	{
		m_objects_tree.delete_element( m_objects_list.at( obj_index ).id( ), el );
		m_objects_list.delete_element( m_objects_list.at( obj_index ).id( ), el );
		update_widget( );
	}
}

void pa_db::pa_manager::move_element_from_obj_to_other( const pa_element &el, int object_id )
{
	move_element_from_obj_to_free( el );
	move_free_element_to_obj( el, object_id );
}

//function for species management
void pa_db::pa_manager::speciesToObject( const pa_species &el, int object_id )
{
	m_objects_list.addSpecies( object_id, el );
	//QMessageBox::warning( m_qgis_iface->mainWindow(), "DEBUG", QString( "in id %1" ).arg( object_id ) );
	m_objects_tree.addSpecies( object_id, el );
	update_widget();
}

void pa_db::pa_manager::speciesFromObject( const pa_species &el )
{
	//const int obj_index = m_objects_list.find_object( el );
	const int obj_index = el.parentId();
	//QMessageBox::warning( m_qgis_iface->mainWindow(), "DEBUG", QString( " out id %1" ).arg( obj_index ) );
	if ( obj_index != -1 )
	{
		//m_objects_tree.deleteSpecies( m_objects_list.at( obj_index ).id(), el );
		m_objects_tree.deleteSpecies( obj_index, el ); // obj_index - индекс (id) элемента
		m_objects_list.deleteSpecies( obj_index, el );
		//m_objects_list.deleteSpecies( m_objects_list.at( obj_index ).id( ), el );
		update_widget();
	}
}

void pa_db::pa_manager::showSpeciesProperties( const pa_species &el )
{
	QgsSpeciesPropertiesDialog form( qgis_iface()->mainWindow(), el );
	form.exec();
	//m_elements_list.select( el, zoom );
}

QgisInterface *pa_db::pa_manager::qgis_iface( ) const
{
	return m_qgis_iface;
}

void pa_db::pa_manager::select( const pa_element &el, bool zoom )
{
	m_elements_list.select( el, zoom );
}

void pa_db::pa_manager::select( int object_id, bool zoom )
{
	m_elements_list.select( m_objects_list.at( m_objects_list.find_object( object_id ) ).elements( ), zoom );
}

bool pa_db::pa_manager::is_element_free( const pa_element &el ) const
{
	return m_objects_list.is_element_free( el );
}

bool pa_db::pa_manager::is_element_valid( const pa_element &el ) const
{
	return m_elements_list.is_element_valid( el );
}

void pa_db::pa_manager::update_widget( )
{
	m_free_elements_cntrl.render( m_elements_list.free_elements( ) );
	m_objects_tree.update_icons( );
}

void pa_db::pa_manager::delete_object( int object_id )
{
	const int index = m_objects_list.find_object( object_id );
	if ( index != -1 )
	{
		if ( QMessageBox::question( m_qgis_iface->mainWindow( ), pa_db::text::pa_db_plugin_name( )
			, pa_db::text::question_delete_object( ).arg( m_objects_list.at( index ).name( ) )
			, QMessageBox::Yes|QMessageBox::No, QMessageBox::No ) == QMessageBox::Yes )
		{
			m_objects_list.delete_object( object_id );
			m_objects_tree.delete_object( object_id );
			update_widget( );
		}
	}
}

void pa_db::pa_manager::show_properties_form_ALTER( int object_id )
{
	properties_form form( object_id, this );
	form.exec( );
	if ( form.is_changes_made( ) )
	{
		m_objects_list.load( object_id );
		const int index = m_objects_list.find_object( object_id );
		m_objects_tree.update_object_name( object_id, m_objects_list.at( index ).name( ) );
	}
}

bool pa_db::pa_manager::object_has_elements( int object_id ) const
{
	const int index = m_objects_list.find_object( object_id );
	return index != -1 && m_objects_list.at( index ).elements( ).count( ) != 0;
}

void pa_db::pa_manager::on_about( )
{
	QDialog dlg( m_qgis_iface->mainWindow( ) );
	dlg.setWindowFlags( dlg.windowFlags( ) | Qt::MSWindowsFixedSizeDialogHint );
	dlg.setWindowFlags( dlg.windowFlags( ) &~ Qt::WindowContextHelpButtonHint );
	QVBoxLayout *lines = new QVBoxLayout( &dlg );
	lines->addWidget( new QLabel( "<b>" + text::version( ) + "</b>" ) );
	lines->addWidget( new QLabel( "<b>" + text::developers( ) + "</b>" ) );
	lines->addWidget( new QLabel( "    sploid" ) );
	lines->addWidget( new QLabel( "    Maxim Dubinin" ) );
	lines->addWidget( new QLabel( "<b>" + text::link( ) + "</b>" ) );
	QLabel *link = new QLabel( "<a href=\"http://gis-lab.info/programs/padb\">http://gis-lab.info/programs/padb</a>" );
	link->setOpenExternalLinks( true );
	lines->addWidget( link );

	dlg.exec( );
}

void pa_db::pa_manager::map_object_data_to_shape( int object_id )
{
	QMap< QString, QVariant > fields;
	QSqlDatabase db = QSqlDatabase::database( pa_db::text::database_connection_name( ) );
	if ( db.isOpen( ) )
	{
		QSqlQuery query( db );
		if ( query.exec( QString(
			"SELECT objects.*, status.name AS status_name,profile.name AS profile_name,category.name AS category_name "\
			"FROM objects,status,profile,category "\
			"WHERE objects.id=%1 AND objects.status_id=status.id AND objects.profile_id=profile.id AND objects.category_id=category.id" )
			.arg( object_id ) ) )
		{
			if ( query.next( ) )
			{
				const QSqlRecord rec_desc = query.record( );
				for ( int nn = 0; nn < rec_desc.count( ); ++nn )
				{
					if ( settings::isDbFieldMapped( rec_desc.fieldName( nn ) ) )
					{
						const QVariant value = query.value( nn );
						if ( !value.isNull( ) && value.isValid( ) )
						{
							fields[ settings::nameDbFieldMapped( rec_desc.fieldName( nn ) ) ] =
								value;
						}
					}
				}
			}
		}
		else
		{
			qDebug( ) << query.lastError( ).text( );
		}
	}
	m_elements_list.fill_elements_data(
		m_objects_list.at( m_objects_list.find_object( object_id ) ).elements( ),
		fields );
}

void pa_db::pa_manager::on_select_object( )
{
	m_qgis_iface->mapCanvas( )->setMapTool( m_tool_select );
}

void pa_db::pa_manager::on_map_tool_set( QgsMapTool *tool )
{
	aux::test_tool( tool, m_action_select, m_tool_select );
}

void pa_db::pa_manager::try_show_object_properties( const QString &layer_id, int feature_id )
{
	pa_element el;
	el.set_layer_id( layer_id );
	el.set_feature_id( feature_id );
	const int iobj = m_objects_list.find_object( el );
	if ( iobj == -1 )
	{
		QMessageBox::warning( m_qgis_iface->mainWindow( ), QObject::tr( "Object not found" ),
			QObject::tr( "Object not found" ) );
	}
	else
	{
		show_properties_form_ALTER( m_objects_list.at( iobj ).id( ) );
	}
}

/*void pa_db::pa_manager::on_change_layers_count( )
{
	on_load_pa_objects_list( );
	bool all_is_valid = true;
	for ( int nn = 0; nn < 	m_objects_list.count( ) && all_is_valid; ++nn )
	{
		const QVector< pa_element > el = m_objects_list.at( nn ).elements( );
		for ( int nn = 0; nn < el.count( ); ++nn )
			all_is_valid = all_is_valid && is_element_valid( el.at( nn ) );
	}
	if ( m_ac_map_all_objs )
		m_ac_map_all_objs->setEnabled( all_is_valid );
}*/

void pa_db::pa_manager::on_map_all_objects( )
{
	for ( int nn = 0; nn < m_objects_list.count( ); ++nn )
	{
		map_object_data_to_shape( m_objects_list.at( nn ).id( ) );
	}
}
