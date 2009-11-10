#include "sql_relation_combo.h"
#include <QDebug>
#include <QSqlQuery>

pa_db::sql_relation_combo::sql_relation_combo( QWidget *parent, QSqlDatabase& database, const QString &table_name )
	: QComboBox( parent )
{
	if ( database.isOpen( ) )
	{
		QSqlQuery query( database );
		if ( query.exec( "SELECT id,name FROM " + table_name ) )
		{
			while ( query.next( ) )
				addItem( query.value( 1 ).toString( ), query.value( 0 ).toInt( ) );
		}
		else
			qDebug( ) << "exec query failed";
	}
	else
		qDebug( ) << "db is closed";
}

int pa_db::sql_relation_combo::id( ) const
{
	return currentIndex( ) != -1 ? itemData( currentIndex( ) ).toInt( ) : 0;
}

void pa_db::sql_relation_combo::set_id( int i )
{
	if ( count( ) )
		setCurrentIndex( findData( i ) != -1 ? findData( i ) : 0 );
}

