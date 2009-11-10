#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QSettings>
#include <QDesktopServices>

#include "settings.h"
#include "resources.h"

namespace
{
  namespace aux
  {
    const QString keyDbFileName( "padb/db_file_name" );
    const QString keyZoom( "padb/min_zoom" );
    const QString keyPadbLocale( "padb/user_locale" );
    const QString keyPadbOverrideFlag( "padb/override_locale" );
    const QString keyQgisLocale( "locale/userLocale" );
    const QString keyLayer( "padb/export_layer" );
    const QString groupIsMapping( "padb/group_is_mapping" );
    const QString groupNameMapping( "padb/group_name_mapping" );
  }
}

QString pa_db::settings::qgisLocale()
{
	return QSettings().value( aux::keyQgisLocale, "en_US" ).toString();
}

QString pa_db::settings::padbLocale()
{
	bool padbOverrideFlag = QSettings().value( aux::keyPadbOverrideFlag, false ).toBool();
	if ( padbOverrideFlag ) // override QGIS locale
	{
		return QSettings().value( aux::keyPadbLocale, "en_US" ).toString();
	}
	else // use QGIS locale
	{
		return QSettings().value( aux::keyQgisLocale, "en_US" ).toString();
	}
}

void pa_db::settings::setPadbLocale( const QString &locale )
{
	QSettings().setValue( aux::keyPadbLocale, locale );
}

bool pa_db::settings::padbOverrideFlag()
{
	return QSettings().value( aux::keyPadbOverrideFlag, false ).toBool();
}

void pa_db::settings::setPadbOverrideFlag( bool flag )
{
	QSettings().setValue( aux::keyPadbOverrideFlag, flag );
}

QString pa_db::settings::dbFileName()
{
	return QSettings().value( aux::keyDbFileName,
		QDesktopServices::storageLocation( QDesktopServices::DataLocation ) + "/" + text::db_file_name() ).toString();
}

void pa_db::settings::setDbFileName( const QString &fileName )
{
	QSettings().setValue( aux::keyDbFileName, fileName );
}

bool pa_db::settings::createEmptyDb( const QString &fileName )
{
	QFile fileIn( ":/pa_db/empty_db.db3" );
	if ( fileIn.open( QIODevice::ReadOnly ) )
	{
		QFileInfo info( fileName );
		if ( !QDir().exists( info.absolutePath() ) )
			QDir().mkpath( info.absolutePath() );
		QFile fileOut( fileName );
		if ( fileOut.open( QIODevice::WriteOnly ) )
		{
			fileOut.write( fileIn.readAll() );
			return true;
		}
	}
	return false;
}

int pa_db::settings::zoom()
{
	int result = QSettings().value( aux::keyZoom, "25000" ).toInt();
	if ( result < 5000 || result > 1000000)
		result = 25000;
	//return QString::number( result );
	return result;
}

void pa_db::settings::setZoom( const QString &zoom )
{
	QSettings().setValue( aux::keyZoom, zoom );
}

QString pa_db::settings::ooptLayer()
{
	return QSettings().value( aux::keyLayer, "" ).toString();
}

void pa_db::settings::setOoptLayer( const QString &layer )
{
	QSettings().setValue( aux::keyLayer, layer );
}

bool pa_db::settings::isDbFieldMapped( const QString &dbFieldName )
{
	if ( !dbFieldName.isEmpty() )
	{
		QSettings set;
		set.beginGroup( aux::groupIsMapping );
		return set.value( dbFieldName, true ).toBool();
	}
	return false;
}

void pa_db::settings::setDbFieldMapped( const QString &dbFieldName, bool mapped )
{
	if ( !dbFieldName.isEmpty() )
	{
		QSettings set;
		set.beginGroup( aux::groupIsMapping );
		set.setValue( dbFieldName, mapped );
	}
}

QString pa_db::settings::nameDbFieldMapped( const QString &dbFieldName )
{
	if ( !dbFieldName.isEmpty() )
	{
		QSettings set;
		set.beginGroup( aux::groupNameMapping );
		return set.value( dbFieldName, "" ).toString();
	}
	return "";
}
void pa_db::settings::setNameDbFieldMapped( const QString &dbFieldName, const QString &mappedName )
{
	if ( !dbFieldName.isEmpty() && !mappedName.isEmpty() )
	{
		QSettings set;
		set.beginGroup( aux::groupNameMapping );
		set.setValue( dbFieldName, mappedName );
	}
}

int pa_db::settings::speciesVisibleFlag()
{
	return QSettings().value( "padb/speciesVisible", 0 ).toInt();
}

void pa_db::settings::setSpeciesVisibleFlag( int state )
{
	QSettings().setValue( "padb/speciesVisible", state );
}

int pa_db::settings::fieldSeparator()
{
	return QSettings().value( "padb/fieldSeparator", 0 ).toInt();
}

void pa_db::settings::setFieldSeparator( int sep )
{
	QSettings().setValue( "padb/fieldSeparator", sep );
}

QString pa_db::settings::userSeparator()
{
	return QSettings().value( "padb/customSeparator", "" ).toString();
}

void pa_db::settings::setUserSeparator( const QString &sep )
{
	QSettings().setValue( "padb/customSeparator", sep );
}

int pa_db::settings::namingStyle()
{
	return QSettings().value( "padb/namingStyle", "0" ).toInt();
}

void pa_db::settings::setNamingStyle( int style )
{
	QSettings().setValue( "padb/namingStyle", style );
}
