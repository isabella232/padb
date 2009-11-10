#include "pa_object_list.h"
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QCoreApplication>
#include <QFile>
#include "./resources.h"
#include <algorithm>
#include "settings.h"
//#include <QMessageBox>

namespace { namespace aux {
	class find_object
	{
		int m_id;
	public:
		find_object( int id )
			: m_id( id )
		{
		}
		bool operator ( )( const pa_db::pa_object &obj ) const
		{
			return m_id == obj.id( );
		}
	};
} }

pa_db::pa_object_list::pa_object_list( QObject *parent )
	: QObject( parent )
	, m_db_open( false )
{
	if ( QFile::exists( settings::dbFileName() ) )
	{
		QSqlDatabase db = QSqlDatabase::addDatabase( "QSQLITE", pa_db::text::database_connection_name( ) );
		db.setDatabaseName( settings::dbFileName( ) );
		if ( db.open( ) )
		{
			db.close( );
			load( );
			m_db_open = true;
		}
		else
		{
			qDebug( ) << "error open data db";
		}
	}
}

pa_db::pa_object_list::~pa_object_list( )
{
	QSqlDatabase::removeDatabase( pa_db::text::pa_db_plugin_name( ) );
}

void pa_db::pa_object_list::load( int object_id )
{
	if ( object_id == -1 )
		clear( );
	else
	{
		const int index = find_object( object_id );
		if ( index != -1 )
			remove( index );
	}
	QSqlDatabase db = QSqlDatabase::database( pa_db::text::database_connection_name( ) );
	if ( db.isOpen( ) )
	{
		QSqlQuery query( db );
		QString select( "SELECT id,name FROM objects" );
		if ( object_id != -1 )
			select.append( QString( " WHERE id=%1" ).arg( object_id ) );
		if ( query.exec( select ) )
		{
			while ( query.next( ) )
			{
				pa_object obj;
				obj.set_id( query.value( 0 ).toInt( ) );
				QVariant val = query.value( 1 );
				obj.set_name( query.value( 1 ).toString( ) );
				load_elements( obj );
				loadSpecies( obj );
				push_back( obj );
			}
		}
		else
			qDebug( ) << "query: \"" << select << "\" faild, reason: " << query.lastError( ).text( );
	}
	else
		qDebug( ) << "db not open";
}

void pa_db::pa_object_list::load_elements( pa_object &obj )
{
	QSqlDatabase db = QSqlDatabase::database( pa_db::text::database_connection_name( ) );
	if ( db.isOpen( ) )
	{
		QSqlQuery query( db );
		const QString prepare_text( "SELECT layer_id,feature_id,feature_type FROM links WHERE object_id=:object_id" );
		if ( query.prepare( prepare_text ) )
		{
			query.bindValue( ":object_id", obj.id( ) );
			if ( query.exec( ) )
			{
				QVector< pa_element > obj_els;
					while ( query.next( ) )
				{
					pa_element el;
					el.set_layer_id( query.value( 0 ).toString( ) );
					el.set_feature_id( query.value( 1 ).toInt( ) );
					el.set_feature_type( query.value( 2 ).toString( ) );
					obj_els.push_back( el );
				}
				obj.set_elements( obj_els );
			}
			else
				qDebug( ) << "query faild, reason: " << query.lastError( ).text( );
		}
		else
			qDebug( ) << "prepare faild, reason: " << query.lastError( ).text( );
	}
	else
		qDebug( ) << "db not open";
}

// load species linked with specified object
void pa_db::pa_object_list::loadSpecies( pa_object &obj )
{
	QString nameRu, nameEn;
	
	QSqlDatabase db = QSqlDatabase::database( pa_db::text::database_connection_name() );
	if ( db.isOpen() )
	{
		QSqlQuery query( db );
		const QString qryText( "SELECT * FROM vlinks WHERE objectId=:objId" );
		if ( query.prepare( qryText ) )
		{
			//QMessageBox::warning( 0, "DEBUG", QString("Object id %1").arg( obj.id() ) );
			query.bindValue( ":objId", obj.id() );
			if ( query.exec() )
			{
				QVector< pa_species > obj_spec;
				while ( query.next() )
				{
					pa_species sp;
					sp.setId( query.value( 1 ).toInt() );
					sp.setParentId( obj.id() );
					
					nameRu = query.value( 2 ).toString();
					nameEn = query.value( 3 ).toString();
					
					switch( pa_db::settings::namingStyle() )
					{
						case 0:
							sp.setCaption( nameRu );
							break;
						case 1:
							sp.setCaption( nameEn );
							break;
						case 2:
							sp.setCaption( QString( "%1 (%2)" ).arg( nameRu ).arg( nameEn ) );
							break;
					}
					
					// sp.setNameRu( query.value( 2 ).toString() );
					// sp.setNameEn( query.value( 3 ).toString() );
					sp.setNameRu( nameRu );
					sp.setNameEn( nameEn );
					sp.setComment( query.value( 4 ).toString() );
					sp.setStatus( query.value( 5 ).toString() );
					obj_spec.push_back( sp );
				}
				obj.setSpecies( obj_spec );
			}
			else
			{
				qDebug() << "Query failed. Reason: " << query.lastError().text();
				//QMessageBox::warning( 0, "DEBUG", "Query " +query.lastError().text() );
			}
		}
		else
		{
			qDebug() << "Query preparing failed. Reason: " << query.lastError().text();
			//QMessageBox::warning( 0, "DEBUG", "Prepare " +query.lastError().text() );
		}
	}
	else
	{
		qDebug() << "DB is not open";
	}
}

