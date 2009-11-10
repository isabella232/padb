#pragma once
#include "properties_page_base.h"
class QSqlTableModel;
class QDataWidgetMapper;
class QListView;
class QLabel;
class QTextEdit;
class QLineEdit;

namespace pa_db
{
	class properties_page_docs : public properties_page_base
	{
		Q_OBJECT
	public:
		properties_page_docs( int object_id, QWidget *parent );
		virtual QString page_name( ) const;
		virtual void apply( );

	private Q_SLOTS:
		void on_open_doc( );
		void on_add_doc( );
		void on_remove_doc( );
		void on_change_select_doc( );

	private:
		void setup_ui( );
		int list_view_column( );
		void select_row( int row );

		QSqlTableModel *m_docs_table;
		QDataWidgetMapper *m_table_mapper;
		QListView *m_docs_list;

		QLineEdit *m_date_label;
		QLineEdit *m_url_label;
		QLabel *m_status_label;
		QTextEdit *m_description;
	};
}