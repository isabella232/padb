#include "properties_page_regime.h"
#include "resources.h"
#include <QDataWidgetMapper>
#include <QSqlTableModel>

pa_db::properties_page_regime::properties_page_regime( int object_id, QWidget *parent )
	: properties_page_obj_table( object_id, parent )
{
	create_text_exit( text::explicat( ), m_objects_table->fieldIndex( "explicat" ) );
	create_text_exit( text::zoning( ), m_objects_table->fieldIndex( "zoning" ) );
	create_text_exit( text::buffer( ), m_objects_table->fieldIndex( "buffer" ) );
	create_text_exit( text::border( ), m_objects_table->fieldIndex( "border" ) );
	create_text_exit( text::objects( ), m_objects_table->fieldIndex( "objects" ) );
	create_text_exit( text::measures( ), m_objects_table->fieldIndex( "measures" ) );
	create_text_exit( text::suggest( ), m_objects_table->fieldIndex( "suggest" ) );
	if ( m_table_mapper )
		m_table_mapper->toFirst( );
}

QString pa_db::properties_page_regime::page_name( ) const
{
	return text::page_title_regime( );
}

void pa_db::properties_page_regime::apply( )
{
	m_objects_table->submitAll( );
}

