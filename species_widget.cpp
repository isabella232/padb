#include <QMouseEvent>
#include <QDrag>

#include "species_widget.h"
#include "species_controller.h"
#include "resources.h"

pa_db::species_widget::species_widget( QWidget *parent )
	:QListWidget( parent )
{
	setAcceptDrops( true );
}

void pa_db::species_widget::setController( species_controller *cntrl )
{
	mController = cntrl;
}

void pa_db::species_widget::mouseMoveEvent( QMouseEvent *event )
{
	if ( event->buttons() & Qt::LeftButton )
	{
		if ( currentItem() )
		{
			QDrag *drag = new QDrag( this );
			QMimeData *mimeData = new QMimeData;
			mimeData->setData( "application/x-qt-windows-mime;value=\"species_mime_data\"", currentItem()->data( Qt::UserRole ).toByteArray() );
			drag->setMimeData( mimeData );
			drag->start( Qt::CopyAction );
		}
	}
}

bool pa_db::species_widget::dropMimeData( int index, const QMimeData *data, Qt::DropAction action )
{
	// call function for species removal from object
	if ( data->hasFormat( "application/x-qt-windows-mime;value=\"species_mime_data\"" ) )
	{
		const pa_species el = pa_species::fromByteArray( data->data( "application/x-qt-windows-mime;value=\"species_mime_data\"" ) );
		mController->dropElement( el );
	}
	return true;
}

QStringList pa_db::species_widget::mimeTypes( ) const
{
	return QStringList() << "application/x-qt-windows-mime;value=\"species_mime_data\"";
}

Qt::DropActions pa_db::species_widget::supportedDropActions( ) const
{
	//return Qt::MoveAction;
	return Qt::CopyAction | Qt::MoveAction;
}
