#include "properties_page_common.h"
#include "resources.h"
#include "sql_relation_combo.h"
#include <QFormLayout>
#include <QDataWidgetMapper>
#include <QSqlTableModel>
#include <QComboBox>

pa_db::properties_page_common::properties_page_common( int object_id, QWidget *parent )
	: properties_page_obj_table( object_id, parent )
{
	create_line_edit( text::properties_name( ), m_objects_table->fieldIndex( "name" ) );
	create_line_edit( text::entity_number( ), m_objects_table->fieldIndex( "entity_number" ) );
	create_line_edit( text::kod_okato( ), m_objects_table->fieldIndex( "okato" ) );
	create_spin_box( text::cluster( ), m_objects_table->fieldIndex( "cluster" ) );
	create_text_exit( text::locate( ), m_objects_table->fieldIndex( "locate" ) );
	m_form_layout->addRow( text::category( ), m_category = new sql_relation_combo( this, m_database, "category" ) );
	m_form_layout->addRow( text::profile( ), m_profile = new sql_relation_combo( this, m_database, "profile" ) );
	m_form_layout->addRow( text::status_label( ), m_status = new sql_relation_combo( this, m_database, "status" ) );
	create_line_edit( text::year_create( ), m_objects_table->fieldIndex( "year" ), text::year_short( ) );
	create_line_edit( text::area( ), m_objects_table->fieldIndex( "area" ), text::area_short( ) );
	if ( m_table_mapper )
	{
		Q_ASSERT( m_objects_table->fieldIndex( "category_id" ) != -1 );
		m_table_mapper->addMapping( m_category, m_objects_table->fieldIndex( "category_id" ), "m_id" );
		Q_ASSERT( m_objects_table->fieldIndex( "profile_id" ) != -1 );
		m_table_mapper->addMapping( m_profile, m_objects_table->fieldIndex( "profile_id" ), "m_id" );
		Q_ASSERT( m_objects_table->fieldIndex( "status_id" ) != -1 );
		m_table_mapper->addMapping( m_status, m_objects_table->fieldIndex( "status_id" ), "m_id" );

		m_table_mapper->toFirst( );
	}
	connect( m_category, SIGNAL( currentIndexChanged( int ) ), SIGNAL( changed( ) ) );
	connect( m_profile, SIGNAL( currentIndexChanged( int ) ), SIGNAL( changed( ) ) );
	connect( m_status, SIGNAL( currentIndexChanged( int ) ), SIGNAL( changed( ) ) );
}

QString pa_db::properties_page_common::page_name( ) const
{
	return text::page_title_general( );
}

void pa_db::properties_page_common::apply( )
{
	m_objects_table->submitAll( );
}

