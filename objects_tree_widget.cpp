#include "objects_tree_widget.h"
#include "resources.h"
#include "objects_tree_controller.h"
#include <QMouseEvent>
#include <QDrag>
#include <QKeyEvent>

pa_db::objects_tree_widget::objects_tree_widget( QWidget *parent )
	: QTreeWidget( parent )
	, m_cntrl( 0 )
{
	setAcceptDrops( true );
}

void pa_db::objects_tree_widget::set_controller( pa_db::objects_tree_controller *cntrl )
{
	m_cntrl = cntrl;
}

Qt::DropActions pa_db::objects_tree_widget::supportedDropActions( ) const
{
	return Qt::CopyAction | Qt::MoveAction;
}

QStringList pa_db::objects_tree_widget::mimeTypes( ) const
{
	return QStringList( ) << pa_db::text::free_element_mime_data( ) << pa_db::text::link_element_mime_data( );
}

bool pa_db::objects_tree_widget::dropMimeData( QTreeWidgetItem *parent, int index, const QMimeData *data, Qt::DropAction action )
{
	return m_cntrl->drop_element( parent, data );;
}

void pa_db::objects_tree_widget::mouseMoveEvent( QMouseEvent *event )
{
	if ( event->buttons( ) & Qt::LeftButton )
	{
		if ( currentItem( ) && currentItem( )->parent( ) )
		{
			QDrag *drag = new QDrag( this );
			QMimeData *mimeData = new QMimeData;
			mimeData->setData( pa_db::text::link_element_mime_data( ), currentItem( )->data( 0, Qt::UserRole ).toByteArray( ) );
			drag->setMimeData( mimeData );
			drag->start( Qt::MoveAction );
		}
	}
}

void pa_db::objects_tree_widget::keyPressEvent( QKeyEvent *event )
{
	if  ( currentItem( ) )
		m_cntrl->on_key_clicked( event->key( ) );
}

void pa_db::objects_tree_widget::mouseDoubleClickEvent( QMouseEvent *event )
{
	m_cntrl->on_double_click( );
}
