#include "properties_page_nature.h"
#include "resources.h"
#include "utils.h"
#include <QFormLayout>
#include <QDataWidgetMapper>
#include <QSqlTableModel>
#include <QDebug>
#include <QTextEdit>
#include <QLabel>

pa_db::properties_page_nature::properties_page_nature( int object_id, QWidget *parent )
	: properties_page_obj_table( object_id, parent )
{
	create_text_exit( text::geogr( ), m_objects_table->fieldIndex( "geogr" ) );
	create_text_exit( text::relief( ), m_objects_table->fieldIndex( "relief" ) );
	create_text_exit( text::climate( ), m_objects_table->fieldIndex( "climate" ) );
	create_text_exit( text::veg( ), m_objects_table->fieldIndex( "veg" ) );
	create_text_exit( text::flora( ), m_objects_table->fieldIndex( "flora" ) );
	create_text_exit( text::fauna( ), m_objects_table->fieldIndex( "fauna" ) );
 	if ( m_table_mapper )
 		m_table_mapper->toFirst( );
}

QString pa_db::properties_page_nature::page_name( ) const
{
	return text::page_title_nature( );
}

void pa_db::properties_page_nature::apply( )
{
	m_objects_table->submitAll( );
}

