#include "properties_page_organization.h"
#include "resources.h"
#include <QFormLayout>
#include <QLineEdit>
#include <QDataWidgetMapper>
#include <QSqlTableModel>

pa_db::properties_page_organization::properties_page_organization( int object_id, QWidget *parent )
	: properties_page_obj_table( object_id, parent )
{
	create_spin_box( text::number( ), m_objects_table->fieldIndex( "number" ) );
	create_text_exit( text::codes( ), m_objects_table->fieldIndex( "codes" ) );
	create_text_exit( text::fulladdr( ), m_objects_table->fieldIndex( "fulladdr" ) );
	create_line_edit( text::phone( ), m_objects_table->fieldIndex( "phone" ) );
	create_line_edit( text::email( ), m_objects_table->fieldIndex( "email" ) );
	create_line_edit( text::fio( ), m_objects_table->fieldIndex( "fio" ) );
	create_text_exit( text::staff( ), m_objects_table->fieldIndex( "staff" ) );
	create_line_edit( text::protect( ), m_objects_table->fieldIndex( "protect" ) );
	create_text_exit( text::protectbuffer( ), m_objects_table->fieldIndex( "protectbuffer" ) );
	if ( m_table_mapper )
		m_table_mapper->toFirst( );
}

QString pa_db::properties_page_organization::page_name( ) const
{
	return text::page_title_organization( );
}

void pa_db::properties_page_organization::apply( )
{
	m_objects_table->submitAll( );
}

