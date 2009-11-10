#pragma once
#include <QObject>
#include <QPointer>
#include "../qgisplugin.h"

class QAction;
class QToolBar;
class QgisInterface;

namespace pa_db
{
	class pa_manager;
	class pa_db_plugin : public QObject, public QgisPlugin
	{
		Q_OBJECT;
	public:
		pa_db_plugin( QgisInterface *the_interface );

	public Q_SLOTS:
		virtual void initGui( );
		void unload( );

	private:
		QPointer< pa_manager >	m_manager;
		QgisInterface		*m_qgis_iface;
	};

}
