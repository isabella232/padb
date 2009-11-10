#pragma once
#include "properties_page_obj_table.h"

namespace pa_db
{
	class sql_relation_combo;
	class properties_page_common : public properties_page_obj_table
	{
		Q_OBJECT
	public:
		properties_page_common( int object_id, QWidget *parent );
		virtual QString page_name( ) const;
		virtual void apply( );

	private:
		sql_relation_combo			*m_category;
		sql_relation_combo			*m_profile;
		sql_relation_combo			*m_status;
	};
}
