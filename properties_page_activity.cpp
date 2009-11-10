#include "properties_page_activity.h"
#include "resources.h"
#include <QDataWidgetMapper>
#include <QSqlTableModel>

pa_db::properties_page_activity::properties_page_activity( int object_id, QWidget *parent )
	: properties_page_obj_table( object_id, parent )
{
	create_text_exit( text::science( ), m_objects_table->fieldIndex( "science" ) );
	create_text_exit( text::educat( ), m_objects_table->fieldIndex( "educat" ) );
	create_text_exit( text::cost( ), m_objects_table->fieldIndex( "cost" ) );
	create_text_exit( text::add_label( ), m_objects_table->fieldIndex( "add" ) );
	create_line_edit( text::creator( ), m_objects_table->fieldIndex( "creator" ) );
	if ( m_table_mapper )
		m_table_mapper->toFirst( );
}

QString pa_db::properties_page_activity::page_name( ) const
{
	return text::page_title_activity( );
}

void pa_db::properties_page_activity::apply( )
{
	m_objects_table->submitAll( );
}

