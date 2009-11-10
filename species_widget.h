#ifndef SPECIESWIDGET_H
#define SPECIESWIDGET_H

#include <QListWidget>

namespace pa_db
{
	class species_controller;
	
	class species_widget : public QListWidget
	{
		Q_OBJECT
	public:
		species_widget( QWidget *parent );
		void setController( species_controller *cntrl );
	protected:
		void mouseMoveEvent( QMouseEvent *event );
		virtual bool dropMimeData( int index, const QMimeData *data, Qt::DropAction action );
		virtual QStringList mimeTypes( ) const;
		virtual Qt::DropActions supportedDropActions( ) const;
	private:
		species_controller *mController;
	};
}

#endif // SPECIESWIDGET_H