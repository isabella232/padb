#include "properties_page_base.h"
#include "resources.h"

pa_db::properties_page_base::properties_page_base( int object_id, QWidget *parent )
	: QWidget( parent )
	, m_object_id( object_id )
{
	m_database = QSqlDatabase::database( pa_db::text::database_connection_name( ) );
}
