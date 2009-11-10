#pragma once
#include <QDialog>
#include <QVector>
class QTabWidget;
class QPushButton;

namespace pa_db
{
	class properties_page_base;
	class pa_manager;

	class properties_form : public QDialog
	{
		Q_OBJECT
	public:
		properties_form( int object_id, pa_manager *manager );
		bool is_changes_made( ) const;

	private Q_SLOTS:
		void on_change( );
		void on_apply( );
		void on_ok( );
		void on_cancel( );
		void on_report( );

	private:
		void add_page( properties_page_base *page );
		QString image_replace_text( );
		QString projection_replace_text( );

		pa_manager							*m_manager;
		QVector< properties_page_base* >	m_pages;
		const int							m_object_id;
		QTabWidget							*m_tabs;
		QPushButton							*m_apply;
		bool								m_changes_was_made;
	};
}