const pa_db::pa_object *pa_db::pa_object_list::create_object( const QString &name )
{
	QSqlDatabase db = QSqlDatabase::database( pa_db::text::database_connection_name( ) );
	if ( db.isOpen( ) )
	{
		QSqlQuery query( db );
		const QString prepare_text( "INSERT INTO objects (name) VALUES (:name)" );
		if ( query.prepare( prepare_text ) )
		{
			query.bindValue( ":name", name.left( pa_db::def::text_max_len ) );
			if ( query.exec( ) )
			{
				int last_row_id = query.lastInsertId( ).toInt( );
				pa_object obj;
				obj.set_id( last_row_id );
				obj.set_name( name );
				push_back( obj );
				return &at( count( ) - 1);
			}
			else
				qDebug( ) << "query faild, reason: " << query.lastError( ).text( );
		}
		else
			qDebug( ) << "prepare faild, reason: " << query.lastError( ).text( );
	}
	else
		qDebug( ) << "db not open";
	return 0;
}

int pa_db::pa_object_list::find_object( int object_id ) const
{
	QVector< pa_object >::const_iterator it = std::find_if( constBegin( ), constEnd( ), aux::find_object( object_id ) );
	return it == constEnd( ) ? -1 : it - constBegin( );
}

int pa_db::pa_object_list::find_object( const pa_element &el ) const
{
	for ( QVector< pa_object >::const_iterator it_obj = constBegin( );
		it_obj != constEnd( ); ++it_obj )
	{
		QVector< pa_element > els = it_obj->elements( );
		if ( std::find_if( els.constBegin( ), els.constEnd( ), find_element( el.layer_id( ), el.feature_id( ) ) ) != els.constEnd( ) )
			return it_obj - constBegin( );
	}
	return -1;
}

int pa_db::pa_object_list::count( ) const
{
	return QVector< pa_object >::count( );
}

const pa_db::pa_object& pa_db::pa_object_list::at ( int i ) const
{
	return QVector< pa_object >::at( i );
}

void pa_db::pa_object_list::delete_object( int object_id )
{
	const int index = find_object( object_id );
	if ( index != -1 )
	{
		QSqlDatabase db = QSqlDatabase::database( pa_db::text::database_connection_name( ) );
		if ( db.isOpen( ) )
		{
			{
				QSqlQuery query( db );
				if ( !query.exec( QString( "DELETE FROM objects WHERE id=%1" ).arg( object_id ) ) )
					qDebug( ) << "query faild, reason: " << query.lastError( ).text( );
			}
			{
				QSqlQuery query( db );
				if ( !query.exec( QString( "DELETE FROM links WHERE object_id=%1" ).arg( object_id ) ) )
					qDebug( ) << "query faild, reason: " << query.lastError( ).text( );
			}
			{
				QSqlQuery query( db );
				if ( !query.exec( QString( "DELETE FROM link_species WHERE object_id=%1" ).arg( object_id ) ) )
					qDebug( ) << "Query failed. Reason: " << query.lastError().text();
			}
		}
		else
			qDebug( ) << "db not open";
	}
	remove( index );
}

void pa_db::pa_object_list::add_element( int object_id, const pa_element &el )
{
	const int index = find_object( object_id );
	if ( index != -1 )
	{
		QSqlDatabase db = QSqlDatabase::database( pa_db::text::database_connection_name( ) );
		if ( db.isOpen( ) )
		{
			QSqlQuery query( db );
			const QString prepare_text( "INSERT INTO links (object_id,layer_id,feature_id,feature_type)"
				" VALUES (:object_id,:layer_id,:feature_id,:feature_type)" );
			if ( query.prepare( prepare_text ) )
			{
				query.bindValue( ":object_id", object_id );
				query.bindValue( ":layer_id", el.layer_id( ).left( pa_db::def::text_max_len ) );
				query.bindValue( ":feature_id", el.feature_id( ) );
				query.bindValue( ":feature_type", el.feature_type( ) );
				if ( query.exec( ) )
				{
					QVector< pa_element > els = this->operator[]( index ).elements( );
					els.push_back( el );
					this->operator[]( index ).set_elements( els );
				}
				else
					qDebug( ) << "query faild, reason: " << query.lastError( ).text( );
			}
			else
				qDebug( ) << "prepare faild, reason: " << query.lastError( ).text( );
		}
		else
			qDebug( ) << "db not open";
	}
}

