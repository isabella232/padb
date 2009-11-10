#pragma once
#include <QObject>
#include <QVector>
#include <QPair>
#include "pa_object.h"

namespace pa_db
{
	class pa_object_list : public QObject, protected QVector< pa_object >
	{
		Q_OBJECT
		public:
			pa_object_list( QObject *parent );
			virtual ~pa_object_list( );
			const pa_object *create_object( const QString &name );
			int find_object( int object_id ) const;
			int find_object( const pa_element &el ) const;
			int count( ) const;
			const pa_object& at( int i ) const;
			void delete_object( int object_id );
			void add_element( int object_id, const pa_element &el );
			void delete_element( int object_id, const pa_element &el );
			// for species
			void addSpecies( int object_id, const pa_species &el );
			void deleteSpecies( int object_id, const pa_species &el );
			
			bool is_element_free( const pa_element &el ) const;
			void load( int object_id = -1 );
			
			bool is_db_open( ) const;
			
		private:
			void load_elements( pa_object &obj );
			void loadSpecies( pa_object &obj ); // load species from DB
			bool m_db_open;
	};
}
