#pragma once
#include <QListWidget>

namespace pa_db
{
	class free_elements_controller;
	class free_elements_widget : public QListWidget
	{
		Q_OBJECT
	public:
		free_elements_widget( QWidget *parent );
		void set_controller( free_elements_controller *cntrl );

	protected:
		void mouseMoveEvent( QMouseEvent *event );
		virtual bool dropMimeData( int index, const QMimeData *data, Qt::DropAction action );
		virtual QStringList mimeTypes( ) const;    
		virtual Qt::DropActions supportedDropActions( ) const;
	private:
		free_elements_controller	*m_cntrl;
	};
}
