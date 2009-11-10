#include "properties_form.h"
#include "properties_page_common.h"
#include "properties_page_docs.h"
#include "properties_page_nature.h"
#include "properties_page_regime.h"
#include "properties_page_organization.h"
#include "properties_page_importance.h"
#include "properties_page_subjection.h"
#include "properties_page_activity.h"
#include "qgsprojectionselector.h"
#include "resources.h"
#include "pa_manager.h"
#include "qgisinterface.h"
#include "qgsmapcanvas.h"
#include <QTabWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFile>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QVariant>
#include <QSqlError>
#include <QDebug>
#include <QTextStream>
#include <QDesktopServices>
#include <QUrl>
#include <QBuffer>
#include <QMessageBox>
#include <algorithm>
#include <functional>
#include "qgsmapcanvas.h"
#include "qgsmaprenderer.h"
#include "settings.h"

namespace { namespace aux {
	QString get_output_file( )
	{
		int current_index = 0;
		while ( true )
		{
 			const QString test_file_name =
				QDesktopServices::storageLocation( QDesktopServices::DocumentsLocation ) +
				"/qgis_pa_db_" + QString::number( current_index ) + ".doc";
 			if ( !QFile::exists( test_file_name ) )
				return test_file_name;
 			current_index++;
		}
	}
	const QString first_image_part( "<w:p>\n<w:r>\n<w:pict>\n<w:binData w:name=\"wordml://03000001.png\">" );
	const QString second_image_part( "</w:binData>\n"
		"<v:shape id=\"_x0000_i1025\" type=\"#_x0000_t75\" style=\"width:400pt;height:400pt\">\n"
		"<v:imagedata src=\"wordml://03000001.png\" o:title=\"network\"/>\n"
		"</v:shape>\n</w:pict>\n</w:r>\n</w:p>" );
	QString replace_key( const QString &key )
	{
		return "${" + key + "}";
	}

} }


pa_db::properties_form::properties_form( int object_id, pa_manager *manager )
	: QDialog( manager->qgis_iface( )->mainWindow( ) )
	, m_object_id( object_id )
	, m_changes_was_made( false )
	, m_manager( manager )
{
	setWindowFlags( windowFlags( ) | Qt::MSWindowsFixedSizeDialogHint );
	setWindowFlags( windowFlags( ) &~ Qt::WindowContextHelpButtonHint );
	setMaximumWidth( 600 );
	setMinimumWidth( 600 );
	setWindowTitle( pa_db::text::pa_db_plugin_name( ) );
	QVBoxLayout *main_layout = new QVBoxLayout( this );
	m_tabs = new QTabWidget( this );
	m_tabs->setMaximumHeight( 500 );
	main_layout->addWidget( m_tabs );
	add_page( new properties_page_common( object_id, m_tabs ) );
	add_page( new properties_page_importance( object_id, m_tabs ) );
	add_page( new properties_page_subjection( object_id, m_tabs ) );
	add_page( new properties_page_nature( object_id, m_tabs ) );
	add_page( new properties_page_regime( object_id, m_tabs ) );
	add_page( new properties_page_organization( object_id, m_tabs ) );
	add_page( new properties_page_activity( object_id, m_tabs ) );
	add_page( new properties_page_docs( object_id, m_tabs ) );

	QHBoxLayout *buttons = new QHBoxLayout;
	main_layout->addLayout( buttons );

	QPushButton *but_report = new QPushButton( pa_db::text::report( ), this );
	buttons->addWidget( but_report );
	connect( but_report, SIGNAL( clicked( bool ) ), SLOT( on_report( ) ) );

	buttons->addStretch( 1 );
	buttons->addWidget( m_apply = new QPushButton( pa_db::text::apply( ), this ) );
	connect( m_apply, SIGNAL( clicked( bool ) ), SLOT( on_apply( ) ) );
	m_apply->setEnabled( false );

	QPushButton *but_cancel = new QPushButton( pa_db::text::cancel( ), this );
	buttons->addWidget( but_cancel );
	connect( but_cancel, SIGNAL( clicked( bool ) ), SLOT( on_cancel( ) ) );

	QPushButton *but_ok = new QPushButton( pa_db::text::ok( ), this );
	buttons->addWidget( but_ok );
	connect( but_ok, SIGNAL( clicked( bool ) ), SLOT( on_ok( ) ) );
}

void pa_db::properties_form::add_page( properties_page_base *page )
{
	m_pages.push_back( page );
	m_tabs->addTab( page, page->page_name( ) );
	connect( page, SIGNAL( changed( ) ), SLOT( on_change( ) ) );
}

