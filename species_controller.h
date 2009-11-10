#ifndef SPECIESCONTROLLER_H
#define SPECIESCONTROLLER_H

#include <QObject>
#include <QPointer>

#include "pa_species.h"

class QListWidget;
class QListWidgetItem;

namespace pa_db
{
  class pa_manager;
  class pa_species_list;
  class species_widget;
  
  class species_controller : public QObject
  {
    Q_OBJECT
    public:
      species_controller( pa_manager *manager );
      //species_controller();
      void setListWidget( species_widget *widget );
      //void render( const QVector< pa_species > &elements );
      void render( const pa_species_list &elements );
      void dropElement( const pa_species &el );
    private Q_SLOTS:
      void onSpeciesDoubleClicked( QListWidgetItem *item );
    private:
      QPointer< species_widget > mView;
      pa_manager *mManager;
  };
}

#endif //SPECIESCONTROLLER_H
