#pragma once
#include <QDockWidget>
class QListWidget;
class QLabel;

namespace pa_db
{
	class objects_tree_widget;
	class free_elements_widget;
	class species_widget;
	class dock_widget : public QDockWidget
	{
		Q_OBJECT
	public:
		dock_widget( const QString& title, QWidget *parent );
		objects_tree_widget *objects_tree( );
		free_elements_widget *free_elements_list( );
		species_widget *speciesList(); // accessor for species list
		void show_error_message( );
		
		void hideSpecies();
		void showSpecies();

	private:
		void create_up_part( QWidget *parent );
		void create_mid_part( QWidget *parent ); // область для списка видов
		void create_bottom_part( QWidget *parent );

		objects_tree_widget		*m_objects_tree;
		free_elements_widget	*m_free_elements_list;
		species_widget	*mSpeciesList; // список видов
		QLabel *m_first_text;
		QLabel *m_second_text;
		QLabel *lblSpeciesList; // подпись списка видов
	};
}
