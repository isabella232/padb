#include "settings.h"
#include <QSettings>
#include <QDesktopServices>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include "resources.h"

namespace { namespace aux {
	const QString key_db_file_name( "key_db_file_name" );
	const QString key_min_zoom( "key_min_zoom" );
	const QString key_export_layer( "export_layer" );
	const QString key_group_is_mapping( "group_is_mapping" );
	const QString key_group_name_mapping( "group_name_mapping" );
} }

QString pa_db::settings::db_file_name( )
{
	return QSettings( ).value( aux::key_db_file_name,
		QDesktopServices::storageLocation( QDesktopServices::DataLocation ) + "/" + text::db_file_name( ) ).toString( );
}

void pa_db::settings::set_db_file_name( const QString &file_name )
{
	QSettings( ).setValue( aux::key_db_file_name, file_name );
}

bool pa_db::settings::create_empy_db_file( const QString &file_name )
{
	QFile fl_in( ":/pa_db/empty_db.db3" );
	if ( fl_in.open( QIODevice::ReadOnly ) )
	{
		QFileInfo info( file_name );
		if ( !QDir( ).exists( info.absolutePath( ) ) )
			QDir( ).mkpath( info.absolutePath( ) );
		QFile fl_out( file_name );
		if ( fl_out.open( QIODevice::WriteOnly ) )
		{
			fl_out.write( fl_in.readAll( ) );
			return true;
		}
	}
	return false;
}

QString pa_db::settings::min_zoom( )
{
	int result = QSettings( ).value( aux::key_min_zoom, "25000" ).toInt( );
	if ( result < 5000 || result > 1000000)
		result = 25000;
	return QString::number( result );
}

void pa_db::settings::set_min_zoom( const QString &value )
{
	QSettings( ).setValue( aux::key_min_zoom, value );
}

bool pa_db::settings::is_db_field_mapped( const QString &db_field_name )
{
	if ( !db_field_name.isEmpty( ) )
	{
		QSettings set;
		set.beginGroup( aux::key_group_is_mapping );
		return set.value( db_field_name, true ).toBool( );
	}
	return false;
}

void pa_db::settings::set_db_field_mapped( const QString &db_field_name, bool mapped )
{
	if ( !db_field_name.isEmpty( ) )
	{
		QSettings set;
		set.beginGroup( aux::key_group_is_mapping );
		set.setValue( db_field_name, mapped );
	}
}

QString pa_db::settings::name_db_field_mapped( const QString &db_field_name )
{
	if ( !db_field_name.isEmpty( ) )
	{
		QSettings set;
		set.beginGroup( aux::key_group_name_mapping );
		return set.value( db_field_name, "" ).toString( );
	}
	return "";
}
void pa_db::settings::set_name_db_field_mapped( const QString &db_field_name, const QString &mapped_name )
{
	if ( !db_field_name.isEmpty( ) && !mapped_name.isEmpty( ) )
	{
		QSettings set;
		set.beginGroup( aux::key_group_name_mapping );
		set.setValue( db_field_name, mapped_name );
	}
}

QString pa_db::settings::name_export_layer( )
{
	return QSettings( ).value( aux::key_export_layer, "" ).toString( );
}

void pa_db::settings::set_name_export_layer( const QString &name )
{
	QSettings( ).setValue( aux::key_export_layer, name );
}
