#pragma once
#include <QComboBox>
class QSqlDatabase;
class QString;

namespace pa_db
{
	class sql_relation_combo : public QComboBox
	{
		Q_OBJECT
		Q_PROPERTY(int m_id READ id WRITE set_id);
	public:
		sql_relation_combo( QWidget *parent, QSqlDatabase& database, const QString &table_name );
		int id( ) const;
		void set_id( int i );

	private:
		int m_id;
	};
}