#ifndef PASPECIESLIST_H
#define PASPECIESLIST_H

#include <QObject>
#include "pa_species.h"

namespace pa_db
{
  class pa_manager;
  
  class pa_species_list : public QObject, public QVector< pa_species >
  {
    public:
      pa_species_list( pa_manager *manager );
      //pa_species_list();
      void reload();
    private:
      void loadSpecies();
      pa_manager *mManager;
  };
}

#endif // PASPECIESLIST_H
