#pragma once
class QTextEdit;
class QFormLayout;
class QString;
class QDataWidgetMapper;
class QSpinBox;
class QLineEdit;

namespace pa_db
{
	namespace utils
	{
		QTextEdit *create_text_exit( QFormLayout* form_layout, const QString &label,
			QDataWidgetMapper *data_mapper, int field_index );
		QSpinBox *create_spin_box( QFormLayout* form_layout, const QString &label,
			QDataWidgetMapper *data_mapper, int field_index );
		QLineEdit *create_line_edit( QFormLayout* form_layout, const QString &label,
			QDataWidgetMapper *data_mapper, int field_index, const QString &addon );
	}
}