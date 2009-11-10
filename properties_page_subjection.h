#pragma once
#include "properties_page_obj_table.h"
class QTextEdit;

namespace pa_db
{
	class properties_page_subjection : public properties_page_obj_table
	{
		Q_OBJECT
	public:
		properties_page_subjection( int object_id, QWidget *parent );
		virtual QString page_name( ) const;
		virtual void apply( );

	private:
		QTextEdit					*m_subst;
		QTextEdit					*m_address;
		QTextEdit					*m_substpa;

	};
}
