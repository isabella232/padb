#include "properties_page_subjection.h"
#include "resources.h"
#include <QDataWidgetMapper>
#include <QSqlTableModel>

pa_db::properties_page_subjection::properties_page_subjection( int object_id, QWidget *parent )
	: properties_page_obj_table( object_id, parent )
{
	create_text_exit( text::subst( ), m_objects_table->fieldIndex( "subst" ) );
	create_text_exit( text::address( ), m_objects_table->fieldIndex( "address" ) );
	create_text_exit( text::substpa( ), m_objects_table->fieldIndex( "substpa" ) );
	if ( m_table_mapper )
		m_table_mapper->toFirst( );
}

QString pa_db::properties_page_subjection::page_name( ) const
{
	return text::page_title_subjection( );
}

void pa_db::properties_page_subjection::apply( )
{
	m_objects_table->submitAll( );
}

