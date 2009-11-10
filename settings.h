#pragma once
#include <QString>

namespace pa_db
{
	namespace settings
	{
		QString db_file_name( );
		void set_db_file_name( const QString &file_name );
		bool create_empy_db_file( const QString &file_name );

		QString min_zoom( );
		void set_min_zoom( const QString &value );

		bool is_db_field_mapped( const QString &db_field_name );
		void set_db_field_mapped( const QString &db_field_name, bool mapped );
		QString name_db_field_mapped( const QString &db_field_name );
		void set_name_db_field_mapped( const QString &db_field_name, const QString &mapped_name );
		QString name_export_layer( );
		void set_name_export_layer( const QString &name );
	}
}