bool pa_db::pa_object_list::is_element_free( const pa_element &el ) const
{
	for ( int nn = 0; nn < count( ); ++nn )
	{
		QVector< pa_element > els = at( nn ).elements( );
		QVector< pa_element >::const_iterator it_el =
			std::find_if( els.constBegin( ), els.constEnd( ), find_element( el.layer_id( ), el.feature_id( ) ) );
		if ( it_el != els.constEnd( ) )
			return false;
	}
	return true;
}

void pa_db::pa_object_list::delete_element( int object_id, const pa_element &el )
{
	QSqlDatabase db = QSqlDatabase::database( pa_db::text::database_connection_name( ) );
	if ( db.isOpen( ) )
	{
		QSqlQuery query( db );
		const QString prepare_text( "DELETE FROM links WHERE "
			"object_id=:object_id AND layer_id=:layer_id AND feature_id=:feature_id" );
		if ( query.prepare( prepare_text ) )
		{
			query.bindValue( ":object_id", object_id );
			query.bindValue( ":layer_id", el.layer_id( ) );
			query.bindValue( ":feature_id", el.feature_id( ) );
			if ( !query.exec( ) )
				qDebug( ) << "query faild, reason: " << query.lastError( ).text( );
		}
		else
			qDebug( ) << "prepare faild, reason: " << query.lastError( ).text( );
	}
	else
		qDebug( ) << "db not open";
	const int obj_index = find_object( object_id );
	if ( obj_index != -1 )
	{
		QVector< pa_element > els = at( obj_index ).elements( );
		els.erase( std::remove_if( els.begin( ), els.end( ), find_element( el.layer_id( ), el.feature_id( ) ) ), els.end( ) );
		(*this)[ obj_index ].set_elements( els );
	}
}

// for species
void pa_db::pa_object_list::addSpecies( int object_id, const pa_species &el )
{
	const int index = find_object( object_id );
	if ( index != -1 )
	{
		QSqlDatabase db = QSqlDatabase::database( pa_db::text::database_connection_name() );
		if ( db.isOpen() )
		{
			QSqlQuery query( db );
			const QString qryText( "INSERT INTO link_species (objectId, speciesId) VALUES (:objId,:specId)" );
			if ( query.prepare( qryText ) )
			{
				query.bindValue( ":objId", object_id );
				query.bindValue( ":specId", el.id() );
				if ( query.exec() )
				{
					//QMessageBox::warning( 0,"DEBUG", QString( "in id %1" ).arg( object_id ) );
					QVector< pa_species > els = this->operator[]( index ).species();
					els.push_back( el );
					this->operator[]( index ).setSpecies( els );
				}
				else
				{
					qDebug() << "Query failed. Reason: " << query.lastError().text();
					//QMessageBox::warning( 0, "DEBUG", "Query " +query.lastError().text() );
				}
			}
			else
			{
				qDebug() << "Query preparing failed. Reason: " << query.lastError().text();
				//QMessageBox::warning( 0, "DEBUG", "Prepare " +query.lastError().text() );
			}
		}
		else
		{
			qDebug() << "DB is not open";
			//QMessageBox::warning( 0, "DEBUG", "db not open" );
		}
	}
}

void pa_db::pa_object_list::deleteSpecies( int object_id, const pa_species &el )
{
	QSqlDatabase db = QSqlDatabase::database( pa_db::text::database_connection_name() );
	if ( db.isOpen() )
	{
		QSqlQuery query( db );
		const QString qryText( "DELETE FROM link_species WHERE objectId=:objId AND speciesId=:specId" );
		if ( query.prepare( qryText ) )
		{
			query.bindValue( ":objId", object_id );
			query.bindValue( ":specId", el.id() );
			if ( !query.exec() )
			{
				qDebug() << "Query failed. Reason: " << query.lastError().text();
			}
		}
		else
		{
			qDebug() << "Query preparing failed. Reason: " << query.lastError().text();
		}
	}
	else
	{
		qDebug() << "DB is not open";
	}
	
	const int obj_index = find_object( object_id );
	if ( obj_index != -1 )
	{
		QVector< pa_species > els = at( obj_index ).species();
		// search in vector for index of deleted species
		int idxRemove = -1;
		for (int i = 0; i < els.size(); ++i)
		{
			if( els.at(i).id() == el.id() )
			{
				idxRemove = i;
			}
		}
		
		// now remove species
		els.remove( idxRemove );
		//els.erase( std::remove_if( els.begin(), els.end(), find_element( el.layer_id(), el.feature_id() ) ), els.end() );
		(*this)[ obj_index ].setSpecies( els );
	}
}

bool pa_db::pa_object_list::is_db_open( ) const
{
	return m_db_open;
}
