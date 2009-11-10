#include "pa_species.h"

#include "qgsspeciespropertiesdialog.h"

pa_db::QgsSpeciesPropertiesDialog::QgsSpeciesPropertiesDialog( QWidget* parent, const pa_species &el ):QDialog()
{
  setupUi( this );
  connect( buttonBox, SIGNAL( accepted() ), this, SLOT( accept() ) );
  connect( buttonBox, SIGNAL( rejected() ), this, SLOT( reject() ) );

  // setting up controls
  leNameRu->setText( el.nameRu() );
  leNameEn->setText( el.nameEn() );
  leComment->setText( el.comment() );
  leStatus->setText( el.status() );
}

