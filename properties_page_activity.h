#pragma once
#include "properties_page_obj_table.h"

namespace pa_db
{
	class properties_page_activity : public properties_page_obj_table
	{
		Q_OBJECT
	public:
		properties_page_activity( int object_id, QWidget *parent );
		virtual QString page_name( ) const;
		virtual void apply( );
	};
}
