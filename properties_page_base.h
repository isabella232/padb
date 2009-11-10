#pragma once
#include <QWidget>
#include <QSqlDatabase>

namespace pa_db
{
	class properties_page_base : public QWidget
	{
		Q_OBJECT
	public:
		properties_page_base( int object_id, QWidget *parent );
		virtual QString page_name( ) const = 0;
		virtual void apply( ) = 0;

	Q_SIGNALS:
		void changed( );

	protected:
		const int			m_object_id;
		QSqlDatabase		m_database;
	};
}