void pa_db::properties_form::on_change( )
{
	m_apply->setEnabled( true );
}

void pa_db::properties_form::on_apply( )
{
	std::for_each( m_pages.begin( ), m_pages.end( ), std::mem_fun( &properties_page_base::apply ) );
	m_apply->setEnabled( false );
	m_changes_was_made = true;
}

void pa_db::properties_form::on_ok( )
{
	if ( m_apply->isEnabled( ) )
		on_apply( );
	close( );
}

void pa_db::properties_form::on_cancel( )
{
	close( );
}

bool pa_db::properties_form::is_changes_made( ) const
{
	return m_changes_was_made;
}

void pa_db::properties_form::on_report( )
{
	QFile f_report( ":/pa_db/report_template.doc" );
	if ( f_report.open( QIODevice::ReadOnly|QIODevice::Text ) )
	{
		QString report_string = QString::fromUtf8( f_report.readAll( ) );
		report_string.replace( aux::replace_key( "image" ), image_replace_text( ) );
		report_string.replace( aux::replace_key( "projection" ), projection_replace_text( ) );

		QSqlDatabase db = QSqlDatabase::database( pa_db::text::database_connection_name( ) );
		if ( db.isOpen( ) )
		{
			QSqlQuery query( db );
			if ( query.exec( QString(
				"SELECT objects.*, status.name AS status_name,profile.name AS profile_name,category.name AS category_name "\
				"FROM objects,status,profile,category "\
				"WHERE objects.id=%1 AND objects.status_id=status.id AND objects.profile_id=profile.id AND objects.category_id=category.id" )
				.arg( m_object_id ) ) )
			{
				if ( query.next( ) )
				{
					const QSqlRecord rec_desc = query.record( );
					for ( int nn = 0; nn < rec_desc.count( ); ++nn )
					{
						QVariant value = query.value( nn );
						QString new_string( "" );
						const QString replace = aux::replace_key( rec_desc.fieldName( nn ) );
						if ( !value.isNull( ) && value.isValid( ) && !value.toString( ).isEmpty( ) )
							new_string = value.toString( );
						report_string.replace( replace, new_string );
					}
					const QString output_file = aux::get_output_file( );
					QFile out( output_file );
					if ( out.open( QIODevice::WriteOnly ) )
					{
						QTextStream stream( &out );
						stream.setCodec( "UTF-8" );
						stream << report_string;
						out.close( );
						QDesktopServices::openUrl( QUrl::fromLocalFile( output_file ) );
					}
					else
					{
						QMessageBox::warning( this, text::pa_db_plugin_name( ), text::cannot_open_file( ) );
					}
				}
			}
			else
			{
				qDebug( ) << query.lastError( ).text( );
			}
		}

		f_report.close( );
	}
}

QString pa_db::properties_form::image_replace_text( )
{
	QString result;
	if ( m_manager->object_has_elements( m_object_id ) )
	{
		m_manager->select( m_object_id, true );
		const double current_scale = m_manager->qgis_iface( )->mapCanvas( )->mapRenderer( )->scale( );
		if ( current_scale < settings::zoom() )
		{
			QgsRectangle scale_rect = m_manager->qgis_iface( )->mapCanvas( )->extent( );
			scale_rect.scale( double( settings::zoom() ) / current_scale );
			m_manager->qgis_iface( )->mapCanvas( )->setExtent( scale_rect );
			m_manager->qgis_iface( )->mapCanvas( )->updateFullExtent( );
		}
		const QPixmap can_pm = m_manager->qgis_iface( )->mapCanvas( )->canvasPixmap( );
		QByteArray image_data;
		{
			QBuffer buffer( &image_data );
			buffer.open( QIODevice::WriteOnly );
			can_pm.save( &buffer, "png" );
		}
		result = QString( "%1%2%3" )
			.arg( aux::first_image_part )
			.arg( QString::fromLatin1( image_data.toBase64( ) ) )
			.arg( aux::second_image_part );
	}
	return result;
}

QString pa_db::properties_form::projection_replace_text( )
{
	QgsProjectionSelector selector( this );
	long myCRSID = m_manager->qgis_iface( )->mapCanvas( )->mapRenderer( )->destinationSrs( ).srsid( );
	selector.setSelectedCrsId( myCRSID );
	selector.setMaximumSize( 1, 1 );
	selector.show( );
	const QString result = selector.selectedProj4String( );
	selector.hide( );
	return result;
}
