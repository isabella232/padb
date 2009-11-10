#pragma once
#include <QObject>
#include "pa_element.h"
#include <QVariant>
#include <QMap>

namespace pa_db
{
	class pa_manager;
	class map_pa_elements_list : public QObject, protected QVector< pa_element >
	{
	public:
		map_pa_elements_list( pa_manager *manager );
		void reload( );
		QVector< pa_element > free_elements( ) const;
		void select( const pa_element &el, bool zoom = false );
		void select( const QVector< pa_element > &els, bool zoom = false );
		bool is_element_valid( const pa_element &el ) const;

		void fill_elements_data( const QVector< pa_element > &els, const QMap< QString, QVariant > &data );

	private:
		void load_free_elements( );

		pa_manager *m_manager;
	};
}
