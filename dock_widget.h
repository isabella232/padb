#pragma once
#include <QDockWidget>
class QListWidget;
class QLabel;

namespace pa_db
{
	class objects_tree_widget;
	class free_elements_widget;
	class dock_widget : public QDockWidget
	{
		Q_OBJECT
	public:
		dock_widget( const QString& title, QWidget *parent );
		objects_tree_widget	*objects_tree( );
		free_elements_widget *free_elements_list( );
		void show_error_message( );

	private:
		void create_up_part( QWidget *parent );
		void create_bottom_part( QWidget *parent );

		objects_tree_widget		*m_objects_tree;
		free_elements_widget	*m_free_elements_list;
		QLabel *m_first_text;
		QLabel *m_second_text;
	};
}
