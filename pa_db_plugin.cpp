#include <qgisinterface.h>
#include <qgisgui.h>

#include "pa_db_plugin.h"
#include "pa_manager.h"
#include "resources.h"

static const char * const sIdent = "PA_DB_PLUGIN_27195DAC_2F4C_4324_A9ED_657B785AE4B6";
static const QString sDescription = pa_db::text::description( );
static const QString sPluginVersion = pa_db::text::version( );
static const QgisPlugin::PLUGINTYPE sPluginType = QgisPlugin::UI;

pa_db::pa_db_plugin::pa_db_plugin( QgisInterface *the_interface )
	: QgisPlugin( pa_db::text::pa_db_plugin_name( ), sDescription, sPluginVersion, sPluginType )
	, m_qgis_iface( the_interface )
{
}

void pa_db::pa_db_plugin::initGui( )
{
	Q_ASSERT( !m_manager );
	m_manager = new pa_manager( m_qgis_iface, this );
}

void pa_db::pa_db_plugin::unload( )
{
	
	
	if ( m_manager )
		delete m_manager;
}

QGISEXTERN QgisPlugin *classFactory( QgisInterface *theQgisInterfacePointer )
{
	return new pa_db::pa_db_plugin( theQgisInterfacePointer );
}

QGISEXTERN QString name( )
{
	return pa_db::text::pa_db_plugin_name_in_plugin_manager( ) + "(" + sPluginVersion + ")";
}

QGISEXTERN QString description( )
{
  return sDescription;
}

QGISEXTERN int type( )
{
  return sPluginType;
}

QGISEXTERN QString version( )
{
  return sPluginVersion;
}

QGISEXTERN void unload( QgisPlugin * thePluginPointer )
{
  delete thePluginPointer;
}
