#include <QListWidget>
#include <QMouseEvent>
#include <QBuffer>
#include <QUrl>

#include "resources.h"
#include "pa_manager.h"
#include "pa_species_list.h"
#include "species_widget.h"
#include "species_controller.h"

pa_db::species_controller::species_controller( pa_manager *manager )
  : QObject( manager )
  , mManager( manager )
{
}

// pa_db::species_controller::species_controller()
// {
// }

void pa_db::species_controller::setListWidget( species_widget *widget )
{
  mView = widget;
  connect( mView, SIGNAL( itemDoubleClicked( QListWidgetItem* ) ), SLOT( onSpeciesDoubleClicked( QListWidgetItem* ) ) );
  mView->setController( this );
}

void pa_db::species_controller::render( const pa_species_list &elements )
{
  if ( mView )
  {
    mView->clear( );
    for ( int i = 0; i < elements.count(); ++i )
    {
      QListWidgetItem *item = new QListWidgetItem;
      item->setData( Qt::UserRole, elements[ i ].toByteArray() );
      //QString text = elements[ nn ].feature_type( );
      item->setText( elements[ i ].caption() );
      // item->setText( elements[ i ].nameRu() );
      item->setIcon( QPixmap( pa_db::icon::oopt_species ) );
      mView->addItem( item );
    }
  }
}

void pa_db::species_controller::dropElement( const pa_species &el )
{
    mManager->speciesFromObject( el );
}

void pa_db::species_controller::onSpeciesDoubleClicked( QListWidgetItem *item )
{
  if ( item )
    mManager->showSpeciesProperties( pa_db::pa_species::fromByteArray( item->data( Qt::UserRole ).toByteArray() ) );
}
