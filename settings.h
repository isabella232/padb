#pragma once
#include <QString>

namespace pa_db
{
	namespace settings
	{
		// read/write locale specific settings
		QString qgisLocale();
		QString padbLocale();
		void setPadbLocale( const QString &locale );
		
		bool padbOverrideFlag();
		void setPadbOverrideFlag( bool flag );
		
		// read/write database path
		QString dbFileName();
		void setDbFileName( const QString &fileName );
		bool createEmptyDb( const QString &fileName );
		
		// read/write zoom settings
		int zoom();
		void setZoom( const QString &zoom );
		
		// read/write work layer settings
		QString ooptLayer();
		void setOoptLayer( const QString &layer );
		
		// helper functions for field mapping
		bool isDbFieldMapped( const QString &dbFieldName );
		void setDbFieldMapped( const QString &dbFieldName, bool mapped );
		QString nameDbFieldMapped( const QString &dbFieldName );
		void setNameDbFieldMapped( const QString &dbFieldName, const QString &mappedName );
		
		// read/write species specific settings
		int speciesVisibleFlag();
		void setSpeciesVisibleFlag( int state );
		
		int fieldSeparator();
		void setFieldSeparator( int sep );
		QString userSeparator();
		void setUserSeparator( const QString &sep );
		
		int namingStyle();
		void setNamingStyle( int style );
	}
}