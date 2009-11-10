#ifndef QGSSPECIESPROPERTIESDIALOG_H
#define QGSSPECIESPROPERTIESDIALOG_H

#include "ui_qgsspeciespropertiesbase.h"
#include <QDialog>

/*
 * dialog for displaying species properties
 */
namespace pa_db
{
	class pa_species;

 
class QgsSpeciesPropertiesDialog: public QDialog, private Ui::QgsSpeciesPropertiesBase
{
   Q_OBJECT
  public:
    QgsSpeciesPropertiesDialog( QWidget* parent, const pa_species &el );
};

}
#endif // QGSSPECIESPROPERTIESDIALOG_H
