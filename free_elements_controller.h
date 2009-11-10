#pragma once
#include <QObject>
#include <QPointer>
class QListWidget;
class QListWidgetItem;
#include "pa_element.h"

namespace pa_db
{
	class map_pa_elements_list;
	class pa_manager;
	class free_elements_widget;
	class free_elements_controller : public QObject
	{
		Q_OBJECT
	public:
		free_elements_controller( pa_manager *manager );
		void set_list_widget( free_elements_widget *widget );
		void render( const QVector< pa_element > &elements );
		void drop_element( const pa_element &el );

	private Q_SLOTS:
		void on_element_double_click( QListWidgetItem *item );

	private:
		QPointer< free_elements_widget >	m_view;
		pa_manager							*m_manager;
	};
}
