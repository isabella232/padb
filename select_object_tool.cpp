#include "select_object_tool.h"
#include "qgsmapcanvas.h"
#include "qgsvectorlayer.h"
#include "qgisinterface.h"
#include "qgsmaptopixel.h"
#include "qgscsexception.h"
#include "qgslogger.h"
#include "pa_manager.h"
#include <QMessageBox>
#include <QMouseEvent>
#include <QApplication>

pa_db::select_object_tool::select_object_tool( pa_manager* manager )
	: QgsMapTool( manager->qgis_iface( )->mapCanvas( ) )
	, m_manager( manager )
{
	mCursor = QCursor( Qt::PointingHandCursor );
}

bool pa_db::select_object_tool::isTransient( )
{
	return true;
}

void pa_db::select_object_tool::canvasReleaseEvent( QMouseEvent *e )
{
	if ( !m_manager->qgis_iface( )->mapCanvas( )->currentLayer( )
		|| dynamic_cast< QgsVectorLayer* >( m_manager->qgis_iface( )->mapCanvas( )->currentLayer( ) ) == NULL )
	{
		QMessageBox::warning( m_manager->qgis_iface( )->mapCanvas( ), QObject::tr( "No active layer" ),
			QObject::tr( "To select features, you must choose a "
			"vector layer by clicking on its name in the legend"
			) );
		return;
	}
	QgsVectorLayer* vlayer = dynamic_cast< QgsVectorLayer* >( m_manager->qgis_iface( )->mapCanvas( )->currentLayer( ) );
	select_by_pos( vlayer, e->pos( ) );
	const QgsFeatureIds& ids = vlayer->selectedFeaturesIds( );
	if ( ids.size( ) )
	{
		m_manager->try_show_object_properties( vlayer->name( ), *ids.begin( ) );
	}
}

void pa_db::select_object_tool::select_by_pos( QgsVectorLayer* vlayer, const QPoint &pos )
{
	//if the user simply clicked without dragging a rect
	//we will fabricate a small 1x1 pix rect and then continue
	//as if they had dragged a rect
	const int box_size = vlayer->geometryType( ) != QGis::Polygon ? 5 : 1;
	QRect select_rect;
	select_rect.setLeft( pos.x( ) - box_size );
	select_rect.setRight( pos.x( ) + box_size );
	select_rect.setTop( pos.y( ) - box_size );
	select_rect.setBottom( pos.y( ) + box_size );

	const QgsMapToPixel* transform = m_manager->qgis_iface( )->mapCanvas( )->getCoordinateTransform( );
	QgsPoint ll = transform->toMapCoordinates( select_rect.left( ), select_rect.bottom( ) );
	QgsPoint ur = transform->toMapCoordinates( select_rect.right( ), select_rect.top( ) );

	QgsRectangle search( ll.x( ), ll.y( ), ur.x( ), ur.y( ) );
	try
	{
		search = toLayerCoordinates( vlayer, search );
	}
	catch ( QgsCsException &cse )
	{
		Q_UNUSED( cse );
		// catch exception for 'invalid' rectangle and leave existing selection unchanged
		QgsLogger::warning( "Caught CRS exception " + QString( __FILE__ ) + ": " + QString::number( __LINE__ ) );
		QMessageBox::warning( m_manager->qgis_iface( )->mapCanvas( ), QObject::tr( "CRS Exception" ),
			QObject::tr( "Selection extends beyond layer's coordinate system." ) );
		return;
	}
	QApplication::setOverrideCursor( Qt::WaitCursor );
	vlayer->select( search, false );
	QApplication::restoreOverrideCursor( );
}



