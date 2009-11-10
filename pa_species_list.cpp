#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>
#include <QVariant>

#include "pa_species_list.h"
#include "pa_manager.h"
#include "resources.h"
#include "settings.h"

pa_db::pa_species_list::pa_species_list( pa_db::pa_manager *manager )
  : QObject( manager )
  , mManager( manager )
{
}

// pa_db::pa_species_list::pa_species_list()
//   :QObject( parent )
// {
// }

void pa_db::pa_species_list::reload()
{
  loadSpecies();
}

void pa_db::pa_species_list::loadSpecies()
{
  QString nameRu, nameEn;
  
  clear();
  QSqlDatabase db = QSqlDatabase::database( pa_db::text::database_connection_name() );
  if ( db.isOpen() )
  {
    QSqlQuery query( db );
    if ( query.exec( "SELECT * FROM species" ) )
    {
      QSqlRecord rec = query.record();
        while ( query.next() )
        {
          pa_species spec;
          
          spec.setId( query.value( rec.indexOf( "id" ) ).toInt() );
          spec.setParentId( -1 );
          
          nameRu = query.value( rec.indexOf( "nameRu" ) ).toString();
          nameEn = query.value( rec.indexOf( "nameEn" ) ).toString();
          
          switch ( pa_db::settings::namingStyle() )
          {
            case 0: // nameRu
              spec.setCaption( nameRu );
              break;
            case 1: // nameEn
              spec.setCaption( nameEn );
              break;
            case 2: // nameBoth
              spec.setCaption( QString( "%1 (%2)" ).arg( nameRu ).arg( nameEn ) );
              break;
          }
          
          // spec.setNameRu( query.value( rec.indexOf( "nameRu" ) ).toString() );
          // spec.setNameEn( query.value( rec.indexOf( "nameEn" ) ).toString() );
          spec.setNameRu( nameRu );
          spec.setNameEn( nameEn );
          spec.setComment( query.value( rec.indexOf( "comment" ) ).toString() );
          spec.setStatus( query.value( rec.indexOf( "protectstatus" ) ).toString() );
          
          push_back( spec );
        }
    }
    else
    {
      qDebug() << "Query failed: " << query.lastError().text();
    }
  }
  else
  {
    qDebug() << "DB is not open";
  }
}
