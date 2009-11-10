#include "dock_widget.h"
#include <QVBoxLayout>
#include <QLabel>
#include "free_elements_widget.h"
#include <QSplitter>
#include "objects_tree_widget.h"
#include "resources.h"

pa_db::dock_widget::dock_widget( const QString& title, QWidget *parent )
	: QDockWidget( title, parent )
	, m_first_text( 0 )
	, m_second_text( 0 )
{
	QSplitter *sp = new QSplitter( Qt::Vertical, this );
	setWidget( sp );

	QWidget *up_part = new QWidget( sp );
	sp->addWidget( up_part );
	create_up_part( up_part );

	QWidget *bottom_part = new QWidget( sp );
	sp->addWidget( bottom_part );
	create_bottom_part( bottom_part );
}

void pa_db::dock_widget::create_up_part( QWidget *parent )
{
	QVBoxLayout *main_layout = new QVBoxLayout( parent );
	main_layout->setContentsMargins( 0, 0, 0, 0 );
	main_layout->addWidget( m_first_text = new QLabel( text::objects_list( ), parent ) );
	m_objects_tree = new objects_tree_widget( parent );
	main_layout->addWidget( m_objects_tree );
}

void pa_db::dock_widget::create_bottom_part( QWidget *parent )
{
	QVBoxLayout *main_layout = new QVBoxLayout( parent );
	main_layout->setContentsMargins( 0, 0, 0, 0 );
	main_layout->addWidget( m_second_text = new QLabel( text::free_elements_list( ), parent ) );
	main_layout->addWidget( m_free_elements_list = new free_elements_widget( parent ) );
}

pa_db::objects_tree_widget *pa_db::dock_widget::objects_tree( )
{
	return m_objects_tree;
}

pa_db::free_elements_widget *pa_db::dock_widget::free_elements_list( )
{
	return m_free_elements_list;
}

void pa_db::dock_widget::show_error_message( )
{
	objects_tree( )->hide( );
	free_elements_list( )->hide( );
	m_first_text->setText( text::error_open_db( ) );
	m_second_text->hide( );
}
