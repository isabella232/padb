#include "properties_page_importance.h"
#include "resources.h"
#include <QDataWidgetMapper>
#include <QSqlTableModel>

pa_db::properties_page_importance::properties_page_importance( int object_id, QWidget *parent )
	: properties_page_obj_table( object_id, parent )
{
	create_text_exit( text::norma( ), m_objects_table->fieldIndex( "norma" ) );
	create_text_exit( text::osnova( ), m_objects_table->fieldIndex( "osnova" ) );
	create_text_exit( text::diploms( ), m_objects_table->fieldIndex( "inter" ) );
	create_text_exit( text::sec_obj_list( ), m_objects_table->fieldIndex( "list" ) );
	if ( m_table_mapper )
		m_table_mapper->toFirst( );
}

QString pa_db::properties_page_importance::page_name( ) const
{
	return text::page_title_importance( );
}

void pa_db::properties_page_importance::apply( )
{
	m_objects_table->submitAll( );
}

