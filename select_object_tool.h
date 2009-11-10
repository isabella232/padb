#pragma once

#include "qgsmaptool.h"
class QgisInterface;
class QgsVectorLayer;

namespace pa_db
{
	class pa_manager;
	class select_object_tool : public QgsMapTool
	{
		Q_OBJECT
	public:
		select_object_tool( pa_manager *manager );
		virtual bool isTransient( );

	protected:
		virtual void canvasReleaseEvent( QMouseEvent *e );

	private:
		void select_by_pos( QgsVectorLayer* vlayer, const QPoint &pos );

		pa_manager *m_manager;

	};
}
