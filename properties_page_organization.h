#pragma once
#include "properties_page_obj_table.h"
class QSpinBox;

namespace pa_db
{
	class properties_page_organization : public properties_page_obj_table
	{
		Q_OBJECT
	public:
		properties_page_organization( int object_id, QWidget *parent );
		virtual QString page_name( ) const;
		virtual void apply( );
	};
}
