#pragma once
#include "properties_page_base.h"
class QSqlTableModel;
class QDataWidgetMapper;
class QFormLayout;
class QTextEdit;
class QSpinBox;
class QLineEdit;

namespace pa_db
{
	class properties_page_obj_table : public properties_page_base
	{
		Q_OBJECT
	public:
		properties_page_obj_table( int object_id, QWidget *parent );

	protected:
		QTextEdit *create_text_exit( const QString &label, int field_index );
		QSpinBox *create_spin_box( const QString &label, int field_index );
		QLineEdit *create_line_edit( const QString &label, int field_index, const QString &addon = QString( ) );

		QSqlTableModel				*m_objects_table;
		QDataWidgetMapper			*m_table_mapper;
		QFormLayout					*m_form_layout;
	};
}
