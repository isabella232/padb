#include "free_elements_widget.h"
#include <QMouseEvent>
#include "resources.h"
#include "free_elements_controller.h"
#include <QDrag>

pa_db::free_elements_widget::free_elements_widget( QWidget *parent )
	: QListWidget( parent )
{
	setAcceptDrops( true );
}

void pa_db::free_elements_widget::mouseMoveEvent( QMouseEvent *event )
{
	if ( event->buttons( ) & Qt::LeftButton )
	{
		if ( currentItem( ) )
		{
			QDrag *drag = new QDrag( this );
			QMimeData *mimeData = new QMimeData;
			mimeData->setData( pa_db::text::free_element_mime_data( ), currentItem( )->data( Qt::UserRole ).toByteArray( ) );
			drag->setMimeData( mimeData );
			drag->start( Qt::MoveAction );
		}
	}
}

bool pa_db::free_elements_widget::dropMimeData( int index, const QMimeData *data, Qt::DropAction action )
{
	if ( data->hasFormat( pa_db::text::link_element_mime_data( ) ) )
	{
		const pa_element el = pa_element::from_byte_array( data->data( pa_db::text::link_element_mime_data( ) ) );
		m_cntrl->drop_element( el );
	}
	return true;
}

QStringList pa_db::free_elements_widget::mimeTypes( ) const
{
	return QStringList( ) << pa_db::text::link_element_mime_data( );
}

Qt::DropActions pa_db::free_elements_widget::supportedDropActions( ) const
{
	return Qt::CopyAction | Qt::MoveAction;
}

void pa_db::free_elements_widget::set_controller( free_elements_controller *cntrl )
{
	m_cntrl = cntrl;
}
