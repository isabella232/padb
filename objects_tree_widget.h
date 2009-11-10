#pragma once
#include <QTreeWidget>
class QTreeWidgetItem;
class QMimeData;
class QMouseEvent;

namespace pa_db
{
	class objects_tree_controller;
	class objects_tree_widget : public QTreeWidget
	{
		Q_OBJECT
	public:
		objects_tree_widget( QWidget *parent );
		void set_controller( objects_tree_controller *cntrl );

	protected:
		virtual Qt::DropActions supportedDropActions( ) const;
		virtual QStringList mimeTypes( ) const;
		virtual bool dropMimeData( QTreeWidgetItem *parent, int index, const QMimeData *data, Qt::DropAction action );
		void mouseMoveEvent( QMouseEvent *event );
		virtual void keyPressEvent( QKeyEvent *event );
		virtual void mouseDoubleClickEvent( QMouseEvent *event );

	private:
		objects_tree_controller *m_cntrl;
	};
}
