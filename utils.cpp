#include "utils.h"
#include <QTextEdit>
#include <QFormLayout>
#include <QDataWidgetMapper>
#include <QSpinBox>
#include <QLineEdit>
#include <QLabel>

namespace { namespace aux {
	QLayout *create_line( QWidget *wid, const QString &text )
	{
		QHBoxLayout *result = new QHBoxLayout;
		result->addWidget( wid );
		result->addWidget( new QLabel( text ) );
		result->addStretch( );
		return result;
	}
} }

QTextEdit* pa_db::utils::create_text_exit( QFormLayout* form_layout, const QString &label,
	QDataWidgetMapper *data_mapper, int field_index )
{
	Q_ASSERT( field_index != -1 );
	QTextEdit *result = new QTextEdit;
	form_layout->addRow( label, result );
	result->setAcceptRichText( false );
	if ( data_mapper )
		data_mapper->addMapping( result, field_index, "plainText" );
	return result;
}

QSpinBox* pa_db::utils::create_spin_box( QFormLayout* form_layout, const QString &label,
						  QDataWidgetMapper *data_mapper, int field_index )
{
	Q_ASSERT( field_index != -1 );
	QSpinBox *result = new QSpinBox;
	form_layout->addRow( label, result );
	if ( data_mapper )
		data_mapper->addMapping( result, field_index );
	return result;
}

QLineEdit *pa_db::utils::create_line_edit( QFormLayout* form_layout, const QString &label,
							QDataWidgetMapper *data_mapper, int field_index, const QString &addon )
{
	Q_ASSERT( field_index != -1 );
	QLineEdit *result = new QLineEdit;
	if ( addon.isEmpty( ) )
		form_layout->addRow( label, result );
	else
		form_layout->addRow( label, aux::create_line( result, addon ) );
	if ( data_mapper )
		data_mapper->addMapping( result, field_index );
	return result;
}
