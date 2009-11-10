#include "properties_page_obj_table.h"
#include "resources.h"
#include "utils.h"
#include <QDataWidgetMapper>
#include <QSqlTableModel>
#include <QDebug>
#include <QFormLayout>
#include <QTextEdit>
#include <QSpinBox>
#include <QLineEdit>

pa_db::properties_page_obj_table::properties_page_obj_table( int object_id, QWidget *parent )
	: properties_page_base( object_id, parent )
	, m_objects_table( 0 )
	, m_table_mapper( 0 )
{
	if ( m_database.isOpen( ) )
	{
		m_objects_table = new QSqlTableModel( this, m_database );
		m_objects_table->setTable( "objects" );

		m_objects_table->setEditStrategy( QSqlTableModel::OnManualSubmit );
		m_objects_table->setFilter( QString( "id=%1" ).arg( object_id ) );
		m_objects_table->select( );

		m_table_mapper = new QDataWidgetMapper( this );
		m_table_mapper->setModel( m_objects_table );
	}
	else
		qDebug( ) << "db not open";

	QHBoxLayout *main_layout = new QHBoxLayout( this );
	QVBoxLayout *v_layout = new QVBoxLayout;
	main_layout->addLayout( v_layout );
	main_layout->addStretch( 1 );

	m_form_layout = new QFormLayout;
	v_layout->addLayout( m_form_layout );
	v_layout->addStretch( );
}

QTextEdit* pa_db::properties_page_obj_table::create_text_exit( const QString &label, int field_index )
{
	QTextEdit *result = utils::create_text_exit( m_form_layout, label, m_table_mapper, field_index );
	connect( result, SIGNAL( textChanged( ) ), SIGNAL( changed( ) ) );
	return result;
}

QSpinBox* pa_db::properties_page_obj_table::create_spin_box( const QString &label, int field_index )
{
	QSpinBox *result = utils::create_spin_box( m_form_layout, label, m_table_mapper, field_index );
	connect( result, SIGNAL( valueChanged( const QString& ) ), SIGNAL( changed( ) ) );
	return result;
}

QLineEdit *pa_db::properties_page_obj_table::create_line_edit( const QString &label, int field_index, const QString &addon )
{
	QLineEdit *result = utils::create_line_edit( m_form_layout, label, m_table_mapper, field_index, addon );
	connect( result, SIGNAL( textChanged( const QString& ) ), SIGNAL( changed( ) ) );
	return result;
}
